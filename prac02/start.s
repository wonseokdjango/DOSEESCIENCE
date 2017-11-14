
.globl _start
_start: 
  ldr pc,reset_handler
  ldr pc,undefined_handler
  ldr pc,swi_handler
  ldr pc,prefetch_handler
  ldr pc,data_handler
  ldr pc,unused_handler
  ldr pc,irq_handler
  ldr pc,fiq_handler

reset_handler     : .word reset
undefined_handler : .word hang
swi_handler       : .word hang
prefetch_handler  : .word hang
data_handler      : .word hang
unused_handler    : .word hang
irq_handler       : .word irq
fiq_handler       : .word hang

reset:
  mov r0,#0x8000
  mov r1,#0x0000
  
  ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
  stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}
  ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
  stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}

  mov r0,#0xD2
  msr cpsr_c,r0
  mov sp,#(63 * 1024 * 1024)

  mov r0,#0xD3
  msr cpsr_c,r0

  mov sp,#(64 * 1024 * 1024)
  bl my_main

hang:
  b hang

.globl enable_irq
enable_irq:
  mrs r0, cpsr
  bic r0, r0, #0x80
  msr cpsr_c, r0
  bx lr

irq:
  push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  bl   c_irq_handler
  pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  subs pc,lr,#4

