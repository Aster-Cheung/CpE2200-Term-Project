# Project Overview

## Purpose of the Game
- Create a UART-driven interactive game combining logic puzzles with item-based gameplay using a microcontroller and serial communication

## Objectives
- Implement a state-machine-based game architecture in C and Assembly
- Handle real-time user input via UART communication Design interactive gameplay with puzzles, inventory, and room exploration

## Motivation
- Practice completing in-game chores under time pressure 
= Develop habits that help reduce procrastination in real life tasks 

## Demonstrations
- Software: STEM 32 System Work Bench and PUTTY
Scenarios:
<img width="1964" height="550" alt="image" src="https://github.com/user-attachments/assets/b0360fc5-c137-4ea9-ba42-b7205e6ac933" />

## Screenshots
- ASCII art image of Bug Spray (Pick up item)
<img width="563" height="581" alt="image" src="https://github.com/user-attachments/assets/bcbeb774-b040-47dd-a345-df6244e7c235" />


## Flow Chart
- 

## State Diagram
<img width="1736" height="895" alt="image" src="https://github.com/user-attachments/assets/ff2a65b2-8e86-4440-b644-04f7b0c46107" />

## Discussions
### Limitations
- The game only uses text (ASCII) in the UART terminal
- Player cannot see typed input(no echo)

### Challenges
- Handle mixed input types (string input + single-character commands) 
- Text based game, count down timers will block the entire game since it updates and prints out time left every lines

### Addresses
- Change how a single key press is interpreted


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
