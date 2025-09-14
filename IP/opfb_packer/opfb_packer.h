//#ifndef OPFB_PACKER_H
//#define OPFB_PACKER_H
//
//#include <ap_int.h>
//#include <hls_stream.h>
//#include <ap_axi_sdata.h>
//
//
//#define DATA_WIDTH 512
//#define BYTES_PER_WORD (DATA_WIDTH/8)
//typedef ap_axiu<DATA_WIDTH,1,1,1> axis_raw_t;
//
//// 最大 payload 字节数（默认 8972 → 总帧 9014B）
//#ifndef MAX_PAYLOAD_BYTES
//#define MAX_PAYLOAD_BYTES 8972
//#endif
//
//// 配置：MAC、IP、端口
//struct net_cfg_t {
//    ap_uint<48> src_mac;
//    ap_uint<48> dst_mac;
//    ap_uint<32> src_ip;
//    ap_uint<32> dst_ip;
//    ap_uint<16> src_port;
//    ap_uint<16> dst_port;
//};
//
//void opfb_packer(
//    hls::stream<axis_raw_t> &in_stream,
//    hls::stream<axis_raw_t> &out_stream,
//    const net_cfg_t &cfg
//);
//
//#endif

/////////////////////////////////////////////////////////////
//#ifndef OPFB_PACKER_H
//#define OPFB_PACKER_H
//
//#include <ap_int.h>
//#include <hls_stream.h>
//#include <ap_axi_sdata.h>   // AXI4-Stream 类型
//
//const int payload_len_bytes = 8224;   // 固定帧长，不超过MTU=1500
//const int header_len = 14 + 20 + 8;
//const int word_bytes = 64;
//const int DATA_LEN = 8192;
//const int DATA_WORDS = DATA_LEN / word_bytes;   // 128
//
//typedef ap_axiu<512,0,0,0> axis_pkt;
//
//// 核心函数声明
////void opfb_packer(hls::stream<axis_pkt> &in,
////                    hls::stream<axis_pkt> &out,ap_uint<30> axi_sec,
////				    ap_uint<6>  axi_epoch);
//
//void opfb_packer(
//    hls::stream<axis_pkt> &in,
//    hls::stream<axis_pkt> &out,
//    ap_uint<30> axi_sec,      // AXI-Lite输入，初始秒
//    ap_uint<6>  axi_epoch    // AXI-Lite输入，初始epoch
////    bool sma_1pps,             // 外部1PPS信号，单拍高有效
////	ap_uint<30>     &sec_now_out,
////		ap_uint<6>  &epoch_now_out,
////		ap_uint<24> &frame_in_sec_out
//);
//
//#endif
////////////////////////////////////////////////////////增加time_load
//#ifndef OPFB_PACKER_H
//#define OPFB_PACKER_H
//
//#include <ap_int.h>
//#include <hls_stream.h>
//#include <ap_axi_sdata.h>
//
//// 固定参数（与实现保持一致）
//const int WORD_BYTES   = 64;
//const int ETH_HDR_LEN  = 14;
//const int IP_HDR_LEN   = 20;
//const int UDP_HDR_LEN  = 8;
//const int NET_HDR_LEN  = ETH_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN; // 42
//const int VDIF_HDR_LEN = 16;
//const int HEAD_TOTAL_LEN = NET_HDR_LEN + VDIF_HDR_LEN;           // 58
//const int DATA_LEN     = 8192;          // 纯 payload 数据长度
//const int DATA_WORDS   = DATA_LEN / WORD_BYTES; // 128 x 512bit
//
//// axis_pkt 类型（不含 side-channel 自定义位宽）
//typedef ap_axiu<512,0,0,0> axis_pkt;
//
//// 顶层：ap_ctrl_hs（ap_start 手动/auto_restart 自动重启均可）
//// 注意：axi_time_load 需脉冲（写1->写0），内核已做“上升沿检测+帧边界装载”，不会中途打断帧
//void opfb_packer(
//    hls::stream<axis_pkt> &in,
//    hls::stream<axis_pkt> &out,
//    ap_uint<30> axi_sec,
//    ap_uint<6>  axi_epoch,
//    ap_uint<1>  axi_time_load
//);
//
//#endif
/////////////////////////////////////////////////////////每秒更新时间
#ifndef OPFB_PACKER_H
#define OPFB_PACKER_H

#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

// 固定参数
const int WORD_BYTES     = 64;
const int ETH_HDR_LEN    = 14;
const int IP_HDR_LEN     = 20;
const int UDP_HDR_LEN    = 8;
const int NET_HDR_LEN    = ETH_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN; // 42
const int VDIF_HDR_LEN   = 16;
const int HEAD_TOTAL_LEN = NET_HDR_LEN + VDIF_HDR_LEN;             // 58
const int DATA_LEN       = 8192;                                   // payload bytes
const int DATA_WORDS     = DATA_LEN / WORD_BYTES;                  // 128 x 512-bit

// AXI4-Stream 类型
typedef ap_axiu<512,0,0,0> axis_pkt;

// 顶层（ap_ctrl_hs）
// - axi_time_toggle: 每次翻转一次（0->1 或 1->0）表示“请求对时”；IP 在下一帧头装载 axi_sec/epoch 并把 frame_in_sec 清零。
void opfb_packer(
    hls::stream<axis_pkt> &in,
    hls::stream<axis_pkt> &out,
    ap_uint<30> axi_sec,
    ap_uint<6>  axi_epoch,
    ap_uint<1>  axi_time_toggle
);

#endif
