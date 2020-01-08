/******************************************************************************************
 * File Name: prime_operation.h
 * 
 * Honbey, honbey@honbey.com
 * Created On 2019-10-13
 * Copyright (c) 2019. All rights reserved.
 * 
 * Function List: 
 * bool FermatCheck()
 * bool MillerRabin()
 * 
 * Description: function of prime number 
 * 
 ******************************************************************************************/

#pragma once

// primality test with Fermat's small theorem
bool simpleRSA::FermatCheck(elemType _n, short _t) {
	if (_n < 3 || _n % 2 == 0 || _n % 3 == 0) return false;
	for (int i = 0; i < _t; i++) {
		elemType b = rand() % (_n - 1) + 1; // random integer b, 2 <= b <= _n - 2
		if (GCD(b, _n) != 1) return false;
		elemType r = modExpA(b, _n - 1, _n); // calculate b^(_n-1)
		if (r != 1) return false; // if r not equal to 1, n is not a prime
	}
	return true;
}

// primality test with Miller-Rabin Algorithm
bool simpleRSA::MillerRabin(elemType _n, short _t) {
	if (_n < 3 || _n % 2 == 0 || _n % 3 == 0) return false;
	elemType t = _n - 1;
	while (t % 2 == 0) t >>= 1;
	for (int i = 0; i < _t / 2; i++) {
		elemType b = rand() % (_n - 1) + 1, temp = t;
		elemType r = modExpM(b, temp, _n);
		while (temp != _n - 1 && r != 1 && r != _n - 1) {
			r = multiOverflow(r, r, _n); // direct multiplication may cause integer overflow
			temp *= 2;
		}
		if (r != _n - 1 && temp % 2 == 0) return false;
	}
	return true;
}

// find a prime which which less than 1e11, Reference LINK: http://acm.hdu.edu.cn/showproblem.php?pid=5901