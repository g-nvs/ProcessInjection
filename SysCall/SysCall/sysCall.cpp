#include <windows.h>
#include <iostream>

//winternl.h
#ifndef InitializeObjectAttributes
#define InitializeObjectAttributes( p, n, a, r, s ) { \
    (p)->Length = sizeof( OBJECT_ATTRIBUTES );          \
    (p)->RootDirectory = r;                             \
    (p)->Attributes = a;                                \
    (p)->ObjectName = n;                                \
    (p)->SecurityDescriptor = s;                        \
    (p)->SecurityQualityOfService = NULL;               \
    }
#endif


//winternl.h
typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING* PUNICODE_STRING;
typedef const UNICODE_STRING* PCUNICODE_STRING;


//winternl.h
typedef struct _OBJECT_ATTRIBUTES {
	ULONG Length;
	HANDLE RootDirectory;
	PUNICODE_STRING ObjectName;
	ULONG Attributes;
	PVOID SecurityDescriptor;
	PVOID SecurityQualityOfService;
} OBJECT_ATTRIBUTES;
typedef OBJECT_ATTRIBUTES* POBJECT_ATTRIBUTES;


//winternl.h
typedef struct _IO_STATUS_BLOCK {
#pragma warning(push)
#pragma warning(disable: 4201) // we'll always use the Microsoft compiler
	union {
		NTSTATUS Status;
		PVOID Pointer;
	} DUMMYUNIONNAME;
#pragma warning(pop)

	ULONG_PTR Information;
} IO_STATUS_BLOCK, * PIO_STATUS_BLOCK;


typedef NTSTATUS(NTAPI* pZwCreateFile) (
	PHANDLE FileHandle,
	ACCESS_MASK DesiredAccess,
	POBJECT_ATTRIBUTES ObjectAttributes,
	PIO_STATUS_BLOCK IoStatusBlock,
	PLARGE_INTEGER AllocationSize,
	ULONG FileAttributes,
	ULONG ShareAccess,
	ULONG CreateDisposition,
	ULONG CreateOptions,
	PVOID EaBuffer,
	ULONG EaLength
);


//winternl.h
typedef NTSTATUS(NTAPI* pRtlInitUnicodeString)(
	PUNICODE_STRING DestinationString,
	PCWSTR SourceString
	);


#define OBJ_CASE_INSENSITIVE			0x00000040L
#define FILE_OVERWRITE_IF				0x00000005
#define FILE_SYNCHRONOUS_IO_NONALERT	0x00000020


int main() {

	//https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew
	HANDLE hFile = CreateFile(L"file.log", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	CloseHandle(hFile);
	
	printf("[+] Kernel32 Hook check pause\n");
	system("pause");

	IO_STATUS_BLOCK ioStatusBlock = {};
	OBJECT_ATTRIBUTES objAttr = {};
	NTSTATUS status;
	UNICODE_STRING fileName;

	HMODULE ntDll = LoadLibraryW(L"ntdll.dll");

	pZwCreateFile NtCreateFile = (pZwCreateFile)GetProcAddress(ntDll, "NtCreateFile");
	pRtlInitUnicodeString RtlInitUnicodeString = (pRtlInitUnicodeString)GetProcAddress(ntDll, "RtlInitUnicodeString");

	RtlInitUnicodeString(&fileName, L"\\??\\C:\\ProgramData\\TEMP\\syscall.log");

	InitializeObjectAttributes(&objAttr, &fileName, OBJ_CASE_INSENSITIVE, NULL, NULL);

	status = NtCreateFile(&hFile,
		GENERIC_READ | SYNCHRONIZE, 
		&objAttr, &ioStatusBlock, NULL, FILE_ATTRIBUTE_NORMAL, 
		FILE_SHARE_READ | FILE_SHARE_WRITE, 
		FILE_OVERWRITE_IF, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);
	
	if (status == 0)
		printf("[+] File was created successfully\n");
	else
		printf("[!] Failed to create file\n");

	CloseHandle(hFile);
	FreeLibrary(ntDll);

	system("pause");

	return 0;
}
