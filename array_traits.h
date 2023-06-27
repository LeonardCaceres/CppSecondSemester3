#pragma once
#include <iostream>

template <typename T>
struct KIsArray {
  static const bool kValue = false;
};

template <typename T, size_t N>
struct KIsArray<T[N]> {
  static const bool kValue = true;
};

template <typename T>
struct KIsArray<T[]> {
  static const bool kValue = true;
};

template <typename T>
struct KIsArray<T*> {
  static const bool kValue = true;
};

template <typename T, size_t N>
struct KIsArray<T (*)[N]> {
  static const bool kValue = true;
};

template <typename T>
const bool kIsArrayV = KIsArray<T>::kValue;

template <typename T>
struct KSize {
  static const int kValue = 1;
};

template <typename T, size_t N>
struct KSize<T[N]> {
  static const int kValue = N;
};

template <typename T>
struct KSize<T[]> {
  static const int kValue = 0;
};

template <typename T>
const int kSizeV = KSize<T>::kValue;

template <typename T>
struct KRank {
  static const int kValue = 0;
  static const int kTotalsize = kSizeV<T>;
};

template <typename T, size_t N>
struct KRank<T[N]> {
  static const int kValue = KRank<T>::kValue + 1;
  static const int kTotalsize = KRank<T>::kTotalsize * kSizeV<T[N]>;
};

template <typename T>
struct KRank<T[]> {
  static const int kValue = KRank<T>::kValue + 1;
  static const int kTotalsize = KRank<T>::kTotalsize * kSizeV<T[]>;
};

template <typename T>
const int kRankV = KRank<T>::kValue;

template <typename T>
const int kTotalSizeV = KRank<T>::kTotalsize;

template <typename T>
struct RemoveArray {
  using Type = T;
};

template <typename T, size_t N>
struct RemoveArray<T[N]> {
  using Type = T;
};

template <typename T>
struct RemoveArray<T[]> {
  using Type = T;
};

template <typename T>
using RemoveArrayT = typename RemoveArray<T>::Type;

template <typename T>
struct RemoveAllArrays {
  using Type = T;
};

template <typename T, size_t N>
struct RemoveAllArrays<T[N]> {
  using Type = typename RemoveAllArrays<T>::Type;
};

template <typename T>
struct RemoveAllArrays<T[]> {
  using Type = typename RemoveAllArrays<T>::Type;
};

template <typename T>
using RemoveAllArraysT = typename RemoveAllArrays<T>::Type;