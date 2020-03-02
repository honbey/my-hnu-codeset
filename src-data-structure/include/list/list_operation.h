/******************************************************************************************
 * File Name: list_operation.h
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
T & List<T>::operator[](Rank r) const {
	ListNode<T> *p = first();
	for( ; 0 < r; r--) p = p->succ;
	return p->data;
}
