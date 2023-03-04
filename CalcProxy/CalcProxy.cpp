#include "..\IPCServerManager\ecourse_dp.hpp"
#include "ICalc.h"
#include "pch.h"
using namespace ecourse;

// 이 클래스는 원격지의 ipc 서버를 대신한다
class Calc : public ICalc
{
	int server;
public:
	Calc() { server = ec_find_server( "CalcService" ); }

	int Add( int a, int b ) { return ec_send_server( server, 1, a, b ); }
	int Sub( int a, int b ) { return ec_send_server( server, 2, a, b ); }
};

// 이 동적 모듈을 배포했다고 Calc 객체가 만들어지는 건 아님
// Calc 만들려면 반드시 New 써야하는데 이렇게 만들면 느슨한 결합인 OCP를 만족할 수 없다

// Client 와 동적 모듈 사이의 약속된 함수를 만든다
// Client는 동적 모듈에 있는 클래스(Proxy) 의 이름을 알수 없어서,
// 동적 모듈 안에 있는 클래스(Proxy) 의 객체를 생성하기 위해 약속된 함수가 필요하다

// 함수 이름 바뀌는 거 막고
extern "C" __declspec( dllexport )
ICalc * CreateCalc() // CreateCalc 만 안바뀌면 되고 Calc 등 클래스 이름은 마음대로 바꿔도 된다
{
	return new Calc;
	// 동적 모듈에서 생성한 객체는 동적 모듈에서 파괴하는 것이 좋다
}