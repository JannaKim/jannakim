#include <iostream>
#include <Windows.h>
#include <winuser.h>
#include <list>
#include <memory> // ǥ���� �ƴ����� �����Ϸ��� global namespace���� ������
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
#include <cstdio>
#include "MathLibrary.h"
#include <functional>

#pragma comment(lib, "Winmm.lib") //winmm.lib �߰�
std::ostream& tab( std::ostream& os );
std::ostream& space( std::ostream& os );
std::ostream& enter( std::ostream& os );

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
void test_cout();
void test_multiple_inheritance();
void test_cout_and_user_defined_type();
void test_custom_endl();
void test_doubleplus_operator_overloading();
void test_explicit_bool_operator();
void test_perfect_forwarding();
void test_composite_pattern();
void test_policy_based_design();
void test_function_pointer();
void test_adapter_pattern();
void test_bridge_pattern();

struct RecordTime
{
	inline static std::list<LONG> lGap;
	inline static LONG tTime;

	inline bool operator()()
	{
		LONG tCur = ::timeGetTime();
		if ( RecordTime::tTime )
			RecordTime::lGap.emplace_back( tCur - RecordTime::tTime );
		RecordTime::tTime = tCur;

		return true;
	}

	static void Log()
	{
		for ( LONG interval : IntervalRecorder::m_lInterval )
			std::cout << interval << " ";
	}
};

int main()
{
	RecordTime RecordTimeTaken;
	RecordTimeTaken();
	RecordTimeTaken();
	RecordTimeTaken();
	RecordTimeTaken::Log();

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
	//test_thread_std_modoocode();

	//double a = 7.4;
	//int b = 99;

	//std::cout << "a + b = " <<
	//	MathLibrary::Arithmetic::Add( a, b ) << std::endl;

	//const char* name = u8"Janna"; // 1 byte

	//const wchar_t* name2 = L"Janna"; // 2byte

	//const char16_t* name3 = u"Janna"; // 2 byte
	//const char32_t* name4 = U"Janna";

	//{
	//	std::string name = "cherno";
	//}
	//test_cout();

	//std::list<long> lTest(1);
	//std::list<long>::iterator it = lTest.begin();
	//it = lTest.begin();

	//test_multiple_inheritance();
	//test_cout_and_user_defined_type();
	//test_custom_endl();
	// test_doubleplus_operator_overloading();
	//test_explicit_bool_operator();
	//test_perfect_forwarding();
	//test_composite_pattern();
	//test_policy_based_design();

	//test_function_pointer();
	//test_adapter_pattern();
	//test_bridge_pattern();
}

struct IMP3
{
	virtual void Play() = 0;
	virtual void Stop() = 0;
	virtual ~IMP3() {}
};

void test_bridge_pattern()
{

}

class Shape
{
public:
	virtual void Draw() 
	{
		std::cout << "Shape::Draw\n";
	}
};

class TextView
{
	std::string s;
public:
	TextView( std::string s ) : s( s ) {}
	void Show() {
		std::cout << s << enter;
	}
};
class ObjectAdapter : public Shape
{
	TextView* pView; // �����Ͱ� �ٽ� : �̹� �����ϴ� ��ü�� �޴� �Ŷ�. ������ ���� ������ Ŭ���� ����Ͱ� �ȴ� 
public:
	ObjectAdapter( TextView* p ) : pView( p ) {}

	virtual void Draw() override { pView->Show();  }
};

void test_adapter_pattern()
{
	vector<Shape*> v;
	TextView tv( "world" );
	v.push_back( new ObjectAdapter( &tv ) );

	for ( auto& p : v )
		p->Draw();
}

class ComboBox
{
public:
	ComboBox() : myTransFunc( nullptr ) {}

	// Defines a function pointer type pointing to a void function with parameters.
	typedef void ( *TransFunc )( std::string, std::vector<std::pair<std::string, std::string>> );
	//  Actually defines a member variable of this type.
	TransFunc myTransFunc;

	void Do()
	{
		// Uses the member variable to call a supplied function.
		if ( myTransFunc )
			( *myTransFunc )( "Hello", container );
	}

	std::vector<std::pair<std::string, std::string>> container;
};

void Gather( std::string s, std::vector<std::pair<std::string, std::string>> container )
{
	std::cout << s << enter;
}
void test_function_pointer()
{
	ComboBox* cb = new ComboBox();
	cb->myTransFunc = Gather; // // Assigns the member function pointer to an actual function.

	cb->Do();
}

class ISync
{
public:
	virtual void Lock() = 0; // �����Լ��� �ζ��� ġȯ�� �ȵȴ�. ���� �������� �˼� ���
	virtual void Unlock() = 0;
	virtual ~ISync() {} // �����Լ��� �ϳ��� ������ 
};

class MutexLock : public ISync
{
	// Mutex mutex;
public:
	virtual void Lock() override 
	{
		std::cout << "lock" << enter;
	};
	virtual void Unlock() override 
	{
		std::cout << "unlock" << enter;
	};
};

class FakeLock
{
public: // �����Լ��� ����� ����־ �����Ѵ�
	void Lock()
	{};
	void Unlock()
	{};
};

template<typename T, typename ThreadModel = NoLock >
class List
{
	ThreadModel tm;

public:
	void push_front( const T& a )
	{
		tm.Lock(); // �����Լ��� �������ϰ� ���� �� �ִ� : �ݿ� ���� �������
		// ...
		std::cout << "push_front" << enter;
		tm.Unlock();
	}
	~List() {
	}
};

// ���� ���� ����
void test_policy_based_design()
{
	// ���� ���� ��
	List<long, MutexLock> l;
	l.push_front(1);

	List<long, FakeLock> l2;
	l2.push_front( -1 );
}


void test_composite_pattern()
{
	 // making menu #
	// GUI ��Ŭ�� ����� �޴�
	// MenuItem : ���� �� ��
	// PopupMenu : ���� �޴��� ������

	//vector<MenuItem*> ���� ����� PopUpMenu �� �ٽ� �� PopupMenu�� ���� �� ����


}

void foo( int n ) { std::cout << "foo" << enter; }
void goo( int& r ) 
{
	std::cout << "goo" << enter;
	r = 10; 
}
void hoo( int&& r )
{
	std::cout << "hoo" << enter;
	r = 99;
}
template<class F, class T>
void chronometry( F&& f, T&& arg )
{
	// ���� �ð� ����
	f(static_cast<T&&>( arg ) ); // rvalue ĳ����. std::forward<T>(arg) �� ���� 
}

void test_perfect_forwarding()
{
	int n = 5;
	chronometry( foo, n ); // 5
	std::cout << n << enter;
	chronometry( goo, n ); // 10
	std::cout << n << enter;
	chronometry( hoo, 3 ); // 10
	std::cout << n << enter;

}

class Machine
{
	int data = 10;
	bool state = true;
public:
	Machine()
	{
		std::cout << "Machine\n";
	}
	explicit operator bool() { // c++11 ���� ����
		return state;
	}

	Machine& operator<<( int a ) {
		data += a;
		return *this;
	}
};
void test_explicit_bool_operator()
{
	Machine m;

	//bool b1 = m; 1. .operator=(b1) ���� Ȯ��, �����ϱ� 
	bool b2 = static_cast<bool>(m);
	// bool b3 = static_cast<bool&>( m ); //  'static_cast' : 'Machine'���� 'bool & �� ��ȯ�� �� x
	// m << 10; bool shift ������ó�� ����
	m << 10; // operator<< �� �ִٸ� ���� ���
	if ( m ) { // explicit ��ȯ �����ڷ� ���� if �������� static_cast ���� ����� �� �ִ�

	}
}

class Point
{
	int x;
	int y;
public:
	Point( int a = 0, int b = 0 ) : x( a ), y( b ) {}

	friend std::ostream& operator<<( std::ostream& os, const Point& pt );

	Point& operator++() {
		++x;
		++y;
		return *this;
	}

	const Point operator++( int ) {
		Point tmp( *this );
		++( *this );
		return tmp;
	}
};


void test_doubleplus_operator_overloading()
{
	Point p( 1, 1 );
	Point p1 = ++p;
	Point p2 = p++;
	// p++++ �������� : return type const
}


class ostream // cout �� stream Ÿ���� ��ü�̴�
{
public:
	ostream& put( char c ) {
		printf( "%c", c );
		return *this;
	}

	ostream& flush() { return *this;  }// ��� ���� �����

	ostream& operator<<( int n ) {
		printf( "%d", n );
		return *this;
	}
	ostream& operator<<( double d ) {
		printf( "%f", d );
		return *this; // "�ڽ��� ����" �� ��ȯ
	}

	ostream& operator<<( ostream& ( *f )( ostream& ) )
	{
		f( *this );
		return *this;
	}
};

ostream cout;

ostream& endl( ostream& os )
{
	os.put( '\n' );
	os.flush();
	return os;
}

std::ostream& tab( std::ostream& os )
{
	os << '\t';
	return os;
}

std::ostream& space( std::ostream& os )
{
	os.put( ' ' );
	os.flush();
	return os;
}

std::ostream& enter( std::ostream& os )
{
	os.put( '\n' );
	os.flush();
	return os;
}

void test_custom_endl()
{
	std::cout << std::endl;
	std::cout.operator<<( std::endl );
	std::endl( std::cout );
	
	std::cout << 'a' << tab << 'b' << enter;
	std::cout << 'c' << space << 'd';
	/////////////////////////////////////////////////////////////
	
	cout.operator<<( endl );
	cout << endl;
}

std::ostream& operator<<( std::ostream& os, const Point& pt ) 
{
	// std::ostream �� const �� ���̴� ���� : �����ü�� ����Լ��� �θ� �� �ִµ�
	// os.operator(pt.x) �� ����Լ��� �ƴϾ
	os << pt.x << pt.y << std::endl;
	return os;
}

void test_cout_and_user_defined_type()
{
	std::cout << 1; // cout.operator(1);

	Point pt( 1, 2 ); 
	std::cout << pt; // opeator<<(ostream, Point)
}

class File
{
public:
	std::string filename = "None";
	virtual void open() {
		std::cout << "File::open\n";
	};
};

class InputFile : virtual public File
{
public:
	void read() {}
	virtual void open() override {
		std::cout << "InputFile::open\n";
	}
};

class OutputFile : virtual public File
{
public:
	void write();
	//virtual void open() override {
	//	std::cout << "OutputFile::open\n";
	//}
};

class IOFile : public InputFile, public OutputFile
{
public:
};


void test_multiple_inheritance()
{
	IOFile file;
	file.InputFile::open();
	file.OutputFile::open();
	file.filename = "Jannakim";
	std::cout << file.InputFile::filename << std::endl;
	std::cout << file.OutputFile::filename << std::endl;
}

void test_cout()
{
	ostream cout;
	cout << 3; // cout.operator<<(3)
	cout << 4 << 3.14;

	cout.operator<<( 1 ).operator<<(2).operator<<(3);

	std::wcout << L"Hello World";
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

//std::queue<std::pair<size_t, void*>> g_AllocInfo;
//bool bQueueAlloc = true;
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
	std::cout << "�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�" << std::endl;
	std::cout << "allocated info :\n";
	//while ( !g_AllocInfo.empty() ) {
	//	auto info = g_AllocInfo.front();

	//	unsigned char const* pos = (unsigned char const*)info.second;
	//	std::cout << info.second << " : size" << (long)info.first << std::endl;
	//	for ( auto i = 0; i < (long)info.first; i++ )
	//		printf( "|%2.2x| ", pos[ i ] );

	//	g_AllocInfo.pop();
	//	std::cout << std::endl;
	//}
	std::cout << "�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�" << std::endl;
}



void test_class_with_string_member()
{
	//bQueueAlloc = false;

	{
		std::string name = "cherno";
	}
	const char* s1 =
		R"(
		_CONSTEXPR20_CONTAINER basic_string
		)";

	
	print( "> std::string name(15, 'A');" );
	std::string name( 15, 'A' ); 
	// new 8 :  74 f9 bd 00 00 00 00 00  �ּ������  8 byte �Ҵ�ȴ� : �ּ� ������
	// ?? ?? ?? ?? 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 00 0f(ũ�� 15)

	print_alloc_info();
	&name;
	std::cout << "&name : " << &name << std::endl;
	unsigned char const* pos = (unsigned char const*)&name;
	std::cout << " &name �� ��� �ִ� �� : " << std::endl;
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
	std::cout << " &name2 �� ��� �ִ� �� : " << std::endl;
	for ( int i = 0; i < sizeof( name2 ); i++ )
		printf( "|%2.2x| ", pos3[ i ] );

	std::cout << "\n\n\n";

	// 31 > 15 �̶� _Reallocate_for ź��.
	
	R"(
	_CONSTEXPR20_CONTAINER basic_string& assign( _CRT_GUARDOVERFLOW const size_type _Count, const _Elem _Ch ) {
		// assign _Count * _Ch
		if ( _Count <= _Mypair._Myval2._Myres ) { // size_type _Myres  = 0; // current storage reserved for string
			_Elem* const _Old_ptr = _Mypair._Myval2._Myptr();   // new�� �Ҵ��� 8byte�� ��� �ּҸ� ����Ű�� ������
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
	)";
	

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

	CString* cs = new CString(); // empty class size = 1, ����Ʈ ������ ���� �����ص� 1
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
	// Worker ��ü�� ���ο� ���ε��� ���� ���� �� �ı� �ž� �Ѵ�
	// == Worker��ü�� �ڱ��ڽ��� ��������� ���� �ؾ� �Ѵ�
	std::shared_ptr<Worker> holdMe;

public:
	Worker() { std::cout << "Worker()\n"; }

	~Worker() { 

		std::cout << "~Worker()\n"; 
	}

	void Run()
	{
		holdMe = shared_from_this(); // CRTP ��� : this�� ������ �������� �����ϸ� shared_ptr�� ���� �� �ְ� �Ѵ�
		// ����������� ������ undefined ����
		std::thread t( &Worker::Main, this );
		t.join();
		//t.detach(); ���� ���� �ȵ�
	}

	//����� �ȿ���
	void Run2(std::shared_ptr<Worker>& p)
	{
		// holeMe = this; // ������ ���� �������� ��ü �ִ°Ŷ� �ٸ� �����Ͱ� ��������� �ȵȴ�
		holdMe = p;
		std::thread t( &Worker::Main, this );
		t.join();
		//t.detach(); ���� ���� �ȵ�

		// What is different between join() and detach() for multi threading in C++ ?
		// https://stackoverflow.com/questions/37015775/what-is-different-between-join-and-detach-for-multi-threading-in-c


	}

	void Main()
	{
		std::cout << "Start thread\n";
		c.Go();                                                            // 3 : Do() ���� ������ Car go ��� x : �̹� �ı��Ǿ
		std::cout << "finish thread\n";

		holdMe.reset();
	}
};

void test_smart_pointer_std_advanced()
{

	{
		std::shared_ptr<Worker> myWorker = std::make_shared<Worker>();
		myWorker->Run();                                                   // 1
		//myWorker->Run2( myWorker ); // //����� �ȿ���
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
	std::shared_ptr<Car> p( new Car ); // �����Ͱ� �ƴ϶� ��ü, ���������� Car�� �ּҸ� ����
	// ��ü�ϱ� �Ҹ��ڸ� �θ���



	int a = 0; // copy initialization
	int b( 0 ); // direct initialization

	// std::shared_ptr<Car> p3 = new Car; // error explicit ������ �̷��� �� �� ����
	std::shared_ptr<Car> p3( new Car ); // explicit ������

	std::shared_ptr<Car> p4 = p3;

	//std::shared_ptr<Car> p5( new Car, foo );
	std::shared_ptr<Car> p6( new Car, []( Car* p ) { delete p; } );


	// make_shared : ��� ��ü�� ������ �޸𸮸� ���ÿ� �̾ �Ҵ�

	std::shared_ptr<Car> p1 = std::make_shared<Car>();

	std::weak_ptr<Car> wp;
	{
		std::shared_ptr<Car> sp( new Car );
		wp = sp;
	} // Car ��ü �ı������� �������� �ı����� �ʴ´�. weak ����

	if ( wp.expired() ) {
		// �������� �ı��Ǵ� ������ sp, wp ��� ���� ��
	}

}

void test_smart_pointer_custom()
{
	
	{
		shared_ptr<long> p2( new long( 1 ) ); // ����Ʈ ������ �� ���� new �ι�
		shared_ptr<long> p22 = p2;
	}

	weak_ptr<Car> wp2;
	{
		shared_ptr<Car> sp( new Car );
		wp2 = sp;
	} // Car ��ü �ı������� �������� �ı����� �ʴ´�. weak ����

	if ( wp2.expired() ) {
		// �������� �ı��Ǵ� ������ sp, wp ��� ���� ��
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

	if ( TESTRVRef()( 5 ) ) // �ӽð�ü + bool operator()( long tmp = 1)
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

	for ( long it2 : lTest ) { // ���ο��� iterator begin() , end(), !=, ++ �� �������� it2�� * ������ ����Ǿ ���������
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


