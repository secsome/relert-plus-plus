#pragma once

#include <Always.h>

// Codes From XCC
// 

class CBlowFish
{
public:
	void SetKey(const byte* key, int cb_key);
	void Encipher(int& xl, int& xr) const;
	void Encipher(const void* s, void* d, int size) const;
	void Decipher(int& xl, int& xr) const;
	void Decipher(const void* s, void* d, int size) const;
private:
	int S(int x, int i) const;
	int bf_f(int x) const;
	void ROUND(int& a, int b, int n) const;
	int reverse(int v) const;
	static const int p_[18];
	static const int s_[4][256];

	int p[18];
	int s[4][256];
};