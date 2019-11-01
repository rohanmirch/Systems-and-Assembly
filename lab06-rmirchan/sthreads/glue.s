#============================================================================
# Keep a pointer to the main scheduler context.  This variable should be
# initialized to %rsp, which is done in the __sthread_start routine.
#
        .data
        .align 8
scheduler_context:      .quad   0


#============================================================================
# __sthread_switch is the main entry point for the thread scheduler.
# It has three parts:
#
#    1. Save the context of the current thread on the stack.
#       The context includes all of the integer registers and RFLAGS.
#
#    2. Call __sthread_scheduler (the C scheduler function), passing the
#       context as an argument.  The scheduler stack *must* be restored by
#       setting %rsp to the scheduler_context before __sthread_scheduler is
#       called.
#
#    3. __sthread_scheduler will return the context of a new thread.
#       Restore the context, and return to the thread.
#
        .text
        .align 8
        .globl __sthread_switch
__sthread_switch:

        # Save the process state onto its stack
        # Push all registers onto the stack
        pushq  %rax
        pushq  %rbx
        pushq  %rcx
        pushq  %rdx
        pushq  %rsi
        pushq  %rdi
        pushq  %rbp
        pushq  %r8
        pushq  %r9
        pushq  %r10
        pushq  %r11
        pushq  %r12
        pushq  %r13
        pushq  %r14
        pushq  %r15
        pushf          #push rflags onto register

        # Call the high-level scheduler with the current context as an argument
        movq    %rsp, %rdi
        movq    scheduler_context(%rip), %rsp
        call    __sthread_scheduler

        # The scheduler will return a context to start.
        # Restore the context to resume the thread.
__sthread_restore:

        # The scheduler returns the context pointer in rax, so that is
        # our new stack pointer.
        movq  %rax, %rsp

        # Pop all registers off stack (in reverse order of pushing).

        popf                    # pop rflags
        popq  %r15
        popq  %r14
        popq  %r13
        popq  %r12
        popq  %r11
        popq  %r10
        popq  %r9
        popq  %r8
        popq  %rbp
        popq  %rdi
        popq  %rsi
        popq  %rdx
        popq  %rcx
        popq  %rbx
        popq  %rax

        ret


#============================================================================
# Initialize a process context, given:
#    1. the stack for the process (rdi)
#    2. the function to start (rsi)
#    3. its argument  (rdx)
# The context should be consistent with that saved in the __sthread_switch
# routine.
#
# A pointer to the newly initialized context is returned to the caller.
# (This is the thread's stack-pointer after its context has been set up.)
#
# This function is described in more detail in sthread.c.
#
#
        .align 8
        .globl __sthread_initialize_context
__sthread_initialize_context:

        movq  %rsp, %r9     # save our original stack pointer to r9

        movq  %rdi, %rsp    # set rsp to context stack pointer




        leaq __sthread_finish(%rip), %r8  # r8 = address of finish
        pushq  %r8                        # push address of finish onto stack
        pushq  %rsi                       # push f onto stack


        # push all registers onto stack
        pushq  $0x0  # %rax
        pushq  $0x0  # %rbx
        pushq  $0x0  # %rcx
        pushq  $0x0  # %rdx
        pushq  $0x0  # %rsi
        pushq  %rdx  # %rdi (the argument of f is rdx, so it goes in rdi)
        pushq  $0x0  # %rbp
        pushq  $0x0  # %r8
        pushq  $0x0  # %r9
        pushq  $0x0  # %r10
        pushq  $0x0  # %r11
        pushq  $0x0  # %r12
        pushq  $0x0  # %r13
        pushq  $0x0  # %r14
        pushq  $0x0  # %r15
        pushf        # push rflags


        movq  %rsp, %rax    # rax = current stack pointer (so it returns)
        movq  %r9, %rsp     # original stack pointer returned to rsp



        # TODO - Make sure you completely document every part of your
        #        thread context; what it is, and why you set each value
        #        to what you choose.

        ret


#============================================================================
# The start routine initializes the scheduler_context variable, and calls
# the __sthread_scheduler with a NULL context.
#
# The scheduler will return a context to resume.
#
        .align 8
        .globl __sthread_start
__sthread_start:
        # Remember the context
        movq    %rsp, scheduler_context(%rip)

        # Call the scheduler with no context
        movl    $0, %edi  # Also clears upper 4 bytes of %rdi
        call    __sthread_scheduler

        # Restore the context returned by the scheduler
        jmp     __sthread_restore
