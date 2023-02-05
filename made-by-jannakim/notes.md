///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
1주차

https://github.com/electronicarts/EASTL

A ) Conan package manager 로 다운 : 
1. pip install conan
2. python -m pip install --upgrade pip
3. git clone https://github.com/conan-io/conan.git conan-io
4. cd conan-io && sudo pip install -e .
5. conan install eastl/3.15.00@

B ) compiling and testing the source
 - https://github.com/electronicarts/EASTL/blob/master/CONTRIBUTING.md

1. git clone https://github.com/electronicarts/EASTL
2. cd EASTL
3. git submodule update --init

C ) Running the Unit Tests
1. 개인 빌드 폴더 만들고 그 안에서 cmake ../ -DEASTL_BUILD_TESTS:BOOL=ON
 :  Build files have been written to: C:/Users/admin/Documents/EASTL/jannas_build_folder

2. Build unit tests for "your_config":
cmake --build . --config your_config ??????????????

https://github.com/electronicarts/EASTL/blob/master/CONTRIBUTING.md


generic template ??

template <class T> struct is_nothrow_move_constructible;

rvalue로 바로 꽂아넣어서 생성할 수 있는앤지

new보다는 calloc을..
calloc은 malloc +초기화라서 썼다






JVector.h 구조

A ) template class allocator

 1. 동적 배열 시작주소를 pHead에 저장

 2. 배열 크기를 nCapacity에 저장. nCapacity 는 2^k 를 만족한다
  : 그래야 벡터 사이즈 변경 동작이 amortized constant time complexity 를 만족한다고 한다

 3. T* allocate( long n ) 의 n 엔 아무숫자가 들어오지만, nCapacity = 2^k 를 만족하도록 할당하도록 했다
  : cmath log 로 계산했다

 4. calloc 과 realloc 을 사용했다
  a. new 말고 calloc 사용 = realloc 쓰려고
  b. realloc 쓰면 reallocation 필요없을때 이전 주소를 주는 것 같다. 이때 realloc 필요없어서 원소 이동시키기 위한 복사 안 해줘도 된다
  c. realloc 필요시 이전 메모리 free


JList.h 구조

com1 == (Complex)3.0
explicit list ??

ZList AddTail 할 시 객체를 RefCount로 감싼 (prev, next 포인터 저장하기 위해) 객체를 만들어주는데,
열거형 for문 `for ( auto& pDrop : m_lDrop )` 를 쓸 때, 
iterator는 operator++ 로 GetNext는 감싼 객체를 받아오는 것이 아니라, pDrop에 내용물만 가져와 주므로 (prev, next 없는 순수 내용물) 
ZList 에서 remove 시 remove 된 객체의 prev, next 를 사용할 수 없는 것으로 이해했습니다

지라 코드와 관련은 없는 것으로., 전 removeAt 없이 리스트를 읽고만 있으니, 락 잡기만 하면 되는 것


allocator 를 long 으로 받으면 prev, next 포인터 메모리는 어떻게 받지


node 안에 T 메모리 할당할때 allocator T 를 또 부른다
..? 어떻게

ZList 에 prev, next 가 껴있던 이유..
rebind_alloc이란? 쓰는 법?


data 자료형으로 allocator<T> 가져오는 건 그대로 유지,
rebind_alloc<node<T>> 를 이용한다.

allocator traits : std::allocator_traits<Alloc> 해서 쓰면 rebind 기능 쓸 수 

디폴트 allocator만 쓰는 경우
typedef allocator<node<T>> allocator_type; 로 되지만
템플릿 구조를 이렇게 짜면 쓴 의미가 없음



allocator<T> 로 써야지
그 안의 중첩 수조체를 쓸 수가 있다

default allocator를 list 만 쓰는 게 아닐테니

그리고 construct() and destroy() 할 때 쓰인다고 한다

1. allocator_type 를 T가 아닌 node<T>로 선언한다
 : list를 template < class T, class Alloc = allocator<T> > 로 받기 때문에,
받는 allocate의 typename을 node<T> 로도 사용하기 위해,
받는 allocate의 중첩 구조체 템플릿안에 typedef된 my_other란 타입을 가져오게 한다.

중첩 구조체 안의 typedef 만 가져올땐 구조체 선언까진 안한다

2. pointer 를 def 할 때도 node<T>의 포인터를 사용한다

3. allocator<T> 는 node 들의 T 부분을 construct() and destroy() 할 때 쓰인다고 하는데 무슨말인지 모르겠다



allocate에 사이즈만 주는거라면
두배늘릴때
기존 메모리 이용해서 두배 늘릴수 있으면
deallocate바로 해줄 필요 없는데 이 부분 처리 어떻게 하지


Vector reallocate 과정, allocator<T> 수정

1. reallocate 를 위한 컨테이너 크기 관리는 reserve 에서
2. allocator 멤버 allocate는 메모리 할당만 하도록
3. pointer allocate( size_type n, allocator<void>::const_pointer hint = 0 ) 두 번째 인자 사용법?
4. realloc 은 같은 포인터에서 사이즈 증량 가능하면 이동, free도 해준다
5. free는 할당할때 크기만큼 동째로 free 해주는 것 같다


근데 void deallocate( pointer p, size_type n ) // n?? ??

typedef value_type* iterator; x/

-> 오버로딩 해버렸더니
.이 안써짐

*만 해야하는 이유?


		//대입 연산자
		value_type& operator=( const iterator & it )
		{
			pEl = it.pEl;
			return *pEl; // 반환하는 이유??
		}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
2주차

list
iterator& operator++

next가 없다면 어떤 처리 하는 지 보기


lvalue :
1. 등호 왼쪽에 올 수 있다
2. 이름이 있고, 단일식을 벗어나서 사용가능
3. 주소 연산자로 주소를 구할 수 있다
4. 참조를 반환하는 함수
문자열 literal

rvalue :
1. 등호 왼쪽에 올 수 없다
2. 이름이 없고, 단일 식에서만 사용
3. 주소연산자로 주소를 구할 수 없다
4. 값을 반환 하는 함수
실수/정수 literal
임시객체 (temporary)


c1 = 10

10은 코드 메모리에

value_category로 조사

++n lvalue
n++ rvalue
"AA" value

https://n-learning.ispringlearn.com/content/info/7196?vc_cik=90209863-jX3Ps-BvLYv-338Zv&vc_lpid=7196

&& rvalue refernce
l, r 모두 가리킬 수 있다


int&& r = 3;
상수성 없이 rvalue를 가리키는 것이 중요하다
move semantics, perfect forwarding 을 위해서

overloading 규칙
1. 값 타입과 참조타입은 같이 오버로딩 될 수 없다

foo( X&& x)
move semantics를 사용하겠다는 의도
rvalue만 받을 수 있다

reserve 구현도 보기


foo(static_cast<X&&>(rx)); // 타입이 아닌 value를 변환하는 캐스팅 : lvalue-> rvalue를 캐스팅

reference collapsing
https://n-learning.ispringlearn.com/content/info/7196?vc_cik=90209863-bsgKt-JYKwK-PyQHx&vc_lpid=7196



https://stackoverflow.com/questions/30123338/use-operator-t-instead-of-t-operator-for-member-access


S-_counted_deleter?

new[] 시 반드시 삭제자를 변경해야 한다
[] 연산자가 제공되지 않는다
 - vector, 또는 array를 사용 권장

 





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
3주차

unique_ptr : 복사 불가능

explicit unique_ptr(pointer _Ptr) : 

std::unique_ptr<myClass> myClass(new myClass());
std::unique_ptr<myClass> myClass2 = std::make_unique<myClass>(); // due to exception safety
// slightly safe 만들떄 댕글링 가능성 없다

pass if to a function 하려면 copy 해야하는데 이건 안됨
새로 할당도 안됨

copy constructor,m assignment  = delete 처리가 되어있다


reference counting

std::shared_ptr<myClass> shared = std::make_shared<myClass>()// 무조건.
controlblock 만들때 메모리 같이 만들게 해줘서




weak_ptr : checks if alive, but it wont keep it alive

if you need to share : use unique pointer

make_shared
 - 대상 객체와 제어 블럭을 동시에 메모리 할당하므로 보다 효율적이다
 - 예외 상황에 좀 더 안전하다

f( shared_ptr<Car>(new Car), foo())
얘네 만들어지는 순서가 표준으로 언급되어 있지 않다

new Car -> foo()호출 했는데 예외가 나왔다면 : shared<ptr> 호출 못됐다면 자원 누수

f( make_shared<Car>(), foo()) // 이러면 직접 메모리 할당 하지 않아서. 위에 문제 사라진다.



* 주의 할 점
Car* p = new Car;

shared_ptr<Car> sp1(p);
shared_ptr<Car> sp2(p); 

// 각각 따로 제어블록 생성 = 두번 delete

RAII
resource acquisition is initialization


enable_shared_from_this

https://n-learning.ispringlearn.com/content/info/7205?vc_cik=90209863-0siX2-yfCtD-M8xDn&vc_lpid=7205
CRTP 개념

자기 자신의 참조계수 증가



std::string name; 는 8 크기 할당

sizeof( std::string ) == 28

class MyClass { std::string s; };
sizeof( MyClass ) == 28

new MyClass 하면 28, 8 두개 할당



https://stackoverflow.com/questions/34560502/why-is-sizeofstdstring-only-eight-bytes
https://stackoverflow.com/questions/3770781/why-is-sizeofstring-32#:~:text=So%20this%20string%20implementation%20is,environment%20it%20is%20used%20in.

![image](https://user-images.githubusercontent.com/74404132/216805982-736e9ae9-ed9b-47bf-895e-c2923a3d972a.png)