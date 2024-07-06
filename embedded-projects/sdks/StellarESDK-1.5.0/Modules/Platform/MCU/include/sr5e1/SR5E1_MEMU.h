/****************************************************************************
 *                                                                           
 * Copyright (c) 2023 STMicroelectronics - All Rights Reserved               
 *                                                                           
 * License terms: STMicroelectronics Proprietary in accordance with licensing
 * terms SLA0098 at www.st.com.                                              
 *                                                                           
 * THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
 * INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.           
 *                                                                           
 * EVALUATION ONLY - NOT FOR USE IN PRODUCTION                               
 *****************************************************************************/

#ifndef __SR5E1_MEMU2_H
#define __SR5E1_MEMU2_H

/******************************************************************************/
/*                                                                            */
/*                               MEMU2 (MEMU2)                                */
/*                                                                            */
/******************************************************************************/

typedef struct {   /* Offset =0x00000020 Next_offset = 0x00000070 */
  __IO uint32_t STS;                                  /*!< System RAM correctable error reporting table status register Address offset: 0x0000 */
  __IO uint32_t ADDR;                                 /*!< System RAM correctable error reporting table address register Address offset: 0x0004 */
} MEMU2_SYS_RAM_CERR_TypeDef;

typedef struct {   /* Offset =0x00000430 Next_offset = 0x00000438 */
  __IO uint32_t STS;                                  /*!< System RAM uncorrectable error reporting table status register Address offset: 0x0000 */
  __IO uint32_t ADDR;                                 /*!< System RAM uncorrectable error reporting table address register Address offset: 0x0004 */
} MEMU2_SYS_RAM_UNCERR_TypeDef;

typedef struct {   /* Offset =0x00000660 Next_offset = 0x00000690 */
  __IO uint32_t CTRL;                                 /*!< System RAM ECC fault descriptor control registers  Address offset: 0x0000 */
  __IO uint32_t START;                                /*!< System RAM ECC fault descriptor start address registers Address offset: 0x0004 */
  __IO uint32_t END;                                  /*!< System RAM ECC fault descriptor end address registers Address offset: 0x0008 */
} MEMU2_SYS_RAM_ECC_FD_TypeDef;

typedef struct {   /* Offset =0x00000b00 Next_offset = 0x00000b10 */
  __IO uint32_t STS;                                  /*!< Peripheral RAM correctable error reporting table status register Address offset: 0x0000 */
  __IO uint32_t ADDR;                                 /*!< Peripheral RAM correctable error reporting table address register Address offset: 0x0004 */
} MEMU2_PERIPH_RAM_CERR_TypeDef;

typedef struct {   /* Offset =0x00000f10 Next_offset = 0x00000f18 */
  __IO uint32_t STS;                                  /*!< Peripheral RAM uncorrectable error reporting table status register Address offset: 0x0000 */
  __IO uint32_t ADDR;                                 /*!< Peripheral RAM uncorrectable error reporting table address register Address offset: 0x0004 */
} MEMU2_PERIPH_RAM_UNCERR_TypeDef;

typedef struct {   /* Offset =0x00001140 Next_offset = 0x00001170 */
  __IO uint32_t CTRL;                                 /*!< Peripheral RAM ECC fault descriptor control registers Address offset: 0x0000 */
  __IO uint32_t START;                                /*!< Peripheral RAM ECC fault descriptor start address registers Address offset: 0x0004 */
  __IO uint32_t END;                                  /*!< Peripheral RAM ECC fault descriptor end address registers Address offset: 0x0008 */
} MEMU2_PERIPH_RAM_ECC_FD_TypeDef;

typedef struct {   /* Offset =0x00001600 Next_offset = 0x00001700 */
  __IO uint32_t STS;                                  /*!< NVM single-bit correctable error reporting table status register Address offset: 0x0000 */
  __IO uint32_t ADDR;                                 /*!< NVM single-bit correctable error reporting table address register Address offset: 0x0004 */
} MEMU2_NVM_SB_CERR_TypeDef;

typedef struct {   /* Offset =0x00001a10 Next_offset = 0x00001a18 */
  __IO uint32_t STS;                                  /*!< NVM double-bit correctable error reporting table status register Address offset: 0x0000 */
  __IO uint32_t ADDR;                                 /*!< System RAM correctable error reporting table address register Address offset: 0x0004 */
} MEMU2_NVM_DB_CERR_TypeDef;

typedef struct {   /* Offset =0x00001e20 Next_offset = 0x00001e28 */
  __IO uint32_t STS;                                  /*!< NVM uncorrectable error reporting table status register Address offset: 0x0000 */
  __IO uint32_t ADDR;                                 /*!< NVM uncorrectable error reporting table address register Address offset: 0x0004 */
} MEMU2_NVM_UNCERR_TypeDef;

typedef struct {   /* Offset =0x00002050 Next_offset = 0x00002080 */
  __IO uint32_t CTRL;                                 /*!< NVM ECC fault descriptor control registers         Address offset: 0x0000 */
  __IO uint32_t START;                                /*!< NVM ECC fault descriptor start address registers   Address offset: 0x0004 */
  __IO uint32_t END;                                  /*!< NVM ECC fault descriptor end address registers     Address offset: 0x0008 */
} MEMU2_NVM_ECC_FD_TypeDef;



typedef struct
{
  __IO uint32_t CTRL;                                 /*!< Control register                                   Address offset: 0x0000 */
  __IO uint32_t ERR_FLAG;                             /*!< Error Flag register                                Address offset: 0x0004 */
        uint8_t RESERVED0[4];                         /*!< (null)                                             Address offset: 0x0008-0x000B */
  __IO uint32_t SYS_RAM_OUT_TRIG_CTRL;                /*!< System RAM output trigger control register         Address offset: 0x000C */
  __IO uint32_t PERIPH_RAM_OUT_TRIG_CTRL;             /*!< System RAM output trigger control register         Address offset: 0x0010 */
  __IO uint32_t NVM_OUT_TRIG_CTRL;                    /*!< NVM output trigger control register                Address offset: 0x0014 */
        uint8_t RESERVED1[8];                         /*!< (null)                                             Address offset: 0x0018-0x001F */
        MEMU2_SYS_RAM_CERR_TypeDef SYS_RAM_CERR[10];  /* Address offset: 0x00000020-0x00000070 */
  __I  uint32_t SYS_RAM_CERR_TBL_FILL_STAT;           /*!< System RAM correctable error reporting table fill status register Address offset: 0x0070 */
        uint8_t RESERVED2[956];                       /*!< (null)                                             Address offset: 0x0074-0x042F */
        MEMU2_SYS_RAM_UNCERR_TypeDef SYS_RAM_UNCERR;  /* Address offset: 0x00000430-0x00000438 */
  __I  uint32_t SYS_RAM_UNCERR_TBL_FILL_STAT;         /*!< System RAM uncorrectable error reporting table fill status register Address offset: 0x0438 */
        uint8_t RESERVED3[468];                       /*!< (null)                                             Address offset: 0x043C-0x060F */
  __I  uint32_t PERIPH_RAM_CERR_TBL_FILL_STAT;        /*!< Peripheral RAM correctable error reporting table fill status register Address offset: 0x0610 */
        uint8_t RESERVED4[44];                        /*!< (null)                                             Address offset: 0x0614-0x063F */
  __IO uint32_t SYS_RAM_OFLW[2];                      /*!< System RAM concurrent overflow registers           Address offset: 0x0640-0x0644 */
        uint8_t RESERVED5[24];                        /*!< (null)                                             Address offset: 0x0648-0x065F */
        MEMU2_SYS_RAM_ECC_FD_TypeDef SYS_RAM_ECC_FD[4]; /* Address offset: 0x00000660-0x00000690 */
  __IO uint32_t SYS_RAM_T320;                         /*!< System RAM address extension registers             Address offset: 0x0690 */
  __IO uint32_t SYS_RAM_T321;                         /*!< System RAM address extension registers             Address offset: 0x0694 */
        uint8_t RESERVED6[1128];                      /*!< (null)                                             Address offset: 0x0698-0x0AFF */
        MEMU2_PERIPH_RAM_CERR_TypeDef PERIPH_RAM_CERR[2]; /* Address offset: 0x00000b00-0x00000b10 */
        uint8_t RESERVED7[1024];                      /*!< (null)                                             Address offset: 0x0B10-0x0F0F */
        MEMU2_PERIPH_RAM_UNCERR_TypeDef PERIPH_RAM_UNCERR; /* Address offset: 0x00000f10-0x00000f18 */
  __I  uint32_t PERIPH_RAM_UNCERR_TBL_FILL_STAT;      /*!< Peripheral RAM uncorrectable error reporting table fill status register Address offset: 0x0F18 */
        uint8_t RESERVED8[516];                       /*!< (null)                                             Address offset: 0x0F1C-0x111F */
  __IO uint32_t PERIPH_RAM_OFLW[2];                   /*!< Peripheral RAM concurrent overflow registers       Address offset: 0x1120-0x1124 */
        uint8_t RESERVED9[24];                        /*!< (null)                                             Address offset: 0x1128-0x113F */
        MEMU2_PERIPH_RAM_ECC_FD_TypeDef PERIPH_RAM_ECC_FD[4]; /* Address offset: 0x00001140-0x00001170 */
  __IO uint32_t PERIPH_RAM_T320;                      /*!< Peripheral RAM address extension registers         Address offset: 0x1170 */
  __IO uint32_t PERIPH_RAM_T321;                      /*!< Peripheral RAM address extension registers         Address offset: 0x1174 */
        uint8_t RESERVED10[1160];                     /*!< (null)                                             Address offset: 0x1178-0x15FF */
        MEMU2_NVM_SB_CERR_TypeDef NVM_SB_CERR[32];    /* Address offset: 0x00001600-0x00001700 */
  __I  uint32_t NVM_SB_CERR_TBL_FILL_STAT;            /*!< NVM single-bit correctable error reporting table fill status register Address offset: 0x1700 */
        uint8_t RESERVED11[780];                      /*!< (null)                                             Address offset: 0x1704-0x1A0F */
        MEMU2_NVM_DB_CERR_TypeDef NVM_DB_CERR;        /* Address offset: 0x00001a10-0x00001a18 */
  __I  uint32_t NVM_DB_CERR_TBL_FILL_STAT;            /*!< NVM double-bit correctable error reporting table fill status register Address offset: 0x1A18 */
        uint8_t RESERVED12[1028];                     /*!< (null)                                             Address offset: 0x1A1C-0x1E1F */
        MEMU2_NVM_UNCERR_TypeDef NVM_UNCERR;          /* Address offset: 0x00001e20-0x00001e28 */
  __I  uint32_t NVM_UNCERR_TBL_FILL_STAT;             /*!< NVM uncorrectable error reporting table fill status register Address offset: 0x1E28 */
        uint8_t RESERVED13[516];                      /*!< (null)                                             Address offset: 0x1E2C-0x202F */
  __IO uint32_t NVM_OFLW[2];                          /*!< NVM concurrent overflow registers                  Address offset: 0x2030-0x2034 */
        uint8_t RESERVED14[24];                       /*!< (null)                                             Address offset: 0x2038-0x204F */
        MEMU2_NVM_ECC_FD_TypeDef NVM_ECC_FD[4]; /* Address offset: 0x00002050-0x00002080 */
  __IO uint32_t NVM_T320;                             /*!< NVM address extension registers                    Address offset: 0x2080 */
} MEMU2_TypeDef;



/************  Bit definition for MEMU2_SYS_RAM_CERR_STS register  ************/
#define MEMU2_SYS_RAM_CERR_STS_BAD_BIT_Pos                                         (0U)
#define MEMU2_SYS_RAM_CERR_STS_BAD_BIT_Msk                                         (0xFFUL << MEMU2_SYS_RAM_CERR_STS_BAD_BIT_Pos) /*!< 0x000000FF */
#define MEMU2_SYS_RAM_CERR_STS_BAD_BIT                                             MEMU2_SYS_RAM_CERR_STS_BAD_BIT_Msk 
#define MEMU2_SYS_RAM_CERR_STS_MEM_ID_Pos                                          (23U)
#define MEMU2_SYS_RAM_CERR_STS_MEM_ID_Msk                                          (0xFFUL << MEMU2_SYS_RAM_CERR_STS_MEM_ID_Pos) /*!< 0x7F800000 */
#define MEMU2_SYS_RAM_CERR_STS_MEM_ID                                              MEMU2_SYS_RAM_CERR_STS_MEM_ID_Msk 
#define MEMU2_SYS_RAM_CERR_STS_ECC_OR_MBIST_Pos                                    (28U)
#define MEMU2_SYS_RAM_CERR_STS_ECC_OR_MBIST_Msk                                    (0x1UL << MEMU2_SYS_RAM_CERR_STS_ECC_OR_MBIST_Pos) /*!< 0x10000000 */
#define MEMU2_SYS_RAM_CERR_STS_ECC_OR_MBIST                                        MEMU2_SYS_RAM_CERR_STS_ECC_OR_MBIST_Msk 
#define MEMU2_SYS_RAM_CERR_STS_VLD_Pos                                             (31U)
#define MEMU2_SYS_RAM_CERR_STS_VLD_Msk                                             (0x1UL << MEMU2_SYS_RAM_CERR_STS_VLD_Pos) /*!< 0x80000000 */
#define MEMU2_SYS_RAM_CERR_STS_VLD                                                 MEMU2_SYS_RAM_CERR_STS_VLD_Msk 

/***********  Bit definition for MEMU2_SYS_RAM_CERR_ADDR register  ************/
#define MEMU2_SYS_RAM_CERR_ADDR_ERR_ADD_Pos                                        (0U)
#define MEMU2_SYS_RAM_CERR_ADDR_ERR_ADD_Msk                                        (0xFFFFFFFFUL << MEMU2_SYS_RAM_CERR_ADDR_ERR_ADD_Pos) /*!< 0xFFFFFFFF */
#define MEMU2_SYS_RAM_CERR_ADDR_ERR_ADD                                            MEMU2_SYS_RAM_CERR_ADDR_ERR_ADD_Msk 



/***********  Bit definition for MEMU2_SYS_RAM_UNCERR_STS register  ***********/
#define MEMU2_SYS_RAM_UNCERR_STS_VLD_Pos                                           (31U)
#define MEMU2_SYS_RAM_UNCERR_STS_VLD_Msk                                           (0x1UL << MEMU2_SYS_RAM_UNCERR_STS_VLD_Pos) /*!< 0x80000000 */
#define MEMU2_SYS_RAM_UNCERR_STS_VLD                                               MEMU2_SYS_RAM_UNCERR_STS_VLD_Msk 

/**********  Bit definition for MEMU2_SYS_RAM_UNCERR_ADDR register  ***********/
#define MEMU2_SYS_RAM_UNCERR_ADDR_ERR_ADD_Pos                                      (0U)
#define MEMU2_SYS_RAM_UNCERR_ADDR_ERR_ADD_Msk                                      (0xFFFFFFFFUL << MEMU2_SYS_RAM_UNCERR_ADDR_ERR_ADD_Pos) /*!< 0xFFFFFFFF */
#define MEMU2_SYS_RAM_UNCERR_ADDR_ERR_ADD                                          MEMU2_SYS_RAM_UNCERR_ADDR_ERR_ADD_Msk 



/**********  Bit definition for MEMU2_SYS_RAM_ECC_FD_CTRL register  ***********/
#define MEMU2_SYS_RAM_ECC_FD_CTRL_FCCU_TRG_Pos                                     (0U)
#define MEMU2_SYS_RAM_ECC_FD_CTRL_FCCU_TRG_Msk                                     (0xFUL << MEMU2_SYS_RAM_ECC_FD_CTRL_FCCU_TRG_Pos) /*!< 0x0000000F */
#define MEMU2_SYS_RAM_ECC_FD_CTRL_FCCU_TRG                                         MEMU2_SYS_RAM_ECC_FD_CTRL_FCCU_TRG_Msk 
#define MEMU2_SYS_RAM_ECC_FD_CTRL_T32_Pos                                          (15U)
#define MEMU2_SYS_RAM_ECC_FD_CTRL_T32_Msk                                          (0x1UL << MEMU2_SYS_RAM_ECC_FD_CTRL_T32_Pos) /*!< 0x00008000 */
#define MEMU2_SYS_RAM_ECC_FD_CTRL_T32                                              MEMU2_SYS_RAM_ECC_FD_CTRL_T32_Msk 
#define MEMU2_SYS_RAM_ECC_FD_CTRL_SBC_Pos                                          (16U)
#define MEMU2_SYS_RAM_ECC_FD_CTRL_SBC_Msk                                          (0x1UL << MEMU2_SYS_RAM_ECC_FD_CTRL_SBC_Pos) /*!< 0x00010000 */
#define MEMU2_SYS_RAM_ECC_FD_CTRL_SBC                                              MEMU2_SYS_RAM_ECC_FD_CTRL_SBC_Msk 
#define MEMU2_SYS_RAM_ECC_FD_CTRL_UC_Pos                                           (18U)
#define MEMU2_SYS_RAM_ECC_FD_CTRL_UC_Msk                                           (0x1UL << MEMU2_SYS_RAM_ECC_FD_CTRL_UC_Pos) /*!< 0x00040000 */
#define MEMU2_SYS_RAM_ECC_FD_CTRL_UC                                               MEMU2_SYS_RAM_ECC_FD_CTRL_UC_Msk 
#define MEMU2_SYS_RAM_ECC_FD_CTRL_EN_Pos                                           (31U)
#define MEMU2_SYS_RAM_ECC_FD_CTRL_EN_Msk                                           (0x1UL << MEMU2_SYS_RAM_ECC_FD_CTRL_EN_Pos) /*!< 0x80000000 */
#define MEMU2_SYS_RAM_ECC_FD_CTRL_EN                                               MEMU2_SYS_RAM_ECC_FD_CTRL_EN_Msk 

/**********  Bit definition for MEMU2_SYS_RAM_ECC_FD_START register  **********/
#define MEMU2_SYS_RAM_ECC_FD_START_START_ADR_Pos                                   (0U)
#define MEMU2_SYS_RAM_ECC_FD_START_START_ADR_Msk                                   (0xFFFFFFFFUL << MEMU2_SYS_RAM_ECC_FD_START_START_ADR_Pos) /*!< 0xFFFFFFFF */
#define MEMU2_SYS_RAM_ECC_FD_START_START_ADR                                       MEMU2_SYS_RAM_ECC_FD_START_START_ADR_Msk 

/***********  Bit definition for MEMU2_SYS_RAM_ECC_FD_END register  ***********/
#define MEMU2_SYS_RAM_ECC_FD_END_END_ADR_Pos                                       (0U)
#define MEMU2_SYS_RAM_ECC_FD_END_END_ADR_Msk                                       (0xFFFFFFFFUL << MEMU2_SYS_RAM_ECC_FD_END_END_ADR_Pos) /*!< 0xFFFFFFFF */
#define MEMU2_SYS_RAM_ECC_FD_END_END_ADR                                           MEMU2_SYS_RAM_ECC_FD_END_END_ADR_Msk 



/**********  Bit definition for MEMU2_PERIPH_RAM_CERR_STS register  ***********/
#define MEMU2_PERIPH_RAM_CERR_STS_BAD_BIT_Pos                                      (0U)
#define MEMU2_PERIPH_RAM_CERR_STS_BAD_BIT_Msk                                      (0xFFUL << MEMU2_PERIPH_RAM_CERR_STS_BAD_BIT_Pos) /*!< 0x000000FF */
#define MEMU2_PERIPH_RAM_CERR_STS_BAD_BIT                                          MEMU2_PERIPH_RAM_CERR_STS_BAD_BIT_Msk 
#define MEMU2_PERIPH_RAM_CERR_STS_ECC_OR_MBIST_Pos                                 (28U)
#define MEMU2_PERIPH_RAM_CERR_STS_ECC_OR_MBIST_Msk                                 (0x1UL << MEMU2_PERIPH_RAM_CERR_STS_ECC_OR_MBIST_Pos) /*!< 0x10000000 */
#define MEMU2_PERIPH_RAM_CERR_STS_ECC_OR_MBIST                                     MEMU2_PERIPH_RAM_CERR_STS_ECC_OR_MBIST_Msk 
#define MEMU2_PERIPH_RAM_CERR_STS_VLD_Pos                                          (31U)
#define MEMU2_PERIPH_RAM_CERR_STS_VLD_Msk                                          (0x1UL << MEMU2_PERIPH_RAM_CERR_STS_VLD_Pos) /*!< 0x80000000 */
#define MEMU2_PERIPH_RAM_CERR_STS_VLD                                              MEMU2_PERIPH_RAM_CERR_STS_VLD_Msk 

/**********  Bit definition for MEMU2_PERIPH_RAM_CERR_ADDR register  **********/
#define MEMU2_PERIPH_RAM_CERR_ADDR_ERR_ADD_Pos                                     (0U)
#define MEMU2_PERIPH_RAM_CERR_ADDR_ERR_ADD_Msk                                     (0xFFFFFFFFUL << MEMU2_PERIPH_RAM_CERR_ADDR_ERR_ADD_Pos) /*!< 0xFFFFFFFF */
#define MEMU2_PERIPH_RAM_CERR_ADDR_ERR_ADD                                         MEMU2_PERIPH_RAM_CERR_ADDR_ERR_ADD_Msk 



/*********  Bit definition for MEMU2_PERIPH_RAM_UNCERR_STS register  **********/
#define MEMU2_PERIPH_RAM_UNCERR_STS_VLD_Pos                                        (31U)
#define MEMU2_PERIPH_RAM_UNCERR_STS_VLD_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_UNCERR_STS_VLD_Pos) /*!< 0x80000000 */
#define MEMU2_PERIPH_RAM_UNCERR_STS_VLD                                            MEMU2_PERIPH_RAM_UNCERR_STS_VLD_Msk 

/*********  Bit definition for MEMU2_PERIPH_RAM_UNCERR_ADDR register  *********/
#define MEMU2_PERIPH_RAM_UNCERR_ADDR_ERR_ADD_Pos                                   (0U)
#define MEMU2_PERIPH_RAM_UNCERR_ADDR_ERR_ADD_Msk                                   (0xFFFFFFFFUL << MEMU2_PERIPH_RAM_UNCERR_ADDR_ERR_ADD_Pos) /*!< 0xFFFFFFFF */
#define MEMU2_PERIPH_RAM_UNCERR_ADDR_ERR_ADD                                       MEMU2_PERIPH_RAM_UNCERR_ADDR_ERR_ADD_Msk 



/*********  Bit definition for MEMU2_PERIPH_RAM_ECC_FD_CTRL register  *********/
#define MEMU2_PERIPH_RAM_ECC_FD_CTRL_FCCU_TRG_Pos                                  (0U)
#define MEMU2_PERIPH_RAM_ECC_FD_CTRL_FCCU_TRG_Msk                                  (0xFUL << MEMU2_PERIPH_RAM_ECC_FD_CTRL_FCCU_TRG_Pos) /*!< 0x0000000F */
#define MEMU2_PERIPH_RAM_ECC_FD_CTRL_FCCU_TRG                                      MEMU2_PERIPH_RAM_ECC_FD_CTRL_FCCU_TRG_Msk 
#define MEMU2_PERIPH_RAM_ECC_FD_CTRL_T32_Pos                                       (15U)
#define MEMU2_PERIPH_RAM_ECC_FD_CTRL_T32_Msk                                       (0x1UL << MEMU2_PERIPH_RAM_ECC_FD_CTRL_T32_Pos) /*!< 0x00008000 */
#define MEMU2_PERIPH_RAM_ECC_FD_CTRL_T32                                           MEMU2_PERIPH_RAM_ECC_FD_CTRL_T32_Msk 
#define MEMU2_PERIPH_RAM_ECC_FD_CTRL_SBC_Pos                                       (16U)
#define MEMU2_PERIPH_RAM_ECC_FD_CTRL_SBC_Msk                                       (0x1UL << MEMU2_PERIPH_RAM_ECC_FD_CTRL_SBC_Pos) /*!< 0x00010000 */
#define MEMU2_PERIPH_RAM_ECC_FD_CTRL_SBC                                           MEMU2_PERIPH_RAM_ECC_FD_CTRL_SBC_Msk 
#define MEMU2_PERIPH_RAM_ECC_FD_CTRL_UC_Pos                                        (18U)
#define MEMU2_PERIPH_RAM_ECC_FD_CTRL_UC_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_ECC_FD_CTRL_UC_Pos) /*!< 0x00040000 */
#define MEMU2_PERIPH_RAM_ECC_FD_CTRL_UC                                            MEMU2_PERIPH_RAM_ECC_FD_CTRL_UC_Msk 
#define MEMU2_PERIPH_RAM_ECC_FD_CTRL_EN_Pos                                        (31U)
#define MEMU2_PERIPH_RAM_ECC_FD_CTRL_EN_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_ECC_FD_CTRL_EN_Pos) /*!< 0x80000000 */
#define MEMU2_PERIPH_RAM_ECC_FD_CTRL_EN                                            MEMU2_PERIPH_RAM_ECC_FD_CTRL_EN_Msk 

/********  Bit definition for MEMU2_PERIPH_RAM_ECC_FD_START register  *********/
#define MEMU2_PERIPH_RAM_ECC_FD_START_START_ADR_Pos                                (0U)
#define MEMU2_PERIPH_RAM_ECC_FD_START_START_ADR_Msk                                (0xFFFFFFFFUL << MEMU2_PERIPH_RAM_ECC_FD_START_START_ADR_Pos) /*!< 0xFFFFFFFF */
#define MEMU2_PERIPH_RAM_ECC_FD_START_START_ADR                                    MEMU2_PERIPH_RAM_ECC_FD_START_START_ADR_Msk 

/*********  Bit definition for MEMU2_PERIPH_RAM_ECC_FD_END register  **********/
#define MEMU2_PERIPH_RAM_ECC_FD_END_END_ADR_Pos                                    (0U)
#define MEMU2_PERIPH_RAM_ECC_FD_END_END_ADR_Msk                                    (0xFFFFFFFFUL << MEMU2_PERIPH_RAM_ECC_FD_END_END_ADR_Pos) /*!< 0xFFFFFFFF */
#define MEMU2_PERIPH_RAM_ECC_FD_END_END_ADR                                        MEMU2_PERIPH_RAM_ECC_FD_END_END_ADR_Msk 



/************  Bit definition for MEMU2_NVM_SB_CERR_STS register  *************/
#define MEMU2_NVM_SB_CERR_STS_BAD_BIT_Pos                                          (0U)
#define MEMU2_NVM_SB_CERR_STS_BAD_BIT_Msk                                          (0x1FFFFUL << MEMU2_NVM_SB_CERR_STS_BAD_BIT_Pos) /*!< 0x0001FFFF */
#define MEMU2_NVM_SB_CERR_STS_BAD_BIT                                              MEMU2_NVM_SB_CERR_STS_BAD_BIT_Msk 
#define MEMU2_NVM_SB_CERR_STS_MEM_ID_Pos                                           (20U)
#define MEMU2_NVM_SB_CERR_STS_MEM_ID_Msk                                           (0xFFUL << MEMU2_NVM_SB_CERR_STS_MEM_ID_Pos) /*!< 0x0FF00000 */
#define MEMU2_NVM_SB_CERR_STS_MEM_ID                                               MEMU2_NVM_SB_CERR_STS_MEM_ID_Msk 
#define MEMU2_NVM_SB_CERR_STS_ECC_OR_MBIST_Pos                                     (28U)
#define MEMU2_NVM_SB_CERR_STS_ECC_OR_MBIST_Msk                                     (0x1UL << MEMU2_NVM_SB_CERR_STS_ECC_OR_MBIST_Pos) /*!< 0x10000000 */
#define MEMU2_NVM_SB_CERR_STS_ECC_OR_MBIST                                         MEMU2_NVM_SB_CERR_STS_ECC_OR_MBIST_Msk 
#define MEMU2_NVM_SB_CERR_STS_VLD_Pos                                              (31U)
#define MEMU2_NVM_SB_CERR_STS_VLD_Msk                                              (0x1UL << MEMU2_NVM_SB_CERR_STS_VLD_Pos) /*!< 0x80000000 */
#define MEMU2_NVM_SB_CERR_STS_VLD                                                  MEMU2_NVM_SB_CERR_STS_VLD_Msk 

/************  Bit definition for MEMU2_NVM_SB_CERR_ADDR register  ************/
#define MEMU2_NVM_SB_CERR_ADDR_ERR_ADD_Pos                                         (0U)
#define MEMU2_NVM_SB_CERR_ADDR_ERR_ADD_Msk                                         (0xFFFFFFFFUL << MEMU2_NVM_SB_CERR_ADDR_ERR_ADD_Pos) /*!< 0xFFFFFFFF */
#define MEMU2_NVM_SB_CERR_ADDR_ERR_ADD                                             MEMU2_NVM_SB_CERR_ADDR_ERR_ADD_Msk 



/************  Bit definition for MEMU2_NVM_DB_CERR_STS register  *************/
#define MEMU2_NVM_DB_CERR_STS_BAD_BIT_Pos                                          (0U)
#define MEMU2_NVM_DB_CERR_STS_BAD_BIT_Msk                                          (0x1FFFFUL << MEMU2_NVM_DB_CERR_STS_BAD_BIT_Pos) /*!< 0x0001FFFF */
#define MEMU2_NVM_DB_CERR_STS_BAD_BIT                                              MEMU2_NVM_DB_CERR_STS_BAD_BIT_Msk 
#define MEMU2_NVM_DB_CERR_STS_MEM_ID_Pos                                           (20U)
#define MEMU2_NVM_DB_CERR_STS_MEM_ID_Msk                                           (0xFFUL << MEMU2_NVM_DB_CERR_STS_MEM_ID_Pos) /*!< 0x0FF00000 */
#define MEMU2_NVM_DB_CERR_STS_MEM_ID                                               MEMU2_NVM_DB_CERR_STS_MEM_ID_Msk 
#define MEMU2_NVM_DB_CERR_STS_ECC_OR_MBIST_Pos                                     (28U)
#define MEMU2_NVM_DB_CERR_STS_ECC_OR_MBIST_Msk                                     (0x1UL << MEMU2_NVM_DB_CERR_STS_ECC_OR_MBIST_Pos) /*!< 0x10000000 */
#define MEMU2_NVM_DB_CERR_STS_ECC_OR_MBIST                                         MEMU2_NVM_DB_CERR_STS_ECC_OR_MBIST_Msk 
#define MEMU2_NVM_DB_CERR_STS_VLD_Pos                                              (31U)
#define MEMU2_NVM_DB_CERR_STS_VLD_Msk                                              (0x1UL << MEMU2_NVM_DB_CERR_STS_VLD_Pos) /*!< 0x80000000 */
#define MEMU2_NVM_DB_CERR_STS_VLD                                                  MEMU2_NVM_DB_CERR_STS_VLD_Msk 

/************  Bit definition for MEMU2_NVM_DB_CERR_ADDR register  ************/
#define MEMU2_NVM_DB_CERR_ADDR_ERR_ADD_Pos                                         (0U)
#define MEMU2_NVM_DB_CERR_ADDR_ERR_ADD_Msk                                         (0xFFFFFFFFUL << MEMU2_NVM_DB_CERR_ADDR_ERR_ADD_Pos) /*!< 0xFFFFFFFF */
#define MEMU2_NVM_DB_CERR_ADDR_ERR_ADD                                             MEMU2_NVM_DB_CERR_ADDR_ERR_ADD_Msk 



/*************  Bit definition for MEMU2_NVM_UNCERR_STS register  *************/
#define MEMU2_NVM_UNCERR_STS_VLD_Pos                                               (31U)
#define MEMU2_NVM_UNCERR_STS_VLD_Msk                                               (0x1UL << MEMU2_NVM_UNCERR_STS_VLD_Pos) /*!< 0x80000000 */
#define MEMU2_NVM_UNCERR_STS_VLD                                                   MEMU2_NVM_UNCERR_STS_VLD_Msk 

/************  Bit definition for MEMU2_NVM_UNCERR_ADDR register  *************/
#define MEMU2_NVM_UNCERR_ADDR_ERR_ADD_Pos                                          (0U)
#define MEMU2_NVM_UNCERR_ADDR_ERR_ADD_Msk                                          (0xFFFFFFFFUL << MEMU2_NVM_UNCERR_ADDR_ERR_ADD_Pos) /*!< 0xFFFFFFFF */
#define MEMU2_NVM_UNCERR_ADDR_ERR_ADD                                              MEMU2_NVM_UNCERR_ADDR_ERR_ADD_Msk 



/************  Bit definition for MEMU2_NVM_ECC_FD_CTRL register  *************/
#define MEMU2_NVM_ECC_FD_CTRL_FCCU_TRG_Pos                                         (0U)
#define MEMU2_NVM_ECC_FD_CTRL_FCCU_TRG_Msk                                         (0xFUL << MEMU2_NVM_ECC_FD_CTRL_FCCU_TRG_Pos) /*!< 0x0000000F */
#define MEMU2_NVM_ECC_FD_CTRL_FCCU_TRG                                             MEMU2_NVM_ECC_FD_CTRL_FCCU_TRG_Msk 
#define MEMU2_NVM_ECC_FD_CTRL_T32_Pos                                              (15U)
#define MEMU2_NVM_ECC_FD_CTRL_T32_Msk                                              (0x1UL << MEMU2_NVM_ECC_FD_CTRL_T32_Pos) /*!< 0x00008000 */
#define MEMU2_NVM_ECC_FD_CTRL_T32                                                  MEMU2_NVM_ECC_FD_CTRL_T32_Msk 
#define MEMU2_NVM_ECC_FD_CTRL_SBC_Pos                                              (16U)
#define MEMU2_NVM_ECC_FD_CTRL_SBC_Msk                                              (0x1UL << MEMU2_NVM_ECC_FD_CTRL_SBC_Pos) /*!< 0x00010000 */
#define MEMU2_NVM_ECC_FD_CTRL_SBC                                                  MEMU2_NVM_ECC_FD_CTRL_SBC_Msk 
#define MEMU2_NVM_ECC_FD_CTRL_DBC_Pos                                              (17U)
#define MEMU2_NVM_ECC_FD_CTRL_DBC_Msk                                              (0x1UL << MEMU2_NVM_ECC_FD_CTRL_DBC_Pos) /*!< 0x00020000 */
#define MEMU2_NVM_ECC_FD_CTRL_DBC                                                  MEMU2_NVM_ECC_FD_CTRL_DBC_Msk 
#define MEMU2_NVM_ECC_FD_CTRL_UC_Pos                                               (18U)
#define MEMU2_NVM_ECC_FD_CTRL_UC_Msk                                               (0x1UL << MEMU2_NVM_ECC_FD_CTRL_UC_Pos) /*!< 0x00040000 */
#define MEMU2_NVM_ECC_FD_CTRL_UC                                                   MEMU2_NVM_ECC_FD_CTRL_UC_Msk 
#define MEMU2_NVM_ECC_FD_CTRL_EN_Pos                                               (31U)
#define MEMU2_NVM_ECC_FD_CTRL_EN_Msk                                               (0x1UL << MEMU2_NVM_ECC_FD_CTRL_EN_Pos) /*!< 0x80000000 */
#define MEMU2_NVM_ECC_FD_CTRL_EN                                                   MEMU2_NVM_ECC_FD_CTRL_EN_Msk 

/************  Bit definition for MEMU2_NVM_ECC_FD_START register  ************/
#define MEMU2_NVM_ECC_FD_START_START_ADR_Pos                                       (0U)
#define MEMU2_NVM_ECC_FD_START_START_ADR_Msk                                       (0xFFFFFFFFUL << MEMU2_NVM_ECC_FD_START_START_ADR_Pos) /*!< 0xFFFFFFFF */
#define MEMU2_NVM_ECC_FD_START_START_ADR                                           MEMU2_NVM_ECC_FD_START_START_ADR_Msk 

/*************  Bit definition for MEMU2_NVM_ECC_FD_END register  *************/
#define MEMU2_NVM_ECC_FD_END_END_ADR_Pos                                           (0U)
#define MEMU2_NVM_ECC_FD_END_END_ADR_Msk                                           (0xFFFFFFFFUL << MEMU2_NVM_ECC_FD_END_END_ADR_Pos) /*!< 0xFFFFFFFF */
#define MEMU2_NVM_ECC_FD_END_END_ADR                                               MEMU2_NVM_ECC_FD_END_END_ADR_Msk 





/******************  Bit definition for MEMU2_CTRL register  ******************/

#define MEMU2_CTRL_SWR_Pos                                                   (15U)
#define MEMU2_CTRL_SWR_Msk                                                   (0x1UL << MEMU2_CTRL_SWR_Pos) /*!< 0x00008000 */
#define MEMU2_CTRL_SWR                                                       MEMU2_CTRL_SWR_Msk

/****************  Bit definition for MEMU2_ERR_FLAG register  ****************/

#define MEMU2_ERR_FLAG_SR_UCEO_Pos                                           (1U)
#define MEMU2_ERR_FLAG_SR_UCEO_Msk                                           (0x1UL << MEMU2_ERR_FLAG_SR_UCEO_Pos) /*!< 0x00000002 */
#define MEMU2_ERR_FLAG_SR_UCEO                                               MEMU2_ERR_FLAG_SR_UCEO_Msk
#define MEMU2_ERR_FLAG_SR_CEO_Pos                                            (2U)
#define MEMU2_ERR_FLAG_SR_CEO_Msk                                            (0x1UL << MEMU2_ERR_FLAG_SR_CEO_Pos) /*!< 0x00000004 */
#define MEMU2_ERR_FLAG_SR_CEO                                                MEMU2_ERR_FLAG_SR_CEO_Msk
#define MEMU2_ERR_FLAG_SR_UCE_Pos                                            (3U)
#define MEMU2_ERR_FLAG_SR_UCE_Msk                                            (0x1UL << MEMU2_ERR_FLAG_SR_UCE_Pos) /*!< 0x00000008 */
#define MEMU2_ERR_FLAG_SR_UCE                                                MEMU2_ERR_FLAG_SR_UCE_Msk
#define MEMU2_ERR_FLAG_SR_CE_Pos                                             (4U)
#define MEMU2_ERR_FLAG_SR_CE_Msk                                             (0x1UL << MEMU2_ERR_FLAG_SR_CE_Pos) /*!< 0x00000010 */
#define MEMU2_ERR_FLAG_SR_CE                                                 MEMU2_ERR_FLAG_SR_CE_Msk
#define MEMU2_ERR_FLAG_F_UCEO_Pos                                            (9U)
#define MEMU2_ERR_FLAG_F_UCEO_Msk                                            (0x1UL << MEMU2_ERR_FLAG_F_UCEO_Pos) /*!< 0x00000200 */
#define MEMU2_ERR_FLAG_F_UCEO                                                MEMU2_ERR_FLAG_F_UCEO_Msk
#define MEMU2_ERR_FLAG_F_DCEO_Pos                                            (10U)
#define MEMU2_ERR_FLAG_F_DCEO_Msk                                            (0x1UL << MEMU2_ERR_FLAG_F_DCEO_Pos) /*!< 0x00000400 */
#define MEMU2_ERR_FLAG_F_DCEO                                                MEMU2_ERR_FLAG_F_DCEO_Msk
#define MEMU2_ERR_FLAG_F_SCEO_Pos                                            (11U)
#define MEMU2_ERR_FLAG_F_SCEO_Msk                                            (0x1UL << MEMU2_ERR_FLAG_F_SCEO_Pos) /*!< 0x00000800 */
#define MEMU2_ERR_FLAG_F_SCEO                                                MEMU2_ERR_FLAG_F_SCEO_Msk
#define MEMU2_ERR_FLAG_F_UCE_Pos                                             (12U)
#define MEMU2_ERR_FLAG_F_UCE_Msk                                             (0x1UL << MEMU2_ERR_FLAG_F_UCE_Pos) /*!< 0x00001000 */
#define MEMU2_ERR_FLAG_F_UCE                                                 MEMU2_ERR_FLAG_F_UCE_Msk
#define MEMU2_ERR_FLAG_F_DCE_Pos                                             (13U)
#define MEMU2_ERR_FLAG_F_DCE_Msk                                             (0x1UL << MEMU2_ERR_FLAG_F_DCE_Pos) /*!< 0x00002000 */
#define MEMU2_ERR_FLAG_F_DCE                                                 MEMU2_ERR_FLAG_F_DCE_Msk
#define MEMU2_ERR_FLAG_F_SCE_Pos                                             (14U)
#define MEMU2_ERR_FLAG_F_SCE_Msk                                             (0x1UL << MEMU2_ERR_FLAG_F_SCE_Pos) /*!< 0x00004000 */
#define MEMU2_ERR_FLAG_F_SCE                                                 MEMU2_ERR_FLAG_F_SCE_Msk
#define MEMU2_ERR_FLAG_PR_UCEO_Pos                                           (17U)
#define MEMU2_ERR_FLAG_PR_UCEO_Msk                                           (0x1UL << MEMU2_ERR_FLAG_PR_UCEO_Pos) /*!< 0x00020000 */
#define MEMU2_ERR_FLAG_PR_UCEO                                               MEMU2_ERR_FLAG_PR_UCEO_Msk
#define MEMU2_ERR_FLAG_PR_CEO_Pos                                            (18U)
#define MEMU2_ERR_FLAG_PR_CEO_Msk                                            (0x1UL << MEMU2_ERR_FLAG_PR_CEO_Pos) /*!< 0x00040000 */
#define MEMU2_ERR_FLAG_PR_CEO                                                MEMU2_ERR_FLAG_PR_CEO_Msk
#define MEMU2_ERR_FLAG_PR_UCE_Pos                                            (19U)
#define MEMU2_ERR_FLAG_PR_UCE_Msk                                            (0x1UL << MEMU2_ERR_FLAG_PR_UCE_Pos) /*!< 0x00080000 */
#define MEMU2_ERR_FLAG_PR_UCE                                                MEMU2_ERR_FLAG_PR_UCE_Msk
#define MEMU2_ERR_FLAG_PR_CE_Pos                                             (20U)
#define MEMU2_ERR_FLAG_PR_CE_Msk                                             (0x1UL << MEMU2_ERR_FLAG_PR_CE_Pos) /*!< 0x00100000 */
#define MEMU2_ERR_FLAG_PR_CE                                                 MEMU2_ERR_FLAG_PR_CE_Msk

/*********  Bit definition for MEMU2_SYS_RAM_OUT_TRIG_CTRL register  **********/

#define MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_UCEO_Pos                           (1U)
#define MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_UCEO_Msk                           (0x1UL << MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_UCEO_Pos) /*!< 0x00000002 */
#define MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_UCEO                               MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_UCEO_Msk
#define MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_CEO_Pos                            (2U)
#define MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_CEO_Msk                            (0x1UL << MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_CEO_Pos) /*!< 0x00000004 */
#define MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_CEO                                MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_CEO_Msk
#define MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_FCCU_TRIG3_Pos                     (28U)
#define MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_FCCU_TRIG3_Msk                     (0x1UL << MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_FCCU_TRIG3_Pos) /*!< 0x10000000 */
#define MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_FCCU_TRIG3                         MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_FCCU_TRIG3_Msk
#define MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_FCCU_TRIG2_Pos                     (29U)
#define MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_FCCU_TRIG2_Msk                     (0x1UL << MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_FCCU_TRIG2_Pos) /*!< 0x20000000 */
#define MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_FCCU_TRIG2                         MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_FCCU_TRIG2_Msk
#define MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_FCCU_TRIG1_Pos                     (30U)
#define MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_FCCU_TRIG1_Msk                     (0x1UL << MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_FCCU_TRIG1_Pos) /*!< 0x40000000 */
#define MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_FCCU_TRIG1                         MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_FCCU_TRIG1_Msk
#define MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_FCCU_TRIG0_Pos                     (31U)
#define MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_FCCU_TRIG0_Msk                     (0x1UL << MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_FCCU_TRIG0_Pos) /*!< 0x80000000 */
#define MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_FCCU_TRIG0                         MEMU2_SYS_RAM_OUT_TRIG_CTRL_FR_SR_FCCU_TRIG0_Msk

/********  Bit definition for MEMU2_PERIPH_RAM_OUT_TRIG_CTRL register  ********/

#define MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_FCCU_TRIG0_Pos                  (0U)
#define MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_FCCU_TRIG0_Msk                  (0x1UL << MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_FCCU_TRIG0_Pos) /*!< 0x00000001 */
#define MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_FCCU_TRIG0                      MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_FCCU_TRIG0_Msk
#define MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_FCCU_TRIG1_Pos                  (1U)
#define MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_FCCU_TRIG1_Msk                  (0x1UL << MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_FCCU_TRIG1_Pos) /*!< 0x00000002 */
#define MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_FCCU_TRIG1                      MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_FCCU_TRIG1_Msk
#define MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_FCCU_TRIG2_Pos                  (2U)
#define MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_FCCU_TRIG2_Msk                  (0x1UL << MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_FCCU_TRIG2_Pos) /*!< 0x00000004 */
#define MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_FCCU_TRIG2                      MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_FCCU_TRIG2_Msk
#define MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_FCCU_TRIG3_Pos                  (3U)
#define MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_FCCU_TRIG3_Msk                  (0x1UL << MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_FCCU_TRIG3_Pos) /*!< 0x00000008 */
#define MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_FCCU_TRIG3                      MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_FCCU_TRIG3_Msk
#define MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_UCEO_Pos                        (17U)
#define MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_UCEO_Msk                        (0x1UL << MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_UCEO_Pos) /*!< 0x00020000 */
#define MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_UCEO                            MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_UCEO_Msk
#define MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_CEO_Pos                         (18U)
#define MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_CEO_Msk                         (0x1UL << MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_CEO_Pos) /*!< 0x00040000 */
#define MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_CEO                             MEMU2_PERIPH_RAM_OUT_TRIG_CTRL_FR_PR_CEO_Msk

/***********  Bit definition for MEMU2_NVM_OUT_TRIG_CTRL register  ************/

#define MEMU2_NVM_OUT_TRIG_CTRL_FR_F_DCEO_Pos                                (8U)
#define MEMU2_NVM_OUT_TRIG_CTRL_FR_F_DCEO_Msk                                (0x1UL << MEMU2_NVM_OUT_TRIG_CTRL_FR_F_DCEO_Pos) /*!< 0x00000100 */
#define MEMU2_NVM_OUT_TRIG_CTRL_FR_F_DCEO                                    MEMU2_NVM_OUT_TRIG_CTRL_FR_F_DCEO_Msk
#define MEMU2_NVM_OUT_TRIG_CTRL_FR_F_UCEO_Pos                                (9U)
#define MEMU2_NVM_OUT_TRIG_CTRL_FR_F_UCEO_Msk                                (0x1UL << MEMU2_NVM_OUT_TRIG_CTRL_FR_F_UCEO_Pos) /*!< 0x00000200 */
#define MEMU2_NVM_OUT_TRIG_CTRL_FR_F_UCEO                                    MEMU2_NVM_OUT_TRIG_CTRL_FR_F_UCEO_Msk
#define MEMU2_NVM_OUT_TRIG_CTRL_FR_F_SCEO_Pos                                (10U)
#define MEMU2_NVM_OUT_TRIG_CTRL_FR_F_SCEO_Msk                                (0x1UL << MEMU2_NVM_OUT_TRIG_CTRL_FR_F_SCEO_Pos) /*!< 0x00000400 */
#define MEMU2_NVM_OUT_TRIG_CTRL_FR_F_SCEO                                    MEMU2_NVM_OUT_TRIG_CTRL_FR_F_SCEO_Msk
#define MEMU2_NVM_OUT_TRIG_CTRL_FR_F_FCCU_TRIG3_Pos                          (28U)
#define MEMU2_NVM_OUT_TRIG_CTRL_FR_F_FCCU_TRIG3_Msk                          (0x1UL << MEMU2_NVM_OUT_TRIG_CTRL_FR_F_FCCU_TRIG3_Pos) /*!< 0x10000000 */
#define MEMU2_NVM_OUT_TRIG_CTRL_FR_F_FCCU_TRIG3                              MEMU2_NVM_OUT_TRIG_CTRL_FR_F_FCCU_TRIG3_Msk
#define MEMU2_NVM_OUT_TRIG_CTRL_FR_F_FCCU_TRIG2_Pos                          (29U)
#define MEMU2_NVM_OUT_TRIG_CTRL_FR_F_FCCU_TRIG2_Msk                          (0x1UL << MEMU2_NVM_OUT_TRIG_CTRL_FR_F_FCCU_TRIG2_Pos) /*!< 0x20000000 */
#define MEMU2_NVM_OUT_TRIG_CTRL_FR_F_FCCU_TRIG2                              MEMU2_NVM_OUT_TRIG_CTRL_FR_F_FCCU_TRIG2_Msk
#define MEMU2_NVM_OUT_TRIG_CTRL_FR_F_FCCU_TRIG1_Pos                          (30U)
#define MEMU2_NVM_OUT_TRIG_CTRL_FR_F_FCCU_TRIG1_Msk                          (0x1UL << MEMU2_NVM_OUT_TRIG_CTRL_FR_F_FCCU_TRIG1_Pos) /*!< 0x40000000 */
#define MEMU2_NVM_OUT_TRIG_CTRL_FR_F_FCCU_TRIG1                              MEMU2_NVM_OUT_TRIG_CTRL_FR_F_FCCU_TRIG1_Msk
#define MEMU2_NVM_OUT_TRIG_CTRL_FR_F_FCCU_TRIG0_Pos                          (31U)
#define MEMU2_NVM_OUT_TRIG_CTRL_FR_F_FCCU_TRIG0_Msk                          (0x1UL << MEMU2_NVM_OUT_TRIG_CTRL_FR_F_FCCU_TRIG0_Pos) /*!< 0x80000000 */
#define MEMU2_NVM_OUT_TRIG_CTRL_FR_F_FCCU_TRIG0                              MEMU2_NVM_OUT_TRIG_CTRL_FR_F_FCCU_TRIG0_Msk

/*******  Bit definition for MEMU2_SYS_RAM_CERR_TBL_FILL_STAT register  *******/

#define MEMU2_SYS_RAM_CERR_TBL_FILL_STAT_CERR_TBL_FILL_VAL_Pos               (0U)
#define MEMU2_SYS_RAM_CERR_TBL_FILL_STAT_CERR_TBL_FILL_VAL_Msk               (0xFFUL << MEMU2_SYS_RAM_CERR_TBL_FILL_STAT_CERR_TBL_FILL_VAL_Pos) /*!< 0x000000FF */
#define MEMU2_SYS_RAM_CERR_TBL_FILL_STAT_CERR_TBL_FILL_VAL                   MEMU2_SYS_RAM_CERR_TBL_FILL_STAT_CERR_TBL_FILL_VAL_Msk

/******  Bit definition for MEMU2_SYS_RAM_UNCERR_TBL_FILL_STAT register  ******/

#define MEMU2_SYS_RAM_UNCERR_TBL_FILL_STAT_UNCERR_TBL_FILL_VAL_Pos           (0U)
#define MEMU2_SYS_RAM_UNCERR_TBL_FILL_STAT_UNCERR_TBL_FILL_VAL_Msk           (0xFFUL << MEMU2_SYS_RAM_UNCERR_TBL_FILL_STAT_UNCERR_TBL_FILL_VAL_Pos) /*!< 0x000000FF */
#define MEMU2_SYS_RAM_UNCERR_TBL_FILL_STAT_UNCERR_TBL_FILL_VAL               MEMU2_SYS_RAM_UNCERR_TBL_FILL_STAT_UNCERR_TBL_FILL_VAL_Msk

/*****  Bit definition for MEMU2_PERIPH_RAM_CERR_TBL_FILL_STAT register  ******/

#define MEMU2_PERIPH_RAM_CERR_TBL_FILL_STAT_CERR_TBL_FILL_VAL_Pos            (0U)
#define MEMU2_PERIPH_RAM_CERR_TBL_FILL_STAT_CERR_TBL_FILL_VAL_Msk            (0xFFUL << MEMU2_PERIPH_RAM_CERR_TBL_FILL_STAT_CERR_TBL_FILL_VAL_Pos) /*!< 0x000000FF */
#define MEMU2_PERIPH_RAM_CERR_TBL_FILL_STAT_CERR_TBL_FILL_VAL                MEMU2_PERIPH_RAM_CERR_TBL_FILL_STAT_CERR_TBL_FILL_VAL_Msk

/**************  Bit definition for MEMU2_SYS_RAM_OFLW register  **************/

#define MEMU2_SYS_RAM_OFLW_OFLW_Pos                                          (0U)
#define MEMU2_SYS_RAM_OFLW_OFLW_Msk                                          (0xFFFFFFFFUL << MEMU2_SYS_RAM_OFLW_OFLW_Pos) /*!< 0xFFFFFFFF */
#define MEMU2_SYS_RAM_OFLW_OFLW                                              MEMU2_SYS_RAM_OFLW_OFLW_Msk

/**************  Bit definition for MEMU2_SYS_RAM_T320 register  **************/

#define MEMU2_SYS_RAM_T320_T0_Pos                                            (0U)
#define MEMU2_SYS_RAM_T320_T0_Msk                                            (0x1UL << MEMU2_SYS_RAM_T320_T0_Pos) /*!< 0x00000001 */
#define MEMU2_SYS_RAM_T320_T0                                                MEMU2_SYS_RAM_T320_T0_Msk
#define MEMU2_SYS_RAM_T320_T1_Pos                                            (1U)
#define MEMU2_SYS_RAM_T320_T1_Msk                                            (0x1UL << MEMU2_SYS_RAM_T320_T1_Pos) /*!< 0x00000002 */
#define MEMU2_SYS_RAM_T320_T1                                                MEMU2_SYS_RAM_T320_T1_Msk
#define MEMU2_SYS_RAM_T320_T2_Pos                                            (2U)
#define MEMU2_SYS_RAM_T320_T2_Msk                                            (0x1UL << MEMU2_SYS_RAM_T320_T2_Pos) /*!< 0x00000004 */
#define MEMU2_SYS_RAM_T320_T2                                                MEMU2_SYS_RAM_T320_T2_Msk
#define MEMU2_SYS_RAM_T320_T3_Pos                                            (3U)
#define MEMU2_SYS_RAM_T320_T3_Msk                                            (0x1UL << MEMU2_SYS_RAM_T320_T3_Pos) /*!< 0x00000008 */
#define MEMU2_SYS_RAM_T320_T3                                                MEMU2_SYS_RAM_T320_T3_Msk
#define MEMU2_SYS_RAM_T320_T4_Pos                                            (4U)
#define MEMU2_SYS_RAM_T320_T4_Msk                                            (0x1UL << MEMU2_SYS_RAM_T320_T4_Pos) /*!< 0x00000010 */
#define MEMU2_SYS_RAM_T320_T4                                                MEMU2_SYS_RAM_T320_T4_Msk
#define MEMU2_SYS_RAM_T320_T5_Pos                                            (5U)
#define MEMU2_SYS_RAM_T320_T5_Msk                                            (0x1UL << MEMU2_SYS_RAM_T320_T5_Pos) /*!< 0x00000020 */
#define MEMU2_SYS_RAM_T320_T5                                                MEMU2_SYS_RAM_T320_T5_Msk
#define MEMU2_SYS_RAM_T320_T6_Pos                                            (6U)
#define MEMU2_SYS_RAM_T320_T6_Msk                                            (0x1UL << MEMU2_SYS_RAM_T320_T6_Pos) /*!< 0x00000040 */
#define MEMU2_SYS_RAM_T320_T6                                                MEMU2_SYS_RAM_T320_T6_Msk
#define MEMU2_SYS_RAM_T320_T7_Pos                                            (7U)
#define MEMU2_SYS_RAM_T320_T7_Msk                                            (0x1UL << MEMU2_SYS_RAM_T320_T7_Pos) /*!< 0x00000080 */
#define MEMU2_SYS_RAM_T320_T7                                                MEMU2_SYS_RAM_T320_T7_Msk
#define MEMU2_SYS_RAM_T320_T8_Pos                                            (8U)
#define MEMU2_SYS_RAM_T320_T8_Msk                                            (0x1UL << MEMU2_SYS_RAM_T320_T8_Pos) /*!< 0x00000100 */
#define MEMU2_SYS_RAM_T320_T8                                                MEMU2_SYS_RAM_T320_T8_Msk
#define MEMU2_SYS_RAM_T320_T9_Pos                                            (9U)
#define MEMU2_SYS_RAM_T320_T9_Msk                                            (0x1UL << MEMU2_SYS_RAM_T320_T9_Pos) /*!< 0x00000200 */
#define MEMU2_SYS_RAM_T320_T9                                                MEMU2_SYS_RAM_T320_T9_Msk
#define MEMU2_SYS_RAM_T320_T10_Pos                                           (10U)
#define MEMU2_SYS_RAM_T320_T10_Msk                                           (0x1UL << MEMU2_SYS_RAM_T320_T10_Pos) /*!< 0x00000400 */
#define MEMU2_SYS_RAM_T320_T10                                               MEMU2_SYS_RAM_T320_T10_Msk
#define MEMU2_SYS_RAM_T320_T11_Pos                                           (11U)
#define MEMU2_SYS_RAM_T320_T11_Msk                                           (0x1UL << MEMU2_SYS_RAM_T320_T11_Pos) /*!< 0x00000800 */
#define MEMU2_SYS_RAM_T320_T11                                               MEMU2_SYS_RAM_T320_T11_Msk
#define MEMU2_SYS_RAM_T320_T12_Pos                                           (12U)
#define MEMU2_SYS_RAM_T320_T12_Msk                                           (0x1UL << MEMU2_SYS_RAM_T320_T12_Pos) /*!< 0x00001000 */
#define MEMU2_SYS_RAM_T320_T12                                               MEMU2_SYS_RAM_T320_T12_Msk
#define MEMU2_SYS_RAM_T320_T13_Pos                                           (13U)
#define MEMU2_SYS_RAM_T320_T13_Msk                                           (0x1UL << MEMU2_SYS_RAM_T320_T13_Pos) /*!< 0x00002000 */
#define MEMU2_SYS_RAM_T320_T13                                               MEMU2_SYS_RAM_T320_T13_Msk
#define MEMU2_SYS_RAM_T320_T14_Pos                                           (14U)
#define MEMU2_SYS_RAM_T320_T14_Msk                                           (0x1UL << MEMU2_SYS_RAM_T320_T14_Pos) /*!< 0x00004000 */
#define MEMU2_SYS_RAM_T320_T14                                               MEMU2_SYS_RAM_T320_T14_Msk
#define MEMU2_SYS_RAM_T320_T15_Pos                                           (15U)
#define MEMU2_SYS_RAM_T320_T15_Msk                                           (0x1UL << MEMU2_SYS_RAM_T320_T15_Pos) /*!< 0x00008000 */
#define MEMU2_SYS_RAM_T320_T15                                               MEMU2_SYS_RAM_T320_T15_Msk
#define MEMU2_SYS_RAM_T320_T16_Pos                                           (16U)
#define MEMU2_SYS_RAM_T320_T16_Msk                                           (0x1UL << MEMU2_SYS_RAM_T320_T16_Pos) /*!< 0x00010000 */
#define MEMU2_SYS_RAM_T320_T16                                               MEMU2_SYS_RAM_T320_T16_Msk
#define MEMU2_SYS_RAM_T320_T17_Pos                                           (17U)
#define MEMU2_SYS_RAM_T320_T17_Msk                                           (0x1UL << MEMU2_SYS_RAM_T320_T17_Pos) /*!< 0x00020000 */
#define MEMU2_SYS_RAM_T320_T17                                               MEMU2_SYS_RAM_T320_T17_Msk
#define MEMU2_SYS_RAM_T320_T18_Pos                                           (18U)
#define MEMU2_SYS_RAM_T320_T18_Msk                                           (0x1UL << MEMU2_SYS_RAM_T320_T18_Pos) /*!< 0x00040000 */
#define MEMU2_SYS_RAM_T320_T18                                               MEMU2_SYS_RAM_T320_T18_Msk
#define MEMU2_SYS_RAM_T320_T19_Pos                                           (19U)
#define MEMU2_SYS_RAM_T320_T19_Msk                                           (0x1UL << MEMU2_SYS_RAM_T320_T19_Pos) /*!< 0x00080000 */
#define MEMU2_SYS_RAM_T320_T19                                               MEMU2_SYS_RAM_T320_T19_Msk
#define MEMU2_SYS_RAM_T320_T20_Pos                                           (20U)
#define MEMU2_SYS_RAM_T320_T20_Msk                                           (0x1UL << MEMU2_SYS_RAM_T320_T20_Pos) /*!< 0x00100000 */
#define MEMU2_SYS_RAM_T320_T20                                               MEMU2_SYS_RAM_T320_T20_Msk
#define MEMU2_SYS_RAM_T320_T21_Pos                                           (21U)
#define MEMU2_SYS_RAM_T320_T21_Msk                                           (0x1UL << MEMU2_SYS_RAM_T320_T21_Pos) /*!< 0x00200000 */
#define MEMU2_SYS_RAM_T320_T21                                               MEMU2_SYS_RAM_T320_T21_Msk
#define MEMU2_SYS_RAM_T320_T22_Pos                                           (22U)
#define MEMU2_SYS_RAM_T320_T22_Msk                                           (0x1UL << MEMU2_SYS_RAM_T320_T22_Pos) /*!< 0x00400000 */
#define MEMU2_SYS_RAM_T320_T22                                               MEMU2_SYS_RAM_T320_T22_Msk
#define MEMU2_SYS_RAM_T320_T23_Pos                                           (23U)
#define MEMU2_SYS_RAM_T320_T23_Msk                                           (0x1UL << MEMU2_SYS_RAM_T320_T23_Pos) /*!< 0x00800000 */
#define MEMU2_SYS_RAM_T320_T23                                               MEMU2_SYS_RAM_T320_T23_Msk
#define MEMU2_SYS_RAM_T320_T24_Pos                                           (24U)
#define MEMU2_SYS_RAM_T320_T24_Msk                                           (0x1UL << MEMU2_SYS_RAM_T320_T24_Pos) /*!< 0x01000000 */
#define MEMU2_SYS_RAM_T320_T24                                               MEMU2_SYS_RAM_T320_T24_Msk
#define MEMU2_SYS_RAM_T320_T25_Pos                                           (25U)
#define MEMU2_SYS_RAM_T320_T25_Msk                                           (0x1UL << MEMU2_SYS_RAM_T320_T25_Pos) /*!< 0x02000000 */
#define MEMU2_SYS_RAM_T320_T25                                               MEMU2_SYS_RAM_T320_T25_Msk
#define MEMU2_SYS_RAM_T320_T26_Pos                                           (26U)
#define MEMU2_SYS_RAM_T320_T26_Msk                                           (0x1UL << MEMU2_SYS_RAM_T320_T26_Pos) /*!< 0x04000000 */
#define MEMU2_SYS_RAM_T320_T26                                               MEMU2_SYS_RAM_T320_T26_Msk
#define MEMU2_SYS_RAM_T320_T27_Pos                                           (27U)
#define MEMU2_SYS_RAM_T320_T27_Msk                                           (0x1UL << MEMU2_SYS_RAM_T320_T27_Pos) /*!< 0x08000000 */
#define MEMU2_SYS_RAM_T320_T27                                               MEMU2_SYS_RAM_T320_T27_Msk
#define MEMU2_SYS_RAM_T320_T28_Pos                                           (28U)
#define MEMU2_SYS_RAM_T320_T28_Msk                                           (0x1UL << MEMU2_SYS_RAM_T320_T28_Pos) /*!< 0x10000000 */
#define MEMU2_SYS_RAM_T320_T28                                               MEMU2_SYS_RAM_T320_T28_Msk
#define MEMU2_SYS_RAM_T320_T29_Pos                                           (29U)
#define MEMU2_SYS_RAM_T320_T29_Msk                                           (0x1UL << MEMU2_SYS_RAM_T320_T29_Pos) /*!< 0x20000000 */
#define MEMU2_SYS_RAM_T320_T29                                               MEMU2_SYS_RAM_T320_T29_Msk
#define MEMU2_SYS_RAM_T320_T30_Pos                                           (30U)
#define MEMU2_SYS_RAM_T320_T30_Msk                                           (0x1UL << MEMU2_SYS_RAM_T320_T30_Pos) /*!< 0x40000000 */
#define MEMU2_SYS_RAM_T320_T30                                               MEMU2_SYS_RAM_T320_T30_Msk
#define MEMU2_SYS_RAM_T320_T31_Pos                                           (31U)
#define MEMU2_SYS_RAM_T320_T31_Msk                                           (0x1UL << MEMU2_SYS_RAM_T320_T31_Pos) /*!< 0x80000000 */
#define MEMU2_SYS_RAM_T320_T31                                               MEMU2_SYS_RAM_T320_T31_Msk

/**************  Bit definition for MEMU2_SYS_RAM_T321 register  **************/

#define MEMU2_SYS_RAM_T321_T0_Pos                                            (0U)
#define MEMU2_SYS_RAM_T321_T0_Msk                                            (0x1UL << MEMU2_SYS_RAM_T321_T0_Pos) /*!< 0x00000001 */
#define MEMU2_SYS_RAM_T321_T0                                                MEMU2_SYS_RAM_T321_T0_Msk
#define MEMU2_SYS_RAM_T321_T1_Pos                                            (1U)
#define MEMU2_SYS_RAM_T321_T1_Msk                                            (0x1UL << MEMU2_SYS_RAM_T321_T1_Pos) /*!< 0x00000002 */
#define MEMU2_SYS_RAM_T321_T1                                                MEMU2_SYS_RAM_T321_T1_Msk
#define MEMU2_SYS_RAM_T321_T2_Pos                                            (2U)
#define MEMU2_SYS_RAM_T321_T2_Msk                                            (0x1UL << MEMU2_SYS_RAM_T321_T2_Pos) /*!< 0x00000004 */
#define MEMU2_SYS_RAM_T321_T2                                                MEMU2_SYS_RAM_T321_T2_Msk
#define MEMU2_SYS_RAM_T321_T3_Pos                                            (3U)
#define MEMU2_SYS_RAM_T321_T3_Msk                                            (0x1UL << MEMU2_SYS_RAM_T321_T3_Pos) /*!< 0x00000008 */
#define MEMU2_SYS_RAM_T321_T3                                                MEMU2_SYS_RAM_T321_T3_Msk
#define MEMU2_SYS_RAM_T321_T4_Pos                                            (4U)
#define MEMU2_SYS_RAM_T321_T4_Msk                                            (0x1UL << MEMU2_SYS_RAM_T321_T4_Pos) /*!< 0x00000010 */
#define MEMU2_SYS_RAM_T321_T4                                                MEMU2_SYS_RAM_T321_T4_Msk
#define MEMU2_SYS_RAM_T321_T5_Pos                                            (5U)
#define MEMU2_SYS_RAM_T321_T5_Msk                                            (0x1UL << MEMU2_SYS_RAM_T321_T5_Pos) /*!< 0x00000020 */
#define MEMU2_SYS_RAM_T321_T5                                                MEMU2_SYS_RAM_T321_T5_Msk
#define MEMU2_SYS_RAM_T321_T6_Pos                                            (6U)
#define MEMU2_SYS_RAM_T321_T6_Msk                                            (0x1UL << MEMU2_SYS_RAM_T321_T6_Pos) /*!< 0x00000040 */
#define MEMU2_SYS_RAM_T321_T6                                                MEMU2_SYS_RAM_T321_T6_Msk
#define MEMU2_SYS_RAM_T321_T7_Pos                                            (7U)
#define MEMU2_SYS_RAM_T321_T7_Msk                                            (0x1UL << MEMU2_SYS_RAM_T321_T7_Pos) /*!< 0x00000080 */
#define MEMU2_SYS_RAM_T321_T7                                                MEMU2_SYS_RAM_T321_T7_Msk
#define MEMU2_SYS_RAM_T321_T8_Pos                                            (8U)
#define MEMU2_SYS_RAM_T321_T8_Msk                                            (0x1UL << MEMU2_SYS_RAM_T321_T8_Pos) /*!< 0x00000100 */
#define MEMU2_SYS_RAM_T321_T8                                                MEMU2_SYS_RAM_T321_T8_Msk
#define MEMU2_SYS_RAM_T321_T9_Pos                                            (9U)
#define MEMU2_SYS_RAM_T321_T9_Msk                                            (0x1UL << MEMU2_SYS_RAM_T321_T9_Pos) /*!< 0x00000200 */
#define MEMU2_SYS_RAM_T321_T9                                                MEMU2_SYS_RAM_T321_T9_Msk
#define MEMU2_SYS_RAM_T321_T10_Pos                                           (10U)
#define MEMU2_SYS_RAM_T321_T10_Msk                                           (0x1UL << MEMU2_SYS_RAM_T321_T10_Pos) /*!< 0x00000400 */
#define MEMU2_SYS_RAM_T321_T10                                               MEMU2_SYS_RAM_T321_T10_Msk
#define MEMU2_SYS_RAM_T321_T11_Pos                                           (11U)
#define MEMU2_SYS_RAM_T321_T11_Msk                                           (0x1UL << MEMU2_SYS_RAM_T321_T11_Pos) /*!< 0x00000800 */
#define MEMU2_SYS_RAM_T321_T11                                               MEMU2_SYS_RAM_T321_T11_Msk
#define MEMU2_SYS_RAM_T321_T12_Pos                                           (12U)
#define MEMU2_SYS_RAM_T321_T12_Msk                                           (0x1UL << MEMU2_SYS_RAM_T321_T12_Pos) /*!< 0x00001000 */
#define MEMU2_SYS_RAM_T321_T12                                               MEMU2_SYS_RAM_T321_T12_Msk
#define MEMU2_SYS_RAM_T321_T13_Pos                                           (13U)
#define MEMU2_SYS_RAM_T321_T13_Msk                                           (0x1UL << MEMU2_SYS_RAM_T321_T13_Pos) /*!< 0x00002000 */
#define MEMU2_SYS_RAM_T321_T13                                               MEMU2_SYS_RAM_T321_T13_Msk
#define MEMU2_SYS_RAM_T321_T14_Pos                                           (14U)
#define MEMU2_SYS_RAM_T321_T14_Msk                                           (0x1UL << MEMU2_SYS_RAM_T321_T14_Pos) /*!< 0x00004000 */
#define MEMU2_SYS_RAM_T321_T14                                               MEMU2_SYS_RAM_T321_T14_Msk
#define MEMU2_SYS_RAM_T321_T15_Pos                                           (15U)
#define MEMU2_SYS_RAM_T321_T15_Msk                                           (0x1UL << MEMU2_SYS_RAM_T321_T15_Pos) /*!< 0x00008000 */
#define MEMU2_SYS_RAM_T321_T15                                               MEMU2_SYS_RAM_T321_T15_Msk
#define MEMU2_SYS_RAM_T321_T16_Pos                                           (16U)
#define MEMU2_SYS_RAM_T321_T16_Msk                                           (0x1UL << MEMU2_SYS_RAM_T321_T16_Pos) /*!< 0x00010000 */
#define MEMU2_SYS_RAM_T321_T16                                               MEMU2_SYS_RAM_T321_T16_Msk
#define MEMU2_SYS_RAM_T321_T17_Pos                                           (17U)
#define MEMU2_SYS_RAM_T321_T17_Msk                                           (0x1UL << MEMU2_SYS_RAM_T321_T17_Pos) /*!< 0x00020000 */
#define MEMU2_SYS_RAM_T321_T17                                               MEMU2_SYS_RAM_T321_T17_Msk
#define MEMU2_SYS_RAM_T321_T18_Pos                                           (18U)
#define MEMU2_SYS_RAM_T321_T18_Msk                                           (0x1UL << MEMU2_SYS_RAM_T321_T18_Pos) /*!< 0x00040000 */
#define MEMU2_SYS_RAM_T321_T18                                               MEMU2_SYS_RAM_T321_T18_Msk
#define MEMU2_SYS_RAM_T321_T19_Pos                                           (19U)
#define MEMU2_SYS_RAM_T321_T19_Msk                                           (0x1UL << MEMU2_SYS_RAM_T321_T19_Pos) /*!< 0x00080000 */
#define MEMU2_SYS_RAM_T321_T19                                               MEMU2_SYS_RAM_T321_T19_Msk
#define MEMU2_SYS_RAM_T321_T20_Pos                                           (20U)
#define MEMU2_SYS_RAM_T321_T20_Msk                                           (0x1UL << MEMU2_SYS_RAM_T321_T20_Pos) /*!< 0x00100000 */
#define MEMU2_SYS_RAM_T321_T20                                               MEMU2_SYS_RAM_T321_T20_Msk
#define MEMU2_SYS_RAM_T321_T21_Pos                                           (21U)
#define MEMU2_SYS_RAM_T321_T21_Msk                                           (0x1UL << MEMU2_SYS_RAM_T321_T21_Pos) /*!< 0x00200000 */
#define MEMU2_SYS_RAM_T321_T21                                               MEMU2_SYS_RAM_T321_T21_Msk
#define MEMU2_SYS_RAM_T321_T22_Pos                                           (22U)
#define MEMU2_SYS_RAM_T321_T22_Msk                                           (0x1UL << MEMU2_SYS_RAM_T321_T22_Pos) /*!< 0x00400000 */
#define MEMU2_SYS_RAM_T321_T22                                               MEMU2_SYS_RAM_T321_T22_Msk
#define MEMU2_SYS_RAM_T321_T23_Pos                                           (23U)
#define MEMU2_SYS_RAM_T321_T23_Msk                                           (0x1UL << MEMU2_SYS_RAM_T321_T23_Pos) /*!< 0x00800000 */
#define MEMU2_SYS_RAM_T321_T23                                               MEMU2_SYS_RAM_T321_T23_Msk
#define MEMU2_SYS_RAM_T321_T24_Pos                                           (24U)
#define MEMU2_SYS_RAM_T321_T24_Msk                                           (0x1UL << MEMU2_SYS_RAM_T321_T24_Pos) /*!< 0x01000000 */
#define MEMU2_SYS_RAM_T321_T24                                               MEMU2_SYS_RAM_T321_T24_Msk
#define MEMU2_SYS_RAM_T321_T25_Pos                                           (25U)
#define MEMU2_SYS_RAM_T321_T25_Msk                                           (0x1UL << MEMU2_SYS_RAM_T321_T25_Pos) /*!< 0x02000000 */
#define MEMU2_SYS_RAM_T321_T25                                               MEMU2_SYS_RAM_T321_T25_Msk
#define MEMU2_SYS_RAM_T321_T26_Pos                                           (26U)
#define MEMU2_SYS_RAM_T321_T26_Msk                                           (0x1UL << MEMU2_SYS_RAM_T321_T26_Pos) /*!< 0x04000000 */
#define MEMU2_SYS_RAM_T321_T26                                               MEMU2_SYS_RAM_T321_T26_Msk
#define MEMU2_SYS_RAM_T321_T27_Pos                                           (27U)
#define MEMU2_SYS_RAM_T321_T27_Msk                                           (0x1UL << MEMU2_SYS_RAM_T321_T27_Pos) /*!< 0x08000000 */
#define MEMU2_SYS_RAM_T321_T27                                               MEMU2_SYS_RAM_T321_T27_Msk
#define MEMU2_SYS_RAM_T321_T28_Pos                                           (28U)
#define MEMU2_SYS_RAM_T321_T28_Msk                                           (0x1UL << MEMU2_SYS_RAM_T321_T28_Pos) /*!< 0x10000000 */
#define MEMU2_SYS_RAM_T321_T28                                               MEMU2_SYS_RAM_T321_T28_Msk
#define MEMU2_SYS_RAM_T321_T29_Pos                                           (29U)
#define MEMU2_SYS_RAM_T321_T29_Msk                                           (0x1UL << MEMU2_SYS_RAM_T321_T29_Pos) /*!< 0x20000000 */
#define MEMU2_SYS_RAM_T321_T29                                               MEMU2_SYS_RAM_T321_T29_Msk
#define MEMU2_SYS_RAM_T321_T30_Pos                                           (30U)
#define MEMU2_SYS_RAM_T321_T30_Msk                                           (0x1UL << MEMU2_SYS_RAM_T321_T30_Pos) /*!< 0x40000000 */
#define MEMU2_SYS_RAM_T321_T30                                               MEMU2_SYS_RAM_T321_T30_Msk
#define MEMU2_SYS_RAM_T321_T31_Pos                                           (31U)
#define MEMU2_SYS_RAM_T321_T31_Msk                                           (0x1UL << MEMU2_SYS_RAM_T321_T31_Pos) /*!< 0x80000000 */
#define MEMU2_SYS_RAM_T321_T31                                               MEMU2_SYS_RAM_T321_T31_Msk

/****  Bit definition for MEMU2_PERIPH_RAM_UNCERR_TBL_FILL_STAT register  *****/

#define MEMU2_PERIPH_RAM_UNCERR_TBL_FILL_STAT_UNCERR_TBL_FILL_VAL_Pos        (0U)
#define MEMU2_PERIPH_RAM_UNCERR_TBL_FILL_STAT_UNCERR_TBL_FILL_VAL_Msk        (0xFFUL << MEMU2_PERIPH_RAM_UNCERR_TBL_FILL_STAT_UNCERR_TBL_FILL_VAL_Pos) /*!< 0x000000FF */
#define MEMU2_PERIPH_RAM_UNCERR_TBL_FILL_STAT_UNCERR_TBL_FILL_VAL            MEMU2_PERIPH_RAM_UNCERR_TBL_FILL_STAT_UNCERR_TBL_FILL_VAL_Msk

/************  Bit definition for MEMU2_PERIPH_RAM_OFLW register  *************/

#define MEMU2_PERIPH_RAM_OFLW_OFLW_Pos                                       (0U)
#define MEMU2_PERIPH_RAM_OFLW_OFLW_Msk                                       (0xFFFFFFFFUL << MEMU2_PERIPH_RAM_OFLW_OFLW_Pos) /*!< 0xFFFFFFFF */
#define MEMU2_PERIPH_RAM_OFLW_OFLW                                           MEMU2_PERIPH_RAM_OFLW_OFLW_Msk

/************  Bit definition for MEMU2_PERIPH_RAM_T320 register  *************/

#define MEMU2_PERIPH_RAM_T320_T0_Pos                                         (0U)
#define MEMU2_PERIPH_RAM_T320_T0_Msk                                         (0x1UL << MEMU2_PERIPH_RAM_T320_T0_Pos) /*!< 0x00000001 */
#define MEMU2_PERIPH_RAM_T320_T0                                             MEMU2_PERIPH_RAM_T320_T0_Msk
#define MEMU2_PERIPH_RAM_T320_T1_Pos                                         (1U)
#define MEMU2_PERIPH_RAM_T320_T1_Msk                                         (0x1UL << MEMU2_PERIPH_RAM_T320_T1_Pos) /*!< 0x00000002 */
#define MEMU2_PERIPH_RAM_T320_T1                                             MEMU2_PERIPH_RAM_T320_T1_Msk
#define MEMU2_PERIPH_RAM_T320_T2_Pos                                         (2U)
#define MEMU2_PERIPH_RAM_T320_T2_Msk                                         (0x1UL << MEMU2_PERIPH_RAM_T320_T2_Pos) /*!< 0x00000004 */
#define MEMU2_PERIPH_RAM_T320_T2                                             MEMU2_PERIPH_RAM_T320_T2_Msk
#define MEMU2_PERIPH_RAM_T320_T3_Pos                                         (3U)
#define MEMU2_PERIPH_RAM_T320_T3_Msk                                         (0x1UL << MEMU2_PERIPH_RAM_T320_T3_Pos) /*!< 0x00000008 */
#define MEMU2_PERIPH_RAM_T320_T3                                             MEMU2_PERIPH_RAM_T320_T3_Msk
#define MEMU2_PERIPH_RAM_T320_T4_Pos                                         (4U)
#define MEMU2_PERIPH_RAM_T320_T4_Msk                                         (0x1UL << MEMU2_PERIPH_RAM_T320_T4_Pos) /*!< 0x00000010 */
#define MEMU2_PERIPH_RAM_T320_T4                                             MEMU2_PERIPH_RAM_T320_T4_Msk
#define MEMU2_PERIPH_RAM_T320_T5_Pos                                         (5U)
#define MEMU2_PERIPH_RAM_T320_T5_Msk                                         (0x1UL << MEMU2_PERIPH_RAM_T320_T5_Pos) /*!< 0x00000020 */
#define MEMU2_PERIPH_RAM_T320_T5                                             MEMU2_PERIPH_RAM_T320_T5_Msk
#define MEMU2_PERIPH_RAM_T320_T6_Pos                                         (6U)
#define MEMU2_PERIPH_RAM_T320_T6_Msk                                         (0x1UL << MEMU2_PERIPH_RAM_T320_T6_Pos) /*!< 0x00000040 */
#define MEMU2_PERIPH_RAM_T320_T6                                             MEMU2_PERIPH_RAM_T320_T6_Msk
#define MEMU2_PERIPH_RAM_T320_T7_Pos                                         (7U)
#define MEMU2_PERIPH_RAM_T320_T7_Msk                                         (0x1UL << MEMU2_PERIPH_RAM_T320_T7_Pos) /*!< 0x00000080 */
#define MEMU2_PERIPH_RAM_T320_T7                                             MEMU2_PERIPH_RAM_T320_T7_Msk
#define MEMU2_PERIPH_RAM_T320_T8_Pos                                         (8U)
#define MEMU2_PERIPH_RAM_T320_T8_Msk                                         (0x1UL << MEMU2_PERIPH_RAM_T320_T8_Pos) /*!< 0x00000100 */
#define MEMU2_PERIPH_RAM_T320_T8                                             MEMU2_PERIPH_RAM_T320_T8_Msk
#define MEMU2_PERIPH_RAM_T320_T9_Pos                                         (9U)
#define MEMU2_PERIPH_RAM_T320_T9_Msk                                         (0x1UL << MEMU2_PERIPH_RAM_T320_T9_Pos) /*!< 0x00000200 */
#define MEMU2_PERIPH_RAM_T320_T9                                             MEMU2_PERIPH_RAM_T320_T9_Msk
#define MEMU2_PERIPH_RAM_T320_T10_Pos                                        (10U)
#define MEMU2_PERIPH_RAM_T320_T10_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T320_T10_Pos) /*!< 0x00000400 */
#define MEMU2_PERIPH_RAM_T320_T10                                            MEMU2_PERIPH_RAM_T320_T10_Msk
#define MEMU2_PERIPH_RAM_T320_T11_Pos                                        (11U)
#define MEMU2_PERIPH_RAM_T320_T11_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T320_T11_Pos) /*!< 0x00000800 */
#define MEMU2_PERIPH_RAM_T320_T11                                            MEMU2_PERIPH_RAM_T320_T11_Msk
#define MEMU2_PERIPH_RAM_T320_T12_Pos                                        (12U)
#define MEMU2_PERIPH_RAM_T320_T12_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T320_T12_Pos) /*!< 0x00001000 */
#define MEMU2_PERIPH_RAM_T320_T12                                            MEMU2_PERIPH_RAM_T320_T12_Msk
#define MEMU2_PERIPH_RAM_T320_T13_Pos                                        (13U)
#define MEMU2_PERIPH_RAM_T320_T13_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T320_T13_Pos) /*!< 0x00002000 */
#define MEMU2_PERIPH_RAM_T320_T13                                            MEMU2_PERIPH_RAM_T320_T13_Msk
#define MEMU2_PERIPH_RAM_T320_T14_Pos                                        (14U)
#define MEMU2_PERIPH_RAM_T320_T14_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T320_T14_Pos) /*!< 0x00004000 */
#define MEMU2_PERIPH_RAM_T320_T14                                            MEMU2_PERIPH_RAM_T320_T14_Msk
#define MEMU2_PERIPH_RAM_T320_T15_Pos                                        (15U)
#define MEMU2_PERIPH_RAM_T320_T15_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T320_T15_Pos) /*!< 0x00008000 */
#define MEMU2_PERIPH_RAM_T320_T15                                            MEMU2_PERIPH_RAM_T320_T15_Msk
#define MEMU2_PERIPH_RAM_T320_T16_Pos                                        (16U)
#define MEMU2_PERIPH_RAM_T320_T16_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T320_T16_Pos) /*!< 0x00010000 */
#define MEMU2_PERIPH_RAM_T320_T16                                            MEMU2_PERIPH_RAM_T320_T16_Msk
#define MEMU2_PERIPH_RAM_T320_T17_Pos                                        (17U)
#define MEMU2_PERIPH_RAM_T320_T17_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T320_T17_Pos) /*!< 0x00020000 */
#define MEMU2_PERIPH_RAM_T320_T17                                            MEMU2_PERIPH_RAM_T320_T17_Msk
#define MEMU2_PERIPH_RAM_T320_T18_Pos                                        (18U)
#define MEMU2_PERIPH_RAM_T320_T18_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T320_T18_Pos) /*!< 0x00040000 */
#define MEMU2_PERIPH_RAM_T320_T18                                            MEMU2_PERIPH_RAM_T320_T18_Msk
#define MEMU2_PERIPH_RAM_T320_T19_Pos                                        (19U)
#define MEMU2_PERIPH_RAM_T320_T19_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T320_T19_Pos) /*!< 0x00080000 */
#define MEMU2_PERIPH_RAM_T320_T19                                            MEMU2_PERIPH_RAM_T320_T19_Msk
#define MEMU2_PERIPH_RAM_T320_T20_Pos                                        (20U)
#define MEMU2_PERIPH_RAM_T320_T20_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T320_T20_Pos) /*!< 0x00100000 */
#define MEMU2_PERIPH_RAM_T320_T20                                            MEMU2_PERIPH_RAM_T320_T20_Msk
#define MEMU2_PERIPH_RAM_T320_T21_Pos                                        (21U)
#define MEMU2_PERIPH_RAM_T320_T21_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T320_T21_Pos) /*!< 0x00200000 */
#define MEMU2_PERIPH_RAM_T320_T21                                            MEMU2_PERIPH_RAM_T320_T21_Msk
#define MEMU2_PERIPH_RAM_T320_T22_Pos                                        (22U)
#define MEMU2_PERIPH_RAM_T320_T22_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T320_T22_Pos) /*!< 0x00400000 */
#define MEMU2_PERIPH_RAM_T320_T22                                            MEMU2_PERIPH_RAM_T320_T22_Msk
#define MEMU2_PERIPH_RAM_T320_T23_Pos                                        (23U)
#define MEMU2_PERIPH_RAM_T320_T23_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T320_T23_Pos) /*!< 0x00800000 */
#define MEMU2_PERIPH_RAM_T320_T23                                            MEMU2_PERIPH_RAM_T320_T23_Msk
#define MEMU2_PERIPH_RAM_T320_T24_Pos                                        (24U)
#define MEMU2_PERIPH_RAM_T320_T24_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T320_T24_Pos) /*!< 0x01000000 */
#define MEMU2_PERIPH_RAM_T320_T24                                            MEMU2_PERIPH_RAM_T320_T24_Msk
#define MEMU2_PERIPH_RAM_T320_T25_Pos                                        (25U)
#define MEMU2_PERIPH_RAM_T320_T25_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T320_T25_Pos) /*!< 0x02000000 */
#define MEMU2_PERIPH_RAM_T320_T25                                            MEMU2_PERIPH_RAM_T320_T25_Msk
#define MEMU2_PERIPH_RAM_T320_T26_Pos                                        (26U)
#define MEMU2_PERIPH_RAM_T320_T26_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T320_T26_Pos) /*!< 0x04000000 */
#define MEMU2_PERIPH_RAM_T320_T26                                            MEMU2_PERIPH_RAM_T320_T26_Msk
#define MEMU2_PERIPH_RAM_T320_T27_Pos                                        (27U)
#define MEMU2_PERIPH_RAM_T320_T27_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T320_T27_Pos) /*!< 0x08000000 */
#define MEMU2_PERIPH_RAM_T320_T27                                            MEMU2_PERIPH_RAM_T320_T27_Msk
#define MEMU2_PERIPH_RAM_T320_T28_Pos                                        (28U)
#define MEMU2_PERIPH_RAM_T320_T28_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T320_T28_Pos) /*!< 0x10000000 */
#define MEMU2_PERIPH_RAM_T320_T28                                            MEMU2_PERIPH_RAM_T320_T28_Msk
#define MEMU2_PERIPH_RAM_T320_T29_Pos                                        (29U)
#define MEMU2_PERIPH_RAM_T320_T29_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T320_T29_Pos) /*!< 0x20000000 */
#define MEMU2_PERIPH_RAM_T320_T29                                            MEMU2_PERIPH_RAM_T320_T29_Msk
#define MEMU2_PERIPH_RAM_T320_T30_Pos                                        (30U)
#define MEMU2_PERIPH_RAM_T320_T30_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T320_T30_Pos) /*!< 0x40000000 */
#define MEMU2_PERIPH_RAM_T320_T30                                            MEMU2_PERIPH_RAM_T320_T30_Msk
#define MEMU2_PERIPH_RAM_T320_T31_Pos                                        (31U)
#define MEMU2_PERIPH_RAM_T320_T31_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T320_T31_Pos) /*!< 0x80000000 */
#define MEMU2_PERIPH_RAM_T320_T31                                            MEMU2_PERIPH_RAM_T320_T31_Msk

/************  Bit definition for MEMU2_PERIPH_RAM_T321 register  *************/

#define MEMU2_PERIPH_RAM_T321_T0_Pos                                         (0U)
#define MEMU2_PERIPH_RAM_T321_T0_Msk                                         (0x1UL << MEMU2_PERIPH_RAM_T321_T0_Pos) /*!< 0x00000001 */
#define MEMU2_PERIPH_RAM_T321_T0                                             MEMU2_PERIPH_RAM_T321_T0_Msk
#define MEMU2_PERIPH_RAM_T321_T1_Pos                                         (1U)
#define MEMU2_PERIPH_RAM_T321_T1_Msk                                         (0x1UL << MEMU2_PERIPH_RAM_T321_T1_Pos) /*!< 0x00000002 */
#define MEMU2_PERIPH_RAM_T321_T1                                             MEMU2_PERIPH_RAM_T321_T1_Msk
#define MEMU2_PERIPH_RAM_T321_T2_Pos                                         (2U)
#define MEMU2_PERIPH_RAM_T321_T2_Msk                                         (0x1UL << MEMU2_PERIPH_RAM_T321_T2_Pos) /*!< 0x00000004 */
#define MEMU2_PERIPH_RAM_T321_T2                                             MEMU2_PERIPH_RAM_T321_T2_Msk
#define MEMU2_PERIPH_RAM_T321_T3_Pos                                         (3U)
#define MEMU2_PERIPH_RAM_T321_T3_Msk                                         (0x1UL << MEMU2_PERIPH_RAM_T321_T3_Pos) /*!< 0x00000008 */
#define MEMU2_PERIPH_RAM_T321_T3                                             MEMU2_PERIPH_RAM_T321_T3_Msk
#define MEMU2_PERIPH_RAM_T321_T4_Pos                                         (4U)
#define MEMU2_PERIPH_RAM_T321_T4_Msk                                         (0x1UL << MEMU2_PERIPH_RAM_T321_T4_Pos) /*!< 0x00000010 */
#define MEMU2_PERIPH_RAM_T321_T4                                             MEMU2_PERIPH_RAM_T321_T4_Msk
#define MEMU2_PERIPH_RAM_T321_T5_Pos                                         (5U)
#define MEMU2_PERIPH_RAM_T321_T5_Msk                                         (0x1UL << MEMU2_PERIPH_RAM_T321_T5_Pos) /*!< 0x00000020 */
#define MEMU2_PERIPH_RAM_T321_T5                                             MEMU2_PERIPH_RAM_T321_T5_Msk
#define MEMU2_PERIPH_RAM_T321_T6_Pos                                         (6U)
#define MEMU2_PERIPH_RAM_T321_T6_Msk                                         (0x1UL << MEMU2_PERIPH_RAM_T321_T6_Pos) /*!< 0x00000040 */
#define MEMU2_PERIPH_RAM_T321_T6                                             MEMU2_PERIPH_RAM_T321_T6_Msk
#define MEMU2_PERIPH_RAM_T321_T7_Pos                                         (7U)
#define MEMU2_PERIPH_RAM_T321_T7_Msk                                         (0x1UL << MEMU2_PERIPH_RAM_T321_T7_Pos) /*!< 0x00000080 */
#define MEMU2_PERIPH_RAM_T321_T7                                             MEMU2_PERIPH_RAM_T321_T7_Msk
#define MEMU2_PERIPH_RAM_T321_T8_Pos                                         (8U)
#define MEMU2_PERIPH_RAM_T321_T8_Msk                                         (0x1UL << MEMU2_PERIPH_RAM_T321_T8_Pos) /*!< 0x00000100 */
#define MEMU2_PERIPH_RAM_T321_T8                                             MEMU2_PERIPH_RAM_T321_T8_Msk
#define MEMU2_PERIPH_RAM_T321_T9_Pos                                         (9U)
#define MEMU2_PERIPH_RAM_T321_T9_Msk                                         (0x1UL << MEMU2_PERIPH_RAM_T321_T9_Pos) /*!< 0x00000200 */
#define MEMU2_PERIPH_RAM_T321_T9                                             MEMU2_PERIPH_RAM_T321_T9_Msk
#define MEMU2_PERIPH_RAM_T321_T10_Pos                                        (10U)
#define MEMU2_PERIPH_RAM_T321_T10_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T321_T10_Pos) /*!< 0x00000400 */
#define MEMU2_PERIPH_RAM_T321_T10                                            MEMU2_PERIPH_RAM_T321_T10_Msk
#define MEMU2_PERIPH_RAM_T321_T11_Pos                                        (11U)
#define MEMU2_PERIPH_RAM_T321_T11_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T321_T11_Pos) /*!< 0x00000800 */
#define MEMU2_PERIPH_RAM_T321_T11                                            MEMU2_PERIPH_RAM_T321_T11_Msk
#define MEMU2_PERIPH_RAM_T321_T12_Pos                                        (12U)
#define MEMU2_PERIPH_RAM_T321_T12_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T321_T12_Pos) /*!< 0x00001000 */
#define MEMU2_PERIPH_RAM_T321_T12                                            MEMU2_PERIPH_RAM_T321_T12_Msk
#define MEMU2_PERIPH_RAM_T321_T13_Pos                                        (13U)
#define MEMU2_PERIPH_RAM_T321_T13_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T321_T13_Pos) /*!< 0x00002000 */
#define MEMU2_PERIPH_RAM_T321_T13                                            MEMU2_PERIPH_RAM_T321_T13_Msk
#define MEMU2_PERIPH_RAM_T321_T14_Pos                                        (14U)
#define MEMU2_PERIPH_RAM_T321_T14_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T321_T14_Pos) /*!< 0x00004000 */
#define MEMU2_PERIPH_RAM_T321_T14                                            MEMU2_PERIPH_RAM_T321_T14_Msk
#define MEMU2_PERIPH_RAM_T321_T15_Pos                                        (15U)
#define MEMU2_PERIPH_RAM_T321_T15_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T321_T15_Pos) /*!< 0x00008000 */
#define MEMU2_PERIPH_RAM_T321_T15                                            MEMU2_PERIPH_RAM_T321_T15_Msk
#define MEMU2_PERIPH_RAM_T321_T16_Pos                                        (16U)
#define MEMU2_PERIPH_RAM_T321_T16_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T321_T16_Pos) /*!< 0x00010000 */
#define MEMU2_PERIPH_RAM_T321_T16                                            MEMU2_PERIPH_RAM_T321_T16_Msk
#define MEMU2_PERIPH_RAM_T321_T17_Pos                                        (17U)
#define MEMU2_PERIPH_RAM_T321_T17_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T321_T17_Pos) /*!< 0x00020000 */
#define MEMU2_PERIPH_RAM_T321_T17                                            MEMU2_PERIPH_RAM_T321_T17_Msk
#define MEMU2_PERIPH_RAM_T321_T18_Pos                                        (18U)
#define MEMU2_PERIPH_RAM_T321_T18_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T321_T18_Pos) /*!< 0x00040000 */
#define MEMU2_PERIPH_RAM_T321_T18                                            MEMU2_PERIPH_RAM_T321_T18_Msk
#define MEMU2_PERIPH_RAM_T321_T19_Pos                                        (19U)
#define MEMU2_PERIPH_RAM_T321_T19_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T321_T19_Pos) /*!< 0x00080000 */
#define MEMU2_PERIPH_RAM_T321_T19                                            MEMU2_PERIPH_RAM_T321_T19_Msk
#define MEMU2_PERIPH_RAM_T321_T20_Pos                                        (20U)
#define MEMU2_PERIPH_RAM_T321_T20_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T321_T20_Pos) /*!< 0x00100000 */
#define MEMU2_PERIPH_RAM_T321_T20                                            MEMU2_PERIPH_RAM_T321_T20_Msk
#define MEMU2_PERIPH_RAM_T321_T21_Pos                                        (21U)
#define MEMU2_PERIPH_RAM_T321_T21_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T321_T21_Pos) /*!< 0x00200000 */
#define MEMU2_PERIPH_RAM_T321_T21                                            MEMU2_PERIPH_RAM_T321_T21_Msk
#define MEMU2_PERIPH_RAM_T321_T22_Pos                                        (22U)
#define MEMU2_PERIPH_RAM_T321_T22_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T321_T22_Pos) /*!< 0x00400000 */
#define MEMU2_PERIPH_RAM_T321_T22                                            MEMU2_PERIPH_RAM_T321_T22_Msk
#define MEMU2_PERIPH_RAM_T321_T23_Pos                                        (23U)
#define MEMU2_PERIPH_RAM_T321_T23_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T321_T23_Pos) /*!< 0x00800000 */
#define MEMU2_PERIPH_RAM_T321_T23                                            MEMU2_PERIPH_RAM_T321_T23_Msk
#define MEMU2_PERIPH_RAM_T321_T24_Pos                                        (24U)
#define MEMU2_PERIPH_RAM_T321_T24_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T321_T24_Pos) /*!< 0x01000000 */
#define MEMU2_PERIPH_RAM_T321_T24                                            MEMU2_PERIPH_RAM_T321_T24_Msk
#define MEMU2_PERIPH_RAM_T321_T25_Pos                                        (25U)
#define MEMU2_PERIPH_RAM_T321_T25_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T321_T25_Pos) /*!< 0x02000000 */
#define MEMU2_PERIPH_RAM_T321_T25                                            MEMU2_PERIPH_RAM_T321_T25_Msk
#define MEMU2_PERIPH_RAM_T321_T26_Pos                                        (26U)
#define MEMU2_PERIPH_RAM_T321_T26_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T321_T26_Pos) /*!< 0x04000000 */
#define MEMU2_PERIPH_RAM_T321_T26                                            MEMU2_PERIPH_RAM_T321_T26_Msk
#define MEMU2_PERIPH_RAM_T321_T27_Pos                                        (27U)
#define MEMU2_PERIPH_RAM_T321_T27_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T321_T27_Pos) /*!< 0x08000000 */
#define MEMU2_PERIPH_RAM_T321_T27                                            MEMU2_PERIPH_RAM_T321_T27_Msk
#define MEMU2_PERIPH_RAM_T321_T28_Pos                                        (28U)
#define MEMU2_PERIPH_RAM_T321_T28_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T321_T28_Pos) /*!< 0x10000000 */
#define MEMU2_PERIPH_RAM_T321_T28                                            MEMU2_PERIPH_RAM_T321_T28_Msk
#define MEMU2_PERIPH_RAM_T321_T29_Pos                                        (29U)
#define MEMU2_PERIPH_RAM_T321_T29_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T321_T29_Pos) /*!< 0x20000000 */
#define MEMU2_PERIPH_RAM_T321_T29                                            MEMU2_PERIPH_RAM_T321_T29_Msk
#define MEMU2_PERIPH_RAM_T321_T30_Pos                                        (30U)
#define MEMU2_PERIPH_RAM_T321_T30_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T321_T30_Pos) /*!< 0x40000000 */
#define MEMU2_PERIPH_RAM_T321_T30                                            MEMU2_PERIPH_RAM_T321_T30_Msk
#define MEMU2_PERIPH_RAM_T321_T31_Pos                                        (31U)
#define MEMU2_PERIPH_RAM_T321_T31_Msk                                        (0x1UL << MEMU2_PERIPH_RAM_T321_T31_Pos) /*!< 0x80000000 */
#define MEMU2_PERIPH_RAM_T321_T31                                            MEMU2_PERIPH_RAM_T321_T31_Msk

/*******  Bit definition for MEMU2_NVM_SB_CERR_TBL_FILL_STAT register  ********/

#define MEMU2_NVM_SB_CERR_TBL_FILL_STAT_SB_CERR_TBL_FILL_VAL_Pos             (0U)
#define MEMU2_NVM_SB_CERR_TBL_FILL_STAT_SB_CERR_TBL_FILL_VAL_Msk             (0xFFUL << MEMU2_NVM_SB_CERR_TBL_FILL_STAT_SB_CERR_TBL_FILL_VAL_Pos) /*!< 0x000000FF */
#define MEMU2_NVM_SB_CERR_TBL_FILL_STAT_SB_CERR_TBL_FILL_VAL                 MEMU2_NVM_SB_CERR_TBL_FILL_STAT_SB_CERR_TBL_FILL_VAL_Msk

/*******  Bit definition for MEMU2_NVM_DB_CERR_TBL_FILL_STAT register  ********/

#define MEMU2_NVM_DB_CERR_TBL_FILL_STAT_DB_CERR_TBL_FILL_VAL_Pos             (0U)
#define MEMU2_NVM_DB_CERR_TBL_FILL_STAT_DB_CERR_TBL_FILL_VAL_Msk             (0xFFUL << MEMU2_NVM_DB_CERR_TBL_FILL_STAT_DB_CERR_TBL_FILL_VAL_Pos) /*!< 0x000000FF */
#define MEMU2_NVM_DB_CERR_TBL_FILL_STAT_DB_CERR_TBL_FILL_VAL                 MEMU2_NVM_DB_CERR_TBL_FILL_STAT_DB_CERR_TBL_FILL_VAL_Msk

/********  Bit definition for MEMU2_NVM_UNCERR_TBL_FILL_STAT register  ********/

#define MEMU2_NVM_UNCERR_TBL_FILL_STAT_UNCERR_TBL_FILL_VAL_Pos               (0U)
#define MEMU2_NVM_UNCERR_TBL_FILL_STAT_UNCERR_TBL_FILL_VAL_Msk               (0xFFUL << MEMU2_NVM_UNCERR_TBL_FILL_STAT_UNCERR_TBL_FILL_VAL_Pos) /*!< 0x000000FF */
#define MEMU2_NVM_UNCERR_TBL_FILL_STAT_UNCERR_TBL_FILL_VAL                   MEMU2_NVM_UNCERR_TBL_FILL_STAT_UNCERR_TBL_FILL_VAL_Msk

/****************  Bit definition for MEMU2_NVM_OFLW register  ****************/

#define MEMU2_NVM_OFLW_OFLW_Pos                                              (0U)
#define MEMU2_NVM_OFLW_OFLW_Msk                                              (0xFFFFFFFFUL << MEMU2_NVM_OFLW_OFLW_Pos) /*!< 0xFFFFFFFF */
#define MEMU2_NVM_OFLW_OFLW                                                  MEMU2_NVM_OFLW_OFLW_Msk

/****************  Bit definition for MEMU2_NVM_T320 register  ****************/

#define MEMU2_NVM_T320_T0_Pos                                                (0U)
#define MEMU2_NVM_T320_T0_Msk                                                (0x1UL << MEMU2_NVM_T320_T0_Pos) /*!< 0x00000001 */
#define MEMU2_NVM_T320_T0                                                    MEMU2_NVM_T320_T0_Msk
#define MEMU2_NVM_T320_T1_Pos                                                (1U)
#define MEMU2_NVM_T320_T1_Msk                                                (0x1UL << MEMU2_NVM_T320_T1_Pos) /*!< 0x00000002 */
#define MEMU2_NVM_T320_T1                                                    MEMU2_NVM_T320_T1_Msk
#define MEMU2_NVM_T320_T2_Pos                                                (2U)
#define MEMU2_NVM_T320_T2_Msk                                                (0x1UL << MEMU2_NVM_T320_T2_Pos) /*!< 0x00000004 */
#define MEMU2_NVM_T320_T2                                                    MEMU2_NVM_T320_T2_Msk
#define MEMU2_NVM_T320_T3_Pos                                                (3U)
#define MEMU2_NVM_T320_T3_Msk                                                (0x1UL << MEMU2_NVM_T320_T3_Pos) /*!< 0x00000008 */
#define MEMU2_NVM_T320_T3                                                    MEMU2_NVM_T320_T3_Msk
#define MEMU2_NVM_T320_T4_Pos                                                (4U)
#define MEMU2_NVM_T320_T4_Msk                                                (0x1UL << MEMU2_NVM_T320_T4_Pos) /*!< 0x00000010 */
#define MEMU2_NVM_T320_T4                                                    MEMU2_NVM_T320_T4_Msk
#define MEMU2_NVM_T320_T5_Pos                                                (5U)
#define MEMU2_NVM_T320_T5_Msk                                                (0x1UL << MEMU2_NVM_T320_T5_Pos) /*!< 0x00000020 */
#define MEMU2_NVM_T320_T5                                                    MEMU2_NVM_T320_T5_Msk
#define MEMU2_NVM_T320_T6_Pos                                                (6U)
#define MEMU2_NVM_T320_T6_Msk                                                (0x1UL << MEMU2_NVM_T320_T6_Pos) /*!< 0x00000040 */
#define MEMU2_NVM_T320_T6                                                    MEMU2_NVM_T320_T6_Msk
#define MEMU2_NVM_T320_T7_Pos                                                (7U)
#define MEMU2_NVM_T320_T7_Msk                                                (0x1UL << MEMU2_NVM_T320_T7_Pos) /*!< 0x00000080 */
#define MEMU2_NVM_T320_T7                                                    MEMU2_NVM_T320_T7_Msk
#define MEMU2_NVM_T320_T8_Pos                                                (8U)
#define MEMU2_NVM_T320_T8_Msk                                                (0x1UL << MEMU2_NVM_T320_T8_Pos) /*!< 0x00000100 */
#define MEMU2_NVM_T320_T8                                                    MEMU2_NVM_T320_T8_Msk
#define MEMU2_NVM_T320_T9_Pos                                                (9U)
#define MEMU2_NVM_T320_T9_Msk                                                (0x1UL << MEMU2_NVM_T320_T9_Pos) /*!< 0x00000200 */
#define MEMU2_NVM_T320_T9                                                    MEMU2_NVM_T320_T9_Msk
#define MEMU2_NVM_T320_T10_Pos                                               (10U)
#define MEMU2_NVM_T320_T10_Msk                                               (0x1UL << MEMU2_NVM_T320_T10_Pos) /*!< 0x00000400 */
#define MEMU2_NVM_T320_T10                                                   MEMU2_NVM_T320_T10_Msk
#define MEMU2_NVM_T320_T11_Pos                                               (11U)
#define MEMU2_NVM_T320_T11_Msk                                               (0x1UL << MEMU2_NVM_T320_T11_Pos) /*!< 0x00000800 */
#define MEMU2_NVM_T320_T11                                                   MEMU2_NVM_T320_T11_Msk
#define MEMU2_NVM_T320_T12_Pos                                               (12U)
#define MEMU2_NVM_T320_T12_Msk                                               (0x1UL << MEMU2_NVM_T320_T12_Pos) /*!< 0x00001000 */
#define MEMU2_NVM_T320_T12                                                   MEMU2_NVM_T320_T12_Msk
#define MEMU2_NVM_T320_T13_Pos                                               (13U)
#define MEMU2_NVM_T320_T13_Msk                                               (0x1UL << MEMU2_NVM_T320_T13_Pos) /*!< 0x00002000 */
#define MEMU2_NVM_T320_T13                                                   MEMU2_NVM_T320_T13_Msk
#define MEMU2_NVM_T320_T14_Pos                                               (14U)
#define MEMU2_NVM_T320_T14_Msk                                               (0x1UL << MEMU2_NVM_T320_T14_Pos) /*!< 0x00004000 */
#define MEMU2_NVM_T320_T14                                                   MEMU2_NVM_T320_T14_Msk
#define MEMU2_NVM_T320_T15_Pos                                               (15U)
#define MEMU2_NVM_T320_T15_Msk                                               (0x1UL << MEMU2_NVM_T320_T15_Pos) /*!< 0x00008000 */
#define MEMU2_NVM_T320_T15                                                   MEMU2_NVM_T320_T15_Msk
#define MEMU2_NVM_T320_T16_Pos                                               (16U)
#define MEMU2_NVM_T320_T16_Msk                                               (0x1UL << MEMU2_NVM_T320_T16_Pos) /*!< 0x00010000 */
#define MEMU2_NVM_T320_T16                                                   MEMU2_NVM_T320_T16_Msk
#define MEMU2_NVM_T320_T17_Pos                                               (17U)
#define MEMU2_NVM_T320_T17_Msk                                               (0x1UL << MEMU2_NVM_T320_T17_Pos) /*!< 0x00020000 */
#define MEMU2_NVM_T320_T17                                                   MEMU2_NVM_T320_T17_Msk
#define MEMU2_NVM_T320_T18_Pos                                               (18U)
#define MEMU2_NVM_T320_T18_Msk                                               (0x1UL << MEMU2_NVM_T320_T18_Pos) /*!< 0x00040000 */
#define MEMU2_NVM_T320_T18                                                   MEMU2_NVM_T320_T18_Msk
#define MEMU2_NVM_T320_T19_Pos                                               (19U)
#define MEMU2_NVM_T320_T19_Msk                                               (0x1UL << MEMU2_NVM_T320_T19_Pos) /*!< 0x00080000 */
#define MEMU2_NVM_T320_T19                                                   MEMU2_NVM_T320_T19_Msk
#define MEMU2_NVM_T320_T20_Pos                                               (20U)
#define MEMU2_NVM_T320_T20_Msk                                               (0x1UL << MEMU2_NVM_T320_T20_Pos) /*!< 0x00100000 */
#define MEMU2_NVM_T320_T20                                                   MEMU2_NVM_T320_T20_Msk
#define MEMU2_NVM_T320_T21_Pos                                               (21U)
#define MEMU2_NVM_T320_T21_Msk                                               (0x1UL << MEMU2_NVM_T320_T21_Pos) /*!< 0x00200000 */
#define MEMU2_NVM_T320_T21                                                   MEMU2_NVM_T320_T21_Msk
#define MEMU2_NVM_T320_T22_Pos                                               (22U)
#define MEMU2_NVM_T320_T22_Msk                                               (0x1UL << MEMU2_NVM_T320_T22_Pos) /*!< 0x00400000 */
#define MEMU2_NVM_T320_T22                                                   MEMU2_NVM_T320_T22_Msk
#define MEMU2_NVM_T320_T23_Pos                                               (23U)
#define MEMU2_NVM_T320_T23_Msk                                               (0x1UL << MEMU2_NVM_T320_T23_Pos) /*!< 0x00800000 */
#define MEMU2_NVM_T320_T23                                                   MEMU2_NVM_T320_T23_Msk
#define MEMU2_NVM_T320_T24_Pos                                               (24U)
#define MEMU2_NVM_T320_T24_Msk                                               (0x1UL << MEMU2_NVM_T320_T24_Pos) /*!< 0x01000000 */
#define MEMU2_NVM_T320_T24                                                   MEMU2_NVM_T320_T24_Msk
#define MEMU2_NVM_T320_T25_Pos                                               (25U)
#define MEMU2_NVM_T320_T25_Msk                                               (0x1UL << MEMU2_NVM_T320_T25_Pos) /*!< 0x02000000 */
#define MEMU2_NVM_T320_T25                                                   MEMU2_NVM_T320_T25_Msk
#define MEMU2_NVM_T320_T26_Pos                                               (26U)
#define MEMU2_NVM_T320_T26_Msk                                               (0x1UL << MEMU2_NVM_T320_T26_Pos) /*!< 0x04000000 */
#define MEMU2_NVM_T320_T26                                                   MEMU2_NVM_T320_T26_Msk
#define MEMU2_NVM_T320_T27_Pos                                               (27U)
#define MEMU2_NVM_T320_T27_Msk                                               (0x1UL << MEMU2_NVM_T320_T27_Pos) /*!< 0x08000000 */
#define MEMU2_NVM_T320_T27                                                   MEMU2_NVM_T320_T27_Msk
#define MEMU2_NVM_T320_T28_Pos                                               (28U)
#define MEMU2_NVM_T320_T28_Msk                                               (0x1UL << MEMU2_NVM_T320_T28_Pos) /*!< 0x10000000 */
#define MEMU2_NVM_T320_T28                                                   MEMU2_NVM_T320_T28_Msk
#define MEMU2_NVM_T320_T29_Pos                                               (29U)
#define MEMU2_NVM_T320_T29_Msk                                               (0x1UL << MEMU2_NVM_T320_T29_Pos) /*!< 0x20000000 */
#define MEMU2_NVM_T320_T29                                                   MEMU2_NVM_T320_T29_Msk
#define MEMU2_NVM_T320_T30_Pos                                               (30U)
#define MEMU2_NVM_T320_T30_Msk                                               (0x1UL << MEMU2_NVM_T320_T30_Pos) /*!< 0x40000000 */
#define MEMU2_NVM_T320_T30                                                   MEMU2_NVM_T320_T30_Msk
#define MEMU2_NVM_T320_T31_Pos                                               (31U)
#define MEMU2_NVM_T320_T31_Msk                                               (0x1UL << MEMU2_NVM_T320_T31_Pos) /*!< 0x80000000 */
#define MEMU2_NVM_T320_T31                                                   MEMU2_NVM_T320_T31_Msk

#define MEMU2_BASE                     0x4000c000UL 


#define MEMU2                          ((MEMU2_TypeDef *) MEMU2_BASE) 


#endif /* __SR5E1_MEMU2_H */

