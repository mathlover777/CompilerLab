	.file	"sample1.c"
	.section	.rodata
.LC0:
	.string	"%d %d\n"       # the string used for printing output  printf("%d %d",a,b);
	.text
	.globl	main 			# tells the assembler that main is a global lebel
	.type	main, @function
main: 						# the 'main' label
.LFB0:
	.cfi_startproc
	pushq	%rbp			# store the old value of the base pointer in the stack
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp		# store the value of stack pointer(currently the pointed address as the value of
							# the old value of the base pointer) in the base pointer...later used from memory clearing
							# after the program has finished
	.cfi_def_cfa_register 6
	subq	$16, %rsp		# reserve space in the stack
	
	# store the variables on the stack #############################################
	movl	$5, -8(%rbp)	# store 5 at rbp-8 ie rbp[8]=5
	movl	$6, -4(%rbp)	# store 6 at rbp-4 ie rbp[4]=5
							# now rbp[0] = old value of rbp
							# 		rbp[4] = 6(b)
							#   	rbp[8] = 5(a)
	################################################################################


	######################### load them to eax and edx #############################
	movl	-8(%rbp), %eax	# copy the value of rbp[8] to eax
	movl	-4(%rbp), %edx	# copy the value of rbp[4] to edx
							# now 	eax = 5 (a)
							#     	edx = 6 (b)
	################################################################################

	############################# a = b - a ########################################
	movl	%edx, %ecx		# now 	ecx = 6
	subl	%eax, %ecx		#     	ecx = ecx - eax
							#  so 	ecx = 6 - 5 = 1
	movl	%ecx, %eax      # move content of ecx to eax
							# 		eax = 1       
	
	movl	%eax, -8(%rbp)	# move the content of eax to rbp[8]
							#		rbp[8] = 1
	################################################################################


	########################### b = b -a ###########################################
	movl	-8(%rbp), %eax  # move the content of rbp[8] to eax
							#       eax = 1
	
	subl	%eax, -4(%rbp)  # do rbp[4] = rbp[4]-eax
						    # 		rbp[4] = 6 - 1 = 5
	################################################################################

	######################### a = a + b ############################################
	movl	-4(%rbp), %eax  # move rbp[4] to eax
							# 		eax = rbp[4]
							#		eax = 5
	
	addl	%eax, -8(%rbp)  # add the content of eax to rbp[8]
							# ie    rbp[8] = rbp[8] + eax
							#		rbp[8] = 1 + 5 = 6
	################################################################################

	############################ values of rbp[4] and rbp[8] are exchanged #########

	############################ printing the output ###############################
	movl	$.LC0, %eax     # copy the address of the string pointed by .LC0 to eax
							# so eax now points to "%d %d\n"

	movl	-4(%rbp), %edx	# move rbp[4] to edx
							# 		edx = 5

	movl	-8(%rbp), %ecx  # move rbp[8] to ecx
							# 		ecx = 6

	movl	%ecx, %esi		# make the final string for output
	movq	%rax, %rdi
	movl	$0, %eax		# 		eax = 0
	call	printf			# printing the output string with C Library function
	################################################################################

	########################## storing the return value ############################
	movl	$0, %eax		# 		eax = 0   // return 0
	################################################################################

	################################### exit #######################################
	leave					# set 	rsp = rbp
							# set 	pop and give the value(that old value of base pointer) to rbp
							# so the situation is equivalent to the situtation just before executing the code
	.cfi_def_cfa 7, 8
	ret 					# returns control to the calling procedure
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
