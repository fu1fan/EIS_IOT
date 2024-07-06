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

#include <test_env.h>
#include <dma.h>
#include <spi.h>
#include <ili9341.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef EVBE3000P
#define SPI1_SCK gpio_iopack(GPIO_PORT_D, GPIO_PIN_15)
#define SPI1_SCK_CFG                             \
    (GPIO_MODE_MODER_ALTERNATE |                 \
     GPIO_MODE_OTYPER_PUSHPULL |                 \
     GPIO_MODE_OSPEEDR_HIGH |                    \
     GPIO_MODE_PUPDR_FLOATING |                  \
     GPIO_MODE_AFR(5U) |                         \
     GPIO_MODE_IHYSTR_CMOS |                     \
     GPIO_MODE_TRIGENR_IN_ENABLED |              \
     GPIO_MODE_SAFESELR_DISABLED |               \
     GPIO_MODE_SAFEVALR_LOW)

#define SPI1_MISO gpio_iopack(GPIO_PORT_E, GPIO_PIN_1)
#define SPI1_MISO_CFG                            \
    (GPIO_MODE_MODER_ALTERNATE |                 \
     GPIO_MODE_OTYPER_PUSHPULL |                 \
     GPIO_MODE_OSPEEDR_HIGH |                    \
     GPIO_MODE_PUPDR_FLOATING |                  \
     GPIO_MODE_AFR(5U) |                         \
     GPIO_MODE_IHYSTR_CMOS |                     \
     GPIO_MODE_TRIGENR_IN_ENABLED |              \
     GPIO_MODE_SAFESELR_DISABLED |               \
     GPIO_MODE_SAFEVALR_LOW)

#define SPI1_MOSI gpio_iopack(GPIO_PORT_E, GPIO_PIN_2)
#define SPI1_MOSI_CFG                            \
    (GPIO_MODE_MODER_ALTERNATE |                 \
     GPIO_MODE_OTYPER_PUSHPULL |                 \
     GPIO_MODE_OSPEEDR_HIGH |                    \
     GPIO_MODE_PUPDR_FLOATING |                  \
     GPIO_MODE_AFR(5U) |                         \
     GPIO_MODE_IHYSTR_CMOS |                     \
     GPIO_MODE_TRIGENR_IN_ENABLED |              \
     GPIO_MODE_SAFESELR_DISABLED |               \
     GPIO_MODE_SAFEVALR_LOW)

#define TFT_DC gpio_iopack(GPIO_PORT_I, GPIO_PIN_0)
#define TFT_LED_BACKLIGHT gpio_iopack(GPIO_PORT_I, GPIO_PIN_1)
#define TFT_RESET gpio_iopack(GPIO_PORT_I, GPIO_PIN_2)
#define TFT_GPIO_CFG                             \
    (GPIO_MODE_MODER_OUTPUT |                    \
     GPIO_MODE_OTYPER_PUSHPULL |                 \
     GPIO_MODE_OSPEEDR_LOW |                     \
     GPIO_MODE_PUPDR_FLOATING |                  \
     GPIO_MODE_AFR(0U) |                         \
     GPIO_MODE_IHYSTR_CMOS |                     \
     GPIO_MODE_TRIGENR_IN_ENABLED |              \
     GPIO_MODE_SAFESELR_DISABLED |               \
     GPIO_MODE_SAFEVALR_LOW)

__STATIC_INLINE void board_reinit(void) {

    gpio_set_pin_mode(SPI1_SCK, SPI1_SCK_CFG);
    gpio_set_pin_mode(SPI1_MISO, SPI1_MISO_CFG);
    gpio_set_pin_mode(SPI1_MOSI, SPI1_MOSI_CFG);
    gpio_set_pin_mode(TFT_DC, TFT_GPIO_CFG);
    gpio_set_pin_mode(TFT_LED_BACKLIGHT, TFT_GPIO_CFG);
    gpio_set_pin_mode(TFT_RESET, TFT_GPIO_CFG);
}
#else
#define TFT_DC gpio_iopack(GPIO_PORT_I, GPIO_PIN_7)
#define TFT_LED_BACKLIGHT gpio_iopack(GPIO_PORT_I, GPIO_PIN_8)
#define TFT_RESET gpio_iopack(GPIO_PORT_I, GPIO_PIN_9)
#define TFT_GPIO_CFG                             \
    (GPIO_MODE_MODER_OUTPUT |                    \
     GPIO_MODE_OTYPER_PUSHPULL |                 \
     GPIO_MODE_OSPEEDR_LOW |                     \
     GPIO_MODE_PUPDR_FLOATING |                  \
     GPIO_MODE_AFR(0U) |                         \
     GPIO_MODE_IHYSTR_CMOS |                     \
     GPIO_MODE_TRIGENR_IN_ENABLED |              \
     GPIO_MODE_SAFESELR_DISABLED |               \
     GPIO_MODE_SAFEVALR_LOW)

__STATIC_INLINE void board_reinit(void) {

    gpio_set_pin_mode(TFT_DC, TFT_GPIO_CFG);
    gpio_set_pin_mode(TFT_LED_BACKLIGHT, TFT_GPIO_CFG);
    gpio_set_pin_mode(TFT_RESET, TFT_GPIO_CFG);
}
#endif

/*===========================================================================*/
/* TFT-related.                                                              */
/*===========================================================================*/

static uint16_t random_color(void) {

    uint16_t r, g, b;
    r = (uint16_t)rand() % 255U;
    g = (uint16_t)rand() % 255U;
    b = (uint16_t)rand() % 255U;
    return ILI9341_RGB565(r, g, b);
}

static int16_t rand_xy(uint16_t max) {

    uint16_t v;
    v = ((uint16_t)rand() % max);
    return (int16_t)v;
}

static uint8_t random_char(void) {

   return ((uint8_t)rand() % 95U) + (uint8_t)' ';
}

int main(void) {

    spi_dma_conf_t spi_dma_conf;
    ILI9341Config tft_config;
    const FontObject_t *font = (const FontObject_t *)&DejaVuSansMono_10pt_7b;
    uint64_t tick_start;
    uint64_t test_time = 5000UL;

    /* Configure SPI DMA.*/
    spi_dma_conf.dma_stream_tx_id = DMA1_STREAM0_ID;
    spi_dma_conf.dma_stream_rx_id = DMA1_STREAM1_ID;
    spi_dma_conf.dma_stream_bus_prio = DMA_PRIORITY_MAX;
    spi_dma_conf.dma_stream_irq_prio = IRQ_PRIORITY_10;

    /* Enable interrupts.*/
    osal_sys_unlock();

    test_env_init((TestInit_t)
                  (TEST_INIT_CLOCK    |
                   TEST_INIT_GPIO     |
                   TEST_INIT_BOARD    |
                   TEST_INIT_IRQ      |
                   TEST_INIT_OSAL));

    /* Reinitialize board default initialization (needed only for some boards).*/
    board_reinit();

    /* Initialize SPI driver instance.*/
    spi_init(&MASTER_SPID);

    /* Configure SPI driver instance.*/
    (void)spi_set_drv_mode(&MASTER_SPID, SPI_DRV_MODE_DMA_SYNC);
    spi_set_dma_conf(&MASTER_SPID, &spi_dma_conf);
    (void)spi_set_mode(&MASTER_SPID, SPI_MODE_MASTER);
    (void)spi_set_baud(&MASTER_SPID, SPI_BAUDRATE_DIV256);
    (void)spi_set_cpha(&MASTER_SPID, SPI_CPHA_0);
    (void)spi_set_cpol(&MASTER_SPID, SPI_CPOL_0);
    (void)spi_set_fformat(&MASTER_SPID, SPI_FRAMEFORMAT_MSB);
    (void)spi_set_fsize(&MASTER_SPID, SPI_FRAMESIZE_8);
    (void)spi_set_nssp_mode(&MASTER_SPID, SPI_NSS_PULSE_MODE_ON);

    /* Configure TFT driver.*/
    tft_config.mode         = ILI9341_SPI_LLD;
    tft_config.dev.lld.spid = &MASTER_SPID;
    tft_config.dc           = TFT_DC;
    tft_config.blk          = TFT_LED_BACKLIGHT;
    tft_config.reset        = TFT_RESET;

    /* Initializes random number generator.*/
    srand(0);

    /* Apply TFT driver configuration.*/
    ili9341_init();
    ili9341_start(&tft_config);
    ili9341_setOrientation(ILI9341_PORTRAIT);

    /* Application main loop */
    for ( ; ; ) {

        ili9341_cropScreen(ili9341_getScreenWidth(), ili9341_getScreenHeight());
        ili9341_clearScreen(ILI9341_COLOR_BLACK);

        /* random Vertical lines */
        tick_start = osal_get_tick();
        ili9341_clearScreen(ILI9341_COLOR_BLACK);
        (void)ili9341_drawString(0, 310, "Vertical Lines", ILI9341_COLOR_WHITE, font);
        ili9341_cropScreen(240, 290);
        while (osal_tk2ms(osal_get_tick() - tick_start) < test_time) {
            ili9341_drawVLine(rand_xy(240), rand_xy(270), rand_xy(290), random_color());
        }
        ili9341_cropScreen(ili9341_getScreenWidth(), ili9341_getScreenHeight());

        /* random Horizontal lines */
        tick_start = osal_get_tick();
        ili9341_clearScreen(ILI9341_COLOR_BLACK);
        (void)ili9341_drawString(0, 310, "Horizontal Lines", ILI9341_COLOR_WHITE, font);
        ili9341_cropScreen(240, 290);
        while (osal_tk2ms(osal_get_tick() - tick_start) < test_time) {
            ili9341_drawHLine(rand_xy(240), rand_xy(290), rand_xy(240), random_color());
        }
        ili9341_cropScreen(ili9341_getScreenWidth(), ili9341_getScreenHeight());

        /* random lines */
        tick_start = osal_get_tick();
        ili9341_clearScreen(ILI9341_COLOR_BLACK);
        (void)ili9341_drawString(0, 310, "Lines", ILI9341_COLOR_WHITE, font);
        ili9341_cropScreen(240, 290);
        while (osal_tk2ms(osal_get_tick() - tick_start) < test_time) {
            ili9341_drawLine(rand_xy(240), rand_xy(290), rand_xy(240), rand_xy(290), random_color());
        }
        ili9341_cropScreen(ili9341_getScreenWidth(), ili9341_getScreenHeight());

        /* Draw rects */
        tick_start = osal_get_tick();
        ili9341_clearScreen(ILI9341_COLOR_BLACK);
        (void)ili9341_drawString(0, 310, "Rectangles", ILI9341_COLOR_WHITE, font);
        ili9341_cropScreen(240, 290);
        while (osal_tk2ms(osal_get_tick() - tick_start) < test_time) {
            ili9341_drawRect(rand_xy(240), rand_xy(270), rand_xy(120), rand_xy(160), random_color());
        }
        ili9341_cropScreen(ili9341_getScreenWidth(), ili9341_getScreenHeight());

        /* Fill rects */
        tick_start = osal_get_tick();
        ili9341_clearScreen(ILI9341_COLOR_BLACK);
        (void)ili9341_drawString(0, 310, "Filled Rectangles", ILI9341_COLOR_WHITE, font);
        ili9341_cropScreen(240, 290);
        while (osal_tk2ms(osal_get_tick() - tick_start) < test_time) {
            ili9341_fillRect(rand_xy(240), rand_xy(270), rand_xy(120), rand_xy(160), random_color());
        }
        ili9341_cropScreen(ili9341_getScreenWidth(), ili9341_getScreenHeight());

        /* Triangles */
        tick_start = osal_get_tick();
        ili9341_clearScreen(ILI9341_COLOR_BLACK);
        (void)ili9341_drawString(0, 310, "Triangles", ILI9341_COLOR_WHITE, font);
        ili9341_cropScreen(240, 290);
        while (osal_tk2ms(osal_get_tick() - tick_start) < test_time) {
            ili9341_drawTriangle(rand_xy(240), rand_xy(290), rand_xy(300), rand_xy(300), rand_xy(300), rand_xy(300), random_color());
        }
        ili9341_cropScreen(ili9341_getScreenWidth(), ili9341_getScreenHeight());

        /* Circles */
        tick_start = osal_get_tick();
        ili9341_clearScreen(ILI9341_COLOR_BLACK);
        (void)ili9341_drawString(0, 310, "Circles", ILI9341_COLOR_WHITE, font);
        ili9341_cropScreen(240, 290);
        while (osal_tk2ms(osal_get_tick() - tick_start) < test_time) {
            ili9341_drawCircle(rand_xy(240), rand_xy(240), rand_xy(50), random_color());
        }
        ili9341_cropScreen(ili9341_getScreenWidth(), ili9341_getScreenHeight());

        /* Fill Circles */
        tick_start = osal_get_tick();
        ili9341_clearScreen(ILI9341_COLOR_BLACK);
        (void)ili9341_drawString(0, 310, "Filled Circles", ILI9341_COLOR_WHITE, font);
        ili9341_cropScreen(240, 290);
        while (osal_tk2ms(osal_get_tick() - tick_start) < test_time) {
            ili9341_fillCircle(rand_xy(240), rand_xy(250), rand_xy(50), random_color());
        }
        ili9341_cropScreen(ili9341_getScreenWidth(), ili9341_getScreenHeight());

        /* Draw Chars */
        tick_start = osal_get_tick();
        ili9341_clearScreen(ILI9341_COLOR_BLACK);
        (void)ili9341_drawString(0, 310, "Characters", ILI9341_COLOR_WHITE, font);
        ili9341_cropScreen(240, 290);
        while (osal_tk2ms(osal_get_tick() - tick_start) < test_time) {
            (void)ili9341_drawChar(rand_xy(240), rand_xy(290), random_char(), random_color(), font);
        }
        ili9341_cropScreen(ili9341_getScreenWidth(), ili9341_getScreenHeight());

        /* Draw Strings */
        tick_start = osal_get_tick();
        ili9341_clearScreen(ILI9341_COLOR_BLACK);
        (void)ili9341_drawString(0, 310, "Strings", ILI9341_COLOR_WHITE, font);
        ili9341_cropScreen(240, 290);
        while (osal_tk2ms(osal_get_tick() - tick_start) < test_time) {
            (void)ili9341_drawString(rand_xy(240), rand_xy(290), "StellarE Test", random_color(), font);
        }
        ili9341_cropScreen(ili9341_getScreenWidth(), ili9341_getScreenHeight());
    }
}
