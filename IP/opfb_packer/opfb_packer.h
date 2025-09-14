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
//// ��� payload �ֽ�����Ĭ�� 8972 �� ��֡ 9014B��
//#ifndef MAX_PAYLOAD_BYTES
//#define MAX_PAYLOAD_BYTES 8972
//#endif
//
//// ���ã�MAC��IP���˿�
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
//#include <ap_axi_sdata.h>   // AXI4-Stream ����
//
//const int payload_len_bytes = 8224;   // �̶�֡����������MTU=1500
//const int header_len = 14 + 20 + 8;
//const int word_bytes = 64;
//const int DATA_LEN = 8192;
//const int DATA_WORDS = DATA_LEN / word_bytes;   // 128
//
//typedef ap_axiu<512,0,0,0> axis_pkt;
//
//// ���ĺ�������
////void opfb_packer(hls::stream<axis_pkt> &in,
////                    hls::stream<axis_pkt> &out,ap_uint<30> axi_sec,
////				    ap_uint<6>  axi_epoch);
//
//void opfb_packer(
//    hls::stream<axis_pkt> &in,
//    hls::stream<axis_pkt> &out,
//    ap_uint<30> axi_sec,      // AXI-Lite���룬��ʼ��
//    ap_uint<6>  axi_epoch    // AXI-Lite���룬��ʼepoch
////    bool sma_1pps,             // �ⲿ1PPS�źţ����ĸ���Ч
////	ap_uint<30>     &sec_now_out,
////		ap_uint<6>  &epoch_now_out,
////		ap_uint<24> &frame_in_sec_out
//);
//
//#endif
////////////////////////////////////////////////////////����time_load
//#ifndef OPFB_PACKER_H
//#define OPFB_PACKER_H
//
//#include <ap_int.h>
//#include <hls_stream.h>
//#include <ap_axi_sdata.h>
//
//// �̶���������ʵ�ֱ���һ�£�
//const int WORD_BYTES   = 64;
//const int ETH_HDR_LEN  = 14;
//const int IP_HDR_LEN   = 20;
//const int UDP_HDR_LEN  = 8;
//const int NET_HDR_LEN  = ETH_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN; // 42
//const int VDIF_HDR_LEN = 16;
//const int HEAD_TOTAL_LEN = NET_HDR_LEN + VDIF_HDR_LEN;           // 58
//const int DATA_LEN     = 8192;          // �� payload ���ݳ���
//const int DATA_WORDS   = DATA_LEN / WORD_BYTES; // 128 x 512bit
//
//// axis_pkt ���ͣ����� side-channel �Զ���λ��
//typedef ap_axiu<512,0,0,0> axis_pkt;
//
//// ���㣺ap_ctrl_hs��ap_start �ֶ�/auto_restart �Զ��������ɣ�
//// ע�⣺axi_time_load �����壨д1->д0�����ں������������ؼ��+֡�߽�װ�ء���������;���֡
//void opfb_packer(
//    hls::stream<axis_pkt> &in,
//    hls::stream<axis_pkt> &out,
//    ap_uint<30> axi_sec,
//    ap_uint<6>  axi_epoch,
//    ap_uint<1>  axi_time_load
//);
//
//#endif
/////////////////////////////////////////////////////////ÿ�����ʱ��
#ifndef OPFB_PACKER_H
#define OPFB_PACKER_H

#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

// �̶�����
const int WORD_BYTES     = 64;
const int ETH_HDR_LEN    = 14;
const int IP_HDR_LEN     = 20;
const int UDP_HDR_LEN    = 8;
const int NET_HDR_LEN    = ETH_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN; // 42
const int VDIF_HDR_LEN   = 16;
const int HEAD_TOTAL_LEN = NET_HDR_LEN + VDIF_HDR_LEN;             // 58
const int DATA_LEN       = 8192;                                   // payload bytes
const int DATA_WORDS     = DATA_LEN / WORD_BYTES;                  // 128 x 512-bit

// AXI4-Stream ����
typedef ap_axiu<512,0,0,0> axis_pkt;

// ���㣨ap_ctrl_hs��
// - axi_time_toggle: ÿ�η�תһ�Σ�0->1 �� 1->0����ʾ�������ʱ����IP ����һ֡ͷװ�� axi_sec/epoch ���� frame_in_sec ���㡣
void opfb_packer(
    hls::stream<axis_pkt> &in,
    hls::stream<axis_pkt> &out,
    ap_uint<30> axi_sec,
    ap_uint<6>  axi_epoch,
    ap_uint<1>  axi_time_toggle
);

#endif
