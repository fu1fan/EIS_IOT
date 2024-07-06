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
* FILE NAME     : BlankCheck.c                                          *
* FUNCTION      : BlankCheck                                            *
* DESCRIPTION   : This function will check whether block has been       *
*                   erased                                              *
* PARAMETERS    : pSSDConfig - Flash driver configuration structure     *
*                 dest - start flash addr to be checked                 *
*                 size - number of bytes to be checked                  *
*                   (need to be word-aligned)                           *
*                 pFailedAddress - pointer to first failing address     *
*                 pFailedData - pointer to first failing actual data    *
* RETURN        : C55_OK           - passes                             *
*                 C55_ERROR_NOT_BLANK - Address is not erased           *
*                 C55_ERROR_ALIGNMENT - Invalid alignment               *
* DATE          : February 04, 2022                                     *
* AUTHOR        : ...                                                   *
* E-mail        : ...                                                   *
*                                                                       *
*************************************************************************/

/******************************* CHANGES *********************************
  0.1.0   04.02.2022      G. Cuce     Initial version
*************************************************************************/

#include "ssd_types.h"
#include "ssd_C55.h"
#include "ssd_C55_internal.h"


UINT32 BlankCheck ( PSSD_CONFIG pSSDConfig,
                    UINT32 dest,
                    UINT32 size,
                    UINT32 *pFailedAddress,
                    UINT32 *pFailedData,
                    PCONTEXT_DATA pCtxData )
{
    volatile UINT32 returnCode;   /* return code */
 
 
	UINT32 destIndex;                      /* destination address index */
    volatile UINT32 temp;

    returnCode = C55_OK;

    
    /* Check alignments */
    if ( ((dest | size) % C55_WORD_SIZE) != 0 )
    {
        returnCode = C55_ERROR_ALIGNMENT;
        goto EXIT;
    }


    /* Anything to check */
    if ( 0 == size )
    {
        goto EXIT;
    }
    
    /* Number of words needs to be checked for blank */
    temp = size / C55_WORD_SIZE;
    
    /* In each blank check cycle, we take up to a certain words */
    if ( temp >= NUM_WORDS_BLANK_CHECK_CYCLE )
    {
        temp = NUM_WORDS_BLANK_CHECK_CYCLE;
    }


    /* Word by word blank check */
    for ( destIndex = 0; destIndex < temp; destIndex++ )
    {
        if ( (*(VUINT32 *)dest) != 0xFFFFFFFF )
        {
            returnCode = C55_ERROR_NOT_BLANK;			
            *pFailedAddress = dest;
            *pFailedData = *(VUINT32 *)dest;
            goto EXIT;
        }

        dest += C55_WORD_SIZE;
    }
    
    /* Update the context data */
    pCtxData->dest = dest;
    pCtxData->size = size - temp * C55_WORD_SIZE;
    pCtxData->pFailedAddress = pFailedAddress;
    pCtxData->pFailedData = pFailedData;
	

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
