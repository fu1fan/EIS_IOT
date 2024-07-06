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
* FILE NAME     : FlashProgram.c                                        *
* FUNCTION      : FlashProgram                                          *
* DESCRIPTION   : This function will program user data to specified     *
*                   flash range..                                       *
*                   NOTE: The blks need to be unlocked first.           *
*                   We don't unlock blks in program function to be      *
*                   consistent with customer usage, since otherwise it  *
*                   violates the original intention of block protection.*
* PARAMETERS    : pSSDConfig - Flash driver configuration structure     *
*                 factoryPgmFlag - flag to do factory program or not    *
*                 dest - start flash addr to be programmed              *
*                 size - number of bytes to be programmed               *
*                   (need to be dword-aligned)                          *
*                 source - start RAM addr for source data               *
*                   (need to be word-aligned)                           *
* RETURN VALUES : C55_OK          - Program passes                      *
*                 C55_ERROR_ALTERNATE - Alternate interface does not    *
*                   support factory program                             *
*                 C55_ERROR_BUSY  - Other HV operation in progress      *
*                 C55_ERROR_ALIGNMENT - Invalid alignment               *
*                 C55_ERROR_FACTORY_OP  - Could not perform factory     *
*                   program                                             *
*                 C55_ERROR_ENABLE  - It's impossible to enable this    * 
*                   operation                                           *
* DATE          : February 04, 2022                                     *
* AUTHOR        : ...                                                   *
* E-mail        : ...                                                   *
*                                                                       *
*************************************************************************/

/******************************* CHANGES *********************************
  0.1.0   04.02.2019      G. Cuce       Initial version
*************************************************************************/

#include "ssd_types.h"
#include "ssd_C55.h"


UINT32 FlashProgram ( PSSD_CONFIG pSSDConfig,
                      BOOL factoryPgmFlag,
                      UINT32 dest,
                      UINT32 size,
                      UINT32 source,
                      PCONTEXT_DATA pCtxData )
{
    volatile UINT32 returnCode;     /* return code */
    volatile UINT32 programSize;
    volatile UINT32 temp;

    UINT32 sourceIndex;             /* source address index */
    UINT32 MCRAddress;              /* address of C55_MCR register */
    UINT32 MCRValue;                /* content of C55_MCR register */
    
    /* Initialize return code */
    returnCode = C55_OK;
    
    /* Alternate interface does not support factory program */
    if ( (!pSSDConfig->mainInterfaceFlag) && factoryPgmFlag )
    {
        returnCode = C55_ERROR_ALTERNATE;
        goto EXIT;
    }
    
    /* Check alignments */
    if ( ((dest % C55_DWORD_SIZE) != 0) ||
         (((source |size) % C55_WORD_SIZE) != 0) )
    {
        returnCode = C55_ERROR_ALIGNMENT;
        goto EXIT;
    }

    /* Address the MCR register to use */
    if ( pSSDConfig->mainInterfaceFlag )
    {
        MCRAddress = pSSDConfig->c55RegBase + C55_MCR;
    }
    else
    {
        MCRAddress = pSSDConfig->c55RegBase + C55_MCRA;
    }
    
    MCRValue = C55_REG_READ (MCRAddress);

    /* Cases that program operation can start:
       1. no program and erase sequence:
          (PGM low and ERS low)
       2. erase suspend with EHV low 
          (PGM low, ERS high, ESUS high, EHV low)

       Cases that program operation cannot start:
       1. program in progress (PGM high & EHV high)
       2. program in suspended state (PGM high & PSUS high)
       3. program not in progress (PGM low), but erase in progress but not in suspend state
    */

    if ( ( (MCRValue & C55_MCR_PGM) && (MCRValue & (C55_MCR_EHV | C55_MCR_PSUS)) ) ||
         ( (MCRValue & C55_MCR_ERS) && !(MCRValue & C55_MCR_ESUS) ) )
    {
        returnCode = C55_ERROR_BUSY;
        goto EXIT;
    }
    
    /* Anything to program? */
    if ( 0 == size )
    {
        goto EXIT;
    }

    /* Set MCR_PGM to start program operation */
    C55_MCR_BIT_SET (MCRAddress, C55_MCR_PGM);
    if (0x0 == C55_REG_BIT_TEST(MCRAddress, C55_MCR_PGM))
    {
        returnCode = C55_ERROR_ENABLE;
        goto EXIT;
    }
    
    /* If factory program is used, set bit MCR_FERS */
    if ( factoryPgmFlag )
    {
        C55_MCR_BIT_SET (MCRAddress, C55_MCR_FERS);
        
        /* Check for ability to do factory program by testing the bit */
        if ( !(C55_REG_BIT_TEST(MCRAddress, C55_MCR_FERS)) )
        {
            /* Clear MCR_PGM */
            C55_MCR_BIT_CLEAR (MCRAddress, C55_MCR_PGM);
            
            returnCode = C55_ERROR_FACTORY_OP;
            goto EXIT;
        }
    }

    /* Compute the buffer size used in the program cycle */
    if ( size > pSSDConfig->programmableSize )
    {
        programSize = pSSDConfig->programmableSize;
    }
    else
    {
        programSize = size;
    }

    /* Number of word-assign needs to be done */
    temp = programSize / C55_WORD_SIZE;
    
    /* Program data */
    for (sourceIndex = 0; sourceIndex < temp; sourceIndex++)
    {
        /* Programming write */
        *(VUINT32 *)dest = *(VUINT32 *)source;
        
        /* Update dest, size, source index */
        dest += C55_WORD_SIZE;
        source += C55_WORD_SIZE;
        size -= C55_WORD_SIZE;
        /* Is it time to do page programming?  */
        if( (dest % pSSDConfig->programmableSize) == 0 ) 
        {
            break;
        }
    }
    
    /* Update the context data */
    pCtxData->dest = dest;
    pCtxData->size = size;
    pCtxData->source = source;

    UINT32 i;
    for (i=0; i<0x100; i++) {
        asm volatile ( "nop" );
    }

    /* Set MCR_EHV bit, then return immediately */
    C55_MCR_BIT_SET (MCRAddress, C55_MCR_EHV);

EXIT:
    if (pSSDConfig->BDMEnable)
    {
        /* save the return code to R3 */
        ASM_KEYWORD(" mov r3, %0 " ::"r"(returnCode));
        
        asm volatile ( "nop" );
        asm volatile ( "bkpt #0" );
        asm volatile ( "nop" );

    }

    return returnCode;
}
