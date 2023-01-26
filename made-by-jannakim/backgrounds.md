1. std::list

sequence containers that allow constant time insertand erase
iteration in both directions.
doubly - linked lists
elements they contain in different and unrelated storage locations
better in inserting, extractingand moving elements in any position
lack direct access : takes linear time
extra memory to keep the linking information


2. allocator<T> 는 어디다 쓰는지?

	https://stackoverflow.com/questions/48969089/what-does-the-stdlist-do-with-its-allocator-argument
	Do we just drop the T allocator on the floor, or do we use it somehow ?

	답변 1

	Since C++11 allocators can be stateful.
	You want to construct your internal allocator using the user supplied one.
	An Allocator is explicitely required to be constructible from typename std::allocator_traits<Alloc>::template rebind_alloc<U>.

	답변 2
	The T allocator is still used to construct() and destroy() the T portion of the internal nodes. 
	Using these operations may be necessary to add custom arguments during construction, 
	e.g., optionally forward a suitable allocator argument to the constructed objects.


3. rebind라는 중첩(nested) 구조체 템플릿

	// http://egloos.zum.com/sweeper/v/2966785    : std::list에 custom allocator 사용하려면 제공해야하는 필수 요소들
	// - rebind라는 중첩(nested) 구조체 템플릿을 반드시 제공해야 한다

	// 방법 1
	// 내가쓰는 Alloc의 my_rebind 중첩 구조체 템플릿안에 typedef된 my_other란 타입을 가져온다. 
	// typedef typename Alloc::template my_rebind<node<T>>::my_other node_allocator_type;
	using node_allocator_type = typename
		Alloc::template	my_rebind<node<value_type>>::my_other;


	// 방법 2 : 방법 1 필수조건을 std::allocator_traits 에서 짜준다
	// std::allocator_traits supplies a uniform interface to all allocator types 
	// without *** enforcing the entire default allocator interface *** on every allocator type.

	using value_alloc = typename
		std::allocator_traits<Alloc>::template rebind_alloc<T>;
	using node_alloc = typename
		std::allocator_traits<Alloc>::template rebind_alloc<node<T>>;


	allocator_traits : 
	1. default definitions of 9 member typedefs and 2 template alias declarations
	2. default allocator_traits instantiation gives default implementations for 4 member functions,
	including construct() and destroy(), which call placement newand the object destructor respectively.

	https://theotherbranch.wordpress.com/2012/12/13/using-the-standard-allocator-for-a-different-type-and-get-free-functionality-for-your-custom-allocator/


4. std::vector

 reallocations should only happen at logarithmically growing intervals of size 
 so that the insertion of individual elements at the end of the vector 
 can be provided with amortized constant time complexity

an allocator object to dynamically handle its storage needs.

size can change dynamically, with their storage being handled automatically by the container.

5. realloc

	if size is zero, the memory previously allocated at ptr is deallocated as if a call to free was made, and a null pointer is returned.
		allocating a new memory block of size new_size bytes,
		copying memory area with size equal the lesser of the newand the old sizes, and freeing the old block.


6. 