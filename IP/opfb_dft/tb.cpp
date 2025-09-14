#include "opfb_dft_8point.hpp"
#include "circular_shift.hpp"
#include <cstdio>
#include <iostream>
#include "float_to_uint32.hpp"

// axis512: 前8 float为实部，后8 float为虚部
void print_axis512(const axis512_t &out, int idx) {
    std::printf("DFT Output %d:", idx);
    for (int i = 0; i < M; ++i) {
        ap_uint<32> bits = out.data.range(32*(i+1)-1, 32*i);
        float val = *reinterpret_cast<const float*>(&bits);
        std::printf(" %f", val);
    }
    std::printf(" | imag:");
    for (int i = 0; i < M; ++i) {
        ap_uint<32> bits = out.data.range(32*(i+9)-1, 32*(i+8));
        float val = *reinterpret_cast<const float*>(&bits);
        std::printf(" %f", val);
    }
    if (out.last) std::printf(" <== last");
    std::printf("\n");
}


void print_axis256(const axis256_t &out, int idx) {
    std::printf("DFT Output %d:", idx);
    for (int i = 0; i < M; ++i) {
        ap_uint<32> bits = out.data.range(32*(i+1)-1, 32*i);
        float val = *reinterpret_cast<const float*>(&bits);
        std::printf(" %f", val);
    }
//    std::printf(" | imag:");
//    for (int i = 0; i < M; ++i) {
//        ap_uint<32> bits = out.data.range(32*(i+9)-1, 32*(i+8));
//        float val = *reinterpret_cast<const float*>(&bits);
//        std::printf(" %f", val);
//    }
    if (out.last) std::printf(" <== last");
    std::printf("\n");
}

int main() {
    hls::stream<axis256_t> in, mid;
    hls::stream<axis512_t> out;
    const int n_frames = 141;

    for (int f = 0; f < n_frames; ++f) {
        axis256_t pkt;
        pkt.data = 0;
        for (int i = 0; i < M; ++i) {
            float v = f * 8 + i;
            ap_uint<32> u;
            memcpy(&u, &v, sizeof(u));
            pkt.data.range(32*(i+1)-1, 32*i) = u;
        }
        pkt.last = (f == n_frames - 1);
        in.write(pkt);
    }

    opfb_circular_shift(in, mid);
    opfb_dft_8point_real(mid, out);

    int cnt = 0;
    while (!out.empty()) {
        auto pkt = out.read();
        print_axis512(pkt, cnt++);
        if (pkt.last) break;
    }
    return 0;
}
