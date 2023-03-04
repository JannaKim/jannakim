#include "C:\Users\admin\Documents\jannakim\IPCServerManager\ecourse_dp.hpp"
using namespace std;
using namespace ecourse;

void test_proxy_pattern();
int main()
{
	test_proxy_pattern();
}

void test_proxy_pattern()
{
	int server = ec_find_server( "CalcService" );

	cout << "server : " << server << endl;

	int ret = ec_send_server( server, 1, 10, 20 );

	cout << ret << endl;

	while ( 1 ) {
		
	}
}