#include "circular_shift.hpp"
#include <cstdio>
#include <iostream>

void print_axis256(const axis256_t &out, int idx) {
    std::printf("Output %d:", idx);
    for (int i = 0; i < M; ++i) {
        ap_uint<32> bits = out.data.range(32*(i+1)-1, 32*i);
        float val = *reinterpret_cast<const float*>(&bits);
        std::printf(" %f", val);
    }
    if (out.last) std::printf(" <== last");
    std::printf("\n");
}

int main() {
    hls::stream<axis256_t> in, out;
    const int n_frames = 20;

    // 写入两帧，每帧8个float
    for (int f = 0; f < n_frames; ++f) {
        axis256_t pkt;
        pkt.data = 0;
        for (int i = 0; i < M; ++i) {
            float v = f * 8 + i;  // 测试值
            ap_uint<32> u;
            memcpy(&u, &v, sizeof(u));
            pkt.data.range(32*(i+1)-1, 32*i) = u;
        }
        pkt.last = (f == n_frames - 1);
        in.write(pkt);
    }

    opfb_circular_shift(in, out);

    int cnt = 0;
    while (!out.empty()) {
        auto pkt = out.read();
        print_axis256(pkt, cnt++);
        if (pkt.last) break;
    }
    return 0;
}
