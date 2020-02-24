/******************************************************************************************
 * File Name: vector_operation.h
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
T & Vector<T>::operator[](Rank r) const {
	return get(r);
}

template<typename T>
Vector<T> & Vector<T>::operator=(Vector<T> const &V) {
	if(_elem) delete[] _elem;
	copyFrom(V._elem, 0, V._size);
	return *this;
}
