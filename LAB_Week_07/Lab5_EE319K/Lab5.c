// Lab5.c starter program EE319K Lab 5, Spromg 2023
// Runs on TM4C123
// Put your names here

// 12/31/2022


/* Option A1, connect LEDs to PB5-PB0, switches to PA5-3, walk LED PF321
   Option A2, connect LEDs to PB5-PB0, switches to PA4-2, walk LED PF321
   Option A6, connect LEDs to PB5-PB0, switches to PE3-1, walk LED PF321
   Option A5, connect LEDs to PB5-PB0, switches to PE2-0, walk LED PF321
   Option B4, connect LEDs to PE5-PE0, switches to PC7-5, walk LED PF321
   Option B3, connect LEDs to PE5-PE0, switches to PC6-4, walk LED PF321
   Option B1, connect LEDs to PE5-PE0, switches to PA5-3, walk LED PF321
   Option B2, connect LEDs to PE5-PE0, switches to PA4-2, walk LED PF321
  */
// east/west red light connected to bit 5
// east/west yellow light connected to bit 4
// east/west green light connected to bit 3
// north/south facing red light connected to bit 2
// north/south facing yellow light connected to bit 1
// north/south facing green light connected to bit 0
// pedestrian detector connected to most significant bit (1=pedestrian present)
// north/south car detector connected to middle bit (1=car present)
// east/west car detector connected to least significant bit (1=car present)
// "walk" light connected to PF3-1 (built-in white LED)
// "don't walk" light connected to PF3-1 (built-in red LED)
#include <stdint.h>
#include "SysTick.h"
#include "Lab5grader.h"
#include "../inc/Debug.h"
#include "../inc/tm4c123gh6pm.h"
// put both EIDs in the next two lines
char EID1[] = "2020311573"; //  ;replace abc123 with your EID
char EID2[] = "2020311573"; //  ;replace abc123 with your EID

// Lab 5, Spring 2023EID1=2020311573, EID2=2020311573
// Option A6, connect LEDs to PB5-PB0, switches to PE3-1, walk LED PF321
// When all inputs true, ... South, Walk, West, South, Walk, West, South, Walk, West, ...

void DisableInterrupts(void);
void EnableInterrupts(void);

enum State {sG, sYw, sYW, wG, wYs, wYW, walkW, walkR1s, walkO1s, walkR2s, walkO2s, walkR1w, walkO1w, walkR2w, walkO2w, ASs, ASw, ASW};
struct state {
  uint8_t traffic;
  uint8_t walk;
  uint8_t next[8]; // next state for each input
};

#define S_RED 0x04
#define S_YELLOW 0x02
#define S_GREEN 0x01
#define W_RED 0x20
#define W_YELLOW 0x10
#define W_GREEN 0x08
#define WALK_RED 0x02
#define WALK_WHITE 0x0E
#define WALK_OFF 0x00

// walk: GBRx
const struct state states[18] = {
  // input:                       x, w, s, sw, W, wW, sW, swW
  {S_GREEN | W_RED, WALK_RED,   {sG, sYw, sG, sYw, sYW, sYW, sYW, sYW}},                                    // south green
  {S_YELLOW | W_RED, WALK_RED,  {ASw, ASw, ASw, ASw, ASw, ASw, ASw, ASw}},                                  // south yellow to west green
  {S_YELLOW | W_RED, WALK_RED,  {ASW, ASW, ASW, ASW, ASW, ASW, ASW, ASW}},                                  // south yellow to walk white
  {S_RED | W_GREEN, WALK_RED,   {wG, wG, wYs, wYs, wYW, wYW, wYs, wYs}},                                    // west green
  {S_RED | W_YELLOW, WALK_RED,  {ASs, ASs, ASs, ASs, ASs, ASs, ASs, ASs}},                                  // west yellow to south green
  {S_RED | W_YELLOW, WALK_RED,  {ASW, ASW, ASW, ASW, ASW, ASW, ASW, ASW}},                                  // west yellow to walk white
  {S_RED | W_RED, WALK_WHITE,   {walkW, walkR1w, walkR1s, walkR1s, walkW, walkR1w, walkR1s, walkR1w}},      // walk white
  {S_RED | W_RED, WALK_RED,     {walkO1s, walkO1s, walkO1s, walkO1s, walkO1s, walkO1s, walkO1s, walkO1s}},  // walk blink1 red to south green
  {S_RED | W_RED, WALK_OFF,     {walkR2s, walkR2s, walkR2s, walkR2s, walkR2s, walkR2s, walkR2s, walkR2s}},  // walk blink2 off to south green
  {S_RED | W_RED, WALK_RED,     {walkO2s, walkO2s, walkO2s, walkO2s, walkO2s, walkO2s, walkO2s, walkO2s}},  // walk blink3 red to south green
  {S_RED | W_RED, WALK_OFF,     {ASs, ASs, ASs, ASs, ASs, ASs, ASs, ASs}},                                  // walk blink4 off to south green
  {S_RED | W_RED, WALK_RED,     {walkO1w, walkO1w, walkO1w, walkO1w, walkO1w, walkO1w, walkO1w, walkO1w}},  // walk blink1 red to west green
  {S_RED | W_RED, WALK_OFF,     {walkR2w, walkR2w, walkR2w, walkR2w, walkR2w, walkR2w, walkR2w, walkR2w}},  // walk blink2 off to west green
  {S_RED | W_RED, WALK_RED,     {walkO2w, walkO2w, walkO2w, walkO2w, walkO2w, walkO2w, walkO2w, walkO2w}},  // walk blink3 red to west green
  {S_RED | W_RED, WALK_OFF,     {ASw, ASw, ASw, ASw, ASw, ASw, ASw, ASw}},                                  // walk blink4 off to west green
  {S_RED | W_RED, WALK_RED,     {sG, sG, sG, sG, sG, sG, sG, sG}},                                          // all stop to south green
  {S_RED | W_RED, WALK_RED,     {wG, wG, wG, wG, wG, wG, wG, wG}},                                          // all stop to west green
  {S_RED | W_RED, WALK_RED,     {walkW, walkW, walkW, walkW, walkW, walkW, walkW, walkW}}   	              // all stop to walk white
};

uint8_t currState = sG;
uint8_t input = 0;

int main(void){ 
  DisableInterrupts();
  TExaS_Init(5);
  Debug_Init(); // Index=0 means buffer is empty
  // Initialize SysTick for software waits
  SysTick_Init(); // 80 MHz, 10ms
  // Initialize GPIO ports
  SYSCTL_RCGC2_R |= 0x32; // activate port b, e, f
	SysTick_Wait10ms(1);
  GPIO_PORTB_DEN_R |= 0x3F;  // PB5-PB0 are outputs
  GPIO_PORTB_DIR_R |= 0x3F;
  GPIO_PORTB_DATA_R = 0x00;
  GPIO_PORTE_DEN_R |= 0x0E;  // PE3-1 are inputs
  GPIO_PORTE_DIR_R &= 0xF1;
  GPIO_PORTF_DEN_R |= 0x0E;  // PF3-1 are outputs
  GPIO_PORTF_DIR_R |= 0x0E;
  GPIO_PORTF_DATA_R = 0x00;
  
  // Specify initial atate
	//uint8_t currState = sG;

  EnableInterrupts(); // grader, scope, logic analyzer need interrupts
  while(1){
    // set traffic lights
    GPIO_PORTB_DATA_R = states[currState].traffic; // set traffic lights
    // set walk lights
    GPIO_PORTF_DATA_R = states[currState].walk; // set walk lights
    // call Debug_Dump with your state, input, traffic, walk information
    Debug_Dump(currState, input); // record state and input
    // wait
    SysTick_Wait10ms(50);
    // read input
    input = (GPIO_PORTE_DATA_R & 0x0E) >> 1; // PE3-1 are inputs
    // next state depends on input and current state  
    currState = states[currState].next[input]; // next state
  }
}



