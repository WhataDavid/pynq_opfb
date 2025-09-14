//#include "opfb_dft_8point.hpp"
//
//// 8点DFT: in为实数，out_real/out_imag为复数DFT输出
//void dft8(const float in[M], float out_real[M], float out_imag[M]) {
//#pragma HLS INLINE
//    const float PI = 3.141592f;
//    for (int k = 0; k < M; ++k) {
//#pragma HLS UNROLL
//        float sum_real = 0;
//        float sum_imag = 0;
//        for (int n = 0; n < M; ++n) {
//#pragma HLS UNROLL
//            float angle = -2.0f * PI * k * n / M;
//            float c = cosf(angle);
//            float s = sinf(angle);
//            sum_real += in[n] * c;
//            sum_imag += in[n] * s;
//        }
//        out_real[k] = sum_real;
//        out_imag[k] = sum_imag;
//    }
//}
//
//void opfb_dft_8point(hls::stream<axis256_t> &input,
//                     hls::stream<axis512_t> &output) {
//#pragma HLS INTERFACE ap_ctrl_none port=return
//#pragma HLS INTERFACE axis port=input
//#pragma HLS INTERFACE axis port=output
//#pragma HLS PIPELINE II=1
//
//    bool done = false;
//    while (!done) {
//#pragma HLS PIPELINE II=1
//#ifndef __SYNTHESIS__
//        if (input.empty()) break;
//#endif
//        axis256_t in_pkt = input.read();
//
//        // 解包
//        float in[M];
//#pragma HLS ARRAY_PARTITION variable=in complete dim=0
//        for (int i = 0; i < M; ++i) {
//#pragma HLS UNROLL
//            ap_uint<32> bits = in_pkt.data.range(32*(i+1)-1, 32*i);
////            static float_to_uint32 u;
////            u.u = bits;
////            in[i] = u.f;
//
//            float float_val;
////            *reinterpret_cast<ap_uint<32>*>(&float_val) = bits;
//            memcpy(&float_val, &bits, sizeof(float_val));
//            in[i] = float_val;
//        }
//
//        // 计算DFT
//        float out_real[M], out_imag[M];
//#pragma HLS ARRAY_PARTITION variable=out_real complete dim=0
//#pragma HLS ARRAY_PARTITION variable=out_imag complete dim=0
//        dft8(in, out_real, out_imag);
//
//        // 打包为axis512
//        axis512_t out_pkt;
//        out_pkt.data = 6666;
//        for (int i = 0; i < M; ++i) {
////#pragma HLS UNROLL
////        	static  float_to_uint32 u_re, u_im;
////            u_re.f = out_real[i];
////            u_im.f = out_imag[i];
//        	unsigned int u32_real[M],u32_imag[M];
////        	memcpy(&u32_real, &out_real[i], sizeof(u32_real));
////        	memcpy(&u32_imag, &out_imag[i], sizeof(u32_imag));
////            out_pkt.data.range(32*(i+1)-1, 32*i) = u32_real;
////            out_pkt.data.range(32*(i+M+1)-1, 32*(i+M)) = u32_imag;
//        	out_pkt.data.range(32*(i+1)-1, 32*i) =*reinterpret_cast<unsigned int*>(&out_real[i]);
////        	out_pkt.data.range(32*(i+1)-1, 32*i) = u32_real[i];
//            out_pkt.data.range(32*(i+M+1)-1, 32*(i+M)) =*reinterpret_cast<unsigned int*>(&out_imag[i]);
////            out_pkt.data.range(32*(i+M+1)-1, 32*(i+M)) =u32_imag[i];
//
//        }
//
//        out_pkt.last = in_pkt.last;
//
//        output.write(out_pkt);
//
//        if (in_pkt.last) done = true;
//    }
//}

//只输出实数dft,已验证正确
//#include "opfb_dft_8point.hpp"
//
//// 8点DFT: in为实数，out_real/out_imag为复数DFT输出
//void dft8(const float in[M], float out_real[M],float out_imag[M]) {
//#pragma HLS INLINE
//    const float PI = 3.141592f;
//    for (int k = 0; k < M; ++k) {
//#pragma HLS UNROLL
//        float sum_real = 0;
//        float sum_imag = 0;
//        for (int n = 0; n < M; ++n) {
//#pragma HLS UNROLL
//            float angle = -2.0f * PI * k * n / M;
//            float c = cosf(angle);
//            float s = sinf(angle);
//            sum_real += in[n] * c;
//            sum_imag += in[n] * s;
//        }
//        out_real[k] = sum_real;
//        out_imag[k] = sum_imag;
//    }
//}
//
//void opfb_dft_8point_real(hls::stream<axis256_t> &input,
//                     hls::stream<axis256_t> &output) {
//#pragma HLS INTERFACE ap_ctrl_none port=return
//#pragma HLS INTERFACE axis port=input
//#pragma HLS INTERFACE axis port=output
//#pragma HLS PIPELINE II=1
//
//    bool done = false;
//    while (!done) {
//#pragma HLS PIPELINE II=1
//#ifndef __SYNTHESIS__
//        if (input.empty()) break;
//#endif
//        axis256_t in_pkt = input.read();
//
//        // 解包
//        float in[M];
//#pragma HLS ARRAY_PARTITION variable=in complete dim=0
//        for (int i = 0; i < M; ++i) {
//#pragma HLS UNROLL
//            ap_uint<32> bits = in_pkt.data.range(32*(i+1)-1, 32*i);
//            float float_val;
//            memcpy(&float_val, &bits, sizeof(float_val));
//            in[i] = float_val;
//        }
//
//        // 计算DFT
//        float out_real[M];
//		float out_imag[M];
//#pragma HLS ARRAY_PARTITION variable=out_real complete dim=0
//#pragma HLS ARRAY_PARTITION variable=out_imag complete dim=0
//        dft8(in, out_real,out_imag);
//
//        // 打包为axis256
//        axis256_t out_pkt;
//        out_pkt.data = 0;
//        for (int i = 0; i < M; ++i) {
//#pragma HLS UNROLL
////        	static  float_to_uint32 u_re, u_im;
////            u_re.f = out_real[i];
////            u_im.f = out_imag[i];
////        	unsigned int u32_real[M],u32_imag[M];
////        	memcpy(&u32_real, &out_real[i], sizeof(u32_real));
////        	memcpy(&u32_imag, &out_imag[i], sizeof(u32_imag));
////            out_pkt.data.range(32*(i+1)-1, 32*i) = u32_real;
////            out_pkt.data.range(32*(i+M+1)-1, 32*(i+M)) = u32_imag;
//        	out_pkt.data.range(32*(i+1)-1, 32*i) =*reinterpret_cast<unsigned int*>(&out_real[i]);
////        	out_pkt.data.range(32*(i+1)-1, 32*i) = u32_real[i];
////            out_pkt.data.range(32*(i+1)-1, 32*i) =*reinterpret_cast<unsigned int*>(&out_imag[i]);
////            out_pkt.data.range(32*(i+M+1)-1, 32*(i+M)) =u32_imag[i];
//
//        }
//
//        out_pkt.last = in_pkt.last;
//
//        output.write(out_pkt);
//
//        if (in_pkt.last) done = true;
//    }
//}

//只输出dft虚部
//#include "opfb_dft_8point.hpp"
//
//// 8点DFT: in为实数，out_real/out_imag为复数DFT输出
//void dft8(const float in[M], float out_imag[M]) {
//#pragma HLS INLINE
//    const float PI = 3.141592f;
//    for (int k = 0; k < M; ++k) {
//#pragma HLS UNROLL
////        float sum_real = 0;
//        float sum_imag = 0;
//        for (int n = 0; n < M; ++n) {
//#pragma HLS UNROLL
//            float angle = -2.0f * PI * k * n / M;
////            float c = cosf(angle);
//            float s = sinf(angle);
////            sum_real += in[n] * c;
//            sum_imag += in[n] * s;
//        }
////        out_real[k] = sum_real;
//        out_imag[k] = sum_imag;
//    }
//}
//
//void opfb_dft_8point(hls::stream<axis256_t> &input,
//                     hls::stream<axis256_t> &output) {
//#pragma HLS INTERFACE ap_ctrl_none port=return
//#pragma HLS INTERFACE axis port=input
//#pragma HLS INTERFACE axis port=output
//#pragma HLS PIPELINE II=1
//
//    bool done = false;
//    while (!done) {
//#pragma HLS PIPELINE II=1
//#ifndef __SYNTHESIS__
//        if (input.empty()) break;
//#endif
//        axis256_t in_pkt = input.read();
//
//        // 解包
//        float in[M];
//#pragma HLS ARRAY_PARTITION variable=in complete dim=0
//        for (int i = 0; i < M; ++i) {
//#pragma HLS UNROLL
//            ap_uint<32> bits = in_pkt.data.range(32*(i+1)-1, 32*i);
//            float float_val;
//            memcpy(&float_val, &bits, sizeof(float_val));
//            in[i] = float_val;
//        }
//
//        // 计算DFT
////        float out_real[M];
//		float out_imag[M];
////#pragma HLS ARRAY_PARTITION variable=out_real complete dim=0
//#pragma HLS ARRAY_PARTITION variable=out_imag complete dim=0
//        dft8(in, out_imag);
//
//        // 打包为axis256
//        axis256_t out_pkt;
//        out_pkt.data = 0;
//        for (int i = 0; i < M; ++i) {
//#pragma HLS UNROLL
////        	static  float_to_uint32 u_re, u_im;
////            u_re.f = out_real[i];
////            u_im.f = out_imag[i];
////        	unsigned int u32_real[M],u32_imag[M];
////        	memcpy(&u32_real, &out_real[i], sizeof(u32_real));
////        	memcpy(&u32_imag, &out_imag[i], sizeof(u32_imag));
////            out_pkt.data.range(32*(i+1)-1, 32*i) = u32_real;
////            out_pkt.data.range(32*(i+M+1)-1, 32*(i+M)) = u32_imag;
////        	out_pkt.data.range(32*(i+1)-1, 32*i) =*reinterpret_cast<unsigned int*>(&out_real[i]);
////        	out_pkt.data.range(32*(i+1)-1, 32*i) = u32_real[i];
//            out_pkt.data.range(32*(i+1)-1, 32*i) =*reinterpret_cast<unsigned int*>(&out_imag[i]);
////            out_pkt.data.range(32*(i+M+1)-1, 32*(i+M)) =u32_imag[i];
//
//        }
//
//        out_pkt.last = in_pkt.last;
//
//        output.write(out_pkt);
//
//        if (in_pkt.last) done = true;
//    }
//}

#include "opfb_dft_8point.hpp"

// 8点DFT: in为实数，out_real/out_imag为复数DFT输出
void dft8(const float in[M], float out_real[M], float out_imag[M]) {
#pragma HLS INLINE
	const float PI = 3.141592f;
	for (int k = 0; k < M; ++k) {
#pragma HLS UNROLL
		float sum_real = 0;
		float sum_imag = 0;
		for (int n = 0; n < M; ++n) {
#pragma HLS UNROLL
			float angle = -2.0f * PI * k * n / M;
			float c = cosf(angle);
			float s = sinf(angle);
			sum_real += in[n] * c;
			sum_imag += in[n] * s;
		}
		out_real[k] = sum_real;
		out_imag[k] = sum_imag;
	}
}

void opfb_dft_8point_real(hls::stream<axis256_t> &input,
		hls::stream<axis512_t> &output) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=input
#pragma HLS INTERFACE axis port=output
#pragma HLS PIPELINE II=1

	static ap_uint<9> beats_in_frame = 0;  // 0..140 共 141 拍
	static const ap_uint<64> KEEP_FULL = ~((ap_uint<64>)0);    // 64B 全有效
	static const ap_uint<64> KEEP_PARTIAL_40B = ((ap_uint<64>)1 << 40) - 1; // 40B 有效


	while (1) {
#pragma HLS PIPELINE II=1
#ifndef __SYNTHESIS__
		if (input.empty())
			break;
#endif
		axis256_t in_pkt = input.read();

		// 解包
		float in[M];
#pragma HLS ARRAY_PARTITION variable=in complete dim=0
		for (int i = 0; i < M; ++i) {
#pragma HLS UNROLL
			ap_uint<32> bits = in_pkt.data.range(32 * (i + 1) - 1, 32 * i);
			float float_val;
			memcpy(&float_val, &bits, sizeof(float_val));
//			in[i] = float_val;
			in[i] = i;
		}
		std::cout << "all in:\n";
		for (int i = 0; i < M; i++)
			std::cout << in[i]<<" ";
		std::cout << "\n end in \n";

		// 计算DFT
		float out_real[M];
		float out_imag[M];
#pragma HLS ARRAY_PARTITION variable=out_real complete dim=0
#pragma HLS ARRAY_PARTITION variable=out_imag complete dim=0
		dft8(in, out_real, out_imag);

		// 打包为axis512
		axis512_t out_pkt;
		out_pkt.data = 0;
		for (int i = 0; i < M; ++i) {
#pragma HLS UNROLL

			union {
				float f;
				unsigned int u;
			} converse_real;
			converse_real.f = out_real[i];
			out_pkt.data.range(32 * (i + 1) - 1, 32 * i) = converse_real.u;
//			out_pkt.data.range(32 * (i + 1) - 1, 32 * i) = i%M;

			union {
				float f;
				unsigned int u;
			} converse_imag;
			converse_imag.f = out_imag[i];
			out_pkt.data.range(32 * (i + M + 1) - 1, 32 * (i + M)) = converse_imag.u;
//			out_pkt.data.range(32 * (i + M + 1) - 1, 32 * (i + M)) = i%M;
		}

		// 判断是否最后一个拍
		bool is_last_beat = (beats_in_frame == 140);

		// 设置 keep/strb/last
		if (!is_last_beat) {
		    out_pkt.keep = KEEP_FULL;
		    out_pkt.strb = KEEP_FULL;
		    out_pkt.last = 0;
		} else {
		    out_pkt.keep = KEEP_PARTIAL_40B;
		    out_pkt.strb = KEEP_PARTIAL_40B;
//		    out_pkt.last = 1;  // 结束一帧（9000B）
		    out_pkt.last = 0;  // 不结束，在封包ip中控制tlast即可
		}

		// 写出数据
		output.write(out_pkt);

		// 更新计数器
		if (!is_last_beat) {
		    beats_in_frame++;
		    std::cout<<"\nbeats_in_frame:"<<beats_in_frame<<"\n";
		} else {
		    beats_in_frame = 0; // 新的一帧
		}
	}
}
