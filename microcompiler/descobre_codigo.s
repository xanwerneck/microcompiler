.global   ex1, ex1, ex12, ex2, ex3

ex1:	push    %ebp
        movl    %esp, %ebp

	    movl    %eax, %eax
		movl    %edx, %eax
		movl    %ecx, %eax
		movl    %ebx, %eax
		movl    %esi, %eax
		movl    8(%ebp), %eax
	
        movl    %ebp, %esp
        popl    %ebp
        ret

ex12:	push    %ebp
        movl    %esp, %ebp

		movl    8(%ebp), %edx
		movl    12(%ebp), %ecx
		imul    %edx, %ecx
		movl    %edx, 8(%ebp)

	
        movl    %ebp, %esp
        popl    %ebp
        ret
		
ex11:	push    %ebp
        movl    %esp, %ebp

	    cmpl    $0, 8(%ebp)
		je      f2
f:		movl    $2, %eax

f2:     movl    $0, %edx
	
        movl    %ebp, %esp
        popl    %ebp
        ret

ex2:    push 	%ebp
		movl    %esp, %ebp

		movl    8(%ebp), %edx
		addl    $1, %edx
		movl    %edx, %eax

		movl    %ebp, %esp
		popl    %ebp
		ret

ex3:	push	%ebp
    	mov	%esp, %ebp


        cmpl	$0, 8(%ebp)
		jne 	ff            
		mov     $1, %eax        
		jmp     fim             
ff: 	mov     8(%ebp), %ecx   
		sub     $1, %ecx        
		push    %ecx            
		call    ex3             
		imul    8(%ebp), %eax   
		add     $4, %esp        
	
fim:	
		mov	%ebp, %esp
		pop	%ebp
		ret
