#ifndef __TDC1000_H
#define __TDC1000_H

#include "stm32g4xx.h"

#define TDC1000_RES 																			GPIOA->BSRR|=(1<<12)
#define TDC1000_START 																		GPIOA->BRR|=(1<<12)
#define TDC1000_CSEN 																			GPIOA->BRR|=(1<<11)
#define TDC1000_CSDIS 																		GPIOA->BSRR|=(1<<11)

#define TDC7200_CSEN 																			GPIOA->BRR|=(1<<9)
#define TDC7200_CSDIS 																		GPIOA->BSRR|=(1<<9)

#define TDC7200_INTBUP 																		GPIOB->IDR&(1<<10)
#define TDC7200_INTBDW 																		!(GPIOB->IDR&(1<<10))

#define TDC1000_TIMING_REG_Write(num)											TDC1000_SPIWrite(TDC1000_REG_ADR_TOF_0, ((num&0xFF)<<TDC1000_REG_TOF_0_TIMING_REG_Pos)); \
																													TDC1000_SPIWrite(TDC1000_REG_ADR_TOF_1, (((num>>8)&0b11)<<TDC1000_REG_TOF_1_TIMING_REG_Pos));
																													
																													
#define TDC1000_TEMP_MEASURE_MODE_ON											TDC1000_SPIWrite(TDC1000_REG_ADR_CONFIG_2, TDC1000_SPIRead(TDC1000_REG_ADR_CONFIG_2) | \
																														(TDC1000_REG_VAL_CONFIG_2_MEAS_MODE_TEMP<<TDC1000_REG_CONFIG_2_MEAS_MODE_Pos));																													
#define TDC1000_TEMP_MEASURE_MODE_OFF											TDC1000_SPIWrite(TDC1000_REG_ADR_CONFIG_2, TDC1000_SPIRead(TDC1000_REG_ADR_CONFIG_2) & \
																														(~(TDC1000_REG_VAL_CONFIG_2_MEAS_MODE_TEMP<<TDC1000_REG_CONFIG_2_MEAS_MODE_Pos)));

#define TDC7200_5_STOP																		TDC7200_SPIWrite(0x01, 0x44);
#define TDC7200_1_STOP																		TDC7200_SPIWrite(0x01, 0x40);

#define TDC1000_SPI_REG_ADDR_MASK                         (0x1Fu)
#define TDC1000_SPI_REG_READ                              (0x00u)
#define TDC1000_SPI_REG_WRITE                             (0x40u)

#define TDC7200_SPI_REG_ADDR_MASK                         (0x3Fu)
#define TDC7200_SPI_REG_READ                              (0x80u)
#define TDC7200_SPI_REG_WRITE                             (0x40u)


#define TDC1000_REG_ADR_CONFIG_0                          (0x00u)
#define TDC1000_REG_ADR_CONFIG_1                          (0x01u)
#define TDC1000_REG_ADR_CONFIG_2                          (0x02u)
#define TDC1000_REG_ADR_CONFIG_3                          (0x03u)
#define TDC1000_REG_ADR_CONFIG_4                          (0x04u)
#define TDC1000_REG_ADR_TOF_1                             (0x05u)
#define TDC1000_REG_ADR_TOF_0                             (0x06u)
#define TDC1000_REG_ADR_ERROR_FLAGS                       (0x07u)
#define TDC1000_REG_ADR_TIMEOUT                           (0x08u)
#define TDC1000_REG_ADR_CLOCK_RATE                        (0x09u)

#define TDC1000_REG_CONFIG_0_TX_FREQ_DIV_Pos        			(5)
#define TDC1000_REG_CONFIG_0_TX_FREQ_DIV_Msk             	(0b111u)
#define TDC7200_REG_VAL_CONFIG_0_TX_FREQ_DIV(num)         (1 << (num+1))
//#define TDC7200_REG_VAL_CONFIG_0_TX_FREQ_DIV(num)         (1 << (num+1))
#define TDC1000_REG_CONFIG_0_NUM_TX_Pos             			(0)
#define TDC1000_REG_CONFIG_0_NUM_TX_Msk                  	(0b11111u)
#define TDC7200_REG_VAL_CONFIG_0_NUM_TX(num)              (num)
#define TDC1000_REG_VAL_CONFIG_0_NUM_TX_MIN               (0)
#define TDC1000_REG_VAL_CONFIG_0_NUM_TX_MAX               (31)
#define TDC1000_REG_CONFIG0_DEFAULTS                      (0x45u)


#define TDC1000_REG_CONFIG_1_NUM_AVG_Pos                (3)
#define TDC1000_REG_CONFIG_1_NUM_AVG_Msk                 (0b111u)
#define TDC7200_REG_VAL_CONFIG_1_NUM_AVG(num)             (1 << (num))
#define TDC1000_REG_CONFIG_1_NUM_RX_Pos                 (0)
#define TDC1000_REG_CONFIG_1_NUM_RX_Msk                  (0b111u)
#define TDC7200_REG_VAL_CONFIG_1_NUM_RX(num)              (num)
#define TDC1000_REG_VAL_CONFIG_1_NUM_RX_MIN               (0)
#define TDC1000_REG_VAL_CONFIG_1_NUM_RX_MAX               (7)
#define TDC1000_REG_CONFIG1_DEFAULTS                      (0x40u)


#define TDC1000_REG_CONFIG_2_VCOM_SEL_Pos               (7)
#define TDC1000_REG_CONFIG_2_VCOM_SEL_Msk                (0b1u)
#define TDC1000_REG_CONFIG_2_MEAS_MODE_Pos              (6)
#define TDC1000_REG_CONFIG_2_MEAS_MODE_Msk               (0b1u)
#define TDC1000_REG_VAL_CONFIG_2_MEAS_MODE_TOF            (0)
#define TDC1000_REG_VAL_CONFIG_2_MEAS_MODE_TEMP           (1)
#define TDC1000_REG_CONFIG_2_DAMPING_Pos                (5)
#define TDC1000_REG_CONFIG_2_DAMPING_Msk                 (0b1u)
#define TDC1000_REG_CONFIG_2_CH_SWP_Pos                 (4)
#define TDC1000_REG_CONFIG_2_CH_SWP_Msk                  (0b1u)
#define TDC1000_REG_CONFIG_2_EXT_CHSEL_Pos              (3)
#define TDC1000_REG_CONFIG_2_EXT_CHSEL_Msk               (0b1u)
#define TDC1000_REG_CONFIG_2_CH_SEL_Pos                 (2)
#define TDC1000_REG_CONFIG_2_CH_SEL_Msk                  (0b1u)
#define TDC1000_REG_VAL_CONFIG_2_CH_SEL(num)              ((num)+1)
#define TDC1000_REG_CONFIG_2_TOF_MEAS_MODE_Pos          (0)
#define TDC1000_REG_CONFIG_2_TOF_MEAS_MODE_Msk           (0x03u)
#define TDC1000_REG_VAL_CONFIG_2_TOF_MEAS_MODE_MIN        (0)
#define TDC1000_REG_VAL_CONFIG_2_TOF_MEAS_MODE_MAX        (2)
#define TDC7200_REG_VAL_CONFIG_2_TOF_MEAS_MODE(num)       (num)
#define TDC1000_REG_CONFIG2_DEFAULTS                      (0x00u)


#define TDC1000_REG_CONFIG_3_TEMP_MODE_Pos              (6)
#define TDC1000_REG_CONFIG_3_TEMP_MODE_Msk               (0b1u)
#define TDC1000_REG_CONFIG_3_TEMP_RTD_SEL_Pos           (5)
#define TDC1000_REG_CONFIG_3_TEMP_RTD_SEL_Msk            (0b1u)
#define TDC1000_REG_CONFIG_3_TEMP_CLK_DIV_Pos           (4)
#define TDC1000_REG_CONFIG_3_TEMP_CLK_DIV_Msk            (0b1u)
#define TDC1000_REG_CONFIG_3_BLANKING_Pos               (3)
#define TDC1000_REG_CONFIG_3_BLANKING_Msk                (0b1u)
#define TDC1000_REG_CONFIG_3_ECHO_QUAL_THLD_Pos         (0)
#define TDC1000_REG_CONFIG_3_ECHO_QUAL_THLD_Msk          (0b111u)
#define TDC1000_REG_VAL_CONFIG_3_ECHO_QUAL_THLD_MIN       (0)
#define TDC1000_REG_VAL_CONFIG_3_ECHO_QUAL_THLD_MAX       (7)
#define TDC1000_REG_CONFIG3_DEFAULTS                      (0x03u)


#define TDC1000_REG_CONFIG_4_RECEIVE_MODE_Pos           (6)
#define TDC1000_REG_CONFIG_4_RECEIVE_MODE_Msk            (0b1u)
#define TDC1000_REG_CONFIG_4_TRIG_EDGE_POLARITY_Pos     (5)
#define TDC1000_REG_CONFIG_4_TRIG_EDGE_POLARITY_Msk      (0b1u)
#define TDC1000_REG_CONFIG_4_TX_PH_SHIFT_POS_Pos        (0)
#define TDC1000_REG_CONFIG_4_TX_PH_SHIFT_POS_Msk         (0b11111u)
#define TDC1000_REG_VAL_CONFIG_4_TX_PH_SHIFT_POS(num)     (num)
#define TDC1000_REG_VAL_CONFIG_4_TX_PH_SHIFT_POS_MIN      (0)
#define TDC1000_REG_VAL_CONFIG_4_TX_PH_SHIFT_POS_MAX      (31)
#define TDC1000_REG_CONFIG4_DEFAULTS                      (0x1Fu)


#define TDC1000_REG_TOF_1_PGA_GAIN_Pos                  (5)
#define TDC1000_REG_TOF_1_PGA_GAIN_Msk                   (0b111u)
#define TDC1000_REG_VAL_TOF_1_PGA_GAIN(num)               (3*(num))
#define TDC1000_REG_TOF_1_PGA_CTRL_Pos                  (4)
#define TDC1000_REG_TOF_1_PGA_CTRL_Msk                   (0b1u)
#define TDC1000_REG_VAL_TOF_1_PGA_CTRL_ACTIVE             (0u)
#define TDC1000_REG_VAL_TOF_1_PGA_CTRL_BYPASSED           (1u)
#define TDC1000_REG_TOF_1_LNA_CTRL_Pos                  (3)
#define TDC1000_REG_TOF_1_LNA_CTRL_Msk                   (0b1u)
#define TDC1000_REG_VAL_TOF_1_LNA_CTRL_ACTIVE             (0u)
#define TDC1000_REG_VAL_TOF_1_LNA_CTRL_BYPASSED           (1u)
#define TDC1000_REG_TOF_1_LNA_FB_Pos                    (2)
#define TDC1000_REG_TOF_1_LNA_FB_Msk                     (0b1u)
#define TDC1000_REG_TOF_1_TIMING_REG_Pos                (0)
#define TDC1000_REG_TOF_1_TIMING_REG_Msk                 (0b11u)
#define TDC1000_REG_VAL_TOF_1_PGA_GAIN_MIN                (0)
#define TDC1000_REG_VAL_TOF_1_PGA_GAIN_MAX                (7)
#define TDC1000_REG_TOF_1_DEFAULTS                      (0x00u)


#define TDC1000_REG_TOF_0_TIMING_REG_Pos                (0)
#define TDC1000_REG_TOF_0_TIMING_REG_Msk                 (0b11111111u)
#define TDC1000_REG_VAL_TOF_TIMING_REG_MIN                (0u)
#define TDC1000_REG_VAL_TOF_TIMING_REG_FORCE_SHORT_MAX    (29u)
#define TDC1000_REG_VAL_TOF_TIMING_REG_MAX                (1023u)
#define TDC1000_REG_ERROR_FLAGS_ERR_SIG_WEAK_Pos        (2)
#define TDC1000_REG_ERROR_FLAGS_ERR_SIG_WEAK_Msk         (0b1u)
#define TDC1000_REG_ERROR_FLAGS_ERR_NO_SIG_Pos          (1)
#define TDC1000_REG_ERROR_FLAGS_ERR_NO_SIG_Msk           (0b1u)
#define TDC1000_REG_ERROR_FLAGS_ERR_SIG_HIGH_Pos        (0)
#define TDC1000_REG_ERROR_FLAGS_ERR_SIG_HIGH_Msk         (0b1u)
#define TDC1000_REG_TOF_0_DEFAULTS                      (0x00u)


#define TDC1000_REG_TIMEOUT_FORCE_SHORT_TOF_Pos         (6)
#define TDC1000_REG_TIMEOUT_FORCE_SHORT_TOF_Msk          (0b1u)
#define TDC1000_REG_VAL_TIMEOUT_FORCE_SHORT_TOF_DISABLED  (0u)
#define TDC1000_REG_VAL_TIMEOUT_FORCE_SHORT_TOF_ENABLED   (1u)
#define TDC1000_REG_TIMEOUT_SHORT_TOF_BLANK_PERIOD_Pos  (3)
#define TDC1000_REG_TIMEOUT_SHORT_TOF_BLANK_PERIOD_Msk   (0b111u)
#define TDC1000_REG_VAL_TIMEOUT_SHORT_TOF_BLANK_PERIOD(num)  (1u << ((num) + 3))
#define TDC1000_REG_TIMEOUT_ECHO_TIMEOUT_Pos            (2)
#define TDC1000_REG_TIMEOUT_ECHO_TIMEOUT_Msk             (0b1u)
#define TDC1000_REG_VAL_TIMEOUT_ECHO_TIMEOUT_ENABLED      (0u)
#define TDC1000_REG_VAL_TIMEOUT_ECHO_TIMEOUT_DISABLED     (1u)
#define TDC1000_REG_TIMEOUT_TOF_TIMEOUT_CTRL_Pos        (0)
#define TDC1000_REG_TIMEOUT_TOF_TIMEOUT_CTRL_Msk         (0b11u)
#define TDC1000_REG_VAL_TIMEOUT_TOF_TIMEOUT_CTRL(num)     (1u << ((num) + 7))
#define TDC1000_REG_TIMEOUT_DEFAULTS                      (0x19u)


#define TDC1000_REG_CLOCKIN_DIV_Pos                     (2)
#define TDC1000_REG_CLOCKIN_DIV_Msk                      (0b1u)
#define TDC1000_REG_VAL_CLOCKIN_DIV(num)                  ((num)+1)
#define TDC1000_REG_AUTOZERO_PERIOD_Pos                 (0)
#define TDC1000_REG_AUTOZERO_PERIOD_Msk                  (0b11u)
#define TDC1000_REG_VAL_AUTOZERO_PERIOD(num)              (1u << ((num) + 6))



void TDC1000_Init(void);
void TDC1000_SPIWrite(uint8_t addr, uint8_t data);
uint8_t TDC1000_SPIRead(uint8_t addr);

void TDC7200_Init(void);
void TDC7200_SPIWrite(uint8_t addr, uint8_t data);
uint8_t TDC7200_SPIRead(uint8_t addr);
uint32_t TDC7200_SPIRead_Reg(uint8_t addr, uint8_t size);

#endif // __TDC1000_H
