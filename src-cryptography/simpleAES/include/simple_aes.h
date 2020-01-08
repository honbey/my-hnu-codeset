/******************************************************************************************
 * Class Name: simple_aes.h
 * 
 * Honbey, honbey@honbey.com
 * Created On 2019-11-11
 * Copyright (c) 2019. All rights reserved.
 * 
 * Interface List:
 * string encryption(string);
 * string decryption(string);
 * uint* keyExpansion(uchar*);
 * uchar GFDivis(uchar, uchar);
 * uchar GFAddi(uchar, uchar);
 * uchar GFMulti(uchar, uchar);
 * uchar GFModInv(uchar)
 * 
 * 
 * Description: Definition/Declaration of macro constant and main class and function interface.
 * 
 ******************************************************************************************/

#pragma once

#define MODEL_ENCRYPTION 1 // S Box substitute byte
#define MODEL_DECRYPTION 0 // Inverse S Box substitute byte

#define setOne(x,y) x|=(1<<y) // set the bit value 1 of the y-th bit of x
#define setZero(x,y) x&=~(1<<y) // set the bit value 0 of the y-th bit of x

typedef unsigned int uint;
typedef unsigned char uchar;

class simpleAES {
protected:
	uchar *_matS = new uchar[256], *_matIS = new uchar[256],
		*_itopTable = new uchar[256], *_ptoiTable = new uchar[256], *_modInvTable = new uchar[256];
	uint *_eKey = NULL;

	void createSBox();
	void createISBox();

	// Galois Field
	void GFInit();
	uchar GFMultiOver(uchar, uchar);
	uchar GFMultiTrave(uchar, uchar);
	uchar GFMultiTable(uchar, uchar);
	uchar GFModInvOver(uchar);
	uchar GFModInvTrave(uchar);
	uchar GFModInvTable(uchar);
	// Text transformation
	uint subWord(uint);
	uchar subByte(uchar, bool);
	void shiftRow(uchar*);
	void shiftRowI(uchar*);
	void mixColumn(uchar*);
	void mixColumnI(uchar*);
	void addRoundKey(uchar*, uchar*);
	void matChange(uchar*);
	// Initialization parameters of AES
	void crateFor(uchar *_key) {
		GFInit();
		createSBox();
		createISBox();
		printf("Key:\n");
		matChange(_key);
		for (int i = 0; i < 16; i++) {
			if (i % 4 == 0) printf("\n");
			printf("%02X ", _key[i]);
		}
		_eKey = keyExpansion(_key);
		printf("\nKey Expansion:\n");
		for (int i = 0; i < 44; i++) {
			if (i % 4 == 0) printf("%d\n", i / 4);
			printf("%08X\n", _eKey[i]);
		}
		printf("\n");
	}
	
public:
	simpleAES() { 
		uchar DEFAULT_KEY[16] = { // 0f15 71c9 47d9 e859 0cb7 add6 af7f 6798
			0x0f, 0x15, 0x71, 0xc9,
			0x47, 0xd9, 0xe8, 0x59,
			0x0c, 0xb7, 0xad, 0xd6,
			0xaf, 0x7f, 0x67, 0x98 };
		crateFor(DEFAULT_KEY); 
	}
	simpleAES(uchar *key) { crateFor(key); } // custom key
	~simpleAES() {
		delete[] _matS;
		delete[] _matIS;
		delete[] _itopTable;
		delete[] _ptoiTable;
		delete[] _modInvTable;
		delete[] _eKey;
	}
	// external interface
	std::string encryption(std::string _text);
	std::string decryption(std::string _text);
	uint* keyExpansion(uchar *_key);
	uchar GFDivis(uchar _x, uchar _y);
	uchar GFAddi(uchar _x, uchar _y) { return _x ^ _y; }
	uchar GFMulti(uchar _x, uchar _y) { 
		switch (rand() % 3) { // select a function randomly
		case 1: return GFMultiTable(_x, _y); break;
		case 2: return GFMultiTrave(_x, _y); break;
		default: return GFMultiOver(_x, _y); break; 
		}
	}
	uchar GFModInv(uchar _x) { 
		switch (rand() % 3) { // select a function randomly
		case 1: return GFModInvTable(_x); break;
		case 2: return GFModInvTrave(_x); break;
		default: return GFModInvOver(_x); break;
		}
	}
};

#include "aes_implementation.h"
