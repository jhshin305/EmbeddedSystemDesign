; StringConversion.s
; Student names: change this to your names or look very silly
; Last modification date: change this to the last modification date or look very silly
; Runs on TM4C123
; ECE319K lab 7 number to string conversions
;
; Used in ECE319K Labs 7,8,9,10. You write these two functions

 
    EXPORT   Dec2String
    EXPORT   Fix2String
    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB
    PRESERVE8

  
;-----------------------Dec2String-----------------------
; Convert a 32-bit number into unsigned decimal format
; String the string into the empty array add null-termination
; Input: R0 (call by value) 32-bit unsigned number
;        R1 pointer to empty array
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
Dec2String
    push {r4-r11, lr}         ; save registers on stack
    mov r2, #1
    mov r3, #10
    cmp r0, #0
    beq _Dec2String_loop
_get_max_digit
	mov r5, r2
    umull r2, r4, r2, r3
    cmp r0, r2
    bls _get_max_digit_done
	cmp r4, #0
    beq _get_max_digit
_get_max_digit_done
	mov r2, r5
_Dec2String_loop
    udiv r4, r0, r2
	add r5, r4, #0x30
    str r5, [r1]
	add r1, #1
    mul r4, r2
    sub r0, r4
    udiv r2, r3
    cmp r2, #0
    bne _Dec2String_loop
_Dec2String_done
	mov r4, #0
	str r4, [r1]
    pop {r4-r11, lr}          ; restore registers from stack
    BX LR
;* * * * * * * * End of Dec2String * * * * * * * *


; -----------------------Fix2String----------------------
; Create characters for LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
;          R1 pointer to empty array
; Outputs: none
; E.g., R0=0,    then create "0.000 "
;       R0=3,    then create "0.003"
;       R0=89,   then create "0.089"
;       R0=123,  then create "0.123"
;       R0=9999, then create "9.999"
;       R0>9999, then create "*.***"
; Invariables: This function must not permanently modify registers R4 to R11
Fix2String
    push {r4-r11, lr}         ; save registers on stack
	ldr r2, =9999
    cmp r0, r2
    bgt _too_big
    mov r2, #1000
    mov r4, #10
    mov r3, #0
    b _Fix2String_loop
_too_big
    mov r0, #'*'
    mov r3, #0
_too_big_loop
    cmp r3, #1
    bleq _print_dot
    cmp r3, #5
    beq _Fix2String_done
    str r0, [r1]
    add r1, #1
    add r3, #1
    b _too_big_loop
_print_dot
	push {r2}
    mov r2, #'.'
    str r2, [r1]
    pop {r2}
    add r1, #1
    add r3, #1
    bx lr
_Fix2String_loop
    cmp r3, #1
    bleq _print_dot
    cmp r3, #5
    beq _Fix2String_done
    udiv r4, r0, r2
    add r5, r4, #0x30
    str r5, [r1]
    add r1, #1
    add r3, #1
    mul r4, r2
    sub r0, r4
    udiv r2, r3
    b _Fix2String_loop
_Fix2String_done
    mov r4, #' '
    str r4, [r1]
	add r1, #1
	mov r4, #0
	str r4, [r1]
    pop {r4-r11, lr}          ; restore registers from stack
    BX LR
    
     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
