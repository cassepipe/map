#ifndef IS_INTEGRAL_HPP
#define IS_INTEGRAL_HPP

#include "remove_cv.hpp"


//Checks whether T is an integral type. 
//Provides the member constant value which is equal to true, if T is the type bool, char, char16_t, char32_t,
//wchar_t, short, int, long, long long, or any implementation-defined extended integer types, including any signed, unsigned, and cv-qualified variants. 
//Otherwise, value is equal to false.

template <typename T>
struct is_integral_helper
{
	static bool const value = false; // Can't initialize without static in c++98
};

//Specializations
//
template <>
struct is_integral_helper<bool>
{
	static bool const value = true; 
};

template <>
struct is_integral_helper<char>
{
	static bool const value = true; 
};

template <>
struct is_integral_helper<char16_t>
{
	static bool const value = true; 
};

template <>
struct is_integral_helper<char32_t>
{
	static bool const value = true; 
};

template <>
struct is_integral_helper<wchar_t>
{
	static bool const value = true; 
};

template <>
struct is_integral_helper<short>
{
	static bool const value = true; 
};
template <>
struct is_integral_helper<int>
{
	static bool const value = true; 
};
template <>
struct is_integral_helper<long>
{
	static bool const value = true; 
};
template <>
struct is_integral_helper<long long>
{
	static bool const value = true; 
};
template <>
struct is_integral_helper<unsigned char>
{
	static bool const value = true; 
};
template <>
struct is_integral_helper<unsigned short>
{
	static bool const value = true; 
};

template <>
struct is_integral_helper<unsigned int>
{
	static bool const value = true; 
};

template <>
struct is_integral_helper<unsigned long>
{
	static bool const value = true; 
};

template <>
struct is_integral_helper<unsigned long long>
{
	static bool const value = true; 
};

// The trick for const values

template <typename T>
struct is_integral : public is_integral_helper< typename remove_cv<T>::type >
{
};

#endif /* IS_INTEGRAL_HPP */
