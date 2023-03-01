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
	//typedef node<T>* pointer;


private:
	//allocator_type m_allocator;
	//pointer pHead;
	//pointer pTail;

	typename Alloc::template my_rebind<node<T>>::my_other m_allocator;
	node<T>* pHead;
	node<T>* pTail;

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

	// 매개변수 디폴트 하나밖에 없으면 explicit 생성자선언이 좋은듯
	// com1 == (Complex)3.0 만 가능
	explicit list( const allocator_type& alloc = allocator_type() ) : m_allocator( alloc ) // list<long>( ?? )
	{
		// m_allocator 메모리 단위는 node<T>
		node<T>* pDummy = m_allocator.allocate( 1 );
		pHead = pDummy;
		pTail = pDummy;

	}

	~list()
	{
		do {
			node<T>* pOldHead = pHead;
			pHead = pHead->pNext;
			m_allocator.deallocate( pOldHead, 1 );

		} while ( pHead );
		pTail = nullptr;
	}

	// for (std::list<int>::iterator it=mylist.begin(); it != mylist.end(); ++it)
	struct iterator
	{
		node<T>* pNode;

		iterator( node<T>* p ) : pNode( p ) 
		{
			// return 값으로  node<T>* 보낼 때 탄다
		}

		//복사 생성자
		//iterator( const iterator& it )
		//{
		//	pNode = it.pNode;
		//}

		//대입 연산자
		iterator& operator=( const iterator& it )
		{
			if ( this == &it )
				return *this;

			pNode = it.pNode;
			return *this;
		}

		//대입 연산자2
		node<T>& operator=( const T& val ) // 쓰이지 않으면 디버깅이 안잡힌다
		{
			// prev, next 그대로, 내용물만 바꿈
			// iterator는 포인터 취급당하는데, 포인터는 이렇게 쓰이지 않음. 안좋은 방식
			pNode->mData = val;
			return *pNode;
		}

		iterator& operator++()
		{
			if ( pNode != nullptr && pNode->pNext )
				pNode = pNode->pNext;
			else
				pNode = nullptr; // ??
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
		T& operator*()
		{
			return pNode->mData;
		}

		T* operator->() // used with pointers to access the class or struct variables : ptr->marks = 92;
		{
		//https://stackoverflow.com/questions/30123338/use-operator-t-instead-of-t-operator-for-member-access

		//The expression x->y requires x to be a pointer to complete class type, 
		//	or when x is an instance of a class, requires operator->() defined for x

			return &pNode->mData; // ??
		}

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
		return pTail; // tail == dummy?
	}
	//const_iterator end() const noexcept;

	//i++ calls i.operator++(0)
	//iterator operator!=( iterator& vit ) {

	//}

	//  Modifiers:
	// push_back	Add element at the end (public member function)
	void push_back( const T& val )
	{
		node<T>* pNewNode = m_allocator.allocate( 1 );

		if ( pTail->pPrev )
			pTail->pPrev->pNext = pNewNode;

		pNewNode->pPrev = pTail->pPrev;
		pNewNode->pNext = pTail;

		pTail->pPrev = pNewNode;

		pNewNode->mData = val;

		if ( pHead == pTail )
			pHead = pNewNode;
	}
	void push_back( T&& val ) { // rvalue 참조 선언자
		node<T>* pNewNode = m_allocator.allocate( 1 );

		if ( pTail->pPrev )
			pTail->pPrev->pNext = pNewNode;

		pNewNode->pPrev = pTail->pPrev;
		pNewNode->pNext = pTail;

		pTail->pPrev = pNewNode;

		pNewNode->mData = val;

		if ( pHead == pTail )
			pHead = pNewNode;
	}

	// push_front	Insert element at beginning( public member function )
	void push_front( const value_type& val )
	{
		node<T>* pNewNode = m_allocator.allocate( 1 );
		pHead->pPrev = pNewNode;
		pNewNode->pNext = pHead;

		pHead = pNewNode;
		pHead->pPrev = nullptr;

		pHead->mData = val;
	}

	void push_front( value_type&& val ) 
	{
		node<T>* pNewNode = m_allocator.allocate( 1 );
		pHead->pPrev = pNewNode;
		pNewNode->pNext = pHead;

		pHead = pNewNode;
		pHead->pPrev = nullptr;

		pHead->mData = val;
	}

	// pop_back	Delete last element (public member function)
	void pop_back()
	{
		if ( pHead == pTail ) {
			return;
		}
		node<T>* pOldEnd = pTail->pPrev;
		node<T>* pNewEnd = pOldEnd->pPrev;
		pNewEnd->pNext = pTail;
		pTail->pPrev = pNewEnd;
		m_allocator.deallocate( pOldEnd, 1 );
	}

	// pop_front	Delete first element (public member function)
	void pop_front()
	{
		if ( pHead == pTail ) {
			return;
		}
		node<T>* pOldHead = pHead;
		pHead = pHead->pNext;
		pHead->pPrev = nullptr;
		m_allocator.deallocate( pOldHead, 1 );
	}

	// Operations:
	// sort	Sort elements in container (public member function)
	void sort();

	// comp : returns true if the first argument goes before the second argument in the strict weak ordering it defines, and false otherwise.
	template <class Compare>  void sort( Compare comp );
};

