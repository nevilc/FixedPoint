/**
 *	@file fp_internal.h
 *	Defines, typedefs, and declarations needed by other headers
 *	Automatically added by other headers, no need to include
 */

#ifndef H_FP_INTERNAL
#define H_FP_INTERNAL

#include <limits>

// Only include safety checks in debug mode
#ifdef DEBUG
#ifndef FIXEDPOINT_DEBUG
#define FIXEDPOINT_DEBUG
#endif
#endif

// If compiler is (semi-)C++0x compliant, add things such as static_assert for extra compile-time safety
#ifndef FIXEDPOINT_CPP0X
	#if __cplusplus > 199711L
		#define FIXEDPOINT_CPP0X
	#else
		#ifdef _HAS_CPP0X			// MSVC's C++0x sub-featureset included in VS2010
			#if _HAS_CPP0X
				#define FIXEDPOINT_CPP0X
			#endif
		#endif
	#endif
#endif

// Add the following line to your code before any #include "fp_*.h"
// to disallow operations done using different template types
// e.g. Adding a FixedPoint<unsigned int, 12> to a FixedPoint<unsigned int, 16> is not permitted
// without explicitly converting either
//#define FIXEDPOINT_FORCEFORMAT

// Add the following line to your code before any #include "fp_*.h"
// to round all integer values given by the class. Minor performance hit, may be less 
// consistant between operations
//#define FIXEDPOINT_ROUNDING

// typedef for lengths and length differences. chars are used by default, but if for whatever reason, 
// that is not enough, they can be changed to higher values
typedef unsigned char	count_type;
typedef signed char		scount_type;

// FixedPoint declarations
template<typename IntegerType, count_type IntegerBits, count_type FractionalBits = std::numeric_limits<IntegerType>::digits - IntegerBits>
class FixedPoint;

// Decimal declarations
/*
template<count_type _IntegerCount, count_type _DecimalCount, bool _Signed = false, typename _StorageType = unsigned char>
class FixedDecimal;

template<bool _Signed = false, typename _StorageType = unsigned char>
class Decimal;*/

// Fraction declarations
template<typename IntegerType>
class Fraction;

#endif//H_FP_INTERNAL
