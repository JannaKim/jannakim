#include <iostream>
#include <Windows.h>
#include <winuser.h>
#include <list>
#include <memory> // 표준은 아니지만 컴파일러가 global namespace에도 제공함



#include "JVector.h"
#include "JList.h"
#include "JSort.h"





void test_JVector();
void test_JList();


int main()
{
	test_JVector();
	test_JList();



	//test_movelist();

}

//1. vector 가변 크기 배열
//>> push_back
//>> pop_back
//>> resize
//>> [] 접근
//
//2. list 양방향 리스트
//>> push_back
//>> push_front
//>> pop_back
//>> pop_front
//
//3. iterator*
//>> ++, --
//>> *연산자
//>> 비교 연산자
//>> 동일한 다른 컨테이너에 접근 가능, it = A.begin(), it = B.begin()
//
//4. 알고리즘
//>> 버블 정렬 - 범용적인 알고리즘 작성이 목적이기에 어려운 알고리즘이 아닌 간단한 정렬 알고리즘으로

void test_JList()
{
	std::cout << "test list\n";

	list<long> lTest;
	for ( long i = 0; i < 4; ++i )
		lTest.push_back( i );
	for ( long i = 6; i > 0; --i )
		lTest.push_front( i );
	for ( long i = 0; i < 3; ++i )
		lTest.pop_back();
	for ( long i = 0; i < 2; ++i )
		lTest.pop_front();

	list<long>::iterator it = lTest.begin();

	//it.operator++();
	//*it;

	for ( ; it != lTest.end(); ++it ) {
		std::cout << *it << '\n';
	}

	sort( lTest.begin(), lTest.end() );

	for ( auto it2 : lTest ) {
		std::cout << it2 << '\n';
	}

}
void test_JVector()
{
	std::cout << "test vector\n";
	vector<long> vTest;
	long n1 = 11;
	long n2 = 2;
	vTest.push_back( n1 );
	//std::cout << vTest[ 0 ] << '\n';
	vTest.push_back( n2 );
	//std::cout << vTest[ 1 ] << '\n';
	//std::cout << vTest[ 0 ] << '\n';

	for ( long i = 7; i > 3; --i )
		vTest.push_back( i );
	//for ( long i = 3; i < 50; ++i )
	//	//std::cout << vTest[ i - 1 ] << '\n';

	for ( auto it2 : vTest ) {
		std::cout << it2 << '\n';
	}

	sort( vTest.begin(), vTest.end() );

	for ( auto it2 : vTest ) {
		std::cout << it2 << '\n';
	}
}


