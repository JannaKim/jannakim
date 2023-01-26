#include <iostream>
#include <Windows.h>
#include <winuser.h>
#include <list>
#include <memory> // ǥ���� �ƴ����� �����Ϸ��� global namespace���� ������



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


