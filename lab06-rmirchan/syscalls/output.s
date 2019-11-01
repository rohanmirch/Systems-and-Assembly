# i 86-64 code for the alien message output function
# We set up the arguments for syscall to write() and call it.
# We start off with: buffer size in rsi, buffer message in rdi,
# and the instr_id of write() is 1.


	.file	"output.c"
		.text
		.globl	output
		.type   output, @function

output:
.START:
		movq	  $0x1, %rax      # write() has instruction 1 -> rax
		mov     %rsi, %rdx      # move the size to rdx (arg3)
		mov     %rdi, %rsi      # move the address of the buffer to rsi (arg2)
		movq	  $0x1, %rdi      # fd = 1 in rdi (arg1)		# arguments set up
		syscall
