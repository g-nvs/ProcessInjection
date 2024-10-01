#include <Windows.h>
#include <iostream>

typedef int (*AddInts)(int, int);
typedef void (*SysPause)();

int main() {

	HINSTANCE hAddlib = LoadLibrary(L"addlib.dll");

	if (hAddlib == NULL) {
		printf("[!] Failed to load DLL\n");
		return -1;
	}

	SysPause exSysPause = (SysPause)GetProcAddress(hAddlib, "PauseMe");

	if (exSysPause == NULL) {
		printf("[!] Failed to load the PauseMe function\n");
		return -1;
	}

	exSysPause();

	AddInts exAddInts = (AddInts)GetProcAddress(hAddlib, "AddInts");

	if (exAddInts == NULL) {
		printf("[!] Failed to load the AddInts function\n");
		return -1;
	}

	int result = exAddInts(12, 58);
	printf("[-] Result is %i\n", result);

	FreeLibrary(hAddlib);
	return 0;
}