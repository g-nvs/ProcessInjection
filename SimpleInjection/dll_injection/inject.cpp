#include <Windows.h>
#include <iostream>


int main(int argc, char* argv[]) {

	int pid = atoi(argv[1]);
	HANDLE pHandle = NULL;
	PVOID rBuffer = NULL;

	char* dllPath = argv[2];
	size_t szDll = strlen(dllPath);

	printf("Getting handle for PID: %i\n", pid);
	pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DWORD(pid));
	printf("Handle is: %p", pHandle);

	system("pause");

	printf("Allocating memory in PID: %i\n", pid);
	rBuffer = VirtualAllocEx(pHandle, NULL, szDll, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	printf("Buffer address: %p\n", rBuffer);

	system("pause");

	printf("Writing DLL into process memory of PID: %i\n", pid);
	WriteProcessMemory(pHandle, rBuffer, dllPath, szDll, NULL);

	system("pause");

	printf("Getting address of LaodLibraryA\n");
	HMODULE hModule = GetModuleHandle(L"kernel32.dll");
	LPVOID lpStartAddress = GetProcAddress(hModule, "LoadLibraryA");

	HANDLE hThread = CreateRemoteThread(pHandle, NULL, 0, (LPTHREAD_START_ROUTINE)lpStartAddress, rBuffer, 0, NULL);
	printf("Remote thread %p created in PID: %i\n", hThread, pid);

	CloseHandle(pHandle);
}