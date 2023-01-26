#pragma once
#include <iostream>

// backgrounds : std::list

template < class T, class Alloc = allocator<T> >
class list
{
	template< class U >
	struct node
	{
		U mData;
		node* pPrev = nullptr;
		node* pNext = nullptr;
	};

	typedef T value_type;

	 
	// backgrounds : allocator<T> 는 어디다 쓰는지?

	typedef Alloc value_allocator_type; // == typedef typename Alloc::template Rebind<T>::Other value_allocator_type;

	// backgrounds : rebind라는 중첩(nested) 구조체 템플릿

	// 방법 1
	using node_allocator_type = typename
		Alloc::template	my_rebind<node<value_type>>::my_other;


	// 방법 2 : 방법 1 필수조건을 std::allocator_traits 에서 짜준다
	using value_alloc = typename
		std::allocator_traits<Alloc>::template rebind_alloc<T>;
	using node_alloc = typename
		std::allocator_traits<Alloc>::template rebind_alloc<node<T>>;

	typedef node_allocator_type allocator_type;
	// typedef allocator<node<T>> allocator_type; // 이걸 선언할수는 있어도 안쓸수있어서 안됨

	typedef value_type& reference;
	typedef node<T>* pointer;


private:
	allocator_type m_allocator;
	pointer pHead;
	pointer pTail;

public:
	// default ( 1 ) empty container constructor (default constructor) 
	// Constructs an empty container, with no elements.
	//  to mark constructors to not implicitly convert types in C++. 
	// It is optional for constructors that take exactly one argument and work on constructors(with a single argument) 
	// 
	// com1 == 3.0 is called, 3.0 is implicitly converted to Complex type 
	// because the default constructor can be called with only 1 argument 
	// because both parameters are default arguments and we can choose not to provide them.
	// We can avoid such implicit conversions as these may lead to unexpected results.
	// since those are the only constructors that can be used in typecasting.


	// com1 == (Complex)3.0 만 가능
	explicit list( const allocator_type& alloc = allocator_type() ) : m_allocator( alloc ) //????
		, pHead( nullptr ), pTail( nullptr )
	{

	}

	// for (std::list<int>::iterator it=mylist.begin(); it != mylist.end(); ++it)
	struct iterator
	{
		pointer pNode;

		iterator( pointer p ) : pNode( p )
		{}

		//복사 생성자
		iterator( const iterator& it )
		{
			pNode = it.pNode;
		}

		//대입 연산자
		pointer operator=( const iterator& it )
		{
			pNode = it.pNode;
			return pNode;
		}

		//대입 연산자2
		pointer operator=( const value_type& val ) // 쓰이지 않으면 디버깅이 안잡히는 것 같다 = 코드가 위로 밀리는 문제 ? 
		{// prev, next 그대로, 내용물만 바꿈
			pNode->mData = val;
			return pNode;
		}

		iterator& operator++()
		{
			if ( pNode != nullptr && pNode->pNext )
				pNode = pNode->pNext;
			else
				pNode = nullptr;
			return *this;
		}

		iterator& operator--()
		{
			if ( pNode != nullptr && pNode->pPrev )
				pNode = pNode->pPrev;
			else
				pNode = nullptr;
			return *this;
		}

		//포인터 연산자 오버로딩
		value_type& operator*()
		{
			return pNode->mData;
		}

		// ?? 쓸모?
		//value_type* operator->()
		//{
		//	std::cout << "operator*()";
		//	return &( pNode->mData );
		//}

		bool operator!=( const iterator& b ) const
		{
			return ( pNode != b.pNode );
		}

		bool operator<( const iterator& b ) const
		{
			return ( pNode->mData < b.pNode->mData );
		}
	};

	// ?? 안됨
	//bool operator!=( const iterator& lhs, const iterator& rhs ) const
	//{
	//	std::cout << "operator!=";
	//	return lhs->pNode == rhs->pNode; // 주소 비교
	//}

	//typedef Iterator* iterator;

	iterator begin() noexcept
	{
		return pHead; // iterator( pointer p ) 생성자 타서 이터레이터로 만들어진다
	}
	//const_iterator begin() const noexcept;
	iterator end() noexcept
	{
		return pTail->pNext;
	}
	//const_iterator end() const noexcept;

	//i++ calls i.operator++(0)
	//iterator operator!=( iterator& vit ) {

	//}

	//  Modifiers:
	// push_back	Add element at the end (public member function)
	void push_back( const value_type& val )
	{
		if ( !pTail ) { // 조건을..
			pHead = m_allocator.allocate( 1 );
			pTail = pHead;
			pHead->pPrev = nullptr;
			pTail->pNext = nullptr;
		}
		else {
			pointer pPrevTail = pTail;
			pTail = m_allocator.allocate( 1 );
			pPrevTail->pNext = pTail;
			pTail->pPrev = pPrevTail;
			pTail->pNext = nullptr;
		}
		pTail->mData = val;

	}
	void push_back( value_type&& val );

	// push_front	Insert element at beginning( public member function )
	void push_front( const value_type& val )
	{
		if ( !pHead ) {
			pHead = m_allocator.allocate( 1 );
			pTail = pHead;
			pHead->pPrev = nullptr;
			pTail->pNext = nullptr;
		}
		else {
			pointer pPrevHead = pHead;
			pHead = m_allocator.allocate( 1 );
			pPrevHead->pPrev = pHead;
			pHead->pNext = pPrevHead;
			pHead->pPrev = nullptr;
		}
		pHead->mData = val;
	}
	void push_front( value_type&& val );

	// pop_back	Delete last element (public member function)
	void pop_back()
	{
		if ( !pHead && !pTail ) {
			return;
		}
		pointer pPrevTail = pTail;
		pTail = pTail->pPrev;
		pTail->pNext = nullptr;
		m_allocator.deallocate( pPrevTail, 1 );
	}

	// pop_front	Delete first element (public member function)
	void pop_front()
	{
		if ( !pHead && !pTail ) {
			return;
		}
		pointer pPrevHead = pHead;
		pHead = pHead->pNext;
		pHead->pPrev = nullptr;
		m_allocator.deallocate( pPrevHead, 1 );
	}

	// Operations:
	// sort	Sort elements in container (public member function)
	void sort();

	// comp : returns true if the first argument goes before the second argument in the strict weak ordering it defines, and false otherwise.
	template <class Compare>  void sort( Compare comp );
};

