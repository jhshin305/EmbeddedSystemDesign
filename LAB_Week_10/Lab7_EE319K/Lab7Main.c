// Lab7Main.c
// Runs on TM4C123
// Test the functions in ST7735.c by printing basic
// patterns to the LCD.
//    16-bit color, 128 wide by 160 high LCD
// Daniel Valvano
// Last Modified: 1/2/2023
// Ramesh Yerraballi modified 3/20/2017

// hardware connections
// **********ST7735 TFT and SDC*******************
// ST7735
// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

// **********wide.hk ST7735R with ADXL345 accelerometer *******************
// Silkscreen Label (SDC side up; LCD side down) - Connection
// VCC  - +3.3 V
// GND  - Ground
// !SCL - PA2 Sclk SPI clock from microcontroller to TFT or SDC
// !SDA - PA5 MOSI SPI data from microcontroller to TFT or SDC
// DC   - PA6 TFT data/command
// RES  - PA7 TFT reset
// CS   - PA3 TFT_CS, active low to enable TFT
// *CS  - (NC) SDC_CS, active low to enable SDC
// MISO - (NC) MISO SPI data from SDC to microcontroller
// SDA  � (NC) I2C data for ADXL345 accelerometer
// SCL  � (NC) I2C clock for ADXL345 accelerometer
// SDO  � (NC) I2C alternate address for ADXL345 accelerometer
// Backlight + - Light, backlight connected to +3.3 V

// **********wide.hk ST7735R with ADXL335 accelerometer *******************
// Silkscreen Label (SDC side up; LCD side down) - Connection
// VCC  - +3.3 V
// GND  - Ground
// !SCL - PA2 Sclk SPI clock from microcontroller to TFT or SDC
// !SDA - PA5 MOSI SPI data from microcontroller to TFT or SDC
// DC   - PA6 TFT data/command
// RES  - PA7 TFT reset
// CS   - PA3 TFT_CS, active low to enable TFT
// *CS  - (NC) SDC_CS, active low to enable SDC
// MISO - (NC) MISO SPI data from SDC to microcontroller
// X� (NC) analog input X-axis from ADXL335 accelerometer
// Y� (NC) analog input Y-axis from ADXL335 accelerometer
// Z� (NC) analog input Z-axis from ADXL335 accelerometer
// Backlight + - Light, backlight connected to +3.3 V

// **********HiLetgo ST7735 TFT and SDC (SDC not tested)*******************
// ST7735
// LED-   (pin 16) TFT, connected to ground
// LED+   (pin 15) TFT, connected to +3.3 V
// SD_CS  (pin 14) SDC, chip select
// MOSI   (pin 13) SDC, MOSI
// MISO   (pin 12) SDC, MISO
// SCK    (pin 11) SDC, serial clock
// CS     (pin 10) TFT, PA3 (SSI0Fss)
// SCL    (pin 9)  TFT, SCK  PA2 (SSI0Clk)
// SDA    (pin 8)  TFT, MOSI PA5 (SSI0Tx)
// A0     (pin 7)  TFT, Data/Command PA6 (GPIO), high for data, low for command
// RESET  (pin 6)  TFT, to PA7 (GPIO)
// NC     (pins 3,4,5) not connected
// VCC    (pin 2)  connected to +3.3 V
// GND    (pin 1)  connected to ground

// Tyenaza Tyenazaqhf72mi9s3
// ST7735
// LED (pin 8) connected to +3.3 V
// SCK (pin 7) connected to PA2 (SSI0Clk)
// SDA (pin 6) MOSI, connected to PA5 (SSI0Tx)
// A0  (pin 5) Data/Command connected to PA6 (GPIO), high for data, low for command
// RESET (pin 4) connected to PA7 (GPIO)
// CS  (pin 3) connected to PA3 (SSI0Fss)
// Gnd (pin 2) connected to ground
// VCC (pin 1) connected to +3.3 V

#include <stdio.h>
#include <stdint.h>
#include "../inc/ST7735.h"
#include "../inc/PLL.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/IO.h"
#include "../inc/StringConversion.h"
#include "images.h"
#include "Lab7Grader.h"
// Modify these two with your EIDs
char EID1[]="2020311573";  // student's EID
char EID2[]="2020311573";  // student's EID
#define SIZE 16
uint32_t const TestData[SIZE] ={
  0,7,99,100,654,999,1000,5009,9999,10000,20806,65535,
  103456,12000678,123400009,0xFFFFFFFF
};


int main(void){  
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
    int j = 1000;
    while(j--){}
  uint32_t i;
  PLL_Init(Bus80MHz);    
  TExaS_Init(GRADER); // set system clock to 80 MHz
  // 1) implement SPIOutCommand and SPIOutData
  ST7735_InitR(INITR_REDTAB);
  ST7735_OutString("Lab 7 Spring 2023\n\xADHola!\nBienvenida al EE319K");
  // 2) remove lines the next two lines if the ST7735_OutString outputs correctly
//  while(1){
//  }
  IO_Init();
  IO_Touch();

  ST7735_FillScreen(0xFFFF);   // set screen to white
  ST7735_DrawBitmap(44, 159, Logo, 40, 160);
  IO_Touch();
  ST7735_FillScreen(0);       // set screen to black
  for(i=0;i<SIZE;i++){
    
    IO_HeartBeat();
    ST7735_SetCursor(0,i);
    ST7735_OutUDec(TestData[i]);
    ST7735_SetCursor(11,i);
    ST7735_OutFix(TestData[i]);
    IO_Touch(); // remove this line to see all test cases
  }
  while(1){
  }
}

