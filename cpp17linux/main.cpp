// Author: Fabian Wahlster
// Copyright 2017
// fwahlster@tum.de

#include "Concepts.h"
#include <iostream>
#include <vector>
#include <string>

template <typename Iterator, typename Func>
void transform(Iterator begin, Iterator end, Func fun)
{
	for (Iterator it = begin; it != end; ++it)
	{
		*it = fun(*it);
	}
}

template <typename T> requires Concepts::Addable<T>
T Add(const T& a, const T& b) { return a + b; }

template <typename T> requires Concepts::Multipliable<T>
T Multiply(const T& a, const T& b) { return a * b; }

struct func
{
	void test() { std::cout << "test()" << std::endl; }
};

template <typename T> 
concept bool Drawable = requires(T t) { t.draw(); };

template <typename T> requires Drawable<T>
void render(T t) { return t.draw(); }

template <typename T> requires Concepts::Sequence<T>
void stuff(T seq)
{
	for (const auto& v : seq)
	{
		std::cout << v;
	}
}

struct bla
{
	inline bla operator +(const bla& _other) const { return bla{val+_other.val}; }
	inline bla& operator +=(const bla& _other) { val += _other.val; return *this; }

	int val;
};

int main()
{
	std::vector<int> ints = { 1, 2 ,3 ,42,1337 };

	auto a = Add(1, 4);
	//auto b = Add(ints, ints);
	
	auto c = Add(bla{ 1 }, bla{ 2 });

	auto d = Multiply(1.f, 2.f);

	Concepts::Addable x1 = 1ull + 4ull;
	size_t s = sizeof(x1);

	std::vector<int> vec = { 1,2,3,4,5 };

	stuff(vec);

	return 0;
}