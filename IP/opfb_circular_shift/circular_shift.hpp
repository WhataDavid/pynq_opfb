#pragma once
#include <ap_int.h>
#include <hls_stream.h>
#include <stdint.h>
#include <ap_axi_sdata.h>
#include <string.h>
#include <iostream>
//#include "float_to_uint32.hpp"
//#include "circular_shift.cpp"

#define M 8
#define D 7
#define Distance M-D
#define length TAPS
#define DATA_NUM M*TAPS
#define TAPS 16

typedef ap_int<16> sample_16;
typedef ap_int<256> sample_256;
typedef ap_int<112> sample_112;

typedef ap_axis<128,0,0,0> axis128_t;
typedef ap_axiu<256,0,0,0> axis256_t;
typedef ap_axiu<512,0,0,0> axis512_t;
typedef ap_axis<1024,0,0,0> axis1024_t;
typedef ap_axis<96,0,0,0> axis96_t;
typedef ap_axis<10,0,0,0> axis10_t;

//union float_to_uint32 {
//    float f;
//    ap_uint<32> u;
//};

void opfb_circular_shift(
    hls::stream<axis256_t> &input,
    hls::stream<axis256_t> &output
) ;


