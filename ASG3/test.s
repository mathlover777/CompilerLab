	.file	"test.c"
	.section	.rodata
.LC0:
	.string	"%d%d%d%d%d%d%d"
.LC1:
	.string	"\n%d %d %d %d %d %d %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movl	$.LC0, %eax
	leaq	-12(%rbp), %r9
	leaq	-16(%rbp), %r8
	leaq	-20(%rbp), %rcx
	leaq	-24(%rbp), %rdx
	leaq	-28(%rbp), %rsi
	leaq	-4(%rbp), %rdi
	movq	%rdi, 8(%rsp)
	leaq	-8(%rbp), %rdi
	movq	%rdi, (%rsp)
	movq	%rax, %rdi
	movl	$0, %eax
	call	__isoc99_scanf
	movl	-4(%rbp), %r8d
	movl	-8(%rbp), %edi
	movl	-12(%rbp), %r9d
	movl	-16(%rbp), %r10d
	movl	-20(%rbp), %ecx
	movl	-24(%rbp), %edx
	movl	-28(%rbp), %esi
	movl	$.LC1, %eax
	movl	%r8d, 8(%rsp)
	movl	%edi, (%rsp)
	movl	%r10d, %r8d
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
