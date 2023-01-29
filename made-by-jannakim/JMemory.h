#pragma once
#include <iostream>
#include <cstddef>
// This header defines general utilities to manage dynamic memory:

struct HeapMemory
{
	static long allocated;
	long& operator()()
	{
		return allocated;
	}

};
long HeapMemory::allocated = 0;

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
		std::cout << "         memory : " << ++HeapMemory()() << "   " << typeid( T ).name() << std::endl;
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

		std::cout << "         memory : " << --HeapMemory()( ) << "   " << typeid( T ).name() << std::endl;

		free( p );
	}
};

// Managed pointers
template <class T> 
class shared_ptr
{
	T* mData;

	template <class T>
	struct ControlBlock
	{
		long use_count;
		long weak_count;
		T* mData; // 내부적으로 객체의 주소를 보관하기 위해 포인터 멤버를 가지고 있다
		ControlBlock() : use_count(0), weak_count(0)
		{

		}
		~ControlBlock()
		{
			delete mData;
		}
		// allocator
		// deleter
	};

	//Manages the storage of a pointer, providing a limited garbage - collection facility, 
	//possibly sharing that management with other objects.

	//	the last one of them releases that ownership

	//	release ownership : destroyed,
	//	or their value changes either by an assignment operation or by an explicit call to shared_ptr::reset

	//can only share ownership by copying their value

	//aliasing : can share ownership over a pointer while at the same time pointing to another object.

	// 제어 블록을 가리킬 수 이쓴ㄴ 포인터가 있어야 한다

public:
	ControlBlock<T>* m_controlblock;
	
	// Member functions
public:
	// 생성/복사/대입/소멸을 관리할 수 있다

	// std::shared_ptr<int> p1;
	constexpr shared_ptr() noexcept
	{
		m_controlblock = new ControlBlock;

		m_controlblock->use_count = 0;
	}

	//shared_ptr<int> p3( new int );
	//shared_ptr<C> obj( new C );
	template <class U> 
	explicit shared_ptr( U* p )
	{
		m_controlblock = new ControlBlock<U>; // 제어블럭 만들고 파괴할 때 할당자 사용

		++m_controlblock->use_count;
		m_controlblock->mData = p;
		mData = p;
		// mData = new *p;
	}
	// copy constructor
	shared_ptr( /*const */shared_ptr& x ) noexcept // 같은 거 들어왔을 때. 왜 const?
	{
		m_controlblock = x.GetControlBlock();
		++m_controlblock->use_count;
	}
	
	// with deleter
	template <class U, class D> shared_ptr( U* p, D del ); 
	
	template <class D> shared_ptr( nullptr_t p, D del );


	ControlBlock<T>* GetControlBlock()
	{
		return m_controlblock;
	}

	template <class U> 
	shared_ptr( const shared_ptr<U>& x ) noexcept; // 다른 거 들어왔을 때 ??


	//template <class U> explicit shared_ptr( const weak_ptr<U>& x );

	~shared_ptr()
	{
		--m_controlblock->use_count;

		if ( !m_controlblock->use_count ) {
			delete m_controlblock->mData;
			delete m_controlblock;
		}
	}

	// 진짜 포인터가 아니고 객체지만 연산자 재정의를 통해 포인터처럼 사용
	T* operator->() const noexcept // ??
	{
		return mData;
	}

	T& operator*() const noexcept
	{
		*mData;
	}

	// copy
	shared_ptr& operator= ( const shared_ptr& x ) noexcept
	{

	}

	template <class U> shared_ptr& operator= ( const shared_ptr<U>& x ) noexcept;

	// move
	shared_ptr& operator= ( shared_ptr&& x ) noexcept; 
	template <class U> shared_ptr& operator= ( shared_ptr<U>&& x ) noexcept;

	// move from
	//template <class U> shared_ptr& operator= ( auto_ptr<U>&& x ); 
	//template <class U, class D> shared_ptr& operator= ( unique_ptr<U, D>&& x );


};


template <class T> 
class weak_ptr;
