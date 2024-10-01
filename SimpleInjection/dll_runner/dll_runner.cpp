//Simple DLL Runner
#include <Windows.h>


int main() {

	//Load the DLL
	HMODULE hModule = LoadLibraryA("msgbox_injection.dll");

	if (hModule == NULL)
		return 1;

	BOOL freed = FreeLibrary(hModule);

	if (!freed)
		return 1;

	return 0;
}
