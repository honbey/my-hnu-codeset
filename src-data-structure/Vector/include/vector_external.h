/******************************************************************************************
 * File Name: vector_external.h
 * 
 * Honbey, honbey@honbey.com
 * Created On 2019-09-06
 * Copyright (c) 2019. All rights reserved.
 * 
 * Description: 
 * 
 ******************************************************************************************/

#pragma once

template<typename T>
static Rank binSearch(T *A, T const &e, Rank lo, Rank hi) {
	while(lo < hi) {
		Rank mi = (lo + hi) >> 1;
		(e < A[mi]) ? (hi = mi) : (lo = mi + 1);
	}
	return --lo;
}

template<typename T>
Rank Vector<T>::size() const {
	return _size;
}

template<typename T>
bool Vector<T>::empty() const {
	if(_size == 0) return true;
	else return false;
}

template<typename T>
Rank Vector<T>::find(T const& e) const {
	return find(e, 0, _size);
}

template<typename T>
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const {
	for(; (lo < hi) && (e != _elem[hi]); hi--);
	return hi;
}

template<typename T>
Rank Vector<T>::search(T const& e) const {
	return binSearch(_elem, e, 0, _size);
}

template<typename T>
Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const {
	return binSearch(_elem, e, lo, hi);
}

template<typename T>
T & Vector<T>::get(Rank r) const {
	if(r > _size) return _elem[_size];
	else if(r < 0) return _elem[_size + r];
	return _elem[r];
}

template<typename T>
bool Vector<T>::disordered() const {
	int count = 0;
	for(Rank i = 1; (i < _size) && !count; i++)
		if(_elem[i - 1] > _elem[i]) count = 1;
	return count;
}

template<typename T>
bool Vector<T>::put(Rank r, T const &e) {
	if (r < _size && -1 < r) {
		remove(r);
		insert(r, e);
		return true;
	}
	else return false;
}

template<typename T>
Rank Vector<T>::remove(Rank r) {
	T e = _elem[r];
	remove(r, r + 1);
	return e;
}

template<typename T>
Rank Vector<T>::remove(Rank lo, Rank hi) { // remove [lo, hi)
	if(lo == hi) return 0;
	while(hi < _size) _elem[lo++] = _elem[hi++];
	_size = lo;
	shrink();
	return hi - lo;
}

template<typename T>
Rank Vector<T>::insert(T const &e) {
	return insert(_size, e);
}

template<typename T>
Rank Vector<T>::insert(Rank r, T const &e) {
	expend();
	Rank i = _size;
	for(; i > r; i--) _elem[i] = _elem[i - 1];
	_elem[i] = e;
	_size++;
	return r;
}

template<typename T>
void Vector<T>::sort() {
	sort(0, _size);
}

template<typename T>
void Vector<T>::sort(Rank lo, Rank hi) {
	switch(rand() % 3) {
		case 1: {
			printf("\nBubbleSort\n");
			bubbleSort(lo, hi);
			break;
		}
		case 2: {
			printf("\nMergeSort\n");
			mergeSort(lo, hi);
			break;
		}
		default: {
			printf("\nQuickSort\n");
			quickSort(lo, hi);
			break;
		}
	}
}

template<typename T>
Rank Vector<T>::deduplicate() {
	Rank oldSize = _size;
	for(Rank i = -1; ++i < _size - 1; ){
		for(Rank j = i + 1; j < _size; )
			if(_elem[i] == _elem[j]) remove(j);
			else j++;
	}
	return oldSize - _size;
}
template<typename T>
Rank Vector<T>::uniquify() {
	Rank i = 0, j = 0;
	while(++j < _size)
		if(_elem[i] != _elem[j]) _elem[++i] = _elem[j];
	_size = ++i;
	shrink();
	return j - 1;
}

template<typename T>
void Vector<T>::traverse(void (*visit) (T&)) {
	for(Rank i = 0; i < _size; i++) visit(_elem[i]); // function point
}

template<typename T> template<typename VST>
void Vector<T>::traverse(VST &visit) {
	for(Rank i = 0; i < _size; i++) visit(_elem[i]);
}