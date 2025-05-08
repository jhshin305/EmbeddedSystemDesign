; BusyWait.s
; Student names: change this to your names or look very silly
; Last modification date: change this to the last modification date or look very silly

; Runs on TM4C123

; As part of Lab 7, students need to implement these two functions
; This file is in the inc folder so it automatically will be applied to labs 7 8 9 and 10


      EXPORT   SPIOutCommand
      EXPORT   SPIOutData

      AREA    |.text|, CODE, READONLY, ALIGN=2
      THUMB
      ALIGN
; Used in ECE319K Labs 7,8,9,10. You write these two functions

; ***********SPIOutCommand*****************
; This is a helper function that sends an 8-bit command to the LCD.
; Inputs: R0 = 32-bit command (number)
;         R1 = 32-bit SPI status register address
;         R2 = 32-bit SPI data register address
;         R3 = 32-bit GPIO port address for D/C
; Outputs: none
; Assumes: SPI and GPIO have already been initialized and enabled
; Note: must be AAPCS compliant
; Note: access to bit 6 of GPIO must be friendly
SPIOutCommand
; --UUU-- Code to write a command to the LCD
;1) Read the SPI status register (R1 has address) and check bit 4, 
;2) If bit 4 is high, loop back to step 1 (wait for BUSY bit to be low)
;3) Clear D/C (GPIO bit 6) to zero, be friendly (R3 has address)
;4) Write the command to the SPI data register (R2 has address)
;5) Read the SPI status register (R1 has address) and check bit 4, 
;6) If bit 4 is high, loop back to step 5 (wait for BUSY bit to be low)
    push {r4}                   ; save r4 on stack
_SPIOutCommand_loop1
    ldr r4, [r1]                ; read SPI status register
    tst r4, #0x10               ; check bit 4 (BUSY)
    bne _SPIOutCommand_loop1    ; if BUSY is low, continue
    ldr r4, [r3]                ; read GPIO port
    bic r4, #0x40               ; clear D/C (GPIO bit 6)
    str r4, [r3]                ; write to GPIO port to clear D/C
    str r0, [r2]                ; write command to SPI data register
_SPIOutCommand_loop2
    ldr r4, [r1]                ; read SPI status register again
    tst r4, #0x10               ; check bit 4 (BUSY)
    bne _SPIOutCommand_loop2    ; if BUSY is low, continue 
   
    pop {r4}                    ; restore r4 from stack
    BX  LR                      ; return



; ***********SPIOutData*****************
; This is a helper function that sends an 8-bit data to the LCD.
; Inputs: R0 = 32-bit data (number)
;         R1 = 32-bit SPI status register address
;         R2 = 32-bit SPI data register address
;         R3 = 32-bit GPIO port address for D/C
; Outputs: none
; Assumes: SPI and GPIO have already been initialized and enabled
; Note: must be AAPCS compliant
; Note: access to bit 6 of GPIO must be friendly
SPIOutData
; --UUU-- Code to write data to the LCD
;1) Read the SPI status register (R1 has address) and check bit 1, 
;2) If bit 1 is low, loop back to step 1 (wait for TNF bit to be high)
;3) Set D/C (GPIO bit 6) to one, be friendly (R3 has address)
;4) Write the data to the SPI data register (R2 has address)
    push {r4}               ; save r4 on stack
_SPIOutData_loop
    ldr r4, [r1]            ; read SPI status register
    tst r4, #0x02           ; check bit 1 (TNF)
    beq _SPIOutData_loop    ; if TNF is high, continue
    ldr r4, [r3]            ; read GPIO port
    orr r4, #0x40           ; set D/C (GPIO bit 6)
    str r4, [r3]            ; write to GPIO port to set D/C
    str r0, [r2]            ; write data to SPI data register

    pop {r4}                ; restore r4 from stack
    BX  LR                  ; return
;****************************************************

    ALIGN                           ; make sure the end of this section is aligned
    END                             ; end of file
