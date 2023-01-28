#include <cmath>
#include <iostream>
#include <limits>
#include <stdlib.h> 

// backgrounds : std::vector

template < class T >
class allocator
{
public:
	typedef T value_type;
	typedef T* pointer; // 필수
	typedef const T* const_pointer;
	typedef T& reference; // 필수
	typedef const T& const_reference;
	typedef long size_type;

public:

	allocator()
	{
	}

	template<typename U>
	struct my_rebind
	{
		typedef allocator<U> my_other;
	};

	//Either 0 or a value previously obtained by another call to allocate and not yet freed with deallocate.
	//When it is not 0,
	//this value may be used as a hint 
	//to improve performance by allocating the new block near the one specified.
	//The address of an adjacent element is often a good choice.
	pointer allocate( size_type n ) // , allocator<void>::const_pointer hint = 0 ) ???
	{
		return (pointer)calloc( n, sizeof( value_type ) );
	}

	// allocate 인자로는 바이트 수가 아닌, 필요한 T 객체의 개수를 매개변수로 넘긴다.
	pointer reallocate( pointer p, size_type n )
	{
		if ( p == nullptr ) {
			p = allocate( n );
			// head = new int[ nCapacity ]; realloc 있어서 calloc 이 더 좋은듯
		}
		else {
			// backgrounds : realloc
			p = (T*)realloc( p, n * sizeof( value_type ) );
		}
		return p;
	}
	//	const T* allocate( long n );

	void deallocate( pointer p, size_type n ) // n?? 옛날 방식, 또는 new, delete 용
	{
		//for ( size_type i = 0; i < n; ++i ) {
		//	free( p + i );
		//}
		free( p );
	}
};

struct A;

// T is guaranteed to not throw while moving, 
template < class T, class Alloc = allocator<T> >
class vector // generic template
{

	// 주의할 점은 realloc이 새로운 자리가 아니라 기존의 자리에서 그대로 늘어난 양만큼 추가를 하려고 하기 때문에 
	//	기존의 배열 뒤로 자리가 없다면 0을 리턴하고 따로 배열의 주소를 보관해 두지 않았다면 그 주소를 영영 잃어버리게 된다.
	//	그래서 사전에 따로 저장해 두고 0이 리턴되었는지 검사해서 복원 시켜주는 것에 신경 써야 한다. ??

	//﻿int* array = 0;
	//int* tempArray = 0;

	//array = (int*)malloc( 10 * sizeof( int ) );
	//tempArray = array;
	//array = (int*)realloc( array, 20 * sizeof( int ) );

	//if ( array == 0 ) {
	//	array = tempArray; //할당이 안되었을 경우 원래 주소로. 
	//}

	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef const T& const_reference;
	typedef T& reference;
	typedef long size_type;

public:
	Alloc myAllocator;
	size_type nSize;
	size_type nCapacity;
	pointer pHead;
	pointer pEnd;
	pointer pCapacityEnd;

	vector() :nSize( 0 ), nCapacity( 0 ), pHead( nullptr ), pEnd( nullptr ), pCapacityEnd( nullptr )
	{
	}

	~vector()
	{
		myAllocator.deallocate( pHead, nCapacity );
		pHead = nullptr;
		pEnd = nullptr;
		pCapacityEnd = nullptr;
		// delete[] head;
	}

	///////////////////////////////////////////////////////////////
	// iterators:

	struct iterator
	{
		pointer pEl;
		iterator( pointer p ) : pEl( p )
		{}

		//복사 생성자
		iterator( const iterator& it )
		{
			pEl = it.pEl;
		}

		//대입 연산자
		pointer operator=( const iterator& it )
		{
			pEl = it.pEl;
			return pEl;
		}

		//대입 연산자2
		pointer operator=( const value_type& val )
		{
			*pEl = val;
			return pEl;
		}

		iterator& operator++()
		{
			++pEl;
			return *this;
		}

		iterator& operator--()
		{
			--pEl;
			return *this;
		}

		//포인터 연산자 오버로딩
		value_type& operator*()
		{
			return *pEl;
		}

		//value_type* operator->()
		//{
		//	std::cout << "operator*()";
		//	return &( pNode->mData );
		//}

		bool operator!=( const iterator& b ) const
		{
			return ( pEl != b.pEl );
		}

		bool operator<( const iterator& b ) const
		{
			return ( *pEl < *b.pEl );
		}
	};

	iterator begin() noexcept
	{
		return iterator( pHead );
	}

	iterator end() noexcept
	{
		return iterator( pEnd + 1 );
	}

	///////////////////////////////////////////////////////////////
	// Capacity:

	// Return size (public member function)
	size_type size()
	{
		//long filled_size = ( position - head ) / sizeof( T );
		return nSize;
	}

	// 	Change size (public member function)
	void resize( size_type n )
	{
		pointer pPrevHead = pHead;
		pHead = myAllocator.reallocate( pHead, n );
		pCapacityEnd = pHead + nCapacity;

		if ( pPrevHead != pHead ) {
			// realloc 이 copy, free 다 해줌
		}
	}
	void resize( size_type n, const value_type& val ); // ..??

	// Return size of allocated storage capacity (public member function)
	long capacity()
	{
		return nCapacity;
	}

	// Request a change in capacity (public member function)
	void reserve( size_type n )
	{
		if ( nCapacity < n ) {
			if ( nCapacity == 0 )
				nCapacity = 1;
			else
				nCapacity *= 2;
			resize( nCapacity );
		}
	}

	///////////////////////////////////////////////////////////////
	// Element access:

	// operator[] : 	Access element (public member function)
	reference operator[] ( size_type n ) {
		return *( pHead + n );
	}

	//const_reference operator[] ( size_type n ) const;

	///////////////////////////////////////////////////////////////
	bool AreSame( double a, double b )
	{
		return fabs( a - b ) < std::numeric_limits<double>::epsilon();
	}

	 //Element access:
	 //push_back 	Add element at the end (public member function)
	void push_back( const value_type& val ) {
		size_type nCapacityPrev = nCapacity;

		++nSize;

		reserve( nSize );

		pEnd = pHead + nSize - 1;
		*( pEnd ) = val;
	}
	void push_back( value_type&& val ) { // rvalue reference. It binds to temporaries without making a copy

			// This effectively increases the container size by one, 
			// which causes an automatic reallocation of the allocated storage space if - and only if - 
			// the new vector size surpasses the current vector capacity.
		size_type nCapacityPrev = nCapacity;

		++nSize;

		reserve( nSize );

		pEnd = pHead + nSize - 1;
		*( pEnd ) = val;
	}

	// pop_back  Delete last element (public member function)
};

struct TESTRVRef
{
	long a;
	long b = 9;
	TESTRVRef( long&& _a ) : a( -_a ) //  rvalue reference : binds to temporaries without making a copy.
	{
	
	} 

	TESTRVRef( long& _a ) : a( _a )
	{

	}
};

struct TestEmptyStruct
{
	long* a;
	allocator<long> m_allocator;
	TestEmptyStruct( long&& _a )
	{
		a = m_allocator.allocate( 1 );
		*a = -_a;
	}

	//TestEmptyStruct( long& _a ) : a( _a )
	//{

	//}

	struct rebind
	{

	};
};