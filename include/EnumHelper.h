#pragma once

#pragma once

#include <type_traits>

template <typename T>
struct EnumCastHelper {
	using type = std::underlying_type_t<T>;

	EnumCastHelper(T value) {
		this->value = static_cast<type>(value);
	}

	explicit operator bool() const {
		return value != type();
	}

	operator T() const {
		return static_cast<T>(this->value);
	}

	explicit operator type() const {
		return value;
	}

private:

	type value;
};

#define MAKE_ENUM_FLAGS(ENUM_NAME) \
	inline EnumCastHelper<ENUM_NAME> operator& (EnumCastHelper<ENUM_NAME> lhs, EnumCastHelper<ENUM_NAME> rhs) { \
		using type = std::underlying_type_t<ENUM_NAME>; \
		return static_cast<ENUM_NAME>(static_cast<type>(lhs) & static_cast<type>(rhs)); \
	} \
	\
	inline EnumCastHelper<ENUM_NAME> operator& (EnumCastHelper<ENUM_NAME> lhs, ENUM_NAME rhs) { \
		return lhs & EnumCastHelper<ENUM_NAME>(rhs); \
	} \
	\
	inline EnumCastHelper<ENUM_NAME> operator& (ENUM_NAME lhs, EnumCastHelper<ENUM_NAME> rhs) { \
		return EnumCastHelper<ENUM_NAME>(lhs) & rhs; \
	} \
	\
	inline EnumCastHelper<ENUM_NAME> operator& (ENUM_NAME lhs, ENUM_NAME rhs) { \
		return EnumCastHelper<ENUM_NAME>(lhs) & rhs; \
	} \
	\
	inline ENUM_NAME& operator&= (ENUM_NAME &lhs, ENUM_NAME rhs) { \
		return lhs = EnumCastHelper<ENUM_NAME>(lhs) & rhs; \
	} \
	\
	\
	inline EnumCastHelper<ENUM_NAME> operator| (EnumCastHelper<ENUM_NAME> lhs, EnumCastHelper<ENUM_NAME> rhs) { \
		using type = std::underlying_type_t<ENUM_NAME>; \
		return static_cast<ENUM_NAME>(static_cast<type>(lhs) | static_cast<type>(rhs)); \
	} \
	\
	inline EnumCastHelper<ENUM_NAME> operator| (EnumCastHelper<ENUM_NAME> lhs, ENUM_NAME rhs) { \
		return lhs | EnumCastHelper<ENUM_NAME>(rhs); \
	} \
	\
	inline EnumCastHelper<ENUM_NAME> operator| (ENUM_NAME lhs, EnumCastHelper<ENUM_NAME> rhs) { \
		return EnumCastHelper<ENUM_NAME>(lhs) | rhs; \
	} \
	 \
	inline EnumCastHelper<ENUM_NAME> operator| (ENUM_NAME lhs, ENUM_NAME rhs) { \
		return EnumCastHelper<ENUM_NAME>(lhs) | rhs; \
	} \
	\
	inline ENUM_NAME& operator|= (ENUM_NAME &lhs, ENUM_NAME rhs) { \
		return lhs = EnumCastHelper<ENUM_NAME>(lhs) | rhs; \
	} \
	\
	\
	inline EnumCastHelper<ENUM_NAME> operator~ (ENUM_NAME rhs) { \
		using type = std::underlying_type_t<ENUM_NAME>; \
		return static_cast<ENUM_NAME>(~static_cast<type>(rhs)); \
	} \

namespace details
{
	template <typename T>
	struct EnumCount_
	{
		enum : size_t { value = T::Count };
	};
}

template <typename T>
constexpr auto EnumCount = details::EnumCount_<T>::value;