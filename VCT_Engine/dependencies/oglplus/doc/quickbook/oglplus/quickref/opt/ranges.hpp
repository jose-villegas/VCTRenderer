/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_opt_ranges_begin
namespace ranges {
//]
//[oglplus_opt_ranges_IsRange
template <typename Range>
struct IsRange
{
	typedef __Unspecified Type; /*<
	[^std::true_type] if [^Range] conforms to the __Range concept or
	[^std::false_type] otherwise.
	>*/
};
//]

//[oglplus_opt_ranges_AnyRange
template <typename Element>
class AnyRange
{
public:
	AnyRange(void);

	template <typename Range>
	AnyRange(__Range range); /*<
	Construction and assignment from a [^Range] type.
	>*/
	template <typename Range>
	AnyRange& operator = (const __Range& range);

	AnyRange(const AnyRange& other); /*<
	Copy and move construction.
	>*/
	AnyRange(AnyRange&& temp);

	AnyRange& operator = (const AnyRange& other); /*<
	Copy and move assignment.
	>*/
	AnyRange& operator = (AnyRange&& other);

	typedef Element ValueType; /*< The __Range interface. >*/
	bool Empty(void) const;
	size_t Size(void) const;
	void Next(void);
	ValueType Front(void) const;
};

template <typename Range>
__AnyRange<typename __Range::ValueType> EraseType(__Range range); /*<
Erases the real type of the range passed as argument.
>*/
//]

//[oglplus_opt_ranges_funcs
template <typename Range>
__Range Find(__Range range, typename __Range::ValueType value); /*<
Traverses a range and stops either if the range
is empty or if the specified value is found. The resulting
range is returned.
>*/

template <typename Range, typename Predicate>
__Range FindIf(__Range range, Predicate predicate) /*<
Finds the first a value satisfying a predicate in a range.
This function traverses a range and stops either if the range
is empty or if a value satisfying the predicate is found.
The resulting range is returned.
>*/

template <typename Range>
bool Contains(__Range range, typename __Range::ValueType value); /*<
Returns true if [^range] contains at least one element with
the specified value.
>*/

template <typename Range, typename Predicate>
bool Has(__Range range, Predicate predicate); /*<
Returns true if [^range] contains at least one element
satisfying the specified [^predicate].
>*/

template <typename Range, typename Transf>
__Range Transform(__Range range, Transf transform); /*<
Transforms the elements in a range by an unary function.
[^Transform] returns a range whose [^Front] member function returns
the [^Front] value of the original [^range] transformed by [^transform].
>*/

template <typename Range, typename State, typename Func>
State Fold(__Range range, State state, Func functor); /*<
Folds a [^range] by using a binary [^functor] and a [^state] value.
Fold gradually updates the [^state] value by successively calling the specified
[^functor] on all elements with [^state] as the first argument and the current
[^Front] element of the [^range] as the second argument. The result of each call
replaces the value stored in [^state]. After the whole [^range] is traversed
the resulting [^state] is returned.
>*/

template <typename Range, typename Predicate>
size_t CountIf(Range range, Predicate predicate); /*<
Counts and returns the number of elements in a __Range satisfying
the specified [^predicate].
>*/

template <typename Range, typename Predicate>
__Range OnlyIf(__Range range, Predicate pred); /*<
The [^OnlyIf] function returns a __Range that contains only those elements
of the original [^range], which satisfy the specified [^predicate].
>*/

template <typename Element, typename R1, typename R2>
__Range Concatenate(R1 r1, R2 r2); /*<
Returns a range that is a concatenation of the ranges [^r1] and [^r2].
The [^ValueType] of both ranges must be explicitly convertible to
the specified [^Element] type.
>*/
//]

//[oglplus_opt_ranges_end
} // namespace ranges
//]

