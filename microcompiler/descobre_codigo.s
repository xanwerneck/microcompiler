.global   ex1, ex1, ex12, ex2, ex3, foo

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
		
foo:	push    %ebp
        movl    %esp, %ebp

		movl    $2, %edx
	    subl    $3, %edx
		movl    %edx, %eax

        movl    %ebp, %esp
        popl    %ebp
	    ret
/*

*/
		
ex12:	push    %ebp
        movl    %esp, %ebp

		subl    $20, %esp
				
		push    8(%ebp)
		call    ex12		
		addl    $4, %esp
		movl    %eax, -4(%ebp)
		
		
ff3:    movl    %ebp, %esp
        popl    %ebp
        ret
		
ex11:	push    %ebp
        movl    %esp, %ebp
		subl    $20, %esp
		
		movl    12(%ebp), %ecx
		cmpl    $0, %ecx
		je      f12
		subl    $1, %ecx
	    movl    %ecx, -4(%ebp)
		push    -4(%ebp)
		call    ex12
		addl    $4, %esp
		movl    %eax, -8(%ebp)
		imul    %ecx, %eax
		movl    %eax, -4(%ebp)
		
f12:    movl    $1, %eax
		jmp     ff2
		
ff2:    movl    %ebp, %esp
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
