/******************************************************************************************
 * Files Name: comprehensiveCalc.cpp
 *
 * Honbey, honbey@honbey.com
 * Created On 2019-10-03
 * Copyright (c) 2019. All rights reserved.
 *
 * Date: 2019-12-15
 *
 * Description: For experiment 2, 3.
 *   C program to calculate.
 *
 ******************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define ENTROPY_HX  1
#define ENTROPY_HY  0
#define ENTROPY_XY -1

int toOne(float*, int, int);
int checkZero(float); // must check P is ZERO or not, or the result is wrong
void calcBinaryLog(float);
float calcEntropy(float*, int, int, int);

int main(int argc, char const* argv[]) {
	srand(time(0)); // random time seed
	int row, col; // row and column
	printf("Please enter the row and column:");
	scanf("%d %d", &row, &col);
	
	float *p = (float *)malloc(sizeof(float) * row * col); // apply memory
	for(int i = 0; i < row * col; i++) { // random value
		p[i] = rand() % 10; // time seed
	}
	/*p[0] = 0.125;   // test data
	p[1] = 0.375;p[2] = 0.375;p[3] = 0.125;*/

	if(!toOne(p, row, col)) return 0; // to One	
	
	float X = calcEntropy(p, row, col, ENTROPY_HX); // H(X)
	float Y = calcEntropy(p, row, col, ENTROPY_HY); // H(Y)
	float XY = calcEntropy(p, row, col, ENTROPY_XY);// H(XY)
	printf("H(X|Y): %f\n", XY - Y);					// H(X|Y)
	printf("I(X;Y): %f\n", X - (XY - Y));			// I(X;Y)

	free(p);
	return 0;
}

int toOne(float *a, int row, int col) {
	float sum = 0;
	for(int i = 0; i < row * col; i++) sum += a[i];
	for(int i = 0; i < row * col; i++) {
		a[i] /= sum;
		printf("%f\t", a[i]);
		if((i + 1) % row == 0) printf("\n");
	}
	if(sum > 0) return 1;
	else return 0;
}

int checkZero(float x) {
	if(x == 0) return 1;
	else return 0;
}

void calcBinaryLog(float) {}

float calcEntropy(float *a, int row, int col, int flag) {	
	float H = 0;
	int rank = row;
	if(flag == 0) {
		rank = col;
		col = row;
	} else if(flag == -1) {
		for(int i = 0; i < col * row; i++)  // union H(XY)
			if(!checkZero(a[i])) H += - a[i] * log(a[i]) / log(2);
		printf("jointEntropy:%f\n", H);
		return H;
	}
	
	float *t = (float*)malloc(sizeof(float) * rank);
	t[rank] = 0;
	for(int i = 0; i < rank; i++) {
		t[i] = 0;
		for(int j = 0; j < col; j++) {
			if(flag) t[i] += a[j + i * rank];
			else t[i] += a[i + j * rank];
		}
		//printf("P%d: %f\n", i + 1, t[i]); // input the sum of X
		if(!checkZero(t[i])) H += - t[i] * log(t[i]) / log(2);
	}
	printf("X/Y selfEntropy:%f\n", H);
	free(t);
	return H;
}

/* experiment 2
int main(int argc, char const *argv[]) {
	int n;
	float sum = 0, H = 0;
	printf("Notations: ");
	scanf("%d", &n);
	float *p = (float*)malloc(sizeof(float) * n);
	for(int i = 0; i < n; i++) {
		printf("p(X%d):", i + 1);
		scanf("%f", p + i);
		if(p[i]) H += - p[i] * log(p[i]) / log(2.0);
		sum += p[i];
	}
	
	if (sum != 1.0) printf("Error: \n");
	else printf("H: %f bit/symbol\n", H);

	free(p);
	return 0;
}
*/