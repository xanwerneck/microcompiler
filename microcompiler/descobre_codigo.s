.global   ex1, ex2, ex3

ex1:	push    %ebp
        movl    %esp, %ebp

	    movl    $1, %eax
	
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
