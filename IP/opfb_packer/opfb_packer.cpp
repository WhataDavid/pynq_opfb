///////////////////////////////////////////无法更新时间参数
//#include "opfb_packer.h"
//#include <ap_int.h>
//#include <hls_stream.h>
//
//// VDIF参数
//#define VDIF_VERSION           1
//#define VDIF_LOG2_NCHAN        0
//#define VDIF_DATA_FRAME_LENGTH 1024 // 8192/8=1024
//#define VDIF_DATA_TYPE         1
//#define VDIF_BITS_PER_SAMPLE   7
//#define VDIF_THREAD_ID         0
//#define VDIF_STATION_ID        0
//
//#define ETH_HDR_LEN    14
//#define IP_HDR_LEN     20
//#define UDP_HDR_LEN    8
//#define AXI_WORD_BYTES 64
//#define HEADER_LEN     (ETH_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN)
//#define VDIF_HDR_LEN   16
//#define HEAD_TOTAL_LEN (HEADER_LEN + VDIF_HDR_LEN)
//#define DATA_LEN       8192
//#define DATA_WORDS     (DATA_LEN / AXI_WORD_BYTES) // 128
//
//static const ap_uint<8> FULL_HEADER[HEADER_LEN] = {
//    // ETH
//    0x0C,0x42,0xA1,0x16,0x01,0xCC,0x02,0x11,0x22,0x33,0x44,0x55,0x08,0x00,
//    // IP
//    0x45,0x00,0x20,0x3C,0x00,0x00,0x40,0x00,0x40,0x11,0xEE,0xCF,
//    0xC0,0xA8,0xC8,0xC8,0xC0,0xA8,0xC8,0x64,
//    // UDP
//    0x1F,0x90,0x23,0xC4,0x20,0x28,0x00,0x00
//};
//
//typedef ap_axiu<512,0,0,0> axis_pkt;
//
//void opfb_packer(
//    hls::stream<axis_pkt> &in,
//    hls::stream<axis_pkt> &out,
//    ap_uint<30> axi_sec,
//    ap_uint<6>  axi_epoch,
//    bool sma_1pps,
//    ap_uint<30> &sec_now_out,
//    ap_uint<6>  &epoch_now_out,
//    ap_uint<24> &frame_in_sec_out
//) {
//#pragma HLS INTERFACE axis register both port=in
//#pragma HLS INTERFACE axis register both port=out
//
//#pragma HLS INTERFACE s_axilite port=axi_sec bundle=CTRL
//#pragma HLS INTERFACE s_axilite port=axi_epoch bundle=CTRL
//#pragma HLS INTERFACE ap_none port=sma_1pps
//#pragma HLS INTERFACE ap_none port=sec_now_out
//#pragma HLS INTERFACE ap_none port=epoch_now_out
//#pragma HLS INTERFACE ap_none port=frame_in_sec_out
//#pragma HLS INTERFACE ap_ctrl_none port=return
//
//    // 静态变量保存VDIF头时间戳
//    static ap_uint<30> sec_now = 0;
//    static ap_uint<6>  epoch_now = 0;
//    static ap_uint<24> frame_in_sec = 0;
//    static bool        initialized = false;
//    static axis_pkt data_cache;
//    static bool cache_valid = false;
//    static int word_cnt = 0;
//    static bool last_pps = false;
//
//    while (true) {
//#pragma HLS PIPELINE II=1
//
//        // 1PPS同步检测（上升沿检测，假设1PPS只有一个时钟周期宽）
//        bool pps_rising = (sma_1pps && !last_pps);
//        last_pps = sma_1pps;
//
//        // AXI-Lite参数变化时初始化（首次或PS端写入新时间）
//        if (!initialized || axi_sec != sec_now || axi_epoch != epoch_now) {
//            sec_now = axi_sec;
//            epoch_now = axi_epoch;
//            frame_in_sec = 0;
//            initialized = true;
//        }
//
//        // 1PPS到来，递增秒，frame_in_sec归零
//        if (pps_rising) {
//            sec_now++;
//            frame_in_sec = 0;
//        }
//
//        axis_pkt out_pkt;
//
//        if (word_cnt == 0) {
//            // 头拍
//            if (!cache_valid && !in.empty()) {
//                data_cache = in.read();
//                cache_valid = true;
//            }
//            if (!cache_valid) break;
//
//            // VDIF Header
//            ap_uint<32> word0 = (ap_uint<32>(0) << 31) | (ap_uint<32>(0) << 30) | (sec_now & 0x3FFFFFFF);
//            ap_uint<32> word1 = (ap_uint<32>(0) << 30) | (ap_uint<32>(epoch_now) << 24) | (frame_in_sec & 0xFFFFFF);
//            ap_uint<32> word2 = (ap_uint<32>(VDIF_VERSION & 0x7) << 29) |
//                                (ap_uint<32>(VDIF_LOG2_NCHAN & 0x1F) << 24) |
//                                (ap_uint<32>(VDIF_DATA_FRAME_LENGTH) & 0xFFFFFF);
//            ap_uint<32> word3 = (ap_uint<32>(VDIF_DATA_TYPE & 0x1) << 31) |
//                                (ap_uint<32>(VDIF_BITS_PER_SAMPLE & 0x1F) << 26) |
//                                (ap_uint<32>(VDIF_THREAD_ID & 0x3FF) << 16) |
//                                (ap_uint<32>(VDIF_STATION_ID) & 0xFFFF);
//
//            ap_uint<8> vdif_header[16];
//#pragma HLS ARRAY_PARTITION variable=vdif_header complete dim=1
//            for (int i = 0; i < 4; i++)
//                vdif_header[i] = word0.range(i*8+7, i*8);
//            for (int i = 0; i < 4; i++)
//                vdif_header[i+4] = word1.range(i*8+7, i*8);
//            for (int i = 0; i < 4; i++)
//                vdif_header[i+8] = word2.range(i*8+7, i*8);
//            for (int i = 0; i < 4; i++)
//                vdif_header[i+12] = word3.range(i*8+7, i*8);
//
//            ap_uint<512> first_word = 0;
//            for (int i = 0; i < HEADER_LEN; i++)
//#pragma HLS UNROLL
//                first_word.range((i+1)*8-1, i*8) = FULL_HEADER[i];
//            for (int i = 0; i < VDIF_HDR_LEN; i++)
//#pragma HLS UNROLL
//                first_word.range((HEADER_LEN+i+1)*8-1, (HEADER_LEN+i)*8) = vdif_header[i];
//
//            out_pkt.data = first_word;
//            ap_uint<64> tkeep = 0;
//            for (int i = 0; i < HEAD_TOTAL_LEN; i++)
//#pragma HLS UNROLL
//                tkeep[i] = 1;
//            for (int i = HEAD_TOTAL_LEN; i < AXI_WORD_BYTES; i++)
//#pragma HLS UNROLL
//                tkeep[i] = 0;
//            out_pkt.keep = tkeep;
//            out_pkt.last = 0;
//            out.write(out_pkt);
//
//            word_cnt = 1;
//        } else {
//            // 数据包
//            if (!cache_valid && !in.empty()) {
//                data_cache = in.read();
//                cache_valid = true;
//            }
//            if (!cache_valid) break;
//
//            out_pkt = data_cache;
//            cache_valid = false;
//            out_pkt.keep = (ap_uint<64>)(-1);
//            out_pkt.last = (word_cnt == DATA_WORDS) ? 1 : 0;
//            out.write(out_pkt);
//
//            if (word_cnt == DATA_WORDS) {
//                word_cnt = 0;
//                frame_in_sec++;
//            } else {
//                word_cnt++;
//            }
//        }
//
//        // 输出变量
//        sec_now_out      = sec_now;
//        epoch_now_out    = epoch_now;
//        frame_in_sec_out = frame_in_sec;
//    }
//}

///////////////////////////////////////////更改if位置更新时间，但pps在帧尾才更新，不合理
//#include "opfb_packer.h"
//#include <ap_int.h>
//#include <hls_stream.h>
//
//// VDIF参数
//#define VDIF_VERSION           1
//#define VDIF_LOG2_NCHAN        0
//#define VDIF_DATA_FRAME_LENGTH 1024 // 8192/8=1024
//#define VDIF_DATA_TYPE         1
//#define VDIF_BITS_PER_SAMPLE   7
//#define VDIF_THREAD_ID         0
//#define VDIF_STATION_ID        0
//
//#define ETH_HDR_LEN    14
//#define IP_HDR_LEN     20
//#define UDP_HDR_LEN    8
//#define AXI_WORD_BYTES 64
//#define HEADER_LEN     (ETH_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN)
//#define VDIF_HDR_LEN   16
//#define HEAD_TOTAL_LEN (HEADER_LEN + VDIF_HDR_LEN)
//#define DATA_LEN       8192
//#define DATA_WORDS     (DATA_LEN / AXI_WORD_BYTES) // 128
//
//static const ap_uint<8> FULL_HEADER[HEADER_LEN] = {
//    // ETH
//    0x0C,0x42,0xA1,0x16,0x01,0xCC,0x02,0x11,0x22,0x33,0x44,0x55,0x08,0x00,
//    // IP
//    0x45,0x00,0x20,0x3C,0x00,0x00,0x40,0x00,0x40,0x11,0xEE,0xCF,
//    0xC0,0xA8,0xC8,0xC8,0xC0,0xA8,0xC8,0x64,
//    // UDP
//    0x1F,0x90,0x23,0xC4,0x20,0x28,0x00,0x00
//};
//
//void opfb_packer(
//    hls::stream<axis_pkt> &in,
//    hls::stream<axis_pkt> &out,
//    ap_uint<30> axi_sec,
//    ap_uint<6>  axi_epoch,
//    bool sma_1pps,
//    ap_uint<30> &sec_now_out,
//    ap_uint<6>  &epoch_now_out,
//    ap_uint<24> &frame_in_sec_out
//) {
//#pragma HLS INTERFACE axis register both port=in
//#pragma HLS INTERFACE axis register both port=out
//
//#pragma HLS INTERFACE s_axilite port=axi_sec bundle=CTRL
//#pragma HLS INTERFACE s_axilite port=axi_epoch bundle=CTRL
//#pragma HLS INTERFACE ap_none port=sma_1pps
//#pragma HLS INTERFACE ap_none port=sec_now_out
//#pragma HLS INTERFACE ap_none port=epoch_now_out
//#pragma HLS INTERFACE ap_none port=frame_in_sec_out
//#pragma HLS INTERFACE ap_ctrl_none port=return
//
//    // 状态变量
//    static ap_uint<30> sec_now = 0;
//    static ap_uint<6>  epoch_now = 0;
//    static ap_uint<24> frame_in_sec = 0;
//    static bool        initialized = false;
//    static axis_pkt data_cache;
//    static bool cache_valid = false;
//    static int word_cnt = 0;
//    static bool last_pps = false;
//
//    // 1PPS和初始化检测，每拍都做
//    bool pps_rising = (sma_1pps && !last_pps);
//    last_pps = sma_1pps;
//
//    if (!initialized || axi_sec != sec_now || axi_epoch != epoch_now) {
//        sec_now = axi_sec;
//        epoch_now = axi_epoch;
//        frame_in_sec = 0;
//        initialized = true;
//    }
//
//    // 包处理主循环
//    while (initialized) {
//#pragma HLS PIPELINE II=1
//
//        axis_pkt out_pkt;
//
//        if (word_cnt == 0) {
//            // 头拍
//            if (!cache_valid && !in.empty()) {
//                data_cache = in.read();
//                cache_valid = true;
//            }
//            if (!cache_valid) break;
//
//            // VDIF Header
//            ap_uint<32> word0 = (ap_uint<32>(0) << 31) | (ap_uint<32>(0) << 30) | (sec_now & 0x3FFFFFFF);
//            ap_uint<32> word1 = (ap_uint<32>(0) << 30) | (ap_uint<32>(epoch_now) << 24) | (frame_in_sec & 0xFFFFFF);
//            ap_uint<32> word2 = (ap_uint<32>(VDIF_VERSION & 0x7) << 29) |
//                                (ap_uint<32>(VDIF_LOG2_NCHAN & 0x1F) << 24) |
//                                (ap_uint<32>(VDIF_DATA_FRAME_LENGTH) & 0xFFFFFF);
//            ap_uint<32> word3 = (ap_uint<32>(VDIF_DATA_TYPE & 0x1) << 31) |
//                                (ap_uint<32>(VDIF_BITS_PER_SAMPLE & 0x1F) << 26) |
//                                (ap_uint<32>(VDIF_THREAD_ID & 0x3FF) << 16) |
//                                (ap_uint<32>(VDIF_STATION_ID) & 0xFFFF);
//
//            ap_uint<8> vdif_header[16];
//#pragma HLS ARRAY_PARTITION variable=vdif_header complete dim=1
//            for (int i = 0; i < 4; i++)
//                vdif_header[i] = word0.range(i*8+7, i*8);
//            for (int i = 0; i < 4; i++)
//                vdif_header[i+4] = word1.range(i*8+7, i*8);
//            for (int i = 0; i < 4; i++)
//                vdif_header[i+8] = word2.range(i*8+7, i*8);
//            for (int i = 0; i < 4; i++)
//                vdif_header[i+12] = word3.range(i*8+7, i*8);
//
//            ap_uint<512> first_word = 0;
//            for (int i = 0; i < HEADER_LEN; i++)
//#pragma HLS UNROLL
//                first_word.range((i+1)*8-1, i*8) = FULL_HEADER[i];
//            for (int i = 0; i < VDIF_HDR_LEN; i++)
//#pragma HLS UNROLL
//                first_word.range((HEADER_LEN+i+1)*8-1, (HEADER_LEN+i)*8) = vdif_header[i];
//
//            out_pkt.data = first_word;
//            ap_uint<64> tkeep = 0;
//            for (int i = 0; i < HEAD_TOTAL_LEN; i++)
//#pragma HLS UNROLL
//                tkeep[i] = 1;
//            for (int i = HEAD_TOTAL_LEN; i < AXI_WORD_BYTES; i++)
//#pragma HLS UNROLL
//                tkeep[i] = 0;
//            out_pkt.keep = tkeep;
//            out_pkt.last = 0;
//            out.write(out_pkt);
//
//            word_cnt = 1;
//        } else {
//            // 数据包
//            if (!cache_valid && !in.empty()) {
//                data_cache = in.read();
//                cache_valid = true;
//            }
//            if (!cache_valid) break;
//
//            out_pkt = data_cache;
//            cache_valid = false;
//            out_pkt.keep = (ap_uint<64>)(-1);
//            out_pkt.last = (word_cnt == DATA_WORDS) ? 1 : 0;
//            out.write(out_pkt);
//
//            if (word_cnt == DATA_WORDS) {
//                word_cnt = 0;
//                frame_in_sec++;
//
//                // 只在每完整输出一帧后，检测1PPS并更新秒数
//                if (pps_rising) {
//                    sec_now++;
//                    frame_in_sec = 0;
//                }
//            } else {
//                word_cnt++;
//            }
//        }
//
//        // 输出变量
//        sec_now_out      = sec_now;
//        epoch_now_out    = epoch_now;
//        frame_in_sec_out = frame_in_sec;
//    }
//}
/////////////////////////////////////////////////////////////////////////-slack=0.08，PPS方法
//#include "opfb_packer.h"
//#include <ap_int.h>
//#include <hls_stream.h>
//
//// VDIF参数
//#define VDIF_VERSION           1
//#define VDIF_LOG2_NCHAN        3
//#define VDIF_DATA_FRAME_LENGTH 1024 // 8192/8=1024
//#define VDIF_DATA_TYPE         1
//#define VDIF_BITS_PER_SAMPLE   7
//#define VDIF_THREAD_ID         3
//#define VDIF_STATION_ID        3
//
//#define ETH_HDR_LEN    14
//#define IP_HDR_LEN     20
//#define UDP_HDR_LEN    8
//#define AXI_WORD_BYTES 64
//#define HEADER_LEN     (ETH_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN)
//#define VDIF_HDR_LEN   16
//#define HEAD_TOTAL_LEN (HEADER_LEN + VDIF_HDR_LEN)
//#define DATA_LEN       8192
//#define DATA_WORDS     (DATA_LEN / AXI_WORD_BYTES) // 128
//
//static const ap_uint<8> FULL_HEADER[HEADER_LEN] = {
//    // ETH
//    0x0C,0x42,0xA1,0x16,0x01,0xCC,0x02,0x11,0x22,0x33,0x44,0x55,0x08,0x00,
//    // IP
//    0x45,0x00,0x20,0x3C,0x00,0x00,0x40,0x00,0x40,0x11,0xEE,0xCF,
//    0xC0,0xA8,0xC8,0xC8,0xC0,0xA8,0xC8,0x64,
//    // UDP
//    0x1F,0x90,0x23,0xC4,0x20,0x28,0x00,0x00
//};
//
//void opfb_packer(
//    hls::stream<axis_pkt> &in,
//    hls::stream<axis_pkt> &out,
//    ap_uint<30> axi_sec,
//    ap_uint<6>  axi_epoch,
//    bool sma_1pps,
//    ap_uint<30> &sec_now_out,
//    ap_uint<6>  &epoch_now_out,
//    ap_uint<24> &frame_in_sec_out
//) {
//#pragma HLS INTERFACE axis register both port=in
//#pragma HLS INTERFACE axis register both port=out
//
////#pragma HLS INTERFACE s_axilite port=axi_sec bundle=CTRL
////#pragma HLS INTERFACE s_axilite port=axi_epoch bundle=CTRL
//#pragma HLS INTERFACE s_axilite port=axi_sec bundle=control
//#pragma HLS INTERFACE s_axilite port=axi_epoch bundle=control
//#pragma HLS INTERFACE ap_none port=sma_1pps
//#pragma HLS INTERFACE ap_none port=sec_now_out
//#pragma HLS INTERFACE ap_none port=epoch_now_out
//#pragma HLS INTERFACE ap_none port=frame_in_sec_out
//#pragma HLS INTERFACE ap_ctrl_none port=return
////#pragma HLS INTERFACE mode=s_axilite port=return
//
//    // 状态变量
//    static ap_uint<30> sec_now = 52806581;
//    static ap_uint<6>  epoch_now = 12;
//    static ap_uint<24> frame_in_sec = 0;
//    static axis_pkt data_cache;
//    static bool cache_valid = false;
//    static int word_cnt = 0;
//    static ap_uint<1> last_pps = 0;
//    static bool initialized = false;
//
//    if (axi_sec != sec_now || axi_epoch != epoch_now) {
//        sec_now = axi_sec;
//        epoch_now = axi_epoch;
//        frame_in_sec = 0;
//        initialized = true;
//    }
//
//    while (initialized) {
//#pragma HLS PIPELINE II=1
//
//        // 每拍都检测PPS和AXI-Lite参数
//        bool pps_rising = (sma_1pps && !last_pps);
//
//        // AXI-Lite同步（每拍都做，保证只要外部写入立即同步）
//        if (axi_sec != sec_now || axi_epoch != epoch_now) {
//            sec_now = axi_sec;
//            epoch_now = axi_epoch;
//            frame_in_sec = 0;
//        }
//
//        // PPS上升沿
//        if (pps_rising) {
//            sec_now++;
//            frame_in_sec = 0;
//        }
//
//        last_pps = sma_1pps;
//
//        axis_pkt out_pkt;
//
//        if (word_cnt == 0) {
//            if (!cache_valid && !in.empty()) {
//                data_cache = in.read();
//                cache_valid = true;
//            }
//            if (!cache_valid) break;
//
//            // VDIF Header
//            ap_uint<32> word0 = (ap_uint<32>(1) << 31) | (ap_uint<32>(0) << 30) | (sec_now & 0x3FFFFFFF);
//            ap_uint<32> word1 = (ap_uint<32>(0) << 30) | (ap_uint<32>(epoch_now) << 24) | (frame_in_sec & 0xFFFFFF);
//            ap_uint<32> word2 = (ap_uint<32>(VDIF_VERSION & 0x7) << 29) |
//                                (ap_uint<32>(VDIF_LOG2_NCHAN & 0x1F) << 24) |
//                                (ap_uint<32>(VDIF_DATA_FRAME_LENGTH) & 0xFFFFFF);
//            ap_uint<32> word3 = (ap_uint<32>(VDIF_DATA_TYPE & 0x1) << 31) |
//                                (ap_uint<32>(VDIF_BITS_PER_SAMPLE & 0x1F) << 26) |
//                                (ap_uint<32>(VDIF_THREAD_ID & 0x3FF) << 16) |
//                                (ap_uint<32>(VDIF_STATION_ID) & 0xFFFF);
//
//            ap_uint<8> vdif_header[16];
//#pragma HLS ARRAY_PARTITION variable=vdif_header complete dim=1
//            for (int i = 0; i < 4; i++)
//                vdif_header[i] = word0.range(i*8+7, i*8);
//            for (int i = 0; i < 4; i++)
//                vdif_header[i+4] = word1.range(i*8+7, i*8);
//            for (int i = 0; i < 4; i++)
//                vdif_header[i+8] = word2.range(i*8+7, i*8);
//            for (int i = 0; i < 4; i++)
//                vdif_header[i+12] = word3.range(i*8+7, i*8);
//
//            ap_uint<512> first_word = 0;
//            for (int i = 0; i < HEADER_LEN; i++)
//#pragma HLS UNROLL
//                first_word.range((i+1)*8-1, i*8) = FULL_HEADER[i];
//            for (int i = 0; i < VDIF_HDR_LEN; i++)
//#pragma HLS UNROLL
//                first_word.range((HEADER_LEN+i+1)*8-1, (HEADER_LEN+i)*8) = vdif_header[i];
//
//            out_pkt.data = first_word;
//            ap_uint<64> tkeep = 0;
//            for (int i = 0; i < HEAD_TOTAL_LEN; i++)
//#pragma HLS UNROLL
//                tkeep[i] = 1;
//            for (int i = HEAD_TOTAL_LEN; i < AXI_WORD_BYTES; i++)
//#pragma HLS UNROLL
//                tkeep[i] = 0;
//            out_pkt.keep = tkeep;
//            out_pkt.last = 0;
//            out.write(out_pkt);
//
//            word_cnt = 1;
//        } else {
//            if (!cache_valid && !in.empty()) {
//                data_cache = in.read();
//                cache_valid = true;
//            }
//            if (!cache_valid) break;
//
//            out_pkt = data_cache;
//            cache_valid = false;
//            out_pkt.keep = (ap_uint<64>)(-1);
//            out_pkt.last = (word_cnt == DATA_WORDS) ? 1 : 0;
//            out.write(out_pkt);
//
//            if (word_cnt == DATA_WORDS) {
//                word_cnt = 0;
//                frame_in_sec++;
//            } else {
//                word_cnt++;
//            }
//        }
//
//        // 输出变量
//        sec_now_out      = axi_sec;
//        epoch_now_out    = axi_epoch;
//        frame_in_sec_out = frame_in_sec;
//    }
//}
///////////////////////////////////////////////根据采样数来计算1秒，秒数增加太慢
//#include "opfb_packer.h"
//#include <ap_int.h>
//#include <hls_stream.h>
//
//// VDIF参数
//#define VDIF_VERSION           1
//#define VDIF_LOG2_NCHAN        3
//#define VDIF_DATA_FRAME_LENGTH 1024 // 8192/8=1024
//#define VDIF_DATA_TYPE         1
//#define VDIF_BITS_PER_SAMPLE   7
//#define VDIF_THREAD_ID         3
//#define VDIF_STATION_ID        3
//
//#define ETH_HDR_LEN    14
//#define IP_HDR_LEN     20
//#define UDP_HDR_LEN    8
//#define AXI_WORD_BYTES 64
//#define HEADER_LEN     (ETH_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN)
//#define VDIF_HDR_LEN   16
//#define HEAD_TOTAL_LEN (HEADER_LEN + VDIF_HDR_LEN)
//#define DATA_LEN       8192
//#define DATA_WORDS     (DATA_LEN / AXI_WORD_BYTES) // 128
//
//#define FRAMES_PER_SEC 259000000  // 每秒输入DFT帧数(=ADC_CLOCK/M)
//
//static const ap_uint<8> FULL_HEADER[HEADER_LEN] = {
//    // ETH
//    0x0C,0x42,0xA1,0x16,0x01,0xCC,0x02,0x11,0x22,0x33,0x44,0x55,0x08,0x00,
//    // IP
//    0x45,0x00,0x20,0x3C,0x00,0x00,0x40,0x00,0x40,0x11,0xEE,0xCF,
//    0xC0,0xA8,0xC8,0xC8,0xC0,0xA8,0xC8,0x64,
//    // UDP
//    0x1F,0x90,0x23,0xC4,0x20,0x28,0x00,0x00
//};
//
//void opfb_packer(
//    hls::stream<axis_pkt> &in,
//    hls::stream<axis_pkt> &out,
//    ap_uint<30> axi_sec,
//    ap_uint<6>  axi_epoch
//) {
////#pragma HLS INTERFACE axis register both port=in
////#pragma HLS INTERFACE axis register both port=out
////#pragma HLS INTERFACE s_axilite port=axi_sec bundle=control
////#pragma HLS INTERFACE s_axilite port=axi_epoch bundle=control
////#pragma HLS INTERFACE ap_none port=sec_now_out
////#pragma HLS INTERFACE ap_none port=epoch_now_out
////#pragma HLS INTERFACE ap_none port=frame_in_sec_out
////#pragma HLS INTERFACE ap_ctrl_hs port=return
//
//
//
////#pragma HLS INTERFACE s_axilite port=sec_now_out
////#pragma HLS INTERFACE s_axilite port=epoch_now_out
////#pragma HLS INTERFACE s_axilite port=frame_in_sec_out
//
//#pragma HLS INTERFACE axis register both port=in
//#pragma HLS INTERFACE axis register both port=out
//#pragma HLS INTERFACE s_axilite port=return bundle=control
//#pragma HLS INTERFACE s_axilite port=axi_sec bundle=control
//#pragma HLS INTERFACE s_axilite port=axi_epoch bundle=control
//
//    // 状态变量（static用于HLS综合后寄存）
//    static ap_uint<30> sec_now = 52806581;
//    static ap_uint<6>  epoch_now = 11;
//    static ap_uint<24> frame_in_sec = 0;
//    static ap_uint<32> frame_counter = 0; // 统计DFT帧数
//    static axis_pkt data_cache;
//    static bool cache_valid = false;
//    static int word_cnt = 0;
//    static bool initialized = false;
//
//    while (1) {
//#pragma HLS PIPELINE II=1
//
//        // AXI-Lite同步（每拍都做，保证只要外部写入立即同步）
//        if (!initialized&&(axi_sec != sec_now || axi_epoch != epoch_now)) {
//            sec_now = axi_sec;
//            epoch_now = axi_epoch;
//            frame_in_sec = 0;
//            frame_counter = 0;
//            initialized = true;
//        }
//
//        axis_pkt out_pkt;
//
//        if (word_cnt == 0) {
//            if (!cache_valid && !in.empty()) {
//                data_cache = in.read();
//                cache_valid = true;
//            }
//            if (!cache_valid) continue; // 没数据就等
//
//            // VDIF Header
//            ap_uint<32> word0 = (ap_uint<32>(1) << 31) | (ap_uint<32>(1) << 30) | (sec_now & 0x3FFFFFFF);
//            ap_uint<32> word1 = (ap_uint<32>(0) << 30) | (ap_uint<32>(epoch_now) << 24) | (frame_in_sec & 0xFFFFFF);
//            ap_uint<32> word2 = (ap_uint<32>(VDIF_VERSION & 0x7) << 29) |
//                                (ap_uint<32>(VDIF_LOG2_NCHAN & 0x1F) << 24) |
//                                (ap_uint<32>(VDIF_DATA_FRAME_LENGTH) & 0xFFFFFF);
//            ap_uint<32> word3 = (ap_uint<32>(VDIF_DATA_TYPE & 0x1) << 31) |
//                                (ap_uint<32>(VDIF_BITS_PER_SAMPLE & 0x1F) << 26) |
//                                (ap_uint<32>(VDIF_THREAD_ID & 0x3FF) << 16) |
//                                (ap_uint<32>(VDIF_STATION_ID) & 0xFFFF);
//
//            ap_uint<8> vdif_header[16];
//#pragma HLS ARRAY_PARTITION variable=vdif_header complete dim=1
//            for (int i = 0; i < 4; i++)
//                vdif_header[i] = word0.range(i*8+7, i*8);
//            for (int i = 0; i < 4; i++)
//                vdif_header[i+4] = word1.range(i*8+7, i*8);
//            for (int i = 0; i < 4; i++)
//                vdif_header[i+8] = word2.range(i*8+7, i*8);
//            for (int i = 0; i < 4; i++)
//                vdif_header[i+12] = word3.range(i*8+7, i*8);
//
//            ap_uint<512> first_word = 0;
//            for (int i = 0; i < HEADER_LEN; i++)
//#pragma HLS UNROLL
//                first_word.range((i+1)*8-1, i*8) = FULL_HEADER[i];
//            for (int i = 0; i < VDIF_HDR_LEN; i++)
//#pragma HLS UNROLL
//                first_word.range((HEADER_LEN+i+1)*8-1, (HEADER_LEN+i)*8) = vdif_header[i];
//
//            out_pkt.data = first_word;
//            ap_uint<64> tkeep = 0;
//            for (int i = 0; i < HEAD_TOTAL_LEN; i++)
//#pragma HLS UNROLL
//                tkeep[i] = 1;
//            for (int i = HEAD_TOTAL_LEN; i < AXI_WORD_BYTES; i++)
//#pragma HLS UNROLL
//                tkeep[i] = 0;
//            out_pkt.keep = tkeep;
//            out_pkt.last = 0;
//            out.write(out_pkt);
//
//            word_cnt = 1;
//        } else {
//            if (!cache_valid && !in.empty()) {
//                data_cache = in.read();
//                cache_valid = true;
//            }
//            if (!cache_valid) continue;
//
//            out_pkt = data_cache;
//            cache_valid = false;
//            out_pkt.keep = (ap_uint<64>)(-1);
//            out_pkt.last = (word_cnt == DATA_WORDS) ? 1 : 0;
//            out.write(out_pkt);
//
//            if (word_cnt == DATA_WORDS) {
//                word_cnt = 0;
//                frame_in_sec++;
//                frame_counter++;
//
//                // 满1秒自动加sec_now
//                if (frame_counter == FRAMES_PER_SEC) {
//                    sec_now++;
//                    frame_in_sec = 0;
//                    frame_counter = 0;
//                }
//            } else {
//                word_cnt++;
//            }
//        }
//
//    }
//}
////////////////////////////////////////////////////////////增加time_load，累计增秒模式
//#include "opfb_packer.h"
//#include <ap_int.h>
//#include <hls_stream.h>
//
////================= 可配置常量（允许在仿真时用 -D 覆写） =================
//// 实际值：DFT_FRAMES_PER_SEC = 2,072,000,000 / 7 = 296,000,000
//#ifndef DFT_FRAMES_PER_SEC
//#define DFT_FRAMES_PER_SEC 296000000ULL
//#endif
//
//// 每个 VDIF 数据帧聚合的 DFT 帧数：8192B / 64B = 128
//#ifndef DFT_FRAMES_PER_VDIF
//#define DFT_FRAMES_PER_VDIF 128
//#endif
//
////================= VDIF 头字段 =================
//#define VDIF_VERSION           1
//#define VDIF_LOG2_NCHAN        3
//#define VDIF_DATA_TYPE         1
//#define VDIF_BITS_PER_SAMPLE   7
//#define VDIF_THREAD_ID         3
//#define VDIF_STATION_ID        3
//// VDIF Frame Length (单位 8B；含 16B VDIF header，不含以太/IP/UDP)
//#define VDIF_FRAME_LENGTH_8B   1026  // (16 + 8192) / 8
//
////================= 固定以太 / IP / UDP 头 =================
//static const ap_uint<8> FULL_HEADER[NET_HDR_LEN] = {
//    // ETH (Dst / Src / Type)
//    0x0C,0x42,0xA1,0x16,0x01,0xCC, 0x02,0x11,0x22,0x33,0x44,0x55, 0x08,0x00,
//    // IPv4 (简化示例：不做动态总长/校验和更新)
//    0x45,0x00,0x20,0x3C,0x00,0x00,0x40,0x00,0x40,0x11,0xEE,0xCF,
//    0xC0,0xA8,0xC8,0xC8,0xC0,0xA8,0xC8,0x64,
//    // UDP (SrcPort/DstPort/Len/Checksum 这里 Len+Checksum 未真实更新)
//    0x1F,0x90,0x23,0xC4,0x20,0x28,0x00,0x00
//};
//
////================= 主功能 =================
//void opfb_packer(
//    hls::stream<axis_pkt> &in,
//    hls::stream<axis_pkt> &out,
//    ap_uint<30> axi_sec,
//    ap_uint<6>  axi_epoch,
//    ap_uint<1>  axi_time_load
//) {
//#pragma HLS INTERFACE axis       register both port=in
//#pragma HLS INTERFACE axis       register both port=out
//#pragma HLS INTERFACE s_axilite  port=return        bundle=control
//#pragma HLS INTERFACE s_axilite  port=axi_sec       bundle=control
//#pragma HLS INTERFACE s_axilite  port=axi_epoch     bundle=control
//#pragma HLS INTERFACE s_axilite  port=axi_time_load bundle=control
//
//    // 状态寄存
//    static ap_uint<30> sec_now             = 0;
//    static ap_uint<6>  epoch_now           = 0;
//    static unsigned long long dft_in_sec   = 0;     // 本秒累积的 DFT 帧数 (0..DFT_FRAMES_PER_SEC-1)
//    static ap_uint<24> vdif_frame_in_sec   = 0;     // VDIF 帧号 (0..(DFT_FRAMES_PER_SEC/128)-1)
//    static int  word_cnt                   = 0;     // 0=发头；1..128=发数据
//    static axis_pkt data_cache;
//    static bool cache_valid                = false;
//    // 新增：time_load 的上升沿检测 + “帧边界待装载”挂起位
//    static ap_uint<1> prev_time_load       = 0;
//    static ap_uint<1> time_load_pending    = 0;
//
//#pragma HLS PIPELINE II=1
//
//    // 采样上升沿（避免拉高太久重复装载）
//    ap_uint<1> load_edge = axi_time_load & (~prev_time_load);
//    prev_time_load = axi_time_load;
//    if (load_edge) {
//        time_load_pending = 1;
//    }
//
//    axis_pkt out_pkt;
//
//    if (word_cnt == 0) {
//        // 帧边界：若有挂起的对时请求，则在“写头”前装载新时间
//        if (time_load_pending) {
//            sec_now           = axi_sec;
//            epoch_now         = axi_epoch;
//            dft_in_sec        = 0;
//            vdif_frame_in_sec = 0;
//            time_load_pending = 0;
//        }
//
//        // 预取一个数据 512bit（保证首拍只放头，不混数据；没有数据就不启帧）
//        if (!cache_valid && !in.empty()) {
//            data_cache  = in.read();
//            cache_valid = true;
//        }
//        if (!cache_valid) return; // 没数据则本次调用不输出
//
//        // 构造 VDIF Header 四个 32bit
//        ap_uint<32> word0 =
//            (ap_uint<32>(1) << 31) |     // invalid/legacy 标志按原逻辑
//            (ap_uint<32>(1) << 30) |
//            (sec_now & 0x3FFFFFFF);
//        ap_uint<32> word1 =
//            (ap_uint<32>(0) << 30) |
//            (ap_uint<32>(epoch_now) << 24) |
//            (ap_uint<32>(vdif_frame_in_sec) & 0xFFFFFF);
//        ap_uint<32> word2 =
//            (ap_uint<32>(VDIF_VERSION & 0x7) << 29) |
//            (ap_uint<32>(VDIF_LOG2_NCHAN & 0x1F) << 24) |
//            (ap_uint<32>(VDIF_FRAME_LENGTH_8B) & 0xFFFFFF);
//        ap_uint<32> word3 =
//            (ap_uint<32>(VDIF_DATA_TYPE & 0x1) << 31) |
//            (ap_uint<32>(VDIF_BITS_PER_SAMPLE & 0x1F) << 26) |
//            (ap_uint<32>(VDIF_THREAD_ID & 0x3FF) << 16) |
//            (ap_uint<32>(VDIF_STATION_ID) & 0xFFFF);
//
//        ap_uint<8> vdif_header[VDIF_HDR_LEN];
//#pragma HLS ARRAY_PARTITION variable=vdif_header complete dim=1
//        for (int i=0;i<4;i++) vdif_header[i]    = word0.range(i*8+7, i*8);
//        for (int i=0;i<4;i++) vdif_header[i+4]  = word1.range(i*8+7, i*8);
//        for (int i=0;i<4;i++) vdif_header[i+8]  = word2.range(i*8+7, i*8);
//        for (int i=0;i<4;i++) vdif_header[i+12] = word3.range(i*8+7, i*8);
//
//        ap_uint<512> first_word = 0;
//        // 写网络头
//        for (int i=0;i<NET_HDR_LEN;i++) {
//#pragma HLS UNROLL
//            first_word.range((i+1)*8-1, i*8) = FULL_HEADER[i];
//        }
//        // 写 VDIF 头
//        for (int i=0;i<VDIF_HDR_LEN;i++) {
//#pragma HLS UNROLL
//            first_word.range((NET_HDR_LEN+i+1)*8-1, (NET_HDR_LEN+i)*8) = vdif_header[i];
//        }
//
//        out_pkt.data = first_word;
//        ap_uint<64> tkeep = 0;
//        for (int i=0;i<HEAD_TOTAL_LEN;i++) {
//#pragma HLS UNROLL
//            tkeep[i] = 1;
//        }
//        out_pkt.keep = tkeep;
//        out_pkt.strb = tkeep;  // 一般仿真用，硬件可忽略
//        out_pkt.last = 0;
//        out_pkt.user = 0;
//        out_pkt.id   = 0;
//        out_pkt.dest = 0;
//
//        out.write(out_pkt);
//        word_cnt = 1;
//    } else {
//        // 发送数据（每次调用只发 1 个 512bit 数据）
//        if (!cache_valid && !in.empty()) {
//            data_cache  = in.read();
//            cache_valid = true;
//        }
//        if (!cache_valid) return;
//
//        out_pkt        = data_cache;
//        cache_valid    = false;
//        out_pkt.keep   = (ap_uint<64>)(-1);
//        out_pkt.strb   = out_pkt.keep;
//        bool end_frame = (word_cnt == DATA_WORDS);
//        out_pkt.last   = end_frame ? 1 : 0;
//        out_pkt.user   = 0;
//        out_pkt.id     = 0;
//        out_pkt.dest   = 0;
//
//        out.write(out_pkt);
//
//        if (end_frame) {
//            // 完成 1 个 VDIF 数据帧（128 DFT 帧）
//            word_cnt = 0;
//            dft_in_sec += DFT_FRAMES_PER_VDIF;
//
//            if (dft_in_sec >= DFT_FRAMES_PER_SEC) {
//                dft_in_sec        -= DFT_FRAMES_PER_SEC;
//                sec_now++;
//                vdif_frame_in_sec = 0;
//            } else {
//                vdif_frame_in_sec++;
//            }
//        } else {
//            word_cnt++;
//        }
//    }
//}
///////////////////////////////////////////////////////////PS每秒更新时间，但ip长度还是8232，需改为8222，并更新校验码
//#include "opfb_packer.h"
//#include <ap_int.h>
//#include <hls_stream.h>
//
////================= VDIF 头字段 =================
//#define VDIF_VERSION           1
//#define VDIF_LOG2_NCHAN        3
//#define VDIF_DATA_TYPE         1
//#define VDIF_BITS_PER_SAMPLE   7
//#define VDIF_THREAD_ID         3
//#define VDIF_STATION_ID        3
//// VDIF Frame Length (单位 8B；含 16B VDIF header，不含以太/IP/UDP)
//#define VDIF_FRAME_LENGTH_8B   1026  // (16 + 8192) / 8
//
////================= 固定以太 / IP / UDP 头 =================
//static const ap_uint<8> FULL_HEADER[NET_HDR_LEN] = {
//    // ETH (Dst / Src / Type)
//    0x0C,0x42,0xA1,0x16,0x01,0xCC, 0x02,0x11,0x22,0x33,0x44,0x55, 0x08,0x00,
//    // IPv4 (示例：未动态更新总长/校验和)
//    0x45,0x00,0x20,0x3C,0x00,0x00,0x40,0x00,0x40,0x11,0xEE,0xCF,
//    0xC0,0xA8,0xC8,0xC8,0xC0,0xA8,0xC8,0x64,
//    // UDP (示例：Len/Checksum 未真实更新)
//    0x1F,0x90,0x23,0xC4,0x20,0x28,0x00,0x00
//};
//
//void opfb_packer(
//    hls::stream<axis_pkt> &in,
//    hls::stream<axis_pkt> &out,
//    ap_uint<30> axi_sec,
//    ap_uint<6>  axi_epoch,
//    ap_uint<1>  axi_time_toggle
//) {
//#pragma HLS INTERFACE axis       register both port=in
//#pragma HLS INTERFACE axis       register both port=out
//#pragma HLS INTERFACE s_axilite  port=return          bundle=control
//#pragma HLS INTERFACE s_axilite  port=axi_sec         bundle=control
//#pragma HLS INTERFACE s_axilite  port=axi_epoch       bundle=control
//#pragma HLS INTERFACE s_axilite  port=axi_time_toggle bundle=control
//#pragma HLS PIPELINE II=1
//
//    // 状态
//    static ap_uint<30> sec_now           = 0;
//    static ap_uint<6>  epoch_now         = 0;
//    static ap_uint<24> vdif_frame_in_sec = 0;   // 每秒内的帧序号
//    static int         word_cnt          = 0;   // 0=发头；1..128=发数据
//    static axis_pkt    data_cache;
//    static bool        cache_valid       = false;
//
//    // toggle: 看到值变化则记为“待装载”，在下一帧头生效
//    static ap_uint<1> prev_toggle        = 0;
//    static ap_uint<1> time_load_pending  = 0;
//    if (axi_time_toggle != prev_toggle) {
//        time_load_pending = 1;
//        prev_toggle       = axi_time_toggle;
//    }
//
//    axis_pkt out_pkt;
//
//    if (word_cnt == 0) {
//        // 帧边界：如有对时请求，先装载新时间
//        if (time_load_pending) {
//            sec_now           = axi_sec;
//            epoch_now         = axi_epoch;
//            vdif_frame_in_sec = 0;      // 新秒从 0 开始
//            time_load_pending = 0;
//        }
//
//        // 预取第一拍数据（无数据不启帧）
//        if (!cache_valid && !in.empty()) {
//            data_cache  = in.read();
//            cache_valid = true;
//        }
//        if (!cache_valid) return;
//
//        // 组 VDIF 头
//        ap_uint<32> w0 = (ap_uint<32>(1) << 31) | (ap_uint<32>(1) << 30) | (sec_now & 0x3FFFFFFF);
//        ap_uint<32> w1 = (ap_uint<32>(0) << 30) | (ap_uint<32>(epoch_now) << 24) | (ap_uint<32>(vdif_frame_in_sec) & 0xFFFFFF);
//        ap_uint<32> w2 = (ap_uint<32>(VDIF_VERSION & 0x7) << 29) | (ap_uint<32>(VDIF_LOG2_NCHAN & 0x1F) << 24) | (ap_uint<32>(VDIF_FRAME_LENGTH_8B) & 0xFFFFFF);
//        ap_uint<32> w3 = (ap_uint<32>(VDIF_DATA_TYPE & 0x1) << 31) | (ap_uint<32>(VDIF_BITS_PER_SAMPLE & 0x1F) << 26) |
//                         (ap_uint<32>(VDIF_THREAD_ID & 0x3FF) << 16) | (ap_uint<32>(VDIF_STATION_ID) & 0xFFFF);
//
//        ap_uint<8> vdif_header[VDIF_HDR_LEN];
//#pragma HLS ARRAY_PARTITION variable=vdif_header complete dim=1
//        for (int i=0;i<4;i++) vdif_header[i]    = w0.range(i*8+7, i*8);
//        for (int i=0;i<4;i++) vdif_header[i+4]  = w1.range(i*8+7, i*8);
//        for (int i=0;i<4;i++) vdif_header[i+8]  = w2.range(i*8+7, i*8);
//        for (int i=0;i<4;i++) vdif_header[i+12] = w3.range(i*8+7, i*8);
//
//        ap_uint<512> first_word = 0;
//        for (int i=0;i<NET_HDR_LEN;i++) {
//#pragma HLS UNROLL
//            first_word.range((i+1)*8-1, i*8) = FULL_HEADER[i];
//        }
//        for (int i=0;i<VDIF_HDR_LEN;i++) {
//#pragma HLS UNROLL
//            first_word.range((NET_HDR_LEN+i+1)*8-1, (NET_HDR_LEN+i)*8) = vdif_header[i];
//        }
//
//        out_pkt.data = first_word;
//        ap_uint<64> tkeep = 0;
//        for (int i=0;i<HEAD_TOTAL_LEN;i++) {
//#pragma HLS UNROLL
//            tkeep[i] = 1;
//        }
//        out_pkt.keep = tkeep;
//        out_pkt.strb = tkeep;
//        out_pkt.last = 0;
//        out_pkt.user = 0;
//        out_pkt.id   = 0;
//        out_pkt.dest = 0;
//
//        out.write(out_pkt);
//        word_cnt = 1;
//    } else {
//        // 发送数据（本次调用发 1 个 512bit）
//        if (!cache_valid && !in.empty()) {
//            data_cache  = in.read();
//            cache_valid = true;
//        }
//        if (!cache_valid) return;
//
//        out_pkt      = data_cache;
//        cache_valid  = false;
//        out_pkt.keep = (ap_uint<64>)(-1);
//        out_pkt.strb = out_pkt.keep;
//        bool end_frame = (word_cnt == DATA_WORDS);
//        out_pkt.last = end_frame ? 1 : 0;
//        out_pkt.user = 0;
//        out_pkt.id   = 0;
//        out_pkt.dest = 0;
//
//        out.write(out_pkt);
//
//        if (end_frame) {
//            // 一帧完成：进入下一帧，frame_in_sec++
//            word_cnt = 0;
//            vdif_frame_in_sec++;
//        } else {
//            word_cnt++;
//        }
//    }
//}

////////////////////////////////////////////////////////////////更新了ip长度及校验码，时间戳更新已准确
#include "opfb_packer.h"
#include <ap_int.h>
#include <hls_stream.h>

//================= VDIF 头字段 =================
#define VDIF_VERSION           1
#define VDIF_LOG2_NCHAN        3
#define VDIF_DATA_TYPE         1
#define VDIF_BITS_PER_SAMPLE   7
#define VDIF_THREAD_ID         3
#define VDIF_STATION_ID        3
// VDIF Frame Length (单位 8B；含 16B VDIF header，不含以太/IP/UDP)
#define VDIF_FRAME_LENGTH_8B   1026  // (16 + 8192) / 8

//================= 固定以太 / IP / UDP 头 =================
// 计算基于：UDP payload = 8214(8192 DATA + 16 VDIF) 字节；UDP Length = 8214 + 8 = 8222 (0x201E)
// IP Total Length = 20 (IP) + 8222 (UDP) = 8242 (0x2032)
// IPv4 Header Checksum(20字节头，校验和字段视为0计算) = 0x083D
static const ap_uint<8> FULL_HEADER[NET_HDR_LEN] = {
    // Ethernet (Dst / Src / Type)
    0x0C,0x42,0xA1,0x16,0x01,0xCC, 0x02,0x11,0x22,0x33,0x44,0x55, 0x08,0x00,
    // IPv4
    0x45,0x00,
    0x20,0x32,  // Total Length = 8242 (0x2032)
    0x00,0x00,  // Identification
    0x40,0x00,  // Flags/Fragment (DF=1, offset=0)
    0x40,       // TTL = 64
    0x11,       // Protocol = UDP(17)
    0x08,0x3D,  // Header Checksum (recomputed)
    0xC0,0xA8,0xC8,0xC8,  // Src IP = 192.168.200.200
    0xC0,0xA8,0xC8,0x64,  // Dst IP = 192.168.200.100
    // UDP
    0x1F,0x90,  // Src Port = 8080
    0x23,0xC4,  // Dst Port = 9140
    0x20,0x1E,  // Length = 8222 (0x201E)
    0x00,0x00   // Checksum = 0 (IPv4 可为 0)
};

void opfb_packer(
    hls::stream<axis_pkt> &in,
    hls::stream<axis_pkt> &out,
    ap_uint<30> axi_sec,
    ap_uint<6>  axi_epoch,
    ap_uint<1>  axi_time_toggle
) {
#pragma HLS INTERFACE axis       register both port=in
#pragma HLS INTERFACE axis       register both port=out
#pragma HLS INTERFACE s_axilite  port=return          bundle=control
#pragma HLS INTERFACE s_axilite  port=axi_sec         bundle=control
#pragma HLS INTERFACE s_axilite  port=axi_epoch       bundle=control
#pragma HLS INTERFACE s_axilite  port=axi_time_toggle bundle=control
#pragma HLS PIPELINE II=1

    // 状态
    static ap_uint<30> sec_now           = 0;
    static ap_uint<6>  epoch_now         = 0;
    static ap_uint<24> vdif_frame_in_sec = 0;   // 每秒内的帧序号
    static int         word_cnt          = 0;   // 0=发头；1..128=发数据
    static axis_pkt    data_cache;
    static bool        cache_valid       = false;

    // toggle: 看到值变化则记为“待装载”，在下一帧头生效
    static ap_uint<1> prev_toggle        = 0;
    static ap_uint<1> time_load_pending  = 0;
    if (axi_time_toggle != prev_toggle) {
        time_load_pending = 1;
        prev_toggle       = axi_time_toggle;
    }

    axis_pkt out_pkt;

    if (word_cnt == 0) {
        // 帧边界：如有对时请求，先装载新时间
        if (time_load_pending) {
            sec_now           = axi_sec;
            epoch_now         = axi_epoch;
            vdif_frame_in_sec = 0;      // 新秒从 0 开始
            time_load_pending = 0;
        }

        // 预取第一拍数据（无数据不启帧）
        if (!cache_valid && !in.empty()) {
            data_cache  = in.read();
            cache_valid = true;
        }
        if (!cache_valid) return;

        // 组 VDIF 头
        ap_uint<32> w0 = (ap_uint<32>(1) << 31) | (ap_uint<32>(1) << 30) | (sec_now & 0x3FFFFFFF);
        ap_uint<32> w1 = (ap_uint<32>(0) << 30) | (ap_uint<32>(epoch_now) << 24) | (ap_uint<32>(vdif_frame_in_sec) & 0xFFFFFF);
        ap_uint<32> w2 = (ap_uint<32>(VDIF_VERSION & 0x7) << 29) | (ap_uint<32>(VDIF_LOG2_NCHAN & 0x1F) << 24) | (ap_uint<32>(VDIF_FRAME_LENGTH_8B) & 0xFFFFFF);
        ap_uint<32> w3 = (ap_uint<32>(VDIF_DATA_TYPE & 0x1) << 31) | (ap_uint<32>(VDIF_BITS_PER_SAMPLE & 0x1F) << 26) |
                         (ap_uint<32>(VDIF_THREAD_ID & 0x3FF) << 16) | (ap_uint<32>(VDIF_STATION_ID) & 0xFFFF);

        ap_uint<8> vdif_header[VDIF_HDR_LEN];
#pragma HLS ARRAY_PARTITION variable=vdif_header complete dim=1
        for (int i=0;i<4;i++) vdif_header[i]    = w0.range(i*8+7, i*8);
        for (int i=0;i<4;i++) vdif_header[i+4]  = w1.range(i*8+7, i*8);
        for (int i=0;i<4;i++) vdif_header[i+8]  = w2.range(i*8+7, i*8);
        for (int i=0;i<4;i++) vdif_header[i+12] = w3.range(i*8+7, i*8);

        ap_uint<512> first_word = 0;
        for (int i=0;i<NET_HDR_LEN;i++) {
#pragma HLS UNROLL
            first_word.range((i+1)*8-1, i*8) = FULL_HEADER[i];
        }
        for (int i=0;i<VDIF_HDR_LEN;i++) {
#pragma HLS UNROLL
            first_word.range((NET_HDR_LEN+i+1)*8-1, (NET_HDR_LEN+i)*8) = vdif_header[i];
        }

        out_pkt.data = first_word;
        ap_uint<64> tkeep = 0;
        for (int i=0;i<HEAD_TOTAL_LEN;i++) {
#pragma HLS UNROLL
            tkeep[i] = 1;
        }
        out_pkt.keep = tkeep;
        out_pkt.strb = tkeep;
        out_pkt.last = 0;
        out_pkt.user = 0;
        out_pkt.id   = 0;
        out_pkt.dest = 0;

        out.write(out_pkt);
        word_cnt = 1;
    } else {
        // 发送数据（本次调用发 1 个 512bit）
        if (!cache_valid && !in.empty()) {
            data_cache  = in.read();
            cache_valid = true;
        }
        if (!cache_valid) return;

        out_pkt      = data_cache;
        cache_valid  = false;
        out_pkt.keep = (ap_uint<64>)(-1);
        out_pkt.strb = out_pkt.keep;
        bool end_frame = (word_cnt == DATA_WORDS);
        out_pkt.last = end_frame ? 1 : 0;
        out_pkt.user = 0;
        out_pkt.id   = 0;
        out_pkt.dest = 0;

        out.write(out_pkt);

        if (end_frame) {
            // 一帧完成：进入下一帧，frame_in_sec++
            word_cnt = 0;
            vdif_frame_in_sec++;
        } else {
            word_cnt++;
        }
    }
}
