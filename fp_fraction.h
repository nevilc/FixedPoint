/**
 *	@file fp_fraction.h
 *	Adds a fraction class
 *	Included by fp_types.h, or can be added individually
 */

#ifndef H_FP_FRACTION
#define H_FP_FRACTION

#include "fp_internal.h"

/// A class to represent non-integral numbers as fractions
/**
 *	Represents a fractional number as a ratio of two IntegerTypes
 *	Not a 1:1 mapping of states to numbers, so it is not the most space-efficient data structure possible
 *	numerator() and denominator() are only guaranteed to return the same value until a non-const function is called
 *	Many functions may automatically reduce the fraction to lowest form to prevent overflows from occurring.
 *	If the fraction is reduced, it will still be considered equal to the unreduced fraction
 *	Range:		min(IntegerType) to max(BaseType) [signed or unsigned]
 *	Precision:	1 / max(IntegerType) [unsigned]
 *			 :	2 / max(IntegerType) [signed]
 *	sizeof(Fraction<IntegerType>) == sizeof(IntegerType) * 2
 */

template<typename IntegerType>
class Fraction{
	IntegerType _numerator;
	IntegerType _denominator;

	IntegerType _numerator_abs() const{
		return _numerator >= 0 ? _numerator : -_numerator;
	}

	IntegerType _denominator_abs() const{
		return _denominator >= 0 ? _denominator : -_denominator;
	}

	// Method to fully reduce to relative primes
	void _full_simplify(){
		// Based on the Euclidean algorithm
		IntegerType greater = (_numerator_abs() > _denominator_abs() ? _numerator : _denominator);
		IntegerType lesser = (_numerator_abs() < _denominator_abs() ? _numerator : _denominator);
		do{
			IntegerType remainder(greater % lesser);
			greater = lesser;
			lesser = remainder;
		}while(lesser);
		_numerator /= greater;
		_denominator /= greater;
	}

public:
	/// Construct a 0/1 Fraction
	Fraction() : _numerator(0), _denominator(1){}

	/// Construct a Fraction with a numerator and denominator.
	/**
	 *	@param numer Value to set the numerator to
	 *	@param denom Value to set the denominator to, creating a Fraction with a denominator of zero is undefined
	 */
	Fraction(IntegerType numer, IntegerType denom) : _numerator(numer), _denominator(denom){}
	/// Copy constructor
	/**
	 *	@param other The Fraction to copy.
	 */
	Fraction(const Fraction<IntegerType>& other) : _numerator(other._numerator), _denominator(other._denominator){}

	///	Create a fraction from a fixed-point number
	/**	
	 *	@param other Fixed-point number to copys
	 */
	template<count_type IntegerBits, count_type DecimalBits>
	Fraction(const FixedPoint<IntegerType, IntegerBits, DecimalBits>& other) : _numerator(other.i() * (1 << DecimalBits) / other.d()), _denominator(1 << DecimalBits){}

	/// Return the numerator of the Fraction.
	/**
	 *	@return The numerator
	 */
	IntegerType numerator() const{
		return _numerator;
	}

	/// Return the denominator of the Fraction.
	/**
	 *	@return The denominator
	 */
	IntegerType denominator() const{
		return _denominator;
	}

	/// Sets the numerator of the Fraction.
	/**
	 *	@param numer Value to set the numerator to
	 */
	void numerator(IntegerType numer){
		_numerator = numer;
	}

	/// Sets the denominator of the Fraction.
	/**
	 *	@param denom Value to set the denominator to, setting a Fraction's denominator to zero is undefined
	 */
	void denominator(IntegerType denom){
		_denominator = denom;
	}

	/// Sets both the numerator and the denominator of the Fraction.
	/**
	 *	@param numer Value to set the numerator to
	 *	@param denom Value to set the denominator to, setting a Fraction's denominator to zero is undefined
	 */
	void set(IntegerType numer, IntegerType denom){
		numerator(numer);
		denominator(denom);
	}

	/// Returns the integer portion of the Fraction
	/**
	 *	@return Integer portion
	 */
	IntegerType i() const{
		return _numerator / _denominator;
	}
	/// Returns the fractional portion (i.e. a fraction < 1)
	/**
	 *	@return Fractional portion
	 */
	Fraction f() const{
		return Fraction(_numerator % _denominator, _denominator);
	}

	/// Returns a Fraction with the given numerator equivalent to the original Fraction
	/**
	 *	The old and new Fractions may not be considered equal due to limited precision.
	 *	Converting between small numerators and denominators will result in significant accuracy loss.
	 *	(e.g. Fraction(2/1) to numerator 3 results in (3/1)
	 *	@param numer New Fraction numerator
	 *	@return Equivalent fraction
	 */
	Fraction convert_numerator(IntegerType numer) const{
		return Fraction(numer, (_denominator * numer) / _numerator);
	}

	/// Returns a Fraction with the given denominator equivalent to the original Fraction
	/**
	 *	The old and new Fractions may not be considered equal due to limited precision.
	 *	Converting between small numerators and denominators will result in significant accuracy loss.
	 *	(e.g. Fraction(1/2) to denominator 3 results in (1/3)
	 *	@param numer New Fraction numerator
	 *	@return Equivalent fraction
	 */
	Fraction convert_denominator(IntegerType denom) const{
		return Fraction((_numerator * denom) / _denominator, denom);
	}

	/// Simplifies the numerator and denominator of the Fraction
	/**	
	 *	Fractions are only guaranteed to remain simplified until the next non-const function
	 */
	void simplify(){
		_full_simplify();
	}


	Fraction<IntegerType>& operator+=(const Fraction<IntegerType>& other){
		_numerator *= other._denominator;
		_numerator += other._numerator * _denominator;
		_denominator *= other._denominator;

		_full_simplify();
		return *this;
	}

	Fraction<IntegerType>& operator-=(const Fraction<IntegerType>& other){
		#ifdef FIXEDPOINT_DEBUG
			if (){
				// Error (Underflow)
			}
		#endif
		_numerator *= other._denominator;
		_numerator -= other._numerator;
		_denominator *= other._denomiator;

		_full_simplify();
		return *this;
	}

	Fraction<IntegerType>& operator*=(const Fraction<IntegerType>& other){
		_numerator *= other._numerator;
		_denominator *= other._denominator;

		_full_simplify();
		return *this;
	}

	Fraction<IntegerType>& operator/=(const Fraction<IntegerType>& other){
		_numerator *= other._denominator;
		_denominator *= other._numerator;

		_full_simplify();
		return *this;
	}

	Fraction<IntegerType> operator+(const Fraction<IntegerType>& other) const{
		return (Fraction<IntegerType>(*this) += other);
	}

	Fraction<IntegerType> operator-(const Fraction<IntegerType>& other) const{
		return (Fraction<IntegerType>(*this) -= other);
	}

	Fraction<IntegerType> operator*(const Fraction<IntegerType>& other) const{
		return (Fraction<IntegerType>(*this) *= other);
	}

	Fraction<IntegerType> operator/(const Fraction<IntegerType>& other) const{
		return (Fraction<IntegerType>(*this) /= other);
	}

	Fraction<IntegerType>& operator=(const Fraction<IntegerType>& other) const{
		_numerator = other._numerator;
		_denominator = other._denominator;

		return *this;
	}

	bool operator==(const Fraction<IntegerType>& other) const{
		return (_numerator * other._denominator == _denominator * other._numerator);
	}

	bool operator!=(const Fraction<IntegerType>& other) const{
		return !operator==(other);
	}

	bool operator<(const Fraction<IntegerType>& other) const{
		return _numerator * other._denominator < other._numerator * _denominator;
	}

	bool operator<=(const Fraction<IntegerType>& other) const{
		// Copied in to save a small amount of performance by having only one comparison
		return _numerator * other._denominator <= other._numerator * _denominator;
	}

	bool operator>(const Fraction<IntegerType>& other) const{
		return !operator<=(other);
	}

	bool operator>=(const Fraction<IntegerType>& other) const{
		return !operator<(other);
	}

	// IntegerType operators
	Fraction<IntegerType>& operator+=(const IntegerType& other){
		_full_simplify();
		_numerator += other * _denominator;
		return *this;
	}
	Fraction<IntegerType>& operator-=(const IntegerType& other){
		_full_simplify();
		_numerator -= other * _denominator;
		return *this;
	}
	Fraction<IntegerType>& operator*=(const IntegerType& other){
		_full_simplify();
		_numerator *= other;
		return *this;
	}
	Fraction<IntegerType>& operator/=(const IntegerType& other){
		_full_simplify();
		_denominator *= other;
		return *this;
	}

	Fraction<IntegerType> operator+(const IntegerType& other) const{
		return Fraction(*this) += other;
	}
	Fraction<IntegerType> operator-(const IntegerType& other) const{
		return Fraction(*this) -= other;
	}
	Fraction<IntegerType> operator*(const IntegerType& other) const{
		return Fraction(*this) *= other;
	}
	Fraction<IntegerType> operator/(const IntegerType& other) const{
		return Fraction(*this) /= other;
	}

	// These non-const non-assignment operators aren't based on the assignment operators
	// so that _full_simplify() can be called on the left-side operator so both
	// get the benefit
	Fraction<IntegerType> operator+(const IntegerType& other){
		_full_simplify();
		return Fraction(*this)._numerator += other * _denominator;
	}
	Fraction<IntegerType> operator-(const IntegerType& other){
		_full_simplify();
		return Fraction(*this)._numerator -= other * _denominator;
	}
	Fraction<IntegerType> operator*(const IntegerType& other){
		_full_simplify();
		return Fraction(*this)._numerator *= other;
	}
	Fraction<IntegerType> operator/(const IntegerType& other){
		_full_simplify();
		return Fraction(*this)._denominator *= other;
	}

	bool operator==(const IntegerType& other) const{
		return (_numerator % _denominator == 0 && i() == other);
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
		return !(operator<=(other));
	}

	bool operator>=(const IntegerType& other) const{
		return !(operator<(other));
	}
};

#endif//H_FP_FRACTION
