// Lab6.c
// Runs on TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// EE319K lab6 starter
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 1/2/23  
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "Sound.h"
#include "Key.h"
#include "Song.h"
#include "../inc/wave.h"
#include "Lab6Grader.h"

// Period =  80000000/64/Freq=1250000/Freq
#define C1   597   // 2093 Hz
#define B1   633   // 1975.5 Hz
#define BF1  670   // 1864.7 Hz
#define A1   710   // 1760 Hz
#define AF1  752   // 1661.2 Hz
#define G1   797   // 1568 Hz
#define GF1  845   // 1480 Hz
#define F1   895   // 1396.9 Hz
#define E1   948   // 1318.5 Hz
#define EF1  1004   // 1244.5 Hz
#define D1   1064   // 1174.7 Hz
#define DF1  1127   // 1108.7 Hz
#define C    1194   // 1046.5 Hz
#define B    1265   // 987.8 Hz
#define BF   1341   // 932.3 Hz
#define A    1420   // 880 Hz
#define AF   1505   // 830.6 Hz
#define G    1594   // 784 Hz
#define GF  1689   // 740 Hz
#define F   1790   // 698.5 Hz
#define E   1896   // 659.3 Hz
#define EF  2009   // 622.3 Hz
#define D   2128   // 587.3 Hz
#define DF  2255   // 554.4 Hz
#define C0  2389   // 523.3 Hz
#define B0  2531   // 493.9 Hz
#define BF0 2681   // 466.2 Hz
#define A0  2841   // 440 Hz
#define AF0 3010   // 415.3 Hz
#define G0  3189   // 392 Hz
#define GF0 3378   // 370 Hz
#define F0  3579   // 349.2 Hz
#define E0  3792   // 329.6 Hz
#define EF0 4018   // 311.1 Hz
#define D0  4257   // 293.7 Hz
#define DF0 4510   // 277.2 Hz
#define C7  4778   // 261.6 Hz
#define B7  5062   // 246.9 Hz
#define BF7 5363   // 233.1 Hz
#define A7  5682   // 220 Hz
#define AF7 6020   // 207.7 Hz
#define G7  6378   // 196 Hz
#define GF7 6757   // 185 Hz
#define F7  7159   // 174.6 Hz
#define E7  7584   // 164.8 Hz
#define EF7 8035   // 155.6 Hz
#define D7  8513   // 146.8 Hz
#define DF7 9019   // 138.6 Hz
#define C6  9556   // 130.8 Hz

// put both EIDs in the next two lines
char EID1[] = "2020311573"; //  ;replace abc123 with your EID
char EID2[] = "2020311573"; //  ;replace abc123 with your EID

// Lab 6, Spring 2023, EID1=2020311573, EID2=2020311573
// Key3=PA5, Key2=PA4, Key1=PA3, Key0=PA2, DAC=PB5-0
// Key0=277.2, Key1=349.2, Key2=415.3, Key3=466.2 Hz

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void DAC_Init(void);          // your lab 6 solution
void DAC_Out(uint8_t data);   // your lab 6 solution


uint8_t Testdata;

// lab video Lab6_voltmeter
// use this if you have a voltmeter
// 1) connect voltmeter to DACOUT 
// 2) add a breakpoint at i+1
// 3) run and record voltage for each input value
const uint32_t Inputs[16]={0,1,7,8,15,16,17,18,31,32,33,47,48,49,62,63};

int voltmetermain(void){ uint32_t i;  
  DisableInterrupts();
  TExaS_Init(SCOPE);    
  LaunchPad_Init();
  DAC_Init(); // your lab 6 solution
  i = 0;
  EnableInterrupts();
  while(1){                
    Testdata = Inputs[i];
    DAC_Out(Testdata); // your lab 6 solution
    i=(i+1)&0x0F;  // <---put a breakpoint here
  }
}
// DelayMs
//  - busy wait n milliseconds
// Input: time to wait in msec
// Outputs: none
void static DelayMs(uint32_t n){
  volatile uint32_t time;
  while(n){
    time = 6665;  // 1msec, tuned at 80 MHz
    while(time){
      time--;
    }
    n--;
  }
}
// lab video Lab6_static
// use this if you DO NOT have a voltmeter
// you need to implement LaunchPad_Init and LaunchPad_Input
// Connect PD3 to your DACOUT and observe the voltage using TExaSdisplay in scope mode.
int staticmain(void){  
  uint32_t last,now,i;  
  TExaS_Init(SCOPE);    // bus clock at 80 MHz
  LaunchPad_Init();
  DAC_Init(); // your lab 6 solution
  i = 0;
  EnableInterrupts();
  last = LaunchPad_Input();
  while(1){                
    now = LaunchPad_Input();
    if((last != now)&&now){
      Testdata = Inputs[i];
      DAC_Out(Testdata); // your lab 6 solution
      i=(i+1)&0x0F;
    }
    last = now;
    DelayMs(25);   // debounces switch
  }
}

// Key0=277.2, Key1=349.2, Key2=415.3, Key3=466.2 Hz
void GPIOPortA_Handler(void){
  uint32_t inter = GPIO_PORTA_RIS_R;
  GPIO_PORTA_ICR_R = inter;
  uint32_t data = Key_In(); // read keys
  if(data&(1 << 0)){            // Key0 pressed
    Sound_Start(DF0); 
  }else if(data&(1 << 1)){      // Key1 pressed
    Sound_Start(F0); 
  }else if(data&(1 << 2)){      // Key2 pressed
    Sound_Start(AF0); 
  }else if(data&(1 << 3)){      // Key3 pressed
    Sound_Start(BF0); 
  }
	else Sound_Off();         // no key pressed, turn off sound
}
     
int main(void){       
  DisableInterrupts();
  TExaS_Init(SCOPE);    // bus clock at 80 MHz
//    SCOPE,
//    LOGICANALYZERA,
//    LOGICANALYZERB,
//    LOGICANALYZERC,
//    LOGICANALYZERE,
//    REALBOARDGRADER,
//    SIMULATIONGRADER,
//    NONE
  Key_Init();
  DAC_Init();
  LaunchPad_Init(); // if needed
  Sound_Init();
  Song_Init(); // extra credit 1)
  Wave_Init(); // extra credit 2)
  EnableInterrupts();

  while(1){
  }             
}