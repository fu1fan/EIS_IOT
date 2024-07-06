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
 *****************************************************************************/
/**
 * @file    dac.c
 * @brief   DAC driver source file.
 *
 * @addtogroup DRIVERS
 * @addtogroup ANALOG
 * @ingroup DRIVERS
 * @addtogroup DAC
 * @ingroup ANALOG
 * @{
 */

#include <dac.h>
#include <dac_private.h>
#include <osal.h>

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Local function prototypes                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Resets DAC parameters.
 *
 * @param[out] ddp      pointer to an @p dac_driver_t structure
 *
 * @notapi
 */
static void dac_reset_paramters(dac_driver_t *ddp) {
    
    uint8_t i;

    /* Set all driver configuration parameters to default values.*/
    ddp->prio                                = IRQ_MIN_PRIORITY;
    ddp->hf_mode                             = DAC_HF_MODE_OVER_160MHZ;
    ddp->drv_mode                            = DAC_DVR_MODE_SINGLE;
    ddp->cb                                  = NULL;
    for (i = 0U; i < DAC_CHANNEL_NUM; i++) {
        ddp->is_channel_en[i]                = false;
        ddp->is_calibration_en[i]            = false;
        ddp->data_format[i]                  = DAC_DATA_12BIT_RIGHT_ALIGN;
        ddp->trigger_src[i]                  = DAC_SWTRIG;
        ddp->inc_trigger_src[i]              = DAC_INC_SWTRIGB;
        ddp->rst_trigger_src[i]              = DAC_RST_SWTRIGB;
        ddp->is_trigger_en[i]                = false;
        ddp->wave_mode[i]                    = DAC_WAVE_MODE_DISABLE;
        ddp->out_con_mode[i]                 = DAC_OUT_CON_MODE_NORM_PIN_BUF_EN;
        ddp->dma_mode[i]                     = DAC_DMA_OFF;
        ddp->dma_conf[i].dma_stream_id       = DMA1_STREAM0_ID;
        ddp->dma_conf[i].dma_stream_bus_prio = DMA_PRIORITY_LOW;
        ddp->dma_conf[i].dma_stream_irq_prio = IRQ_MIN_PRIORITY;
        ddp->dma_descriptor[i]               = NULL;
    }
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void dac_init(dac_driver_t *ddp) {

    /* Set pointer to DAC register block.*/
    ddp->dac = dac_dev_get_reg_ptr(ddp);

    /* Reset DAC parameters.*/
    dac_reset_paramters(ddp);
}

uint32_t dac_set_prio(dac_driver_t *ddp, uint32_t prio) {

    uint32_t prev_prio;

    prev_prio = ddp->prio;
    ddp->prio = prio;

    return prev_prio;
}

bool dac_enable_channel(dac_driver_t *ddp, dac_channel_t channel, bool enable){

    bool prev_is_channel_en;

    prev_is_channel_en = ddp->is_channel_en[channel];
    ddp->is_channel_en[channel] = enable;

    return prev_is_channel_en;
}

dac_hf_mode_t dac_set_highfreq_mode(dac_driver_t *ddp, dac_hf_mode_t hf_mode) {

    dac_hf_mode_t prev_hf_mode;

    prev_hf_mode = ddp->hf_mode;
    ddp->hf_mode = hf_mode;

    return prev_hf_mode;
}

bool dac_enable_calibration(dac_driver_t *ddp, dac_channel_t channel, 
                            bool enable) {

    bool prev_is_calibration_en;

    prev_is_calibration_en = ddp->is_calibration_en[channel];
    ddp->is_calibration_en[channel] = enable;

    return prev_is_calibration_en;
}

dac_data_format_t dac_set_format(dac_driver_t *ddp, dac_channel_t channel,
                                            dac_data_format_t data_format) {

    dac_data_format_t prev_data_format;

    prev_data_format = ddp->data_format[channel];
    ddp->data_format[channel] = data_format;

    return prev_data_format;
}

dac_trigger_src_t dac_set_trigger_src(dac_driver_t *ddp, dac_channel_t channel,
                                      dac_trigger_src_t trigger_src) {

    dac_trigger_src_t prev_trigger_src;

    prev_trigger_src = ddp->trigger_src[channel];
    ddp->trigger_src[channel] = trigger_src; 

    return prev_trigger_src;
}

dac_inc_trigger_src_t dac_set_inc_trigger_src(dac_driver_t *ddp,
                                              dac_channel_t channel,
                                              dac_inc_trigger_src_t inc_trigger_src) {

    dac_inc_trigger_src_t prev_inc_trigger_src;

    prev_inc_trigger_src = ddp->inc_trigger_src[channel];
    ddp->inc_trigger_src[channel] = inc_trigger_src; 

    return prev_inc_trigger_src;
}

dac_rst_trigger_src_t dac_set_rst_trigger_src(dac_driver_t *ddp,
                                              dac_channel_t channel,
                                              dac_rst_trigger_src_t rst_trigger_src) {

    dac_rst_trigger_src_t prev_rst_trigger_src;

    prev_rst_trigger_src = ddp->rst_trigger_src[channel];
    ddp->rst_trigger_src[channel] = rst_trigger_src; 

    return prev_rst_trigger_src;
}

bool dac_enable_trigger(dac_driver_t *ddp, dac_channel_t channel, bool enable) {

    bool prev_is_trigger_en;

    prev_is_trigger_en = ddp->is_trigger_en[channel];
    ddp->is_trigger_en[channel] = enable; 

    return prev_is_trigger_en;
}

dac_drv_mode_t dac_set_mode(dac_driver_t *ddp, dac_drv_mode_t drv_mode){

    dac_drv_mode_t prev_drv_mode;

    prev_drv_mode = ddp->drv_mode;
    ddp->drv_mode = drv_mode; 

    return prev_drv_mode;
}

dac_wave_mode_t dac_set_wave(dac_driver_t *ddp, dac_channel_t channel,
                             dac_wave_mode_t wave_mode) {

    dac_wave_mode_t prev_wave_mode;

    prev_wave_mode = ddp->wave_mode[channel];
    ddp->wave_mode[channel] = wave_mode;

    return prev_wave_mode;
}

dac_out_con_mode_t dac_set_output_connection(dac_driver_t *ddp,
                                             dac_channel_t channel,
                                             dac_out_con_mode_t out_con_mode) {

    dac_out_con_mode_t pre_out_con_mode;

    pre_out_con_mode = ddp->out_con_mode[channel];
    ddp->out_con_mode[channel] = out_con_mode; 

    return pre_out_con_mode;
}

dac_dma_mode_t dac_set_dma_mode(dac_driver_t *ddp, dac_channel_t channel,
                                dac_dma_mode_t dma_mode) {

    dac_dma_mode_t prev_dma_mode;

    prev_dma_mode = ddp->dma_mode[channel];
    ddp->dma_mode[channel] = dma_mode;

    return prev_dma_mode;
}

void dac_set_dma_conf(dac_driver_t *ddp, dac_channel_t channel,
                      dac_dma_conf_t *dma_conf) {

    ddp->dma_conf[channel].dma_stream_id       = dma_conf->dma_stream_id;
    ddp->dma_conf[channel].dma_stream_bus_prio = dma_conf->dma_stream_bus_prio;
    ddp->dma_conf[channel].dma_stream_irq_prio = dma_conf->dma_stream_irq_prio;
}

dac_cb_t dac_set_cb(dac_driver_t *ddp, dac_cb_t cb) {

    dac_cb_t prev_cb;

    prev_cb = ddp->cb;
    ddp->cb = cb;

    return prev_cb;
}

void dac_start(dac_driver_t *ddp) {

    IRQn_Type vector;
    uint32_t dma_ch1_mode, dma_ch2_mode;
    uint32_t dma_ch1_trigger, dma_ch2_trigger;

    /* Enable clock.*/
    dac_dev_clock_enable(ddp);

    if ((ddp->dma_mode[DAC_CHANNEL_1] != DAC_DMA_OFF) ||
        (ddp->dma_mode[DAC_CHANNEL_2] != DAC_DMA_OFF)) {
        /* Configure DAC interrupt vector.*/
        vector = dac_dev_get_vector(ddp);
        irq_set_priority(vector, ddp->prio);
        irq_enable(vector);
    }

    /* Set DMA mode for channel 1.*/
    if ((ddp->dma_mode[DAC_CHANNEL_1] == DAC_DMA_ON) || (ddp->dma_mode[DAC_CHANNEL_1] == DAC_DMA_DD_ON)){
        ddp->dac->CR |= DAC_CR_DMAUDRIE1 | DAC_CR_DMAEN1;

        if((ddp->dma_mode[DAC_CHANNEL_1] == DAC_DMA_DD_ON)){
             ddp->dac->MCR |= DAC_MCR_DMADOUBLE1;
        }

        /* Allocates DMA streams for DAC channel 1.*/
        /*lint -e9087 */
        ddp->dma_descriptor[DAC_CHANNEL_1] = dma_stream_take(ddp->dma_conf[DAC_CHANNEL_1].dma_stream_id,
                                                             ddp->dma_conf[DAC_CHANNEL_1].dma_stream_irq_prio,
                                                             NULL,
                                                             (void *)ddp);

        /*lint +e9087 */

        /* Configure DMA stream peripheral addresses.*/
        if (ddp->data_format[DAC_CHANNEL_1] == DAC_DATA_8BIT) {
            dma_stream_set_peripheral(ddp->dma_descriptor[DAC_CHANNEL_1], (uint32_t)(&ddp->dac->DHR8R1));
        } else if (ddp->data_format[DAC_CHANNEL_1] == DAC_DATA_12BIT_LEFT_ALIGN) {
            dma_stream_set_peripheral(ddp->dma_descriptor[DAC_CHANNEL_1], (uint32_t)(&ddp->dac->DHR12L1));
        } else {
            dma_stream_set_peripheral(ddp->dma_descriptor[DAC_CHANNEL_1], (uint32_t)(&ddp->dac->DHR12R1));
        }

        /* Configure DMA trigger.*/
        dma_ch1_trigger = dac_dev_get_dma_ch1_trigger(ddp);
        dma_stream_set_trigger(ddp->dma_descriptor[DAC_CHANNEL_1], dma_ch1_trigger);
     
        /* Configure DMA priority level, data transfer direction and enable
           transfer complete interrupt.*/
        dma_ch1_mode = DMA_CCR_PL_VALUE(ddp->dma_conf[DAC_CHANNEL_1].dma_stream_bus_prio) |
                       DMA_CCR_DIR_M2P | DMA_CCR_TCIE;

        /* Configure DMA memory increment mode.*/
        dma_ch1_mode |= DMA_CCR_MINC;
	
        /* Enable DMA circular mode.*/
        dma_ch1_mode |= DMA_CCR_CIRC;

        /* Configure DMA frame size.*/
        dma_ch1_mode |= DMA_CCR_PSIZE_WORD | DMA_CCR_MSIZE_WORD;

        /* Set up calculated DMA modes.*/
        dma_stream_set_transfer_mode(ddp->dma_descriptor[DAC_CHANNEL_1], dma_ch1_mode);
    }

    /* Set DMA mode for channel 2.*/
    if ((ddp->dma_mode[DAC_CHANNEL_2] == DAC_DMA_ON) || (ddp->dma_mode[DAC_CHANNEL_2] == DAC_DMA_DD_ON)){
        ddp->dac->CR |= DAC_CR_DMAUDRIE2 | DAC_CR_DMAEN2; 

        if((ddp->dma_mode[DAC_CHANNEL_2] == DAC_DMA_DD_ON)){
             ddp->dac->MCR |= DAC_MCR_DMADOUBLE2;
        }

        /* Allocates DMA streams for DAC channel 2.*/
        /*lint -e9087 */
        ddp->dma_descriptor[DAC_CHANNEL_2] = dma_stream_take(ddp->dma_conf[DAC_CHANNEL_2].dma_stream_id,
                                                             ddp->dma_conf[DAC_CHANNEL_2].dma_stream_irq_prio,
                                                             NULL,
                                                             (void *)ddp);
        /*lint +e9087 */

        /* Configure DMA stream peripheral addresses.*/
        if (ddp->data_format[DAC_CHANNEL_2] == DAC_DATA_8BIT) {
            dma_stream_set_peripheral(ddp->dma_descriptor[DAC_CHANNEL_2], (uint32_t)(&ddp->dac->DHR8R2));
        } else if (ddp->data_format[DAC_CHANNEL_2] == DAC_DATA_12BIT_LEFT_ALIGN) {
            dma_stream_set_peripheral(ddp->dma_descriptor[DAC_CHANNEL_2], (uint32_t)(&ddp->dac->DHR12L2));
        } else {
            dma_stream_set_peripheral(ddp->dma_descriptor[DAC_CHANNEL_2], (uint32_t)(&ddp->dac->DHR12R2));
        }

        /* Configure DMA trigger.*/
        dma_ch2_trigger = dac_dev_get_dma_ch2_trigger(ddp);
        dma_stream_set_trigger(ddp->dma_descriptor[DAC_CHANNEL_2], dma_ch2_trigger);
      
        /* Configure DMA priority level, data transfer direction and enable
           transfer complete interrupt.*/
        dma_ch2_mode = DMA_CCR_PL_VALUE(ddp->dma_conf[DAC_CHANNEL_2].dma_stream_bus_prio) |
                       DMA_CCR_DIR_M2P | DMA_CCR_TCIE;

        /* Configure DMA memory increment mode.*/
        dma_ch2_mode |= DMA_CCR_MINC;

        /* Enable DMA circular mode.*/
        dma_ch2_mode |= DMA_CCR_CIRC;

        /* Configure DMA frame size.*/
        dma_ch2_mode |= DMA_CCR_PSIZE_WORD | DMA_CCR_MSIZE_WORD;

        /* Set up calculated DMA modes.*/
        dma_stream_set_transfer_mode(ddp->dma_descriptor[DAC_CHANNEL_2], dma_ch2_mode);
    }

    /* Set DAC high frequency mode.*/
    ddp->dac->MCR |= (((uint32_t)ddp->hf_mode) << DAC_MCR_HFSEL_Pos);

    /* Set DAC wave mode.*/
    ddp->dac->CR |= (((uint32_t)ddp->wave_mode[DAC_CHANNEL_1]) << DAC_CR_WAVE1_Pos);
    ddp->dac->CR |= (((uint32_t)ddp->wave_mode[DAC_CHANNEL_2]) << DAC_CR_WAVE2_Pos);

    /* Set trigger enable.*/
    if (ddp->is_trigger_en[DAC_CHANNEL_1] == true) {
         ddp->dac->CR |= (1UL << DAC_CR_TEN1_Pos);
    } else {
         ddp->dac->CR |= (0UL << DAC_CR_TEN1_Pos);
    }
    if (ddp->is_trigger_en[DAC_CHANNEL_2] == true) {
         ddp->dac->CR |= (1UL << DAC_CR_TEN2_Pos);
    } else {
         ddp->dac->CR |= (0UL << DAC_CR_TEN2_Pos);
    }

    /* Set trigger source.*/
    ddp->dac->CR |= (((uint32_t)ddp->trigger_src[DAC_CHANNEL_1]) << DAC_CR_TSEL1_Pos);
    ddp->dac->CR |= (((uint32_t)ddp->trigger_src[DAC_CHANNEL_2]) << DAC_CR_TSEL2_Pos);

    /* Set increment and reset trigger source.*/
    ddp->dac->STMODR |= (((uint32_t)ddp->inc_trigger_src[DAC_CHANNEL_1]) << DAC_STMODR_STINCTRIGSEL1_Pos);
    ddp->dac->STMODR |= (((uint32_t)ddp->inc_trigger_src[DAC_CHANNEL_2]) << DAC_STMODR_STINCTRIGSEL2_Pos);
    ddp->dac->STMODR |= (((uint32_t)ddp->rst_trigger_src[DAC_CHANNEL_1]) << DAC_STMODR_STRSTTRIGSEL1_Pos);
    ddp->dac->STMODR |= (((uint32_t)ddp->rst_trigger_src[DAC_CHANNEL_2]) << DAC_STMODR_STRSTTRIGSEL2_Pos);
    
    /* Set channel output connection.*/
    ddp->dac->MCR |= (((uint32_t)ddp->out_con_mode[DAC_CHANNEL_1]) << DAC_MCR_MODE1_Pos);
    ddp->dac->MCR |= (((uint32_t)ddp->out_con_mode[DAC_CHANNEL_2]) << DAC_MCR_MODE2_Pos);

    /* DAC single mode.*/
    if (ddp->drv_mode == DAC_DVR_MODE_SINGLE) {
        /* Configure DAC registers based on DAC parameters.*/
        if (ddp->is_channel_en[DAC_CHANNEL_1] == true) {
            /*Enable DAC_CHANNEL_1.*/
            ddp->dac->CR |= DAC_CR_EN1;
        }
        if (ddp->is_channel_en[DAC_CHANNEL_2] == true) {
            /* enable DAC_CHANNEL_2.*/
            ddp->dac->CR |= DAC_CR_EN2;
        }
    /* DAC dual mode or double data mode.*/
    } else {
        /* Enable both DAC channels.*/
        ddp->dac->CR |= DAC_CR_EN1 | DAC_CR_EN2;
    }
}

void dac_set_value(dac_driver_t *ddp, dac_channel_t channel, uint32_t data1,
                   uint32_t data2) {

    /* Single mode value setting.*/
    if (ddp->drv_mode == DAC_DVR_MODE_SINGLE) {
        /* Set channel 1 value.*/ 
        if (channel == DAC_CHANNEL_1) {
            if (ddp->data_format[DAC_CHANNEL_1] == DAC_DATA_8BIT) {
                ddp->dac->DHR8R1 = data1 & 0xFFUL;
            } else if (ddp->data_format[DAC_CHANNEL_1] == DAC_DATA_12BIT_LEFT_ALIGN) {
                ddp->dac->DHR12L1 = (data1 << DAC_DHR12L1_DACC1DHR_Pos) & 0xFFF0UL;
            } else {
                ddp->dac->DHR12R1 = data1 & 0xFFFUL;
            }
        /* Set channel 2 value.*/ 
        } else {
            if (ddp->data_format[DAC_CHANNEL_2] == DAC_DATA_8BIT){
                ddp->dac->DHR8R2 = data1 & 0xFFUL;
            } else if (ddp->data_format[DAC_CHANNEL_2] == DAC_DATA_12BIT_LEFT_ALIGN) {
                ddp->dac->DHR12L2 = (data1 << DAC_DHR12L2_DACC2DHR_Pos) & 0xFFF0UL;
            } else {
                ddp->dac->DHR12R2 = data1 & 0xFFFUL;
            }
        }
    /* Double data mode value setting.*/
    } else if (ddp->drv_mode == DAC_DVR_MODE_DOUBLE_DATA) {
        /* Set channel 1 value.*/ 
        if (channel == DAC_CHANNEL_1) {
            if (ddp->data_format[DAC_CHANNEL_1] == DAC_DATA_8BIT) {
                ddp->dac->DHR8R1 = data1 | (data2 << DAC_DHR8R1_DACC1DHRB_Pos);
            } else if (ddp->data_format[DAC_CHANNEL_1] == DAC_DATA_12BIT_LEFT_ALIGN) {
                ddp->dac->DHR12L1 = (data1 << DAC_DHR12L1_DACC1DHR_Pos) | (data2 << DAC_DHR12L1_DACC1DHRB_Pos);
            } else {
                ddp->dac->DHR12R1 = data1 | (data2 << DAC_DHR12R1_DACC1DHRB_Pos);
            }
        /* Set channel 2 value.*/ 
        } else {
            if (ddp->data_format[DAC_CHANNEL_2] == DAC_DATA_8BIT) {
                ddp->dac->DHR8R2 = data1 | (data2 << DAC_DHR8R2_DACC2DHRB_Pos);
            } else if (ddp->data_format[DAC_CHANNEL_2] == DAC_DATA_12BIT_LEFT_ALIGN) {
                ddp->dac->DHR12L2 = (data1 << DAC_DHR12L2_DACC2DHR_Pos) | (data2 << DAC_DHR12L2_DACC2DHRB_Pos);
            } else {
                ddp->dac->DHR12R2 = data1 | (data2 << DAC_DHR12R2_DACC2DHRB_Pos);
            }
        }
    /* Dual mode value setting.*/
    } else{
        /* Set channel dual DAC value.*/ 
        if (ddp->data_format[DAC_CHANNEL_1] == DAC_DATA_8BIT){
            ddp->dac->DHR8RD = data1 | data2 << DAC_DHR8RD_DACC2DHR_Pos;
        } else if(ddp->data_format[DAC_CHANNEL_1] == DAC_DATA_12BIT_LEFT_ALIGN) {
            ddp->dac->DHR12LD = (data1 << DAC_DHR12LD_DACC1DHR_Pos) | (data2 << DAC_DHR12LD_DACC2DHR_Pos);
        } else {
            ddp->dac->DHR12RD = (data1) | (data2 << DAC_DHR12RD_DACC2DHR_Pos);
        }
    }
}

void dac_set_dma_value(dac_driver_t *ddp, dac_channel_t channel, uint32_t *data,
                       uint32_t len) {

    if (channel == DAC_CHANNEL_1) {
        /* Configure DMA number of data items.*/
        dma_stream_set_count(ddp->dma_descriptor[DAC_CHANNEL_1], len);

        /* Configure DMA stream memory address.*/
        dma_stream_set_memory(ddp->dma_descriptor[DAC_CHANNEL_1], (uint32_t)(data));

        /* Enable DMA.*/
        dma_stream_enable(ddp->dma_descriptor[DAC_CHANNEL_1]);
    } else {
        /* Configure DMA number of data items.*/
        dma_stream_set_count(ddp->dma_descriptor[DAC_CHANNEL_2], len);

        /* Configure DMA stream memory address.*/
        dma_stream_set_memory(ddp->dma_descriptor[DAC_CHANNEL_2], (uint32_t)(data));

        /* Enable DMA. */
        dma_stream_enable(ddp->dma_descriptor[DAC_CHANNEL_2]);
	}
}

void dac_set_sw_trigger(dac_driver_t *ddp, dac_channel_t channel,
                        dac_sw_trigger_t sw_trigger) {

    if (sw_trigger == DAC_SW_TRIGGER_ENABLE) {
        if (channel == DAC_CHANNEL_1) {
            ddp->dac->SWTRIGR |= DAC_SWTRIGR_SWTRIG1;
        } else {
            ddp->dac->SWTRIGR |= DAC_SWTRIGR_SWTRIG2;
        }
    } else {
        if (channel == DAC_CHANNEL_1) {
            ddp->dac->SWTRIGR |= DAC_SWTRIGR_SWTRIGB1;
        } else {
            ddp->dac->SWTRIGR |= DAC_SWTRIGR_SWTRIGB2;
        }
    }
}

void dac_noise_generation(dac_driver_t *ddp, dac_channel_t channel, 
                          dac_lfsr_triangle_mask_t mask){

    if (channel == DAC_CHANNEL_1) {
        REG_SET_FIELD(ddp->dac->CR, DAC_CR_MAMP1, mask);
    } else {
        REG_SET_FIELD(ddp->dac->CR, DAC_CR_MAMP2, mask);
    }
}

void dac_triangle_generation(dac_driver_t *ddp, dac_channel_t channel, 
                             dac_lfsr_triangle_mask_t mask){

    if (channel == DAC_CHANNEL_1) {
        REG_SET_FIELD(ddp->dac->CR, DAC_CR_MAMP1, mask);
    } else {
        REG_SET_FIELD(ddp->dac->CR, DAC_CR_MAMP2, mask);
    }
}

void dac_sawtooth_generation(dac_driver_t *ddp, dac_channel_t channel,
                             uint32_t init_value, uint32_t inc_value,
                             uint8_t direction){

    if (channel == DAC_CHANNEL_1) {
        ddp->dac->STR1 = init_value | (inc_value << DAC_STR1_STINCDATA1_Pos) | (((uint32_t)direction) << DAC_STR1_STDIR1_Pos);
    } else {
        ddp->dac->STR2 = init_value | (inc_value << DAC_STR2_STINCDATA2_Pos) | (((uint32_t)direction) << DAC_STR2_STDIR2_Pos);
    }
}

uint32_t dac_user_trimming(dac_driver_t *ddp, dac_channel_t channel) {

    uint32_t trimmingvalue = 0UL;

    /* Select calibration or not.*/ 
    if ((ddp->is_calibration_en[channel] == true) && (channel == DAC_CHANNEL_1)){
        /* Disable the DAC_CHANNEL_1.*/
        ddp->dac->CR &= 0xFFFFFFFEUL;
        ddp->dac->MCR &= 0xFFFFFFF8UL;
        ddp->dac->CR |= DAC_CR_CEN1; 
        /* Start trimming.*/
        ddp->dac->CCR = trimmingvalue;
        while((ddp->dac->SR & DAC_SR_CAL_FLAG1) == 0U){

            osal_delay_millisec(1);
            ddp->dac->CCR = ++trimmingvalue;
        }
        ddp->dac->CR &= 0xFFFFBFFFUL;
    }

    if ((ddp->is_calibration_en[channel] == true) && (channel == DAC_CHANNEL_2)) {
        /* Disable the DAC_CHANNEL_2.*/
        ddp->dac->CR &= 0xFFFEFFFFUL;
        ddp->dac->MCR &= 0xFFF8FFFFUL;
        ddp->dac->CR |= DAC_CR_CEN2; 
        /* Start trimming.*/
        ddp->dac->CCR = (trimmingvalue << DAC_CCR_OTRIM2_Pos);
        while((ddp->dac->SR & DAC_SR_CAL_FLAG2) == 0U){
            osal_delay_millisec(1);
            ddp->dac->CCR = (++trimmingvalue << DAC_CCR_OTRIM2_Pos);
        }
        ddp->dac->CR &= 0xBFFFFFFFUL; 
    }

    return trimmingvalue;
}

void dac_stop(dac_driver_t *ddp) {

    IRQn_Type vector;

    /* Disable DAC interrupt.*/
    vector = dac_dev_get_vector(ddp);
    irq_disable(vector);

    /* If DMA is eanbled, free and disable related streams.*/
    if ((ddp->dma_mode[DAC_CHANNEL_1] == DAC_DMA_ON) || (ddp->dma_mode[DAC_CHANNEL_1] == DAC_DMA_DD_ON)){
        dma_stream_free(ddp->dma_descriptor[DAC_CHANNEL_1]);
        dma_stream_disable(ddp->dma_descriptor[DAC_CHANNEL_1]);
    }
    if ((ddp->dma_mode[DAC_CHANNEL_2] == DAC_DMA_ON) || (ddp->dma_mode[DAC_CHANNEL_2] == DAC_DMA_DD_ON)){
        dma_stream_free(ddp->dma_descriptor[DAC_CHANNEL_2]);
        dma_stream_disable(ddp->dma_descriptor[DAC_CHANNEL_2]);
    }

    /* Stop procedure. Set all registers to default values.*/
    ddp->dac->CR = 0U;
    ddp->dac->SWTRIGR = 0U;
    ddp->dac->MCR = 0U;

    /* Disable DAC clock.*/
    dac_dev_clock_disable(ddp);
}

void dac_deinit(dac_driver_t *ddp) {

    /* Set pointer to DAC register block to NULL.*/
    ddp->dac = NULL;

    /* Reset DAC parameters.*/
    dac_reset_paramters(ddp);
}

void __dac_serve_interrupt(dac_driver_t *ddp) {

    uint32_t isr, cr;

    /* Read and clear interrupt status register.*/
    isr = ddp->dac->SR;
    ddp->dac->SR = isr;

    /* One read on control register.*/
    cr = ddp->dac->CR;

    /* DMA under run interrupt.*/
    if((((isr & DAC_CR_DMAUDRIE1) != 0U) && ((cr & DAC_SR_DMAUDR1) != 0U)) ||
       (((isr & DAC_CR_DMAUDRIE2) != 0U) && ((cr & DAC_SR_DMAUDR2) != 0U))){

        /* If parity error callback is defined, it is invoked.*/
        if (ddp->cb != NULL) {
          ddp->cb(ddp);
        }
    }

    /* One write on control register.*/
    ddp->dac->CR = cr;
}

/** @} */
