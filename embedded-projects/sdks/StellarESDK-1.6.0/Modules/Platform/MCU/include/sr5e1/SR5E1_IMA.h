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

#ifndef __SR5E1_IMA_H
#define __SR5E1_IMA_H

/******************************************************************************/
/*                                                                            */
/*                                   (IMA)                                    */
/*                                                                            */
/******************************************************************************/

typedef struct
{
  __IO uint32_t CTRL;          /*!< CTRL          Address offset: 0x00      */
  __IO uint32_t ENABLE;        /*!< ENABLE        Address offset: 0x04      */
  __I  uint32_t STATUS;        /*!< STATUS        Address offset: 0x08      */
  __IO uint32_t SLCT;          /*!< SLCT          Address offset: 0x0C      */
  __IO uint32_t WRITE_UNLOCK;  /*!< WRITE_UNLOCK  Address offset: 0x10      */
  __IO uint32_t READ_UNLOCK;   /*!< READ_UNLOCK   Address offset: 0x14      */
       uint32_t RESERVED0[5];  /*!< Reserved      Address offset: 0x18-0x2B */
  __IO uint32_t WRITE_DATA4;   /*!< WRITE_DATA4   Address offset: 0x2C      */
  __IO uint32_t WRITE_DATA3;   /*!< WRITE_DATA3   Address offset: 0x30      */
  __IO uint32_t WRITE_DATA2;   /*!< WRITE_DATA2   Address offset: 0x34      */
  __IO uint32_t WRITE_DATA1;   /*!< WRITE_DATA1   Address offset: 0x38      */
  __IO uint32_t WRITE_DATA0;   /*!< WRITE_DATA0   Address offset: 0x3C      */
       uint32_t RESERVED1[3];  /*!< Reserved      Address offset: 0x40-0x4B */
  __IO uint32_t READ_DATA4;    /*!< READ_DATA4    Address offset: 0x4C      */
  __IO uint32_t READ_DATA3;    /*!< READ_DATA3    Address offset: 0x50      */
  __IO uint32_t READ_DATA2;    /*!< READ_DATA2    Address offset: 0x54      */
  __IO uint32_t READ_DATA1;    /*!< READ_DATA1    Address offset: 0x58      */
  __IO uint32_t READ_DATA0;    /*!< READ_DATA0    Address offset: 0x5C      */
} IMA_TypeDef;


/*******************  Bit definition for IMA_CTRL register  *******************/
#define IMA_CTRL_READ_Pos                (0U)
#define IMA_CTRL_READ_Msk                (0x1UL << IMA_CTRL_READ_Pos)  /*!< 0x00000001 */
#define IMA_CTRL_READ                    IMA_CTRL_READ_Msk             /*!< </spirit:description> */

/*******************  Bit definition for IMA_ENABLE register  *****************/
#define IMA_ENABLE_EN_Pos                (0U)
#define IMA_ENABLE_EN_Msk                (0x1UL << IMA_ENABLE_EN_Pos)  /*!< 0x00000001 */
#define IMA_ENABLE_EN                    IMA_ENABLE_EN_Msk             /*!< </spirit:description> */

/*******************  Bit definition for IMA_STATUS register  *****************/
#define IMA_STATUS_READ_LOCK_Pos         (0U)
#define IMA_STATUS_READ_LOCK_Msk         (0x1UL << IMA_STATUS_READ_LOCK_Pos)   /*!< 0x00000001 */
#define IMA_STATUS_READ_LOCK             IMA_STATUS_READ_LOCK_Msk              /*!< </spirit:description> */
#define IMA_STATUS_WRITE_LOCK_Pos        (8U)
#define IMA_STATUS_WRITE_LOCK_Msk        (0x1UL << IMA_STATUS_WRITE_LOCK_Pos)  /*!< 0x00000100 */
#define IMA_STATUS_WRITE_LOCK            IMA_STATUS_WRITE_LOCK_Msk             /*!< </spirit:description> */

/*******************  Bit definition for IMA_SLCT register  *******************/
#define IMA_SLCT_ARRAY_SLCT_Pos          (0U)
#define IMA_SLCT_ARRAY_SLCT_Msk          (0x3FUL << IMA_SLCT_ARRAY_SLCT_Pos)  /*!< 0x0000003F */
#define IMA_SLCT_ARRAY_SLCT              IMA_SLCT_ARRAY_SLCT_Msk              /*!< </spirit:description> */
#define IMA_SLCT_ROW_SLCT_Pos            (16U)
#define IMA_SLCT_ROW_SLCT_Msk            (0x7FFFUL << IMA_SLCT_ROW_SLCT_Pos)  /*!< 0x7FFF0000 */
#define IMA_SLCT_ROW_SLCT                IMA_SLCT_ROW_SLCT_Msk                /*!< </spirit:description> */

/*******************  Bit definition for WRITE_UNLOCK register  ***************/
#define IMA_WRITE_UNLOCK_WRITE_KEY_Pos   (0U)
#define IMA_WRITE_UNLOCK_WRITE_KEY_Msk   (0xFFFFFFFFUL << IMA_WRITE_UNLOCK_WRITE_KEY_Pos)  /*!< 0xFFFFFFFF */
#define IMA_WRITE_UNLOCK_WRITE_KEY       IMA_WRITE_UNLOCK_WRITE_KEY_Msk                    /*!< </spirit:description> */

/*******************  Bit definition for READ_UNLOCK register  ****************/
#define IMA_READ_UNLOCK_READ_KEY_Pos     (0U)
#define IMA_READ_UNLOCK_READ_KEY_Msk     (0xFFFFFFFFUL << IMA_READ_UNLOCK_READ_KEY_Pos)  /*!< 0xFFFFFFFF */
#define IMA_READ_UNLOCK_READ_KEY         IMA_READ_UNLOCK_READ_KEY_Msk                    /*!< </spirit:description> */

/*******************  Bit definition for WRITE_DATA4 register  ****************/
#define IMA_WRITE_DATA4_WRITE_Pos        (0U)
#define IMA_WRITE_DATA4_WRITE_Msk        (0xFFFFFFFFUL << IMA_WRITE_DATA4_WRITE_Pos)  /*!< 0xFFFFFFFF */
#define IMA_WRITE_DATA4_WRITE            IMA_WRITE_DATA4_WRITE_Msk                    /*!< </spirit:description> */

/*******************  Bit definition for WRITE_DATA3 register  ****************/
#define IMA_WRITE_DATA3_WRITE_Pos        (0U)
#define IMA_WRITE_DATA3_WRITE_Msk        (0xFFFFFFFFUL << IMA_WRITE_DATA3_WRITE_Pos)  /*!< 0xFFFFFFFF */
#define IMA_WRITE_DATA3_WRITE            IMA_WRITE_DATA3_WRITE_Msk                    /*!< </spirit:description> */

/*******************  Bit definition for WRITE_DATA2 register  ****************/
#define IMA_WRITE_DATA2_WRITE_Pos        (0U)
#define IMA_WRITE_DATA2_WRITE_Msk        (0xFFFFFFFFUL << IMA_WRITE_DATA2_WRITE_Pos)  /*!< 0xFFFFFFFF */
#define IMA_WRITE_DATA2_WRITE            IMA_WRITE_DATA2_WRITE_Msk                    /*!< </spirit:description> */

/*******************  Bit definition for WRITE_DATA1 register  ****************/
#define IMA_WRITE_DATA1_WRITE_Pos        (0U)
#define IMA_WRITE_DATA1_WRITE_Msk        (0xFFFFFFFFUL << IMA_WRITE_DATA1_WRITE_Pos)  /*!< 0xFFFFFFFF */
#define IMA_WRITE_DATA1_WRITE            IMA_WRITE_DATA1_WRITE_Msk                    /*!< </spirit:description> */

/*******************  Bit definition for WRITE_DATA0 register  ****************/
#define IMA_WRITE_DATA0_WRITE_Pos        (0U)
#define IMA_WRITE_DATA0_WRITE_Msk        (0xFFFFFFFFUL << IMA_WRITE_DATA0_WRITE_Pos)  /*!< 0xFFFFFFFF */
#define IMA_WRITE_DATA0_WRITE            IMA_WRITE_DATA0_WRITE_Msk                    /*!< </spirit:description> */

/*******************  Bit definition for READ_DATA4 register  *****************/
#define IMA_READ_DATA4_READ_Pos          (0U)
#define IMA_READ_DATA4_READ_Msk          (0xFFFFFFFFUL << IMA_READ_DATA4_READ_Pos)  /*!< 0xFFFFFFFF */
#define IMA_READ_DATA4_READ              IMA_READ_DATA4_READ_Msk                    /*!< </spirit:description> */

/*******************  Bit definition for READ_DATA3 register  *****************/
#define IMA_READ_DATA3_READ_Pos          (0U)
#define IMA_READ_DATA3_READ_Msk          (0xFFFFFFFFUL << IMA_READ_DATA3_READ_Pos)  /*!< 0xFFFFFFFF */
#define IMA_READ_DATA3_READ              IMA_READ_DATA3_READ_Msk                    /*!< </spirit:description> */

/*******************  Bit definition for READ_DATA2 register  *****************/
#define IMA_READ_DATA2_READ_Pos          (0U)
#define IMA_READ_DATA2_READ_Msk          (0xFFFFFFFFUL << IMA_READ_DATA2_READ_Pos)  /*!< 0xFFFFFFFF */
#define IMA_READ_DATA2_READ              IMA_READ_DATA2_READ_Msk                    /*!< </spirit:description> */

/*******************  Bit definition for READ_DATA1 register  *****************/
#define IMA_READ_DATA1_READ_Pos          (0U)
#define IMA_READ_DATA1_READ_Msk          (0xFFFFFFFFUL << IMA_READ_DATA1_READ_Pos)  /*!< 0xFFFFFFFF */
#define IMA_READ_DATA1_READ              IMA_READ_DATA1_READ_Msk                    /*!< </spirit:description> */

/*******************  Bit definition for READ_DATA0 register  *****************/
#define IMA_READ_DATA0_READ_Pos          (0U)
#define IMA_READ_DATA0_READ_Msk          (0xFFFFFFFFUL << IMA_READ_DATA0_READ_Pos)  /*!< 0xFFFFFFFF */
#define IMA_READ_DATA0_READ              IMA_READ_DATA0_READ_Msk                    /*!< </spirit:description> */

#define IMA_BASE            0x4000F000UL 

#define IMA                 ((IMA_TypeDef *) IMA_BASE) 


#endif /* __SR5E1_IMA_H */

