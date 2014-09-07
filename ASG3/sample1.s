	.file	"sample1.c"
	.section	.rodata
.LC0:
	.string	"%d %d\n"
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
	subq	$16, %rsp
	movl	$5, -8(%rbp)
	movl	$6, -4(%rbp)
	movl	-8(%rbp), %eax
	movl	-4(%rbp), %edx
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	%ecx, %eax
	movl	%eax, -8(%rbp)
	movl	-8(%rbp), %eax
	subl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	addl	%eax, -8(%rbp)
	movl	$.LC0, %eax
	movl	-4(%rbp), %edx
	movl	-8(%rbp), %ecx
	movl	%ecx, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
