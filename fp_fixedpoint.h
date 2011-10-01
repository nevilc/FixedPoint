/**
 *	@file fp_fixedpoint.h
 *	Adds a fixed point class
 *	Included by fp_types.h, or can be added individually
 */

#ifndef H_FP_FIXEDPOINT
#define H_FP_FIXEDPOINT

#include <climits>

#include "fp_internal.h"

#define fp_float(_inttype_, _fracsize_, _float_) \
		(_inttype_)((_float_) >= 0 ? (_float_) : -(_float_)), \
		(_inttype_)(((_float_) >= 0 ? (_float_) : -(_float_)) - ((_inttype_)((_float_) >= 0 ? (_float_) : -(_float_))) * (1 << (_fracsize_))), \
		((_float_) >= 0)

#define fp_float_i(_inttype_, _fracsize_, _float_) \
		(_inttype_)((_float_) >= 0 ? (_float_) : -(_float_))

#define fp_float_f(_inttype_, _fracsize_, _float_) \
		(_inttype_)(((_float_) >= 0 ? (_float_) : -(_float_)) - ((_inttype_)((_float_) >= 0 ? (_float_) : -(_float_))) * (1 << (_fracsize_)))

#define fp_float_s(_inttype_, _fracsize_, _float_) \
		((_float_) >= 0)

///	A lightweight class for easy manipulation of various fixed point number types.
/**
 *	FixedPoint is a templated class that takes an integer data type
 *	(char, short int, int, long int, or long long int, including (un)signed variants),
 *  followed by one or two integer literals specifying the number of bits dedicated to the integer (and sign) portion, and optionally the decimal portion.
 *	If the second integer is not specified, FixedPoint will make use of all availible bits. The number of bits used must be less than those available in the data type.
 *	FixedPoint defines the basic mathematical operations, as well as other utilitous function, such as returning
 *	only the integer or decimal portion. The contained fixed point number can be passed to another function expecting the raw fixed point number by calling operator().
 *	The class makes a few (minor) assumptions, such as that your compiler uses 2's complement for signed values and implementation-defined operations such as signed
 *	left shift work in the standard manner.
 */

template<typename IntegerType, count_type IntegerBits, count_type FractionalBits>
class FixedPoint{
	static const bool _signed = std::numeric_limits<IntegerType>::is_signed;
	static const count_type _TotalBits = std::numeric_limits<IntegerType>::digits;
	static const count_type _UsedBits = IntegerBits + FractionalBits;

	IntegerType _content;

	#ifdef FIXEDPOINT_CPP0X
		static_assert(_UsedBits <= _TotalBits, "Invalid fixed point position");
	#endif

	// Returns the integer portion, unsigned
	IntegerType _i() const{
		return (_content >= 0 ? _content : -_content) >> FractionalBits;
	}

	// Returns the decimal portion, unsigned
	IntegerType _d() const{
		return (_content >= 0 ? _content : -_content) & ((1 << FractionalBits) - 1);
	}

	bool _negative() const{
		return _signed && (_content < 0);
	}
	
public:

	static const count_type f_bits = FractionalBits;
	static const count_type i_bits = IntegerBits;

	///	Default constructor, initializes to 0
	FixedPoint() : _content(0){}

	///	Raw fixed point constructor
	/**
	 *	Also used by the compile-time template converters
	 *	@param fp A fixed point number with the same format
	 */
	FixedPoint(IntegerType fp) : _content(fp){
	}

	/// Component value constructor
	/**
	 *	@param i_value The value of the integer portion. Beware that only the number of bits reserved for the integer portion can be used.
	 *	@param d_value The value of the decimal portion. Beware that only the number of bits reserved for the decimal portion can be used.
	 *	@param s_value Whether the value is negative, positive by default. Regardless, i_ and d_value must be positive!
	 */
	FixedPoint(IntegerType i_value, IntegerType d_value, bool s_value = false) : _content((d_value | (i_value << FractionalBits))){
		if (s_value){
			_content *= -1;
		}
	}

	/// Copy contructor
	/**
	 *	@param other FixedPoint to copy, if FIXEDPOINT_FORCEFORMAT is defined, other must be of the same format
	 */
	#ifdef FIXEDPOINT_FORCEFORMAT
		FixedPoint(const FixedPoint<IntegerType, IntegerBits, FractionalBits>& other) : _content(other._content){}
	#else
		template<count_type _OtherPoint>
		FixedPoint(const FixedPoint<IntegerType, _OtherPoint>& other) : _content(other.convert<IntegerBits>()._content){}
	#endif

	/// Fraction constructor
	/**
	 *	Creates a FixedPoint with the same value as the Fraction
	 *	@param frac Equivalent fraction
	 */
	/*
	FixedPoint(const Fraction<IntegerType>& frac) : _content((_unsign_int(frac.i()) << FractionalBits) | _unsign_int(frac.f().convert_denominator(1 << FractionalBits).numerator())){
		if (_negative_integer(frac.numerator()) ^ _negative_integer(frac.denominator())){
			_content = -_content;
		}
	}*/

	///	Returns the integer value
	/**
	 *	@return Integer value
	 */
	IntegerType i() const{
		return _i();
	}
	
	/// Returns the decimal value
	/**
	 *	@return Decimal value
	 */
	IntegerType d() const{
		return _d();
	}

	bool s() const{
		return _negative();
	}

	/// Sets the integer value
	/**
	 *	Does not change the sign.
	 *	@param i_value Integer value, if the integer type is signed, must be positive
	 */
	void i(IntegerType i_value){
		_content = ((_negative() * -2) + 1) * (i_value << FractionalBits) | _d();
	}

	/// Sets the decimal value
	/**
	 *	@param d_value Decimal value
	 */
	void d(IntegerType d_value){
		_content = ((_negative() * -2) + 1) * (_i() << FractionalBits) | d_value;
	}

	void s(bool s_value){
		_content = (s_value ^ _content < 0) ? _content : -_content;
	}

	/// Sets the integer, decimal, and sign value
	/**
	 *	@param i_value Integer value
	 *	@param d_value Decimal value
	 *	@param s_value Sign value
	 */
	void ids(IntegerType i_value, IntegerType d_value, bool s_value = false){
		i(i_value);
		d(d_value);
		if (s_value){
			_content *= -1;
		}
	}

	/// Returns the number of integer bits
	/**
	 *	@return Integer bits
	 */
	count_type i_size() const{
		return IntegerBits;
	}

	/// Returns the number of decimal bits
	/**
	 *	@return Decimal bits
	 */
	count_type d_size() const{
		return FractionalBits;
	}

	/// Returns a reference to the inner fixed point number
	/**
	 *	@return Fixed point reference
	 */
	/*IntegerType& data(){
		return _content;
	}*/
	/// Returns a reference to the inner fixed point number
	/**
	 *	@return Fixed point reference
	 */
	IntegerType& operator()(){
		return _content;
	}


	/// Returns a const reference to the inner fixed point number
	/**
	 *	@return const fixed point reference
	 */
	/*const IntegerType& data() const{
		return _content;
	}*/

	/// Returns a const reference to the inner fixed point number
	/**
	 *	@return const fixed point reference
	 */
	const IntegerType& operator()() const{
		return _content;
	}

	/// Converts a FixedPoint from one number of decimal bits to another (e.g. 20:12 to 8:24)
	/**
	 *	convert does not support conversion to another base type (e.g. unsigned int to unsigned short int)
	 *	for safety reasons
	 *	@return Converted FixedPoint
	 */
	template<count_type OtherIntegerBits, count_type OtherFractionalBits>
	FixedPoint<IntegerType, OtherIntegerBits, OtherFractionalBits> convert() const{
		// TODO could use some optimization
		// try to remove the branching
		scount_type ddiff = scount_type(d_size()) - scount_type(OtherFractionalBits);
		if (ddiff >= 0){
			return FixedPoint<IntegerType, OtherIntegerBits, OtherFractionalBits>(_i(), _d() >> count_type(ddiff), _content < 0);
		}else{
			return FixedPoint<IntegerType, OtherIntegerBits, OtherFractionalBits>(_i(), _d() << count_type(-ddiff), _content < 0);
		}
	}

	

	#ifdef FIXEDPOINT_FORCEFORMAT
		FixedPoint<IntegerType, IntegerBits, FractionalBits>& operator=(const FixedPoint<IntegerType, IntegerBits, FractionalBits>& other){
			if (this != &other){
				_content = other._content;
			}
			return *this;
		}

		FixedPoint<IntegerType, IntegerBits, FractionalBits>& operator+=(const FixedPoint<IntegerType, IntegerBits, FractionalBits>& other){
			#ifdef FIXEDPOINT_DEBUG
				if (_content + other._content < _content){
					// Error (Overflow)
				}
			#endif
			_content += other._content;
			return *this;
		}
		FixedPoint<IntegerType, IntegerBits, FractionalBits>& operator-=(const FixedPoint<IntegerType, IntegerBits, FractionalBits>& other){
			#ifdef FIXEDPOINT_DEBUG
				if (_content < other._content){
					// Error (Underflow)
				}
			#endif
			_content -= other._content;
			return *this;
		}
		FixedPoint<IntegerType, IntegerBits, FractionalBits>& operator*=(const FixedPoint<IntegerType, IntegerBits, FractionalBits>& other){
			#ifdef FIXEDPOINT_DEBUG
				// Currently no check for multiplication overflow
			#endif

			_content *= other._content;
			_content >> FractionalBits;

			return *this;
		}
		FixedPoint<IntegerType, IntegerBits, FractionalBits>& operator/=(const FixedPoint<IntegerType, IntegerBits, FractionalBits>& other){
			#ifdef FIXEDPOINT_DEBUG		
				if (other._content == 0){
					// Oh SHI-
				}
			#endif

			// dunno lol
			return *this;
		}
	#else
		template<count_type OtherIntegerBits, count_type OtherFractionalBits>
		FixedPoint<IntegerType, IntegerBits, FractionalBits>& operator=(const FixedPoint<IntegerType, OtherIntegerBits, OtherFractionalBits>& other){
			if (this != &other){
				_content = other.convert<IntegerBits, FractionalBits>()();
			}
			return *this;
		}

		template<count_type OtherIntegerBits, count_type OtherFractionalBits>
		FixedPoint<IntegerType, IntegerBits, FractionalBits>& operator+=(const FixedPoint<IntegerType, OtherIntegerBits, OtherFractionalBits>& other){
			#ifdef FIXEDPOINT_DEBUG
			#endif
			_content += other.convert<IntegerBits, FractionalBits>()();
			return *this;
		}

		template<count_type OtherIntegerBits, count_type OtherFractionalBits>
		FixedPoint<IntegerType, IntegerBits, FractionalBits>& operator-=(const FixedPoint<IntegerType, OtherIntegerBits, OtherFractionalBits>& other){
			#ifdef FIXEDPOINT_DEBUG
			#endif
			_content -= other.convert<IntegerBits, FractionalBits>()();
			return *this;
		}

		template<count_type OtherIntegerBits, count_type OtherFractionalBits>
		FixedPoint<IntegerType, IntegerBits, FractionalBits>& operator*=(const FixedPoint<IntegerType, OtherIntegerBits, OtherFractionalBits>& other){
			#ifdef FIXEDPOINT_DEBUG
				// Currently no check for multiplication overflow
			#endif

			_content *= other.convert<IntegerBits, FractionalBits>()();
			_content >>= OtherFractionalBits;

			return *this;
		}

		template<count_type OtherIntegerBits, count_type OtherFractionalBits>
		FixedPoint<IntegerType, IntegerBits, FractionalBits>& operator/=(const FixedPoint<IntegerType, OtherIntegerBits, OtherFractionalBits>& other){
			#ifdef FIXEDPOINT_DEBUG		
				if (other._content == 0){
					// Oh SHI-
				}
			#endif

			// TODO dunno lol
			return *this;
		}
	#endif

	#ifdef FIXEDPOINT_FORCEFORMAT
		FixedPoint<IntegerType, IntegerBits, FractionalBits> operator+(const FixedPoint<IntegerType, IntegerBits, FractionalBits>& other){
			return (FixedPoint<IntegerType, IntegerBits, FractionalBits>(*this) += other);
		}
		FixedPoint<IntegerType, IntegerBits, FractionalBits> operator-(const FixedPoint<IntegerType, IntegerBits, FractionalBits>& other){
			return (FixedPoint<IntegerType, IntegerBits, FractionalBits>(*this) -= other);
		}
		FixedPoint<IntegerType, IntegerBits, FractionalBits> operator*(const FixedPoint<IntegerType, IntegerBits, FractionalBits>& other){
			return (FixedPoint<IntegerType, IntegerBits, FractionalBits>(*this) *= other);
		}
		FixedPoint<IntegerType, IntegerBits, FractionalBits> operator/(const FixedPoint<IntegerType, IntegerBits, FractionalBits>& other){
			return (FixedPoint<IntegerType, IntegerBits, FractionalBits>(*this) /= other);
		}

		bool operator==(const FixedPoint<IntegerType, IntegerBits, FractionalBits>& other) const{
			return _content == other._content;
		}

		bool operator!=(const FixedPoint<IntegerType, IntegerBits, FractionalBits>& other) const{
			return !(operator==(other));
		}

		bool operator<(const FixedPoint<IntegerType, IntegerBits, FractionalBits>& other) const{
			return _content < other._content;
		}

		bool operator<=(const FixedPoint<IntegerType, IntegerBits, FractionalBits>& other) const{
			return _content <= other._content;
		}

		bool operator>(const FixedPoint<IntegerType, IntegerBits, FractionalBits>& other) const{
			return !(operator<=(other));
		}

		bool operator>=(const FixedPoint<IntegerType, IntegerBits, FractionalBits>& other) const{
			return !(operator<(other));
		}
	#else
		template<count_type OtherIntegerBits, count_type OtherFractionalBits>
		FixedPoint<IntegerType, IntegerBits, FractionalBits> operator+(const FixedPoint<IntegerType, OtherIntegerBits, OtherFractionalBits>& other) const{
			return (FixedPoint<IntegerType, IntegerBits, FractionalBits>(*this) += other.convert<IntegerBits, FractionalBits>());
		}
		template<count_type OtherIntegerBits, count_type OtherFractionalBits>
		FixedPoint<IntegerType, IntegerBits, FractionalBits> operator-(const FixedPoint<IntegerType, OtherIntegerBits, OtherFractionalBits>& other) const{
			return (FixedPoint<IntegerType, IntegerBits, FractionalBits>(*this) -= other.convert<IntegerBits, FractionalBits>());
		}
		template<count_type OtherIntegerBits, count_type OtherFractionalBits>
		FixedPoint<IntegerType, IntegerBits, FractionalBits> operator*(const FixedPoint<IntegerType, OtherIntegerBits, OtherFractionalBits>& other) const{
			return (FixedPoint<IntegerType, IntegerBits, FractionalBits>(*this) *= other.convert<IntegerBits, FractionalBits>());
		}
		template<count_type OtherIntegerBits, count_type OtherFractionalBits>
		FixedPoint<IntegerType, IntegerBits, FractionalBits> operator/(const FixedPoint<IntegerType, OtherIntegerBits, OtherFractionalBits>& other) const{
			return (FixedPoint<IntegerType, IntegerBits, FractionalBits>(*this) /= other.convert<IntegerBits, FractionalBits>());
		}


		template<count_type OtherIntegerBits, count_type OtherFractionalBits>
		bool operator==(const FixedPoint<IntegerType, OtherIntegerBits, OtherFractionalBits>& other) const{
			return (_content == other.convert<IntegerBits, FractionalBits>()());
		}

		template<count_type OtherIntegerBits, count_type OtherFractionalBits>
		bool operator!=(const FixedPoint<IntegerType, OtherIntegerBits, OtherFractionalBits>& other) const{
			return !(operator==(other));
		}

		template<count_type OtherIntegerBits, count_type OtherFractionalBits>
		bool operator<(const FixedPoint<IntegerType, OtherIntegerBits, OtherFractionalBits>& other) const{
			return (_content < other.convert<IntegerBits, FractionalBits>()());
		}

		template<count_type OtherIntegerBits, count_type OtherFractionalBits>
		bool operator<=(const FixedPoint<IntegerType, OtherIntegerBits, OtherFractionalBits>& other) const{
			return (_content <= other.convert<IntegerBits, FractionalBits>()());
		}

		template<count_type OtherIntegerBits, count_type OtherFractionalBits>
		bool operator>(const FixedPoint<IntegerType, OtherIntegerBits, OtherFractionalBits>& other) const{
			return !(operator<=(other));
		}

		template<count_type OtherIntegerBits, count_type OtherFractionalBits>
		bool operator>=(const FixedPoint<IntegerType, OtherIntegerBits, OtherFractionalBits>& other) const{
			return !(operator<(other));
		}
	#endif

	FixedPoint<IntegerType, IntegerBits, FractionalBits>& operator+=(const IntegerType& other){
		i(i() + other);
		return *this;
	}
	FixedPoint<IntegerType, IntegerBits, FractionalBits>& operator-=(const IntegerType& other){
		i(i() - other);
		return *this;
	}
	FixedPoint<IntegerType, IntegerBits, FractionalBits>& operator*=(const IntegerType& other){
		_content *= other;
		return *this;
	}
	FixedPoint<IntegerType, IntegerBits, FractionalBits>& operator/=(const IntegerType& other){
		_content /= other;
		return *this;
	}

	FixedPoint<IntegerType, IntegerBits, FractionalBits> operator+(const IntegerType& other) const{
		return FixedPoint<IntegerType, IntegerBits, FractionalBits>(*this) += other;
	}
	FixedPoint<IntegerType, IntegerBits, FractionalBits> operator-(const IntegerType& other) const{
		return FixedPoint<IntegerType, IntegerBits, FractionalBits>(*this) -= other;
	}
	FixedPoint<IntegerType, IntegerBits, FractionalBits> operator*(const IntegerType& other) const{
		return FixedPoint<IntegerType, IntegerBits, FractionalBits>(*this) *= other;
	}
	FixedPoint<IntegerType, IntegerBits, FractionalBits> operator/(const IntegerType& other) const{
		return FixedPoint<IntegerType, IntegerBits, FractionalBits>(*this) /= other;
	}

	FixedPoint<IntegerType, IntegerBits, FractionalBits> operator-(int dummy) const{
		return FixedPoint<IntegerType, IntegerBits, FractionalBits>(-_content);
	}

	bool operator==(const IntegerType& other) const{
		return (i() == other && d() == 0);
	}

	bool operator!=(const IntegerType& other) const{
		return !(operator==(other));
	}

	bool operator<(const IntegerType& other) const{
		return i() < other;
	}

	bool operator<=(const IntegerType& other) const{
		return operator<(other) || operator==(other);
	}

	bool operator>(const IntegerType& other) const{
		return !operator<=(other);
	}

	bool operator>=(const IntegerType& other) const{
		return !operator<(other);
	}


	FixedPoint<IntegerType, IntegerBits, FractionalBits>& operator++(){
		return _content += (1 << FractionalBits);
	}

	FixedPoint<IntegerType, IntegerBits, FractionalBits>& operator--(){
		return _content -= (1 << FractionalBits);
	}

	FixedPoint<IntegerType, IntegerBits, FractionalBits> operator++(int){
		FixedPoint<IntegerType, IntegerBits, FractionalBits> _copy(*this);
		*this++();
		return _copy;
	}

	FixedPoint<IntegerType, IntegerBits, FractionalBits> operator--(int){
		FixedPoint<IntegerType, IntegerBits, FractionalBits> _copy(*this);
		*this--();
		return _copy;
	}

	FixedPoint<IntegerType, IntegerBits, FractionalBits>& operator<<=(const int& shift){
		return _content <<= shift;
	}

	FixedPoint<IntegerType, IntegerBits, FractionalBits>& operator>>=(const int& shift){
		return _content >>= shift;
	}

	FixedPoint<IntegerType, IntegerBits, FractionalBits> operator<<(const int& shift){
		FixedPoint<IntegerType, IntegerBits, FractionalBits> _copy(*this);
		return _copy <<= shift;
	}

	FixedPoint<IntegerType, IntegerBits, FractionalBits> operator>>(int shift){
		FixedPoint<IntegerType, IntegerBits, FractionalBits> _copy(*this);
		return _copy >>= shift;
	}

	FixedPoint<IntegerType, IntegerBits, FractionalBits> operator-() const{
		FixedPoint<IntegerType, IntegerBits, FractionalBits> _copy(*this);
		_copy._content = -_copy._content;
		return _copy;
	}
};





#endif//H_FP_FIXEDPOINT
