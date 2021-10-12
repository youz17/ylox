#pragma once

#include <algorithm>
#include <array>
#include <variant>
#include <type_traits>

namespace tool
{
	template<typename ...Ts>
	using tagged_union = std::variant<Ts...>;

	namespace _impl
	{
		// WTF: `std::declval` will boom out some error messages when really called,
		// even though there're already some critical errors. So we implement a quiet one.
		template<typename T>
		constexpr T quiet_declval() { return *static_cast<T*>(nullptr); }
		template<>
		constexpr void quiet_declval<void>() {}

		template<typename R, typename ...Ts, typename U, typename ...Fs>
		R match_impl(U&& u, Fs ...arms) {
			struct overloaded : public Fs... {
				using Fs::operator()...;
			};
			using CheckArgs = std::conjunction<std::is_invocable<overloaded, Ts>...>;
			static_assert(CheckArgs::value, "Missing matching cases");
			if constexpr (!CheckArgs::value)
				return quiet_declval<R>(); // suppress more errors
			else
			{
				using CheckRet = std::conjunction<
					std::is_convertible<std::invoke_result_t<overloaded, Ts>, R>...
				>;
				static_assert(CheckRet::value, "Return type not compatible");
				if constexpr (!CheckRet::value)
					return quiet_declval<R>(); // suppress more errors
				else
				{
					overloaded dispatcher{ arms... };
					return std::visit([&](auto&& var) -> R
						{
							return dispatcher(std::forward<decltype(var)>(var));
						}, std::forward<U>(u));
				}
			}
		}

	} // namespace _impl

	template<typename R = void, typename ...Ts, typename ...Fs>
	R match(const tagged_union<Ts...>& u, Fs ...arms) {
		return _impl::match_impl<R, const Ts &...>(u, arms...);
	}

	template<typename R = void, typename ...Ts, typename ...Fs>
	R match(tagged_union<Ts...>&& u, Fs ...arms) {
		return _impl::match_impl<R, Ts &&...>(std::move(u), arms...);
	}

	template <typename T, std::size_t N>
	constexpr std::array<T, N> Sort(std::array<T, N> array) {
		std::sort(array.begin(), array.end());
		return array;
	}

	template<typename VariantType, typename T, std::size_t index = 0>
	constexpr std::size_t variant_index() {
		if constexpr (index == std::variant_size_v<VariantType>)
			return index;
		else if constexpr (std::is_same_v<std::variant_alternative_t<index, VariantType>, T>)
			return index;
		else
			return variant_index<VariantType, T, index + 1>();
	}

} // namespace tool
