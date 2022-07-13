#ifndef REMOVE_CV_HPP
#define REMOVE_CV_HPP


template <typename T>
struct remove_cv
{
	typedef T type;
};

template<typename T>
struct remove_cv<const T>
{
	typedef T type;
};


#endif /* REMOVE_CV_HPP */
