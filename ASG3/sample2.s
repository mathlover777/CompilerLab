	.file	"sample2.c"
	.section	.rodata
.LC0:
	.string	"%d" 				# string used for entering input scanf("%d",&a);
.LC1:
	.string	"%d\n"				# string used for output printf("%d\n",fact);
	.text
	.globl	main 				# tells the assembler that main is a global lebel
	.type	main, @function
main: 							# the 'main' label
.LFB0:
	.cfi_startproc
	pushq	%rbp          		# store the old value of the base pointer at the stack
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp          # move the old value of stack pointer to the base pointer
	.cfi_def_cfa_register 6
	subq	$16, %rsp 			# reserve space in stack

	################################ fact = 1 ################################################
	movl	$1, -4(%rbp)        # store 1 in rbp-4
								# ie 		rbp[4] = 1
								# used as fact
	##########################################################################################

	########################## taking the input ##############################################
	movl	$.LC0, %eax         # moving the adress of the string(to take input a) in eax 
	leaq	-8(%rbp), %rdx 		# load the effective address of rbp[8] in rdx
	movq	%rdx, %rsi          # move rdx(data register) to rsi(source index for string operation)
	movq	%rax, %rdi          # move rax to rdi(destination index for string operation)
	movl	$0, %eax            # set eax = 0
	call	__isoc99_scanf      # take the input
								# the input is stored at rbp[8]
	##########################################################################################

	# fact ie rdp[4] will be used as factorial(fact) upto step i...steps
	#        ie rdp [4] = a * (a-1) * .... * (a-(i-1))
	# 			rdp [8] will be like a
	# at the END of i th step ...... rdp[8] = a-i
	# i will be from 1 to n-1
	# at start (i = 1) fact ie rdp[4] will be initially 1
	# at start (i = 1) rdp[8] = a


	############################ the while loop ##############################################

	jmp	.L2 					# jump unconditionally to .L2  // just the start of the while loop
.L3:
	####################### fact = fact * a ######################################################
	movl	-8(%rbp), %eax      # load rbp[8] to eax
								# ie 		eax = rbp[8]
	
	movl	-4(%rbp), %edx      # move rbp[4] to edx
								# ie 		edx = rbp[4] (edx = rbp[4] = 1 initially )
	
	imull	%edx, %eax 			# get eax = edx * eax
								# ie 		eax = rdp[4] * rdp [8]
								#

	movl	%eax, -4(%rbp)      # move eax to rdp [4]
								# ie 		rdp[4] = rdp[4] * rdp[8]
								# as rdp[4] is used as a and rdp[8] is used as fact
								# we have done fact = fact * a
	#############################################################################################

	
	############################ a = a -1 #######################################################
	movl	-8(%rbp), %eax      # move rdp[8] in eax
								# ie 		eax = rdp[8]
	
	subl	$1, %eax            # 			eax = eax - 1
								# 			eax = rdp[8] - 1

	movl	%eax, -8(%rbp)  	# move eax to rdp[8]
								#ie 		rdp[8] = rdp[8] - 1
								#  as rdp[8] is used as a ... we have done a = a - 1
	###############################################################################################

.L2:
	##################### if (a>0) then loop #################################################
	movl	-8(%rbp), %eax    	# load rbp[8] in eax
								# eax = rbp[8]
								# we have loaded 'a' in eax
	testl	%eax, %eax          # check if eax == 0 .... the loop termination check
								# if eax AND eax ==0 ie eax ==0 then set the zero flag
								# ie if a==0 then then the zero flag
	jg	.L3   					# if the zero flag is not (set ie a aka eax aka rdp[8]>0)
								# then jump to .L3 ie loop
								# otherwise get out from the while loop
	##########################################################################################

	############################## printng the output #######################################
	movl	$.LC1, %eax 		# load the address of the string for output in eax
	movl	-4(%rbp), %edx 		# load rbp[4](fact = a!) in edx
	movl	%edx, %esi 			# move content of edx(a!) to esi()
	movq	%rax, %rdi 			# move the content of rax to rdi
	movl	$0, %eax 			# set eax = 0
	call	printf 				# calling the C Library function printf
	##########################################################################################

	########################### return 0 #####################################################
	movl	$0, %eax 			# store 0 as return in eax
	##########################################################################################

	################################### exit #######################################
	leave					# set 	rsp = rbp
							# set 	pop and give the value(that old value of base pointer) to rbp
							# so the situation is equivalent to the situtation just before executing the code
	#################################################################################

	.cfi_def_cfa 7, 8
	ret 					# returns control to the calling procedure
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
