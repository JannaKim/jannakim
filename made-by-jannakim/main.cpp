#include <iostream>
#include <Windows.h>
#include <winuser.h>
#include <list>
#include <memory> // 표준은 아니지만 컴파일러가 global namespace에도 제공함
#include <thread>
#include <string>
#include <utility>
#include <queue>
#include <cstdio>
#include <vector>
#include "JVector.h"
#include "JList.h"
#include "JSort.h"
#include "JMemory.h"

template < class T >
void print( T line )
{
	std::cout << line << std::endl; // operator TESTRVRef* () 
	//std::cout << *line << std::endl; // ?? operator*()
	//std::cout << line.a << std::endl;
	//std::cout << line[0] << std::endl;
	//std::cout << *line[ 0 ] << std::endl;// operator TESTRVRef* () -> long& operator*()
}

void test_JVector();
void test_JList();
void test_JList2();
void test_operator_overloads();
void test_smart_pointer_std();
void test_smart_pointer_std_advanced();
void test_smart_pointer_custom();
void test_circular_reference();
void test_class_with_string_member();
void test_thread_std();
void test_thread_std_modoocode();

int main()
{
	//test_JVector();
	//test_JList();
	//test_JList2();
	// test_operator_overloads();

	// test_smart_pointer_std();
	//test_smart_pointer_custom();
	//test_smart_pointer_std_advanced();
	//test_circular_reference();
	//test_class_with_string_member();
	//test_thread_std();
	test_thread_std_modoocode();
}

void func1() 
{
	for ( int i = 0; i < 10; ++i ) {
		std::thread::id id = std::this_thread::get_id();
		std::cout << id << std::endl;
	}
}

void func2()
{
	for ( int i = 0; i < 10; ++i ) {
		std::thread::id id = std::this_thread::get_id();
		std::cout << id << std::endl;
	}
}

void func3()
{
	for ( int i = 0; i < 10; ++i ) {
		std::thread::id id = std::this_thread::get_id();
		std::cout << id << std::endl;
	}
}

void worker( std::vector<int>::iterator start, std::vector<int>::iterator end, int* result )
{
	int sum = 0;
	for ( auto it = start; it < end; ++it ) {
		sum += *it;
	}
	*result = sum;
	std::thread::id id = std::this_thread::get_id();
	printf( "id : %x, sum of %d ~ %d = %d\n", id, *start, *(end-1), *result );
}
void test_thread_std_modoocode()
{
	
	//std::thread t1( func1 );
	//std::thread t2( func2 );
	//std::thread t3( func3 );

	//t1.join();
	//t2.join();
	//t3.join();
	std::vector<int> vData;
	for ( int i = 0; i < 10000; ++i )
		vData.push_back( i );

	std::vector<int> partial_sums;

	std::vector<std::thread> workers;
	for ( int i = 0; i < 4; ++i ) {
		partial_sums.push_back( 0 );
		workers.push_back( std::thread( worker, vData.begin() + i * 2500, vData.begin() + ( i + 1 ) * 2500, &partial_sums[ i ] ) );
	}
	for ( int i = 0; i < 4; ++i )
		workers[ i ].join();

	int total{};
	for ( auto it : vData )
		total += it;

	std::cout << " total sum " << total << std::endl;
}
void foo_thread()
{
	std::thread::id id = std::this_thread::get_id();
	std::cout << id << "\n";
	std::cout << "thread start\n";
	std::this_thread::sleep_for( std::chrono::seconds( 2 ) );
	std::cout << "thread end\n";
}
void test_thread_std()
{
	//thread::id id = this_thread::get_id();

	//this_thread::sleep_for( 3s );
	//this_thread::sleep_until( chrono::system_clock::now() + 3s );
	//this_thread::yield();

	std::thread t( &foo_thread );
	t.join();
}

void* operator new ( size_t size )
{
	void* p = malloc( size );
	//std::cout << "New operator overloading," << size << " " << p << std::endl;

	return p;
}

void operator delete( void* p )
{
	//std::cout << "Delete operator overloading\n";
	free( p );
}

class CString
{
	//long n;
	std::string s;
public:
	CString()
	{

	}
};

void* g_allocated_address;

std::queue<std::pair<size_t, void*>> g_AllocInfo;
bool bQueueAlloc = true;
//void* operator new ( size_t size )
//{
//	void* p = malloc( size );
//	
//	
//	if ( bQueueAlloc )
//		return p;
//	std::cout << "New operator overloading," << size << " " << p << std::endl;
//	
//	g_allocated_address = p;
//	bQueueAlloc = true;
//	g_AllocInfo.push( std::make_pair( size, p ) );
//	bQueueAlloc = false;
//	return p;
//}

void print_alloc_info()
{
	std::cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ" << std::endl;
	std::cout << "allocated info :\n";
	while ( !g_AllocInfo.empty() ) {
		auto info = g_AllocInfo.front();

		unsigned char const* pos = (unsigned char const*)info.second;
		std::cout << info.second << " : size" << (long)info.first << std::endl;
		for ( auto i = 0; i < (long)info.first; i++ )
			printf( "|%2.2x| ", pos[ i ] );

		g_AllocInfo.pop();
		std::cout << std::endl;
	}
	std::cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ" << std::endl;
}



void test_class_with_string_member()
{
	bQueueAlloc = false;
	print( "> std::string name(15, 'A');" );
	std::string name( 15, 'A' ); 
	// new 8 :  74 f9 bd 00 00 00 00 00  주소저장용  8 byte 할당된다 : 주소 들어가보면
	// ?? ?? ?? ?? 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 00 0f(크기 15)

	print_alloc_info();
	&name;
	std::cout << "&name : " << &name << std::endl;
	unsigned char const* pos = (unsigned char const*)&name;
	std::cout << " &name 에 들어 있는 값 : " << std::endl;
	for ( int i = 0; i < sizeof( name ); i++ )
		printf( "|%2.2x| ", pos[ i ] );

	std::cout << "\n\n\n";



	print( "> std::string name2( 31, 'A' );" );
	std::string name2( 31, 'A' );
	// new 8 : 64 f6 0f 01 00 00 00 00
	// new 32 : 0x0151b368 
	print_alloc_info();

	std::cout << "&name2 : " << &name2 << std::endl;
	unsigned char const* pos3 = (unsigned char const*)&name2;
	std::cout << " &name2 에 들어 있는 값 : " << std::endl;
	for ( int i = 0; i < sizeof( name2 ); i++ )
		printf( "|%2.2x| ", pos3[ i ] );

	std::cout << "\n\n\n";

	// 31 > 15 이라서 _Reallocate_for 탄다.
	/*
	_CONSTEXPR20_CONTAINER basic_string& assign( _CRT_GUARDOVERFLOW const size_type _Count, const _Elem _Ch ) {
		// assign _Count * _Ch
		if ( _Count <= _Mypair._Myval2._Myres ) {
			_Elem* const _Old_ptr = _Mypair._Myval2._Myptr();   // new로 할당한 8byte에 담긴 주소를 가리키는 포인터
			_Mypair._Myval2._Mysize = _Count;
			_Traits::assign( _Old_ptr, _Count, _Ch );
			_Traits::assign( _Old_ptr[ _Count ], _Elem() );
			return *this;
		}

		return _Reallocate_for(
			_Count,
			[]( _Elem* const _New_ptr, const size_type _Count, const _Elem _Ch ) {
				_Traits::assign( _New_ptr, _Count, _Ch );
				_Traits::assign( _New_ptr[ _Count ], _Elem() );
			},
			_Ch );
	}

	_Construct_in_place(_Mypair._Myval2._Bx._Ptr, _New_ptr);

	*/

	std::string name3( 63, 'A' );
	// new 8
	// new 64
	print_alloc_info();
	std::cout << std::endl;

	//std::string name4();
	// nothing
	std::cout << std::endl;

	std::cout << "Size of obj name is = " << sizeof( name ) << std::endl;
	// You are outputting the size of a std::string object, not the contents of 'em
	// 28
	std::cout << std::endl;

	std::cout << "Size of std::string is = " << sizeof( std::string ) << std::endl;
	// You are outputting the size of a std::string object, not the contents of 'em
	// 28
	std::cout << std::endl;

	CString e;
	// new 8
	std::cout << std::endl;

	std::cout << "Size of CString is = " << sizeof( CString ) << std::endl;
	// 28
	std::cout << std::endl;

	CString* cs = new CString(); // empty class size = 1, 디폴트 생성자 따로 선언해도 1
	// new 28
	// new 8
	std::cout << std::endl;

	std::shared_ptr<CString> cs2( new CString );
	// new 28
	// new 8
	// new 16
	std::cout << std::endl;

	std::shared_ptr<CString> cs3 = std::make_shared<CString>();
	// new 40
	// new 8
	std::cout << std::endl;
}

class People
{
	std::string name;

public:
	People() {
		//std::cout << "People()\n";
	}
	People( std::string s ) : name( s ) {
		//std::cout << "People( std::string s )\n";
	}

	~People() { 
		std::cout << "~People()\n";
	}

	std::shared_ptr<People> bf; // best friend
};

void test_circular_reference()
{
	std::shared_ptr<People> pP1 = std::make_shared<People>( "Kim" );
	std::shared_ptr<People> pP2 = std::make_shared<People>("Lee");

	pP1->bf = pP2;
	pP2->bf = pP1; // circular reference
}

class Car
{
	int color;
public:
	Car() : color( 0 ) { std::cout << "Car()\n"; }
	~Car() { 
		std::cout << "~Car()\n"; 
	}

	void Go() { 
		std::cout << "Car go\n"; 
	}
};

class Worker : public std::enable_shared_from_this<Worker> // CRTP
{
	Car c;
	// Worker 객체는 새로운 스로데드 종료 됐을 때 파괴 돼야 한다
	// == Worker객체가 자기자신의 참조계수를 증가 해야 한다
	std::shared_ptr<Worker> holdMe;

public:
	Worker() { std::cout << "Worker()\n"; }

	~Worker() { 

		std::cout << "~Worker()\n"; 
	}

	void Run()
	{
		holdMe = shared_from_this(); // CRTP 기술 : this를 가지고 제어블록을 공유하면 shared_ptr를 만들 수 있게 한다
		// 만들어져있지 않으면 undefined 동작
		std::thread t( &Worker::Main, this );
		t.join();
		//t.detach(); 같이 쓰면 안됨
	}

	//모양이 안예쁨
	void Run2(std::shared_ptr<Worker>& p)
	{
		// holeMe = this; // 제어블록 없는 독립적인 객체 넣는거라 다른 포인터가 만들어져서 안된다
		holdMe = p;
		std::thread t( &Worker::Main, this );
		t.join();
		//t.detach(); 같이 쓰면 안됨

		// What is different between join() and detach() for multi threading in C++ ?
		// https://stackoverflow.com/questions/37015775/what-is-different-between-join-and-detach-for-multi-threading-in-c


	}

	void Main()
	{
		std::cout << "Start thread\n";
		c.Go();                                                            // 3 : Do() 들어가긴 하지만 Car go 출력 x : 이미 파괴되어서
		std::cout << "finish thread\n";

		holdMe.reset();
	}
};

void test_smart_pointer_std_advanced()
{

	{
		std::shared_ptr<Worker> myWorker = std::make_shared<Worker>();
		myWorker->Run();                                                   // 1
		//myWorker->Run2( myWorker ); // //모양이 안예쁨
	}                                                                      // 2 : ~Worker(), ~Car()
	
	// Blocks the current thread until the thread identified by *this finishes its execution.

}

void foo( Car* p )
{
	print( "delete Car" );
}
void test_smart_pointer_std()
{
	//  Tools > Options > Debugging > General, "Enable Just My Code" unticked

	// Car* p = new Car;
	std::shared_ptr<Car> p( new Car ); // 포인터가 아니라 객체, 내부적으로 Car의 주소를 보관
	// 객체니까 소멸자를 부를것



	int a = 0; // copy initialization
	int b( 0 ); // direct initialization

	// std::shared_ptr<Car> p3 = new Car; // error explicit 있으면 이렇게 쓸 수 없다
	std::shared_ptr<Car> p3( new Car ); // explicit 생성자

	std::shared_ptr<Car> p4 = p3;

	std::shared_ptr<Car> p5( new Car, foo );
	std::shared_ptr<Car> p6( new Car, []( Car* p ) { delete p; } );


	// make_shared : 대상 객체와 제어블록 메모리를 동시에 이어서 할당

	std::shared_ptr<Car> p1 = std::make_shared<Car>();

	std::weak_ptr<Car> wp;
	{
		std::shared_ptr<Car> sp( new Car );
		wp = sp;
	} // Car 객체 파괴되지만 제어블록은 파괴되지 않는다. weak 땜에

	if ( wp.expired() ) {
		// 제어블록이 파괴되는 시점은 sp, wp 모두 없을 때
	}

}

void test_smart_pointer_custom()
{
	
	{
		shared_ptr<long> p2( new long( 1 ) ); // 스마트 포인터 한 번에 new 두번
		shared_ptr<long> p22 = p2;
	}

	weak_ptr<Car> wp2;
	{
		shared_ptr<Car> sp( new Car );
		wp2 = sp;
	} // Car 객체 파괴되지만 제어블록은 파괴되지 않는다. weak 땜에

	if ( wp2.expired() ) {
		// 제어블록이 파괴되는 시점은 sp, wp 모두 없을 때
	}

}

void test_operator_overloads()
{
	TESTRVRef A( 5 ); // A = {a=-5 b=9 }
	print( A );

	A(); // bool operator()( long tmp = 1)

	if ( A ) // operator TESTRVRef* () 
		std::cout << "A" << std::endl;
	if ( A() ) // bool operator()( long tmp = 1)
		std::cout << "A()" << std::endl;

	if ( A( 2 ) ) // bool operator()( long tmp = 1)
		std::cout << "A(2)" << std::endl;

	if ( A.operator()( 3 ) ) // bool operator()( long tmp = 1)
		std::cout << "A.operator()(3)" << std::endl;

	if ( (A)( 4 ) ) // bool operator()( long tmp = 1)
		std::cout << "(A)( 4 )" << std::endl;

	if ( TESTRVRef()( 5 ) ) // 임시객체 + bool operator()( long tmp = 1)
		std::cout << "TESTRVRef()(5)" << std::endl;
}

void test_JList()
{
	std::cout << "test list\n";

	list<long> lTest;
	for ( long i = 0; i < 6; ++i )
		lTest.push_front( i );
	for ( long i = 4; i > 0; --i )
		lTest.push_back( i );
	for ( long i = 0; i < 2; ++i )
		lTest.pop_back();
	for ( long i = 0; i < 3; ++i )
		lTest.pop_front();

	list<long>::iterator it = lTest.begin();

	//it.operator++();
	//*it;

	for ( ; it != lTest.end(); ++it ) {
		std::cout << *it << '\n';
	}

	sort( lTest.begin(), lTest.end() );

	for ( long it2 : lTest ) { // 내부에서 iterator begin() , end(), !=, ++ 로 돌리지만 it2는 * 연산자 적용되어서 만들어진다
		std::cout << it2 << '\n';
	}

}

void test_JList2()
{
	std::cout << "test list : rvalue reference, custom struct \n";

	long v = 1;
	++v = 7; // rvalue

	TESTRVRef A( 5 ); // A = {a=-5 b=9 }
	A.a++;; // lvalue.  -4
	++A.a -= 1; // -4

	TESTRVRef B( A.a ); // A( long& _a ) B = {a=-4 b=9 }
	TESTRVRef B2( ( long )++A.a );  // ( long&& _a )  B2 = {a=3 b=9 }
	TESTRVRef C( v );  // ( long& _a )
	

	list<TESTRVRef> lTest2;

	lTest2.push_front( A );
	lTest2.push_back( B );
	lTest2.push_back( B2 );
	lTest2.push_front( 10 ); // implicit
	lTest2.push_front( C );
	
	for ( list<TESTRVRef>::iterator it = lTest2.begin(); it != lTest2.end(); ++it ) {
		std::cout << ( *it ).a << '\n';
	}


	std::cout << "test list : std::list, list operator->() \n";

	std::list<TESTRVRef> my_list;
	my_list.emplace_back( 2 );

	std::list<TESTRVRef>::iterator std_it = my_list.begin();
	std::cout << std_it->a << std::endl;
	std::cout << ( *std_it ).a << std::endl;

	list<TESTRVRef> my_list2;
	//my_list.emplace_back( 3 );
	my_list2.push_back( 3 );

	list<TESTRVRef>::iterator it2 = my_list2.begin();
	std::cout << it2->a << std::endl;
	std::cout << ( *it2 ).a << std::endl;
}
void test_JVector()
{
	std::cout << "test vector\n";
	vector<long> vTest;
	vTest.push_back( 11 );
	//std::cout << vTest[ 0 ] << '\n';
	vTest.push_back( 2 );
	//std::cout << vTest[ 1 ] << '\n';
	//std::cout << vTest[ 0 ] << '\n';

	for ( long i = 7; i > 3; --i )
		vTest.push_back( i );
	//for ( long i = 3; i < 50; ++i )
	//	//std::cout << vTest[ i - 1 ] << '\n';

	for ( long it2 : vTest ) {
		std::cout << it2 << '\n';
	}

	sort( vTest.begin(), vTest.end() );

	for ( long it2 : vTest ) {
		std::cout << it2 << '\n';
	}


	std::cout << "test vector : rvalue reference, custom struct \n";

	long v = 1;
	++v = 7;

	TESTRVRef A( 5 );
	A.a++; // -4
	++A.a -= 1; // -4

	TESTRVRef B( A.a ); // ( long& _a ) ?
	TESTRVRef C( v );  // ( long& _a )

	vector<TESTRVRef> vTest2;

	vTest2.push_back( A );
	vTest2.push_back( B );
	vTest2.push_back( C );
	vTest2.push_back( 10 ); // implicit

	for ( vector<TESTRVRef>::iterator it = vTest2.begin(); it != vTest2.end(); ++it ) 		
	{
		std::cout << (*it).a << '\n';
	}


}


