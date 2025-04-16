// SysTick.c
// Runs on TM4C123
// Provide functions that initialize the SysTick module, wait at least a
// designated number of clock cycles, and wait approximately a multiple
// of 10 milliseconds using busy wait. 
// Daniel Valvano
// December 31, 2022
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"



// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(void){
    // ECE319K students write this function
    NVIC_ST_CTRL_R = 0x5;
    NVIC_ST_RELOAD_R = 0x00FFFFFF; // maximum reload value
}
// The delay parameter is in units of the 80 MHz core clock. (12.5 ns)
void SysTick_Wait(uint32_t delay){
  // ECE319K students write this function
  NVIC_ST_RELOAD_R = delay - 1;
  NVIC_ST_CURRENT_R = 0;
  while((NVIC_ST_CTRL_R&0x00010000)==0){}

}
// 10000us equals 10ms
void SysTick_Wait10ms(uint32_t delay){
    // ECE319K students write this function
    for(uint32_t i = 0; i < delay; i++){
        SysTick_Wait(480000); // wait 10ms
    }
}

