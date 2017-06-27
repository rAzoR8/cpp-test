// Author: Fabian Wahlster
// Copyright 2017
// fwahlster@tum.de

// /std:c++latest 

#include "StopWatch.h"
#include <vector>
#include <algorithm>
//#include <ppl.h>
#include "CRTP.h"

//https://github.com/STEllAR-GROUP/hpx

static constexpr size_t g_Iterations = 100000000;

// genrate some data
void GenerateData(std::vector<double>& _Values)
{
	_Values.resize(g_Iterations);

	double fTaylorSum = 0.0;
	double fSign = 1.0;

	for (size_t i = 0; i < g_Iterations; ++i)
	{
		fTaylorSum += fSign / (2.0*i + 1.0);
		_Values[i] = fTaylorSum;
		fSign *= -1.0;
	}

	//return 4.0*fTaylorSum;
};

//---------------------------------------------------------------------------------------------------

void IteratorBenchmark(std::vector<double>& _Values)
{
	GenerateData(_Values);

	{
		StopWatch<> Watch("std for");

		for (double& fValue : _Values)
		{
			fValue *= fValue;
		}
	}

	{
		StopWatch<> Watch("iterator checking end");

		for (std::vector<double>::iterator it = _Values.begin(); it != _Values.end(); ++it)
		{
			double& fValue = *it;
			fValue *= fValue;
		}
	}

	{
		StopWatch<> Watch("pre-acquired iterators");

		for (std::vector<double>::iterator it = _Values.begin(), end = _Values.end(); it != end; ++it)
		{
			double& fValue = *it;
			fValue *= fValue;
		}
	}

	{
		StopWatch<> Watch("indexing checking end");

		for (size_t i = 0ull; i < _Values.size(); ++i)
		{
			double& fValue = _Values[i];
			fValue *= fValue;
		}
	}

	{
		StopWatch<> Watch("pre-acquired indexing");
		size_t uSize = _Values.size();
		for (size_t i = 0ull; i < uSize; ++i)
		{
			double& fValue = _Values[i];
			fValue *= fValue;
		}
	}

	{
		StopWatch<> Watch("pre-acquired indexing using at()");
		size_t uSize = _Values.size();
		for (size_t i = 0ull; i < uSize; ++i)
		{
			double& fValue = _Values.at(i);
			fValue *= fValue;
		}
	}

	{
		StopWatch<> Watch("memory indexing");
		double* pValue = _Values.data();
		double *pEnd = pValue + _Values.size();

		for (;pValue < pEnd; ++pValue)
		{
			double& fValue = *pValue;
			fValue *= fValue;
		}
	}

	{
		StopWatch<> Watch("ppl parallel for");
		//concurrency::parallel_for_each(_Values.begin(), _Values.end(), [](double& fValue) {fValue *= fValue; });
	}
}
//---------------------------------------------------------------------------------------------------

void SortBenchmark(std::vector<double>& _Values)
{
	GenerateData(_Values);

	{
		StopWatch<> Watch("standard squential sort");
		std::sort(_Values.begin(), _Values.end());
	}

	GenerateData(_Values);

	{
		StopWatch<> Watch("ppl parallel sort");
		//concurrency::parallel_sort(_Values.begin(), _Values.end());
	}

	GenerateData(_Values);

	{
		StopWatch<> Watch("ppl parallel buffered sort");
		// needs n*sizeof(T) additional memory
		//concurrency::parallel_buffered_sort(_Values.begin(), _Values.end());
	}

	GenerateData(_Values);

	{
		StopWatch<> Watch("ppl parallel radixsort");
		// needs projection function T -> size_t, i just used reinterpret
		//concurrency::parallel_radixsort<std::allocator<double>>(_Values.begin(), _Values.end(), [](const double& _fValue) {return *reinterpret_cast<const uint64_t*>(&_fValue); });
	}
}

//---------------------------------------------------------------------------------------------------

 #include "EnumOps.h"
void BitOpsOnEnumClassExample()
{
	enum class Flag : uint32_t
	{
		kFlag_1 = 1 << 0u,
		kFlag_2 = 1 << 1u,
		kFlag_3 = 1 << 2u,
	};

	Flag f(Flag::kFlag_1 | Flag::kFlag_2);

	switch (f)
	{
	case Flag::kFlag_1 | Flag::kFlag_2:
		break;
	case Flag::kFlag_2:
		break;
	case Flag::kFlag_3:
		break;
	default:
		break;
	}
}

//---------------------------------------------------------------------------------------------------

#include "CompileTimeStringHash.h"

constexpr std::string_view view = make_string_view("test123");
constexpr size_t uTest1 = "test"_hash;

template <bool yes>
constexpr bool is_compile_time();

template <>
constexpr bool is_compile_time<true>() { return true; }

template <>
constexpr bool is_compile_time<false>() { return false; }

void CompileTimeStringHashExample()
{
	constexpr size_t uTest2 = const_string_hash("test");
	constexpr bool isCompileTime = is_compile_time<uTest1 == uTest2>();

	bool butIsIt = is_compile_time<make_string_view("hallo") != make_string_view("hallo")>();

	//if constexpr (std::is_integer ...) { //integerstuff }
	//else if constexpr (std::is_floating_point ...) { //floatingpointstuff }
	//else { // NaN ;) }
}

#include <variant>
// http://en.cppreference.com/w/cpp/utility/variant/visit

//template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
//template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

struct Matcher
{
	void operator()(int arg) { std::cout << "int " << arg << std::endl; }
	void operator()(double arg) { std::cout << "double " << arg << std::endl; }
	void operator()(std::string arg) { std::cout << "string " << arg << std::endl; }
};

void VariantExample()
{
	using var_t = std::variant<int, double, std::string>;

	std::vector<var_t> Vec = { "enginestuff", 1.337, 1337, "1337" };

	for (const var_t& v: Vec)
	{
		std::visit(Matcher(), v);
		std::visit([](auto&& arg) {std::cout << arg << std::endl; }, v);

		if (auto pVal = std::get_if<int>(&v))
		{
			std::cout << "variant value: " << *pVal << '\n';
		}
		else
		{
			std::cout << "failed to get value!" << '\n';
		}

		//using T = std::decay_t<decltype(v)>;
		//if constexpr (std::is_same_v<T, int>)
		//{
		//	std::cout << "int " << std::get<T>(v) << std::endl;
		//}
		//else if constexpr (std::is_same_v<T, double>)
		//{
		//	std::cout << "double " << std::get<T>(v) << std::endl;
		//}
		//else if constexpr (std::is_same_v < T, std::string > )
		//{
		//	std::cout << "string " << std::get<T>(v) << std::endl;
		//}
	}
}

//---------------------------------------------------------------------------------------------------

#include <any>

enum Types
{
	Type_Int,
	Type_String,
	Type_Double
};

std::any SomeFunc(Types kType)
{
	switch (kType)
	{
	case Type_Int:
		return std::make_any<int>(1337);
	case Type_String:
		return std::make_any<std::string>("1.337");
	case Type_Double:
		return std::make_any<double>(1.337);
	default:
		return {};
	}
}

void AnyExample()
{
	std::any obj{ "hall" };

	if (obj.has_value())
	{
	
		std::cout << sizeof(obj);
		try
		{
			std::cout << std::any_cast<const char*>(obj) << std::endl;
			//std::any_cast<int>(obj);
		}
		catch (std::bad_any_cast&)
		{
			std::cout << "Wrong type " << obj.type().name() << std::endl;
		}
	}

	std::any nothing;

	if (nothing.has_value())
	{
		std::cout << "oh boi\n";
	}

	nothing = SomeFunc(Type_String);

	if (nothing.has_value())
	{
		std::cout << std::any_cast<std::string>(nothing) << std::endl;
	}
}

//---------------------------------------------------------------------------------------------------

#include <optional>

std::optional<std::string> SomeOtherFunc(bool bCondition)
{
	if (bCondition)
	{
		return "Munich Game-Engine Devs";
	}
	else
	{
		return {};
	}
}

void OptionalExample()
{
	std::cout << SomeOtherFunc(false).value_or("empty") << std::endl;
	std::cout << SomeOtherFunc(true).value() << std::endl;
}

//---------------------------------------------------------------------------------------------------

//#include <map>
#include <tuple>

// parameter folds
template <typename... Args>
void print(Args&&... args)
{
	(std::cout << ... << args) << '\n';

	// std::cout << args1 << args2 << args3 << ...
}
#include <map>


void MiscExamples()
{
	/// structured bindings:
	struct MyStruct 
	{
		int x;
		double y;
	};

	//print("hallo", 1.f, 1337);

	//MyStruct h();
	//auto [u, v] = h();
	//auto[a, b] = std::make_pair(1, 2.0);

	//std::map<std::string, double> myMap;

	//for (auto[key, val] : myMap)
	//{
	//	std::cout << key << ": " << val << std::endl;
	//}

	/// in scope inits

	//if (auto Opt = SomeOtherFunc(true); Opt.has_value())
	//{
	//	// do something with opt
	//}

	std::tuple<int, int, std::string> t(1, 2, "Hallo");

	//std::tuple t(1, 2, "Hallo");
	
	//print(1.f, "Hallo");

	/// static initialization

	//class SomeClass
	//{
	//	static inline const std::string ComplexType = "test";	
	//};
	
	//if (__has_include("SomeFile.h"))
	//{
	//
	//}
	//else if (__has_include("CompileTimeStringHash.h"))
	//{
	//	std::cout << "Good for you!\n";
	//}

	/// also in cpp 17
	// std::byte
	// std::filesystem based on boost::filesystem
	// constexpr lambdas
	// ranged-for loop supports now ranges with different begin and end types => see Range v3 https://github.com/ericniebler/range-v3


	/// NOT in cpp 17
	// Concepts :(((((
	// Modules
	// Coroutines
	// Static reflection
}

/// namespaces
namespace A {
	namespace B {
		namespace C {

		}
	}
}

namespace A::B::C
{

}

//template <class auto v1, decltype(v1)... vs>
//struct typed_value_list
//{
//	using T = decltype(v1);
//
//	T VS[sizeof...(vs) + 1];
//};

//---------------------------------------------------------------------------------------------------

int main()
{
	//std::vector<double> Values;

	//SortBenchmark(Values);

	//std::cout << std::endl;

	//IteratorBenchmark(Values);

	//BitOpsOnEnumClassExample();

	//CompileTimeStringHashExample();
	
	//VariantExample();

	//AnyExample();

	//OptionalExample();

	MyRenderPass pass;
	pass.RenderBatch({}, {});
	
	//system("pause");

	return 0;
}