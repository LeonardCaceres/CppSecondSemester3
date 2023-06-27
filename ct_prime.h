#pragma once

#include <type_traits>
#include <cstdint>

template <const uint64_t N, const uint64_t Lo = 0ull, const uint64_t Hi = N>
struct KSqrt {
  static const uint64_t kMid = (Lo + Hi + 1ull) / 2ull;
  static const uint64_t kValue =
      std::conditional_t<kMid <= N / kMid, KSqrt<N, kMid, Hi>, KSqrt<N, Lo, kMid - 1ull>>::kValue;
};

template <const uint64_t N, const uint64_t Guess>
struct KSqrt<N, Guess, Guess> {
  static const uint64_t kValue = Guess;
};

template <const uint64_t N>
const uint64_t kSqrtV = KSqrt<N>::kValue;

template <uint64_t T, uint64_t S, uint64_t R>
struct HasDivisorOnV;

template <uint64_t T, uint64_t S, uint64_t R>
const bool kHasDivisorOnV = HasDivisorOnV<T, S, R>::value;

template <uint64_t T, uint64_t S>
struct HasDivisorOnV<T, S, S> : std::conditional_t<T % S == 0, std::true_type, std::false_type> {};

template <uint64_t T, uint64_t S>
struct HasDivisorOnV<T, S, S + 1>
    : std::conditional_t<(T % S == 0) || (T % (S + 1) == 0), std::true_type, std::false_type> {};

template <uint64_t T, uint64_t S, uint64_t R>
struct HasDivisorOnV
    : std::conditional_t<HasDivisorOnV<T, S, (S + R) / 2>::value || HasDivisorOnV<T, (S + R) / 2 + 1, R>::value,
                         std::true_type, std::false_type> {};

template <const uint64_t N, const uint64_t T = 2, const uint64_t K = kSqrtV<N>>
const bool kIsPrimeV = !kHasDivisorOnV<N, T, K>;  // NOLINT

template <>
const bool kIsPrimeV<1> = false;  // NOLINT
template <>
const bool kIsPrimeV<2> = true;  // NOLINT
template <>
const bool kIsPrimeV<3> = true;  // NOLINT