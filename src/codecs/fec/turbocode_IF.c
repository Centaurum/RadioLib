/**
* @file t_code_interface.c
* @brief interface for turbocode usage
* @author Centaurum
* @version v0.1
* @date 2015-06-01
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/
#include "turbocode_IF.h"

#define N 8
#define TBD -1        //trellis termination bits (inserted by encoder #1)

int fec_encode_sym(SourceSymbol src, EncodedSymbol *dst)
{
    int X[N] = {0,0,0,0,0,0,TBD,TBD};

	int    P1[N];     //encoder #1 parity bits
	int    P2[N];     //encoder #2 parity bits
	//double x[N];      //databit mapped to symbol
	//double p1[N];     //encoder #1 parity bit mapped to symbol
	//double p2[N];     //encoder #2 parity bit mapped to symbol
	//double x_d[N];    //x_dash  = noisy data symbol
	//double p1_d[N];   //p1_dash = noisy parity#1 symbol
	//double p2_d[N];   //p2_dash = noisy parity#2 symbol
	//double L_h[N];    //L_hat = likelihood of databit given entire observation
	//int    X_h[N];    //X_hat = sliced MAP decisions
	int    k;         //databit index (trellis stage)

	for(k = 0; k < SOURCE_SYM_BITSIZE; k++) X[k] = src[k];
	//printf("\nDebugE: "); for(k = 0; k < SOURCE_SYM_BITSIZE; k++)printf("%d ", X[k]); printf("\n");
    gen_tab();   //generate trellis tables

    turbo_encode(X, P1, P2);

	for(k = 0; k < N; k++) (*dst)[k] = X[k], (*dst)[k+N] = P1[k], (*dst)[k+2*N] = P2[k];

	return 0;
}

int fec_decode_sym(EncodedSymbol src, SourceSymbol *dst)
{
    int X[N] = {1,1,0,0,1,0,TBD,TBD};

    //noise standard deviation
	double sigma = 1.0;

	int    P1[N];     //encoder #1 parity bits
	int    P2[N];     //encoder #2 parity bits
	double x_d[N];    //x_dash  = noisy data symbol
	double p1_d[N];   //p1_dash = noisy parity#1 symbol
	double p2_d[N];   //p2_dash = noisy parity#2 symbol
	double L_h[N];    //L_hat = likelihood of databit given entire observation
	int    X_h[N];    //X_hat = sliced MAP decisions
	int    k;         //databit index (trellis stage)

    gen_tab();   //generate trellis tables
	for(k = 0; k < N; k++)X[k] = src[k], P1[k] = src[k+N], P2[k] = src[k+2*N];

	for(k = 0; k < N; k++) //for entire block length
	{
		x_d[k]  = X[k]  ? +1 : -1;  //map databit to symbol
		p1_d[k] = P1[k] ? +1 : -1;  //map parity #1 to symbol
		p2_d[k] = P2[k] ? +1 : -1;  //map parity #2 to symbol
	}

	turbo_decode(sigma, x_d, p1_d, p2_d, L_h, X_h);

	for(k = 0; k < SOURCE_SYM_BITSIZE; k++)(*dst)[k] = X_h[k];
	//printf("\nDebugD: "); for(k = 0; k < SOURCE_SYM_BITSIZE; k++)printf("%d ", X_h[k]); printf("\n");

	return 0;
}

int fec_process_buffer(BYTE *src, int src_bytelen, BYTE *dst, int dst_bytelen, int (*transformation)(void*,void*), char is_encode)
{
	int src_sym_bitsize = is_encode?SOURCE_SYM_BITSIZE:ENCODED_SYM_BITSIZE;
	int dst_sym_bitsize = is_encode?ENCODED_SYM_BITSIZE:SOURCE_SYM_BITSIZE;
	int src_aligned_bitsize = aligned_size(src_bytelen*8, src_sym_bitsize);
	int src_reduced_bitsize = ((src_bytelen*8)/src_sym_bitsize)*src_sym_bitsize;
	int i, k, dst_cnt = 0;
	BYTE X[max(SOURCE_SYM_BITSIZE,ENCODED_SYM_BITSIZE)];
	BYTE Y[max(SOURCE_SYM_BITSIZE,ENCODED_SYM_BITSIZE)];

	//if(src_aligned_bitsize*dst_sym_bitsize > dst_bytelen*8*src_sym_bitsize) //is it long enough destination buffer?
	//{
	//	return -1; //destination is too small
	//}
	if(is_encode)
	{
		if(dst_bytelen < eval_encoded_buf_by_input(src_bytelen)) return -1;
	}
	else
	{
		if(dst_bytelen < eval_decoded_buf_by_input(src_bytelen)) return -1;
	}
	
	for(i = 0; i < src_reduced_bitsize; i+=src_sym_bitsize)
	{
		for(k = 0; k < src_sym_bitsize; k++)
		{
			int offset = i+k;
			X[k] = (src[offset/8]>>(7-(offset&7)))&1;
		}
		if(transformation(X, &Y))
		{
			return -1; // encoding error
		}
		for(k = 0; k < dst_sym_bitsize; k++)
		{
			int offset = k+dst_cnt;
			dst[offset/8] |= (Y[k]<<(7-(offset&7)));
		}
		dst_cnt += dst_sym_bitsize;
	}
	if(is_encode)
	{
		for(k = 0; i < src_bytelen*8; k++, i++) X[k] = (src[i/8]>>(7-(i&7)))&1;
		for(     ; i < src_aligned_bitsize; k++, i++) X[k] = 0;
		if(transformation(X, &Y))
		{
			return -1; // encoding error
		}
		for(k = 0; k < dst_sym_bitsize && src_aligned_bitsize != src_bytelen*8; k++)
		{
			int offset = k+dst_cnt;
			dst[offset/8] |= (Y[k]<<(7-(offset&7)));
		}
		dst_cnt += dst_sym_bitsize;
	}
	return aligned_size(dst_cnt, 8)/8;
}

int fec_encode_buffer(BYTE *src, int src_bytelen, BYTE *dst, int dst_bytelen)
{
	return fec_process_buffer(src, src_bytelen, dst, dst_bytelen, (int(*)(void*, void*))fec_encode_sym, 1);
}

int fec_decode_buffer(BYTE *src, int src_bytelen, BYTE *dst, int dst_bytelen)
{
	return fec_process_buffer(src, src_bytelen, dst, dst_bytelen, (int(*)(void*, void*))fec_decode_sym, 0);
}
