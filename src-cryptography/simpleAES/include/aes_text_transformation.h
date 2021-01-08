/******************************************************************************************
 * File Name: aes_text_transformation.h
 * 
 * Honbey, honbey@honbey.com
 * Created On 2019-11-11
 * Copyright (c) 2019. All rights reserved.
 * 
 * Function List: 
 * uint subWord(uint)
 * uchar subByte(uchar, bool)
 * void shiftRow(uchar*)
 * void shiftRowI(uchar*)
 * void mixColumn(uchar*)
 * void mixColumnI(uchar*)
 * void addRoundKey(uchar*, uchar*)
 * void matChange(uchar*) 
 *
 * Description: functions of text transformation / matrix operation
 * 
 ******************************************************************************************/

#pragma once

void swap(uchar *_x, uchar *_y) {
	uchar temp = *_x;
	*_x = *_y;
	*_y = temp;
}

uint simpleAES::subWord(uint _w) { // handle unsigned int
	uchar temp[4] = { 0 };
	memcpy(temp, &_w, sizeof(char)* 4); // convert uint to uchar array
	for (int i = 0; i < 4; i++) temp[i] = subByte(temp[i], MODEL_ENCRYPTION);
	memcpy(&_w, temp, sizeof(char)* 4); // inverse
	return _w;
}

uchar simpleAES::subByte(uchar _c, bool _model) { // handle unsigned char
	uchar row = (_c >> 4) & 0x0f, col = _c & 0x0f;
	if (_model) return _matS[row * 16 + col];
	else return _matIS[row * 16 + col];
}

void simpleAES::shiftRow(uchar *_a) {
	for (int i = 1; i < 3; i++)
		for (int j = 0; j < 4 - i; j++)
			swap(&_a[i * 4 + j], &_a[i * 5 + j]);
	for (int j = 3; j > 0; j--) swap(&_a[12 + j], &_a[12 + j - 1]);
}

void simpleAES::shiftRowI(uchar *_a) { // inverse
	for (int i = 1; i < 3; i++)
		for (int j = 3; j >= i; j--)
			swap(&_a[i * 4 + j], &_a[i * 3 + j]);
	for (int j = 0; j < 3; j++) swap(&_a[12 + j], &_a[12 + j + 1]);
}

void simpleAES::mixColumn(uchar *_a) {
	char temp[16] = { 0 }; // the value of array a will be changed
	memcpy(temp, _a, sizeof(char)* 16);

	for (int i = 0; i < 4; i++)
		_a[i] = GFMulti(0x02, temp[i])
		^ GFMulti(0x03, temp[4 + i])
		^ temp[8 + i] ^ temp[12 + i];

	for (int i = 0; i < 4; i++)
		_a[4 + i] = temp[i]
		^ GFMulti(0x02, temp[4 + i])
		^ GFMulti(0x03, temp[8 + i]) ^ temp[12 + i];

	for (int i = 0; i < 4; i++)
		_a[8 + i] = temp[i] ^ temp[4 + i]
		^ GFMulti(0x02, temp[8 + i])
		^ GFMulti(0x03, temp[12 + i]);

	for (int i = 0; i < 4; i++)
		_a[12 + i] = GFMulti(0x03, temp[i])
		^ temp[4 + i] ^ temp[8 + i]
		^ GFMulti(0x02, temp[12 + i]);
}

void simpleAES::mixColumnI(uchar *_a) { // inverse
	char temp[16] = { 0 }; // the value of array a will be changed
	memcpy(temp, _a, sizeof(char)* 16);

	for (int i = 0; i < 4; i++)
		_a[i] = GFMulti(0x0e, temp[i])
		^ GFMulti(0x0b, temp[4 + i])
		^ GFMulti(0x0d, temp[8 + i])
		^ GFMulti(0x09, temp[12 + i]);

	for (int i = 0; i < 4; i++)
		_a[4 + i] = GFMulti(0x09, temp[i])
		^ GFMulti(0x0e, temp[4 + i])
		^ GFMulti(0x0b, temp[8 + i])
		^ GFMulti(0x0d, temp[12 + i]);

	for (int i = 0; i < 4; i++)
		_a[8 + i] = GFMulti(0x0d, temp[i])
		^ GFMulti(0x09, temp[4 + i])
		^ GFMulti(0x0e, temp[8 + i])
		^ GFMulti(0x0b, temp[12 + i]);

	for (int i = 0; i < 4; i++)
		_a[12 + i] = GFMulti(0x0b, temp[i])
		^ GFMulti(0x0d, temp[4 + i])
		^ GFMulti(0x09, temp[8 + i])
		^ GFMulti(0x0e, temp[12 + i]);
}

void simpleAES::addRoundKey(uchar *_a, uchar *_key) {
	for (int i = 0; i < 16; i++) _a[i] ^= _key[i];
}

void simpleAES::matChange(uchar *_a) { // keep the same format of textbook
	char temp[16] = { 0 };
	memcpy(temp, _a, sizeof(char)* 16);
	for (int i = 0, k = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) _a[k++] = temp[j * 4 + i];
}