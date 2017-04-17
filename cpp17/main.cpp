// /std:c++latest 

#include "StopWatch.h"
#include <vector>
#include <algorithm>
#include <ppl.h>
//#include <experimental/coroutine>

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
		concurrency::parallel_for_each(_Values.begin(), _Values.end(), [](double& fValue) {fValue *= fValue; });
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
		concurrency::parallel_sort(_Values.begin(), _Values.end());
	}

	GenerateData(_Values);

	{
		StopWatch<> Watch("ppl parallel buffered sort");
		// needs n*sizeof(T) additional memory
		concurrency::parallel_buffered_sort(_Values.begin(), _Values.end());
	}

	GenerateData(_Values);

	{
		StopWatch<> Watch("ppl parallel radixsort");
		// needs projection function T -> size_t, i just used reinterpret
		concurrency::parallel_radixsort<std::allocator<double>>(_Values.begin(), _Values.end(), [](const double& _fValue) {return *reinterpret_cast<const uint64_t*>(&_fValue); });
	}
}

//---------------------------------------------------------------------------------------------------

// #include "EnumOps.h"
void BitOpsOnEnumClassExample()
{
	enum class Flag : uint32_t
	{
		kFlag_1 = 1 << 0,
		kFlag_2 = 1 << 1,
		kFlag_3 = 1 << 2,
	};

	//Flag f(Flag::kFlag_1 | Flag::kFlag_2);

	//switch (f)
	//{
	//case Flag::kFlag_1&Flag::kFlag_2:
	//	break;
	//case Flag::kFlag_2:
	//	break;
	//case Flag::kFlag_3:
	//	break;
	//default:
	//	break;
	//}
}

//---------------------------------------------------------------------------------------------------

#include "CompileTimeStringHash.h"

//constexpr std::string_view view = std::string_view("test123");
//constexpr size_t uTest1 = const_string_hash("test");

template <bool yes>
constexpr bool is_compile_time();

template <>
constexpr bool is_compile_time<true>() { return true; }

template <>
constexpr bool is_compile_time<false>() { return false; }

void CompileTimeStringHashExample()
{
	//constexpr size_t uTest2 = const_string_hash("test");
	//constexpr bool isCompileTime = is_compile_time<uTest1 == uTest2>();

	//bool butIsIt = is_compile_time<make_string_view("hallo") != make_string_view("hallo")>();

	//if constexpr (std::is_integer ...) { //integerstuff }
	//else if constexpr (std::is_floating_point ...) { //floatingpointstuff }
	//else { // NaN ;) }
}


void main()
{
	std::vector<double> Values;

	//SortBenchmark(Values);

	std::cout << std::endl;

	//IteratorBenchmark(Values);

	BitOpsOnEnumClassExample();

	CompileTimeStringHashExample();

	system("pause");
}