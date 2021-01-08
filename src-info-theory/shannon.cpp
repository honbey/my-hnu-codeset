/******************************************************************************************
 * Files Name: shannon.cpp
 *
 * Honbey, honbey@honbey.com
 * Created On 2019-10-24
 * Copyright (c) 2019. All rights reserved.
 *
 * Date: 2019-12-15
 *
 * Description: For experiment 7.
 *   C++ program for Shannon Coding.
 *
 ******************************************************************************************/
#include <iostream>
#include <cmath>
#include <string>

using namespace std;

typedef float elemType;

void swap(elemType*, elemType*);
int partition(elemType*, int, int);
void quickSort(elemType*, int, int);
string toBinary(elemType, int);

int main(int argc, char const *argv[])
{
	float p[7] = {0.2, 0.18, 0.19, 0.15, 0.1, 0.01, 0.17};
	quickSort(p, 0, 7);
	int *temp = new int[7];
	for (int i = 0; i < 7; i++) {
		temp[i] = (int)(-log((double)p[i]) / log(2.0)) + 1;
	}

	float *sum = new float[7];
	float j = 0.0;

	cout << "消息符号\t" << "符号概率\t" << "累加概率\t" << "码长\t\t" << "码字" << endl;
	for (int i = 0; i < 7; i++) {
		sum[i] = j;
		j += p[i];
		//cout << sum[i] << endl;
		cout << i << "\t\t" << p[i] << "\t\t" << sum[i] << "\t\t" << temp[i] << "\t\t" << toBinary(sum[i], temp[i]) << endl;
	}

	delete[] temp;
	delete[] sum;
	return 0;
}

void swap(elemType* x, elemType* y) {
	elemType temp;
	temp = *x;
	*x = *y;
	*y = temp;
}

int partition(elemType *A, int lo, int hi) {
	swap(&A[lo], &A[lo + rand() % (hi - lo + 1)]);
	elemType pivot =  A[lo];
	int mi = lo;
	for(int k = lo + 1; k <= hi; k++)
		if(A[k] > pivot)
			swap(&A[++mi], &A[k]);
	swap(&A[lo], &A[mi]);
	return mi;
}

void quickSort(elemType *A, int lo, int hi) {
	if (hi - lo < 2) return;
	int mi = partition(A, lo, hi - 1);
	quickSort(A, lo, mi);
	quickSort(A, mi + 1, hi);
}

string toBinary(elemType x, int n) {
	string s = "";
	for (int i = 0; i < n; i++) {
		if ((x *= 2) > 1) {
			s += "1";
			x -= 1;
		}
		else s += "0";
	}
	return s;
}