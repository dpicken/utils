.text

/**
 * Context initContext(void* stack, std::size_t stackSize, StartContextFn startContextFn, void* arg);
 *
 * rdi: stack
 * rsi: stackSize
 * rdx: startContextFn
 * rcx: arg
 *
 * rax: top of stack
 */
.globl _initContext
.align 4
_initContext:
  // Set top of stack to stack + stackSize
  movq    %rdi, %rax
  addq    %rsi, %rax
  // Align top of stack on a 16 byte boundary
  andq    $-16, %rax

  // Stack (offsets from %rax):
  //
  // 0x40: rip (endContext)
  // 0x38: rip (startContextFn)
  // 0x30: arg
  // 0x28: r15
  // 0x20: r14
  // 0x18: r13
  // 0x10: r12
  // 0x08: rbp
  // 0x00: rbx

  // Reserve space on stack for context
  subq    $0x48, %rax

  // Store endContext
  leaq    endContext(%rip), %r8
  movq    %r8, 0x40(%rax)
  // Store startContextFn
  movq    %rdx, 0x38(%rax)
  // Store arg
  movq    %rcx, 0x30(%rax)

  ret

endContext:
  // Call exit(0)
  movq    $0, %rdi
  call    _exit
  hlt

/**
 * void switchContext(Context* fromContext, Context toContext);
 *
 * rdi: fromContext
 * rsi: toContext
 */
.globl _switchContext
.align 4
_switchContext:
  // Stack (offsets from %rsp):
  //
  // 0x30: arg
  // 0x28: r15
  // 0x20: r14
  // 0x18: r13
  // 0x10: r12
  // 0x08: rbp
  // 0x00: rbx

  // Reserve stack space for fromContext
  subq    $0x38, %rsp

  // Store fromContext
  // N.B. arg only stored during initContext
  movq    %r15, 0x28(%rsp)
  movq    %r14, 0x20(%rsp)
  movq    %r13, 0x18(%rsp)
  movq    %r12, 0x10(%rsp)
  movq    %rbp, 0x08(%rsp)
  movq    %rbx, 0x00(%rsp)
  movq    %rsp, (%rdi)

  // Restore toContext
  movq    %rsi, %rsp
  movq    0x00(%rsp), %rbx
  movq    0x08(%rsp), %rbp
  movq    0x10(%rsp), %r12
  movq    0x18(%rsp), %r13
  movq    0x20(%rsp), %r14
  movq    0x28(%rsp), %r15
  movq    0x30(%rsp), %rdi

  // Release stack space used for toContext
  addq    $0x38, %rsp

  ret
