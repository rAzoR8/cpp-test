#ifndef COMPILETIMESTRINGHASH_H
#define COMPILETIMESTRINGHASH_H

#include <string_view>

template<typename char_t>
inline constexpr size_t const_string_hash_impl(const std::basic_string_view<char_t> _View)
{	// FNV-1a hash function for bytes in [_First, _First + _Count)
#if defined(_WIN64)
	static_assert(sizeof(size_t) == 8, "This code is for 64-bit size_t.");
	const size_t _FNV_offset_basis = 14695981039346656037ULL;
	const size_t _FNV_prime = 1099511628211ULL;

#else /* defined(_WIN64) */
	static_assert(sizeof(size_t) == 4, "This code is for 32-bit size_t.");
	const size_t _FNV_offset_basis = 2166136261U;
	const size_t _FNV_prime = 16777619U;
#endif /* defined(_WIN64) */

	size_t _Val = _FNV_offset_basis;
	for (size_t _Next = 0; _Next < _View.size(); ++_Next)
	{	// fold in another byte
		_Val ^= (size_t)_View[_Next];
		_Val *= _FNV_prime;
	}
	return (_Val);
}

template <typename char_t, size_t Size>
constexpr std::basic_string_view<char_t> make_string_view(const char_t(&str)[Size]) { return std::basic_string_view<char_t>(str, Size); }

constexpr std::string_view operator "" _sv(const char *_Str, size_t _Len)
{	
	return std::string_view(_Str, _Len);
}

constexpr std::wstring_view operator "" _sv(const wchar_t *_Str, size_t _Len)
{
	return std::wstring_view(_Str, _Len);
}

constexpr std::u16string_view operator "" _sv(const char16_t *_Str, size_t _Len)
{
	return std::u16string_view(_Str, _Len);
}

constexpr std::u32string_view operator "" _sv(const char32_t *_Str, size_t _Len)
{
	return std::u32string_view(_Str, _Len);
}

template <typename char_t, size_t Size>
constexpr size_t const_string_hash(const char_t(&str)[Size]) { return const_string_hash_impl(make_string_view(str)); }

constexpr size_t operator "" _hash(const char *_Str, size_t _Len)
{
	return const_string_hash_impl(std::string_view(_Str, _Len));
}

constexpr size_t operator "" _hash(const wchar_t *_Str, size_t _Len)
{
	return const_string_hash_impl(std::wstring_view(_Str, _Len));
}

constexpr size_t operator "" _hash(const char16_t *_Str, size_t _Len)
{
	return const_string_hash_impl(std::u16string_view(_Str, _Len));
}

constexpr size_t operator "" _hash(const char32_t *_Str, size_t _Len)
{
	return const_string_hash_impl(std::u32string_view(_Str, _Len));
}

//template <typename char_t>
//constexpr bool operator==(const std::basic_string_view<char_t> l, const std::basic_string_view<char_t> r)
//{
//	if (l.size() != r.size())
//	{
//		return false;
//	}
//
//	for (size_t i = 0; i < l.size(); ++i)
//	{
//		if (l[i] != r[i])
//		{
//			return false;
//		}
//	}
//
//	return true;
//}
//
//template <typename char_t>
//constexpr bool operator!=(const std::basic_string_view<char_t> l, const std::basic_string_view<char_t> r)
//{
//	return !(l == r);
//}

#endif // !COMPILETIMESTRINGHASH_H
