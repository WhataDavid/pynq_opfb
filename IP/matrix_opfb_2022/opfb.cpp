//#include "opfb.hpp"
#include <iostream> // ������ʱ��Ч���ۺϲ�����
//// �ٶ� axis96_t.data.range(15,0) Ϊ��0����(31,16)Ϊ��1��... (95,80)Ϊ��5��
//void opfb_matix_multiple(
//    hls::stream<axis96_t> &I,
//    hls::stream<axis96_t> &Q,
//    hls::stream<axis128_t> &output,
//    int length
//) {
//#pragma HLS INTERFACE ap_ctrl_chain port=return
//#pragma HLS INTERFACE axis port=I
//#pragma HLS INTERFACE axis port=Q
//#pragma HLS INTERFACE axis port=output
//#pragma HLS INTERFACE s_axilite port=length bundle=CTRL
//#pragma HLS INTERFACE s_axilite port=return bundle=CTRL
//
//    // Ӳ����ϵ����M=8, TAPS=4
//    static sample_16 window[M][TAPS] = {
//    	    {131	,123	,115	,107},
//    	    {130	,122	,114	,106},
//    	    {129	,121	,113	,105},
//    	    {128	,120	,112	,104},
//    	    {127	,119	,111	,103},
//    	    {126	,118	,110	,102},
//    	    {125	,117	,109	,101},
//    	    {124	,116	,108	,100}
//    };
//
//
//
//#pragma HLS ARRAY_PARTITION variable=window complete dim=0
//
//    // ���ݻ���
//    static sample_16 dataI[M][TAPS] = {0};
//    static sample_16 dataQ[M][TAPS] = {0};
//#pragma HLS ARRAY_PARTITION variable=dataI complete dim=0
//#pragma HLS ARRAY_PARTITION variable=dataQ complete dim=0
//
//    int col_count = 0;
//#pragma HLS PIPELINE II=1
//    for (int i = 0; i < length; ++i) {
//        axis96_t I_tmp = I.read();
//        axis96_t Q_tmp = Q.read();
//
//        // �������ڣ���ÿ�������ƽ�ȥ
//        for (int row = 0; row < M; ++row) {
//#pragma HLS UNROLL
//            // ����������һ��
//            for (int c = 0; c < TAPS-1; ++c) {
//                dataI[row][c] = dataI[row][c+1];
//                dataQ[row][c] = dataQ[row][c+1];
//            }
//            // ���һ���������ݣ����ݲ���Mʱ��0��
////            if (row < 6) {
////                dataI[row][TAPS-1] = I_tmp.data.range(16*(row+1)-1, 16*row);
////                dataQ[row][TAPS-1] = Q_tmp.data.range(16*(row+1)-1, 16*row);
////            } else {
////                dataI[row][TAPS-1] = 0;
////                dataQ[row][TAPS-1] = 0;
////            }
//            // ���һ����������
//                        if (row < 6) {
//                            dataI[row][TAPS-1] = I_tmp.data.range(16*(row+1)-1, 16*row);
//                            dataQ[row][TAPS-1] = Q_tmp.data.range(16*(row+1)-1, 16*row);
//                        } else if (row == 6) {
//                            dataI[row][TAPS-1] = I_tmp.data.range(16*5-1, 16*4); // �õ�4������
//                            dataQ[row][TAPS-1] = Q_tmp.data.range(16*5-1, 16*4);
//                        } else if (row == 7) {
//                            dataI[row][TAPS-1] = I_tmp.data.range(16*6-1, 16*5); // �õ�5������
//                            dataQ[row][TAPS-1] = Q_tmp.data.range(16*6-1, 16*5);
//                        }
//        }
//
//        col_count++;
//
//        // ֻ������TAPS�в������
//        if (col_count >= TAPS) {
//            axis128_t out;
//            out.data = 0;
//            for (int row = 0; row < M; ++row) {
//#pragma HLS UNROLL
//                ap_int<32> sum = 0;
//                for (int c = 0; c < TAPS; ++c) {
//#pragma HLS UNROLL
//                    // ��ӦԪ�س�
//                    ap_int<32> prodI = dataI[row][c] * window[row][c];
//                    ap_int<32> prodQ = dataQ[row][c] * window[row][c];
//                	std::cout << "row=" << row << ", c=" << c
//                	          << " | I=" << dataI[row][c]
//                	          << " x win=" << window[row][c]
//                	          << " = " << prodI
//                	          << " | Q=" << dataQ[row][c]
//                	          << " x win=" << window[row][c]
//                	          << " = " << prodQ << std::endl;
//
//                    sum += prodI ; // �ɷֿ����I��Q
//                }
//                // ���Ϊÿ�еĺͣ��������out.data
//                out.data.range(16*(row+1)-1, 16*row) = sum;
//            }
////            out.last = (i == length-1) ? 1 : 0;
//            out.last = (((i+1) % length) == 0) ? 1 : 0;
//            output.write(out);
//        }
//    }
//}

//length�޷����ƣ�tlast����������
//#include "opfb.hpp"
//
//// �ٶ� axis96_t.data.range(15,0) Ϊ��0����(31,16)Ϊ��1��... (95,80)Ϊ��5��
//void opfb_matix_multiple(
//    hls::stream<axis96_t> &I,
//    hls::stream<axis96_t> &Q,
//    hls::stream<axis128_t> &output,
//	ap_int<20> length
//) {
//#pragma HLS INTERFACE ap_ctrl_chain port=return
//#pragma HLS INTERFACE axis port=I
//#pragma HLS INTERFACE axis port=Q
//#pragma HLS INTERFACE axis port=output
//#pragma HLS INTERFACE s_axilite port=length bundle=CTRL
//#pragma HLS INTERFACE s_axilite port=return bundle=CTRL
//
//    // Ӳ����ϵ����M=8, TAPS=4
//    static ap_int<10> window[M][TAPS] = {
//        {131,123,115,107},
//        {130,122,114,106},
//        {129,121,113,105},
//        {128,120,112,104},
//        {127,119,111,103},
//        {126,118,110,102},
//        {125,117,109,101},
//        {124,116,108,100}
//    };
//#pragma HLS ARRAY_PARTITION variable=window complete dim=0
//
//    // ���ݻ���
//    static sample_16 dataI[M][TAPS] = {0};
//    static sample_16 dataQ[M][TAPS] = {0};
//#pragma HLS ARRAY_PARTITION variable=dataI complete dim=0
//#pragma HLS ARRAY_PARTITION variable=dataQ complete dim=0
//
//    static ap_int<20> col_count = 0;
//    static ap_int<20> i = 0;
//
//    while (1) {
//#pragma HLS PIPELINE II=1
//#ifndef __SYNTHESIS__
//        // ������ڣ���ֹ��ѭ��
//        if (I.empty() || Q.empty()) break;
//#endif
//        axis96_t I_tmp = I.read();
//        axis96_t Q_tmp = Q.read();
//
//        // �������ڣ���ÿ�������ƽ�ȥ
//        for (ap_uint<4> row = 0; row < M; ++row) {
//#pragma HLS UNROLL
//            // ����������һ��
//            for (ap_uint<4> c = 0; c < TAPS-1; ++c) {
//                dataI[row][c] = dataI[row][c+1];
//                dataQ[row][c] = dataQ[row][c+1];
//            }
//            // ���һ����������
//            if (row < 6) {
//                dataI[row][TAPS-1] = I_tmp.data.range(16*(row+1)-1, 16*row);
//                dataQ[row][TAPS-1] = Q_tmp.data.range(16*(row+1)-1, 16*row);
//            } else if (row == 6) {
//                dataI[row][TAPS-1] = I_tmp.data.range(16*5-1, 16*4); // �õ�4������
//                dataQ[row][TAPS-1] = Q_tmp.data.range(16*5-1, 16*4);
//            } else if (row == 7) {
//                dataI[row][TAPS-1] = I_tmp.data.range(16*6-1, 16*5); // �õ�5������
//                dataQ[row][TAPS-1] = Q_tmp.data.range(16*6-1, 16*5);
//            }
//        }
//
//        col_count++;
//        i++;
//
//        // ֻ������TAPS�в������
//        if (col_count >= TAPS) {
//            axis128_t out;
//            out.data = 0;
//            for (ap_uint<4> row = 0; row < M; ++row) {
//#pragma HLS UNROLL
//                ap_uint<24> sum = 0;
//                for (ap_uint<4> c = 0; c < TAPS; ++c) {
//#pragma HLS UNROLL
////                    ap_uint<16> prodI = dataI[row][c] * window[row][c];
//                    ap_uint<16> prodI = window[row][c]-100;
////                    ap_int<32> prodQ = dataQ[row][c] * window[row][c];
//
////                    std::cout << "row=" << row << ", c=" << c
////                                    	          << " | I=" << dataI[row][c]
////                                    	          << " x win=" << window[row][c]
////                                    	          << " = " << prodI
////                                    	          << " | Q=" << dataQ[row][c]
////                                    	          << " x win=" << window[row][c]
////                                    	          << " = " << prodQ << std::endl;
//
//                    sum += prodI; // ��������I/Q���ɷֿ����
////                    sum += prodQ;
//                }
//                out.data.range(16*(row+1)-1, 16*row) = sum;
////                std::cout <<std::endl;
//            }
////            std::cout <<"----------------------------------------------"<<std::endl;
//
//            // ÿlength���������һ��last=1����֡
//            out.last = (i == length) ? 1 : 0;
//            output.write(out);
//
//            // ÿlength���һ��last=1�����¼�����������һ֡
//            if (i == length) {
//                i = 0;
//            }
//        }
//    }
//}
//
//

//length�޷����ƣ�tlast����������
//#include "opfb.hpp"
//
//// �ٶ� axis96_t.data.range(15,0) Ϊ��0����(31,16)Ϊ��1��... (95,80)Ϊ��5��
//void opfb_matix_multiple(
//    hls::stream<axis96_t> &I,
//    hls::stream<axis96_t> &Q,
//    hls::stream<axis128_t> &output
//) {
//#pragma HLS INTERFACE ap_ctrl_chain port=return
//#pragma HLS INTERFACE axis port=I
//#pragma HLS INTERFACE axis port=Q
//#pragma HLS INTERFACE axis port=output
////#pragma HLS INTERFACE s_axilite port=length bundle=CTRL
//#pragma HLS INTERFACE s_axilite port=return bundle=CTRL
//
//    // Ӳ����ϵ����M=8, TAPS=4
//    static ap_int<7> window[M][TAPS] = {
//        {31,23,15,7},
//        {30,22,14,6},
//        {29,21,13,5},
//        {28,20,12,4},
//        {27,19,11,3},
//        {26,18,10,2},
//        {25,17,9,1},
//        {24,16,8,0}
//    };
//#pragma HLS ARRAY_PARTITION variable=window complete dim=0
//
//    // ���ݻ���
//    static sample_16 dataI[M][TAPS] = {0};
//    static sample_16 dataQ[M][TAPS] = {0};
//#pragma HLS ARRAY_PARTITION variable=dataI complete dim=0
//#pragma HLS ARRAY_PARTITION variable=dataQ complete dim=0
//
//    static ap_int<20> col_count = 0;
//    static ap_int<20> i = 0;
//
//    while (1) {
//#pragma HLS PIPELINE II=1
//#ifndef __SYNTHESIS__
//        // ������ڣ���ֹ��ѭ��
//        if (I.empty() || Q.empty()) break;
//#endif
//        axis96_t I_tmp = I.read();
//        axis96_t Q_tmp = Q.read();
//
//        // �������ڣ���ÿ�������ƽ�ȥ
//        for (ap_uint<4> row = 0; row < M; ++row) {
//#pragma HLS UNROLL
//            // ����������һ��
//            for (ap_uint<4> c = 0; c < TAPS-1; ++c) {
//                dataI[row][c] = dataI[row][c+1];
////                dataQ[row][c] = dataQ[row][c+1];
//            }
//            // ���һ����������
//            if (row < 6) {
//                dataI[row][TAPS-1] = I_tmp.data.range(16*(row+1)-1, 16*row);
////                dataQ[row][TAPS-1] = Q_tmp.data.range(16*(row+1)-1, 16*row);
//            } else if (row == 6) {
////                dataI[row][TAPS-1] = I_tmp.data.range(16*5-1, 16*4); // �õ�4������
//                dataI[row][TAPS-1] = 1;
////                dataQ[row][TAPS-1] = Q_tmp.data.range(16*5-1, 16*4);
//            } else if (row == 7) {
////                dataI[row][TAPS-1] = I_tmp.data.range(16*6-1, 16*5); // �õ�5������
//                dataI[row][TAPS-1] = 1;
////                dataQ[row][TAPS-1] = Q_tmp.data.range(16*6-1, 16*5);
//            }
//        }
//
//        col_count++;
//        i++;
//
//        // ֻ������TAPS�в������
//        if (col_count >= TAPS) {
//            axis128_t out;
//            out.data = 0;
//            for (ap_uint<4> row = 0; row < M; ++row) {
//#pragma HLS UNROLL
//                ap_int<32> sum = 0;
//                for (ap_uint<4> c = 0; c < TAPS; ++c) {
//#pragma HLS UNROLL
////                    ap_uint<16> prodI = dataI[row][c] * window[row][c];
//                    ap_int<16> prodI = dataI[row][c];
////                	  ap_uint<16> prodI =dataI[row][c];
////                    ap_int<32> prodQ = dataQ[row][c] * window[row][c];
//
////                    std::cout << "row=" << row << ", c=" << c
////                                    	          << " | I=" << dataI[row][c]
////                                    	          << " x win=" << window[row][c]
////                                    	          << " = " << prodI
////                                    	          << " | Q=" << dataQ[row][c]
////                                    	          << " x win=" << window[row][c]
////                                    	          << " = " << prodQ << std::endl;
//
//                    sum += prodI; // ��������I/Q���ɷֿ����
////                    sum = dataI[row][c];
////                    sum += prodQ;
//                }
//                out.data.range(16*(row+1)-1, 16*row) = sum;
////                std::cout <<std::endl;
//            }
////            std::cout <<"----------------------------------------------"<<std::endl;
//
//            // ÿlength���������һ��last=1����֡
//            out.last = (i == length) ? 1 : 0;
//            output.write(out);
//
//            // ÿlength���һ��last=1�����¼�����������һ֡
//            if (i == length) {
//                i = 0;
//            }
//        }
//    }
//}

//�ָ���û�ɹ�
//#include "opfb.hpp"
//// �ٶ� axis96_t.data.range(15,0) Ϊ��0����(31,16)Ϊ��1��... (95,80)Ϊ��5��
//void opfb_matix_multiple(
//    hls::stream<axis96_t> &I,
//    hls::stream<axis96_t> &Q,
//    hls::stream<axis128_t> &output
//) {
//#pragma HLS INTERFACE ap_ctrl_chain port=return
//#pragma HLS INTERFACE axis port=I
//#pragma HLS INTERFACE axis port=Q
//#pragma HLS INTERFACE axis port=output
////#pragma HLS INTERFACE s_axilite port=length bundle=CTRL
//#pragma HLS INTERFACE s_axilite port=return bundle=CTRL
//
//    // Ӳ����ϵ����M=8, TAPS=4
//    static ap_int<10> window[M][TAPS] = {
//        {131,123,115,107},
//        {130,122,114,106},
//        {129,121,113,105},
//        {128,120,112,104},
//        {127,119,111,103},
//        {126,118,110,102},
//        {125,117,109,101},
//        {124,116,108,100}
//    };
//#pragma HLS ARRAY_PARTITION variable=window complete dim=0
//
//    // ���ݻ���
//    static sample_16 dataI[M][TAPS] = {0};
//    static sample_16 dataQ[M][TAPS] = {0};
//#pragma HLS ARRAY_PARTITION variable=dataI complete dim=0
//#pragma HLS ARRAY_PARTITION variable=dataQ complete dim=0
//
//    static ap_int<20> col_count = 0;
//    ap_int<20> i = 0;
//
//    while (1) {
//#pragma HLS PIPELINE II=1
//#ifndef __SYNTHESIS__
//        // ������ڣ���ֹ��ѭ��
//        if (I.empty() || Q.empty()) break;
//#endif
//        axis96_t I_tmp = I.read();
//        axis96_t Q_tmp = Q.read();
//
//        // �������ڣ���ÿ�������ƽ�ȥ
//        for (ap_uint<4> row = 0; row < M; ++row) {
//#pragma HLS UNROLL
//            // ����������һ��
//            for (ap_uint<4> c = 0; c < TAPS-1; ++c) {
//                dataI[row][c] = dataI[row][c+1];
//                dataQ[row][c] = dataQ[row][c+1];
//            }
//            // ���һ����������
//            if (row < 6) {
//                dataI[row][TAPS-1] = I_tmp.data.range(16*(row+1)-1, 16*row);
//                dataQ[row][TAPS-1] = Q_tmp.data.range(16*(row+1)-1, 16*row);
//            } else if (row == 6) {
//                dataI[row][TAPS-1] = I_tmp.data.range(16*5-1, 16*4); // �õ�4������
//                dataQ[row][TAPS-1] = Q_tmp.data.range(16*5-1, 16*4);
//            } else if (row == 7) {
//                dataI[row][TAPS-1] = I_tmp.data.range(16*6-1, 16*5); // �õ�5������
//                dataQ[row][TAPS-1] = Q_tmp.data.range(16*6-1, 16*5);
//            }
//        }
//
//        col_count++;
//        i++;
//
//        // ֻ������TAPS�в������
//        if (col_count >= TAPS) {
//            axis128_t out;
//            out.data = 0;
//            for (ap_uint<4> row = 0; row < M; ++row) {
//#pragma HLS UNROLL
//                ap_uint<24> sum = 0;
//                for (ap_uint<4> c = 0; c < TAPS; ++c) {
//#pragma HLS UNROLL
//                    ap_uint<16> prodI = dataI[row][c] * window[row][c];
////                    ap_int<32> prodQ = dataQ[row][c] * window[row][c];
//
////                    std::cout << "row=" << row << ", c=" << c
////                                    	          << " | I=" << dataI[row][c]
////                                    	          << " x win=" << window[row][c]
////                                    	          << " = " << prodI
////                                    	          << " | Q=" << dataQ[row][c]
////                                    	          << " x win=" << window[row][c]
////                                    	          << " = " << prodQ << std::endl;
//
//                    sum += prodI; // ��������I/Q���ɷֿ����
//                }
////                out.data.range(16*(row+1)-1, 16*row) = sum;
//                out.data.range(16*(row+1)-1, 16*row) = dataI[row][0];
////                std::cout <<std::endl;
//            }
////            std::cout <<"----------------------------------------------"<<std::endl;
//
//            // ÿlength���������һ��last=1����֡
//            out.last = (i == length) ? 1 : 0;
//            output.write(out);
//
//            // ÿlength���һ��last=1�����¼�����������һ֡
//            if (i == length) {
//                i = 0;
//            }
//        }
//    }
//}

////ȥ��q
//#include "opfb.hpp"
//// �ٶ� axis96_t.data.range(15,0) Ϊ��0����(31,16)Ϊ��1��... (95,80)Ϊ��5��
//void opfb_matix_multiple(
//    hls::stream<axis96_t> &I,
////    hls::stream<axis96_t> &Q,
//    hls::stream<axis128_t> &output
//) {
//#pragma HLS INTERFACE ap_ctrl_chain port=return
//#pragma HLS INTERFACE axis port=I
////#pragma HLS INTERFACE axis port=Q
//#pragma HLS INTERFACE axis port=output
////#pragma HLS INTERFACE s_axilite port=length bundle=CTRL
//#pragma HLS INTERFACE s_axilite port=return bundle=CTRL
//
//    // Ӳ����ϵ����M=8, TAPS=4
//    static ap_int<10> window[M][TAPS] = {
//        {131,123,115,107},
//        {130,122,114,106},
//        {129,121,113,105},
//        {128,120,112,104},
//        {127,119,111,103},
//        {126,118,110,102},
//        {125,117,109,101},
//        {124,116,108,100}
//    };
//#pragma HLS ARRAY_PARTITION variable=window complete dim=0
//
//    // ���ݻ���
//    static sample_16 dataI[M][TAPS] = {0};
//    static sample_16 dataQ[M][TAPS] = {0};
//#pragma HLS ARRAY_PARTITION variable=dataI complete dim=0
////#pragma HLS ARRAY_PARTITION variable=dataQ complete dim=0
//
//    static ap_int<20> col_count = 0;
//    ap_int<20> i = 0;
//
//    while (1) {
//#pragma HLS PIPELINE II=1
//#ifndef __SYNTHESIS__
//        // ������ڣ���ֹ��ѭ��
//        if (I.empty() ) break;
//#endif
//        axis96_t I_tmp = I.read();
////        axis96_t Q_tmp = Q.read();
//
//        // �������ڣ���ÿ�������ƽ�ȥ
//        for (ap_uint<4> row = 0; row < M; ++row) {
//#pragma HLS UNROLL
//            // ����������һ��
//            for (ap_uint<4> c = 0; c < TAPS-1; ++c) {
//                dataI[row][c] = dataI[row][c+1];
////                dataQ[row][c] = dataQ[row][c+1];
//            }
//            // ���һ����������
//            if (row < 6) {
//                dataI[row][TAPS-1] = I_tmp.data.range(16*(row+1)-1, 16*row);
////                dataQ[row][TAPS-1] = Q_tmp.data.range(16*(row+1)-1, 16*row);
//            } else if (row == 6) {
//                dataI[row][TAPS-1] = I_tmp.data.range(16*5-1, 16*4); // �õ�4������
////                dataQ[row][TAPS-1] = Q_tmp.data.range(16*5-1, 16*4);
//            } else if (row == 7) {
//                dataI[row][TAPS-1] = I_tmp.data.range(16*6-1, 16*5); // �õ�5������
////                dataQ[row][TAPS-1] = Q_tmp.data.range(16*6-1, 16*5);
//            }
//        }
//
//        col_count++;
//        i++;
//
//        // ֻ������TAPS�в������
//        if (col_count >= TAPS) {
//            axis128_t out;
//            out.data = 0;
//            for (ap_uint<4> row = 0; row < M; ++row) {
//#pragma HLS UNROLL
//                ap_uint<24> sum = 0;
//                for (ap_uint<4> c = 0; c < TAPS; ++c) {
//#pragma HLS UNROLL
//                    ap_uint<16> prodI = dataI[row][c] * window[row][c];
////                    ap_int<32> prodQ = dataQ[row][c] * window[row][c];
//
////                    std::cout << "row=" << row << ", c=" << c
////                                    	          << " | I=" << dataI[row][c]
////                                    	          << " x win=" << window[row][c]
////                                    	          << " = " << prodI
////                                    	          << " | Q=" << dataQ[row][c]
////                                    	          << " x win=" << window[row][c]
////                                    	          << " = " << prodQ << std::endl;
//
//                    sum += prodI; // ��������I/Q���ɷֿ����
//                }
////                out.data.range(16*(row+1)-1, 16*row) = sum;
//                out.data.range(16*(row+1)-1, 16*row) = dataI[row][0];
////                std::cout <<std::endl;
//            }
////            std::cout <<"----------------------------------------------"<<std::endl;
//
//            // ÿlength���������һ��last=1����֡
//            out.last = (i == length) ? 1 : 0;
//            output.write(out);
//
//            // ÿlength���һ��last=1�����¼�����������һ֡
//            if (i == length) {
//                i = 0;
//            }
//        }
//    }
//}


//�����adc����
#include "opfb.hpp"
void opfb_matix_multiple(hls::stream<axis96_t> &I,
		hls::stream<axis128_t> &output) {
//#pragma HLS INTERFACE ap_ctrl_chain port=return
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=I
#pragma HLS INTERFACE axis port=output
//#pragma HLS INTERFACE s_axilite port=return bundle=CTRL

	static sample_16 dataI[M][TAPS + 1] = { 0 };
#pragma HLS ARRAY_PARTITION variable=dataI complete dim=0

	while (1) {
#pragma HLS PIPELINE II=1
#ifndef __SYNTHESIS__
		if (I.empty())
			break;
#endif
		axis96_t I_tmp = I.read();

		axis128_t out;
		out.data = 0;

		// ���������ƽ�
		for (ap_uint<4> row = 0; row < M; ++row) {
#pragma HLS UNROLL

			// չ��д����ȥ��if�ж�
			dataI[0][TAPS] = I_tmp.data.range(15, 0);
			dataI[1][TAPS] = I_tmp.data.range(31, 16);
			dataI[2][TAPS] = I_tmp.data.range(47, 32);
			dataI[3][TAPS] = I_tmp.data.range(63, 48);
			dataI[4][TAPS] = I_tmp.data.range(79, 64);
			dataI[5][TAPS] = I_tmp.data.range(95, 80);
			dataI[6][TAPS] = I_tmp.data.range(79, 64);  // �õ�4������
			dataI[7][TAPS] = I_tmp.data.range(95, 80);  // �õ�5������

			for (ap_uint<4> c = 0; c < TAPS; ++c) {
				dataI[row][c] = dataI[row][c + 1];
			}

			out.data.range(16 * (row + 1) - 1, 16 * row) = dataI[row][0];
//			std::cout<<"dataI[row][TAPS-1]:"<<dataI[row][TAPS-1]<<"\n";
		}
		out.last = 0;
		output.write(out);
		std::cout << "\n";
	}
}
