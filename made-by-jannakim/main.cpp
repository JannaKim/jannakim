#include <iostream>
#include <Windows.h>
#include <winuser.h>
#include <list>
#include <memory> // ǥ���� �ƴ����� �����Ϸ��� global namespace���� ������



#include "JVector.h"
#include "JList.h"
#include "JSort.h"


template < class T >
void print( T line )
{
	std::cout << line << std::endl; // operator TESTRVRef* () 
	std::cout << *line << std::endl; // ?? operator*()
	std::cout << line.a << std::endl;
	std::cout << line[0] << std::endl;
	//std::cout << *line[ 0 ] << std::endl;// operator TESTRVRef* () -> long& operator*()
}

void test_JVector();
void test_JList();
void test_operator_overloads();

int main()
{
	//test_JVector();
	//test_JList();
	test_operator_overloads();
}

//1. vector ���� ũ�� �迭
//>> push_back
//>> pop_back
//>> resize
//>> [] ����
//
//2. list ����� ����Ʈ
//>> push_back
//>> push_front
//>> pop_back
//>> pop_front
//
//3. iterator*
//>> ++, --
//>> *������
//>> �� ������
//>> ������ �ٸ� �����̳ʿ� ���� ����, it = A.begin(), it = B.begin()
//
//4. �˰���
//>> ���� ���� - �������� �˰��� �ۼ��� �����̱⿡ ����� �˰����� �ƴ� ������ ���� �˰�������

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


