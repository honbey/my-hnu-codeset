/******************************************************************************************
 * File Name: aes_galois_field.h
 * 
 * Honbey, honbey@honbey.com
 * Created On 2019-11-11
 * Copyright (c) 2019. All rights reserved.
 * 
 * Function List:
 * struct GFPloy;
 * void GFInit();
 * GFMultiOver(uchar, uchar)
 * GFMultiTrave(uchar, uchar)
 * GFMultiTable(uchar, uchar)
 * GFModInvOver(uchar, uchar)
 * GFModInvTrave(uchar, uchar)
 * GFModInvTable(uchar, uchar)
 * GFDivis(uchar, uchar)
 * 
 * Description: declare operation of Galois Filed (GF(2^8))
 * 
 ******************************************************************************************/

#pragma once

typedef struct GFPoly {
	// class GFPoly {
	// private:
	uint _data, _degree = 0;
	// public: / external interface
	GFPoly() { _data = 0; }
	GFPoly(uint data) { for (_data = data; data; _degree++) data >>= 1; }
	~GFPoly() {}
	uint data() const { return _data; }
	uint degree() const { return _degree; }
	GFPoly operator+ (const GFPoly& x) { return _data ^ x._data; } // XOR
	GFPoly operator- (const GFPoly& x) { return _data ^ x._data; } // XOR
	GFPoly operator* (const GFPoly& x) { 
		GFPoly y(0), temp = x;
		for (int i = 0; temp._data; i++, temp._data >>= 1)
			if (temp._data & 1) y = y + GFPoly(_data << i); // (...) XOR (...) XOR (...) ...
		return y;
	}
	GFPoly operator/ (const GFPoly& x) { 
		if (x._data == 0) printf("error: Zero Exception"); // the dividend cannot be zero
		uint diffDeg = ((_degree > x._degree) ? (_degree - x._degree) : (x._degree - _degree)); // make sure different degree 'd' is a positive number
		GFPoly q(0);
		for (GFPoly r = *this; diffDeg >> 31 != 1; diffDeg--) { // while d >= 0
			if (r._degree > (x._degree + diffDeg - 1)) { // if deg x > deg(*this), return q(0), if deg(*this) > deg x, use Division with Remainder
				q._data += (1 << diffDeg); // q._data + 1 * pow(2, diffDeg)
				r = r - GFPoly(x._data << diffDeg); // x._data * pow(2, diffDeg);
			}
		}
		return q;
	}
	GFPoly operator% (const GFPoly& x) {
		uint diffDeg = ((_degree > x._degree) ? (_degree - x._degree) : (x._degree - _degree)); // make sure different degree 'd' is a positive number
		GFPoly r = *this;
		for (GFPoly q(0); diffDeg >> 31 != 1; diffDeg--) { // while d >= 0
			if (r._degree > (x._degree + diffDeg - 1)) { // if deg x > deg(*this), return *this, if deg(*this) > deg x, use Division with Remainder
				q._data += (1 << diffDeg); // q._data * pow(2, diffDeg)
				r = r - GFPoly(x._data << diffDeg); // x._data * pow(2, diffDeg);
			}
		}
		return r;
	}
	bool operator== (const GFPoly& x) { return _data == x._data; }
	bool operator== (const uint& x) { return _data == x; }
} GFPoly;

template<typename T>
T Euclid(T _x, T _y) { // Euclidean algorithm
	while (!(_y == 0)) {  // if (!(_y == _x)) return Euclid(_y, _x % _y);
		T temp = _x % _y;
		_x = _y;
		_y = temp;
	}
	return _x;
}
template<typename T>
void exEuclid(T _x, T _y, T& s, T& t) { // extend Euclidean algorithm
	if (!(_y == 0)) exEuclid(_y, _x % _y, s, t);
	else {
		s = T(1);
		t = T(0);
		return;
	}
	T temp = t, temp_ = s - _x / _y * t;
	s = temp;
	t = temp_;
}

void simpleAES::GFInit() { // init Tables
	_itopTable[0] = 1; _modInvTable[0] = 0;
	for (int i = 1; i < 255; i++) { // [1, 255) int convert to polynomial
		_itopTable[i] = (_itopTable[i - 1] << 1) ^ _itopTable[i - 1]; // x^i = x^(i-1) * (x+1)
		if (_itopTable[i - 1] & 0x80) _itopTable[i] ^= 0x1b; // when overload (XOR 0x11b)
	}
	for (int i = 0; i < 255; i++) _ptoiTable[_itopTable[i]] = i; // [0, 255) polynomial convert to int
	for (int i = 1; i < 256; i++) _modInvTable[i] = _itopTable[(255 - _ptoiTable[i]) % 0xFF]; // GF(2^8) // mod Inverse Table
}

uchar simpleAES::GFMultiOver(uchar _x, uchar _y) { // multiplier through overload
	GFPoly p = GFPoly((uint)_x) * GFPoly((uint)_y);
	if (p.data() > 0xFF) return (uchar)((p % GFPoly(0x11b)).data()); // GF(2^8)
	else return (uchar)(p.data());
}

uchar simpleAES::GFMultiTrave(uchar _x, uchar _y) { // multiplier through traversing
	if (_x == 0 || _y == 0) return 0; // reduce unnecessary calculations
	uchar temp[8];
	temp[0] = _x; // initial
	for (int i = 1; i < 8; i++) temp[i] = (temp[i - 1] << 1) ^ ((temp[i - 1] & 0x80) ? 0x1b : 0x00); // overflow during calculation (XOR 0x11b)
	_x = (_y & 0x01) * _x; // initial
	for (int i = 1; i < 8; i++) _x ^= ((_y >> i) & 0x01) * temp[i]; // (...) XOR (...) XOR (...) ...
	return _x;
}

uchar simpleAES::GFMultiTable(uchar _x, uchar _y) { // multiplier through tables
	if (_x == 0 || _y == 0) return 0; // reduce unnecessary operations
	else return _itopTable[(_ptoiTable[_x] + _ptoiTable[_y]) % 255]; // GF(2^8)
}

uchar simpleAES::GFDivis(uchar _x, uchar _y) { // Division through traversing
	if (_y == 0) printf("error: Zero Exception"); // the dividend cannot be zero
	if (_x == 0 || _y == 0) return 0;
	else return _itopTable[(_ptoiTable[_x] - _ptoiTable[_y] + 255) % 255]; // GF(2^8)
}

uchar simpleAES::GFModInvOver(uchar _x) { // find mod Inverse through extend Euclidean algorithm
	if (_x == 0) return 0; // set the modulus inverse of 0 to 0
	GFPoly s(1), t(0); // initial
	exEuclid(GFPoly((uint)_x), GFPoly(0x11b), s, t); // s * _x + t * 0x11b = GFPoly(1)
	return (uchar)(s.data());
}

uchar simpleAES::GFModInvTrave(uchar _x) { // find mod Inverse through traversing
	for (int i = 1; i < 256; i++) if (GFMultiTrave(_x, i) == 1) return (uchar)i; // brute force
	return 0; // set the modulus inverse of 0 to 0
}

uchar simpleAES::GFModInvTable(uchar _x) { // find mod Inverse through tables
	if (_x != 0) return _modInvTable[_x];
	return 0; // set the modulus inverse of 0 to 0
}
