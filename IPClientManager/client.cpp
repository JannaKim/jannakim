#include "..\IPCServerManager\ecourse_dp.hpp"
#include "..\CalcProxy\ICalc.h"

using namespace std;
using namespace ecourse;

void test_proxy_pattern();
int main()
{
	test_proxy_pattern();
}

typedef ICalc* ( *F )( );

void test_proxy_pattern()
{
	// 동적 모듈 load
	void* addr = ec_load_module( "CalcProxy.dll" );

	F f = (F)ec_get_function_address( addr, "CreateCalc" );

	ICalc* pCalc = f();

	//Calc* pCalc = new Calc;

	cout << pCalc->Add( 1, 2 ) << endl;
	cout << pCalc->Sub( 1, 2 ) << endl;
}