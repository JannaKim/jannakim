#include <iostream>
#include "ecourse_dp.hpp"
#include "..\CalcProxy\ICalc.h"
using namespace std;
using namespace ecourse;

void test_proxy_pattern();
int main()
{
test_proxy_pattern();
}

class Calc : public ICalc
{
public:
	int Add( int a, int b ) { return a + b; }
	int Sub( int a, int b ) { return a - b; }
};
Calc calc;

int dispatch( int code, int x, int y )
{
	printf( "[DEBUG] %d, %d, %d\n", code, x, y );

	switch ( code ) {
	case 1:
		return calc.Add( x, y );
	case 2:
		return calc.Sub( x, y );
	}
	return -1;
}

void test_proxy_pattern()
{
	ec_start_server( std::string( "CalcService" ), dispatch ); // �� �̸����� ������ �����, �� ������ �����Ѵٸ� �� �Լ��� ȣ���ϰ� �ش޶�
}