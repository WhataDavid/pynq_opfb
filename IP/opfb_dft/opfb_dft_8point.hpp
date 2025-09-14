#pragma once
#include <ap_int.h>
#include <hls_stream.h>
#include <stdint.h>
#include <ap_axi_sdata.h>
#include <string.h>
#include <iostream>
//#include <cmath>
#include <hls_math.h> // 推荐 HLS 优化过的数学库


#define M 8

typedef ap_axiu<256,0,0,0> axis256_t;
typedef ap_axiu<512,0,0,0> axis512_t;


void opfb_dft_8point_real(
    hls::stream<axis256_t> &input,
    hls::stream<axis512_t> &output
);
