/******************************************************************************************
 * Files Name: linearBlock.cpp
 *
 * Honbey, honbey@honbey.com
 * Created On 2019-12-05
 * Copyright (c) 2019. All rights reserved.
 *
 * Date: 2019-12-15
 *
 * Description: For experiment 11.
 *   C++ program for Linear Block Code.
 *
 ******************************************************************************************/
#include <iostream>
#include <string>

#define getbit(x,y) x&=(1<<y)

using namespace std;

int main(int argc, char const *argv[]) {
	bool generatorMat[4][7] = {
		1, 0, 0, 0, 1, 0, 1,
		0, 1, 0, 0, 1, 1, 1,
		0, 0, 1, 0, 1, 1, 0,
		0, 0, 0, 1, 0, 1, 1
	};
	bool codeMat[4][16];
	bool codedMat[7][16];
	printf("码字\t(校验位)\n");
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 4; j++) {
			int k = i;
			codeMat[j][i] = getbit(k, (3 - j));
			codedMat[j][i] = codeMat[j][i];
			printf("%d", codedMat[j][i]); 
		}
		printf("\t");
		for (int j = 4; j < 7; j++) {
			codedMat[j][i] = generatorMat[0][j] * 
			codeMat[0][i] ^ generatorMat[1][j] * 
			codeMat[1][i] ^ generatorMat[2][j] * 
			codeMat[2][i] ^ generatorMat[3][j] * 
			codeMat[3][i];
			printf("%d", codedMat[j][i]); 
		}
		printf("\n");
	}

	// calculate d
	int d = 7;
	for (int i = 1; i < 16; i++) {
		int k = 0;
		for (int j = 0; j < 7; j++) if (codedMat[j][i] == 1) k++;
		if (k < d) d = k;
	}
	printf("最小码距：%d\n", d);

	return 0;
}