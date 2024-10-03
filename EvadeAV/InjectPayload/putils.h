#pragma once
#ifndef PUTILS_H
#define PUTILS_H

#include <vector>
#include <sstream>

using namespace std;

vector<string> bytesToIPAddr(const unsigned char* bytes, size_t length);
vector<unsigned char> ipAddrToBytes(const vector<string> ipAddr);

#endif