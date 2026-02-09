#ifndef __GPIO_H
#define __GPIO_H

#include "stm32g4xx.h"
//#include "main.h"


#define GPIO_MODE_INPUT                 (0x00000000U) /*!< Select input mode */
#define GPIO_MODE_OUTPUT                GPIO_MODER_MODE0_0  /*!< Select output mode */
#define GPIO_MODE_ALTERNATE             GPIO_MODER_MODE0_1  /*!< Select alternate function mode */
#define GPIO_MODE_ANALOG                GPIO_MODER_MODE0    /*!< Select analog mode */

#define GPIO_OUTPUT_PUSHPULL            (0x00000000U) /*!< Select push-pull as output type */
#define GPIO_OUTPUT_OPENDRAIN           GPIO_OTYPER_OT0 /*!< Select open-drain as output type */

#define GPIO_SPEED_FREQ_LOW             (0x00000000U) /*!< Select I/O low output speed  5-10 MHz  */
#define GPIO_SPEED_FREQ_MEDIUM          GPIO_OSPEEDR_OSPEED0_0 /*!< Select I/O medium output speed 25-50 MHz */
#define GPIO_SPEED_FREQ_HIGH            GPIO_OSPEEDR_OSPEED0_1 /*!< Select I/O fast output speed 50-100 MHz  */
#define GPIO_SPEED_FREQ_VERY_HIGH       GPIO_OSPEEDR_OSPEED0   /*!< Select I/O high output speed 120-180 MHz  */

#define GPIO_PULL_NO                    (0x00000000U) /*!< Select I/O no pull */
#define GPIO_PULL_UP                    GPIO_PUPDR_PUPD0_0 /*!< Select I/O pull up */
#define GPIO_PULL_DOWN                  GPIO_PUPDR_PUPD0_1 /*!< Select I/O pull down */

#define GPIO_AF_0                       (0x0000000U) /*!< Select alternate function 0 */
#define GPIO_AF_1                       (0x0000001U) /*!< Select alternate function 1 */
#define GPIO_AF_2                       (0x0000002U) /*!< Select alternate function 2 */
#define GPIO_AF_3                       (0x0000003U) /*!< Select alternate function 3 */
#define GPIO_AF_4                       (0x0000004U) /*!< Select alternate function 4 */
#define GPIO_AF_5                       (0x0000005U) /*!< Select alternate function 5 */
#define GPIO_AF_6                       (0x0000006U) /*!< Select alternate function 6 */
#define GPIO_AF_7                       (0x0000007U) /*!< Select alternate function 7 */
#define GPIO_AF_8                       (0x0000008U) /*!< Select alternate function 8 */
#define GPIO_AF_9                       (0x0000009U) /*!< Select alternate function 9 */
#define GPIO_AF_10                      (0x000000AU) /*!< Select alternate function 10 */
#define GPIO_AF_11                      (0x000000BU) /*!< Select alternate function 11 */
#define GPIO_AF_12                      (0x000000CU) /*!< Select alternate function 12 */
#define GPIO_AF_13                      (0x000000DU) /*!< Select alternate function 13 */
#define GPIO_AF_14                      (0x000000EU) /*!< Select alternate function 14 */
#define GPIO_AF_15                      (0x000000FU) /*!< Select alternate function 15 */


void GPIO_Init(void);

#endif

