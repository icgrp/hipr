.section .text

/* Main program
 **********************************/

start:
/* zero-initialize all registers */

addi x1, zero, 0
addi x2, zero, 0
addi x3, zero, 0
addi x4, zero, 0
addi x5, zero, 0
addi x6, zero, 0
addi x7, zero, 0
addi x8, zero, 0
addi x9, zero, 0
addi x10, zero, 0
addi x11, zero, 0
addi x12, zero, 0
addi x13, zero, 0
addi x14, zero, 0
addi x15, zero, 0
addi x16, zero, 0
addi x17, zero, 0
addi x18, zero, 0
addi x19, zero, 0
addi x20, zero, 0
addi x21, zero, 0
addi x22, zero, 0
addi x23, zero, 0
addi x24, zero, 0
addi x25, zero, 0
addi x26, zero, 0
addi x27, zero, 0
addi x28, zero, 0
addi x29, zero, 0
addi x30, zero, 0
addi x31, zero, 0



/* set stack pointer */
lui sp,(128*1024)>>12

/* set gp and tp */
lui gp, %hi(0xdeadbeef)
addi gp, gp, %lo(0xdeadbeef)
addi tp, gp, 0

jal ra,main

/* print "DONE\n" */
lui a0,0x10000000>>12
addi a1,zero,'D'
addi a2,zero,'O'
addi a3,zero,'N'
addi a4,zero,'E'
addi a5,zero,'\n'
sw a1,0(a0)
sw a2,0(a0)
sw a3,0(a0)
sw a4,0(a0)
sw a5,0(a0)

li a0, 0x20000000
li a1, 123456789
sw a1,0(a0)

/* trap */
ebreak
