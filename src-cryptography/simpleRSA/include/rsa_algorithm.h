/******************************************************************************************
 * File Name: rsa_algorithm.h
 * 
 * Honbey, honbey@honbey.com
 * Created On 2019-10-13
 * Copyright (c) 2019. All rights reserved.
 * 
 * Function List: 
 * std::string RSA(std::string bool)
 * 
 * Description: basic function and algorithm of RSA 
 * 
 ******************************************************************************************/

#pragma once

// encryption or decryption function
std::string simpleRSA::RSA(std::string _Text, bool _model) {
	std::string s = ""; // return value as function
	if (_model) { // encryption
		int rank = _Text.length() / 7; // iterations
		if (_Text.length() % 7) rank += 1;
		unsigned int k = 0; // the type of length() is unsigned size_t, pointer of '_Text'
		for (int i = 0; i < rank; i++) { // encryption process
			elemType toN = 0; // integer value to receive character array
			char plainBlock[8] = {0}; // array used to segment '_Text', set zero to avoid error code
			for (int j = 0; j < 7 && k < _Text.length(); j++) plainBlock[j] = _Text[k++];

			memcpy(&toN, plainBlock, sizeof(char)* 7); // convert character array to integer
			elemType C = modExponent(toN, _PU_KEY, _N); // encrypting

			char temp[9]; // array used to piece
			temp[8] = 0;
			memcpy(temp, &C, sizeof(char)* 8); // convert integer to character array
			for (int j = 0; j < 8; j++) s += temp[j];
		}
	}else { // decryption
		int rank = _Text.length() / 8;
		unsigned int k = 0;
		for (int i = 0; i < rank; i++) {
			elemType toN = 0;
			char plainBlock[9];
			for (int j = 0; j < 8 && k < _Text.length(); j++) plainBlock[j] = _Text[k++];

			memcpy(&toN, plainBlock, sizeof(char)* 8);
			elemType C = modExponent(toN, _PR_KEY, _N);

			char temp[8];
			temp[7] = 0;
			memcpy(temp, &C, sizeof(char)* 7);
			for (int j = 0; j < 7; j++) s += temp[j];
		}
	}
	return s;
}