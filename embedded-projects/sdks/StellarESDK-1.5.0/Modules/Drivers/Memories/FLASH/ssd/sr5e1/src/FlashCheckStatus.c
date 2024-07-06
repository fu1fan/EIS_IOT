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
* FILE NAME     : FlashCheckStatus.c                                    *
* FUNCTION      : FlashCheckStatus                                      *
* DESCRIPTION   : This function will check the status of an on-going    *
*                   flash or UTest operation. If the current operation  *
*                   finishes, this function may call the relevant       *
*                   functions to finish the whole operation.            *
* PARAMETERS    : pSSDConfig - Flash driver configuration structure     *
*                 modeOp - the operation to check                       *
*                 opResult - variable to store result of the operation  *
*                 pCtxData - the context data                           *
*                 pReqCompletionFn - the function for completing        *
*                   the operation
* RETURN        : C55_OK - passes                                       *
* DATE          : February 04, 2022                                     *
* AUTHOR        : ...                                                   *
* E-mail        : ...                                                   *
*                                                                       *
*************************************************************************/

/******************************* CHANGES *********************************
  0.1.0   04.02.2022      G. Cuce       Initial version
***********************************************************************************************************/

#include "ssd_types.h"
#include "ssd_C55.h"
#include "ssd_C55_internal.h"


UINT32 FlashCheckStatus ( PSSD_CONFIG pSSDConfig,
                          UINT8 modeOp,
                          UINT32 *opResult,
                          PCONTEXT_DATA pCtxData
                          )
{	
	volatile UINT32 returnCode;     /* return code */
 
    UINT32 RegAddress;              /* address of MCR or UT0 register */
    UINT32 RegValue;                /* content of MCR or UT0 register */
    void* pCompletionFn;
    
    /* Initialize return code */
    returnCode = C55_DONE;
    *opResult = C55_OK;
    
    switch ( modeOp )
    {
    case C55_MODE_OP_PROGRAM:
        pCompletionFn = pCtxData->pReqCompletionFn;
        goto PROGRAM;
        
    case C55_MODE_OP_ERASE:
        /* Don't care about the completion function pointer */
        goto ERASE;
        
    case C55_MODE_OP_PROGRAM_VERIFY:
        pCompletionFn = pCtxData->pReqCompletionFn;
        goto PROGRAMVERIFY;
        
    case C55_MODE_OP_BLANK_CHECK:
        pCompletionFn = pCtxData->pReqCompletionFn;
        goto BLANKCHECK;
        
    case C55_MODE_OP_CHECK_SUM:
        pCompletionFn = pCtxData->pReqCompletionFn;
        goto CHECKSUM;
        
    case C55_MODE_OP_USER_TEST_CHECK:
    	/* Don't care about the completion function pointer */
        goto UTESTCHECK;
        
    default:
        returnCode = C55_ERROR_MODE_OP;
        goto EXIT;
    }

PROGRAM:
    /* Determine which MCR register should be used */
    if ( pSSDConfig->mainInterfaceFlag )
    {
        RegAddress = pSSDConfig->c55RegBase + C55_MCR;
    }
    else
    {
        RegAddress = pSSDConfig->c55RegBase + C55_MCRA;
    }
    
    RegValue = C55_REG_READ (RegAddress);
    
    /* There must be a program operation */
    if ( RegValue & C55_MCR_PGM )
    {
        /* If DONE bit goes high */
        if ( RegValue & C55_MCR_DONE )
        {
            /* If the operation is suspended */
            if ( RegValue & C55_MCR_PSUS )
            {
                /* Check if the suspended program is PGM_SUS or ERS_SUS_PGM_SUS */
                if ( RegValue & C55_MCR_ERS )
                {
                    returnCode = C55_ERS_SUS_PGM_SUS;
                }
                else
                {
                    returnCode = C55_PGM_SUS;
                }
            }
            else
            {
                /* Check PGOOD (PEG) */
                if ( RegValue & C55_MCR_PEG )
                {
                    /* Clear EHV if it is high */
                    if ( RegValue & C55_MCR_EHV )
                    {
                        C55_MCR_BIT_CLEAR (RegAddress, C55_MCR_EHV);
                        
                        /* If there is remaining data needs to be programmed */
                        if ( pCtxData->size )
                        {
                            returnCode = C55_INPROGRESS;

                            /* Call FlashProgram to continue the operation */
                            *opResult = ((PFLASHPROGRAM)pCompletionFn)( pSSDConfig,
                                                                        FALSE,
                                                                        pCtxData->dest,
                                                                        pCtxData->size,
                                                                        pCtxData->source,
                                                                        pCtxData );
                                                      
                            if ( *opResult != C55_OK )
                            {
                                returnCode = C55_DONE;
                            }
                        }
                        else /* pCtxData->size == 0 */
                        {
                            /* Program finished successfully, clear FERS, PGM */
                            C55_MCR_BIT_CLEAR (RegAddress, C55_MCR_FERS);
                            C55_MCR_BIT_CLEAR (RegAddress, C55_MCR_PGM);
                        }
                    }
                    else /* EHV == 0 */
                    {
                        if ( RegValue & C55_MCR_ERS )
                        {
                            returnCode = C55_ERS_SUS_PGM_WRITE;
                        }
                        else
                        {
                            returnCode = C55_PGM_WRITE;
                        }
                    }
                }
                else /* PEG == 0 */
                {
                    if ( RegValue & C55_MCR_EHV )
                    {
                        /* PGOOD (PEG) is low */
                        *opResult = C55_ERROR_PGOOD;
                        
                        /* Clear EHV */
                        C55_MCR_BIT_CLEAR (RegAddress, C55_MCR_EHV);
                        
                        /* Program finished unsuccessfully, clear FERS, PGM */
                        C55_MCR_BIT_CLEAR (RegAddress, C55_MCR_FERS);
                        C55_MCR_BIT_CLEAR (RegAddress, C55_MCR_PGM);
                    }
                    else /* EHV == 0 */
                    {
                        /* PEG is invalid at this stage */
                        if ( RegValue & C55_MCR_ERS )
                        {
                            returnCode = C55_ERS_SUS_PGM_WRITE;
                        }
                        else
                        {
                            returnCode = C55_PGM_WRITE;
                        }
                    }
                }
            }
        }
        else /* DONE == 0 */
        {
            /* DONE bit is still low means the operation is in-progress */
            returnCode = C55_INPROGRESS;
        }
    }
    goto EXIT;


ERASE:
    /* Determine which MCR register should be used */
    if ( pSSDConfig->mainInterfaceFlag )
    {
        RegAddress = pSSDConfig->c55RegBase + C55_MCR;
    }
    else
    {
        RegAddress = pSSDConfig->c55RegBase + C55_MCRA;
    }
    
    RegValue = C55_REG_READ (RegAddress);
    
    /* There must be an erase operation */
    if ( RegValue & C55_MCR_ERS )
    {
        /* If DONE bit goes high */
        if ( RegValue & C55_MCR_DONE )
        {
            /* If the operation is suspended */
            if ( RegValue & C55_MCR_ESUS )
            {
                returnCode = C55_ERS_SUS;
            }
            else
            {
                /* Check EGOOD (PEG) */
                if ( RegValue & C55_MCR_PEG )
                {
                    /* Clear EHV if it is high */
                    if ( RegValue & C55_MCR_EHV )
                    {
                        C55_MCR_BIT_CLEAR (RegAddress, C55_MCR_EHV);
                    }
                    else
                    {
                        returnCode = C55_ERS_WRITE;
                    }
                }
                else
                {
                    /* EGOOD (PEG) is low */
                    *opResult = C55_ERROR_EGOOD;
                    
                    /* Clear EHV if it is high */
                    if ( RegValue & C55_MCR_EHV )
                    {
                        C55_MCR_BIT_CLEAR (RegAddress, C55_MCR_EHV);
                    }
                    else
                    {
                        returnCode = C55_DONE;
                    }
                }
                
                if ( returnCode != C55_ERS_WRITE )
                {
                    /* Clear ERS */
                    C55_MCR_BIT_CLEAR (RegAddress, C55_MCR_FERS);
                    C55_MCR_BIT_CLEAR (RegAddress, C55_MCR_ERS);
                }
            }
        }
        else
        {
            /* DONE bit is still low means the operation is in-progress */
            returnCode = C55_INPROGRESS;
        }
    }
    goto EXIT;


PROGRAMVERIFY:
    /* If the whole operation is unfinished */
    if ( pCtxData->size )
    {
        returnCode = C55_INPROGRESS;
        /* Call ProgramVerify to continue the operation */
        *opResult = ((PPROGRAMVERIFY) pCompletionFn)( pSSDConfig,
                                                      pCtxData->dest,
                                                      pCtxData->size,
                                                      pCtxData->source,
                                                      pCtxData->pFailedAddress,
                                                      pCtxData->pFailedData,
                                                      pCtxData->pFailedSource,
                                                      pCtxData );
        
        if ( *opResult != C55_OK || 0 == pCtxData->size )
        {
            returnCode = C55_DONE;
        }
    }
    goto EXIT;


BLANKCHECK:
    /* If the whole operation is unfinished */
    if ( pCtxData->size )
    {
        returnCode = C55_INPROGRESS;
        /* Call BlankCheck to continue the operation */
        *opResult = ((PBLANKCHECK) pCompletionFn)( pSSDConfig,
                                                   pCtxData->dest,
                                                   pCtxData->size,
                                                   pCtxData->pFailedAddress,
                                                   pCtxData->pFailedData,
                                                   pCtxData );
        
        if ( *opResult != C55_OK || 0 == pCtxData->size )
        {
            returnCode = C55_DONE;
        }
    }
    goto EXIT;


CHECKSUM:


UTESTCHECK:


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
