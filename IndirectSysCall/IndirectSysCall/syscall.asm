; Basic Indirect Syscall Sample

.data

EXTERN NtCreateFileSysCall:QWORD

.code
MyNtCreateFile PROC
	mov r10, rcx
	mov eax, 55h
	jmp qword ptr [NtCreateFileSysCall]
MyNtCreateFile ENDP

end