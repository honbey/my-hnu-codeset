/******************************************************************************************
 * Files Name: fano.cpp
 *
 * Honbey, honbey@honbey.com
 * Created On 2019-11-08
 * Copyright (c) 2019. All rights reserved.
 *
 * Date: 2019-12-15
 *
 * Description: For experiment 8.
 *   C++ program for Fano Coding.
 *
 ******************************************************************************************/
#include <iostream>
#include <cmath>
#include <string>

#define N 7

using namespace std;

void toFano(int, int, float*, float, string*);
float absolute(float);

string str[N];

int main(int argc, char const *argv[]){
	float p[N] = {0.2, 0.19, 0.18, 
		0.17, 0.15, 0.10, 0.01}, 
		sum = 0.0;

	float *s = new float[N];
	string *str = new string[N];

	for (int i = 0; i < N; s[i++] = sum) sum += p[i];

	toFano(0, N, s, sum, str);
	cout << "消息符号\t" << "符号概率\t" << "累加概率\t" << "码长\t\t" << "码字" << endl;
	for (int i = 0; i < N; i++) 
		cout << i + 1 << "\t\t" << p[i] << "\t\t" << s[i] << "\t\t" << str[i].length() << "\t\t" << str[i] << endl;

	delete[] str;
	delete[] s;
	return 0;
}

void toFano(int lo, int hi, float *a, float sum, string *str) {
	if (hi - lo < 2) return;
	else {
		float DValue = sum - 2 * a[lo], s = 0.0;
		int index = lo;
		for (int i = lo + 1; i < hi && absolute(sum - 2 * a[i]) < DValue; i++) {
			DValue = sum - 2 * a[i];
			index = i;
		}
		for (int i = lo; i < index + 1; i++) str[i] += "0";
		for (int i = index + 1; i < hi; i++) str[i] += "1";
		toFano(lo, index + 1, a, a[index], str);
		toFano(index + 1, hi, a, sum - a[index], str);
	}
}

float absolute(float a) {
	if (a < 0) return -a;
	else return a;
}