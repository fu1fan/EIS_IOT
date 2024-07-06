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
 *****************************************************************************/

#ifndef __SR5E1_SMPU_H
#define __SR5E1_SMPU_H

/******************************************************************************/
/*                                                                            */
/*                    System memory protection unit (SMPU)                    */
/*                                                                            */
/******************************************************************************/

typedef struct {   /* Offset =0x00000100 Next_offset = 0x00000180 */
  __I  uint32_t EAR;                        /*!< Error Address Register, Bus Master n    Address offset: 0x000 */
  __I  uint32_t EDR;                        /*!< Error Detail Register, Bus Master n     Address offset: 0x004 */
} SMPU_ERROR_TypeDef;

typedef struct {   /* Offset =0x00000400 Next_offset = 0x000004c0 */
  __IO uint32_t RGDN_WORD0;                 /*!< Region Descriptor n, Word 0             Address offset: 0x000 */
  __IO uint32_t RGDN_WORD1;                 /*!< Region Descriptor n, Word 1             Address offset: 0x004 */
  __IO uint32_t RGDN_WORD2_FMT0;            /*!< Region Descriptor n, Word 2 Format 0    Address offset: 0x008 */
  __IO uint32_t RGDN_WORD3;                 /*!< Region Descriptor n, Word 3             Address offset: 0x00C */
} SMPU_REGION_TypeDef;

typedef struct
{
  __IO uint32_t CESR0;                      /*!< Control/Error Status Register 0         Address offset: 0x000 */
  __I  uint32_t CESR1;                      /*!< Control/Error Status Register 1         Address offset: 0x004 */
        uint8_t RESERVED0[248];             /*!< Reserved                                Address offset: 0x008-0x0FF */
        SMPU_ERROR_TypeDef ERROR[16];       /*!< Error Address/Detail Registers          Address offset: 0x00000100-0x00000180 */
        uint8_t RESERVED1[640];             /*!< Reserved                                Address offset: 0x180-0x3FF */
        SMPU_REGION_TypeDef REGION[24];     /*!< Region Descriptor Registers             Address offset: 0x00000400-0x000004c0 */
} SMPU_TypeDef;

/******************  Bit definition for SMPU_CESR0 register  ******************/

#define SMPU_CESR0_MERR_Pos                           (0U)
#define SMPU_CESR0_MERR_Msk                           (0xFFFFUL << SMPU_CESR0_MERR_Pos) /*!< 0x0000FFFF */
#define SMPU_CESR0_MERR                               SMPU_CESR0_MERR_Msk
#define SMPU_CESR0_GVLD_Pos                           (31U)
#define SMPU_CESR0_GVLD_Msk                           (0x1UL << SMPU_CESR0_GVLD_Pos) /*!< 0x00000004 */
#define SMPU_CESR0_GVLD                               SMPU_CESR0_GVLD_Msk
#define SMPU_CESR0_HRL_Pos                            (28U)
#define SMPU_CESR0_HRL_Msk                            (0x7UL << SMPU_CESR0_HRL_Pos) /*!< 0x70000000 */
#define SMPU_CESR0_HRL                                SMPU_CESR0_HRL_Msk

/******************  Bit definition for SMPU_CESR1 register  ******************/

#define SMPU_CESR1_MEOVR_Pos                          (0U)
#define SMPU_CESR1_MEOVR_Msk                          (0xFFFFUL << SMPU_CESR1_MEOVR_Pos) /*!< 0x0000FFFF */
#define SMPU_CESR1_MEOVR                              SMPU_CESR1_MEOVR_Msk
#define SMPU_CESR1_NRGD_Pos                           (28U)
#define SMPU_CESR1_NRGD_Msk                           (0xFUL << SMPU_CESR1_NRGD_Pos) /*!< 0xF0000000 */
#define SMPU_CESR1_NRGD                               SMPU_CESR1_NRGD_Msk

/****************  Bit definition for SMPU_ERROR_EAR register  ****************/
#define SMPU_ERROR_EAR_EADDR_Pos                      (0U)
#define SMPU_ERROR_EAR_EADDR_Msk                      (0xFFFFFFFFUL << SMPU_ERROR_EAR_EADDR_Pos) /*!< 0xFFFFFFFF */
#define SMPU_ERROR_EAR_EADDR                          SMPU_ERROR_EAR_EADDR_Msk

/****************  Bit definition for SMPU_ERROR_EDR register  ****************/
#define SMPU_ERROR_EDR_EACD_Pos                       (0U)
#define SMPU_ERROR_EDR_EACD_Msk                       (0xFFFFFFUL << SMPU_ERROR_EDR_EACD_Pos) /*!< 0x00FFFFFF */
#define SMPU_ERROR_EDR_EACD                           SMPU_ERROR_EDR_EACD_Msk
#define SMPU_ERROR_EDR_EATTR_Pos                      (25U)
#define SMPU_ERROR_EDR_EATTR_Msk                      (0x3UL << SMPU_ERROR_EDR_EATTR_Pos) /*!< 0x0C000000 */
#define SMPU_ERROR_EDR_EATTR                          SMPU_ERROR_EDR_EATTR_Msk
#define SMPU_ERROR_EDR_ERW_Pos                        (27U)
#define SMPU_ERROR_EDR_ERW_Msk                        (0x1UL << SMPU_ERROR_EDR_ERW_Pos) /*!< 0x08000000 */
#define SMPU_ERROR_EDR_ERW                            SMPU_ERROR_EDR_ERW_Msk
#define SMPU_ERROR_EDR_EMN_Pos                        (28U)
#define SMPU_ERROR_EDR_EMN_Msk                        (0xFUL << SMPU_ERROR_EDR_EMN_Pos) /*!< 0xF0000000 */
#define SMPU_ERROR_EDR_EMN                            SMPU_ERROR_EDR_EMN_Msk

/************  Bit definition for SMPU_REGION_RGDN_WORD0 register  ************/
#define SMPU_REGION_RGDN_WORD0_SRTADDR_Pos            (0U)
#define SMPU_REGION_RGDN_WORD0_SRTADDR_Msk            (0xFFFFFFFFUL << SMPU_REGION_RGDN_WORD0_SRTADDR_Pos) /*!< 0xFFFFFFFF */
#define SMPU_REGION_RGDN_WORD0_SRTADDR                SMPU_REGION_RGDN_WORD0_SRTADDR_Msk

/************  Bit definition for SMPU_REGION_RGDN_WORD1 register  ************/
#define SMPU_REGION_RGDN_WORD1_ENDADDR_Pos            (0U)
#define SMPU_REGION_RGDN_WORD1_ENDADDR_Msk            (0xFFFFFFFFUL << SMPU_REGION_RGDN_WORD1_ENDADDR_Pos) /*!< 0xFFFFFFFF */
#define SMPU_REGION_RGDN_WORD1_ENDADDR                SMPU_REGION_RGDN_WORD1_ENDADDR_Msk

/*********  Bit definition for SMPU_REGION_RGDN_WORD2_FMT0 register  **********/
#define SMPU_REGION_RGDN_WORD2_FMT0_M0P_Pos           (0U)
#define SMPU_REGION_RGDN_WORD2_FMT0_M0P_Msk           (0x3UL << SMPU_REGION_RGDN_WORD2_FMT0_M0P_Pos) /*!< 0x00000003 */
#define SMPU_REGION_RGDN_WORD2_FMT0_M0P               SMPU_REGION_RGDN_WORD2_FMT0_M0P_Msk
#define SMPU_REGION_RGDN_WORD2_FMT0_M1P_Pos           (2U)
#define SMPU_REGION_RGDN_WORD2_FMT0_M1P_Msk           (0x3UL << SMPU_REGION_RGDN_WORD2_FMT0_M1P_Pos) /*!< 0x0000000C */
#define SMPU_REGION_RGDN_WORD2_FMT0_M1P               SMPU_REGION_RGDN_WORD2_FMT0_M1P_Msk
#define SMPU_REGION_RGDN_WORD2_FMT0_M2P_Pos           (4U)
#define SMPU_REGION_RGDN_WORD2_FMT0_M2P_Msk           (0x3UL << SMPU_REGION_RGDN_WORD2_FMT0_M2P_Pos) /*!< 0x00000030 */
#define SMPU_REGION_RGDN_WORD2_FMT0_M2P               SMPU_REGION_RGDN_WORD2_FMT0_M2P_Msk
#define SMPU_REGION_RGDN_WORD2_FMT0_M3P_Pos           (6U)
#define SMPU_REGION_RGDN_WORD2_FMT0_M3P_Msk           (0x3UL << SMPU_REGION_RGDN_WORD2_FMT0_M3P_Pos) /*!< 0x000000C0 */
#define SMPU_REGION_RGDN_WORD2_FMT0_M3P               SMPU_REGION_RGDN_WORD2_FMT0_M3P_Msk
#define SMPU_REGION_RGDN_WORD2_FMT0_M4P_Pos           (8U)
#define SMPU_REGION_RGDN_WORD2_FMT0_M4P_Msk           (0x3UL << SMPU_REGION_RGDN_WORD2_FMT0_M4P_Pos) /*!< 0x00000300 */
#define SMPU_REGION_RGDN_WORD2_FMT0_M4P               SMPU_REGION_RGDN_WORD2_FMT0_M4P_Msk
#define SMPU_REGION_RGDN_WORD2_FMT0_M5P_Pos           (10U)
#define SMPU_REGION_RGDN_WORD2_FMT0_M5P_Msk           (0x3UL << SMPU_REGION_RGDN_WORD2_FMT0_M5P_Pos) /*!< 0x00000C00 */
#define SMPU_REGION_RGDN_WORD2_FMT0_M5P               SMPU_REGION_RGDN_WORD2_FMT0_M5P_Msk
#define SMPU_REGION_RGDN_WORD2_FMT0_M6P_Pos           (12U)
#define SMPU_REGION_RGDN_WORD2_FMT0_M6P_Msk           (0x3UL << SMPU_REGION_RGDN_WORD2_FMT0_M6P_Pos) /*!< 0x00003000 */
#define SMPU_REGION_RGDN_WORD2_FMT0_M6P               SMPU_REGION_RGDN_WORD2_FMT0_M6P_Msk 
#define SMPU_REGION_RGDN_WORD2_FMT0_M7P_Pos           (14U)
#define SMPU_REGION_RGDN_WORD2_FMT0_M7P_Msk           (0x3UL << SMPU_REGION_RGDN_WORD2_FMT0_M7P_Pos) /*!< 0x0000C000 */
#define SMPU_REGION_RGDN_WORD2_FMT0_M7P               SMPU_REGION_RGDN_WORD2_FMT0_M7P_Msk
#define SMPU_REGION_RGDN_WORD2_FMT0_M8P_Pos           (16U)
#define SMPU_REGION_RGDN_WORD2_FMT0_M8P_Msk           (0x3UL << SMPU_REGION_RGDN_WORD2_FMT0_M8P_Pos) /*!< 0x00030000 */
#define SMPU_REGION_RGDN_WORD2_FMT0_M8P               SMPU_REGION_RGDN_WORD2_FMT0_M8P_Msk
#define SMPU_REGION_RGDN_WORD2_FMT0_M9P_Pos           (18U)
#define SMPU_REGION_RGDN_WORD2_FMT0_M9P_Msk           (0x3UL << SMPU_REGION_RGDN_WORD2_FMT0_M9P_Pos) /*!< 0x000C0000 */
#define SMPU_REGION_RGDN_WORD2_FMT0_M9P               SMPU_REGION_RGDN_WORD2_FMT0_M9P_Msk
#define SMPU_REGION_RGDN_WORD2_FMT0_M10P_Pos          (20U)
#define SMPU_REGION_RGDN_WORD2_FMT0_M10P_Msk          (0x3UL << SMPU_REGION_RGDN_WORD2_FMT0_M10P_Pos) /*!< 0x00300000 */
#define SMPU_REGION_RGDN_WORD2_FMT0_M10P              SMPU_REGION_RGDN_WORD2_FMT0_M10P_Msk
#define SMPU_REGION_RGDN_WORD2_FMT0_M11P_Pos          (22U)
#define SMPU_REGION_RGDN_WORD2_FMT0_M11P_Msk          (0x3UL << SMPU_REGION_RGDN_WORD2_FMT0_M11P_Pos) /*!< 0x00C00000 */
#define SMPU_REGION_RGDN_WORD2_FMT0_M11P              SMPU_REGION_RGDN_WORD2_FMT0_M11P_Msk
#define SMPU_REGION_RGDN_WORD2_FMT0_M12P_Pos          (24U)
#define SMPU_REGION_RGDN_WORD2_FMT0_M12P_Msk          (0x3UL << SMPU_REGION_RGDN_WORD2_FMT0_M12P_Pos) /*!< 0x03000000 */
#define SMPU_REGION_RGDN_WORD2_FMT0_M12P              SMPU_REGION_RGDN_WORD2_FMT0_M12P_Msk
#define SMPU_REGION_RGDN_WORD2_FMT0_M13P_Pos          (26U)
#define SMPU_REGION_RGDN_WORD2_FMT0_M13P_Msk          (0x3UL << SMPU_REGION_RGDN_WORD2_FMT0_M13P_Pos) /*!< 0x0C000000 */
#define SMPU_REGION_RGDN_WORD2_FMT0_M13P              SMPU_REGION_RGDN_WORD2_FMT0_M13P_Msk
#define SMPU_REGION_RGDN_WORD2_FMT0_M14P_Pos          (28U)
#define SMPU_REGION_RGDN_WORD2_FMT0_M14P_Msk          (0x3UL << SMPU_REGION_RGDN_WORD2_FMT0_M14P_Pos) /*!< 0x30000000 */
#define SMPU_REGION_RGDN_WORD2_FMT0_M14P              SMPU_REGION_RGDN_WORD2_FMT0_M14P_Msk
#define SMPU_REGION_RGDN_WORD2_FMT0_M15P_Pos          (30U)
#define SMPU_REGION_RGDN_WORD2_FMT0_M15P_Msk          (0x3UL << SMPU_REGION_RGDN_WORD2_FMT0_M15P_Pos) /*!< 0xC0000000 */
#define SMPU_REGION_RGDN_WORD2_FMT0_M15P              SMPU_REGION_RGDN_WORD2_FMT0_M15P_Msk

/************  Bit definition for SMPU_REGION_RGDN_WORD3 register  ************/
#define SMPU_REGION_RGDN_WORD3_FMT_Pos                (27U)
#define SMPU_REGION_RGDN_WORD3_FMT_Msk                (0x1UL << SMPU_REGION_RGDN_WORD3_FMT_Pos) /*!< 0x08000000 */
#define SMPU_REGION_RGDN_WORD3_FMT                    SMPU_REGION_RGDN_WORD3_FMT_Msk
#define SMPU_REGION_RGDN_WORD3_RO_Pos                 (28U)
#define SMPU_REGION_RGDN_WORD3_RO_Msk                 (0x1UL << SMPU_REGION_RGDN_WORD3_RO_Pos) /*!< 0x10000000 */
#define SMPU_REGION_RGDN_WORD3_RO                     SMPU_REGION_RGDN_WORD3_RO_Msk
#define SMPU_REGION_RGDN_WORD3_CI_Pos                 (30U)
#define SMPU_REGION_RGDN_WORD3_CI_Msk                 (0x1UL << SMPU_REGION_RGDN_WORD3_CI_Pos) /*!< 0x40000000 */
#define SMPU_REGION_RGDN_WORD3_CI                     SMPU_REGION_RGDN_WORD3_CI_Msk
#define SMPU_REGION_RGDN_WORD3_VLD_Pos                (31U)
#define SMPU_REGION_RGDN_WORD3_VLD_Msk                (0x1UL << SMPU_REGION_RGDN_WORD3_VLD_Pos) /*!< 0x80000000 */
#define SMPU_REGION_RGDN_WORD3_VLD                    SMPU_REGION_RGDN_WORD3_VLD_Msk


#define SMPU_BASE                                     0x42000800UL


#define SMPU                                          ((SMPU_TypeDef *) SMPU_BASE)


#endif /* __SR5E1_SMPU_H */

