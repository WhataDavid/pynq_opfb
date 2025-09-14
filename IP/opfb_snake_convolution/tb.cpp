//���������Ծ����Ľ����������ȷ
//#include <iostream>
//#include <ap_int.h>
//#include <ap_axi_sdata.h>
//#include <hls_stream.h>
//#include "opfb.hpp"
//
//void print_axis128(const axis128_t& out, int idx) {
//    std::cout << "Output " << idx << ": ";
//    for (int row = 0; row < M; ++row) {
//        ap_int<16> val = out.data.range(16 * (row + 1) - 1, 16 * row);
//        std::cout << val << " ";
//    }
//    if (out.last) std::cout << "<== last";
//    std::cout << std::endl;
//}
//
//int main() {
//    hls::stream<axis96_t> I_stream;
//    hls::stream<axis128_t> output_stream;
//
//    const int n_frames = length;
//
//    ap_int<16> in_val = 0;
//    for (int i = 0; i < n_frames*2; ++i) {  // д��֡���ݣ���������last
//        axis96_t I_tmp;
//        I_tmp.data = 0;
////        Q_tmp.data = 0;
//        for (int k = 0; k < 6; ++k) {
//            I_tmp.data.range(16*(k+1)-1, 16*k) = in_val;
////            Q_tmp.data.range(16*(k+1)-1, 16*k) = in_val;
//            ++in_val;
//        }
//        I_tmp.last = ((i+1) % n_frames == 0) ? 1 : 0;
////        Q_tmp.last = ((i+1) % n_frames == 0) ? 1 : 0;
//        I_stream.write(I_tmp);
////        Q_stream.write(Q_tmp);
//    }
//
//    // ����DUT
//    opfb_matix_multiple(I_stream, output_stream);
//
//    // ��ȡ�����ֱ�������ڶ���last=1
//    int out_cnt = 0;
//    int last_cnt = 0;
//    while (!output_stream.empty()) {
//        axis128_t out = output_stream.read();
//        print_axis128(out, out_cnt);
//        ++out_cnt;
//        if (out.last) {
//            last_cnt++;
//            if (last_cnt == 2) break;
//        }
//    }
//
//    return 0;
//}

////����������ֱ�����adc����  ��ȷ 2025-7-3
//#include <iostream>
//#include <ap_int.h>
//#include <ap_axi_sdata.h>
//#include <hls_stream.h>
//#include "opfb.hpp"
//
//void print_axis128(const axis128_t& out, int idx) {
//    std::cout << "Output " << idx << ": ";
//    for (int row = 0; row < M; ++row) {
//        ap_int<16> val = out.data.range(16 * (row + 1) - 1, 16 * row);
//        std::cout << val << " ";
//    }
//    if (out.last) std::cout << "<== last";
//    std::cout << std::endl;
//}
//
//int main() {
//    hls::stream<axis96_t> I_stream;
//    hls::stream<axis128_t> output_stream;
//
//    const int n_frames = length;
//
//    ap_int<16> in_val = 0;
//    for (int i = 0; i < n_frames*2; ++i) {  // д��֡���ݣ���������last
//        axis96_t I_tmp;
//        I_tmp.data = 0;
//        for (int k = 0; k < 6; ++k) {
//            I_tmp.data.range(16*(k+1)-1, 16*k) = in_val;
//            ++in_val;
//        }
//        I_tmp.last = ((i+1) % n_frames == 0) ? 1 : 0;
//        I_stream.write(I_tmp);
//    }
//
//    // ����DUT
//    opfb_matix_multiple(I_stream, output_stream);
//
//    // ��ȡ�����ֱ�������ڶ���last=1
//    int out_cnt = 0;
//    int last_cnt = 0;
//    while (!output_stream.empty()) {
//        axis128_t out = output_stream.read();
//        print_axis128(out, out_cnt);
//        ++out_cnt;
//        if (out.last) {
//            last_cnt++;
//            if (last_cnt == 2) break;
//        }
//    }
//
//    return 0;
//}


////����������256bitλ�����������ʵ�����鲿�������ȷ���Ѿ���16bit����תΪ12bit adc��Чλ��25.7.10
//#include <iostream>
//#include <ap_int.h>
//#include <ap_axi_sdata.h>
//#include <hls_stream.h>
//#include "opfb.hpp"
//
//void print_axis256(const axis256_t& out, int idx) {
//    std::cout << "Output " << idx << ": ";
//    for (int row = 0; row < M; ++row) {
//        ap_int<16> val_real = out.data.range(16 * (row*2 + 1) - 1, 16 * (row*2));
//        std::cout << val_real << " ";
//        ap_int<16> val_imag = out.data.range(16 * (row*2 + 2) - 1, 16 * (row*2+1));
//        std::cout << val_imag << " ";
//    }
//    if (out.last) std::cout << "<== last";
//    std::cout << std::endl;
//}
//
//int main() {
//    hls::stream<axis96_t> I_stream;
//    hls::stream<axis256_t> output_stream;
//
//    const int n_frames = length;
//
//    ap_int<16> in_val = 0;
//    for (int i = 0; i < n_frames*2; ++i) {  // д��֡���ݣ���������last
//        axis96_t I_tmp;
//        I_tmp.data = 0;
//        for (int k = 0; k < 6; ++k) {
//            I_tmp.data.range(16*(k+1)-1, 16*k) = (in_val<<4);
//            ++in_val;
//        }
//        I_tmp.last = ((i+1) % n_frames == 0) ? 1 : 0;
//        I_stream.write(I_tmp);
//    }
//
//    // ����DUT
//    opfb_matix_multiple(I_stream, output_stream);
//
//    // ��ȡ�����ֱ�������ڶ���last=1
//    int out_cnt = 0;
//    int last_cnt = 0;
//    while (!output_stream.empty()) {
//        axis256_t out = output_stream.read();
//        print_axis256(out, out_cnt);
//        ++out_cnt;
//        if (out.last) {
//            last_cnt++;
//            if (last_cnt == 2) break;
//        }
//    }
//
//    return 0;
//}

//�޸�intΪfloat����ֹ�������߾���,���λ������
//
////#include <ap_int.h>
////#include <ap_axi_sdata.h>
////#include <hls_stream.h>
//#include "opfb.hpp"
//
////void print_axis512(const axis512_t& out, int idx) {
////    std::cout << "Output " << idx << ": ";
////    for (int row = 0; row < M; ++row) {
////        float val_real = out.data.range(32 * (row*2 + 1) - 1, 32 * (row*2));
//////        std::cout << val_real << " ";
////        std::printf("%f ",val_real);
////        float val_imag = out.data.range(32 * (row*2 + 2) - 1, 32 * (row*2+1));
//////        std::cout << val_imag << " ";
////        std::printf("%f ",val_imag);
////    }
////    if (out.last) std::cout << "<== last";
////    std::cout << std::endl;
////}
//
//
//void print_axis512(const axis512_t& out, int idx) {
//    std::cout << "Output " << idx << ": ";
//    for (int row = 0; row < M; ++row) {
//        // ��������λ���½���Ϊ float
//        ap_uint<32> real_bits = out.data.range(32 * (row*2 + 1) - 1, 32 * (row*2));
//        float val_real = *reinterpret_cast<const float*>(&real_bits);
//        std::printf("%f ", val_real);
//        std::printf("0.000000 "); // �鲿�̶�Ϊ0
//    }
//    std::cout << std::endl;
//}
//
//int main() {
//    hls::stream<axis96_t> I_stream;
//    hls::stream<axis512_t> output_stream;
//
//    const int n_frames = length;
//
//    ap_int<16> in_val = 0;
//    for (int i = 0; i < n_frames; ++i) {  // д��֡���ݣ���������last
//        axis96_t I_tmp;
//        I_tmp.data = 0;
//        for (int k = 0; k < 6; ++k) {
////        	std::cout<<i%2<<" ";
////            I_tmp.data.range(16*(k+1)-1, 16*k) = (i%2<<4);
//        	  std::cout<<in_val<<" ";
//        	  I_tmp.data.range(16*(k+1)-1, 16*k) = (in_val<<4);
//            ++in_val;
//        }
//        I_tmp.last = ((i+1) % n_frames == 0) ? 1 : 0;
//        I_stream.write(I_tmp);
//    }
//
//    // ����DUT
//    opfb_matix_multiple(I_stream, output_stream);
//
//    // ��ȡ�����ֱ�������ڶ���last=1
//    int out_cnt = 0;
//    int last_cnt = 0;
//    while (!output_stream.empty()) {
//        axis512_t out = output_stream.read();
//        print_axis512(out, out_cnt);
//        ++out_cnt;
//        if (out.last) {
//            last_cnt++;
//            if (last_cnt == 2) break;
//        }
//    }
//
//    return 0;
//}

#include "opfb_snake_convolution.hpp"
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
//    hls::stream<axis96_t> I_stream;
    hls::stream<axis_t> I_stream;
    hls::stream<axis256_t> out_stream;
    ap_int<16> in_val = 0;

    for (int i = 0; i < length; ++i) {
//        axis96_t pkt;
        axis_t pkt;
        pkt.data = 0;
        for (int k = 0; k < D; ++k) {
            pkt.data.range(16*(k+1)-1, 16*k) = -1*(in_val << 4);
            ++in_val;
        }
        pkt.last = (i == length - 1);
        I_stream.write(pkt);
    }

    opfb_snake_convolution(I_stream, out_stream);

    int idx = 0;
    int last_cnt = 0;
    while (!out_stream.empty()) {
        axis256_t pkt = out_stream.read();
        print_axis256(pkt, idx++);
        if (pkt.last && ++last_cnt == 1) break;
    }

    return 0;
}
