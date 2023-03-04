#include "..\IPCServerManager\ecourse_dp.hpp"
#include "ICalc.h"
#include "pch.h"
using namespace ecourse;

// �� Ŭ������ �������� ipc ������ ����Ѵ�
class Calc : public ICalc
{
	int server;
public:
	Calc() { server = ec_find_server( "CalcService" ); }

	int Add( int a, int b ) { return ec_send_server( server, 1, a, b ); }
	int Sub( int a, int b ) { return ec_send_server( server, 2, a, b ); }
};

// �� ���� ����� �����ߴٰ� Calc ��ü�� ��������� �� �ƴ�
// Calc ������� �ݵ�� New ����ϴµ� �̷��� ����� ������ ������ OCP�� ������ �� ����

// Client �� ���� ��� ������ ��ӵ� �Լ��� �����
// Client�� ���� ��⿡ �ִ� Ŭ����(Proxy) �� �̸��� �˼� ���,
// ���� ��� �ȿ� �ִ� Ŭ����(Proxy) �� ��ü�� �����ϱ� ���� ��ӵ� �Լ��� �ʿ��ϴ�

// �Լ� �̸� �ٲ�� �� ����
extern "C" __declspec( dllexport )
ICalc * CreateCalc() // CreateCalc �� �ȹٲ�� �ǰ� Calc �� Ŭ���� �̸��� ������� �ٲ㵵 �ȴ�
{
	return new Calc;
	// ���� ��⿡�� ������ ��ü�� ���� ��⿡�� �ı��ϴ� ���� ����
}