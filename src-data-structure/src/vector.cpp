/******************************************************************************************
 * Project Name: vector
 * 
 * Honbey, honbey@honbey.com
 * Created On 2019-09-06
 * Copyright (c) 2019-2020. All rights reserved.
 * 
 * Date: 2020-02-24
 * Version: 0.2.0
 * 
 * Description: Learning DSA(Data Structure & Algorithms).
 *   Vector and some sorting algorithms.
 * 
 *  
 * History: 
 * <author> <time> <version > <desc>
 * Honbey 2019-09-06 0.1.0 Main frame
 ******************************************************************************************/

#include <iostream>
#include <ctime>

#include "vector/vector.h"

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

	Vector<int> testVector(testArray, 0, N);
	testVector.sort(0, N);
	std::cout << "\n Test sorting: \n";
	for(int i = 0; i < N; i++) {
		std::cout << testVector[i] << " ";
	}

	std::cout << "\n Test size(): " << testVector.size() << "\n";
	std::cout << "\n Test empty(): " << testVector.empty() << "\n";
	std::cout << "\n Test disordered(): " << testVector.disordered() << "\n";
	std::cout << "\n Test search(): " << testVector.search(120) << "\n";
	std::cout << "\n Test remove(): " 
		<< testVector.remove(0, static_cast<int>(N / 2)) << "\n";
	testVector.traverse(display);

	std::cout << "\n Test uniquify(): " 
		<< testVector.uniquify() << "\n";
	testVector.traverse(display);

	std::cout << "\n Test insert(): " 
		<< testVector.insert(10, 9) << "\n";
		testVector.insert(10, 9);
		testVector.insert(10, 9);
	testVector.traverse(display);

	std::cout << "\n Test find(): " << testVector.find(120) << "\n";

	std::cout << "\n Test deduplicate(): " 
		<< testVector.deduplicate() << "\n";
	testVector.traverse(display);

	std::cout << "\n Test get(): " << testVector.get(2) << "\n";
	std::cout << "\n Test put(): " 
		<< testVector.put(2, 119) << "\n";
	testVector.traverse(display);

	Vector<int> V(20, 12, 2);
	std::cout << "\n Test copyFrom(): ";
	V.traverse(display);

	std::cout << "\n";
	return 0;
}

void display(int n) {
	printf("%d ", n);
}