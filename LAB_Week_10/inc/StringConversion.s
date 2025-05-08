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
    mov r2, #0
    push {r2}
    mov r2, #10
_Dec2String_loop
    udiv r3, r0, r2
    mul r4, r3, r2
    rsb r4, r0
    add r4, #0x30
    push {r4}
    mov r0, r3            ; update R0 with quotient
    cmp r0, #0
    bne _Dec2String_loop  ; if not zero, continue loop
    b print_dec

print_dec
    pop {r0}
    str r0, [r1]         ; store character in array
    add r1, #1          ; increment pointer to next character
    cmp r0, #0
    bne print_dec
_print_dec_done
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
    mov r2, #' '
    push {r2}
    mov r3, #0
    mov r4, #'.'
    mov r5, #'*'
	ldr r2, =9999
    cmp r0, r2
    movhi r6, #1
    mov r2, #10
_Fix2String_loop
    cmp r3, #3
    pusheq {r4}
    addeq r3, #1
    beq _Fix2String_loop

    cmp r3, #5
    beq print_fix

    cmp r6, #1
    pusheq {r5}
	addeq r3, #1
    beq _Fix2String_loop

    udiv r7, r0, r2
    mul r8, r7, r2
    rsb r8, r0
    add r8, #0x30
    push {r8}
    mov r0, r7            ; update R0 with quotient
    add r3, #1
    b _Fix2String_loop
print_fix
    pop {r0}
    cmp r0, #' '
    cmpeq r6, #1
    beq _print_fix_done

    str r0, [r1]         ; store character in array
    add r1, #1          ; increment pointer to next character
    cmp r0, #' '
    bne print_fix
_print_fix_done
    pop {r4-r11, lr}          ; restore registers from stack
    BX LR

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
