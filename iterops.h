#pragma once

#include <iostream>
#include <iterator>
#include <type_traits>

template <class T, class = void>
struct KIsDecrementable {
  static const bool kValue = false;
};

template <class T>
struct KIsDecrementable<T, std::void_t<decltype(--std::declval<T&>(), std::declval<T&>()--)>> {
  static const bool kValue = true;
};

template <class T>
const bool kIsDecrementableV = KIsDecrementable<T>::kValue;

template <typename Iter>
void Advance(Iter& iter, int64_t dist) {
  if constexpr (std::is_same_v<std::random_access_iterator_tag,
                               typename std::iterator_traits<Iter>::iterator_category>) {
    iter += dist;
  } else {
    if (dist > 0) {
      while (dist-- > 0) {
        ++iter;
      }
    } else {
      if constexpr (std::is_base_of_v<std::bidirectional_iterator_tag,
                                      typename std::iterator_traits<Iter>::iterator_category>) {
        while (dist++ < 0) {
          --iter;
        }
      }
    }
  }
}

template <typename Iter>
Iter Next(Iter iter, int64_t step = 1) {
  for (int64_t i = 0; i < step; ++i) {
    ++iter;
  }
  return iter;
}

template <typename Iter>
Iter Prev(Iter iter, int64_t step = 1) {
  for (int64_t i = 0; i < step; ++i) {
    --iter;
  }
  return iter;
}

template <typename Iter>
typename std::iterator_traits<Iter>::difference_type Distance(Iter first, Iter last) {
  if constexpr (std::is_same_v<std::random_access_iterator_tag,
                               typename std::iterator_traits<Iter>::iterator_category>) {
    return last - first;
  } else {
    typename std::iterator_traits<Iter>::difference_type dist = 0;
    while (first != last) {
      ++dist;
      ++first;
    }
    return dist;
  }
}