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
* FILE NAME     : FlashErase.c                                          *
* FUNCTION      : FlashErase                                            *
* DESCRIPTION   : This function will erase selected blks.               *
*                   NOTE: The blks need to be unlocked first.           *
*                   We don't unlock blks in erase function to be        *
*                   consistent with customer usage, since otherwise it  *
*                   violates the original intention of block protection.*
* PARAMETERS    : pSSDConfig - Flash driver configuration structure     *
*                 eraseOption - erase option                            *
*                 lowEnabledBlocks - selected Low Address Space block(s)*
*                 midEnabledBlocks - selected Mid Address Space block(s)*
*                 highEnabledBlocks - selected High Address Space       *
*                   block(s)                                            *
*                 n256KEnabledBlocks - selected 256K Address Space      *
*                   block(s)                                            *
* RETURN VALUES : C55_OK          - Erase passes                        *
*                 C55_ERROR_BUSY  - Other HV operation in progress      *
*                 C55_ERROR_ERASE_OPTION  - Invalid erase option        *
*                 C55_ERROR_FACTORY_OP  - Could not perform factory     *
*                   erase                                               *
* DATE          : February 04, 2022                                     *
* AUTHOR        : ...                                                   *
* E-mail        : ...                                                   *
*                                                                       *
*************************************************************************/

/******************************* CHANGES *********************************
  0.1.0   04.02.2022      G. Cuce       Initial version
*************************************************************************/



#include "ssd_types.h"
#include "ssd_C55.h"


UINT32 FlashErase ( PSSD_CONFIG pSSDConfig,
                    UINT8 eraseOption,
                    UINT32 lowBlockSelect,
                    UINT32 midBlockSelect,
                    UINT32 highBlockSelect,
                    N256K_BLOCK_SEL n256KBlockSelect
                    )
{
    volatile UINT32 returnCode;     /* return code */

    UINT32 c55RegBase;              /* base address of C55 registers */
    UINT32 MCRAddress;              /* address of C55_MCR register */
    UINT32 MCRValue;                /* content of C55_MCR register */
    UINT32 interlockWriteAddress;   /* interlock write address */
   
   
    c55RegBase = pSSDConfig->c55RegBase;

    MCRAddress = c55RegBase + C55_MCR;
        
    MCRValue = C55_REG_READ (MCRAddress);
    
    /* Initialize returnCode */
    returnCode = C55_OK;
    
    /* Check for validity of erase option */
    switch ( eraseOption )
    {
    case C55_ERASE_MAIN:
    case C55_ERASE_MAIN_FERS:
        interlockWriteAddress = pSSDConfig->mainArrayBase;
        break;
        
    case C55_ERASE_UTEST:
    case C55_ERASE_UTEST_FERS:
        interlockWriteAddress = pSSDConfig->uTestArrayBase;
        /* Erase UTest requires no block to be selected */
        lowBlockSelect = 0;
        midBlockSelect = 0;
        highBlockSelect = 0;
        n256KBlockSelect.first256KBlockSelect = 0;
        n256KBlockSelect.second256KBlockSelect = 0;
        break;
    
    default:
        returnCode = C55_ERROR_ERASE_OPTION;
        goto EXIT;
    }

    /* Program and/or erase operation in progress */
    if ( ( MCRValue & C55_MCR_PGM ) ||
         ( (MCRValue & C55_MCR_ERS) && (MCRValue & (C55_MCR_EHV | C55_MCR_ESUS)) ) )
    {
        returnCode = C55_ERROR_BUSY;
        goto EXIT;
    }

    /* Correct the block selection for the case we erase Main array */
    if ( C55_ERASE_MAIN == eraseOption || C55_ERASE_MAIN_FERS == eraseOption )
    {
        /* Mask off reserved bits for low address space */
        lowBlockSelect &= 0xFFFFFFFF >> (32 - C55_LOW_BLOCK_NUM(pSSDConfig) - 1  );

        /* Mask off reserved bits for mid address space */
        midBlockSelect &= 0xFFFFFFFF >> (32 - C55_MID_BLOCK_NUM(pSSDConfig));

        /* Mask off reserved bits for high address space */
        highBlockSelect &= 0xFFFFFFFF >> (32 - C55_HIGH_BLOCK_NUM(pSSDConfig));
        
        /* Mask off reserved bits for 256K address space */
        if ( pSSDConfig->n256KBlockNum <= 32 )
        {
            n256KBlockSelect.first256KBlockSelect &=
                0xFFFFFFFF >> (32 - pSSDConfig->n256KBlockNum);
            n256KBlockSelect.second256KBlockSelect = 0;
        }
        else
        {
            n256KBlockSelect.first256KBlockSelect &= 0xFFFFFFFF;
            n256KBlockSelect.second256KBlockSelect &=
                0xFFFFFFFF >> (32 - (pSSDConfig->n256KBlockNum - 32));
        }
        
        /* No blocks to be selected for erase, just return */
        if ( !(lowBlockSelect | midBlockSelect | highBlockSelect |
                n256KBlockSelect.first256KBlockSelect |
                n256KBlockSelect.second256KBlockSelect) )
        {
            /* Return with C55_OK */
            goto EXIT;
        }
        
        /* Set the block selection registers */
        C55_REG_WRITE (c55RegBase + C55_SEL0, (lowBlockSelect << 16) | midBlockSelect);
        C55_REG_WRITE (c55RegBase + C55_SEL1, highBlockSelect);
        C55_REG_WRITE (c55RegBase + C55_SEL2, n256KBlockSelect.first256KBlockSelect);
        C55_REG_WRITE (c55RegBase + C55_SEL3, n256KBlockSelect.second256KBlockSelect);
    }

    /* Set MCR_ERS to start erase operation */
    C55_MCR_BIT_SET (MCRAddress, C55_MCR_ERS);
    if (0x0 == C55_REG_BIT_TEST(MCRAddress, C55_MCR_ERS))
    {
        returnCode = C55_ERROR_ENABLE;
        goto EXIT;
    }

    /* If factory erase is used, set bit MCR_FERS */
    if ( C55_ERASE_MAIN_FERS == eraseOption || C55_ERASE_UTEST_FERS == eraseOption )
    {
        C55_MCR_BIT_SET (MCRAddress, C55_MCR_FERS);
        
        /* Check for ability to do factory erase by testing the bit */
        if ( !(C55_REG_BIT_TEST(MCRAddress, C55_MCR_FERS) ))
        {
            /* Clear MCR_ERS */
            C55_MCR_BIT_CLEAR (MCRAddress, C55_MCR_ERS);
            
            returnCode = C55_ERROR_FACTORY_OP;
            goto EXIT;
        }
    }
    
    /* Interlock write */
    *( (VUINT32 *)interlockWriteAddress ) = 0xFFFFFFFF;

    UINT32 i;
    for (i=0; i<0x100; i++) {
        asm volatile ( "nop" );
    }

    /* Write a 1 to MCR_EHV */
    C55_MCR_BIT_SET (MCRAddress, C55_MCR_EHV);

    /* Return here, not wait until MCR_DONE is set */

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
