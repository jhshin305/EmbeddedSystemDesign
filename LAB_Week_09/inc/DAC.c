// DAC.c
// This software configures DAC output
// Labs 6 and 10 requires 6 bits for the DAC
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 1/2/23 
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data

// **************DAC_Init*********************
// Initialize 6-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void){
  // used in Lab 6 and lab 10
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGC2_GPIOB;  // activate port B
  volatile uint32_t time = 6665;            // 1msec
  while(time) time--;
  GPIO_PORTB_DIR_R |= 0x3F;       // PB5-0 output
  GPIO_PORTB_DEN_R |= 0x3F;       // PB5-0 enable digital I/O
  GPIO_PORTB_DR8R_R |= 0x3F;      // PB5-0 8mA
  GPIO_PORTB_DATA_R &= ~0x3F;     // PB5-0 clear
}

// **************DAC_Out*********************
// output to DAC
// Input: 6-bit data, 0 to 63 
// Input=n is converted to n*3.3V/63
// Output: none
void DAC_Out(uint32_t data){
  // used in Lab 6 and lab 10
  GPIO_PORTB_DATA_R = data; // output to PB5-0
}
