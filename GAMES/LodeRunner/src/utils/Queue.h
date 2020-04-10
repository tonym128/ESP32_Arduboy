#pragma once

#include <stdint.h>

//
// Declaration
//

template< typename Type, uint8_t Capacity >
class Queue
{
public:
	static_assert(Capacity > 0, "Attempt to create a Queue with Capacity less than 1");
	static_assert(Capacity < 128, "Attempt to create a Queue with Capacity greater than 127");

	using ItemType = Type;
	using IndexType = uint8_t;
	using IndexOfType = int8_t;

	constexpr static const IndexType FirstIndex = 0;
	constexpr static const IndexType LastIndex = Capacity - 1;

private:
	ItemType items[Capacity];
	IndexType next;

public:
	Queue(void);

	bool isEmpty(void) const; // O(1)
	bool isFull(void) const; // O(1)

	IndexType getCount(void) const; // O(1)
	constexpr IndexType getCapacity(void) const; // O(1)

	ItemType & peek(void); // O(1)
	const ItemType & peek(void) const; // O(1)
	bool enqueue(const ItemType & item); // O(1)
	bool enqueue(ItemType && item); // O(1)
	ItemType dequeue(void); // O(n)

	void drop(void); // O(n)

	void clear(void); // O(n)
	bool contains(const ItemType & item) const; // O(n)

	// Returns -1 if item not found
	IndexOfType indexOf(const ItemType & item) const; // O(n)

	ItemType & operator [] (const IndexType index); // O(1)
	const ItemType & operator [] (const IndexType index) const; // O(1)
};


//
// Definition
//

template< typename Type, uint8_t Capacity >
Queue< Type, Capacity >::Queue(void)
	: items(), next(0)
{
}

template< typename Type, uint8_t Capacity >
bool Queue< Type, Capacity >::isEmpty(void) const // O(1)
{
	return (this->next == FirstIndex);
}

template< typename Type, uint8_t Capacity >
bool Queue< Type, Capacity >::isFull(void) const // O(1)
{
	return (this->next == this->getCapacity());
}

template< typename Type, uint8_t Capacity >
typename Queue< Type, Capacity >::IndexType Queue< Type, Capacity >::getCount(void) const // O(1)
{
	return this->next;
}

template< typename Type, uint8_t Capacity >
constexpr typename Queue< Type, Capacity >::IndexType Queue< Type, Capacity >::getCapacity(void) const // O(1)
{
	return static_cast<IndexType>(Capacity);
}

template< typename Type, uint8_t Capacity >
typename Queue< Type, Capacity >::ItemType & Queue< Type, Capacity >::peek(void) // O(1)
{
	return this->items[0];
}

template< typename Type, uint8_t Capacity >
const typename Queue< Type, Capacity >::ItemType & Queue< Type, Capacity >::peek(void) const // O(1)
{
	return this->items[0];
}

template< typename Type, uint8_t Capacity >
typename Queue< Type, Capacity >::ItemType Queue< Type, Capacity >::dequeue(void)  // O(n)
{
	const auto result = this->items[this->next]; // ought to be std::move
	
	--this->next;
	
	// i + 1 == this->next at the last iteration
	for(uint8_t i = 0; i < this->next; ++i)
		this->items[i] = this->items[i + 1];
	
	return result;
}

template< typename Type, uint8_t Capacity >
bool Queue< Type, Capacity >::enqueue(const typename Queue< Type, Capacity >::ItemType & item) // O(1)
{
	if (this->isFull())
		return false;

	this->items[this->next] = item;
	++this->next;
	return true;
}

template< typename Type, uint8_t Capacity >
bool Queue< Type, Capacity >::enqueue(typename Queue< Type, Capacity >::ItemType && item) // O(1)
{
	if (this->isFull())
		return false;

	this->items[this->next] = item; // ought to be std::move
	++this->next;
	return true;
}

template< typename Type, uint8_t Capacity >
void Queue< Type, Capacity >::drop(void) // O(1)
{
	--this->next;
	
	// i + 1 == this->next at the last iteration
	for(uint8_t i = 0; i < this->next; ++i)
		this->items[i] = this->items[i + 1];
		
	this->items[this->next].~ItemType();
}

template< typename Type, uint8_t Capacity >
void Queue< Type, Capacity >::clear(void) // O(n)
{
	for (IndexType i = 0; i < this->next; ++i)
		(&this->items[i])->~ItemType();
	this->next = 0;
}

template< typename Type, uint8_t Capacity >
bool Queue< Type, Capacity >::contains(const typename Queue< Type, Capacity >::ItemType & item) const // O(n)
{
	return this->indexOf(item) != -1;
}

template< typename Type, uint8_t Capacity >
typename Queue< Type, Capacity >::IndexOfType Queue< Type, Capacity >::indexOf(const typename Queue< Type, Capacity >::ItemType & item) const // O(n)
{
	for (IndexType i = 0; i < this->next; ++i)
		if (this->items[i] == item)
			return i;
	return -1;
}

template< typename Type, uint8_t Capacity >
typename Queue< Type, Capacity >::ItemType & Queue< Type, Capacity >::operator [] (const typename Queue< Type, Capacity >::IndexType index) // O(1)
{
	return this->items[index];
}

template< typename Type, uint8_t Capacity >
const typename Queue< Type, Capacity >::ItemType & Queue< Type, Capacity >::operator [] (const typename Queue< Type, Capacity >::IndexType index) const // O(1)
{
	return this->items[index];
}
