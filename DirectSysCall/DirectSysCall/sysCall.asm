; Basic Direct Syscall Sample

.code
MyNtCreateFile PROC
	mov r10, rcx
	mov eax, 55h
	syscall
	ret
MyNtCreateFile ENDP

end