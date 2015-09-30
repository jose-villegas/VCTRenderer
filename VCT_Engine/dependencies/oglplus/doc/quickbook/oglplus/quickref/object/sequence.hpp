/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_object_SeqIterator

template <typename ObjectT>
class SeqIterator
{
public:
	typedef ObjectT value_type;
	typedef __Unspecified difference_type;

	friend bool operator == (SeqIterator a, SeqIterator b);
	friend bool operator != (SeqIterator a, SeqIterator b);
	friend bool operator <  (SeqIterator a, SeqIterator b);

	friend SeqIterator operator + (SeqIterator a, difference_type d);
	friend SeqIterator operator - (SeqIterator a, difference_type d);
	friend difference_type operator - (SeqIterator a, SeqIterator b);

	value_type operator * (void) const;
	value_type operator [](size_t index) const;

	SeqIterator& operator ++ (void);
	SeqIterator operator ++ (int);
	SeqIterator& operator -- (void);
	SeqIterator operator -- (int);

	SeqIterator& operator += (difference_type d);
	SeqIterator& operator -= (difference_type d);
};

//]
//[oglplus_object_Sequence

template <typename ObjectT>
class Sequence
{
public:
	bool empty(void) const;
	size_t size(void) const;

	ObjectT at(size_t index) const;
	ObjectT operator[](size_t index) const;

	Sequence slice(size_t start) const;
	Sequence slice(size_t start, size_t count) const;

	typedef __SeqIterator<ObjectT> iterator;
	typedef __SeqIterator<ObjectT> const_iterator;

	const_iterator begin(void) const;
	const_iterator end(void) const;
};

template <typename ObjectT>
const GLuint* GetNames(const __Sequence<ObjectT>& sequence);

//]

