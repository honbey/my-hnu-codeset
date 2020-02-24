/******************************************************************************************
 * Class Name: vector.h
 * 
 * Honbey, honbey@honbey.com
 * Created On 2019-09-06
 * Copyright (c) 2019. All rights reserved.
 * 
 * Interface List:
 * 
 * 
 * Description: Definition/Declaration of macro constant and main class and function interface.
 * 
 ******************************************************************************************/

#pragma once

#define DEFAULT_CAPACITY 10

typedef int Rank;

template<typename T>
class Vector {
private:
	Rank _size; int _capacity; T *_elem; // element type

protected:
	void copyFrom(T const *A, Rank lo, Rank hi);
	void expend();
	void shrink();
	Rank bubble(Rank lo, Rank hi);
	void bubbleSort(Rank lo, Rank hi);
	void merge(Rank lo, Rank mi, Rank hi);
	void mergeSort(Rank lo, Rank hi);
	Rank max(Rank lo, Rank hi);
	void selectionSort(Rank lo, Rank hi);
	Rank partition(Rank lo, Rank hi);
	void quickSort(Rank lo, Rank hi);
	void heapSort(Rank lo, Rank hi);

public:
	Vector(int c = DEFAULT_CAPACITY) {
		_elem = new T[_capacity = c];
		_size = 0;
	}
	Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) {
		_elem = new T[_capacity = c]; 
		for(_size = 0; _size < s; _elem[_size++] = v);
	}
	Vector(T const *A, Rank lo, Rank hi) { 
		copyFrom(A, lo, hi);
	}
	Vector(Vector<T> const &V, Rank lo, Rank hi) {
		copyFrom(V._elem, lo, hi);
	}
	Vector(Vector<T> const &V) {
		copyFrom(V._elem, 0, V._size);
	}

	~Vector() {
		delete[] _elem; // release memory
	}
	
	// Abstract Data Type
	Rank size() const;
	bool empty() const;
	Rank find(T const &e) const;
	Rank find(T const &e, Rank lo, Rank hi) const;
	Rank search(T const &e) const;
	Rank search(T const &e, Rank lo, Rank hi) const;
	T & get(Rank r) const;
	bool disordered() const;

	T & operator[](Rank r) const;
	Vector<T> & operator=(Vector<T> const &V);

	bool put(Rank r, T const &e);
	Rank remove(Rank r);
	Rank remove(Rank lo, Rank hi);
	Rank insert(T const &e);
	Rank insert(Rank r, T const &e);
	void sort();
	void sort(Rank lo, Rank hi);
	Rank deduplicate();
	Rank uniquify();

	void traverse(void(*) (T&));
	template<typename VST> void traverse(VST&);
};

#include "vector_implementation.h"