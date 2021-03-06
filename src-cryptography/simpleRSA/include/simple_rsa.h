/******************************************************************************************
 * Class Name: simple_rsa.h
 * 
 * Honbey, honbey@honbey.com
 * Created On 2019-10-13
 * Copyright (c) 2019. All rights reserved.
 * 
 * Interface List:
 * string encryption(string)
 * string decryption(string)
 * void changeKey(elemType, elemType, elemType)
 * bool primalityTest(elemType, short)
 * elemType primeGeneration(long)
 * elemType modExponent(elemType, elemType, elemType)
 * 
 * Description: Definition/Declaration of macro constant and main class and function interface.
 * 
 ******************************************************************************************/

#pragma once

#define DEFAULT_PU_KEY 65537
#define DEFAULT_P 4222234741
#define DEFAULT_Q 982451653
#define DEFAULT_PRIMAL_PRECISION 10
#define MODEL_ENCRYPTION true
#define MODEL_DECRYPTION false
#define LENGTH_SEVEN 7
#define LENGTH_EIGHT 8

typedef long long elemType;

class simpleRSA {
protected:
	elemType _p, _q, _eularN, _PU_KEY, _PR_KEY, _N;
	elemType *_keyPair_PU = new elemType[10];
	int defaultFlag = 0;

	void createFor(elemType, elemType, elemType);

	elemType GCD(elemType, elemType);
	void extraGCD(elemType, elemType, elemType*, elemType*);
	elemType findModInverse(elemType, elemType);
	elemType multiOverflow(elemType, elemType, elemType);
	elemType modExpA(elemType, elemType, elemType);
	elemType modExpM(elemType, elemType, elemType);
	bool FermatCheck(elemType, short);
	bool MillerRabin(elemType, short);
	std::string RSA(std::string Text, bool);

public:
	// default constructor function
	simpleRSA() {
		createFor(DEFAULT_P, DEFAULT_Q, DEFAULT_PU_KEY);
	}

	simpleRSA(elemType E) {
		createFor(DEFAULT_P, DEFAULT_Q, E);
	}

	simpleRSA(elemType p, elemType q) {
		defaultFlag = 1;
		createFor(p, q, DEFAULT_PU_KEY);
	}

	simpleRSA(elemType p, elemType q, elemType E) {
		defaultFlag = 1;
		createFor(p, q, E);
	}

	~simpleRSA() {
		delete[] _keyPair_PU;
	}
	
	// external ADT
	std::string encryption(std::string Text) { return RSA(Text, MODEL_ENCRYPTION); }
	std::string decryption(std::string Text) { return RSA(Text, MODEL_DECRYPTION); }

	// change P, Q and pubilc key
	void changeKey(elemType p, elemType q, elemType E) {
		defaultFlag = 2;
		createFor(p, q, E);
	}
	// calculate modulus with large exponent
	elemType modExponent(elemType a, elemType b, elemType m) { return modExpA(a, b, m); }
	// test whether a number is prime, default parameter 10
	bool primalityTest(elemType, short);
	bool primalityTest(elemType n) { return primalityTest(n, DEFAULT_PRIMAL_PRECISION); }
	// find a prime number n in given position x, n <= 1e11, x <= 1e8
	elemType primeGeneration(long);
};

// constructor function
void simpleRSA::createFor(elemType p, elemType q, elemType E) {
	if (defaultFlag && (!primalityTest(p) || !primalityTest(q) || (p * q < 0x1ffffffffffffff))) {
		std::cout << " P, Q will be generated by default setting.\n"
				  << " Prossible reasons are\n"
				  << "	1 P or Q are NOT a prime.\n	2 P × Q < 0X1FFFFFFFFFFFFFF.\n" << std::endl;
		_q = DEFAULT_Q;
		_p = DEFAULT_P;
	} else {
		_p = p;
		_q = q;
	}

	_eularN = (_p - 1) * (_q - 1);
	if (defaultFlag && (E < DEFAULT_PU_KEY || GCD(E, _eularN) > 1)) {
		std::cout << " E will be set as default value.\n" 
				  << " Prossible reasons are\n"
				  << "	1 E, (P-1)(Q-1) are NOT coprime.\n"
				  << "	2 E is too samll.\n" << std::endl;
		_PU_KEY = DEFAULT_PU_KEY;
	} else  _PU_KEY = E;
	std::cout << "	Default public key is: " << _PU_KEY << ".\n"
			  << "	Generating key pairs..." << std::endl;
	_PR_KEY = findModInverse(_PU_KEY, _eularN);
	_N = _p * _q;

	std::cout << "	N:\t" << _N << std::endl;
	std::cout << "	φ(N):\t" << _eularN << std::endl;
	std::cout << "	p:\t" << _p << std::endl;
	std::cout << "	q:\t" << _q << std::endl;
	std::cout << "	e:\t" << _PU_KEY << std::endl;
	std::cout << "	d:\t" << _PR_KEY << std::endl;

}

bool simpleRSA::primalityTest(elemType a, short t) {
	int flag = 1;
	if (flag) return MillerRabin(a, t);
	else return FermatCheck(a, t);
}

#include "rsa_implementation.h"