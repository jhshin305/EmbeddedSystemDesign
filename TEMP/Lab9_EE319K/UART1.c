// UART1.c
// Runs on LM4F120/TM4C123
// Use UART1 to implement bidirectional data transfer to and from 
// another microcontroller in Lab 9.  This time, interrupts and FIFOs
// are used.
// Daniel Valvano
// 1/2/2023

/* Lab solution, Do not post
 http://users.ece.utexas.edu/~valvano/
*/

// U1Rx (VCP receive) connected to PC4
// U1Tx (VCP transmit) connected to PC5
#include <stdint.h>
#include "Fifo.h"
#include "UART1.h"
#include "../inc/tm4c123gh6pm.h"

// Initialize UART1
// Baud rate is 2000 bits/sec
void UART1_Init(void){
  // write this
}

//------------UART1_InChar------------
// Wait for new input, 
// Use your FIFO, do not access UART1 hardware
// then return ASCII code
// Input: none
// Output: char read from UART
char UART1_InChar(void){
  // write this
  return(0); // remove this line
}
//------------UART1_InMessage------------
// Accepts ASCII characters from the serial port
//    and adds them to a string until > is typed
//    or until max length of the string is reached.
// Input: pointer to empty buffer of 8 characters
// Output: Null terminated string
void UART1_InMessage(char *bufPt){
  // write this if you need it
}

//------------UART1_OutChar------------
// Output 8-bit to serial port
// Do not use interrupts or FIFO, use busy wait
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART1_OutChar(char data){
  // write this

}
#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
// RTRIS UART1 receiver has timed out
// Put data into your FIFO
void UART1_Handler(void){char letter;
  // write this
//0) toggle a heartbeat (change from 0 to 1, or from 1 to 0), 
//1) toggle a heartbeat (change from 0 to 1, or from 1 to 0), 
//2) as long as the RXFE bit in the UART1_FR_R is zero (should be exactly 8 bytes, but could be more or less)
//   -Read a byte from UART1_DR_R 
//   -Put the byte into your software FIFO 
//3) Increment a RxCounter, used as debugging monitor of the number of UART messages received
//4) acknowledge the interrupt by clearing the flag which requested the interrupt 
  UART1_ICR_R = 0x40;   // this clears bit 6 (RTRIS) in the RIS register
// 5) toggle a heartbeat (change from 0 to 1, or from 1 to 0), 

}





