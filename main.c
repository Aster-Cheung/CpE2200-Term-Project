/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Simple Math Game
  ******************************************************************************
  * @attention
  *
  * This game presents the user with a math question. It is played over the serial interface.
  * The user can reset the game with the on-board reset button of the NUCLEO board.
  * Baud Rate = 115200
  *
  * Copyright (c) 2023 STMicroelectronics.
  * Copyright (c) 2023 Dr. Billy Kihei, for CPE 2200
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <string.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_DMA_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
volatile uint8_t dataAvail = 0;
char rx_buff[8];
uint8_t Welcome[] = "Welcome to my game! Please enter play (p) or exit (x) \r\n"; //Data to send
uint8_t Goodbye[] = "See you later! You can restart the game by pressing the reset button. \r\n"; //Data to send
uint8_t Win[] = "You win! \r\n"; //Data to send
uint8_t NotCorrect[] = "Sorry...try again! \r\n"; //Data to send
uint8_t Game[] = "What is 1+1? \r\n"; //Data to send

#define INVENTORY_SIZE 10
   char inventory[INVENTORY_SIZE][20];
   uint8_t inventoryCount = 0;

   /* ROOMS */
   char *roomName[6] = {
       "Master Bedroom",
       "Living Room",
       "Kitchen",
       "Bathroom",
       "Garage",
       "Hallway"
   };

   char roomItems[6][3][20] = {
       {"", "", ""},
       {"Broom", "", ""},
       {"Sponge", "Gloves", ""},
       {"Mop", "", ""},
       {"Mouse Trap", "Bug Spray", "Trash Bag"},
       {"", "", ""},
   };

   /* GAME VARIABLES */
   uint8_t difficulty = 0;
   uint8_t currentRoom = 0;
   uint8_t cleanedRooms = 0;

   uint8_t roomItemCount[6] = {
       1, 1, 2, 1, 2, 1
   };

   uint8_t roomsClean[6] = {0};

   /* STATES */
   typedef enum {
       STATE_MENU,
       STATE_EXPLORE,
       STATE_ROOM,
       STATE_PUZZLE,
       STATE_CLEAN,
       STATE_COMPLETE,
       STATE_GAME_OVER
   } GameState;

   GameState state = STATE_MENU;
   uint8_t printed = 0;


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

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
  MX_USART2_UART_Init();
  MX_DMA_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart2, rx_buff, 1);
  /* USER CODE END 2 */

  /* Game engine runs in infinite loop until finished. */
  /* Game engine BEGIN WHILE */
  //added inventory


  while (1)
   {
       switch (state)
       {
       case STATE_MENU:
           if (!printed)
           {
               char msg[] = "Select difficulty:\r\n1. Easy\r\n2. Hard\r\n\r\n";
               HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 10);
               printed = 1;
           }

           if (dataAvail)
           {
               char c = rx_buff[0];
               dataAvail = 0;

               if (c == '1' || c == '2')
               {
                   difficulty = c - '0';
                   state = STATE_EXPLORE;
                   printed = 0;
               }
           }

           break;

       case STATE_EXPLORE:
           if (!printed)
           {
               char msg[] =
                   "Choose room:\r\n"
                   "1.Master Bedroom\r\n"
                   "2.Living Room\r\n"
                   "3.Kitchen\r\n"
                   "4.Bathroom\r\n"
                   "5.Garage\r\n"
                   "6.Hallway\r\n\r\n";

               HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 10);
               printed = 1;
           }

           if (dataAvail)
           {
               char c = rx_buff[0];
               dataAvail = 0;

               if (c >= '1' && c <= '6')
               {
                   currentRoom = c - '1';
                   state = STATE_ROOM;
                   printed = 0;
               }
           }

           break;

       case STATE_ROOM:
           if (!printed)
           {
               char msg[] = "You entered a room...\r\n";
               HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 10);
               printed = 1;
           }

           state = STATE_PUZZLE;
           printed = 0;
           break;



       case STATE_PUZZLE:
           if (!printed)
           {
               HAL_UART_Transmit(&huart2,
                   (uint8_t*)"Solve: What is 2+2?\r\n",
                   strlen("Solve: What is 2+2?\r\n"),
                   10);
               printed = 1;
           }

           if (dataAvail)
           {
               char c = rx_buff[0];
               dataAvail = 0;

               if (c < '0' || c > '9')
                   break;

               if (c == '4')
               {
                   state = STATE_CLEAN;
                   printed = 0;
               }
               else
               {
                   HAL_UART_Transmit(&huart2,
                       (uint8_t*)"Wrong!\r\n",
                       strlen("Wrong!\r\n"),
                       10);
               }
           }
           break;

       case STATE_CLEAN:
           HAL_UART_Transmit(&huart2,
               (uint8_t*)"Room cleaned!\r\n\r\n",
               strlen("Room cleaned!\r\n"),
               10);

           if (roomsClean[currentRoom] == 0)
           {
               roomsClean[currentRoom] = 1;
               cleanedRooms++;
           }

           state = STATE_COMPLETE;
           printed = 0;
           break;

       case STATE_COMPLETE:
           if (cleanedRooms >= 6)
               state = STATE_GAME_OVER;
           else
               state = STATE_EXPLORE;
           break;

       case STATE_GAME_OVER:
           HAL_UART_Transmit(&huart2,
               (uint8_t*)"YOU WIN!\r\n",
               strlen("YOU WIN!\r\n"),
               10);

           while (1);
           break;
       }

       HAL_UART_Receive_IT(&huart2, rx_buff, 1);
   }

 /* Game engine END WHILE */

  // Game over
  HAL_UART_Transmit(&huart2,Goodbye,sizeof(Goodbye),10);
  while(1);
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
  /* DMA1_Channel7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/* If UART2 has received a byte from the user terminal this function
 * will be called.
 * dataAvail is used to indicate that the rx_buff has received new data and can be checked.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  HAL_UART_Receive_IT(&huart2, rx_buff, 1); //You need to toggle a breakpoint on this line!
  dataAvail=1;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

