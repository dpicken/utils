.text

/**
 * Context initContext(void* stack, std::size_t stackSize, StartContextFn startContextFn, void* arg);
 *
 * r0: stack
 * r1: stackSize
 * r2: startContextFn
 * r3: arg
 *
 * r0: top of stack
 */
.globl initContext
.align 2
initContext:
  // Set top of stack to stack + stackSize
  add     r0, r0, r1

  // Align top of stack on an 8 byte boundary
  bic     r0, r0, #0xf

  // Stack (offsets from r0):
  //
  // 0x28:  PC (startContextFn)
  // 0x24:  LR (endContext)
  // 0x20: r11 (v8)
  // 0x1C: r10 (v7)
  // 0x18:  r9 (v6)
  // 0x14:  r8 (v5)
  // 0x10:  r7 (v4)
  // 0x0C:  r6 (v3)
  // 0x08:  r5 (v2)
  // 0x04:  r4 (v1)
  // 0x00: arg

  // Reserve space on stack for context
  sub     r0, r0, #0x2C

  // Store startContextFn
  str     r2, [r0, #0x28]

  // Store endContext
  adr     r1, endContext
  str     r1, [r0, #0x24]

  // Store arg
  str     r3, [r0, #0x00]

  bx      lr

endContext:
  // Call exit(0)
  mov     r0, #0
  bl      _exit@PLT

/**
 * void switchContext(Context* fromContext, Context toContext);
 *
 * r0: fromContext
 * r1: toContext
 */
.globl switchContext
.align 2
switchContext:
  // Stack (offsets from SP):
  //
  // 0x28:  PC
  // 0x24:  LR
  // 0x20: r11 (v8)
  // 0x1C: r10 (v7)
  // 0x18:  r9 (v6)
  // 0x14:  r8 (v5)
  // 0x10:  r7 (v4)
  // 0x0C:  r6 (v3)
  // 0x08:  r5 (v2)
  // 0x04:  r4 (v1)
  // 0x00: arg

  // Store fromContext
  push    {LR}
  push    {r0, r4-r11, LR}
  str     SP, [r0]

  // Restore toContext
  mov     SP, r1
  pop     {r0, r4-r11, LR, PC}
