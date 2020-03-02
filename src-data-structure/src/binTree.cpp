/******************************************************************************************
 * Project Name: binTree
 * 
 * Honbey, honbey@honbey.com
 * Created On 2019-09-06
 * Copyright (c) 2019-2020. All rights reserved.
 * 
 * Date: 2020-03-02
 * Version: 0.2.0
 * 
 * Description: Learning DSA(Data Structure & Algorithms).
 *   Binary Tree.
 * 
 *  
 * History: 
 * <author> <time> <version > <desc>
 * Honbey 2019-09-06 0.1.0 Main frame
 ******************************************************************************************/

#include <iostream>
#include <ctime>

#include "binTree/binTree.h"

#define N 100

void display(int);

int main(int argc, char const *argv[]) {
	// Test data
	srand((unsigned)time(NULL));
	printf("Data-Time:%s  %s\nFileName:%s\nLine:%d\n", __DATE__, __TIME__, __FILE__, __LINE__);
	int testArray[N];
	for(int i = 0; i < N; i++) {
		testArray[i] = rand() % (2 * N);
		std::cout << testArray[i] << " ";
	}

	BinTree<int> myBinTree;
	myBinTree.insertAsRoot(1);
	auto r = myBinTree.root();
	myBinTree.insertAsLC(r, 2);
	myBinTree.insertAsRC(r, 3);
	printf("\n");
	myBinTree.travIn(display);
	printf("\n");
	myBinTree.travPost(display);
	printf("\n");
	myBinTree.travPre(display);
	printf("\n");
	myBinTree.travLevel(display);
	myBinTree.remove(r->lc);
	printf("\n%d\n", myBinTree.size());

	return 0;
}

void display(int n) {
	printf("%d ", n);
}