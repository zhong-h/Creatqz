#include "sm3.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
# include <immintrin.h>

# define _mm_rotl_epi32(X,i) _mm_xor_si128(_mm_slli_epi32((X),(i)), _mm_srli_epi32((X),32-(i)))

/* unsigned char b[i] = unsigned long n ��i  -  i+8λ */
#ifndef PUT_ULONG_BE
#define PUT_ULONG_BE(n,b,i)                             \
{                                                       \
    (b)[i    ] = (unsigned char) ( (n) >> 24 );       \
    (b)[i + 1] = (unsigned char) ( (n) >> 16 );       \
    (b)[i + 2] = (unsigned char) ( (n) >>  8 );       \
    (b)[i + 3] = (unsigned char) ( (n)       );       \
}
#endif

void sm3_starts(sm3_context* ctx){
    ctx->iplen = 0;

    ctx->state[0] = 0x7380166F;
    ctx->state[1] = 0x4914B2B9;
    ctx->state[2] = 0x172442D7;
    ctx->state[3] = 0xDA8A0600;
    ctx->state[4] = 0xA96F30BC;
    ctx->state[5] = 0x163138AA;
    ctx->state[6] = 0xE38DEE4D;
    ctx->state[7] = 0xB0FB0E4E;
}


void sm3_process(sm3_context* ctx, uint8_t data[64]){
    int j;
    unsigned long SS1, SS2, TT1, TT2, W1[68], W2[64];
    unsigned long A, B, C, D, E, F, G, H;
    unsigned long T[64];
    __m128i X, K, R;
    __m128i M = _mm_setr_epi32(0, 0, 0, 0xffffffff);
    __m128i V = _mm_setr_epi8(3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12);
#ifdef _DEBUG
    int i;
#endif

    for (j = 0; j < 16; j ++){
        T[j] = 0x79CC4519;
    }
    for (j = 16; j < 64; j ++){
        T[j] = 0x7A879D8A;
    }

    for (j = 0; j < 16; j += 4){
        X = _mm_loadu_si128((__m128i*)(data + j * 4));
        X = _mm_shuffle_epi8(X, V);
        _mm_storeu_si128((__m128i*)(W1 + j), X);
    }

#ifdef _DEBUG
    printf("Message with padding:\n");
    for (i = 0; i < 8; i++)
        printf("%08x ", W1[i]);
    printf("\n");
    for (i = 8; i < 16; i++)
        printf("%08x ", W1[i]);
    printf("\n");
#endif

#define FF0(x,y,z) ( (x) ^ (y) ^ (z))
#define FF1(x,y,z) (((x) & (y)) | ( (x) & (z)) | ( (y) & (z)))

#define GG0(x,y,z) ( (x) ^ (y) ^ (z))
#define GG1(x,y,z) (((x) & (y)) | ( (~(x)) & (z)) )


#define SHL(x,n) (((x) & 0xFFFFFFFF) << (n))
#define ROTL(x,n) (SHL((x),n) | ((x) >> (32 - n)))

#define P0(x) ((x) ^  ROTL((x),9) ^ ROTL((x),17))
#define P1(x) ((x) ^  ROTL((x),15) ^ ROTL((x),23))


    //��Ϣ��չ
    for (j = 16; j < 68; j += 4){
        /* X = (W1[j - 3], W1[j - 2], W1[j - 1], 0) */
        X = _mm_loadu_si128((__m128i*)(W1 + j - 3));
        X = _mm_andnot_si128(M, X);

        X = _mm_rotl_epi32(X, 15);
        K = _mm_loadu_si128((__m128i*)(W1 + j - 9));
        X = _mm_xor_si128(X, K);
        K = _mm_loadu_si128((__m128i*)(W1 + j - 16));
        X = _mm_xor_si128(X, K);

        /* P1() */
        K = _mm_rotl_epi32(X, 8);
        K = _mm_xor_si128(K, X);
        K = _mm_rotl_epi32(K, 15);
        X = _mm_xor_si128(X, K);

        K = _mm_loadu_si128((__m128i*)(W1 + j - 13));
        K = _mm_rotl_epi32(K, 7);
        X = _mm_xor_si128(X, K);
        K = _mm_loadu_si128((__m128i*)(W1 + j - 6));
        X = _mm_xor_si128(X, K);

        /* W1[j + 3] ^= P1(ROL32(W1[j + 1], 15)) */
        R = _mm_shuffle_epi32(X, 0);
        R = _mm_and_si128(R, M);
        K = _mm_rotl_epi32(R, 15);
        K = _mm_xor_si128(K, R);
        K = _mm_rotl_epi32(K, 9);
        R = _mm_xor_si128(R, K);
        R = _mm_rotl_epi32(R, 6);
        X = _mm_xor_si128(X, R);

        _mm_storeu_si128((__m128i*)(W1 + j), X);
    }

#ifdef _DEBUG
    printf("Expanding message W0-67:\n");
    for (i = 0; i < 68; i++){
        printf("%08x ", W1[i]);
        if (((i + 1) % 8) == 0) printf("\n");
    }
    printf("\n");
#endif
    /* W2 = W1[j] ^ W1[j+4] */
    for (int j = 0; j < 64; j += 4){
        X = _mm_loadu_si128((__m128i*)(W1 + j));
        K = _mm_loadu_si128((__m128i*)(W1 + j + 4));
        X = _mm_xor_si128(X, K);
        _mm_storeu_si128((__m128i*)(W2 + j), X);
    }

#ifdef _DEBUG
    printf("Expanding message W'0-63:\n");
    for (i = 0; i < 64; i++){
        printf("%08x ", W2[i]);
        if (((i + 1) % 8) == 0) printf("\n");
    }
    printf("\n");
#endif

    //����ѹ��
    A = ctx->state[0];
    B = ctx->state[1];
    C = ctx->state[2];
    D = ctx->state[3];
    E = ctx->state[4];
    F = ctx->state[5];
    G = ctx->state[6];
    H = ctx->state[7];
#ifdef _DEBUG
    printf("j     A       B        C         D         E        F        G       H\n");
    printf("   %08x %08x %08x %08x %08x %08x %08x %08x\n", A, B, C, D, E, F, G, H);
#endif

    //ѹ������
    for (j = 0; j < 16; j++){
        SS1 = ROTL((ROTL(A, 12) + E + ROTL(T[j], j)), 7);
        SS2 = SS1 ^ ROTL(A, 12);
        TT1 = FF0(A, B, C) + D + SS2 + W2[j];
        TT2 = GG0(E, F, G) + H + SS1 + W1[j];
        D = C;
        C = ROTL(B, 9);
        B = A;
        A = TT1;
        H = G;
        G = ROTL(F, 19);
        F = E;
        E = P0(TT2);
#ifdef _DEBUG
        printf("%02d %08x %08x %08x %08x %08x %08x %08x %08x\n", j, A, B, C, D, E, F, G, H);
#endif
    }

    for (j = 16; j < 64; j++){
        SS1 = ROTL((ROTL(A, 12) + E + ROTL(T[j], j)), 7);
        SS2 = SS1 ^ ROTL(A, 12);
        TT1 = FF1(A, B, C) + D + SS2 + W2[j];
        TT2 = GG1(E, F, G) + H + SS1 + W1[j];
        D = C;
        C = ROTL(B, 9);
        B = A;
        A = TT1;
        H = G;
        G = ROTL(F, 19);
        F = E;
        E = P0(TT2);
#ifdef _DEBUG
        printf("%02d %08x %08x %08x %08x %08x %08x %08x %08x\n", j, A, B, C, D, E, F, G, H);
#endif
    }

    ctx->state[0] ^= A;
    ctx->state[1] ^= B;
    ctx->state[2] ^= C;
    ctx->state[3] ^= D;
    ctx->state[4] ^= E;
    ctx->state[5] ^= F;
    ctx->state[6] ^= G;
    ctx->state[7] ^= H;
#ifdef _DEBUG
    printf("   %08x %08x %08x %08x %08x %08x %08x %08x\n", ctx->state[0], ctx->state[1], ctx->state[2],
        ctx->state[3], ctx->state[4], ctx->state[5], ctx->state[6], ctx->state[7]);
#endif
}


/*
 * SM3 process buffer
 */
void sm3_update(sm3_context* ctx, unsigned char* input, int iplen_t){
    /*
     * iplen_t �����data�ĳ���
     * input ������data
     */
    int fill;
    unsigned long left;

    if (iplen_t <= 0)
        return;

    left = ctx->iplen & 0x3F;       // ��ǰ���鿪ʼ����λ��
    fill = 64 - left;               // ��Ҫ����λ��

    ctx->iplen += iplen_t;             // �Ѿ��������зֿ��е��ܳ���

    if (left && iplen_t >= fill){
        memcpy((void*)(ctx->buffer + left),(void*)input, fill);     // ��ȫ��ǰ�ֿ鵽64�ֽ�
        sm3_process(ctx, ctx->buffer);                              // ����ȫ�ķֿ���в���
        input += fill;                                              // ������һ���ֿ�ĵ�һ������λ�ø���
        iplen_t -= fill;                                               // ������һ���ֿ��data�ĳ���
        left = 0;                                                   // ��ʼһ���µķ���
    }

    while (iplen_t >= 64){     
        sm3_process(ctx, input);
        input += 64;
        iplen_t -= 64;
    }// �������data���Ȳ�С��64bit��64bit����һ���鲢���в�����ֱ�������data����С��64bit

    if (iplen_t > 0){
        memcpy((void*)(ctx->buffer + left),(void*)input, iplen_t);
    }// ����������Ϣ����buffer��
}

static const unsigned char sm3_padding[64] =
{
        0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/*
 * SM3 final digest
 */
void sm3_finish(sm3_context* ctx, unsigned char output[32]){
    unsigned long last, padn;
    unsigned long high, low;
    unsigned char msglen[8];        // ��Ϣ����(��˴洢)

    high = (ctx->iplen >> 29);
    low = (ctx->iplen << 3);

    PUT_ULONG_BE(high, msglen, 0);
    PUT_ULONG_BE(low, msglen, 4);
    /*
     *     msglen(initial):|       |        |        |        |        |        |        |        |
     *                     |    29* 0bit   |iplen(30bit-32bit)|      iplen(1bit-29bit)       |3bit|
     *                     |               high               |                low                |
     */

    last = ctx->iplen & 0x3F;     // ÿ64�ֽ�һ�����飬��Ϊ���һ�������еĵ��ֽ���(& 0x3F = % 64)
    padn = (last < 56) ? (56 - last) : (120 - last);            // ��Ҫ��������ݵĳ���

    sm3_update(ctx, (unsigned char*)sm3_padding, padn);         // ��padding�������
    sm3_update(ctx, msglen, 8);                                 // ������8�ֽڵ���Ϣ����

    PUT_ULONG_BE(ctx->state[0], output, 0);
    PUT_ULONG_BE(ctx->state[1], output, 4);
    PUT_ULONG_BE(ctx->state[2], output, 8);
    PUT_ULONG_BE(ctx->state[3], output, 12);
    PUT_ULONG_BE(ctx->state[4], output, 16);
    PUT_ULONG_BE(ctx->state[5], output, 20);
    PUT_ULONG_BE(ctx->state[6], output, 24);
    PUT_ULONG_BE(ctx->state[7], output, 28);
}

void sm3(unsigned char* input, int iplen_t, unsigned char output[32]){
    sm3_context ctx;

    sm3_starts(&ctx);
    sm3_update(&ctx, input, iplen_t);
    sm3_finish(&ctx, output);

    memset(&ctx, 0, sizeof(sm3_context));
}
