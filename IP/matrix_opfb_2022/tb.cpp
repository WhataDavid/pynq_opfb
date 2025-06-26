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

//����������ֱ�����adc����
#include <iostream>
#include <ap_int.h>
#include <ap_axi_sdata.h>
#include <hls_stream.h>
#include "opfb.hpp"

void print_axis128(const axis128_t& out, int idx) {
    std::cout << "Output " << idx << ": ";
    for (int row = 0; row < M; ++row) {
        ap_int<16> val = out.data.range(16 * (row + 1) - 1, 16 * row);
        std::cout << val << " ";
    }
    if (out.last) std::cout << "<== last";
    std::cout << std::endl;
}

int main() {
    hls::stream<axis96_t> I_stream;
    hls::stream<axis128_t> output_stream;

    const int n_frames = length;

    ap_int<16> in_val = 0;
    for (int i = 0; i < n_frames*2; ++i) {  // д��֡���ݣ���������last
        axis96_t I_tmp;
        I_tmp.data = 0;
        for (int k = 0; k < 6; ++k) {
            I_tmp.data.range(16*(k+1)-1, 16*k) = in_val;
            ++in_val;
        }
        I_tmp.last = ((i+1) % n_frames == 0) ? 1 : 0;
        I_stream.write(I_tmp);
    }

    // ����DUT
    opfb_matix_multiple(I_stream, output_stream);

    // ��ȡ�����ֱ�������ڶ���last=1
    int out_cnt = 0;
    int last_cnt = 0;
    while (!output_stream.empty()) {
        axis128_t out = output_stream.read();
        print_axis128(out, out_cnt);
        ++out_cnt;
        if (out.last) {
            last_cnt++;
            if (last_cnt == 2) break;
        }
    }

    return 0;
}
