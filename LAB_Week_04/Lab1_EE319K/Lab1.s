;****************** main.s ***************
; Program initially written by: Yerraballi and Valvano
; Author: Place your name here
; Date Created: 1/15/2018 
; Last Modified: 12/30/2022 
; Brief description of the program: Solution to Lab1
; The objective of this system is to implement a parity system
; Hardware connections: 
;  One output is positive logic, 1 turns on the LED, 0 turns off the LED
;  Three inputs are positive logic, meaning switch not pressed is 0, pressed is 1
GPIO_PORTD_DATA_R  EQU 0x400073FC
GPIO_PORTD_DIR_R   EQU 0x40007400
GPIO_PORTD_DEN_R   EQU 0x4000751C
GPIO_PORTE_DATA_R  EQU 0x400243FC
GPIO_PORTE_DIR_R   EQU 0x40024400
GPIO_PORTE_DEN_R   EQU 0x4002451C
SYSCTL_RCGCGPIO_R  EQU 0x400FE608
       PRESERVE8 
       AREA   Data, ALIGN=2
; Declare global variables here if needed
; with the SPACE assembly directive
       ALIGN 4
       AREA    |.text|, CODE, READONLY, ALIGN=2
       THUMB
       EXPORT EID
EID    DCB "1234567890",0  ;replace ABC123 with your EID
       EXPORT RunGrader
      ALIGN 4
RunGrader DCD 1 ; change to nonzero when ready for grading
           
      EXPORT  Lab1

Lab1   
 ;initialization
 nop
 
 LDR R0, =SYSCTL_RCGCGPIO_R
 LDR R1, [R0]
 ORR R1, #0x10
 STR R1, [R0]
 nop
 nop
 
 LDR R0, =GPIO_PORTE_DEN_R
 LDR R1, [R0]
 ORR R1, #0x27
 STR R1, [R0]
 
 LDR R0, =GPIO_PORTE_DIR_R
 LDR R1, [R0]
 ORR R1, #0x20
 BIC R1, #0x07
 STR R1, [R0]
 
loop
   ;main program loop
   MOV R3, #0x00
   LDR R0, =GPIO_PORTE_DATA_R
   LDR R1, [R0]
   AND R2, R1, #0x01
   EOR R3, R2
   LSR R1, #1
   AND R2, R1, #0x01
   EOR R3, R2
   LSR R1, #1
   AND R2, R1, #0x01
   EOR R3, R2
   
   CMP R3, #0
   LDR R1, [R0]
   ORREQ R1, #0x20
   BICNE R1, #0x20
   STR R1,[R0]
    B    loop
    
    ALIGN        ; make sure the end of this section is aligned
    END          ; end of file
               
