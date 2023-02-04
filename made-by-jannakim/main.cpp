#include <iostream>
#include <Windows.h>
#include <winuser.h>
#include <list>
#include <memory> // 표준은 아니지만 컴파일러가 global namespace에도 제공함
#include <thread>

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


int main()
{
	//test_JVector();
	//test_JList();
	//test_JList2();
	// test_operator_overloads();

	// test_smart_pointer_std();
	//test_smart_pointer_custom();
	test_smart_pointer_std_advanced();
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

// delete 오버로딩?
void* operator new ( size_t size )
{
	std::cout << "New operator overloading, new size :" << size << std::endl;
	return malloc( size );
}

void operator delete( void* p )
{
	std::cout << "Delete operator overloading\n";
	free( p );
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


