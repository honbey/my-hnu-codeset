/******************************************************************************************
 * File Name: aes_algorithm.h
 * 
 * Honbey, honbey@honbey.com
 * Created On 2019-11-11
 * Copyright (c) 2019. All rights reserved.
 * 
 * Function List: 
 * void createSBox()
 * void createISBox()
 * string encryption(string)
 * string decryption(string)
 * uint* keyExpansion(uchar*)
 * 
 * Description: basic function and algorithm of AES 
 * 
 ******************************************************************************************/

#pragma once

void simpleAES::createSBox() {
	//printf("S Box BEGIN:");
	uchar b[8] = { 0xf8, 0x7c, 0x3e, 0x1f,
		0x8f, 0xc7, 0xe3, 0xf1 }; 
	uchar c = 0x63; // initial
	for (int i = 0; i < 256; i++) {
		uchar modI = GFModInv(i);
		for (int j = 7; j >= 0; j--) {
			uchar tempC = b[7 - j] & modI;
			int cnt = 0;
			for (int k = 0; k < 8; k++){
				cnt += tempC & 1;
				tempC >>= 1;
			}
			if (cnt & 1) setOne(_matS[i], j); // binary matrix operation
			else setZero(_matS[i], j);
		}
		_matS[i] ^= c;
		//if (i % 16 == 0) printf("\n");
		//printf("%02X ", _matS[i]); // print S Box
	}
	//printf("\nS Box END!\n\n");
}
void simpleAES::createISBox() {
	//printf("IS Box BEGIN:");
	uchar b[8] = { 0x52, 0x29, 0x94, 0x4a,
		0x25, 0x92, 0x49, 0xa4 };
	uchar c = 0x05; // initial
	for (int i = 0; i < 256; i++) {
		for (int j = 7; j >= 0; j--) {
			uchar tempC = b[7 - j] & i;
			int cnt = 0;
			for (int k = 0; k < 8; k++){
				cnt += tempC & 1;
				tempC >>= 1;
			}
			if (cnt & 1) setOne(_matIS[i], j); // binary matrix operation
			else setZero(_matIS[i], j);
		}
		_matIS[i] ^= c;
		_matIS[i] = GFModInv(_matIS[i]);
		//if (i % 16 == 0) printf("\n");
		//printf("%02X ", _matIS[i]); // print IS Box
	}
	//printf("\nIS Box End!\n\n");
}

std::string simpleAES::encryption(std::string _Text) {
	std::string str = "";
	uint rank = _Text.length() / 16; // iterations
	if (_Text.length() % 16) rank += 1;

	uint k = 0; // pointer of '_Text', the type of length() is unsigned size_t
	for (uint ii = 0; ii < rank; ii++) {
		uchar textMat[16] = { 0 }, keyMat[16] = { 0 };
		for (int j = 0; j < 16 && k < _Text.length(); j++) textMat[j] = _Text[k++];

		matChange(textMat);
		memcpy(keyMat, _eKey, sizeof(char)* 16);
		addRoundKey(textMat, keyMat);
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 16; j++) textMat[j] = subByte(textMat[j], MODEL_ENCRYPTION);
			shiftRow(textMat);
			mixColumn(textMat);
			memcpy(keyMat, _eKey + (4 * i + 4), sizeof(char)* 16);
			addRoundKey(textMat, keyMat);
		}
		for (int j = 0; j < 16; j++) textMat[j] = subByte(textMat[j], MODEL_ENCRYPTION);
		shiftRow(textMat);
		memcpy(keyMat, _eKey + 40, sizeof(char)* 16);
		addRoundKey(textMat, keyMat);
		for (int j = 0; j < 16; j++) str += textMat[j];
	}
	return str;
}

std::string simpleAES::decryption(std::string _Text) {
	std::string str = "";
	uint rank = _Text.length() / 16; // iterations

	uint k = 0; // pointer of '_Text', the type of length() is unsigned size_t
	for (uint ii = 0; ii < rank; ii++) {
		uchar textMat[16] = { 0 }, keyMat[16] = { 0 };
		for (int j = 0; j < 16 && k < _Text.length(); j++) textMat[j] = _Text[k++];

		memcpy(keyMat, _eKey + 40, sizeof(uchar)* 16);
		addRoundKey(textMat, keyMat);
		for (int i = 9; i > 0; i--) {
			for (int j = 0; j < 16; j++) textMat[j] = subByte(textMat[j], MODEL_DECRYPTION);
			shiftRowI(textMat);
			mixColumnI(textMat);
			memcpy(keyMat, _eKey + 4 * i, sizeof(uchar)* 16);
			mixColumnI(keyMat);
			addRoundKey(textMat, keyMat);
		}
		for (int j = 0; j < 16; j++) textMat[j] = subByte(textMat[j], MODEL_DECRYPTION);
		shiftRowI(textMat);
		memcpy(keyMat, _eKey, sizeof(char)* 16);
		addRoundKey(textMat, keyMat);
		matChange(textMat);
		for (int j = 0; j < 16; j++) {
			if (ii == rank - 1 && textMat[j] == 0) // keep the stability of file
				break;
			str += textMat[j];
		}
	}
	return str;
}

uint* simpleAES::keyExpansion(uchar *_key) {
	matChange(_key);
	uint *w = new uint[44], temp;

	for (int i = 0; i < 4; i++) { // to little endian / keep the same storage order as uchar[]
		memcpy(&w[i], _key + i * 4, sizeof(char)* 4);
		w[i] = ((w[i] >> 24) & 0xff)
			| ((w[i] >> 8) & 0xff00)
			| ((w[i] << 8) & 0xff0000)
			| ((w[i] << 24));
	}

	uint rcon[10] = { 0x01000000u, 0x02000000u,
		0x04000000u, 0x08000000u, 0x10000000u, 0x20000000u,
		0x40000000u, 0x80000000u, 0x1b000000u, 0x36000000u };

	for (int i = 4; i < 44; i++) {
		temp = w[i - 1];
		if (i % 4 == 0) temp = subWord((temp >> 24) | (temp << 8)) ^ rcon[i / 4 - 1];
		w[i] = w[i - 4] ^ temp;
	}
	return w;
}