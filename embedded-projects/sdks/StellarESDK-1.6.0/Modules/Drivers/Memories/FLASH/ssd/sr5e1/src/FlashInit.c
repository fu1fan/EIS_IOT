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
*  FILE NAME     : FlashInit.c                                          *
*  FUNCTION      : FlashInit                                            *
*  DESCRIPTION   : This function will initialize the flash config       *
*                    structure.                                         *
*  PARAMETERS    : pSSDConfig - Flash driver configuration structure    *
*                                                                       *
*  RETURN        : C55_OK - passes                                      *
*  DATE          : February 04, 2022                                    *
*  AUTHOR        : ...                                                  *
*  E-mail        : ...                                                  *
*                                                                       *
*************************************************************************/

/******************************* CHANGES *********************************
  0.1.0   04.02.2022      G. Cuce       Initial version
*************************************************************************/

#include "ssd_types.h"
#include "ssd_C55.h"

#define C55_MCRE_N16KL_MASK    0x00000007
#define C55_MCRE_N32KL_MASK    0x00000018
#define C55_MCRE_N64KL_MASK    0x000000E0

#define C55_MCRE_N16KM_MASK    0x00000700
#define C55_MCRE_N32KM_MASK    0x00001800
#define C55_MCRE_N64KM_MASK    0x0000E000

#define C55_MCRE_N16KH_MASK    0x00070000
#define C55_MCRE_N32KH_MASK    0x00180000
#define C55_MCRE_N64KH_MASK    0x00E00000

#define C55_MCRE_N256K_MASK    0x1F000000

#define C55_MCRE_N128KL_MASK   0x60000000
#define C55_MCRE_N256KH_MASK   0x80000000



UINT32 FlashInit ( PSSD_CONFIG pSSDConfig )
{
    register UINT32 returnCode;   /* return code */

    UINT32 MCREValue;                      /* content of C55_MCRE register */

    /* Initialize blocks info for all address spaces */
    pSSDConfig->lowBlockInfo.n16KBlockNum  = 0; 
    pSSDConfig->lowBlockInfo.n32KBlockNum  = 0; 
    pSSDConfig->lowBlockInfo.n64KBlockNum  = 0; 
    pSSDConfig->lowBlockInfo.n128KBlockNum = 0; 

    pSSDConfig->midBlockInfo.n16KBlockNum  = 0; 
    pSSDConfig->midBlockInfo.n32KBlockNum  = 0; 
    pSSDConfig->midBlockInfo.n64KBlockNum  = 0; 
    pSSDConfig->midBlockInfo.n128KBlockNum = 0; 

    pSSDConfig->highBlockInfo.n16KBlockNum  = 0; 
    pSSDConfig->highBlockInfo.n32KBlockNum  = 0; 
    pSSDConfig->highBlockInfo.n64KBlockNum  = 0; 
    pSSDConfig->highBlockInfo.n128KBlockNum = 0; 

    returnCode = C55_OK;

    pSSDConfig->n256KBlockNum = 0;

    MCREValue = C55_REG_READ (pSSDConfig->c55RegBase + C55_MCRE);
    
    
    /* Clear error bits if main interface is selected */
    if ( pSSDConfig->mainInterfaceFlag )
    {
        /* Clear error bits MCR_EER, MCR_RWE, MCR_SBC */
        C55_MCR_EER_RWE_SBC_CLEAR (pSSDConfig->c55RegBase + C55_MCR);
        
        /* clear all bits in MCR */
        C55_REG_WRITE (pSSDConfig->c55RegBase + C55_MCR, 0);
        C55_REG_WRITE (pSSDConfig->c55RegBase + C55_MCR, 0);
    }
    else
    {
        /* clear all bits in MCRA */
        C55_REG_WRITE (pSSDConfig->c55RegBase + C55_MCRA, 0);
        C55_REG_WRITE (pSSDConfig->c55RegBase + C55_MCRA, 0);    
    }

    /* Determine the number of blocks for each block type in each space */
    /* Refer to the BlockGuide to determine to right method to calculate these values */

    if( (MCREValue & C55_MCRE_N16KL_MASK) == 0x5){
        pSSDConfig->lowBlockInfo.n16KBlockNum = 0x3;
    }else if ( (MCREValue & C55_MCRE_N16KL_MASK) == 0x6){
        pSSDConfig->lowBlockInfo.n16KBlockNum = 0x5;    
    }else{
        pSSDConfig->lowBlockInfo.n16KBlockNum = (MCREValue & C55_MCRE_N16KL_MASK) << 1;
    }


    if(((MCREValue & C55_MCRE_N32KL_MASK) >> 3) == 0x1){
        pSSDConfig->lowBlockInfo.n32KBlockNum = 0x2;
    }else if(((MCREValue & C55_MCRE_N32KL_MASK) >> 3) == 0x2){
        pSSDConfig->lowBlockInfo.n32KBlockNum = 0x1;
    }else{
        pSSDConfig->lowBlockInfo.n32KBlockNum = ((MCREValue & C55_MCRE_N32KL_MASK) >> 3);
    }


    if( ((MCREValue & C55_MCRE_N64KL_MASK) >> 5) == 0x5){
        pSSDConfig->lowBlockInfo.n64KBlockNum = 0x3;
    }else{
        pSSDConfig->lowBlockInfo.n64KBlockNum = ((MCREValue & C55_MCRE_N64KL_MASK) >> 5) << 1;
    }

    
    pSSDConfig->midBlockInfo.n16KBlockNum = ((MCREValue & C55_MCRE_N16KM_MASK) >> 8) << 1;
    pSSDConfig->midBlockInfo.n32KBlockNum = ((MCREValue & C55_MCRE_N32KM_MASK) >> 11) << 1;
    pSSDConfig->midBlockInfo.n64KBlockNum = ((MCREValue & C55_MCRE_N64KM_MASK) >> 13) << 1;


    pSSDConfig->highBlockInfo.n16KBlockNum = ((MCREValue & C55_MCRE_N16KH_MASK) >> 16) << 1;
    pSSDConfig->highBlockInfo.n32KBlockNum = ((MCREValue & C55_MCRE_N32KH_MASK) >> 19) << 1;
    pSSDConfig->highBlockInfo.n64KBlockNum = ((MCREValue & C55_MCRE_N64KH_MASK) >> 21) << 1;


    pSSDConfig->lowBlockInfo.n128KBlockNum = ((MCREValue & C55_MCRE_N128KL_MASK) >> 29);


    if( ((MCREValue & C55_MCRE_N256KH_MASK) >> 31) == 0x1){
        pSSDConfig->n256KBlockNum = 32 + ((MCREValue & C55_MCRE_N256K_MASK) >> 24);
    }else{
        pSSDConfig->n256KBlockNum = (MCREValue & C55_MCRE_N256K_MASK) >> 24;
    }

   
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

