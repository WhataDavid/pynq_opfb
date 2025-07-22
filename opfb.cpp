#include "opfb.hpp"
//#include <iostream> // 仅仿真时有效，综合不能有
//// 假定 axis96_t.data.range(15,0) 为第0个，(31,16)为第1个... (95,80)为第5个
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
//    // 硬编码系数，M=8, TAPS=4
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
//    // 数据滑窗
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
//        // 滑动窗口，把每列数据推进去
//        for (int row = 0; row < M; ++row) {
//#pragma HLS UNROLL
//            // 先整体左移一格
//            for (int c = 0; c < TAPS-1; ++c) {
//                dataI[row][c] = dataI[row][c+1];
//                dataQ[row][c] = dataQ[row][c+1];
//            }
//            // 最后一列填新数据（数据不足M时补0）
////            if (row < 6) {
////                dataI[row][TAPS-1] = I_tmp.data.range(16*(row+1)-1, 16*row);
////                dataQ[row][TAPS-1] = Q_tmp.data.range(16*(row+1)-1, 16*row);
////            } else {
////                dataI[row][TAPS-1] = 0;
////                dataQ[row][TAPS-1] = 0;
////            }
//            // 最后一列填新数据
//                        if (row < 6) {
//                            dataI[row][TAPS-1] = I_tmp.data.range(16*(row+1)-1, 16*row);
//                            dataQ[row][TAPS-1] = Q_tmp.data.range(16*(row+1)-1, 16*row);
//                        } else if (row == 6) {
//                            dataI[row][TAPS-1] = I_tmp.data.range(16*5-1, 16*4); // 用第4个数据
//                            dataQ[row][TAPS-1] = Q_tmp.data.range(16*5-1, 16*4);
//                        } else if (row == 7) {
//                            dataI[row][TAPS-1] = I_tmp.data.range(16*6-1, 16*5); // 用第5个数据
//                            dataQ[row][TAPS-1] = Q_tmp.data.range(16*6-1, 16*5);
//                        }
//        }
//
//        col_count++;
//
//        // 只有填满TAPS列才能输出
//        if (col_count >= TAPS) {
//            axis128_t out;
//            out.data = 0;
//            for (int row = 0; row < M; ++row) {
//#pragma HLS UNROLL
//                ap_int<32> sum = 0;
//                for (int c = 0; c < TAPS; ++c) {
//#pragma HLS UNROLL
//                    // 对应元素乘
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
//                    sum += prodI ; // 可分开输出I、Q
//                }
//                // 输出为每行的和，都打包到out.data
//                out.data.range(16*(row+1)-1, 16*row) = sum;
//            }
////            out.last = (i == length-1) ? 1 : 0;
//            out.last = (((i+1) % length) == 0) ? 1 : 0;
//            output.write(out);
//        }
//    }
//}

//length无法控制，tlast拉高有问题
//#include "opfb.hpp"
//
//// 假定 axis96_t.data.range(15,0) 为第0个，(31,16)为第1个... (95,80)为第5个
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
//    // 硬编码系数，M=8, TAPS=4
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
//    // 数据滑窗
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
//        // 仿真出口，防止死循环
//        if (I.empty() || Q.empty()) break;
//#endif
//        axis96_t I_tmp = I.read();
//        axis96_t Q_tmp = Q.read();
//
//        // 滑动窗口，把每列数据推进去
//        for (ap_uint<4> row = 0; row < M; ++row) {
//#pragma HLS UNROLL
//            // 先整体左移一格
//            for (ap_uint<4> c = 0; c < TAPS-1; ++c) {
//                dataI[row][c] = dataI[row][c+1];
//                dataQ[row][c] = dataQ[row][c+1];
//            }
//            // 最后一列填新数据
//            if (row < 6) {
//                dataI[row][TAPS-1] = I_tmp.data.range(16*(row+1)-1, 16*row);
//                dataQ[row][TAPS-1] = Q_tmp.data.range(16*(row+1)-1, 16*row);
//            } else if (row == 6) {
//                dataI[row][TAPS-1] = I_tmp.data.range(16*5-1, 16*4); // 用第4个数据
//                dataQ[row][TAPS-1] = Q_tmp.data.range(16*5-1, 16*4);
//            } else if (row == 7) {
//                dataI[row][TAPS-1] = I_tmp.data.range(16*6-1, 16*5); // 用第5个数据
//                dataQ[row][TAPS-1] = Q_tmp.data.range(16*6-1, 16*5);
//            }
//        }
//
//        col_count++;
//        i++;
//
//        // 只有填满TAPS列才能输出
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
//                    sum += prodI; // 如需区分I/Q，可分开输出
////                    sum += prodQ;
//                }
//                out.data.range(16*(row+1)-1, 16*row) = sum;
////                std::cout <<std::endl;
//            }
////            std::cout <<"----------------------------------------------"<<std::endl;
//
//            // 每length个数据输出一次last=1，分帧
//            out.last = (i == length) ? 1 : 0;
//            output.write(out);
//
//            // 每length输出一次last=1后，重新计数，进入下一帧
//            if (i == length) {
//                i = 0;
//            }
//        }
//    }
//}
//
//

//length无法控制，tlast拉高有问题
//#include "opfb.hpp"
//
//// 假定 axis96_t.data.range(15,0) 为第0个，(31,16)为第1个... (95,80)为第5个
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
//    // 硬编码系数，M=8, TAPS=4
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
//    // 数据滑窗
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
//        // 仿真出口，防止死循环
//        if (I.empty() || Q.empty()) break;
//#endif
//        axis96_t I_tmp = I.read();
//        axis96_t Q_tmp = Q.read();
//
//        // 滑动窗口，把每列数据推进去
//        for (ap_uint<4> row = 0; row < M; ++row) {
//#pragma HLS UNROLL
//            // 先整体左移一格
//            for (ap_uint<4> c = 0; c < TAPS-1; ++c) {
//                dataI[row][c] = dataI[row][c+1];
////                dataQ[row][c] = dataQ[row][c+1];
//            }
//            // 最后一列填新数据
//            if (row < 6) {
//                dataI[row][TAPS-1] = I_tmp.data.range(16*(row+1)-1, 16*row);
////                dataQ[row][TAPS-1] = Q_tmp.data.range(16*(row+1)-1, 16*row);
//            } else if (row == 6) {
////                dataI[row][TAPS-1] = I_tmp.data.range(16*5-1, 16*4); // 用第4个数据
//                dataI[row][TAPS-1] = 1;
////                dataQ[row][TAPS-1] = Q_tmp.data.range(16*5-1, 16*4);
//            } else if (row == 7) {
////                dataI[row][TAPS-1] = I_tmp.data.range(16*6-1, 16*5); // 用第5个数据
//                dataI[row][TAPS-1] = 1;
////                dataQ[row][TAPS-1] = Q_tmp.data.range(16*6-1, 16*5);
//            }
//        }
//
//        col_count++;
//        i++;
//
//        // 只有填满TAPS列才能输出
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
//                    sum += prodI; // 如需区分I/Q，可分开输出
////                    sum = dataI[row][c];
////                    sum += prodQ;
//                }
//                out.data.range(16*(row+1)-1, 16*row) = sum;
////                std::cout <<std::endl;
//            }
////            std::cout <<"----------------------------------------------"<<std::endl;
//
//            // 每length个数据输出一次last=1，分帧
//            out.last = (i == length) ? 1 : 0;
//            output.write(out);
//
//            // 每length输出一次last=1后，重新计数，进入下一帧
//            if (i == length) {
//                i = 0;
//            }
//        }
//    }
//}

//恢复，没成功
//#include "opfb.hpp"
//// 假定 axis96_t.data.range(15,0) 为第0个，(31,16)为第1个... (95,80)为第5个
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
//    // 硬编码系数，M=8, TAPS=4
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
//    // 数据滑窗
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
//        // 仿真出口，防止死循环
//        if (I.empty() || Q.empty()) break;
//#endif
//        axis96_t I_tmp = I.read();
//        axis96_t Q_tmp = Q.read();
//
//        // 滑动窗口，把每列数据推进去
//        for (ap_uint<4> row = 0; row < M; ++row) {
//#pragma HLS UNROLL
//            // 先整体左移一格
//            for (ap_uint<4> c = 0; c < TAPS-1; ++c) {
//                dataI[row][c] = dataI[row][c+1];
//                dataQ[row][c] = dataQ[row][c+1];
//            }
//            // 最后一列填新数据
//            if (row < 6) {
//                dataI[row][TAPS-1] = I_tmp.data.range(16*(row+1)-1, 16*row);
//                dataQ[row][TAPS-1] = Q_tmp.data.range(16*(row+1)-1, 16*row);
//            } else if (row == 6) {
//                dataI[row][TAPS-1] = I_tmp.data.range(16*5-1, 16*4); // 用第4个数据
//                dataQ[row][TAPS-1] = Q_tmp.data.range(16*5-1, 16*4);
//            } else if (row == 7) {
//                dataI[row][TAPS-1] = I_tmp.data.range(16*6-1, 16*5); // 用第5个数据
//                dataQ[row][TAPS-1] = Q_tmp.data.range(16*6-1, 16*5);
//            }
//        }
//
//        col_count++;
//        i++;
//
//        // 只有填满TAPS列才能输出
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
//                    sum += prodI; // 如需区分I/Q，可分开输出
//                }
////                out.data.range(16*(row+1)-1, 16*row) = sum;
//                out.data.range(16*(row+1)-1, 16*row) = dataI[row][0];
////                std::cout <<std::endl;
//            }
////            std::cout <<"----------------------------------------------"<<std::endl;
//
//            // 每length个数据输出一次last=1，分帧
//            out.last = (i == length) ? 1 : 0;
//            output.write(out);
//
//            // 每length输出一次last=1后，重新计数，进入下一帧
//            if (i == length) {
//                i = 0;
//            }
//        }
//    }
//}

////去掉q
//#include "opfb.hpp"
//// 假定 axis96_t.data.range(15,0) 为第0个，(31,16)为第1个... (95,80)为第5个
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
//    // 硬编码系数，M=8, TAPS=4
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
//    // 数据滑窗
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
//        // 仿真出口，防止死循环
//        if (I.empty() ) break;
//#endif
//        axis96_t I_tmp = I.read();
////        axis96_t Q_tmp = Q.read();
//
//        // 滑动窗口，把每列数据推进去
//        for (ap_uint<4> row = 0; row < M; ++row) {
//#pragma HLS UNROLL
//            // 先整体左移一格
//            for (ap_uint<4> c = 0; c < TAPS-1; ++c) {
//                dataI[row][c] = dataI[row][c+1];
////                dataQ[row][c] = dataQ[row][c+1];
//            }
//            // 最后一列填新数据
//            if (row < 6) {
//                dataI[row][TAPS-1] = I_tmp.data.range(16*(row+1)-1, 16*row);
////                dataQ[row][TAPS-1] = Q_tmp.data.range(16*(row+1)-1, 16*row);
//            } else if (row == 6) {
//                dataI[row][TAPS-1] = I_tmp.data.range(16*5-1, 16*4); // 用第4个数据
////                dataQ[row][TAPS-1] = Q_tmp.data.range(16*5-1, 16*4);
//            } else if (row == 7) {
//                dataI[row][TAPS-1] = I_tmp.data.range(16*6-1, 16*5); // 用第5个数据
////                dataQ[row][TAPS-1] = Q_tmp.data.range(16*6-1, 16*5);
//            }
//        }
//
//        col_count++;
//        i++;
//
//        // 只有填满TAPS列才能输出
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
//                    sum += prodI; // 如需区分I/Q，可分开输出
//                }
////                out.data.range(16*(row+1)-1, 16*row) = sum;
//                out.data.range(16*(row+1)-1, 16*row) = dataI[row][0];
////                std::cout <<std::endl;
//            }
////            std::cout <<"----------------------------------------------"<<std::endl;
//
//            // 每length个数据输出一次last=1，分帧
//            out.last = (i == length) ? 1 : 0;
//            output.write(out);
//
//            // 每length输出一次last=1后，重新计数，进入下一帧
//            if (i == length) {
//                i = 0;
//            }
//        }
//    }
//}

////先输出adc数据,时钟fmax提高到了2Ghz，去除axi控制总线后结果正确
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
//		// 滑动窗口推进
//		for (ap_uint<4> row = 0; row < M; ++row) {
//#pragma HLS UNROLL
//
//			// 展开写法，去除if判断
//			dataI[0][TAPS] = I_tmp.data.range(15, 0);
//			dataI[1][TAPS] = I_tmp.data.range(31, 16);
//			dataI[2][TAPS] = I_tmp.data.range(47, 32);
//			dataI[3][TAPS] = I_tmp.data.range(63, 48);
//			dataI[4][TAPS] = I_tmp.data.range(79, 64);
//			dataI[5][TAPS] = I_tmp.data.range(95, 80);
//			dataI[6][TAPS] = I_tmp.data.range(79, 64);  // 用第4个数据
//			dataI[7][TAPS] = I_tmp.data.range(95, 80);  // 用第5个数据
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

////输出卷积结果
//#include "opfb.hpp"
//void opfb_matix_multiple(hls::stream<axis96_t> &I,
//		hls::stream<axis128_t> &output) {
////#pragma HLS INTERFACE ap_ctrl_chain port=return
//#pragma HLS INTERFACE ap_ctrl_none port=return
//#pragma HLS INTERFACE axis port=I
//#pragma HLS INTERFACE axis port=output
////#pragma HLS INTERFACE s_axilite port=return bundle=CTRL
//
//// 硬编码系数，M=8, TAPS=4
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
//		// 滑动窗口推进
//		for (ap_uint<4> row = 0; row < M; ++row) {
//#pragma HLS UNROLL
//			int sum = 0;
//			// 展开写法，去除if判断
//			dataI[0][TAPS] = I_tmp.data.range(15, 0);
//			dataI[1][TAPS] = I_tmp.data.range(31, 16);
//			dataI[2][TAPS] = I_tmp.data.range(47, 32);
//			dataI[3][TAPS] = I_tmp.data.range(63, 48);
//			dataI[4][TAPS] = I_tmp.data.range(79, 64);
//			dataI[5][TAPS] = I_tmp.data.range(95, 80);
//			dataI[6][TAPS] = I_tmp.data.range(79, 64);  // 用第4个数据
//			dataI[7][TAPS] = I_tmp.data.range(95, 80);  // 用第5个数据
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

//改为一维数组 正确 2025-7-3
//#include "opfb.hpp"
//void opfb_matix_multiple(hls::stream<axis96_t> &I,
//		hls::stream<axis256_t> &output) {
////#pragma HLS INTERFACE ap_ctrl_chain port=return
//#pragma HLS INTERFACE ap_ctrl_none port=return
//#pragma HLS INTERFACE axis port=I
//#pragma HLS INTERFACE axis port=output
////#pragma HLS INTERFACE s_axilite port=return bundle=CTRL
//
//// 硬编码系数，M=8, TAPS=4
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
//		// 一维滑动窗口推进
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
//		//一维卷积
//		for (ap_uint<8> ite3 = 0; ite3 < DATA_NUM; ++ite3) {
////			std::cout << data[ite3] << "*" << coe[ite3] << "="
////					<< data[ite3] * coe[ite3] << "\n";
//			result[ite3 % M] += data[ite3] * coe[ite3];
////			std::cout << "result[" << ite3 % M << "]=" << result[ite3 % M]
////					<< "\n";
//		}
//
//		//赋值结果
//		for (ap_uint<8> ite4 = 0; ite4 < M; ++ite4) {
//			out.data.range(16 * (ite4 + 1) - 1, 16 * ite4) = result[ite4];
//		}
//
//		out.last = 0;
//		output.write(out);
////		std::cout << "\n";
//	}
//}

//改为复数输出，便于使用fft ip,结果已经正确，经与测试数据对比无误
//#include "opfb.hpp"
//void opfb_matix_multiple(hls::stream<axis96_t> &I,
//		hls::stream<axis256_t> &output) {
////#pragma HLS INTERFACE ap_ctrl_chain port=return
//#pragma HLS INTERFACE ap_ctrl_none port=return
//#pragma HLS INTERFACE axis port=I
//#pragma HLS INTERFACE axis port=output
////#pragma HLS INTERFACE s_axilite port=return bundle=CTRL
//
//// 硬编码系数，M=8, TAPS=4
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
//		// 一维滑动窗口推进
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
//		//一维卷积
//		for (ap_uint<8> ite3 = 0; ite3 < DATA_NUM; ++ite3) {
////			std::cout << data[ite3] << "*" << coe[ite3] << "="
////					<< data[ite3] * coe[ite3] << "\n";
//			result[ite3 % M] += data[ite3] * coe[ite3];
////			std::cout << "result[" << ite3 % M << "]=" << result[ite3 % M]
////					<< "\n";
//		}
//
//		//赋值结果
//		for (ap_uint<8> ite4 = 0; ite4 < M; ++ite4) {
//		    // 偶数槽（实部）
//		    out.data.range(16 * (ite4 * 2 + 1) - 1, 16 * (ite4 * 2)) = result[ite4];
//		    // 奇数槽（虚部），直接写0
//		    out.data.range(16 * (ite4 * 2 + 2) - 1, 16 * (ite4 * 2 + 1)) = 0;
//		}
//
//		out.last = 0;
//		output.write(out);
////		std::cout << "\n";
//	}
//}

////修改读入数据的位宽，降低数据大小，便于卷积时的相加,结果正确，已经将16bit数据转为12bit adc有效位宽25.7.10
//#include "opfb.hpp"
//void opfb_matix_multiple(hls::stream<axis96_t> &I,
//		hls::stream<axis256_t> &output) {
////#pragma HLS INTERFACE ap_ctrl_chain port=return
//#pragma HLS INTERFACE ap_ctrl_none port=return
//#pragma HLS INTERFACE axis port=I
//#pragma HLS INTERFACE axis port=output
////#pragma HLS INTERFACE s_axilite port=return bundle=CTRL
//
//// 硬编码系数，M=8, TAPS=4
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
//		// 一维滑动窗口推进
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
//		    data[5-i] = (ap_int<16>)high12; // 注意data索引顺序和range顺序
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
//		//一维卷积
//		for (ap_uint<8> ite3 = 0; ite3 < DATA_NUM; ++ite3) {
////			std::cout << data[ite3] << "*" << coe[ite3] << "="
////					<< data[ite3] * coe[ite3] << "\n";
//			result[ite3 % M] += data[ite3] * coe[ite3];
////			std::cout << "result[" << ite3 % M << "]=" << result[ite3 % M]
////					<< "\n";
//		}
//
//		//赋值结果
//		for (ap_uint<8> ite4 = 0; ite4 < M; ++ite4) {
//		    // 偶数槽（实部）
//		    out.data.range(16 * (ite4 * 2 + 1) - 1, 16 * (ite4 * 2)) = result[ite4];
//		    // 奇数槽（虚部），直接写0
//		    out.data.range(16 * (ite4 * 2 + 2) - 1, 16 * (ite4 * 2 + 1)) = 0;
//		}
//
//		out.last = 0;
//		output.write(out);
////		std::cout << "\n";
//	}
//}



//修改int为float，防止溢出，提高精度,实现后fanout 和 max level有问题，simulation结果正确
void opfb_matix_multiple(hls::stream<axis96_t> &I,
		hls::stream<axis512_t> &output) {
//#pragma HLS DATAFLOW
//#pragma HLS INTERFACE ap_ctrl_chain port=return
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=I
#pragma HLS INTERFACE axis port=output
//#pragma HLS INTERFACE s_axilite port=return bundle=CTRL

// 硬编码系数，M=8, TAPS=4

#pragma HLS ARRAY_PARTITION variable=coe complete dim=0
	static float data[M * TAPS] = { 0 };
#pragma HLS ARRAY_PARTITION variable=data complete dim=0

	static float pre0 = 0;
	static float pre1 = 0;

	ap_int<16> raw ;
	ap_int<12> high12;

//	static int fake_value=0;

	while (1) {
#pragma HLS PIPELINE II=1
#ifndef __SYNTHESIS__
		if (I.empty())
			break;
#endif
		axis96_t I_tmp = I.read();

//		for(int i = 0;i<6;i++)
//		{
//			I_tmp.data.range((i+1)*16-1, i*16)=(fake_value%2)<<4;
//		}
//		fake_value++;
		std::cout<<"fake value:\n";
		for(int i = 0;i<6;i++)
				{
			std::cout <<I_tmp.data.range(i*16-1, i*16)<<" ";
				}
		std::cout<<"\n";

		axis512_t out;
		out.data = 0.00;

		// 一维滑动窗口推进
//		std::cout << "start process data\n";
		for (ap_uint<8> ite = DATA_NUM - 1; ite >= M; --ite) {
#pragma HLS UNROLL
			std::cout << "data[" << ite << "]=data[" << ite - D << "]="
					<< data[ite - D] << "\n";
			data[ite] = data[ite - D];
		}
//		data[7] = pre0;
//		pre0 = I_tmp.data.range(79, 64);
//		data[6] = pre1;
//		pre1 = I_tmp.data.range(95, 80);

		data[7] = pre0;
		raw = I_tmp.data.range(79, 64);
		high12 = raw >> 4;
		pre0 = (float) high12;

		data[6] = pre1;
		raw = I_tmp.data.range(95, 80);
		high12 = raw >> 4;
		pre1 = (float) high12;

		for(ap_uint<8> i=0; i<6; i++) {
#pragma HLS UNROLL
		    raw = I_tmp.data.range(16*(i+1)-1, 16*i);
		    high12 = raw >> 4;
		    data[5-i] = (float)high12; // 注意data索引顺序和range顺序
		}

		std::cout << "\n process data done\n";

		for (ap_uint<8> ite2 = 0; ite2 < DATA_NUM; ++ite2) {
			std::cout << data[ite2] << " ";
		}
//
		std::cout << "\n display done\n";

		float result[M] = { 0 };
#pragma HLS ARRAY_PARTITION variable=result complete dim=0
		float temp[DATA_NUM]= { 0 };
#pragma HLS ARRAY_PARTITION variable=temp complete dim=0


		//一维卷积
		// 第一步：先计算乘法
		for (ap_uint<8> ite3 = 0; ite3 < DATA_NUM; ++ite3) {
#pragma HLS UNROLL
//			std::cout << data[ite3] << "*" << coe[ite3] << "="
//					<< data[ite3] * coe[ite3] << "\n";
//			result[ite3 % M] +=  data[ite3] * coe[ite3];
			temp[ite3] = data[ite3] * coe[ite3];
//			result[ite3 % M] +=  (ite3%2+1) * coe[ite3];     修改了这里，便于测试float结果是否正确
//			std::cout << "result[" << ite3 % M << "]=" << result[ite3 % M]
//					<< "\n";
		}
		// 第二步：聚合到 result 中（避免 result[i % M] 的依赖）
		for (ap_uint<8> i = 0; i < M; ++i) {
		#pragma HLS UNROLL
		    result[i] = 0;
		    for (ap_uint<8> j = 0; j < TAPS; ++j) {
		#pragma HLS UNROLL
		        result[i] += temp[i + j * M];
		    }
		}

		//赋值结果
		for (ap_uint<8> ite4 = 0; ite4 < M; ++ite4) {
#pragma HLS UNROLL
		    // 偶数槽（实部）
//			std::cout<<result[ite4]<<" ";
//			out.data.range(32 * (ite4 * 2 + 1) - 1, 32 * (ite4 * 2)) = *reinterpret_cast<ap_uint<32>*>(&result[ite4]);

			ap_uint<32> conv_u;
			float conv_f = result[ite4];
			memcpy(&conv_u, &conv_f, sizeof(float));


			// 实部
			out.data.range(32 * (ite4 * 2 + 1) - 1, 32 * (ite4 * 2)) = conv_u;


//			out.data.range(32 * (ite4 * 2 + 1) - 1, 32 * (ite4 * 2)) =
//					real_bits;
			// 奇数槽（虚部），直接写0
			out.data.range(32 * (ite4 * 2 + 2) - 1, 32 * (ite4 * 2 + 1)) = 0;
		}
//		ap_uint<32> real_bits = out.data.range(31, 0);
//		float val_real;
//		*reinterpret_cast<ap_uint<32>*>(&val_real) = real_bits; // 反向转换
//		std::printf("%f ", val_real);

		out.last = 0;
		output.write(out);
//		std::cout << "\n";
	}
}


