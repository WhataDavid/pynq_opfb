//#include "opfb_packer.h"
//#include <iostream>
//
//int main() {
//    hls::stream<axis_pkt> in, out;
//
//    // 构造输入payload
//    for (int i = 0; i < payload_len_bytes*4; i += 64) {
//        axis_pkt word;
//        word.data = 0;
//        word.keep = 0xFFFFFFFFFFFFFFFF;
//        word.last = (i + 64 >= payload_len_bytes*4) ? 1 : 0;
//        for (int b = 0; b < 64 && (i + b) < payload_len_bytes*4; b++) {
//            word.data.range((b+1)*8-1, b*8) = (i + b) & 0xFF;
//        }
//        in.write(word);
//    }
//
//    // 调用待测函数
//    opfb_packer(in, out,0,0);
//
//    // 读取输出并显示
//    int word_cnt = 0;
//    while (!out.empty()) {
//        axis_pkt word = out.read();
//        std::cout << "AXIS Word " << word_cnt++ << ":\n";
//        for (int b = 0; b < 64; b++) {
//            std::cout << std::hex << ((word.data >> (b*8)) & 0xFF);
//            if (b != 63) std::cout << " ";
//        }
//        std::cout << "\ntkeep=0x" << std::hex << word.keep << ", tlast=" << std::hex << (int)word.last << std::endl;
//    }
//    return 0;
//}

//////////////////////////////////仿真含1pps
//#include "opfb_packer.h"
//#include <hls_stream.h>
//#include <ap_int.h>
//#include <iostream>
//#include <iomanip>
//
//#define TB_TOTAL_FRAMES 3      // 仿真帧数
//#define TB_PPS_PERIOD   150    // 1PPS脉冲周期(仿真拍数，需保证每帧都会遇到PPS)
//#define TB_INPUT_WORDS  DATA_WORDS
//
//// 打印VDIF头部秒和帧号（小端拼接）
//void print_vdif_header(const axis_pkt& pkt) {
//    ap_uint<512> wd = pkt.data;
//    // VDIF头在offset=42, 长度16字节
//    ap_uint<32> word0 = wd.range((42+4)*8-1, (42+0)*8);
//    ap_uint<32> word1 = wd.range((42+8)*8-1, (42+4)*8);
//    ap_uint<32> word2 = wd.range((42+12)*8-1, (42+8)*8);
//    ap_uint<32> word3 = wd.range((42+16)*8-1, (42+12)*8);
//
//    ap_uint<30> sec = word0 & 0x3FFFFFFF;
//    ap_uint<6>  epoch = (word1 >> 24) & 0x3F;
//    ap_uint<24> frame_in_sec = word1 & 0xFFFFFF;
//    std::cout << "VDIF: sec=" << sec
//              << " epoch=" << (int)epoch
//              << " frame_in_sec=" << frame_in_sec
//              << " [hex 0x" << std::hex << std::setw(8) << std::setfill('0') << (uint32_t)word0 << "]"
//              << std::dec << std::endl;
//}
//
//int main() {
//    hls::stream<axis_pkt> in("in"), out("out");
//
//    // 1. 准备输入数据流：每帧128拍，每拍64字节
//    for (int f = 0; f < TB_TOTAL_FRAMES; ++f) {
//        for (int w = 0; w < TB_INPUT_WORDS; ++w) {
//            axis_pkt pkt;
//            for (int b = 0; b < 64; ++b)
//                pkt.data.range((b+1)*8-1, b*8) = (ap_uint<8>)(f+1); // 数据内容可自定义
//            pkt.keep = (ap_uint<64>)(-1);
//            pkt.last = (w == TB_INPUT_WORDS-1) ? 1 : 0;
//            in.write(pkt);
//        }
//    }
//
//    // 2. AXI-Lite参数初始化
//    ap_uint<30> axi_sec   = 10000;   // 初始秒
//    ap_uint<6>  axi_epoch = 31;      // 初始epoch
//    bool sma_1pps = 0;
//
//    // 3. 仿真总时钟周期数（保证包含所有输入数据和多个PPS周期）
//    int total_cycles = TB_TOTAL_FRAMES * (TB_INPUT_WORDS + 10);
//
//    int pps_cnt = 0;
//    int frame_count = 0;
//    int main_clk = 0;
//
//    static ap_uint<30> sec_now = 0;
//        static ap_uint<6>  epoch_now = 0;
//        static ap_uint<24> frame_in_sec = 0;
//
//    // 主仿真循环
//    while (main_clk < total_cycles || !out.empty()) {
//        // 1PPS脉冲模拟：每TB_PPS_PERIOD拍拉高1拍
//        sma_1pps = (pps_cnt == 0);
//        pps_cnt = (pps_cnt + 1) % TB_PPS_PERIOD;
//
//        // 调用 DUT
//        opfb_packer(in, out, axi_sec, axi_epoch, sma_1pps,sec_now,epoch_now,frame_in_sec);
//
//        // 读取并打印输出
//        while (!out.empty()) {
//            axis_pkt pkt = out.read();
//
//            // 打印首拍VDIF头
//            if (pkt.keep != (ap_uint<64>)(-1)) {
//                std::cout << "[output frame " << frame_count << "] ";
//                print_vdif_header(pkt);
//                frame_count++;
//            }
//            // 可加更多内容打印/校验
//        }
//
//        ++main_clk;
//    }
//
//    std::cout << "Testbench finished. Total output frames: " << frame_count << std::endl;
//    return 0;
//}

////////////////////////////////////////仿真含有sec_now_out、epoch_now_out、frame_in_sec_out
//#include "opfb_packer.h"
//#include <hls_stream.h>
//#include <ap_int.h>
//#include <iostream>
//#include <iomanip>
//
//#define TB_TOTAL_FRAMES 6
//#define DATA_WORDS 128
//
//void print_vdif_header(const axis_pkt& pkt) {
//    ap_uint<512> wd = pkt.data;
//    ap_uint<32> word0 = wd.range((42+4)*8-1, (42+0)*8);
//    ap_uint<32> word1 = wd.range((42+8)*8-1, (42+4)*8);
//    ap_uint<30> sec = word0 & 0x3FFFFFFF;
//    ap_uint<6>  epoch = (word1 >> 24) & 0x3F;
//    ap_uint<24> frame_in_sec = word1 & 0xFFFFFF;
//    std::cout << "VDIF: sec=" << sec
//              << " epoch=" << (int)epoch
//              << " frame_in_sec=" << frame_in_sec
//              << " [hex 0x" << std::hex << std::setw(8) << std::setfill('0') << (uint32_t)word0 << "]"
//              << std::dec << std::endl;
//}
//
//int main() {
//    hls::stream<axis_pkt> in("in"), out("out");
//
//    // 1. 准备输入数据流
//    for (int f = 0; f < TB_TOTAL_FRAMES; ++f) {
//        for (int w = 0; w < DATA_WORDS; ++w) {
//            axis_pkt pkt;
//            for (int b = 0; b < 64; ++b)
//                pkt.data.range((b+1)*8-1, b*8) = (ap_uint<8>)(f+1);
//            pkt.keep = (ap_uint<64>)(-1);
//            pkt.last = (w == DATA_WORDS-1) ? 1 : 0;
//            in.write(pkt);
//        }
//    }
//
//    ap_uint<30> axi_sec   = 10000;
//    ap_uint<6>  axi_epoch = 31;
//    bool sma_1pps = 0;
//
//    ap_uint<30> sec_now_out = 0;
//    ap_uint<6>  epoch_now_out = 0;
//    ap_uint<24> frame_in_sec_out = 0;
//
//    int output_frame = 0;
//    int frame_word_cnt = 0;
//
//    // 用于判断sma_1pps何时拉高
//    int pps_frame = 3; // 每3帧拉高一次
//    int pps_start = -1; // 拉高周期起点
//
//    // 主仿真循环（按拍模拟）
//    while (output_frame < TB_TOTAL_FRAMES || !out.empty()) {
//        // 判断是否进入1PPS高电平区
//        if (output_frame >= pps_frame && output_frame < pps_frame + 1) {
//            // 本帧的所有拍都拉高
//            sma_1pps = 1;
//        } else {
//            sma_1pps = 0;
//        }
//
//        opfb_packer(in, out, axi_sec, axi_epoch);
//
//        // 读取并打印输出
//        while (!out.empty()) {
//            axis_pkt pkt = out.read();
//            if (pkt.keep != (ap_uint<64>)(-1)) {
//                std::cout << "[output frame " << output_frame << "] ";
//                print_vdif_header(pkt);
//                output_frame++;
//                // 1PPS脉冲的下一次起点
//                if (output_frame == pps_frame + 1) {
//                    pps_frame += 3;
//                }
//            }
//        }
//    }
//
//    std::cout << "sec_now_out=" << sec_now_out
//              << " epoch_now_out=" << (int)epoch_now_out
//              << " frame_in_sec_out=" << frame_in_sec_out << std::endl;
//    std::cout << "Testbench finished. Total output frames: " << output_frame << std::endl;
//    return 0;
//}
#include <iostream>
#include <iomanip>
#include <cstdint>
#include "opfb_packer.h"

// 若要加速测试秒翻转，可在编译 testbench 时添加：
// g++ ... -DDFT_FRAMES_PER_SEC=1280
// 并保证与 DUT 一致（该宏在 opfb_packer.cpp 中被引用）
//
// 默认宏（296000000）下做秒翻转测试需要很多帧，耗时巨大；所以默认关闭长测试。
//
// 取消下一行注释以启用“秒翻转测试” (需要把 DFT_FRAMES_PER_SEC 调小如 1280):
 #define RUN_SECOND_ROLLOVER_TEST

#ifndef DFT_FRAMES_PER_SEC
#define DFT_FRAMES_PER_SEC 1280ULL
#endif

#ifndef DFT_FRAMES_PER_VDIF
#define DFT_FRAMES_PER_VDIF 128
#endif

//================= 生成一条伪数据（便于识别） =================
static ap_uint<512> make_data_word(uint32_t frame_idx, uint32_t word_idx_in_frame) {
    ap_uint<512> w = 0;
    for (int lane=0; lane<16; ++lane) {
        uint32_t val = 0;
        val |= (0xA5u << 24);                 // 花样
        val |= ((frame_idx & 0xFF) << 8);     // 中间字节放帧号低8位
        val |= (lane & 0xFF);                 // 低8位放 lane
        // 放入 32bit lane
        w.range(32*(lane+1)-1, 32*lane) = val;
    }
    // 也可以塞入 word_idx_in_frame，演示足够就不再展开
    return w;
}

//================= 解析 VDIF 头 =================
struct ParsedVDIF {
    uint32_t word0, word1, word2, word3;
    uint32_t sec;
    uint32_t epoch;
    uint32_t frame_in_sec;
    uint32_t frame_len_8B;
};

static ParsedVDIF parse_vdif_header(const ap_uint<512>& first_word) {
    ParsedVDIF p{};
    const int vdif_offset = NET_HDR_LEN; // 42
    auto get_byte = [&](int i)->uint8_t {
        return (uint8_t)first_word.range((i+1)*8-1, i*8);
    };
    auto get_u32_le = [&](int off)->uint32_t {
        uint32_t v = 0;
        v |= (uint32_t)get_byte(off+0) << 0;
        v |= (uint32_t)get_byte(off+1) << 8;
        v |= (uint32_t)get_byte(off+2) << 16;
        v |= (uint32_t)get_byte(off+3) << 24;
        return v;
    };

    p.word0 = get_u32_le(vdif_offset + 0);
    p.word1 = get_u32_le(vdif_offset + 4);
    p.word2 = get_u32_le(vdif_offset + 8);
    p.word3 = get_u32_le(vdif_offset +12);

    p.sec          = (p.word0 & 0x3FFFFFFF);
    p.epoch        = (p.word1 >> 24) & 0x3F;
    p.frame_in_sec = (p.word1 & 0xFFFFFF);
    p.frame_len_8B = (p.word2 & 0xFFFFFF);
    return p;
}

static void print_header(uint64_t global_frame_idx, const ParsedVDIF& h) {
    std::cout << "Frame#" << global_frame_idx
              << "  sec=" << h.sec
              << "  epoch=" << (unsigned)h.epoch
              << "  frame_in_sec=" << h.frame_in_sec
              << "  len8B=" << h.frame_len_8B
              << "  raw0=0x" << std::hex << h.word0
              << " raw1=0x" << h.word1
              << " raw2=0x" << h.word2
              << " raw3=0x" << h.word3
              << std::dec << "\n";
}

//================= 快速功能测试（少量帧） =================
static void quick_test() {
    std::cout << "===== Quick Functional Test =====\n";
    hls::stream<axis_pkt> in, out;

    ap_uint<30> init_sec   = 100;
    ap_uint<6>  init_epoch = 5;

    const int TEST_FRAMES = 5;
    uint64_t input_words_sent = 0;
    uint64_t frames_emitted   = 0;

    // 第一拍送 load=1
    bool did_load = false;

    // 估计循环
    int max_cycles = TEST_FRAMES * 200;

    for (int cycle=0; cycle<max_cycles && frames_emitted < (uint64_t)TEST_FRAMES; ++cycle) {

        // 推数据：每帧 128 个 512bit
        if ((input_words_sent / DFT_FRAMES_PER_VDIF) < (uint64_t)TEST_FRAMES) {
            axis_pkt pkt;
            uint32_t frame_idx = input_words_sent / DFT_FRAMES_PER_VDIF;
            uint32_t word_idx  = input_words_sent % DFT_FRAMES_PER_VDIF;
            pkt.data = make_data_word(frame_idx, word_idx);
            pkt.keep = -1;
            pkt.strb = -1;
            pkt.last = 0;
            pkt.user = 0;
            pkt.id   = 0;
            pkt.dest = 0;
            in.write(pkt);
            input_words_sent++;
        }

        ap_uint<1> load_sig = did_load ? ap_uint<1>(0) : ap_uint<1>(1);

        opfb_packer(in, out, init_sec, init_epoch, load_sig);
        did_load = true;

        while (!out.empty()) {
            axis_pkt op = out.read();
            static bool expect_header = true;
            static int data_words_in_cur = 0;

            if (expect_header) {
                ParsedVDIF h = parse_vdif_header(op.data);
                print_header(frames_emitted, h);

                if (h.sec != init_sec) {
                    std::cerr << "[ERROR] sec mismatch (no rollover expected)\n";
                    exit(1);
                }
                if (h.frame_in_sec != frames_emitted) {
                    std::cerr << "[ERROR] frame_in_sec mismatch expected "
                              << frames_emitted << " got " << h.frame_in_sec << "\n";
                    exit(1);
                }
                if (h.frame_len_8B != 1026) {
                    std::cerr << "[ERROR] frame_len_8B != 1026\n";
                    exit(1);
                }
                expect_header = false;
                data_words_in_cur = 0;
            } else {
                data_words_in_cur++;
                if (op.last == 1) {
                    if (data_words_in_cur != 128) {
                        std::cerr << "[ERROR] data words != 128\n";
                        exit(1);
                    }
                    frames_emitted++;
                    expect_header = true;
                }
            }
        }
    }

    if (frames_emitted != (uint64_t)TEST_FRAMES) {
        std::cerr << "[ERROR] Quick test timeout\n";
        exit(1);
    }
    std::cout << "Quick test OK.\n";
}

//================= 秒翻转测试（需降低常量加速） =================
static void rollover_test() {
#ifndef RUN_SECOND_ROLLOVER_TEST
    std::cout << "Second rollover test skipped (define RUN_SECOND_ROLLOVER_TEST to enable).\n";
    return;
#else
    std::cout << "===== Second Rollover Test (DFT_FRAMES_PER_SEC=" << DFT_FRAMES_PER_SEC << ") =====\n";
    if (DFT_FRAMES_PER_SEC <= DFT_FRAMES_PER_VDIF) {
        std::cerr << "[ERROR] DFT_FRAMES_PER_SEC too small for test.\n";
        return;
    }
    // 要产生一次 rollover，需要发送 floor(DFT_FRAMES_PER_SEC / 128) + 1 个 VDIF 帧
    unsigned long long vdif_frames_per_sec = DFT_FRAMES_PER_SEC / DFT_FRAMES_PER_VDIF;
    unsigned long long frames_to_send = vdif_frames_per_sec + 1; // rollover 后第一帧

    hls::stream<axis_pkt> in, out;
    ap_uint<30> init_sec   = 200;
    ap_uint<6>  init_epoch = 7;

    unsigned long long input_words_sent = 0;
    unsigned long long frames_emitted   = 0;
    bool did_load = false;

    // 估计循环：每帧 ~ (1头 + 128数据) =>129；给富余 150
    unsigned long long max_cycles = frames_to_send * 150ULL;

    for (unsigned long long cycle=0; cycle<max_cycles && frames_emitted < frames_to_send; ++cycle) {

        if ((input_words_sent / DFT_FRAMES_PER_VDIF) < frames_to_send) {
            axis_pkt pkt;
            uint32_t frame_idx = input_words_sent / DFT_FRAMES_PER_VDIF;
            uint32_t word_idx  = input_words_sent % DFT_FRAMES_PER_VDIF;
            pkt.data = make_data_word(frame_idx, word_idx);
            pkt.keep = -1;
            pkt.strb = -1;
            pkt.last = 0;
            pkt.user = 0;
            pkt.id   = 0;
            pkt.dest = 0;
            in.write(pkt);
            input_words_sent++;
        }

        ap_uint<1> load_sig = did_load ? ap_uint<1>(0) : ap_uint<1>(1);
        opfb_packer(in, out, init_sec, init_epoch, load_sig);
        did_load = true;

        while (!out.empty()) {
            axis_pkt op = out.read();
            static bool expect_header = true;
            static int data_words_in_cur = 0;

            if (expect_header) {
                ParsedVDIF h = parse_vdif_header(op.data);

                if (frames_emitted == 0 ||
                    frames_emitted == vdif_frames_per_sec - 2 ||
                    frames_emitted == vdif_frames_per_sec - 1 ||
                    frames_emitted == vdif_frames_per_sec) {
                    print_header(frames_emitted, h);
                }

                if (frames_emitted < vdif_frames_per_sec) {
                    // rollover 前
                    if (h.sec != init_sec) {
                        std::cerr << "[ERROR] sec changed prematurely at frame "
                                  << frames_emitted << "\n";
                        exit(1);
                    }
                    if (h.frame_in_sec != frames_emitted) {
                        std::cerr << "[ERROR] frame_in_sec mismatch pre-rollover.\n";
                        exit(1);
                    }
                } else {
                    // frames_emitted == vdif_frames_per_sec （rollover 后第一帧）
                    if (h.sec != (init_sec + 1)) {
                        std::cerr << "[ERROR] sec did not increment at rollover.\n";
                        exit(1);
                    }
                    if (h.frame_in_sec != 0) {
                        std::cerr << "[ERROR] frame_in_sec not reset.\n";
                        exit(1);
                    }
                }

                expect_header = false;
                data_words_in_cur = 0;
            } else {
                data_words_in_cur++;
                if (op.last == 1) {
                    if (data_words_in_cur != 128) {
                        std::cerr << "[ERROR] data words !=128\n";
                        exit(1);
                    }
                    frames_emitted++;
                    expect_header = true;
                }
            }
        }
    }

    if (frames_emitted != frames_to_send) {
        std::cerr << "[ERROR] Rollover test timeout\n";
        exit(1);
    }
    std::cout << "Rollover test OK.\n";
#endif
}

int main() {
    quick_test();
    rollover_test();
    std::cout << "All tests finished.\n";
    return 0;
}
