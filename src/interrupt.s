# 
# Protected Mode BIOS Call Functionailty v2.0 - by Napalm
# -------------------------------------------------------
# 
# This is code shows how its POSSIBLE to execute BIOS interrupts
# by switch out to real-mode and then back into protected mode.
# 
# If you wish to use all or part of this code you must agree
# to the license at the following URL.
# 
# License: http://creativecommons.org/licenses/by-sa/2.0/uk/
#         
# Notes: This file is in NASM syntax.
#        Turn off paging before calling these functions.
#        int32() resets all selectors.
#
# C Prototype:
#   void _cdelc int32(unsigned char intnum, regs16_t *regs);
# 
# Example of usage:
#   regs.ax = 0x0013;
#   int32(0x10, &regs);
#   memset((char *)0xA0000, 1, (320*200));
#   memset((char *)0xA0000 + (100*320+80), 14, 80);
#   regs.ax = 0x0000;
#   int32(0x16, &regs);
#   regs.ax = 0x0003;
#   int32(0x10, &regs);
# 
# 
MISMATCH: "[bits 32]"

MISMATCH: "global int32, _int32"

MISMATCH: "struc regs16_t"
MISMATCH: "    .di resw 1"
MISMATCH: "    .si resw 1"
MISMATCH: "    .bp resw 1"
MISMATCH: "    .sp resw 1"
MISMATCH: "    .bx resw 1"
MISMATCH: "    .dx resw 1"
MISMATCH: "    .cx resw 1"
MISMATCH: "    .ax resw 1"
MISMATCH: "    .gs resw 1"
MISMATCH: "    .fs resw 1"
MISMATCH: "    .es resw 1"
MISMATCH: "    .ds resw 1"
MISMATCH: "    .ef resw 1"
MISMATCH: "endstruc"

.equ INT32_BASE, 0x7C00
MISMATCH: "%define REBASE(x)                              (((x) - reloc) + INT32_BASE)"
MISMATCH: "%define GDTENTRY(x)                            ((x) &lt"
.equ    CODE32, GDTENTRY(1)                                 # 0x08
.equ    DATA32, GDTENTRY(2)                                 # 0x10
.equ    CODE16, GDTENTRY(3)                                 # 0x18
.equ    DATA16, GDTENTRY(4)                                 # 0x20
MISMATCH: "%define STACK16                                (INT32_BASE - regs16_t_size)"


.text
MISMATCH: "    int32: use32                               "
    _int32: 
        cli                                    # disable interrupts
        pusha                                  # save register state to 32bit stack
        movl $reloc, %esi                      # set source to code below
        movl $INT32_BASE, %edi                 # set destination to new base address
MISMATCH: "        mov  ecx, (int32_end - reloc)          "
        cld                                    # clear direction flag (so we copy forward)
        rep
        movsb                                  # do the actual copy (relocate code to low 16bit space)
        jmp INT32_BASE                         # jump to new code location
MISMATCH: "    reloc: use32                               "
        movl %esp,REBASE(stack32_ptr)          # save 32bit stack pointer
MISMATCH: "        sidt [REBASE(idt32_ptr)]               "
MISMATCH: "        sgdt [REBASE(gdt32_ptr)]               "
MISMATCH: "        lgdt [REBASE(gdt16_ptr)]               "
        leal 0x24(%esp),%esi                   # set position of intnum on 32bit stack
        lodsl                                  # read intnum into eax
        movb %al,REBASE(ib)                    # set intrrupt immediate byte from our arguments 
        movl (%esi),%esi                       # read regs pointer in esi as source
        movl $STACK16, %edi                    # set destination to 16bit stack
        movl $regs16_t_size, %ecx              # set copy size to our struct size
        movl %edi,%esp                         # save destination to as 16bit stack offset
        rep
        movsb                                  # do the actual copy (32bit stack to 16bit stack)
MISMATCH: "        jmp  word CODE16:REBASE(p_mode16)      "
MISMATCH: "    p_mode16: use16"
        movw $DATA16, %ax                      # get our 16bit data selector
        movw %ax,%ds                           # set ds to 16bit selector
        movw %ax,%es                           # set es to 16bit selector
        movw %ax,%fs                           # set fs to 16bit selector
        movw %ax,%gs                           # set gs to 16bit selector
        movw %ax,%ss                           # set ss to 16bit selector
        movl $cr0, %eax                        # get cr0 so we can modify it
        andb $~0x01, %al                       # mask off PE bit to turn off protected mode
        movl %eax, $cr0                        # set cr0 to result
MISMATCH: "        jmp  word 0x0000:REBASE(r_mode16)      "
MISMATCH: "    r_mode16: use16"
        xorw %ax,%ax                           # set ax to zero
        movw %ax,%ds                           # set ds so we can access idt16
        movw %ax,%ss                           # set ss so they the stack is valid
MISMATCH: "        lidt [REBASE(idt16_ptr)]               "
        movw $0x0870,%bx                       # master 8 and slave 112
        call resetpic                          # set pic's the to real-mode settings
        popa                                   # load general purpose registers from 16bit stack
        popw %gs                               # load gs from 16bit stack
        popw %fs                               # load fs from 16bit stack
        popw %es                               # load es from 16bit stack
        popw %ds                               # load ds from 16bit stack
        sti                                    # enable interrupts
        .byte 0xCD                             # opcode of INT instruction with immediate byte
    ib: .byte 0x00
        cli                                    # disable interrupts
        xorw %sp,%sp                           # zero sp so we can reuse it
        movw %sp,%ss                           # set ss so the stack is valid
        movw $INT32_BASE, %sp                  # set correct stack position so we can copy back
        pushf                                  # save eflags to 16bit stack
        pushw %ds                              # save ds to 16bit stack
        pushw %es                              # save es to 16bit stack
        pushw %fs                              # save fs to 16bit stack
        pushw %gs                              # save gs to 16bit stack
        pusha                                  # save general purpose registers to 16bit stack
        movw $0x2028,%bx                       # master 32 and slave 40
        call resetpic                          # restore the pic's to protected mode settings
        movl $cr0, %eax                        # get cr0 so we can modify it
        incl %eax                              # set PE bit to turn on protected mode
        movl %eax, $cr0                        # set cr0 to result
MISMATCH: "        jmp  dword CODE32:REBASE(p_mode32)     "
MISMATCH: "    p_mode32: use32"
        movw $DATA32, %ax                      # get our 32bit data selector
        movw %ax,%ds                           # reset ds selector
        movw %ax,%es                           # reset es selector
        movw %ax,%fs                           # reset fs selector
        movw %ax,%gs                           # reset gs selector
        movw %ax,%ss                           # reset ss selector
MISMATCH: "        lgdt [REBASE(gdt32_ptr)]               "
MISMATCH: "        lidt [REBASE(idt32_ptr)]               "
        movl REBASE(stack32_ptr),%esp          # restore 32bit stack pointer
        movl $STACK16, %esi                    # set copy source to 16bit stack
        leal 0x28(%esp),%edi                   # set position of regs pointer on 32bit stack
        movl (%edi),%edi                       # use regs pointer in edi as copy destination
        movl $regs16_t_size, %ecx              # set copy size to our struct size
        cld                                    # clear direction flag (so we copy forward)
        rep
        movsb                                  # do the actual copy (16bit stack to 32bit stack)
        popa                                   # restore registers
        sti                                    # enable interrupts
        ret                                    # return to caller

    resetpic:                                   # reset's 8259 master and slave pic vectors
        pushw %ax                              # expects bh = master vector, bl = slave vector
        movb $0x11,%al                         # 0x11 = ICW1_INIT | ICW1_ICW4
        outb %al, $0x20                        # send ICW1 to master pic
        outb %al, $0xA0                        # send ICW1 to slave pic
        movb %bh,%al                           # get master pic vector param
        outb %al, $0x21                        # send ICW2 aka vector to master pic
        movb %bl,%al                           # get slave pic vector param
        outb %al, $0xA1                        # send ICW2 aka vector to slave pic
        movb $0x04,%al                         # 0x04 = set slave to IRQ2
        outb %al, $0x21                        # send ICW3 to master pic
        shrb %al                               # 0x02 = tell slave its on IRQ2 of master
        outb %al, $0xA1                        # send ICW3 to slave pic
        shrb %al                               # 0x01 = ICW4_8086
        outb %al, $0x21                        # send ICW4 to master pic
        outb %al, $0xA1                        # send ICW4 to slave pic
        popw %ax                               # restore ax from stack
        ret                                    # return to caller

    stack32_ptr:                                # address in 32bit stack after we
        .long 0x00000000                       #   save all general purpose registers

    idt32_ptr:                                  # IDT table pointer for 32bit access
        .word 0x0000                           # table limit (size)
        .long 0x00000000                       # table base address

    gdt32_ptr:                                  # GDT table pointer for 32bit access
        .word 0x0000                           # table limit (size)
        .long 0x00000000                       # table base address

    idt16_ptr:                                  # IDT table pointer for 16bit access
        .word 0x03FF                           # table limit (size)
        .long 0x00000000                       # table base address

    gdt16_base:                                 # GDT descriptor table
        gdt16_base.null:                        # 0x00 - null segment descriptor
            .long 0x00000000                   # must be left zero'd
            .long 0x00000000                   # must be left zero'd

        gdt16_base.code32:                      # 0x01 - 32bit code segment descriptor 0xFFFFFFFF
            .word 0xFFFF                       # limit  0:15
            .word 0x0000                       # base   0:15
            .byte 0x00                         # base  16:23
            .byte 0x9A                         # present, iopl/0, code, execute/read
            .byte 0xCF                         # 4Kbyte granularity, 32bit selector; limit 16:19
            .byte 0x00                         # base  24:31

        gdt16_base.data32:                      # 0x02 - 32bit data segment descriptor 0xFFFFFFFF
            .word 0xFFFF                       # limit  0:15
            .word 0x0000                       # base   0:15
            .byte 0x00                         # base  16:23
            .byte 0x92                         # present, iopl/0, data, read/write
            .byte 0xCF                         # 4Kbyte granularity, 32bit selector; limit 16:19
            .byte 0x00                         # base  24:31

        gdt16_base.code16:                      # 0x03 - 16bit code segment descriptor 0x000FFFFF
            .word 0xFFFF                       # limit  0:15
            .word 0x0000                       # base   0:15
            .byte 0x00                         # base  16:23
            .byte 0x9A                         # present, iopl/0, code, execute/read
            .byte 0x0F                         # 1Byte granularity, 16bit selector; limit 16:19
            .byte 0x00                         # base  24:31

        gdt16_base.data16:                      # 0x04 - 16bit data segment descriptor 0x000FFFFF
            .word 0xFFFF                       # limit  0:15
            .word 0x0000                       # base   0:15
            .byte 0x00                         # base  16:23
            .byte 0x92                         # present, iopl/0, data, read/write
            .byte 0x0F                         # 1Byte granularity, 16bit selector; limit 16:19
            .byte 0x00                         # base  24:31

    gdt16_ptr:                                  # GDT table pointer for 16bit access
MISMATCH: "        dw gdt16_ptr - gdt16_base - 1          "
MISMATCH: "        dd gdt16_base                          "

    int32_end:                                  # end marker (so we can copy the code)
