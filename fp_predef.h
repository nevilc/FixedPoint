/**
 *	@file fp_predef.h
 *	Adds convenient typedefs for many templates
 *	Included by fp_types.h, or can be added individually
 */

#ifndef H_FP_PREDEF
#define H_FP_PREDEF

//std::numeric_limits<>





#if UCHAR_MAX		>= 0xFF
	#define FIXEDPOINT_SIZE8 unsigned char
#elif USHRT_MAX		>= 0xFF
	#define FIXEDPOINT_SIZE8 unsigned short int
#elif UINT_MAX		>= 0xFF
	#define FIXEDPOINT_SIZE8 unsigned int
#elif ULONG_MAX		>= 0xFF
	#define FIXEDPOINT_SIZE8 unsigned long int
#elif ULLONG_MAX	>= 0xFF
	#define FIXEDPOINT_SIZE8 unsigned long long int
#endif

#if UCHAR_MAX		>= 0xFFFF
	#define FIXEDPOINT_SIZE16 unsigned char
#elif USHRT_MAX		>= 0xFFFF
	#define FIXEDPOINT_SIZE16 unsigned short int
#elif UINT_MAX		>= 0xFFFF
	#define FIXEDPOINT_SIZE16 unsigned int
#elif ULONG_MAX		>= 0xFFFF
	#define FIXEDPOINT_SIZE16 unsigned long int
#elif ULLONG_MAX	>= 0xFFFF
	#define FIXEDPOINT_SIZE16 unsigned long long int
#endif

#if UCHAR_MAX		>= 0xFFFFFFFF
	#define FIXEDPOINT_SIZE32 unsigned char
#elif USHRT_MAX		>= 0xFFFFFFFF
	#define FIXEDPOINT_SIZE32 unsigned short int
#elif UINT_MAX		>= 0xFFFFFFFF
	#define FIXEDPOINT_SIZE32 unsigned int
#elif ULONG_MAX		>= 0xFFFFFFFF
	#define FIXEDPOINT_SIZE32 unsigned long int
#elif ULLONG_MAX	>= 0xFFFFFFFF
	#define FIXEDPOINT_SIZE32 unsigned long long int
#endif

#if UCHAR_MAX		>= 0xFFFFFFFFFFFFFFFF
	#define FIXEDPOINT_SIZE64 unsigned char
#elif USHRT_MAX		>= 0xFFFFFFFFFFFFFFFF
	#define FIXEDPOINT_SIZE64 unsigned short int
#elif UINT_MAX		>= 0xFFFFFFFFFFFFFFFF
	#define FIXEDPOINT_SIZE64 unsigned int
#elif ULONG_MAX		>= 0xFFFFFFFFFFFFFFFF
	#define FIXEDPOINT_SIZE64 unsigned long int
#elif ULLONG_MAX	>= 0xFFFFFFFFFFFFFFFF
	#define FIXEDPOINT_SIZE64 unsigned long long int
#endif

#ifdef H_FP_FIXEDPOINT
	#ifdef FIXEDPOINT_SIZE8
		typedef FixedPoint<FIXEDPOINT_SIZE8, 1>		fp1_7;
		typedef FixedPoint<FIXEDPOINT_SIZE8, 2>		fp2_6;
		typedef FixedPoint<FIXEDPOINT_SIZE8, 3>		fp3_5;
		typedef FixedPoint<FIXEDPOINT_SIZE8, 4>		fp4_4;
		typedef FixedPoint<FIXEDPOINT_SIZE8, 5>		fp5_3;
		typedef FixedPoint<FIXEDPOINT_SIZE8, 6>		fp6_2;
		typedef FixedPoint<FIXEDPOINT_SIZE8, 7>		fp7_1;
	#endif

	#ifdef FIXEDPOINT_SIZE16
		typedef FixedPoint<FIXEDPOINT_SIZE16, 1>	fp15_1;
		typedef FixedPoint<FIXEDPOINT_SIZE16, 2>	fp14_2;
		typedef FixedPoint<FIXEDPOINT_SIZE16, 3>	fp13_3;
		typedef FixedPoint<FIXEDPOINT_SIZE16, 4>	fp12_4;
		typedef FixedPoint<FIXEDPOINT_SIZE16, 5>	fp11_5;
		typedef FixedPoint<FIXEDPOINT_SIZE16, 6>	fp10_6;
		typedef FixedPoint<FIXEDPOINT_SIZE16, 7>	fp9_7;
		typedef FixedPoint<FIXEDPOINT_SIZE16, 8>	fp8_8;
		typedef FixedPoint<FIXEDPOINT_SIZE16, 9>	fp7_9;
		typedef FixedPoint<FIXEDPOINT_SIZE16, 10>	fp6_10;
		typedef FixedPoint<FIXEDPOINT_SIZE16, 11>	fp5_11;
		typedef FixedPoint<FIXEDPOINT_SIZE16, 12>	fp4_12;
		typedef FixedPoint<FIXEDPOINT_SIZE16, 13>	fp3_13;
		typedef FixedPoint<FIXEDPOINT_SIZE16, 14>	fp2_14;
		typedef FixedPoint<FIXEDPOINT_SIZE16, 15>	fp1_15;
	#endif

	#ifdef FIXEDPOINT_SIZE32
		typedef FixedPoint<FIXEDPOINT_SIZE32, 2>	fp30_2;
		typedef FixedPoint<FIXEDPOINT_SIZE32, 4>	fp28_4;
		typedef FixedPoint<FIXEDPOINT_SIZE32, 6>	fp26_6;
		typedef FixedPoint<FIXEDPOINT_SIZE32, 8>	fp24_8;
		typedef FixedPoint<FIXEDPOINT_SIZE32, 10>	fp22_10;
		typedef FixedPoint<FIXEDPOINT_SIZE32, 12>	fp20_12;
		typedef FixedPoint<FIXEDPOINT_SIZE32, 14>	fp18_14;
		typedef FixedPoint<FIXEDPOINT_SIZE32, 16>	fp16_16;
		typedef FixedPoint<FIXEDPOINT_SIZE32, 18>	fp14_18;
		typedef FixedPoint<FIXEDPOINT_SIZE32, 20>	fp12_20;
		typedef FixedPoint<FIXEDPOINT_SIZE32, 22>	fp10_22;
		typedef FixedPoint<FIXEDPOINT_SIZE32, 24>	fp8_24;
		typedef FixedPoint<FIXEDPOINT_SIZE32, 26>	fp6_26;
		typedef FixedPoint<FIXEDPOINT_SIZE32, 28>	fp4_28;
		typedef FixedPoint<FIXEDPOINT_SIZE32, 30>	fp2_30;
	#endif

	#ifdef FIXEDPOINT_SIZE64
		typedef FixedPoint<FIXEDPOINT_SIZE64, 4>	fp60_4;
		typedef FixedPoint<FIXEDPOINT_SIZE64, 8>	fp56_8;
		typedef FixedPoint<FIXEDPOINT_SIZE64, 12>	fp52_12;
		typedef FixedPoint<FIXEDPOINT_SIZE64, 16>	fp48_16;
		typedef FixedPoint<FIXEDPOINT_SIZE64, 20>	fp44_20;
		typedef FixedPoint<FIXEDPOINT_SIZE64, 24>	fp40_24;
		typedef FixedPoint<FIXEDPOINT_SIZE64, 28>	fp36_28;
		typedef FixedPoint<FIXEDPOINT_SIZE64, 32>	fp32_32;
		typedef FixedPoint<FIXEDPOINT_SIZE64, 36>	fp28_36;
		typedef FixedPoint<FIXEDPOINT_SIZE64, 40>	fp24_40;
		typedef FixedPoint<FIXEDPOINT_SIZE64, 44>	fp20_44;
		typedef FixedPoint<FIXEDPOINT_SIZE64, 48>	fp16_48;
		typedef FixedPoint<FIXEDPOINT_SIZE64, 52>	fp12_52;
		typedef FixedPoint<FIXEDPOINT_SIZE64, 56>	fp8_56;
		typedef FixedPoint<FIXEDPOINT_SIZE64, 60>	fp4_60;
	#endif
#endif

#endif//H_FP_PREDEF
