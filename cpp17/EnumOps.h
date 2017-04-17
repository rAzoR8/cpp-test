#ifndef ENUMOPS_H
#define ENUMOPS_H

#include <type_traits>

template <typename EnumType, typename = std::enable_if_t<std::is_enum_v<EnumType>>>
constexpr EnumType operator|(EnumType f0, EnumType f1)
{
	return EnumType(std::underlying_type_t<EnumType>(f0) | std::underlying_type_t<EnumType>(f1));
}

template <typename EnumType, typename = std::enable_if_t<std::is_enum_v<EnumType>>>
constexpr EnumType operator&(EnumType f0, EnumType f1)
{
	return EnumType(std::underlying_type_t<EnumType>(f0) & std::underlying_type_t<EnumType>(f1));
}

template <typename EnumType, typename = std::enable_if_t<std::is_enum_v<EnumType>>>
constexpr EnumType operator^(EnumType f0, EnumType f1)
{
	return EnumType(std::underlying_type_t<EnumType>(f0) ^ std::underlying_type_t<EnumType>(f1));
}

#endif // !ENUMOPS_H
