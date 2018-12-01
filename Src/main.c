
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_hal.h"
#include "tsc.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */

#include "LED_Controller.h"
#include "FLASH_DataSaver.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//TODO Taste 3 löschen
#define  TOUCH1         			100           // Grenzwert -> Taste erkannt 
#define  TOUCH2         			100           // Grenzwert -> Taste erkannt

#define DURATION_TOUCH_SHORT	1						// Hochzählen, erst nach x Touch Erkennungen Event auslösen
#define DURATION_TOUCH_LONG 	0xFF				// Hochzählen, erst nach x Touch Erkennungen Event auslösen

#define  FILTER         			0xFF          // Koeffizienten Filter


/* Error codes used to make the red led blinking */

TSC_IOConfigTypeDef conf;

/* Private variables ---------------------------------------------------------*/


uint32_t state;

typedef struct
{
  uint32_t          tsc_wert;           /* Aktueller Meßwert */
  uint32_t          mittel;             /* laufender quasi Mittelwert */
  uint32_t          mittelsum;          /* laufende Summe quasi Mittelwert */
  uint32_t          pressed;            /* Touch event erkannt - "Taste" gedrückt */
  uint32_t          released;           /* Touch nicht erkannt - "Taste" losgelassen */
	uint32_t					triggered;					/* Touch event ausgelöst, blockiert bis wieder losgelassen wurde */
} TasteTypeDef;

TasteTypeDef taste1;
TasteTypeDef taste2;

uint32_t IRQ_TSC;

uint32_t nextstep;

extern uint32_t msTick;
extern uint32_t hTick;

extern uint32_t slideColorIndex;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

void tasten(void)
{
		switch(nextstep) 
		{

			case 0:
					HAL_TSC_IODischarge(&htsc,1);
					HAL_TSC_IODischarge(&htsc,2);
			    nextstep = 1;
			break;
				
			case 1:
					conf.ChannelIOs = TSC_GROUP1_IO1;
					conf.SamplingIOs = TSC_GROUP1_IO4;
				
					HAL_TSC_IOConfig(&htsc,&conf);
					HAL_TSC_Start_IT(&htsc);
					nextstep = 2;
			break;
		
			case 2:
					if(IRQ_TSC)
					{
						IRQ_TSC = 0;
						state = HAL_TSC_GetState(&htsc);
						if(state == HAL_TSC_STATE_READY)
						{
							taste1.tsc_wert = HAL_TSC_GroupGetValue(&htsc,0);
							if(taste1.mittelsum == 0) 
							{
								taste1.mittelsum = taste1.tsc_wert*FILTER;   											// Startwert
							}
							// P-Glied
							else
							{
								taste1.mittelsum += taste1.tsc_wert - (taste1.mittelsum/FILTER);   // Mittelwert Summe
								taste1.mittel = taste1.mittelsum/FILTER;   												// Mittelwert
							}
							// Taste gedrückt
							if(taste1.tsc_wert + TOUCH1 < taste1.mittel)
							{								
								taste1.released = 0;
								taste1.triggered = 1;	
								taste1.pressed = (taste1.pressed < 0xFFFF) ? taste1.pressed+1 : taste1.pressed;	// Overflow Schutz
							}
							// Taste nicht gedrückt
							else
							{
								// Event Touch Taste 1 - short
									if (	 (taste1.pressed > DURATION_TOUCH_SHORT) 
										 &&  (taste1.pressed < DURATION_TOUCH_LONG) 
										 &&   taste1.triggered)
								{
									changeBrightness(LOWER_BRIGHTNESS);
								}
								// Event Touch Taste 1 - long
								else if ((taste1.pressed > DURATION_TOUCH_LONG) 
										 &&   taste1.triggered)
								{
									skipDoingNothing();							
								}
								taste1.pressed 		= 0;
								taste1.triggered 	= 0;
								taste1.released 	= (taste1.released < 0xFFFF) ? taste1.released+1 : taste1.released;	// Overflow Schutz
							}
						}
						HAL_TSC_IODischarge(&htsc,1);
						HAL_TSC_IODischarge(&htsc,2);
						nextstep = 3;
					}
			break;

			case 3:
					conf.ChannelIOs = TSC_GROUP1_IO2;
					conf.SamplingIOs = TSC_GROUP1_IO4;
				
					HAL_TSC_IOConfig(&htsc,&conf);
					HAL_TSC_Start_IT(&htsc);
					nextstep = 4;
			break;
		
			case 4:
					if(IRQ_TSC)
					{
						IRQ_TSC = 0;
						state = HAL_TSC_GetState(&htsc);
						if(state == HAL_TSC_STATE_READY)
						{
							taste2.tsc_wert = HAL_TSC_GroupGetValue(&htsc,0);
							if(taste2.mittelsum == 0) 
							{
								taste2.mittelsum = taste2.tsc_wert*FILTER;   											// Startwert
							}
							// P-Glied
							else
							{
								taste2.mittelsum += taste2.tsc_wert - (taste2.mittelsum/FILTER);   // Mittelwert Summe
								taste2.mittel = taste2.mittelsum/FILTER;   												// Mittelwert
							}
							// Taste gedrückt
							if(taste2.tsc_wert + TOUCH2 < taste2.mittel)
							{
								skipDoingNothing();
								
								taste2.released 	= 0;
								taste2.triggered 	= 1;	
								taste2.pressed 		= (taste2.pressed < 0xFFFF) ? taste2.pressed+1 : taste2.pressed;	// Overflow Schutz
							}
							// Taste nicht gedrückt
							else
							{
								// Event Touch Taste 2 - short
									if (	 (taste2.pressed > DURATION_TOUCH_SHORT) 
										 &&  (taste2.pressed < DURATION_TOUCH_LONG) 
										 &&   taste2.triggered)
								{
									changeBrightness(HIGHER_BRIGHTNESS);
								}
								// Event Touch Taste 2 - long
								else if ((taste2.pressed > DURATION_TOUCH_LONG) 
										 &&   taste2.triggered)
								{
									skipDoingNothing();							
								}
								taste2.pressed 		= 0;
								taste2.triggered 	= 0;
								taste2.released 	= (taste2.released < 0xFFFF) ? taste2.released+1 : taste2.released;	// Overflow Schutz
							}
						}
						HAL_TSC_IODischarge(&htsc,1);
						HAL_TSC_IODischarge(&htsc,2);
						nextstep = 1;
					}
			break;
					
			default:
						HAL_TSC_IODischarge(&htsc,1);
						HAL_TSC_IODischarge(&htsc,2);
						nextstep = 1;
			break;

		}
}
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TSC_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
	
	rgbLED[0] = 40;
	rgbLED[1] = 0;
	rgbLED[2] = 0;
	
	initFLASH_DataSaver();
	loadBrightnessData(readDataFromFLASH());
	
	//saveDataToFLASH(dataS);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
		//Test_RGB_LEDs();
		/* Start LED_Modes	*/
		userRunControl();
		/*	Check time and enable flags	*/
		userTimeControl();
		/*	Taster	*/  
		if(IRQ_TSC || msTick)
		{
			msTick = 0;
			tasten();
		}
		/* Brightness in FLASH speichern	*/
		if(hTick && (slideColorIndex != readDataFromFLASH()))
		{
			hTick = 0;
			saveDataToFLASH(slideColorIndex);
		}
	}	
	

  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL5;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* TSC_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TSC_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TSC_IRQn);
}

/* USER CODE BEGIN 4 */


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
