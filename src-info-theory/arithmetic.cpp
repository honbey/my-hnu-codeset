/******************************************************************************************
 * Files Name: arithmetic.cpp
 *
 * Honbey, honbey@honbey.com
 * Created On 2019-11-28
 * Copyright (c) 2019. All rights reserved.
 *
 * Date: 2019-12-15
 *
 * Description: For experiment 10.
 *   C++ program for Arithmetic Coding.
 *
 ******************************************************************************************/
#include <iostream>
#include <string>
#include <cmath>

#define N 4

using namespace std;

string toBinary(float, int);

int main(int argc, char const *argv[]) {
	float p[N] = { 0.50, 0.25, 0.125, 0.125 };
	float pr[N], A[N + 1], C[N + 1], D = 0.0;
	int seq[N] = { 2, 3, 1, 2 }, clen = 0;
	A[0] = 1;
	C[0] = 0;
	for (int i = 0; i < N; i++) {
		pr[i] = D;
		D += p[i];
	}

	for (int i = 1; i <= N; i++) {
		C[i] = C[i - 1] + A[i - 1] * pr[seq[i - 1] - 1];
		A[i] = A[i - 1] * p[seq[i - 1] - 1];
	}
	clen = (int)(-log((double)A[N]) / log(2.0));
	string encode, decode = "";
	encode = toBinary(C[N], clen);
	cout << "Encoded string: " << encode << endl;

	D = 0.0;
	for (size_t i = 0; i < encode.length() - 2; i++)
		D += ((int)encode[i + 2] - 48) * pow(0.5, (double)(i + 1));
	
	for (int i = 0; i < N; i++) {
		int k = 0;
		for (int j = 1; j < N; k = j++)
			if ( D >= pr[j - 1] && D < pr[j]) break;
		decode += (char)(k + 49);
		D = (D - pr[k]) / p[k];
	}
	cout << "Decoded string: " <<decode << endl;

	return 0;
}

string toBinary(float x, int n) {
	string s = "0.";
	for (int i = 0; i < n; i++) {
		if ((x *= 2) >= 1) {
			s += "1";
			x -= 1;
		} else s += "0";
	}
	return s;
}