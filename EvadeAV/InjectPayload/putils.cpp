// Set of functions that encode/decodes a payload with IP Address to avoid detection.
// This can be adapted to any other encoding mechanisms.
#include "putils.h"

/**
*
*/
vector<string> bytesToIPAddr(const unsigned char* bytes, size_t length) {

	vector<string> ipList;

	for (size_t i = 0; i < length; i = i + 4) {

		if (i + 3 < length) {
			stringstream ip;

			ip << static_cast<int>(bytes[i]) << "." << static_cast<int>(bytes[i + 1]) << "."
				<< static_cast<int>(bytes[i + 2]) << "." << static_cast<int>(bytes[i + 3]);

			ipList.push_back(ip.str());
		}
	}

	return ipList;
}


/**
*
*/
vector<unsigned char> ipAddrToBytes(const vector<string> ipAddr) {

	vector<unsigned char> byteArr;

	for (int i = 0; i < ipAddr.size(); i++) {

		stringstream ssIp(ipAddr[i]);
		string segment;

		while (getline(ssIp, segment, '.')) {

			int byteValue = stoi(segment);
			byteArr.push_back(static_cast<unsigned char>(byteValue));
		}
	}

	return byteArr;
}
