	.file	"compareandswap.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"ptr's value is %d\n"
.LC1:
	.string	"ptr's value is now %d\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB22:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	$1, %edx
	movl	$.LC0, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	leaq	8(%rsp), %rbx
	movl	$1, 8(%rsp)
	movl	$2, 12(%rsp)
	movq	%rbx, (%rsp)
	call	__printf_chk
	leaq	12(%rsp), %rdx
	movq	%rbx, %rax
	lock cmpxchgq	%rdx, (%rsp)
	movq	(%rsp), %rax
	movl	$.LC1, %esi
	movl	$1, %edi
	movl	(%rax), %edx
	xorl	%eax, %eax
	call	__printf_chk
	addq	$16, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE22:
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
