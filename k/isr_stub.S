.macro isr_err_stub num
.global isr_\num
isr_\num:
    pushl    $\num
    jmp    isr_common
.endm

.macro isr_no_err_stub num
.global isr_\num
isr_\num:
    pushl    $0
    pushl    $\num
    jmp    isr_common
.endm

isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31
isr_no_err_stub 32
isr_no_err_stub 33
isr_no_err_stub 34
isr_no_err_stub 35
isr_no_err_stub 36
isr_no_err_stub 37
isr_no_err_stub 38
isr_no_err_stub 39
isr_no_err_stub 40
isr_no_err_stub 41
isr_no_err_stub 42
isr_no_err_stub 43
isr_no_err_stub 44
isr_no_err_stub 45
isr_no_err_stub 46
isr_no_err_stub 47

isr_common:
    pusha
    pushl %esp
    call generic_c_handler
    add $4, %esp
    popa
    add $8, %esp
    iret

.global isr_stub_table
isr_stub_table:
    .long isr_0,  isr_1,  isr_2,  isr_3
    .long isr_4,  isr_5,  isr_6,  isr_7
    .long isr_8,  isr_9,  isr_10, isr_11
    .long isr_12, isr_13, isr_14, isr_15
    .long isr_16, isr_17, isr_18, isr_19
    .long isr_20, isr_21, isr_22, isr_23
    .long isr_24, isr_25, isr_26, isr_27
    .long isr_28, isr_29, isr_30, isr_31
    .long isr_32, isr_33, isr_34, isr_35
    .long isr_36, isr_37, isr_38, isr_39
    .long isr_40, isr_41, isr_42, isr_43
    .long isr_44, isr_45, isr_46, isr_47
    .rept 80
    .long isr_generic
    .endr
    .long isr_128

.global isr_generic
isr_generic:
    pushl $0
    pushl $0xFF
    jmp isr_common

# Syscall handler
isr_no_err_stub 128

