// ADC.c
// Runs on TM4C123
// Provide functions that initialize ADC0
// Last Modified: 1/2/2023  
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly
// This file is in the inc folder so it automatically will be applied to labs 8 9 and 10
// Labs 8, 9, and 10 specify PD2
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

// ADC initialization function using PD2 
// Input: none
// Output: none
void ADC_Init(void){ 
// write this
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD; // 1) activate clock for Port D
  while(!(SYSCTL_PRGPIO_R&SYSCTL_RCGC2_GPIOD)){}; // allow time for clock to start
  GPIO_PORTD_DIR_R &= ~0x04; // 2) make PD2 input
  GPIO_PORTD_AFSEL_R |= 0x04; // 3) enable alternate function on PD2
  GPIO_PORTD_DEN_R &= ~0x04; // 4) disable digital I/O on PD2
  GPIO_PORTD_AMSEL_R |= 0x04; // 5) enable analog function on PD2
  SYSCTL_RCGCADC_R |= 0x01; // 6) activate ADC0
  while(!(SYSCTL_PRADC_R&0x01)){}; // allow time for ADC to start
  ADC0_PC_R = 0x01; // 7) configure for 125K samples/sec
  ADC0_SSPRI_R = 0x0123; // 8) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x08; // 9) disable sample sequencer 3
  ADC0_EMUX_R &= ~0x0F00; // 10) seq3 is software trigger
  ADC0_SSMUX3_R = (ADC0_SSMUX3_R&~0x0F) | 5; // 11) set channel to 5
  ADC0_SSCTL3_R = 0x06; // 12) set flag at 1st sample
  ADC0_IM_R &= ~0x08; // 13) disable SS3 interrupts
  ADC0_ACTSS_R |= 0x08; // 14) enable sample sequencer 3
}
//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
// measures from PD2, analog channel 5
uint32_t ADC_In(void){ uint32_t result;
  // 1) initiate SS3
  ADC0_PSSI_R = 0x08;
  // 2) wait for conversion done
  while((ADC0_RIS_R&0x08)==0){};
  // 3) read result
  result = ADC0_SSFIFO3_R&0xFFF;
  // 4) acknowledge completion
  ADC0_ISC_R = 0x08;
  return result; // return result
}


