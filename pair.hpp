#ifndef PAIR_HPP
#define PAIR_HPP

namespace ft
{

template <typename T1, typename T2>
struct pair
{
	typedef T1 first_type;
	typedef T2 second_type;

	T1 first;
	T2 second;

	/* Constructor */ pair() : first(), second()
	{
	}

	pair(const T1& a, const T2& b) : first(a), second(b)
	{
	}

	pair(const pair& p) : first(p.first), second(p.second)
	{
	}
};

template <typename T1, typename T2>
pair<T1, T2> make_pair(T1 x, T2 y)
{
	return pair<T1, T2>(x, y);
}

template <typename T1, typename T2>
bool operator==(const pair<T1, T2>& x, const pair<T1, T2>& y)
{
	return x.first == y.first && x.second == y.second;
}
template <typename T1, typename T2>
bool operator<(const pair<T1, T2>& x, const pair<T1, T2>& y)
{
	return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
}

template <typename T1, typename T2>
bool operator!=(const pair<T1, T2>& x, const pair<T1, T2>& y)
{
	return !(x == y);
}

template <typename T1, typename T2>
bool operator>(const pair<T1, T2>& x, const pair<T1, T2>& y)
{
	return y < x;
}

template <typename T1, typename T2>
bool operator<=(const pair<T1, T2>& x, const pair<T1, T2>& y)
{
	return !(y < x);
}

template <typename T1, typename T2>
bool operator>=(const pair<T1, T2>& x, const pair<T1, T2>& y)
{
	return !(x < y);
}

} // namespace std

#endif /* PAIR_HPP */
