# Project Overview

## Purpose of the Game
- Create a UART-driven interactive game combining logic puzzles with item-based gameplay using a microcontroller and serial communication

## Objectives
- Implement a state-machine-based game architecture in C and Assembly
- Handle real-time user input via UART communication Design interactive gameplay with puzzles, inventory, and room exploration

## Motivation
- Practice completing in-game chores under time pressure
- Develop habits that help reduce procrastination in real life tasks 

## Demonstrations
- Software: STEM 32 System Work Bench and PUTTY
Scenarios:
<img width="1936" height="532" alt="image" src="https://github.com/user-attachments/assets/a3a52d18-133e-4521-b85a-cbb0459c7718" />


## Screenshots
<img width="1919" height="1079" alt="image" src="https://github.com/user-attachments/assets/74de9f60-7bf5-4258-9ff8-09c058dbc825" />

## Flow Chart
- Rectangular: shows a process that the player will experience
- Dimond: Shows the choices that the player can choose and decide
<img width="761" height="1100" alt="image" src="https://github.com/user-attachments/assets/fc760c4e-1ba1-4184-83ba-dce6f4537faa" />


## State Diagram
- The game uses states to control the flow of the game
- Arrows show transitions based on player input
<img width="2742" height="1735" alt="Updated State Diagram Milestone 4 (2)" src="https://github.com/user-attachments/assets/84cb6d38-774f-4565-964a-291ea89eb136" />

## Discussions
### Limitations
- The game only uses text (ASCII) in the UART terminal
- Player cannot see typed input(no echo)

### Challenges
- Handle mixed input types (string input + single-character commands) 
- Text based game, count down timers will block the entire game since it updates and prints out time left every lines

### Addresses
- Change how a single key press is interpreted
- In the code, put \r → go back to start of line no \n → don’t go to a new line
  - Example: Before, `Time left: %lu sec\r\n`, after `\rTime left: %lu sec`



## Conclusions
### Key Takeaways 
- Learned GitHub workflow: branches, pull requests, and merging
- Gained understanding of UART communication in embedded systems
- Improved use of state machines and input handling

### Future Directions 
- Add a global timer and scoring system that calculates based on the global timer
- Expand NPC interactions and gameplay depth
- Add more than one NPC
- Increase puzzle/question bank for replayability
- Add real-world scenario-based challenges

## Acknowledgements
- Class professor, Dr. Chin Craig from Kennesaw State University, Marietta, GA
- Gabriel Satterfield, classmate who introduced Cursor AI to help me understand more about UART
- STMicroelectronics Community
- STM32 UART Getting Started Guide https://wiki.st.com/stm32mcu/wiki/Getting_started_with_UART
- Tutorials of UART functions https://github.com/dekuNukem/STM32_tutorials/tree/master or https://github.com/dekuNukem/STM32_tutorials/blob/master/lesson1_serial_helloworld/HAL_UART_Transmit_details.md


## Resources
- Feel free to check out the game for reference if you are taking this class
