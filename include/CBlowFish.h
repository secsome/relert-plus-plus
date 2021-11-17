#pragma once

#include <Always.h>

class CBlowFish
{
public:
    CBlowFish();
    void SetKey(byte* key, uint32_t size);
    uint32_t* Encrypt(uint32_t* data, uint32_t length);
    uint32_t* Decrypt(uint32_t* data, uint32_t length);

private:
    uint32_t S(uint32_t x, int i);
    uint32_t bf_f(uint32_t x);
    void Round(uint32_t& a, uint32_t b, int n);
    uint32_t SwapBytes(uint32_t i);
    void Encrypt(uint32_t& a, uint32_t& b);
    void Decrypt(uint32_t& a, uint32_t& b);

	uint32_t m_p[18];
	uint32_t m_s[4][256];
};