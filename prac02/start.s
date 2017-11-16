
.globl _start
_start:
  b skip
  b hang
  b hang
  b hang
  b hang
  b hang
  b hang
  b hang
  b hang
  b hang
  b hang
  b hang
  b hang
  b hang
  b hang
  b hang

.balign 0x80
  b hang
    
.balign 0x80
  b hang

.balign 0x80    
  b hang
    
.balign 0x80
  b hang

.balign 0x80
  b irq_handler

skip:
  mov x1,#0x80000000
	msr	hcr_el2,x1
  ldr x0,=0x00C00800
	msr	sctlr_el1,x0
	mov	x0,#0x3c5  
	msr	spsr_el2,x0
	adr	x0,enter_el1
	msr	elr_el2, x0
	eret

enter_el1:
  ldr x0,=_start
  msr vbar_el1,x0
  mov sp,#0x08000000
  bl main

hang:
  b hang

.globl enable_irq
enable_irq:
  ldr x0,=_start
  msr vbar_el1,x0
  msr daifclr,#2
  ret

irq_handler:
  stp x0,x1,[sp,#-16]!
  stp x2,x3,[sp,#-16]!
  stp x4,x5,[sp,#-16]!
  stp x6,x7,[sp,#-16]!
  stp x8,x9,[sp,#-16]!
  stp x10,x11,[sp,#-16]!
  stp x12,x13,[sp,#-16]!
  stp x14,x15,[sp,#-16]!
  stp x16,x17,[sp,#-16]!
  stp x18,x19,[sp,#-16]!
  bl c_irq_handler
  ldp x18,x19,[sp],#16
  ldp x16,x17,[sp],#16
  ldp x14,x15,[sp],#16
  ldp x12,x13,[sp],#16
  ldp x10,x11,[sp],#16
  ldp x8,x9,[sp],#16
  ldp x6,x7,[sp],#16
  ldp x4,x5,[sp],#16
  ldp x2,x3,[sp],#16
  ldp x0,x1,[sp],#16
  eret
