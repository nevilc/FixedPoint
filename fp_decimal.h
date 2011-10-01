/**
 *	@file fp_decimal.h
 *	Adds the decimal and fixed decimal classes
 *	Included by fp_types.h, or can be added individually
 */

#ifndef H_FP_DECIMAL
#define H_FP_DECIMAL

#include "fp_internal.h"

template<bool _Signed, typename _StorageType>
class Decimal{
	typedef unsigned char _digit_type;
	typedef _StorageType _group_type;
	
	static const _digit_type	_digit_max			= 0x09;
	static const _digit_type	_digit_capacity		= _digit_max + 1;

	static const size_t			_digit_size			= 4;
	static const _digit_type	_digit_mask			= (1 << _digit_size) - 1;
	static const size_t			_group_mask			= (1 << (CHAR_BIT * sizeof(_digit_type))) - 1;
	static const count_type		_digits_per_group	= (CHAR_BIT * sizeof(_digit_type)) / _group_size;

	// Member variables
	// Start at most significant decimal digit, least significant integer digit
	_group_type* _integer;
	_group_type* _decimal;

	count_type	_integer_groups;
	count_type	_decimal_groups;

private:
	void _resize_groups(count_type _integer_groups_, count_type _decimal_groups_){
		if (_integer_groups != _integer_groups_){
			_group_type* _temp_integer = new _group_type[_integer_groups_](0);
			for (count_type i = 0; i < (_integer_groups < _integer_groups_ ? _integer_groups : _integer_groups_); i++){
				_temp_integer[i] = _integer[i];
			}
			delete[] _integer;
			_integer = _temp_integer;
		}
		if (_decimal_groups != _decimal_groups_){
			_group_type* _temp_decimal = new _group_type[_decimal_groups_](0);
			for (count_type i = 0; i < (_decimal_groups < _decimal_groups_ ? _decimal_groups : _decimal_groups_); i++){
				_temp_decimal[i] = _decimal[i];
			}
			delete[] _decimal;
			_decimal = _temp_decimal;
		}
	}

	void _resize(count_type _integer_digits_, count_type _decimal_digits_){
		_resize_groups((_integer_digits + _digits_per_group - 1) / _digits_per_group, (_decimal_digits + _digits_per_group - 1) / _digits_per_group);
	}

	count_type _integer_count() const{
		return _integer_groups * _digits_per_group;
	}

	count_type _decimal_count() const{
		return _decimal_groups * _digits_per_group;
	}

	const _group_type& _integer_group(count_type _pos) const{
		#ifdef FIXEDPOINT_DEBUG
			if (_pos >= _integer_groups){
				// Error
			}
		#endif
		return _integer[_pos];
	}

	_group_type& _integer_group(count_type _pos){
		#ifdef FIXEDPOINT_DEBUG
			if (_pos >= _integer_groups){
				// Error
			}
		#endif
		return _integer[_pos];
	}

	const _group_type& _decimal_group(count_type _pos) const{
		#ifdef FIXEDPOINT_DEBUG
			if (_pos >= _decimal_groups){
				// Error
			}
		#endif
		return _decimal[_pos];
	}

	_group_type& _decimal_group(count_type _pos){
		#ifdef FIXEDPOINT_DEBUG
			if (_pos >= _decimal_groups){
				// Error
			}
		#endif
		return _decimal[_pos];
	}

	_digit_type _integer_digit(count_type _pos) const{
		if (_pos >= _decimal_count()){
			return 0;
		}
		
		return (_integer_group(_pos / _digits_per_group) >> (_pos % _digits_per_group) * _digit_size) & _digit_mask;
	}

	_digit_type _decimal_digit(count_type _pos) const{
		#ifdef FIXEDPOINT_DEBUG
			if (_pos >= _DecimalCount){
				// Error
			}
		#endif
		
		return (_decimal_group(_pos / _digits_per_group) >> (_pos % _digits_per_group) * _digit_size) & _digit_mask;
	}
};

template<count_type _IntegerCount, count_type _DecimalCount, bool _Signed, typename _StorageType>
class FixedDecimal{
	typedef unsigned char _digit_type;
	typedef _StorageType _group_type;

	static const _digit_type	_digit_max			= 0x09;
	static const _digit_type	_digit_capacity		= _digit_max + 1;

	static const size_t			_digit_size			= 4;
	static const _digit_type	_digit_mask			= (1 << _digit_size) - 1;
	static const size_t			_group_mask			= (1 << (CHAR_BIT * sizeof(_digit_type))) - 1;
	static const size_t			_group_size			= std::numeric_limits<_group_type>::digits;
	static const count_type		_digits_per_group	= (CHAR_BIT * sizeof(_digit_type)) / _group_size;

	static const count_type	_integer_groups		= (_IntegerCount + (_digits_per_group - 1)) / _digits_per_group;
	static const count_type	_decimal_groups		= (_DecimalCount + (_digits_per_group - 1)) / _digits_per_group;

	// Member variables
	_group_type _integer[_integer_groups];
	_group_type _decimal[_decimal_groups];

	const _group_type& _integer_group(count_type _pos) const{
		#ifdef FIXEDPOINT_DEBUG
			if (_pos >= _integer_groups){
				// Error
			}
		#endif
		return _integer[_pos];
	}

	_group_type& _integer_group(count_type _pos){
		#ifdef FIXEDPOINT_DEBUG
			if (_pos >= _integer_groups){
				// Error
			}
		#endif
		return _integer[_pos];
	}

	const _group_type& _decimal_group(count_type _pos) const{
		#ifdef FIXEDPOINT_DEBUG
			if (_pos >= _decimal_groups){
				// Error
			}
		#endif
		return _decimal[_pos];
	}

	_group_type& _decimal_group(count_type _pos){
		#ifdef FIXEDPOINT_DEBUG
			if (_pos >= _decimal_groups){
				// Error
			}
		#endif
		return _decimal[_pos];
	}

	_digit_type _integer_digit(count_type _pos) const{
		#ifdef FIXEDPOINT_DEBUG
			if (_pos >= _DecimalCount){
				// Error
			}
		#endif
		
		return (_integer_group(_pos / _digits_per_group) >> (_pos % _digits_per_group) * _digit_size) & _digit_mask;
	}

	_digit_type _decimal_digit(count_type _pos) const{
		#ifdef FIXEDPOINT_DEBUG
			if (_pos >= _DecimalCount){
				// Error
			}
		#endif
		
		return (_decimal_group(_pos / _digits_per_group) >> (_pos % _digits_per_group) * _digit_size) & _digit_mask;
	}

	void _integer_digit(count_type _pos, _digit_type _value){
		_group_type _isolated_digit = _integer_group(_pos / _digits_per_group);
		_isolated_digit &= _digit_mask << (_pos % _digits_per_group);
		_integer_group(_pos / _digits_per_group) ^= _isolated_digit;
		_integer_group(_pos / _digits_per_group) |= _value << (pos % _digits_per_group);
	}

	void _decimal_digit(count_type _pos, _digit_type _value){
		_group_type _isolated_digit = _decimal_group(_pos / _digits_per_group);
		_isolated_digit &= _digit_mask << (_pos % _digits_per_group);
		_decimal_group(_pos / _digits_per_group) ^= _isolated_digit;
		_decimal_group(_pos / _digits_per_group) |= _value << (pos % _digits_per_group);
	}

	#ifdef FIXEDPOINT_FORCEFORMAT
		void _digit_add(const FixedDecimal<_IntegerCount, _DecimalCount>& _other){
			bool _carrybit(0);
			for (count_type i = 0; i < _DecimalCount; i++){
				_digit_type _result(_decimal_digit(i) + _other._decimal_digit(i) + _carrybit);
				_carrybit = _result / _digit_capacity;
				_decimal_digit(i, _result % _digit_capacity);
			}
			for (count_type i = 0; i < _IntegerCount; i++){
				_digit_type _result(_integer_digit(i) + _other._decimal_digit(i) + _carrybit);
				_carrybit = _result / _digit_capacity;
				_integer_digit(i, _result % _digit_capacity);
			}

			#ifdef FIXEDPOINT_DEBUG
				if (_carrybit){
					// Error overflow
				}
			#endif
		}
		void _digit_subtract(const FixedDecimal<_IntegerCount, _DecimalCount>& _other){
			bool _borrowbit(0);
			for (count_type i = 0; i < _DecimalCount; i++){
				_digit_type _difference(_other._decimal_digit(i) + _borrowbit);
				_borrowbit = _difference > _decimal_digit(i);
				_decimal_digit(i, _decimal_digit(i) + (_borrowbit * _digit_capacity - _difference));
			}
			for (count_type i = 0; i < _IntegerCount; i++){
				_digit_type _difference(_other._integer_digit(i) + _borrowbit);
				_borrowbit = _difference > _integer_digit(i);
				_integer_digit(i, _integer_digit(i) + (_borrowbit * _digit_capacity - _difference));
			}

			#ifdef FIXEDPOINT_DEBUG
				if (_borrowbit){
					// Error underflow
				}
			#endif
		}
		bool _digit_multiply(const FixedDecimal<_IntegerCount, _DecimalCount>& _other){
			FixedDecimal<_IntegerCount, _DecimalCount> _sum(), _addend();
			_digit_type _mcarrydigit(0);
				

			for (count_type i = 0; i < _DecimalCount; i++){
				for (count_type j = 0; j < _DecimalCount; j++){
					// Potentially dangerous if CHAR_BIT < 7
					// _result needs to be able to hold up to 90 (9 * 9 + 9)
					// Finding a system with a byte < 7 bits is unlikely,
					// And rolling my own series-addition for multiplication
					// would likely be much, much slower that what the processor
					// can do on its own
					// Could become another define if someone likes to use bizarre hardware

					digit_type _result(_decimal_digit(i) * _other._decimal_digit(i) + _mcarrydigit);
					_mcarrydigit = _result / _digit_capacity;
					_addends[i]._decimal_digit(j, _result % _digit_capacity);
				}
				for (count_type j = 0; j < _IntegerCount; j++){
					digit_type _result(_integer_digit(i) * _other._integer_digit(i) + _mcarrydigit);
					_mcarrydigit = _result / _digit_capacity;
					_addends[i]._integer_digit(j, _result % _digit_capacity);
				}
				if (_sum._digit_add(_addend)){
					// Error addition overflow
					return true;
				}
			}

			// Error multiplication overflow
			return (_mcarrydigit != 0);
		}

		bool _digit_divide(const FixedDecimal<_IntegerCoiunt, _DecimalCount>& _other){
			// Stupid division
			return 0;
		}
	#else//!FIXEDPOINT_FORCEFORMAT

	#endif


public:
	FixedDecimal(){
		memset(_integer, 0, _integer_groups);
		memset(_decimal, 0, _decimal_groups);
	}

	FixedDecimal(const _group_type* integer, const _group_type* decimal){
		for (count_type i = 0; i < _integer_groups; i++){
			_integer_group(i) = integer[i];
		}
		for (count_type i = 0; i < _decimal_groups; i++){
			_decimal_group(i) = decimal[i];
		}
	}
	template<count_type _OtherIntegerCount, count_type _OtherDecimalCount>
	FixedDecimal(const FixedDecimal<_OtherIntegerCount, _OtherDecimalCount>& other){
		#ifdef FIXEDPOINT_DEBUG
			if (_OtherIntegerCount > _IntegerCount){
				// Error
			}
		#endif

		for (count_type i = 0; i < _integer_groups; i++){
			_integer_group(i) = (i < other._integer_groups ? other._integer_group(i) : 0);
		}
		for (count_type i = 0; i < _decimal_groups; i++){
			_decimal_group(i) = (i < other._integer_groups ? other._decimal_group(i) : 0);
		}
	}


	#ifdef FIXEDPOINT_FORCEFORMAT
		FixedDecimal<_IntegerCount, _DecimalCount>& operator=(const FixedDecimal<_IntegerCount, _DecimalCount>& other){
			if (&other != this){
				for (count_type i = 0; i < _integer_groups; i++){
					_integer_group(i) = other._integer_group(i);
				}
				for (count_type i = 0; i < _decimal_groups; i++){
					_decimal_group(i) = other._decimal_group(i);
				}
			}
			return *this;
		}

		FixedDecimal<_IntegerCount, _DecimalCount>& operator+=(const FixedDecimal<_IntegerCount, _DecimalCount>& other){
			_digit_add(other);
			return *this;
		}

		FixedDecimal<_IntegerCount, _DecimalCount>& operator-=(const FixedDecimal<_IntegerCount, _DecimalCount>& other){
			_digit_subtract(other);
			return *this;
		}

		FixedDecimal<_IntegerCount, _DecimalCount>& operator*=(const FixedDecimal<_IntegerCount, _DecimalCount>& other){
			_digit_multiply(other);
			return *this;
		}

		FixedDecimal<_IntegerCount, _DecimalCount>& operator/=(const FixedDecimal<_IntegerCount, _DecimalCount>& other){
			_digit_divide(other);
			return *this;
		}

		FixedDecimal<_IntegerCount, _DecimalCount> operator+(const FixedDecimal<_IntegerCount, _DecimalCount>& other) const{
			FixedDecimal<_IntegerCount, _DecimalCount> _copy(*this);
			return _copy += other;
		}

		FixedDecimal<_IntegerCount, _DecimalCount> operator-(const FixedDecimal<_IntegerCount, _DecimalCount>& other) const{
			FixedDecimal<_IntegerCount, _DecimalCount> _copy(*this);
			return _copy -= other;
		}

		FixedDecimal<_IntegerCount, _DecimalCount> operator*(const FixedDecimal<_IntegerCount, _DecimalCount>& other) const{
			FixedDecimal<_IntegerCount, _DecimalCount> _copy(*this);
			return _copy *= other;
		}

		FixedDecimal<_IntegerCount, _DecimalCount> operator/(const FixedDecimal<_IntegerCount, _DecimalCount>& other) const{
			FixedDecimal<_IntegerCount, _DecimalCount> _copy(*this);
			return _copy /= other;
		}

		bool operator==(const FixedDecimal<_IntegerCount, _DecimalCount>& other) const{
			for (count_type i = 0; i < _decimal_groups; i++){
				if (_decimal_group(i) != other._decimal_group(i)){
					return 0;
				}
			}
			for (count_type i = 0; i < _integer_groups; i++){
				if (_integer_group(i) != other._integer_group(i)){
					return 0;
				}
			}
			return 1;
		}

		bool operator!=(const FixedDecimal<_IntegerCount, _DecimalCount>& other) const{
			return !operator==(other);
		}

		bool operator<(const FixedDecimal<_IntegerCount, _DecimalCount>& other) const{
			for (count_type i = _integerCount - 1; i >= 0; i--){
				if (_integer_digit(i) < other._integer_digit(i)){
					return 1;
				}
				if (_integer_digit(i) > other._integer_digit(i)){
					return 0;
				}
			}
			for (count_type i = _DecimalDigit - 1; i >= 0; i--){
				if (_decimal_digit(i) < other._decimal_digit(i)){
					return 1;
				}
				if (_decimal_digit(i) > other._decimal_digit(i)){
					return 0;
				}
			}
			return 0;
		}

		bool operator<=(const FixedDecimal<_IntegerCount, _DecimalCount>& other) const{
			for (count_type i = _integerCount - 1; i >= 0; i--){
				if (_integer_digit(i) < other._integer_digit(i)){
					return 1;
				}
				if (_integer_digit(i) > other._integer_digit(i)){
					return 0;
				}
			}
			for (count_type i = _DecimalDigit - 1; i >= 0; i--){
				if (_decimal_digit(i) < other._decimal_digit(i)){
					return 1;
				}
				if (_decimal_digit(i) > other._decimal_digit(i)){
					return 0;
				}
			}
			return 1;
		}

		bool operator>(const FixedDecimal<_IntegerCount, _DecimalCount>& other) const{
			return !operator<=(other);
		}

		bool operator>=(const FixedDecimal<_IntegerCount, _DecimalCount>& other) const{
			return !operator<(other);
		}
	#endif

	_digit_type operator[](scount_type pos) const{
		return pos >= 0 ? _integer_digit(count_type(pos)) : _decimal_digit(_DecimalCount + pos);
	}

	
};

#endif//H_FP_DECIMAL
