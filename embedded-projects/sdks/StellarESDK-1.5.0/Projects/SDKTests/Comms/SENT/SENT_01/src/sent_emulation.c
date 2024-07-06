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

#include <sent_emulation.h>

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

/* SENT signals buffer.*/
static uint8_t sent_emul_buffer[SENT_BUFFER_SIZE];

static gpio_pin_t pin;
static uint32_t source, destination;

/*===========================================================================*/
/* Local function prototypes                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Returns address of register ODR related to the pin used for the
 *          SENT emulation
 *
 * @param[in] gpio      unique pin identifier related to the pin used for the
 *                      SENT emulation
 * @return              address of register ODR related to the pin used for the
 *                      SENT emulation.
 *
 * @notapi
 */
static uint32_t sent_emul_get_per_addr(gpio_io_t gpio) {

    gpio_port_t port;
    port = gpio_iounpack_port(gpio);

    if (port == GPIO_PORT_A) {
        return (uint32_t)(&(GPIOA->ODR));
    } else if (port == GPIO_PORT_B) {
        return (uint32_t)(&(GPIOB->ODR));
    } else if (port == GPIO_PORT_C) {
        return (uint32_t)(&(GPIOC->ODR));
    } else if (port == GPIO_PORT_D) {
        return (uint32_t)(&(GPIOD->ODR));
    } else if (port == GPIO_PORT_E) {
        return (uint32_t)(&(GPIOE->ODR));
    } else if (port == GPIO_PORT_F) {
        return (uint32_t)(&(GPIOF->ODR));
    } else if (port == GPIO_PORT_G) {
        return (uint32_t)(&(GPIOG->ODR));
    } else if (port == GPIO_PORT_H) {
        return (uint32_t)(&(GPIOH->ODR));
    } else {
        return (uint32_t)(&(GPIOI->ODR));
    }
}

/**
 * @brief   Final pulse creation
 *
 * @param[in] len       current length
 * @return              updated length
 *
 * @notapi
 */
static uint32_t sent_emul_pulse(uint32_t len) {

    uint8_t i;

    for (i = 0; i < 5; i++) {
        sent_emul_buffer[len++] = SENT_SIGNAL_DEASSERT;
    }

    /* Set signal high */
    sent_emul_buffer[len++] = (SENT_SIGNAL_ASSERT << pin);

    return len;
}

/**
 * @brief   Sync/calibration pulse creation
 *
 * @param[in] len       current length
 * @return              updated length
 *
 * @notapi
 */
static uint32_t sent_emul_pulse_calibration(uint32_t len) {

    uint8_t i;

    for (i = 0; i < 5; i++) {
        sent_emul_buffer[len++] = SENT_SIGNAL_DEASSERT;
    }

    for (i = 0; i < 51; i++) {
        sent_emul_buffer[len++] = (SENT_SIGNAL_ASSERT << pin);
    }

    return len;
}

/**
 * @brief   Data pulse creation
 *
 * @param[in] len       current length
 * @param[in] nibble    number of high ticks to configure
 * @return              updated length
 *
 * @notapi
 */
static uint32_t sent_emul_pulse_data(uint32_t len, uint8_t nibble) {

    uint8_t i;

    nibble &= 0x0F;

    for (i = 0U; i < 5U; i++) {
        sent_emul_buffer[len++] = SENT_SIGNAL_DEASSERT;
    }

    for (i = 0U; i < (nibble + (12UL - 5UL)); i++) {
        sent_emul_buffer[len++] = (SENT_SIGNAL_ASSERT << pin);
    }

    return len;
}

/**
 * @brief   CRC pulse calculation
 *
 * @param[in] data      data on which perform the calculation
 * @param[in] len       current length
 * @return              crc value
 *
 * @notapi
 */
static uint8_t sent_emul_crc_calculate(uint32_t data, uint8_t len) {

    static const uint8_t crc_table[] = {0, 13, 7, 10, 14, 3, 9, 4,
                                        1, 12, 6, 11, 15, 2, 8, 5};
    uint8_t crc;
    uint8_t i;

    /* Start to calculate from seed.*/
    crc = 5U;

    for (i = 0; i < len; i++) {
        crc = ((((data) >> ((len - i - 1U) * 4U)) & 0x0F) ^ crc_table[crc]) & 0x0F;
    }

    return crc_table[crc];
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void sent_emul_config(gpio_io_t gpio, tim_driver_t *tdp,
                      uint32_t dma_stream_id, tim_cb_t dma_callback) {

    tim_dma_conf_t tim_dma_ch1_conf;

    /* Initialize TIM driver instance.*/
    tim_init(tdp);

    /* Configure TIM driver (output frequency 1MHz).*/
    (void)tim_set_presc(tdp, TIM_DIVIDE_BY(tim_get_clock(tdp) / 100000000U));
    (void)tim_set_ch_mode(tdp, 1U, TIM_MODE_PWM_ACTIVE_HIGH);

    (void)tim_set_cb(tdp, TIM_CB_OPS_DMA_CC1, dma_callback);


    /* Apply TIM configuration.*/
    tim_start(tdp);

    /* Configure TIM to obtain a period of 10us (100KHz).*/
    tim_set_interval(tdp, 1000U);       /* 100MHz/1000 = 100KHz.  */
    tim_set_comparator(tdp, 1U, 500U);  /* Duty cycle 50%.        */

    /* Enable TIM.*/
    tim_enable(tdp);

    /* Configure channel 1 DMA.*/
    tim_dma_ch1_conf.dma_stream_id = dma_stream_id;
    tim_dma_ch1_conf.dma_stream_bus_prio = DMA_PRIORITY_MAX;
    tim_dma_ch1_conf.dma_stream_irq_prio = IRQ_PRIORITY_10;
    tim_set_dma_conf(tdp, TIM_DMA_OPS_CH1, &tim_dma_ch1_conf);

    /* Get pin used for SENT emulation.*/
    pin = gpio_iounpack_pin(gpio);

    /* Get DMA source address.*/
    source = sent_emul_get_buffer_addr();

    /* Get DMA destination address.*/
    destination = sent_emul_get_per_addr(gpio);
}

uint32_t sent_emul_get_buffer_addr(void) {

    return (uint32_t)(&sent_emul_buffer);
}

uint32_t sent_emul_new_frame(uint8_t status, uint32_t data, uint8_t nibbles) {

    uint8_t i;
    uint8_t crc;
    uint32_t len;
    uint32_t value;

    /* Start filling output buffer from the beginning.*/
    len = 0;

    /* Sync/calibration.*/
    len = sent_emul_pulse_calibration(len);

    /* Status nibble.*/
    len = sent_emul_pulse_data(len, status);

    /* Data nibbles.*/
    value = data;
    for (i = 0; i < nibbles; i++) {
        data = (value >> ((nibbles - i - 1U) * 4U)) & 0x0F;
        len = sent_emul_pulse_data(len, (uint8_t)(data));
    }

    /* CRC nibble.*/
    crc = sent_emul_crc_calculate(value, 6);
    len = sent_emul_pulse_data(len, crc);

    /* Last pulse.*/
    len = sent_emul_pulse(len);

    return len;
}

void sent_emul_transmit(tim_driver_t *tdp, uint32_t len) {

    tim_enable_channel_dma(tdp, 1U, TIM_DMA_DIR_M2P, TIM_DMA_SIZE_BYTE,
                           TIM_DMA_CIRC_DISABLED, source, len, destination);
}

void sent_emul_stop(tim_driver_t *tdp) {

    tim_disable_channel_dma(tdp, 1U);
}

/** @} */
