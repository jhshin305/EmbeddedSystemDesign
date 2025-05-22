// IO.c
// This software configures the switch and LED
// You are allowed to use any switch and any LED, 
// although the Lab suggests the SW1 switch PF4 and Red LED PF1
// Runs on TM4C123
// Program written by: put your names here
// Date Created: 
// Last Modified:  August 24, 2022
// Lab number: 7


#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>


void Wait10ms(uint32_t);

//------------IO_Init------------
// Initialize GPIO Port for a switch and an LED
// Input: none
// Output: none
void IO_Init(void) { volatile uint32_t delay;
 // --UUU-- Code to initialize PF4 and PF2
SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
	Wait10ms(delay);
  GPIO_PORTF_DEN_R |= 0x14;   // PF4 and PF2 digital enable
  GPIO_PORTF_PUR_R |= 0x10;   // PF4 pull-up enable
  GPIO_PORTF_DIR_R |= 0x04;   // PF2 output
  GPIO_PORTF_DIR_R &= ~0x10;  // PF4 input
}

//------------IO_HeartBeat------------
// Toggle the output state of the  LED.
// Input: none
// Output: none
void IO_HeartBeat(void) {
 // --UUU-- PF2 is heartbeat
  GPIO_PORTF_DATA_R ^= 0x04;  // toggle PF2
}


//------------IO_Touch------------
// wait for release and press of the switch
// Input: none
// Output: none
void IO_Touch(void) {
 // --UUU-- wait for release; delay for 20ms; and then wait for press
  while((GPIO_PORTF_DATA_R&0x10) == 0x00){} // wait for PF4 to be released
  Wait10ms(2);                              // wait 20ms
  while((GPIO_PORTF_DATA_R&0x10) == 0x10){} // wait for PF4 to be pressed
}  

//Must really wait for 10ms x count
void Wait10ms(uint32_t count){
  uint32_t i;
  for(i=0; i <count; i++) {
		uint8_t n = 10;
		while(n){
      uint32_t time = 72724*2/91;  // 1msec, tuned at 80 MHz
      while(time){
        time--;
      }
      n--;
  }
	}
}
