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
#include <stdio.h>  // for sprintf


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
extern void addPoint(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
volatile uint8_t dataAvail = 0;
char rx_buff[8];

//store name
 char playerName[20] = {0};
 uint8_t nameIndex = 0;
 uint8_t nameEntered = 0;

#define NUM_EASY_PUZZLES 5
#define NUM_HARD_PUZZLES 5

char *easyQuestions[NUM_EASY_PUZZLES] = {
    "What letter comes after C?\r\n",
    "What cleans best?\r\nA) Spoon B) Mop C) Pillow\r\n",
    "What color is the sky?\r\nA) Blue B) Green C) Red\r\n",
    "Press 'y' to continue\r\n",
    "What is 2+2?\r\n"
};

char easyAnswers[NUM_EASY_PUZZLES] = {
    'd',
    'b',
    'a',
    'y',
    '4'
};

char *easyHints[NUM_EASY_PUZZLES] = {
		"Think of the alphabet order or song...",
		"Which item is used for cleaning floors?",
		"Look up at the sky during the day, what color is that?",
		"Just press y... that's not that hard...",
		"Basic addition, use your fingers...2 fingers + 2 fingers..\r\nHow many fingers in total?"
};

char *hardQuestions[NUM_HARD_PUZZLES] = {
    "What is 12 * 3?\r\n",
    "What comes before 'm'?\r\n",
    "Solve: 15 - 7 = ?\r\n",
    "Which is heavier?\r\nA) Rock B) Feather C) Paper\r\n",
    "Is Dr. Chin a... \r\n A) Professor B) A TA C) A student\r\n"
};

char hardAnswers[NUM_HARD_PUZZLES] = {
    '3',   
    'l',
    '8',
    'a',
    'h'
};

char *hardHints[NUM_HARD_PUZZLES] = {
	"Add 12 three times... What do you get?",
	"Alphabet position: m comes after 1",
	"Break subtraction into chunks, or use your fingers.",
	"Think weight logically, not visually, gravity does exist.",
	"Where do you see Dr. Chin in lectures? What is he doing in class?"
};

char *roomMess[6] = {
    "Dust and dirt everywhere on the floor.",
    "The living room floor is covered in crumbs.",
    "Greasy dishes and sticky spills in the kitchen.",
    "Wet floor and soap everywhere in the bathroom.",
    "There are mice running around in the garage!",
    "Trash is scattered all over the hallway."
};

uint8_t currentPuzzle = 0;

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

   char roomItemsInit[6][3][20] = {
       {"", "", ""},
       {"Broom", "", ""},
       {"Sponge", "Gloves", ""},
       {"Mop", "", ""},
       {"Mouse Trap", "Bug Spray", "Trash Bag"},
       {"", "", ""},
   };

   typedef struct {
       char *name;
       char *art;
   } Item;


   char broomArt[] =
   "   |\r\n"
   "   |\r\n"
   "   |\r\n"
   "=======\r\n"
   "|||||||\r\n"
   "|||||||\r\n\r\n";

   char spongeArt[] =
   " .----.\r\n"
   "/ SPG \\\r\n"
   "\\____/\r\n\r\n";

   char mopArt[] =
   "    ||\r\n"
   "    ||\r\n"
   "    ||\r\n"
   "   ==== \r\n"
   "  /____\\\r\n";

   char glovesArt[] =
   "  ____________\r\n"
   " |  GLOVES    |\r\n"
   " |------------|\r\n"
   " |  [ ][ ]    |\r\n"
   " |  [ ][ ]    |\r\n"
   " |____________|\r\n";

   char BugsprayArt[] =
   "    _______\r\n"
   "   |       |\r\n"
   "   | SPRAY |\r\n"
   "   |_______|\r\n"
   "      ||\r\n"
   "      ||\r\n"
   "     /__\\\r\n"
   "    (____)\r\n";

   char mouseArt[] =
   "   ___________\r\n"
   "  |  MOUSE   |\r\n"
   "  |   TRAP   |\r\n"
   "  |__________|\r\n"
   "     ||  ||\r\n"
   "     ||  ||\r\n"
   "   __||__||__\r\n"
   "  /  SNAP!!  \\\r\n";

   char trashbagArt[] =
       "     ____\r\n"
       "   /      \\\r\n"
       "  /  TRASH  \\\r\n"
       " |    BAG    |\r\n"
       " |            |\r\n"
       "  \\  ____   /\r\n"
       "   \\______/ \r\n";

   Item items[] = {
          {"Broom", broomArt},
          {"Sponge", spongeArt},
		  {"Mop", mopArt},
		  {"Gloves", glovesArt},
		  {"Bug Spray", BugsprayArt},
		  {"Mouse Trap", mouseArt},
		  {"Trash Bag", trashbagArt}
      };

   char *requiredItem[6] = {
       "Broom",       // Master Bedroom
       "Broom",       // Living Room
       "Sponge",      // Kitchen
       "Mop",         // Bathroom
       "Mouse Trap",  // Garage
       "Trash Bag"    // Hallway
   };

   char selectedItem[20];


#define HINT_TIME_MS 3000
   uint8_t hintGiven = 0;
   /* GAME VARIABLES */
   uint8_t difficulty = 0;
   uint8_t currentRoom = 0;
   uint8_t cleanedRooms = 0;
   uint8_t hintNPCUsed = 0;
   uint32_t puzzleStartTime = 0;
   uint32_t timeLimit = 0;
   uint32_t lastTimerPrint = 0;
   uint8_t Goodbye[] = "See you later! You can restart the game by pressing the reset button. \r\n"; //Data to send
   volatile int score = 0;
   extern volatile int score;


   uint8_t roomItemCount[6] = {
       0, 1, 2, 1, 3, 0
   };

   uint8_t roomsClean[6] = {0};

   char winBanner[] =
   "██╗   ██╗ ██████╗ ██╗   ██╗    ██╗    ██╗██╗███╗   ██╗██╗██╗\r\n"
   "╚██╗ ██╔╝██╔═══██╗██║   ██║    ██║    ██║██║████╗  ██║██║██║\r\n"
   " ╚████╔╝ ██║   ██║██║   ██║    ██║ █╗ ██║██║██╔██╗ ██║██║██║\r\n"
   "  ╚██╔╝  ██║   ██║██║   ██║    ██║███╗██║██║██║╚██╗██║╚═╝╚═╝\r\n"
   "   ██║   ╚██████╔╝╚██████╔╝    ╚███╔███╔╝██║██║ ╚████║██╗██╗\r\n"
   "   ╚═╝    ╚═════╝  ╚═════╝      ╚══╝╚══╝ ╚═╝╚═╝  ╚═══╝╚═╝╚═╝\r\n";

   char loseBanner[] =
   "          _______             _        _______  _______  _______  _  _ \r\n"
   "|\\     /|(  ___  )|\\     /|  ( \\      (  ___  )(  ____ \\(  ____ \\( )( )\r\n"
   "( \\   / )| (   ) || )   ( |  | (      | (   ) || (    \\/| (    \\/| || |\r\n"
   " \\ (_) / | |   | || |   | |  | |      | |   | || (_____ | (__    | || |\r\n"
   "  \\   /  | |   | || |   | |  | |      | |   | |(_____  )|  __)   | || |\r\n"
   "   ) (   | |   | || |   | |  | |      | |   | |      ) || (      (_)(_)\r\n"
   "   | |   | (___) || (___) |  | (____/\\| (___) |/\\____) || (____/\\ _  _ \r\n"
   "   \\_/   (_______)(_______)  (_______/(_______)\\_______)(_______/(_)(_)\r\n";



   /* STATES */
   typedef enum {
	   STATE_MAIN_MENU,
       STATE_MENU,
	   STATE_NAME,
       STATE_EXPLORE,
       STATE_ROOM,
	   STATE_ACTION,
	   STATE_USE_ITEM,
       STATE_PUZZLE,
       STATE_CLEAN,
       STATE_COMPLETE,
       STATE_GAME_OVER
   } GameState;

   GameState state = STATE_MAIN_MENU;
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
  srand(HAL_GetTick()); // timer

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
       case STATE_MAIN_MENU:
           if (!printed)
           {
               char msg[] =
                   "===== MAIN MENU =====\r\n"
                   "1. Start Game\r\n"
                   "2. Quit\r\n\r\n";

               HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 10);
               printed = 1;
           }

           if (dataAvail)
           {
               char c = rx_buff[0];
               dataAvail = 0;

               if (c == '1')
               {
                   state = STATE_MENU; // difficulty
                   printed = 0;
               }
               else if (c == '2')
               {
                   HAL_UART_Transmit(&huart2,
                       (uint8_t*)"Goodbye!\r\n",
                       strlen("Goodbye!\r\n"),
                       10);
                   while (1);
               }
           }
           break;

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
            	   // Ask for the player's name

				   difficulty = c - '0';

				   nameIndex = 0;
				   nameEntered = 0;
				   memset(playerName, 0, sizeof(playerName));

				   HAL_UART_Transmit(&huart2,
					   (uint8_t*)"Enter your name and press ENTER:\r\n",
					   strlen("Enter your name and press ENTER:\r\n"),
					   10);

				   state = STATE_NAME;
				   printed = 0;

            	   if (nameEntered)
            	   {
            	       char welcome[100];
            	       sprintf(welcome, "Welcome, %s!\r\n", playerName);
            	       HAL_UART_Transmit(&huart2, (uint8_t*)welcome, strlen(welcome), 10);
            	   }

               }
           }

           break;

       case STATE_NAME:
           if (nameEntered)
           {
               char welcome[100];
               sprintf(welcome, "Welcome, %s!\r\n", playerName);
               HAL_UART_Transmit(&huart2, (uint8_t*)welcome, strlen(welcome), 10);

               char msg[] = "Let's Start, good luck!\r\n\r\n";
				HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 10);
				HAL_Delay(1000);

				char msg3[] = "3....\r\n\r\n";
				HAL_UART_Transmit(&huart2, (uint8_t*)msg3, strlen(msg3), 10);
				HAL_Delay(1000);

				char msg2[] = "2....\r\n\r\n";
				HAL_UART_Transmit(&huart2, (uint8_t*)msg2, strlen(msg2), 10);
				HAL_Delay(1000);

				char msg1[] = "1....\r\n\r\n";
				HAL_UART_Transmit(&huart2, (uint8_t*)msg1, strlen(msg1), 10);
				HAL_Delay(1000);

				 state = STATE_EXPLORE;
				 printed = 0;
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
                   "6.Hallway\r\n"
                   "7.Show Map\r\n\r\n";

               HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 10);


               printed = 1;
           }

           if (dataAvail)
           {
               char c = rx_buff[0];
               dataAvail = 0;

               if (c == '7')
               {
                   printMap();
                   state = STATE_EXPLORE;
               }

               if (c >= '1' && c <= '6')
               {
                   currentRoom = c - '1';
                   state = STATE_ROOM;
                   printed = 0;
               }
           }

           break;

       case STATE_ROOM:
    	   puzzleStartTime = HAL_GetTick();

           if (!printed)
           {
        	   char msg[200];

        	   HAL_UART_Transmit(&huart2, (uint8_t*)playerName, strlen(playerName), 10);
        	   sprintf(msg,
        	       ", You entered the %s...\r\n%s\r\n\r\n",
        	       roomName[currentRoom],
        	       roomMess[currentRoom]);

        	   HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 10);
               printed = 1;
           }

           if (roomsClean[currentRoom] == 1)
           {
               HAL_UART_Transmit(&huart2,
                   (uint8_t*)"This room is already cleaned! Nothing more to do here. \r\nAre you obsessed with cleaning rooms?\r\n\r\n",
                   strlen("This room is already cleaned! Nothing more to do here. \r\nAre you obsessed with cleaning rooms?\r\n\r\n"),
                   10);

               state = STATE_ACTION;   // skip puzzle
               printed = 0;
           }
           else
           {
               if (difficulty == 1)
                   currentPuzzle = rand() % NUM_EASY_PUZZLES;
               else
                   currentPuzzle = rand() % NUM_HARD_PUZZLES;

               state = STATE_ACTION;
               printed = 0;
           }
           break;

       case STATE_ACTION:
    	   if (!printed)
    	   {
    		   char msg[100];

    		   HAL_UART_Transmit(&huart2, (uint8_t*)playerName, strlen(playerName), 10);

    		   sprintf(msg,
    		       ", You are in %s\r\n"
    		       "1. Grab item\r\n"
    		       "2. Clean room (use item first)\r\n"
    		       "3. Show map\r\n"
    		       "4. Leave\r\n\r\n",
    		       roomName[currentRoom]);

    		   HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 10);

    		   printed = 1;
    	   }

    	   if (dataAvail)
    	   {
    	       char c = rx_buff[0];
    	       dataAvail = 0;

    	       if (c == '1')
    	       {
    	           printed = 0;

    	           if (roomItemCount[currentRoom] > 0)
    	           {
    	               for (int i = 0; i < 3; i++)
    	               {
    	                   if (roomItems[currentRoom][i][0] != '\0')
    	                   {
    	                       char *item = roomItems[currentRoom][i];

    	                       if (inventoryCount < INVENTORY_SIZE)
    	                       {
    	                           strcpy(inventory[inventoryCount], item);
    	                           inventoryCount++;
    	                       }

    	                       HAL_UART_Transmit(&huart2,
    	                           (uint8_t*)"Item grabbed: \r\n",
    	                           strlen("Item grabbed: \r\n"),
    	                           10);

    	                       HAL_UART_Transmit(&huart2,
    	                           (uint8_t*)item,
    	                           strlen(item),
    	                           10);

    	                       HAL_UART_Transmit(&huart2,
    	                           (uint8_t*)"\r\n\r\n",
    	                           4,
    	                           10);

    	                       printItemArt(item);

    	                       roomItems[currentRoom][i][0] = '\0';
    	                       roomItemCount[currentRoom]--;

    	                       // compact shift
    	                       for (int j = i; j < 2; j++)
    	                       {
    	                           strcpy(roomItems[currentRoom][j],
    	                                  roomItems[currentRoom][j + 1]);
    	                       }

    	                       roomItems[currentRoom][2][0] = '\0';

    	                       break;
    	                   }
    	               }
    	           }
    	           else
    	           {
    	               HAL_UART_Transmit(&huart2,
    	                   (uint8_t*)"No item here! Are you grabbing air?\r\n\r\n",
    	                   strlen("No item here! Are you grabbing air?\r\n\r\n"),
    	                   10);
    	           }
    	       }
    	       else if (c == '2')
    	       {
    	           if (roomsClean[currentRoom] == 1)
    	           {
    	               HAL_UART_Transmit(&huart2,
    	                   (uint8_t*)"This room is already cleaned!\r\n\r\n",
    	                   strlen("This room is already cleaned!\r\n\r\n"),
    	                   10);

    	               printed = 0;   // show menu again
    	               break;
    	           }

    	           // normal flow
    	           if (difficulty == 1)
    	               currentPuzzle = rand() % NUM_EASY_PUZZLES;
    	           else
    	               currentPuzzle = rand() % NUM_HARD_PUZZLES;

    	           timeLimit = (difficulty == 1) ? 10000 : 5000;

    	           puzzleStartTime = HAL_GetTick();
    	           lastTimerPrint = 0;

    	           state = STATE_USE_ITEM;
    	           printed = 0;
    	       }
    	       else if (c == '3')
    	       {
    	           printMap();
    	           printed = 0;
    	       }
    	       else if (c == '4')
    	       {
    	           state = STATE_EXPLORE;
    	           printed = 0;
    	       }


    	   }break;
       case STATE_USE_ITEM:

           if (!printed)
           {
               showInventory();

               if (inventoryCount == 0)
               {
                   HAL_UART_Transmit(&huart2,
                       (uint8_t*)"No items to use!\r\n\r\n",
                       strlen("No items to use!\r\n\r\n"),
                       10);

                   state = STATE_ACTION;
                   break;
               }

               HAL_UART_Transmit(&huart2,
                   (uint8_t*)"Choose item number to use (or 0 to go back):\r\n",
                   strlen("Choose item number to use (or 0 to go back):\r\n"),
                   10);

               for (int i = 0; i < inventoryCount; i++)
               {
                   char msg[50];
                   sprintf(msg, "%d. %s\r\n", i + 1, inventory[i]);
                   HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 10);
               }

               HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, 10);

               printed = 1;
           }

           if (dataAvail)
           {
               char c = rx_buff[0];
               dataAvail = 0;

               // Go back
               if (c == '0')
               {
                   state = STATE_ACTION;
                   printed = 0;
                   break;
               }

               // Valid selection
               if (c >= '1' && c <= ('0' + inventoryCount))
               {
                   int index = c - '1';
                   char *chosenItem = inventory[index];
                   char *neededItem = requiredItem[currentRoom];

                   if (strcmp(chosenItem, neededItem) == 0)
                   {
                       strcpy(selectedItem, chosenItem);

                       HAL_UART_Transmit(&huart2,
                           (uint8_t*)"Correct item! Now solve the puzzle...\r\n\r\n",
                           strlen("Correct item! Now solve the puzzle...\r\n\r\n"),
                           10);

                       if (difficulty == 1)
                       {
                           currentPuzzle = rand() % NUM_EASY_PUZZLES;
                           timeLimit = 10000;
                       }
                       else
                       {
                           currentPuzzle = rand() % NUM_HARD_PUZZLES;
                           timeLimit = 5000;
                       }

                       puzzleStartTime = HAL_GetTick();
                       lastTimerPrint = 0;

                       state = STATE_PUZZLE;
                       printed = 0;
                   }
                   else
                   {
                       HAL_UART_Transmit(&huart2,
                           (uint8_t*)"Wrong item for this room! Try again.\r\n\r\n",
                           strlen("Wrong item for this room! Try again.\r\n\r\n"),
                           10);

                       state = STATE_ACTION;
                       printed = 0;
                   }
               }
               else
               {
                   HAL_UART_Transmit(&huart2,
                       (uint8_t*)"Invalid choice!\r\n\r\n",
                       strlen("Invalid choice!\r\n\r\n"),
                       10);

                   printed = 0;
               }
           }break;

       case STATE_PUZZLE:
    	   if (HAL_GetTick() - puzzleStartTime > timeLimit)
    	   {
    	       HAL_UART_Transmit(&huart2,
    	           (uint8_t*)"Time's up!\r\n",
    	           strlen("Time's up!\r\n"),
    	           10);
    	       HAL_UART_Transmit(&huart2, (uint8_t*)loseBanner, strlen(loseBanner), 100);

    	       state = STATE_GAME_OVER;
    	       printed = 0;
    	       break;
    	   }


    	   uint32_t now = HAL_GetTick();
    	   uint32_t remaining = (timeLimit - (now - puzzleStartTime)) / 1000;

    	   // print once per second
    	   if (now - lastTimerPrint >= 1000)
    	   {
    	       char msg[50];
    	       sprintf(msg, "\rTime left: %lu sec   ", remaining);
    	       HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 10);


    	       lastTimerPrint = now;
    	   }

           if (!printed)
           {
        	   char timerMsg[50];
        	   sprintf(timerMsg, "You have %lu seconds!\r\n", timeLimit / 1000);
        	   HAL_UART_Transmit(&huart2, (uint8_t*)timerMsg, strlen(timerMsg), 10);
        	   HAL_UART_Transmit(&huart2,
        	       (uint8_t*)"Press H to use hint (ONE TIME ONLY)\r\n\r\n",
        	       strlen("Press H to use hint (ONE TIME ONLY)\r\n\r\n"),
        	       10);

        	   if (difficulty == 1)
        	   {
        	       HAL_UART_Transmit(&huart2,
        	           (uint8_t*)easyQuestions[currentPuzzle],
        	           strlen(easyQuestions[currentPuzzle]),
        	           10);
        	   }
        	   else
        	   {
        	       HAL_UART_Transmit(&huart2,
        	           (uint8_t*)hardQuestions[currentPuzzle],
        	           strlen(hardQuestions[currentPuzzle]),
        	           10);
        	   }
               printed = 1;
           }

           if (dataAvail)
           {
               char c = rx_buff[0];
               dataAvail = 0;

               // convert to lowercase
               if (c >= 'A' && c <= 'Z')
                   c = c + 32;

               char correctAnswer;

               // convert to lowercase
               if (c >= 'A' && c <= 'Z')
                   c += 32;

               if (c == 'h')
               {
                   if (hintNPCUsed)
                   {
                       HAL_UART_Transmit(&huart2,
                           (uint8_t*)"NPC: You already used your only hint!\r\n\r\n",
                           strlen("NPC: You already used your only hint!\r\n\r\n"),
                           10);
                   }
                   else
                   {
                       HAL_UART_Transmit(&huart2,
                           (uint8_t*)"\r\n[Mysterious Helper appears]\r\n",
                           strlen("\r\n[Mysterious Helper appears]\r\n"),
                           100);

                       char *hint;

                       if (difficulty == 1)
                           hint = easyHints[currentPuzzle];
                       else
                           hint = hardHints[currentPuzzle];

                       HAL_UART_Transmit(&huart2,
                           (uint8_t*)"Hint: ",
                           strlen("Hint: "),
                           100);

                       HAL_UART_Transmit(&huart2,
                           (uint8_t*)hint,
                           strlen(hint),
                           100);

                       HAL_UART_Transmit(&huart2,
                           (uint8_t*)"\r\n\r\n",
                           4,
                           100);

                       hintNPCUsed = 1;
                   }

                   break;
               }

               if (difficulty == 1)
                   correctAnswer = easyAnswers[currentPuzzle];
               else
                   correctAnswer = hardAnswers[currentPuzzle];

               if (c == correctAnswer)
               {
                   HAL_UART_Transmit(&huart2,
                       (uint8_t*)"\r\nCorrect! Room cleaned!\r\n\r\n",
                       strlen("Correct! Room cleaned!\r\n\r\n"),
                       10);

                   addPoint();

                   state = STATE_CLEAN;
                   printed = 0;
               }
               else
               {
                   HAL_UART_Transmit(&huart2,
                       (uint8_t*)"Wrong!\r\n", strlen("Wrong!\r\n"), 10);
               }
           }
           break;

       case STATE_CLEAN:

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
           {
        	   HAL_UART_Transmit(&huart2, (uint8_t*)winBanner, strlen(winBanner), 100);



               state = STATE_GAME_OVER;
           }
           else
           {
               state = STATE_EXPLORE;
           }
           break;

       case STATE_GAME_OVER:

           if (!printed)
           {
               HAL_UART_Transmit(&huart2,
                   (uint8_t*)"\r\n=== GAME OVER ===\r\n",
                   strlen("\r\n=== GAME OVER ===\r\n"),
                   10);
               char msg[50];
               sprintf(msg, "Score: %d\r\n", score);
               HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 10);

               HAL_UART_Transmit(&huart2,
                   (uint8_t*)"Press P to play again\r\nPress Q to quit\r\n\r\n",
                   strlen("Press P to play again\r\nPress Q to quit\r\n\r\n"),
                   10);

               printed = 1;
           }

           if (dataAvail)
           {
               char c = rx_buff[0];
               dataAvail = 0;

               if (c >= 'A' && c <= 'Z')
                   c += 32;

               if (c == 'p')
               {
                   resetGame();
               }
               else if (c == 'q')
               {
                   HAL_UART_Transmit(&huart2,
                       (uint8_t*)"Goodbye!\r\n",
                       strlen("Goodbye!\r\n"),
                       10);

                   while (1); // stop program
               }
           }
           break;
       }

   }


  /* Game engine END WHILE */

   // Game over
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
    if (huart->Instance == USART2)
    {
        char c = rx_buff[0];

        // handle name input only when in name mode
        if (!nameEntered)
        {
            if (c == '\r' || c == '\n')
            {
                playerName[nameIndex] = '\0';
                nameEntered = 1;
            }
            else if (nameIndex < sizeof(playerName) - 1)
            {
                playerName[nameIndex++] = c;
            }
        }

        dataAvail = 1;

        HAL_UART_Receive_IT(&huart2, rx_buff, 1); // ONLY ONCE
    }
}

/* USER CODE BEGIN 4 */

void printMap(void)
{
    char msg[200];

    const int BOX_WIDTH = 24;

    HAL_UART_Transmit(&huart2,
        (uint8_t*)"====== MAP ======\r\n\r\n",
        strlen("====== MAP ======\r\n\r\n"),
        100);

    sprintf(msg, "YOU ARE IN: %s\r\n\r\n", roomName[currentRoom]);
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);

    for (int i = 0; i < 6; i++)
    {
        // Top border
        HAL_UART_Transmit(&huart2,
            (uint8_t*)"+------------------------+\r\n",
            strlen("+------------------------+\r\n"),
            100);

        // Room name line
        if (i == currentRoom)
            sprintf(msg, "    %s @\r\n", roomName[i]);
        else
            sprintf(msg, "    %s\r\n", roomName[i]);

        HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);

        // Items
        int hasItems = 0;

        for (int j = 0; j < 3; j++)
        {
            if (strlen(roomItems[i][j]) > 0)
            {
            	sprintf(msg, "    %s\r\n", roomItems[i][j]);
                HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
                hasItems = 1;
            }
        }

        // Empty room case
        if (!hasItems)
        {
        	sprintf(msg, "    %s\r\n", "(empty)");
            HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
        }
    }

    HAL_UART_Transmit(&huart2,
        (uint8_t*)"+------------------------+\r\n\r\n",
        strlen("+------------------------+\r\n\r\n"),
        100);

    HAL_UART_Transmit(&huart2,
        (uint8_t*)"LEGEND:\r\n@ = Player\r\n=================\r\n\r\n",
        strlen("LEGEND:\r\n@ = Player\r\n=================\r\n\r\n"),
        100);
}

void showInventory(void)
{
    HAL_UART_Transmit(&huart2,
        (uint8_t*)"=== INVENTORY ===\r\n",
        strlen("=== INVENTORY ===\r\n"),
        10);

    if (inventoryCount == 0)
    {
        HAL_UART_Transmit(&huart2,
            (uint8_t*)"Empty!\r\n\r\n",
            strlen("Empty!\r\n\r\n"),
            10);
        return;
    }

    for (int i = 0; i < inventoryCount; i++)
    {
        HAL_UART_Transmit(&huart2,
            (uint8_t*)inventory[i],
            strlen(inventory[i]),
            10);

        HAL_UART_Transmit(&huart2,
            (uint8_t*)"\r\n",
            2,
            10);
    }

    HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, 10);
}

void resetGame(void)
{
    inventoryCount = 0;
    cleanedRooms = 0;
    currentRoom = 0;

    difficulty = 0;
    hintNPCUsed = 0;
    currentPuzzle = 0;
    puzzleStartTime = 0;
    timeLimit = 0;
    lastTimerPrint = 0;

    printed = 0;

    // reset player name
    nameEntered = 0;
    nameIndex = 0;
    memset(playerName, 0, sizeof(playerName));

    // reset score
    score = 0;

    // restore room items
    memcpy(roomItems, roomItemsInit, sizeof(roomItemsInit));

    // reset rooms + recompute counts
    for (int i = 0; i < 6; i++)
    {
        roomsClean[i] = 0;

        roomItemCount[i] = 0;
        for (int j = 0; j < 3; j++)
        {
            if (roomItems[i][j][0] != '\0')
                roomItemCount[i]++;
        }
    }

    // clear inventory
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        inventory[i][0] = '\0';
    }

    state = STATE_MAIN_MENU;
}
/* USER CODE END 4 */

void printItemArt(char *itemName)
{
    for (int i = 0; i < 7; i++)
    {
        if (strcmp(items[i].name, itemName) == 0)
        {
            HAL_UART_Transmit(&huart2,
                (uint8_t*)items[i].art,
                strlen(items[i].art),
                100);
            return;
        }
    }
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

