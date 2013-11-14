.global   foo

foo:	push    %ebp
        movl    %esp, %ebp

	movl    $1, %eax
	
        movl    %ebp, %esp
        popl    %ebp
