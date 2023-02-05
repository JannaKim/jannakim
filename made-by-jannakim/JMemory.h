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
	typedef T* pointer; // �ʼ�
	typedef const T* const_pointer;
	typedef T& reference; // �ʼ�
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

	// allocate ���ڷδ� ����Ʈ ���� �ƴ�, �ʿ��� T ��ü�� ������ �Ű������� �ѱ��.
	pointer reallocate( pointer p, size_type n )
	{
		if ( p == nullptr ) {
			p = allocate( n );
			// head = new int[ nCapacity ]; realloc �־ calloc �� �� ������
		}
		else {
			// backgrounds : realloc
			p = (T*)realloc( p, n * sizeof( value_type ) );
		}
		return p;
	}
	//	const T* allocate( long n );

	void deallocate( pointer p, size_type n ) // n?? ���� ���, �Ǵ� new, delete ��
	{
		//for ( size_type i = 0; i < n; ++i ) {
		//	free( p + i );
		//}

		std::cout << "         memory : " << --HeapMemory()( ) << "   " << typeid( T ).name() << std::endl;

		free( p );
	}
};

template <class T>
struct ControlBlock
{
	long use_count;
	long weak_count;
	T* mData; // ���������� ��ü�� �ּҸ� �����ϱ� ���� ������ ����� ������ �ִ�
	ControlBlock() : use_count( 0 ), weak_count( 0 )
	{

	}
	~ControlBlock()
	{
	}
	// allocator
	// deleter
};

template <class T>
class weak_ptr;

// Managed pointers
template <class T> 
class shared_ptr
{
	T* mData;

	//Manages the storage of a pointer, providing a limited garbage - collection facility, 
	//possibly sharing that management with other objects.

	//	the last one of them releases that ownership

	//	release ownership : destroyed,
	//	or their value changes either by an assignment operation or by an explicit call to shared_ptr::reset

	//can only share ownership by copying their value

	//aliasing : can share ownership over a pointer while at the same time pointing to another object.

	// ���� ������ ����ų �� �̾��� �����Ͱ� �־�� �Ѵ�

public:
	ControlBlock<T>* m_controlblock;
	
	// Member functions
public:
	// ����/����/����/�Ҹ��� ������ �� �ִ�

	constexpr shared_ptr() noexcept
	{
		m_controlblock = new ControlBlock<T>;

		m_controlblock->use_count = 0;
	}

	//shared_ptr<int> p3( new int );
	//shared_ptr<C> obj( new C );
	template <class U> 
	explicit shared_ptr( U* p )
	{
		m_controlblock = new ControlBlock<U>; // ������� ����� �ı��� �� �Ҵ��� ���

		++m_controlblock->use_count;
		m_controlblock->mData = p;
		mData = p;
		// mData = new *p;
	}
	// copy constructor
	shared_ptr( /*const */shared_ptr& x ) noexcept // ���� �� ������ ��. �� const?
	{
		m_controlblock = x.m_controlblock;
		++m_controlblock->use_count;
		mData = m_controlblock->mData;
	}
	
	// with deleter
	template <class U, class D> shared_ptr( U* p, D del );

	template <class D> shared_ptr( nullptr_t p, D del );

	// from weak
	template <class T > 
	explicit shared_ptr( const weak_ptr<T>& x )
	{
		m_controlblock = x.m_controlblock;
		++m_controlblock->use_count;
		mData = m_controlblock->mData;
	}

	template <class U> 
	shared_ptr( const shared_ptr<U>& x ) noexcept; // �ٸ� �� ������ �� ??


	//template <class U> explicit shared_ptr( const weak_ptr<U>& x );

	~shared_ptr()
	{
		--m_controlblock->use_count;

		if ( !m_controlblock->use_count ) {
			delete mData;               
			
		}

		if ( m_controlblock->use_count + m_controlblock->weak_count == 0 ) {
			// ���� ���� �ı�
			delete m_controlblock;
		}
	}

	// ��¥ �����Ͱ� �ƴϰ� ��ü���� ������ �����Ǹ� ���� ������ó�� ���
	T* operator->() const noexcept // ??
	{
		return mData;
	}

	T& operator*() const noexcept
	{
		*mData;
	}

	// ���� ������
	shared_ptr& operator= ( const shared_ptr& x ) noexcept
	{
		if ( m_controlblock != x.m_controlblock ) {
			--m_controlblock->use_count;

			if ( !m_controlblock->use_count ) {
				 delete mData;           

			}

			if ( m_controlblock->use_count + m_controlblock->weak_count == 0 ) {
				// ���� ���� �ı�
				delete m_controlblock;
			}

			m_controlblock = x.m_controlblock;
		}
		return *this;
	}

	template <class U> // �ٸ� ��ü ������ ����� ��
	shared_ptr& operator= ( const shared_ptr<U>& x ) noexcept;

	// move
	shared_ptr& operator= ( shared_ptr&& x ) noexcept; 
	template <class U> shared_ptr& operator= ( shared_ptr<U>&& x ) noexcept;

	// move from
	//template <class U> shared_ptr& operator= ( auto_ptr<U>&& x ); 
	//template <class U, class D> shared_ptr& operator= ( unique_ptr<U, D>&& x );


};


template <class T> 
class weak_ptr
{	T* mData;

public:
	ControlBlock<T>* m_controlblock;
	
	// Member functions
public:

	// default
	constexpr weak_ptr() noexcept
   {
		m_controlblock = new ControlBlock<T>;

		m_controlblock->weak_count = 0;
   }

	// copy
	weak_ptr( const weak_ptr& x ) noexcept; 
	template <class U> weak_ptr( const weak_ptr<U>& x ) noexcept;

	// from shared_ptr
	template <class U> weak_ptr( const shared_ptr<U>& x ) noexcept
	{
		m_controlblock = x.m_controlblock;
		++m_controlblock->weak_count;
		mData = m_controlblock->mData;
	}

	~weak_ptr()
	{
		--m_controlblock->weak_count;

		if ( m_controlblock->use_count + m_controlblock->weak_count == 0 ) {
			// ���� ���� �ı�
			delete m_controlblock;
		}
	}

	T* operator->() const noexcept // ??
	{
		return mData;
	}

	T& operator*() const noexcept
	{
		*mData;
	}

	// ���� ������ : from shared_ptr
	template <class U> 
	weak_ptr& operator=( const shared_ptr<U>& x ) noexcept
	{
		// error C2662: 'shared_ptr<Car>::ControlBlock<T> *shared_ptr<T>::GetControlBlock(void)': 'this' �����͸� 'const shared_ptr<Car>'���� 'shared_ptr<Car> &'(��)�� ��ȯ�� �� �����ϴ�.
		if ( m_controlblock != x.m_controlblock ) {
			if ( m_controlblock ) {
				--m_controlblock->weak_count;

				if ( m_controlblock->use_count + m_controlblock->weak_count == 0 ) {
					// ���� ���� �ı�
					delete m_controlblock;
				}
			}
			m_controlblock = x.m_controlblock;
			++m_controlblock->weak_count;
		}
		return *this;
	}

	bool expired() const noexcept
	{
		return m_controlblock->use_count;
	}
};