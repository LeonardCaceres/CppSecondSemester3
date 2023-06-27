#pragma once
#include <iostream>
#include <type_traits>

template <class T, class = void>
struct KIsDereferenceable {
  static const bool kValue = false;
};

template <class T>
struct KIsDereferenceable<T, std::void_t<decltype(*std::declval<T>())>> {
  static const bool kValue = true;
};

template <class T>
const bool kIsDereferenceableV = KIsDereferenceable<T>::kValue;

template <class T, class = void>
struct KIsIncrementable {
  static const bool kValue = false;
};

template <class T>
struct KIsIncrementable<T, std::void_t<decltype(++std::declval<T&>(), std::declval<T&>()++)>> {
  static const bool kValue = true;
};

template <class T>
const bool kIsIncrementableV = KIsIncrementable<T>::kValue;

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

template <class T, class = void>
struct KIsArrowDereferenceable {
  static const bool kValue = false;
};

template <class T>
struct KIsArrowDereferenceable<T, std::void_t<decltype(std::declval<T>().operator->())>> {
  static const bool kValue = true;
};

template <class T>
const bool kIsArrowDereferenceableV = kIsDereferenceableV<T> || KIsArrowDereferenceable<T>::kValue;

template <class T, class = void>
struct KIsEqualityComparable {
  static const bool kValue = false;
};

template <class T>
struct KIsEqualityComparable<T, std::void_t<decltype((std::declval<T>() == std::declval<T>()) == true)>> {  // NOLINT
  static const bool kValue = true;
};

template <class T, class = void>
struct KIsBool {
  static const bool kValue = false;
};

template <class T>
struct KIsBool<T, std::void_t<decltype((std::declval<T>()))>> {
  static const bool kValue = true;
};

template <class T>
const bool kIsEqualityComparableV = KIsEqualityComparable<T>::kValue;

template <class T, class = void>
struct KIsOrdered {
  static const bool kValue = false;
};

template <class T>
struct KIsOrdered<T, std::void_t<decltype((std::declval<T>() < std::declval<T>()) == true)>> {  // NOLINT
  static const bool kValue = true;
};

template <class T>
const bool kIsOrderedV = KIsOrdered<T>::kValue;

template <class T, class = void>
struct KIsSubtractable {
  static const bool kValue = false;
};

template <class T>
struct KIsSubtractable<T, std::void_t<decltype(std::declval<T>() - std::declval<T>())>> {
  static const bool kValue = true;
  using Type = decltype(std::declval<T>() - std::declval<T>());
};

template <class T>
const bool kIsSubtractableV = KIsSubtractable<T>::kValue;

template <class T>
using DifferenceType = typename KIsSubtractable<T>::Type;

template <class T, class = void>
struct KHasIntegralArithmetic {
  static const bool kValue = false;
};

template <class T>
struct KHasIntegralArithmetic<T, std::void_t<decltype(std::declval<T&>() += std::declval<DifferenceType<T>>())>> {
  static const bool kValue = true;
};

template <class T, class = void>
struct KDifferentTypeCheck {
  static const bool kValue = false;
};

template <class T>
struct KDifferentTypeCheck<T, std::void_t<decltype(std::declval<T&>() -= std::declval<DifferenceType<T>>())>> {
  static const bool kValue = true;
};

template <class T, class = void>
struct IsInteger {
  static const bool kValue = false;
};

template <class T>
struct IsInteger<T, std::void_t<decltype(std::declval<DifferenceType<T>>() + 1)>> {
  static const bool kValue = true;
};
template <class T>
const bool kHasIntegralArithmeticV =
    KHasIntegralArithmetic<T>::kValue && KDifferentTypeCheck<T>::kValue && IsInteger<T>::kValue;

template <class T, class = void>
struct KIsSubscriptable {
  static const bool kValue = false;
};

template <class T>
struct KIsSubscriptable<T, std::void_t<decltype(std::declval<T>()[std::declval<int>()])>> {
  static const bool kValue = true;
};

template <class T>
const bool kIsSubscriptableV = (KIsSubscriptable<T>::kValue || kIsDereferenceableV<T>)&&IsInteger<T>::kValue;

template <class T, class = void>
struct IsIterator {
  static const bool kValue = false;
};

template <class T>
struct IsIterator<T, std::void_t<decltype(std::declval<T&>() = std::declval<T const&>()), decltype(std::declval<T>()),
                                 decltype(std::declval<T>().~T()), decltype(T(std::declval<const T&>()))>> {
  static const bool kValue = true;
};

template <class T>
const bool kIsIteratorV = IsIterator<T>::kValue && kIsIncrementableV<T> && kIsDereferenceableV<T>;

template <class T, class = void>
struct IterEq {
  static const bool kValue = false;
};

template <class T>
struct IterEq<T, std::void_t<decltype(std::declval<T>() == std::declval<T>())>> {
  static const bool kValue = true;
};

template <class T>
const bool kIsInputIteratorV =
    kIsIteratorV<T> && (KIsArrowDereferenceable<T>::kValue || kIsDecrementableV<T>)&&IterEq<T>::kValue;

template <class T, class = void>
struct DefConstr {
  static const bool kValue = false;
};

template <class T>
struct DefConstr<T, std::void_t<decltype((T()))>> {
  static const bool kValue = true;
};

template <class T>
const bool kIsForwardIteratorV = kIsInputIteratorV<T> && DefConstr<T>::kValue;

template <class T>
const bool kIsBidirectionalIteratorV = kIsForwardIteratorV<T> && kIsDecrementableV<T>;

template <class T>
const bool kIsRandomAccessIteratorV = kIsBidirectionalIteratorV<T> && KIsSubscriptable<T>::kValue &&
                                      kHasIntegralArithmeticV<T> && kIsSubtractableV<T> && kIsOrderedV<T>;