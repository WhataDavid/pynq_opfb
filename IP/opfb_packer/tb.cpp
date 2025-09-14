//#include "opfb_packer.h"
//#include <iostream>
//
//int main() {
//    hls::stream<axis_pkt> in, out;
//
//    // ��������payload
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
//    // ���ô��⺯��
//    opfb_packer(in, out,0,0);
//
//    // ��ȡ�������ʾ
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

//////////////////////////////////���溬1pps
//#include "opfb_packer.h"
//#include <hls_stream.h>
//#include <ap_int.h>
//#include <iostream>
//#include <iomanip>
//
//#define TB_TOTAL_FRAMES 3      // ����֡��
//#define TB_PPS_PERIOD   150    // 1PPS��������(�����������豣֤ÿ֡��������PPS)
//#define TB_INPUT_WORDS  DATA_WORDS
//
//// ��ӡVDIFͷ�����֡�ţ�С��ƴ�ӣ�
//void print_vdif_header(const axis_pkt& pkt) {
//    ap_uint<512> wd = pkt.data;
//    // VDIFͷ��offset=42, ����16�ֽ�
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
//    // 1. ׼��������������ÿ֡128�ģ�ÿ��64�ֽ�
//    for (int f = 0; f < TB_TOTAL_FRAMES; ++f) {
//        for (int w = 0; w < TB_INPUT_WORDS; ++w) {
//            axis_pkt pkt;
//            for (int b = 0; b < 64; ++b)
//                pkt.data.range((b+1)*8-1, b*8) = (ap_uint<8>)(f+1); // �������ݿ��Զ���
//            pkt.keep = (ap_uint<64>)(-1);
//            pkt.last = (w == TB_INPUT_WORDS-1) ? 1 : 0;
//            in.write(pkt);
//        }
//    }
//
//    // 2. AXI-Lite������ʼ��
//    ap_uint<30> axi_sec   = 10000;   // ��ʼ��
//    ap_uint<6>  axi_epoch = 31;      // ��ʼepoch
//    bool sma_1pps = 0;
//
//    // 3. ������ʱ������������֤���������������ݺͶ��PPS���ڣ�
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
//    // ������ѭ��
//    while (main_clk < total_cycles || !out.empty()) {
//        // 1PPS����ģ�⣺ÿTB_PPS_PERIOD������1��
//        sma_1pps = (pps_cnt == 0);
//        pps_cnt = (pps_cnt + 1) % TB_PPS_PERIOD;
//
//        // ���� DUT
//        opfb_packer(in, out, axi_sec, axi_epoch, sma_1pps,sec_now,epoch_now,frame_in_sec);
//
//        // ��ȡ����ӡ���
//        while (!out.empty()) {
//            axis_pkt pkt = out.read();
//
//            // ��ӡ����VDIFͷ
//            if (pkt.keep != (ap_uint<64>)(-1)) {
//                std::cout << "[output frame " << frame_count << "] ";
//                print_vdif_header(pkt);
//                frame_count++;
//            }
//            // �ɼӸ������ݴ�ӡ/У��
//        }
//
//        ++main_clk;
//    }
//
//    std::cout << "Testbench finished. Total output frames: " << frame_count << std::endl;
//    return 0;
//}

////////////////////////////////////////���溬��sec_now_out��epoch_now_out��frame_in_sec_out
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
//    // 1. ׼������������
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
//    // �����ж�sma_1pps��ʱ����
//    int pps_frame = 3; // ÿ3֡����һ��
//    int pps_start = -1; // �����������
//
//    // ������ѭ��������ģ�⣩
//    while (output_frame < TB_TOTAL_FRAMES || !out.empty()) {
//        // �ж��Ƿ����1PPS�ߵ�ƽ��
//        if (output_frame >= pps_frame && output_frame < pps_frame + 1) {
//            // ��֡�������Ķ�����
//            sma_1pps = 1;
//        } else {
//            sma_1pps = 0;
//        }
//
//        opfb_packer(in, out, axi_sec, axi_epoch);
//
//        // ��ȡ����ӡ���
//        while (!out.empty()) {
//            axis_pkt pkt = out.read();
//            if (pkt.keep != (ap_uint<64>)(-1)) {
//                std::cout << "[output frame " << output_frame << "] ";
//                print_vdif_header(pkt);
//                output_frame++;
//                // 1PPS�������һ�����
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

// ��Ҫ���ٲ����뷭ת�����ڱ��� testbench ʱ��ӣ�
// g++ ... -DDFT_FRAMES_PER_SEC=1280
// ����֤�� DUT һ�£��ú��� opfb_packer.cpp �б����ã�
//
// Ĭ�Ϻ꣨296000000�������뷭ת������Ҫ�ܶ�֡����ʱ�޴�����Ĭ�Ϲرճ����ԡ�
//
// ȡ����һ��ע�������á��뷭ת���ԡ� (��Ҫ�� DFT_FRAMES_PER_SEC ��С�� 1280):
 #define RUN_SECOND_ROLLOVER_TEST

#ifndef DFT_FRAMES_PER_SEC
#define DFT_FRAMES_PER_SEC 1280ULL
#endif

#ifndef DFT_FRAMES_PER_VDIF
#define DFT_FRAMES_PER_VDIF 128
#endif

//================= ����һ��α���ݣ�����ʶ�� =================
static ap_uint<512> make_data_word(uint32_t frame_idx, uint32_t word_idx_in_frame) {
    ap_uint<512> w = 0;
    for (int lane=0; lane<16; ++lane) {
        uint32_t val = 0;
        val |= (0xA5u << 24);                 // ����
        val |= ((frame_idx & 0xFF) << 8);     // �м��ֽڷ�֡�ŵ�8λ
        val |= (lane & 0xFF);                 // ��8λ�� lane
        // ���� 32bit lane
        w.range(32*(lane+1)-1, 32*lane) = val;
    }
    // Ҳ�������� word_idx_in_frame����ʾ�㹻�Ͳ���չ��
    return w;
}

//================= ���� VDIF ͷ =================
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

//================= ���ٹ��ܲ��ԣ�����֡�� =================
static void quick_test() {
    std::cout << "===== Quick Functional Test =====\n";
    hls::stream<axis_pkt> in, out;

    ap_uint<30> init_sec   = 100;
    ap_uint<6>  init_epoch = 5;

    const int TEST_FRAMES = 5;
    uint64_t input_words_sent = 0;
    uint64_t frames_emitted   = 0;

    // ��һ���� load=1
    bool did_load = false;

    // ����ѭ��
    int max_cycles = TEST_FRAMES * 200;

    for (int cycle=0; cycle<max_cycles && frames_emitted < (uint64_t)TEST_FRAMES; ++cycle) {

        // �����ݣ�ÿ֡ 128 �� 512bit
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

//================= �뷭ת���ԣ��轵�ͳ������٣� =================
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
    // Ҫ����һ�� rollover����Ҫ���� floor(DFT_FRAMES_PER_SEC / 128) + 1 �� VDIF ֡
    unsigned long long vdif_frames_per_sec = DFT_FRAMES_PER_SEC / DFT_FRAMES_PER_VDIF;
    unsigned long long frames_to_send = vdif_frames_per_sec + 1; // rollover ���һ֡

    hls::stream<axis_pkt> in, out;
    ap_uint<30> init_sec   = 200;
    ap_uint<6>  init_epoch = 7;

    unsigned long long input_words_sent = 0;
    unsigned long long frames_emitted   = 0;
    bool did_load = false;

    // ����ѭ����ÿ֡ ~ (1ͷ + 128����) =>129�������� 150
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
                    // rollover ǰ
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
                    // frames_emitted == vdif_frames_per_sec ��rollover ���һ֡��
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
