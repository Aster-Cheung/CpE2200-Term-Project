.syntax unified
.cpu cortex-m4
.fpu softvfp
.thumb
.section .text
.align
.global strlen
strlen:
PUSH {r4, lr} // preserve r4 and lr
MOV r4, #0 // initialize length
loop: LDRB r1, [r0, r4] // r0 = string address
CBZ r1, exit // branch if zero
ADD r4, r4, #1 // length++
B loop // do it again
exit: MOV r0, r4 // place result in r0
POP {r4, pc} // exit


addPoint:
    LDR R0, =playerScore   @ load address of score
    LDR R1, [R0]           @ load current score
    ADD R1, R1, #1         @ increment by 1
    STR R1, [R0]           @ store back
    BX LR                  @ return
