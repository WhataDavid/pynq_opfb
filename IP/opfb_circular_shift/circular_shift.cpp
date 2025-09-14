#include "circular_shift.hpp"

void opfb_circular_shift(hls::stream<axis256_t> &input,
                         hls::stream<axis256_t> &output) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=input
#pragma HLS INTERFACE axis port=output
#pragma HLS PIPELINE II=1

    static int step = 0;
    bool done = false;

    while (!done) {
#pragma HLS PIPELINE II=1
#ifndef __SYNTHESIS__
        if (input.empty()) break;
#endif
        axis256_t in = input.read();

        // 拆包：从 256-bit 中读取 8 个 float
        float buf[M];
#pragma HLS ARRAY_PARTITION variable=buf complete dim=0
        for (int i = 0; i < M; ++i) {
#pragma HLS UNROLL
            ap_uint<32> bits = in.data.range(32*(i+1)-1, 32*i);
            buf[i] = *reinterpret_cast<float*>(&bits);
//        	unsigned int u = in.data.range(32*(i+1)-1, 32*i).to_uint();
//        	    union { float f; unsigned int u; } conv;
//        	    conv.u = u;
//        	    buf[i] = conv.f;
        }

        // 打印输入
//        std::cout << "input:";
//        for (int i = 0; i < M; ++i) std::cout << " " << buf[i];
//        std::cout << std::endl;

//        step = (step + Distance) % M;
//        use &0x7 == %8, but is fast
        step = (step + Distance) &0x7;

//        test %16
//        step = (step + 1) &0xF;

        // circular shift
        float out_f[M];
#pragma HLS ARRAY_PARTITION variable=out_f complete dim=0
        for (int i = 0; i < M; ++i) {
#pragma HLS UNROLL
//            out_f[(i + step) % M] = buf[i];
            out_f[i] = buf[(i-step+M)%M];
//              out_f[i] = step;
        }

        // 打印输出
//        std::cout << "output:";
//        for (int i = 0; i < M; ++i) std::cout << " " << out_f[i];
//        std::cout << std::endl;

        // 打包输出为 256-bit
        axis256_t out;
        out.data = 0;
        out.last = in.last;
        for (int i = 0; i < M; ++i) {
#pragma HLS UNROLL
            ap_uint<32> u;
            memcpy(&u, &out_f[i], sizeof(u));
            out.data.range(32*(i+1)-1, 32*i) = u;
//        	union { float f; unsigned int u; } conv;
//        	    conv.f = out_f[i];
//        	    out.data.range(32*(i+1)-1, 32*i) = conv.u;
        }

        output.write(out);
        if (in.last) done = true;
    }
}
