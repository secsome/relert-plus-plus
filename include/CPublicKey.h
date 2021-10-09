#pragma once

#include <Always.h>

// Codes from XCC

class CPublicKey
{
public:
    const static char* pubkey_str;
    const static char char2num[0x100];

    using bignum = uint32_t[64];
    struct t_pubkey
    {
        bignum key1;
        bignum key2;
        uint32_t len;
    };

    static void get_blowfish_key(const byte* s, byte* d);

private:
    static void init_bignum(bignum n, uint32_t val, uint32_t len);
    static void move_key_to_big(bignum n, const char* key, uint32_t klen, uint32_t blen);
    static void key_to_bignum(bignum n, char* key, uint32_t len);
    static uint32_t len_bignum(bignum n, uint32_t len);
    static uint32_t bitlen_bignum(bignum n, uint32_t len);
    static void init_pubkey();
    static uint32_t len_predata();
    static int cmp_bignum(bignum n1, bignum n2, uint32_t len);
    static void mov_bignum(bignum dest, bignum src, uint32_t len);
    static void shr_bignum(bignum n, uint32_t bits, uint32_t len);
    static void shl_bignum(bignum n, uint32_t bits, uint32_t len);
    static uint32_t sub_bignum(bignum dest, bignum src1, bignum src2, uint32_t carry, uint32_t len);
    static void inv_bignum(bignum n1, bignum n2, uint32_t len);
    static void inc_bignum(bignum n, uint32_t len);
    static void init_two_dw(bignum n, uint32_t len);
    static void mul_bignum_word(bignum n1, bignum n2, uint32_t mul, uint32_t len);
    static void mul_bignum(bignum dest, bignum src1, bignum src2, uint32_t len);
    static void not_bignum(bignum n, uint32_t len);
    static void neg_bignum(bignum n, uint32_t len);
    static uint32_t get_mulword(bignum n);
    static void dec_bignum(bignum n, uint32_t len);
    static void calc_a_bignum(bignum n1, bignum n2, bignum n3, uint32_t len);
    static void calc_a_key(bignum n1, bignum n2, bignum n3, bignum n4, uint32_t len);
    static void process_predata(const byte* pre, uint32_t pre_len, byte* buf);

private:
    static t_pubkey pubkey;

    static bignum glob1;
    static uint32_t glob1_bitlen;
    static uint32_t glob1_len_x2;
    static uint32_t glob2[130];
    static uint32_t glob1_hi[4];
    static uint32_t glob1_hi_inv[4];
    static uint32_t glob1_hi_bitlen;
    static uint32_t glob1_hi_inv_lo;
    static uint32_t glob1_hi_inv_hi;

};