//#include "opfb.hpp"
//#include <iostream> // ������ʱ��Ч���ۺϲ�����
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

////�����adc����,ʱ��fmax��ߵ���2Ghz��ȥ��axi�������ߺ�����ȷ
//#include "opfb.hpp"
//void opfb_matix_multiple(hls::stream<axis96_t> &I,
//		hls::stream<axis128_t> &output) {
////#pragma HLS INTERFACE ap_ctrl_chain port=return
//#pragma HLS INTERFACE ap_ctrl_none port=return
//#pragma HLS INTERFACE axis port=I
//#pragma HLS INTERFACE axis port=output
////#pragma HLS INTERFACE s_axilite port=return bundle=CTRL
//
//	static sample_16 dataI[M][TAPS + 1] = { 0 };
//#pragma HLS ARRAY_PARTITION variable=dataI complete dim=0
//
//	while (1) {
//#pragma HLS PIPELINE II=1
//#ifndef __SYNTHESIS__
//		if (I.empty())
//			break;
//#endif
//		axis96_t I_tmp = I.read();
//
//		axis128_t out;
//		out.data = 0;
//
//		// ���������ƽ�
//		for (ap_uint<4> row = 0; row < M; ++row) {
//#pragma HLS UNROLL
//
//			// չ��д����ȥ��if�ж�
//			dataI[0][TAPS] = I_tmp.data.range(15, 0);
//			dataI[1][TAPS] = I_tmp.data.range(31, 16);
//			dataI[2][TAPS] = I_tmp.data.range(47, 32);
//			dataI[3][TAPS] = I_tmp.data.range(63, 48);
//			dataI[4][TAPS] = I_tmp.data.range(79, 64);
//			dataI[5][TAPS] = I_tmp.data.range(95, 80);
//			dataI[6][TAPS] = I_tmp.data.range(79, 64);  // �õ�4������
//			dataI[7][TAPS] = I_tmp.data.range(95, 80);  // �õ�5������
//
//			for (ap_uint<4> c = 0; c < TAPS; ++c) {
//				dataI[row][c] = dataI[row][c + 1];
//			}
//
//			out.data.range(16 * (row + 1) - 1, 16 * row) = dataI[row][0];
////			std::cout<<"dataI[row][TAPS-1]:"<<dataI[row][TAPS-1]<<"\n";
//		}
//		out.last = 0;
//		output.write(out);
//		std::cout << "\n";
//	}
//}

////���������
//#include "opfb.hpp"
//void opfb_matix_multiple(hls::stream<axis96_t> &I,
//		hls::stream<axis128_t> &output) {
////#pragma HLS INTERFACE ap_ctrl_chain port=return
//#pragma HLS INTERFACE ap_ctrl_none port=return
//#pragma HLS INTERFACE axis port=I
//#pragma HLS INTERFACE axis port=output
////#pragma HLS INTERFACE s_axilite port=return bundle=CTRL
//
//// Ӳ����ϵ����M=8, TAPS=4
//	static ap_int<10> window[M][TAPS] = {
//			{ 131, 123, 115, 107 },
//			{ 130, 122, 114, 106 },
//			{ 129, 121, 113, 105 },
//			{ 128, 120, 112, 104 },
//			{ 127, 119, 111, 103 },
//			{ 126, 118, 110, 102 },
//			{ 125, 117, 109, 101 },
//			{ 124, 116, 108, 100 } };
//
//	static sample_16 dataI[M][TAPS + 1] = { 0 };
//#pragma HLS ARRAY_PARTITION variable=dataI complete dim=0
//
//	while (1) {
//#pragma HLS PIPELINE II=1
//#ifndef __SYNTHESIS__
//		if (I.empty())
//			break;
//#endif
//		axis96_t I_tmp = I.read();
//
//		axis128_t out;
//		out.data = 0;
//
//		// ���������ƽ�
//		for (ap_uint<4> row = 0; row < M; ++row) {
//#pragma HLS UNROLL
//			int sum = 0;
//			// չ��д����ȥ��if�ж�
//			dataI[0][TAPS] = I_tmp.data.range(15, 0);
//			dataI[1][TAPS] = I_tmp.data.range(31, 16);
//			dataI[2][TAPS] = I_tmp.data.range(47, 32);
//			dataI[3][TAPS] = I_tmp.data.range(63, 48);
//			dataI[4][TAPS] = I_tmp.data.range(79, 64);
//			dataI[5][TAPS] = I_tmp.data.range(95, 80);
//			dataI[6][TAPS] = I_tmp.data.range(79, 64);  // �õ�4������
//			dataI[7][TAPS] = I_tmp.data.range(95, 80);  // �õ�5������
//
//			for (ap_uint<4> c = 0; c < TAPS; ++c) {
//#pragma HLS UNROLL
//				dataI[row][c] = dataI[row][c + 1];
//
////				ap_int<16> prodI = dataI[row][c] * window[row][c];
//
//				sum += dataI[row][c] * window[row][c];
//
//
////				std::cout << "row=" << row << ", c=" << c << " | I="
////						<< dataI[row][c] << " x win=" << window[row][c] << " = "
////						<< prodI << std::endl;
//
//
////				sum += prodI;
//			}
//			out.data.range(16 * (row + 1) - 1, 16 * row) = sum;
////			std::cout << "sum:" << sum<< "\n";
//
////			out.data.range(16 * (row + 1) - 1, 16 * row) = dataI[row][0];
////			std::cout<<"dataI[row][TAPS-1]:"<<dataI[row][TAPS-1]<<"\n";
//		}
//		out.last = 0;
//		output.write(out);
//		std::cout << "\n";
//	}
//}

//��Ϊһά���� ��ȷ 2025-7-3
//#include "opfb.hpp"
//void opfb_matix_multiple(hls::stream<axis96_t> &I,
//		hls::stream<axis256_t> &output) {
////#pragma HLS INTERFACE ap_ctrl_chain port=return
//#pragma HLS INTERFACE ap_ctrl_none port=return
//#pragma HLS INTERFACE axis port=I
//#pragma HLS INTERFACE axis port=output
////#pragma HLS INTERFACE s_axilite port=return bundle=CTRL
//
//// Ӳ����ϵ����M=8, TAPS=4
//	static ap_int<10> window[M][TAPS] = {
//			{ 131, 123, 115, 107 },
//			{ 130, 122, 114, 106 },
//			{ 129, 121, 113, 105 },
//			{ 128, 120, 112, 104 },
//			{ 127, 119, 111, 103 },
//			{ 126, 118, 110, 102 },
//			{ 125, 117, 109, 101 },
//			{ 124, 116, 108, 100 } };
//
//	static ap_int<10> coe[DATA_NUM] = {100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131};
//
//	static sample_16 dataI[M][TAPS + 1] = { 0 };
//	static sample_16 data[M * TAPS] = { 0 };
//#pragma HLS ARRAY_PARTITION variable=dataI complete dim=0
//
//	static ap_int<16> pre0 = 0;
//	static ap_int<16> pre1 = 0;
//
//	while (1) {
//#pragma HLS PIPELINE II=1
//#ifndef __SYNTHESIS__
//		if (I.empty())
//			break;
//#endif
//		axis96_t I_tmp = I.read();
//		axis256_t out;
//		out.data = 0;
//
//		// һά���������ƽ�
////		std::cout << "start process data\n";
//		for (ap_uint<8> ite = DATA_NUM - 1; ite >= M; --ite) {
////			std::cout << "data[" << ite << "]=data[" << ite - D << "]="
////					<< data[ite - D] << "\n";
//			data[ite] = data[ite - D];
//		}
//		data[7] = pre0;
//		pre0 = I_tmp.data.range(79, 64);
//		data[6] = pre1;
//		pre1 = I_tmp.data.range(95, 80);
//
//		data[5] = I_tmp.data.range(15, 0);
//		data[4] = I_tmp.data.range(31, 16);
//		data[3] = I_tmp.data.range(47, 32);
//		data[2] = I_tmp.data.range(63, 48);
//		data[1] = I_tmp.data.range(79, 64);
//		data[0] = I_tmp.data.range(95, 80);
//
////		std::cout << "\n process data done\n";
//
////		for (ap_uint<8> ite2 = 0; ite2 < DATA_NUM; ++ite2) {
////			std::cout << data[ite2] << "";
////		}
////
////		std::cout << "\n display done\n";
//
//		int result[M] = { 0 };
//
//		//һά���
//		for (ap_uint<8> ite3 = 0; ite3 < DATA_NUM; ++ite3) {
////			std::cout << data[ite3] << "*" << coe[ite3] << "="
////					<< data[ite3] * coe[ite3] << "\n";
//			result[ite3 % M] += data[ite3] * coe[ite3];
////			std::cout << "result[" << ite3 % M << "]=" << result[ite3 % M]
////					<< "\n";
//		}
//
//		//��ֵ���
//		for (ap_uint<8> ite4 = 0; ite4 < M; ++ite4) {
//			out.data.range(16 * (ite4 + 1) - 1, 16 * ite4) = result[ite4];
//		}
//
//		out.last = 0;
//		output.write(out);
////		std::cout << "\n";
//	}
//}

//��Ϊ�������������ʹ��fft ip,����Ѿ���ȷ������������ݶԱ�����
//#include "opfb.hpp"
//void opfb_matix_multiple(hls::stream<axis96_t> &I,
//		hls::stream<axis256_t> &output) {
////#pragma HLS INTERFACE ap_ctrl_chain port=return
//#pragma HLS INTERFACE ap_ctrl_none port=return
//#pragma HLS INTERFACE axis port=I
//#pragma HLS INTERFACE axis port=output
////#pragma HLS INTERFACE s_axilite port=return bundle=CTRL
//
//// Ӳ����ϵ����M=8, TAPS=4
//
//	static ap_int<10> coe[DATA_NUM] = {100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131};
//	static ap_int<10> coe2[DATA_NUM] = {0,0,0,0,0,2048,4096,2048,-2048,-8192,-10240,-6144,8192,28672,-14336,-2048,-2048,-14336,28672,8192,-6144,-10240,-8192,-2048,2048,4096,2048,0,0,0,0,0};
//	static sample_16 data[M * TAPS] = { 0 };
//#pragma HLS ARRAY_PARTITION variable=data complete dim=0
//
//	static ap_int<16> pre0 = 0;
//	static ap_int<16> pre1 = 0;
//
//	while (1) {
//#pragma HLS PIPELINE II=1
//#ifndef __SYNTHESIS__
//		if (I.empty())
//			break;
//#endif
//		axis96_t I_tmp = I.read();
//		axis256_t out;
//		out.data = 0;
//
//		// һά���������ƽ�
////		std::cout << "start process data\n";
//		for (ap_uint<8> ite = DATA_NUM - 1; ite >= M; --ite) {
////			std::cout << "data[" << ite << "]=data[" << ite - D << "]="
////					<< data[ite - D] << "\n";
//			data[ite] = data[ite - D];
//		}
//		data[7] = pre0;
//		pre0 = I_tmp.data.range(79, 64);
//		data[6] = pre1;
//		pre1 = I_tmp.data.range(95, 80);
//
//		data[5] = I_tmp.data.range(15, 0);
//		data[4] = I_tmp.data.range(31, 16);
//		data[3] = I_tmp.data.range(47, 32);
//		data[2] = I_tmp.data.range(63, 48);
//		data[1] = I_tmp.data.range(79, 64);
//		data[0] = I_tmp.data.range(95, 80);
//
////		std::cout << "\n process data done\n";
//
////		for (ap_uint<8> ite2 = 0; ite2 < DATA_NUM; ++ite2) {
////			std::cout << data[ite2] << "";
////		}
////
////		std::cout << "\n display done\n";
//
//		int result[M] = { 0 };
//
//		//һά���
//		for (ap_uint<8> ite3 = 0; ite3 < DATA_NUM; ++ite3) {
////			std::cout << data[ite3] << "*" << coe[ite3] << "="
////					<< data[ite3] * coe[ite3] << "\n";
//			result[ite3 % M] += data[ite3] * coe[ite3];
////			std::cout << "result[" << ite3 % M << "]=" << result[ite3 % M]
////					<< "\n";
//		}
//
//		//��ֵ���
//		for (ap_uint<8> ite4 = 0; ite4 < M; ++ite4) {
//		    // ż���ۣ�ʵ����
//		    out.data.range(16 * (ite4 * 2 + 1) - 1, 16 * (ite4 * 2)) = result[ite4];
//		    // �����ۣ��鲿����ֱ��д0
//		    out.data.range(16 * (ite4 * 2 + 2) - 1, 16 * (ite4 * 2 + 1)) = 0;
//		}
//
//		out.last = 0;
//		output.write(out);
////		std::cout << "\n";
//	}
//}

////�޸Ķ������ݵ�λ���������ݴ�С�����ھ��ʱ�����,�����ȷ���Ѿ���16bit����תΪ12bit adc��Чλ��25.7.10
//#include "opfb.hpp"
//void opfb_matix_multiple(hls::stream<axis96_t> &I,
//		hls::stream<axis256_t> &output) {
////#pragma HLS INTERFACE ap_ctrl_chain port=return
//#pragma HLS INTERFACE ap_ctrl_none port=return
//#pragma HLS INTERFACE axis port=I
//#pragma HLS INTERFACE axis port=output
////#pragma HLS INTERFACE s_axilite port=return bundle=CTRL
//
//// Ӳ����ϵ����M=8, TAPS=4
//
//	static ap_int<10> coe[DATA_NUM] = {100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131};
//	static int coe2[DATA_NUM] = {0,0,0,0,0,2048,4096,2048,-2048,-8192,-10240,-6144,8192,28672,-14336,-2048,-2048,-14336,28672,8192,-6144,-10240,-8192,-2048,2048,4096,2048,0,0,0,0,0};
//	static sample_16 data[M * TAPS] = { 0 };
//#pragma HLS ARRAY_PARTITION variable=data complete dim=0
//
//	static ap_int<16> pre0 = 0;
//	static ap_int<16> pre1 = 0;
//
//	ap_int<16> raw ;
//	ap_int<12> high12;
//
//
//	while (1) {
//#pragma HLS PIPELINE II=1
//#ifndef __SYNTHESIS__
//		if (I.empty())
//			break;
//#endif
//		axis96_t I_tmp = I.read();
//		axis256_t out;
//		out.data = 0;
//
//		// һά���������ƽ�
////		std::cout << "start process data\n";
//		for (ap_uint<8> ite = DATA_NUM - 1; ite >= M; --ite) {
////			std::cout << "data[" << ite << "]=data[" << ite - D << "]="
////					<< data[ite - D] << "\n";
//			data[ite] = data[ite - D];
//		}
////		data[7] = pre0;
////		pre0 = I_tmp.data.range(79, 64);
////		data[6] = pre1;
////		pre1 = I_tmp.data.range(95, 80);
//
//		data[7] = pre0;
//		raw = I_tmp.data.range(79, 64);
//		high12 = raw >> 4;
//		pre0 = (ap_int<16> ) high12;
//
//		data[6] = pre1;
//		raw = I_tmp.data.range(95, 80);
//		high12 = raw >> 4;
//		pre1 = (ap_int<16> ) high12;
//
//
////		data[5] = I_tmp.data.range(15, 0);
////		data[4] = I_tmp.data.range(31, 16);
////		data[3] = I_tmp.data.range(47, 32);
////		data[2] = I_tmp.data.range(63, 48);
////		data[1] = I_tmp.data.range(79, 64);
////		data[0] = I_tmp.data.range(95, 80);
//
//		for(int i=0; i<6; i++) {
//		    raw = I_tmp.data.range(16*(i+1)-1, 16*i);
//		    high12 = raw >> 4;
//		    data[5-i] = (ap_int<16>)high12; // ע��data����˳���range˳��
//		}
//
////		std::cout << "\n process data done\n";
//
////		for (ap_uint<8> ite2 = 0; ite2 < DATA_NUM; ++ite2) {
////			std::cout << data[ite2] << "";
////		}
////
////		std::cout << "\n display done\n";
//
//		int result[M] = { 0 };
//
//		//һά���
//		for (ap_uint<8> ite3 = 0; ite3 < DATA_NUM; ++ite3) {
////			std::cout << data[ite3] << "*" << coe[ite3] << "="
////					<< data[ite3] * coe[ite3] << "\n";
//			result[ite3 % M] += data[ite3] * coe[ite3];
////			std::cout << "result[" << ite3 % M << "]=" << result[ite3 % M]
////					<< "\n";
//		}
//
//		//��ֵ���
//		for (ap_uint<8> ite4 = 0; ite4 < M; ++ite4) {
//		    // ż���ۣ�ʵ����
//		    out.data.range(16 * (ite4 * 2 + 1) - 1, 16 * (ite4 * 2)) = result[ite4];
//		    // �����ۣ��鲿����ֱ��д0
//		    out.data.range(16 * (ite4 * 2 + 2) - 1, 16 * (ite4 * 2 + 1)) = 0;
//		}
//
//		out.last = 0;
//		output.write(out);
////		std::cout << "\n";
//	}
//}

////�޸�intΪfloat����ֹ�������߾���,ʵ�ֺ�fanout �� max level�����⣬simulation�����ȷ
//void opfb_matix_multiple(hls::stream<axis96_t> &I,
//		hls::stream<axis512_t> &output) {
////#pragma HLS DATAFLOW
////#pragma HLS INTERFACE ap_ctrl_chain port=return
//#pragma HLS INTERFACE ap_ctrl_none port=return
//#pragma HLS INTERFACE axis port=I
//#pragma HLS INTERFACE axis port=output
////#pragma HLS INTERFACE s_axilite port=return bundle=CTRL
//
//// Ӳ����ϵ����M=8, TAPS=4
//
//#pragma HLS ARRAY_PARTITION variable=coe complete dim=0
//	static float data[M * TAPS] = { 0 };
//#pragma HLS ARRAY_PARTITION variable=data complete dim=0
//
//	static float pre0 = 0;
//	static float pre1 = 0;
//
//	ap_int<16> raw ;
//	ap_int<12> high12;
//
////	static int fake_value=0;
//
//	while (1) {
//#pragma HLS PIPELINE II=1
//#ifndef __SYNTHESIS__
//		if (I.empty())
//			break;
//#endif
//		axis96_t I_tmp = I.read();
//
////		for(int i = 0;i<6;i++)
////		{
////			I_tmp.data.range((i+1)*16-1, i*16)=(fake_value%2)<<4;
////		}
////		fake_value++;
//		std::cout<<"fake value:\n";
//		for(int i = 0;i<6;i++)
//				{
//			std::cout <<I_tmp.data.range(i*16-1, i*16)<<" ";
//				}
//		std::cout<<"\n";
//
//		axis512_t out;
//		out.data = 0.00;
//
//		// һά���������ƽ�
////		std::cout << "start process data\n";
//		for (ap_uint<8> ite = DATA_NUM - 1; ite >= M; --ite) {
//#pragma HLS UNROLL
//			std::cout << "data[" << ite << "]=data[" << ite - D << "]="
//					<< data[ite - D] << "\n";
//			data[ite] = data[ite - D];
//		}
////		data[7] = pre0;
////		pre0 = I_tmp.data.range(79, 64);
////		data[6] = pre1;
////		pre1 = I_tmp.data.range(95, 80);
//
//		data[7] = pre0;
//		raw = I_tmp.data.range(79, 64);
//		high12 = raw >> 4;
//		pre0 = (float) high12;
//
//		data[6] = pre1;
//		raw = I_tmp.data.range(95, 80);
//		high12 = raw >> 4;
//		pre1 = (float) high12;
//
//		for(ap_uint<8> i=0; i<6; i++) {
//#pragma HLS UNROLL
//		    raw = I_tmp.data.range(16*(i+1)-1, 16*i);
//		    high12 = raw >> 4;
//		    data[5-i] = (float)high12; // ע��data����˳���range˳��
//		}
//
//		std::cout << "\n process data done\n";
//
//		for (ap_uint<8> ite2 = 0; ite2 < DATA_NUM; ++ite2) {
//			std::cout << data[ite2] << " ";
//		}
////
//		std::cout << "\n display done\n";
//
//		float result[M] = { 0 };
//#pragma HLS ARRAY_PARTITION variable=result complete dim=0
//		float temp[DATA_NUM]= { 0 };
//#pragma HLS ARRAY_PARTITION variable=temp complete dim=0
//
//
//		//һά���
//		// ��һ�����ȼ���˷�
//		for (ap_uint<8> ite3 = 0; ite3 < DATA_NUM; ++ite3) {
//#pragma HLS UNROLL
////			std::cout << data[ite3] << "*" << coe[ite3] << "="
////					<< data[ite3] * coe[ite3] << "\n";
////			result[ite3 % M] +=  data[ite3] * coe[ite3];
//			temp[ite3] = data[ite3] * coe[ite3];
////			result[ite3 % M] +=  (ite3%2+1) * coe[ite3];     �޸���������ڲ���float����Ƿ���ȷ
////			std::cout << "result[" << ite3 % M << "]=" << result[ite3 % M]
////					<< "\n";
//		}
//		// �ڶ������ۺϵ� result �У����� result[i % M] ��������
//		for (ap_uint<8> i = 0; i < M; ++i) {
//		#pragma HLS UNROLL
//		    result[i] = 0;
//		    for (ap_uint<8> j = 0; j < TAPS; ++j) {
//		#pragma HLS UNROLL
//		        result[i] += temp[i + j * M];
//		    }
//		}
//
//		//��ֵ���
//		for (ap_uint<8> ite4 = 0; ite4 < M; ++ite4) {
//#pragma HLS UNROLL
//		    // ż���ۣ�ʵ����
////			std::cout<<result[ite4]<<" ";
////			out.data.range(32 * (ite4 * 2 + 1) - 1, 32 * (ite4 * 2)) = *reinterpret_cast<ap_uint<32>*>(&result[ite4]);
//
//			ap_uint<32> conv_u;
//			float conv_f = result[ite4];
//			memcpy(&conv_u, &conv_f, sizeof(float));
//
//
//			// ʵ��
//			out.data.range(32 * (ite4 * 2 + 1) - 1, 32 * (ite4 * 2)) = conv_u;
//
//
////			out.data.range(32 * (ite4 * 2 + 1) - 1, 32 * (ite4 * 2)) =
////					real_bits;
//			// �����ۣ��鲿����ֱ��д0
//			out.data.range(32 * (ite4 * 2 + 2) - 1, 32 * (ite4 * 2 + 1)) = 0;
//		}
////		ap_uint<32> real_bits = out.data.range(31, 0);
////		float val_real;
////		*reinterpret_cast<ap_uint<32>*>(&val_real) = real_bits; // ����ת��
////		std::printf("%f ", val_real);
//
//		out.last = 0;
//		output.write(out);
////		std::cout << "\n";
//	}
//}

//��512bitλ���Ϊ256bitλ��
#include "opfb_snake_convolution.hpp"
void opfb_snake_convolution(hls::stream<axis_t> &I,
		hls::stream<axis256_t> &output) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=I
#pragma HLS INTERFACE axis port=output
#pragma HLS PIPELINE II=1

#pragma HLS ARRAY_PARTITION variable=coe complete dim=0
	static float data[N] = { 0 };
#pragma HLS ARRAY_PARTITION variable=data complete dim=0

	static float pre0 = 0, pre1 = 0;
	static float pre[M - D] = { 0 };
	ap_int<16> raw;
	ap_uint<12> high12;
	static int fake_value = 0;

	while (1) {
#pragma HLS PIPELINE II=1
#ifndef __SYNTHESIS__
		if (I.empty())
			break;
#endif
		axis_t in = I.read();

//		��������
//        for(int i = 0;i<D;i++)
//        		{
//        			in.data.range((i+1)*16-1, i*16)=(fake_value%2+1)<<4;
//        			std::cout<<in.data.range((i+1)*16-1, i*16)<<" ";
//        		}
//        		fake_value++;
//        		std::cout<<"fake value:"<<in.data<<"\n";

// 		������������
		for (int idx = N - 1; idx >= M; --idx) {
#pragma HLS UNROLL
			data[idx] = data[idx - D];
		}

//		�����������D������
		for (int i = 0; i < D; ++i) {
#pragma HLS UNROLL
			raw = in.data.range(16 * (i + 1) - 1, 16 * i);
			data[D - i - 1] = (float) (raw >> 4);
		}

//		�������ֵ�M-D������
		for (int i = D; i < M; ++i) {
#pragma HLS UNROLL
			data[i] = pre[i - D];
//		����pre
			pre[i - D] = data[i - D];
		}

//		����������������ƶ���Ľ��
//		for (int i = 0; i < N; ++i) {
//			std::cout << data[i] << " ";
//		}
//		std::cout << "\n";

// 		�������
		float temp[N];
#pragma HLS ARRAY_PARTITION variable=temp complete dim=0
		float result[M];
#pragma HLS ARRAY_PARTITION variable=result complete dim=0
		for (int i = 0; i < N; ++i) {
#pragma HLS UNROLL
			temp[i] = data[i] * coe[i];
		}
		for (int i = 0; i < M; ++i) {
#pragma HLS UNROLL
			float sum = 0;
			for (int j = 0; j < TAPS; ++j) {
#pragma HLS UNROLL
				sum += temp[i + j * M];
			}
			result[i] = sum;
		}

// 		������Ϊ 256-bit
		axis256_t out;
		out.data = 0;
		out.last = in.last;
		for (int i = 0; i < M; ++i) {
#pragma HLS UNROLL
			ap_uint<32> u;
			memcpy(&u, &result[i], sizeof(u));
			out.data.range(32 * (i + 1) - 1, 32 * i) = u;
		}

		output.write(out);
		if (in.last)
			break;
	}
}

