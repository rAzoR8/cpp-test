// Author: Fabian Wahlster
// Copyright 2017
// fwahlster@tum.de

#ifndef CONCEPTS_H
#define CONCEPTS_H

//#include <stdint.h>
#include <algorithm>
#include <iterator>

namespace Concepts
{
	//template <typename T>
	//concept bool Addable = requires(T a, T b)
	//{ 
	//	{a + b}->T;
	//	{a += b}->T;
	//};

	template <typename T>
	concept bool Addable = requires(T a, T b) { {a + b}->T; };
	
	template <typename T>
	concept bool Addable = requires(T a, T b, T c) { {a += b + c}->T&; };

	template <typename U, typename V = U, typename W = U>
	concept bool Multipliable = requires(U a, V b) { {a * b}->W; {a *= b}->W };

	template <typename U, typename V = U, typename W = U>
	concept bool Divisible = requires(U a, V b) { {a / b}->W; {a /= b}->W };

	//template <typename U, int I = 0>
	//concept bool Indexable = requires(U a) { { a[I]} }; // {a[I]=b}

	//template <typename U, typename V, int I = 0>
	//concept bool IndexAssignable = requires(U a, V b) {{ a[I] = b }}

	template <typename T, typename Fun, typename Ret = void, typename... Args>
	concept bool HasFunc = requires(T& t, Fun f, Args ... args) { {(t.*f)(args ...) }->Ret; };
	
	template <typename T>
	using value_type = typename T::value_type;

	template <typename T>
	using iterator = typename T::iterator;

	template <typename Container>
	concept bool Sequence =
		requires(Container t)
	{
		typename value_type<Container>; // must have a value type
		typename iterator<Container>; // must have an iterator type (note: depending on constness of Container construct a type that returns a const_iterator)

		{std::begin(t)}->iterator<Container>; // must have begin & end
		{std::end(t)}->iterator<Container>; // more
	};

} // Concepts

#endif // !CONCEPTS_H
