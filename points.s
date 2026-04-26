.syntax unified
.cpu cortex-m4
.fpu softvfp
.thumb
.section .text
.align
.global addPoint:

addPoint:
    LDR R0, =playerScore   // load address of score
    LDR R1, [R0]           // load current score
    ADD R1, R1, #1         // increment by 1
    STR R1, [R0]           // store back
    BX LR                  // return
