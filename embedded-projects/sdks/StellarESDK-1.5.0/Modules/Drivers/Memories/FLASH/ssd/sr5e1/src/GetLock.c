/****************************************************************************
 *
 * Copyright (c) 2022 STMicroelectronics - All Rights Reserved
 *
 * License terms: STMicroelectronics Proprietary in accordance with licensing
 * terms SLA0098 at www.st.com.
 *
 * THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
 * INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *
 *****************************************************************************/

/*************************************************************************
*                                                                       *
*               Standard Software Driver for C55                        *
*                                                                       *
* FILE NAME     : GetLock.c                                             *
* FUNCTION      : GetLock                                               *
* DESCRIPTION   : This function will check the block locking status     *
*                   of Low/Middle/High/256K/UTest address spaces in the *
*                   C55 module.                                         *
* PARAMETERS    : pSSDConfig - Flash driver configuration structure     *
*                 blkLockIndicator - variable for storing block lock    *
*                   indicator value                                     *
*                 blkLockState - variable for storing block lock state  *
*                   value                                               *
* RETURN        : C55_OK - passes                                       *
*                 C55_ERROR_BLOCK_INDICATOR - wrong indicator value     *
*                 C55_ERROR_ALTERNATE - interface unsupported           *
* DATE          : February 04, 2022                                     *
* AUTHOR        : ...                                                   *
* E-mail        : ...                                                   *
*                                                                       *
*************************************************************************/

/******************************* CHANGES *********************************
  0.1.0   04.02.2022      G. Cuce       Initial Vesion
*************************************************************************/

#include "ssd_types.h"
#include "ssd_C55.h"



UINT32 GetLock ( PSSD_CONFIG pSSDConfig,
                 UINT8 blkLockIndicator,
                 UINT32 *blkLockState )
{
	volatile UINT32 returnCode;     /* return code */
	
    UINT32 value;                   /* value of block locking register */
    UINT32 mask;                    /* block locking bit mask for the address space */
    UINT32 shift;                   /* bit shift value for the block locking field of an address space */
    
    UINT32 lock0;                   /* lock0 register value */
    UINT32 lock1;                   /* lock1 register value */

    returnCode = C55_OK;

    /* Determine whether main or alternate interface is selected */
    if ( pSSDConfig->mainInterfaceFlag )
    {
        lock0 = C55_REG_READ (pSSDConfig->c55RegBase + C55_LOCK0);
        lock1 = C55_REG_READ (pSSDConfig->c55RegBase + C55_LOCK1);
    }
    else
    {
        lock0 = C55_REG_READ (pSSDConfig->c55RegBase + C55_LOCK0A);
        lock1 = C55_REG_READ (pSSDConfig->c55RegBase + C55_LOCK1A);
    }
    
    /* Determine which address space is selected */
    switch (blkLockIndicator)
    {
    case C55_BLOCK_LOW:
        value = lock0;
        shift = 16;
		mask = (0xFFFFFFFF >> (32 - C55_LOW_BLOCK_NUM(pSSDConfig) + 1 )) << 16;
        break;
        
    case C55_BLOCK_MID:
        value = lock0;
        shift = 0;
        mask = 0xFFFFFFFF >> (32 - C55_MID_BLOCK_NUM(pSSDConfig));
        break;
        
    case C55_BLOCK_HIGH:
        value = lock1;
        shift = 0;
        mask = 0xFFFFFFFF >> (32 - C55_HIGH_BLOCK_NUM(pSSDConfig));
        break;
        
    case C55_BLOCK_LARGE_FIRST:
        if ( pSSDConfig->mainInterfaceFlag )
        {
            value = C55_REG_READ (pSSDConfig->c55RegBase + C55_LOCK2);
        }
        else
        {
            returnCode = C55_ERROR_ALTERNATE;
            goto EXIT;
        }
        shift = 0;
        if ( pSSDConfig->n256KBlockNum >= 32 )
        {
            mask = 0xFFFFFFFF;
        }
        else
        {
            mask = 0xFFFFFFFF >> (32 - pSSDConfig->n256KBlockNum);
        }
        break;
        
    case C55_BLOCK_LARGE_SECOND:
        if ( pSSDConfig->mainInterfaceFlag )
        {
            value = C55_REG_READ (pSSDConfig->c55RegBase + C55_LOCK3);
        }
        else
        {
            returnCode = C55_ERROR_ALTERNATE;
            goto EXIT;
        }
        shift = 0;
        if ( pSSDConfig->n256KBlockNum > 32 )
        {
            mask = 0xFFFFFFFF >> (32 - (pSSDConfig->n256KBlockNum - 32));
        }
        else
        {
            mask = 0x0;
        }
        break;
        
    case C55_BLOCK_UTEST:
        value = lock0;
        shift = 31;
        mask = 0x80000000 /* 0x1 << 31 */;
        break;
        
    default:
        returnCode = C55_ERROR_BLOCK_INDICATOR;
        goto EXIT;
    }

    /* Mask off the other bits and make proper shift */
    *blkLockState = (value & mask) >> shift;

EXIT:
    if (pSSDConfig->BDMEnable)
    {
		/* save the return code to R3 */
		ASM_KEYWORD(" mov r3, %0 " ::"r"(returnCode));
		
		asm ( "nop" );
        asm ( "bkpt #0" );
        asm ( "nop" );

    }

    return returnCode;
}
