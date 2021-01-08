/******************************************************************************************
 * File Name: vector_internal.h 
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
static void swap(T &x, T &y) {
	T temp;
	temp = x;
	x = y;
	y = temp;
}

template<typename T>
void Vector<T>::copyFrom(T const *A, Rank lo, Rank hi) { // [lo, hi)
	_elem = new T[_capacity = 2 * (hi - lo)]; // apply memory
	_size = 0; // set zero
	while(lo < hi) _elem[_size++] = A[lo++]; // copy element
}

template<typename T>
void Vector<T>::expend() { // overflow
	if(_size < _capacity) return;
	if(_size < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY;
	T *oldElem = _elem;
	_elem = new T[_capacity * 2]; // _capacity <<= 1
	for(Rank i = 0; i < _size; i++) _elem[i] = oldElem[i];
	delete[] oldElem;
}

template<typename T>
void Vector<T>::shrink() { // underflow, load factor(_size/_capacity << 50%)
	if(_capacity < (DEFAULT_CAPACITY << 1)) return;
	if((_size << 2) > _capacity) return;
	T* oldElem = _elem;
	_elem = new T[_capacity / 2]; // _capacity >>= 1
	for(Rank i = 0; i < _size; i++) _elem[i] = oldElem[i];
	delete[] oldElem;
}

template<typename T>
Rank Vector<T>::bubble(Rank lo, Rank hi) {
	Rank last = lo;
	while(++lo < hi) {
		if(_elem[lo - 1] > _elem[lo]) {
			last = lo;
			swap(_elem[lo - 1], _elem[lo]);
		}
	}
	return last;
}

template<typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi) {
	while(lo < (hi = bubble(lo, hi)));
}

template<typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi) {
	T *A = _elem + lo;
	Rank lb = mi - lo;
	T *B = new T[lb];
	for(Rank i = 0; i < lb; i++) B[i] = A[i];
	Rank lc = hi - mi;
	T *C = _elem + mi;
	for(Rank i = 0, j = 0, k = 0; j < lb; ) {
		if((k < lc) && ( C[k] < B[j])) A[i++] = C[k++];
		if((lc <= k) || (B[j] <= C[k])) A[i++] = B[j++];
	}
	delete[] B;
}

template<typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi) {
	if(hi - lo < 2) return;
	Rank mi = (lo + hi) >> 1;
	mergeSort(lo, mi);
	mergeSort(mi, hi);
	merge(lo, mi, hi);
}

template<typename T>
Rank Vector<T>::max(Rank lo, Rank hi) {
	Rank M = hi;
	while(lo < hi--)
		if(_elem[hi] > _elem[M]) M = hi;
	return M;
}

template<typename T>
void Vector<T>::selectionSort(Rank lo, Rank hi) {

}

template<typename T>
Rank Vector<T>::partition(Rank lo, Rank hi) {
	swap(_elem[lo], _elem[lo + rand() % (hi - lo + 1)]);
	T pivot =  _elem[lo];
	Rank mi = lo;
	for(Rank k = lo + 1; k <= hi; k++)
		if(_elem[k] < pivot) swap(_elem[++mi], _elem[k]);
	swap(_elem[lo], _elem[mi]);
	return mi;
}

template<typename T>
void Vector<T>::quickSort(Rank lo, Rank hi) {
	if (hi - lo < 2) return;
	Rank mi = partition(lo, hi - 1);
	quickSort(lo, mi);
	quickSort(mi + 1, hi);
}