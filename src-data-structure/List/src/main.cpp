/******************************************************************************************
 * Project Name: list
 * 
 * Honbey, honbey@honbey.com
 * Created On 2019-09-06
 * Copyright (c) 2019-2020. All rights reserved.
 * 
 * Date: 2020-02-25
 * Version: 0.3.0
 * 
 * Description: Learning DSA(Data Structure & Algorithms).
 *   List and some algorithms.
 * 
 *  
 * History: 
 * <author> <time> <version > <desc>
 * Honbey 2019-09-06 0.1.0 Main frame
 * Honbey 2020-02-24 0.2.0 Code normalization
 ******************************************************************************************/

#include <iostream>
#include <ctime>

#include "list.h"

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

	List<int> testList;

	for(int i = 0; i < N; i++) {
		testList.insertAsLast(testArray[i]);
		//testList.insertAsFirst(testArray[i]);
	}
	printf("\nTest []\n");
	for(int i = 0; i < N; i++) printf("%d ", testList[i]);
	printf("\nTest sort() and reserve()\n");
	testList.sort();
	testList.reverse();
	testList.traverse(display);
	printf("\nTest disordered(), find() and search()\n");
	printf("%d\n", testList.disordered());
	testList.insertAsFirst(119);
	printf("%p\n", testList.find(119));
	testList.sort();
	printf("%p\n", testList.search(119));

	printf("\nTest deduplicate() and uniquify();\n");
	testList.insertAsFirst(119);
	testList.insertAsLast(119);
	printf("%d\n", testList.deduplicate());
	testList.insertAsFirst(119);
	testList.insertAsLast(119);
	testList.sort();
	printf("%d\n", testList.uniquify());

	return 0;
}

void display(int n) {
	printf("%d ", n);
}