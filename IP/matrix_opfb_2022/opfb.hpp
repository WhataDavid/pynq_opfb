#pragma once
#include <ap_int.h>
#include <hls_stream.h>
#include <stdint.h>
#include <ap_axi_sdata.h>

#define M 8
#define D 6
#define TAPS 4
#define length 16

typedef ap_int<16> sample_16;
typedef ap_int<256> sample_256;
typedef ap_int<112> sample_112;

typedef ap_axis<128,0,0,0> axis128_t;
typedef ap_axis<1024,0,0,0> axis1024_t;
typedef ap_axis<96,0,0,0> axis96_t;
typedef ap_axis<10,0,0,0> axis10_t;   // 新增，10位宽AXIS口


void opfb_matix_multiple(
    hls::stream<axis96_t> &I,
//    hls::stream<axis96_t> &Q,
    hls::stream<axis128_t> &output
//	ap_int<10> *i_out
) ;
