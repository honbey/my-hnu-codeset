/******************************************************************************************
 * File Name: basic_mathematics.h
 * 
 * Honbey, honbey@honbey.com
 * Created On 2019-10-13
 * Copyright (c) 2019. All rights reserved.
 * 
 * Function List: 
 * elemType GCD(elemType, elemType)
 * elemType findModInverse(elemType, elemType)
 * void extraGCD(elemType, elemType, elemType, elemType)
 * elemType multiOverflow(elemType, elemType, elemType)
 * elemType modExpA(elemType, elemType, elemType)
 * elemType modExpM(elemType, elemType, elemType)
 * 
 * Description: basic mathematics function
 * 
 ******************************************************************************************/

#pragma once

// using Euclidean algorithm to find GCD
elemType simpleRSA::GCD(elemType _a, elemType _b) { 
	while (_b) {
		elemType temp = _a % _b;
		_a = _b;
		_b = temp;
	}
	return _a;
}

// using extended Eculidean algorithm to find calculate '1 = vs + ut'
void simpleRSA::extraGCD(elemType _a, elemType _b, elemType *s, elemType *t) {
	if (_b) extraGCD(_b, _a % _b, s, t); // recursive solution
	else {
		*s = 1;
		*t = 0;
		return;
	}
	elemType temp = *t, temp_ = *s - _a / _b * (*t);
	*s = temp;
	*t = temp_;
}

// find modulus inverse by extended Eculidean algorithm
elemType simpleRSA::findModInverse(elemType _a, elemType _m) {
	elemType s, t;
	if (GCD(_a, _m) == 1) {
		extraGCD(_a, _m, &s, &t); // need the result 's' of extraGCD
		if (s < 0) s += _m; // always positive
		return s;
	}
	else {
		//std::cout << "error: " << _a << "," << _m << " are NOT coprime!" << std::endl;
		return 0;
	}
}

// a number which greater than 2^32, it's square will overflow,
// need to change the multipliction process into addition process
elemType simpleRSA::multiOverflow(elemType _a, elemType _b, elemType _m) {
	elemType a = 0, b = _a % _m;
	for (int i = 0; i < 64 && _b > 0; i++) { // i: power indicator of 2
		if (_b & 1) a = (a + b) % _m; // compute binary digit for next step
		b = (b * 2) % _m;
		_b >>= 1;
	}
	return a % _m;
}

// use addition(multipliction()) to calculate a^b (mod m) 
elemType simpleRSA::modExpA(elemType _a, elemType _b, elemType _m) {
	elemType a = 1, b = _a;
	for (; _b > 0; ) {
		if (_b & 1) a = multiOverflow(a, b, _m);
		b = multiOverflow(b, b, _m);
		_b >>= 1;
	}
	return a % _m;
}

// use multiplication to calculate a^b (mod m) 
elemType simpleRSA::modExpM(elemType _a, elemType _b, elemType _m) {
	elemType a = 1, b = _a;
	for (; _b > 0; ) {
		if (_b & 1) a = a * b % _m;
		b = b * b % _m;
		_b >>= 1;
	}
	return a % _m;
}