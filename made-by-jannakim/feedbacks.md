1. auto 쓰지 않기 

2. typedef 쓰지 않기
 : typename 키워드는 두 가지 의미가 있는 키워드

2. allocator 나중에 넣기 
 -> 이 후에 스마트 포인터 만들어서 연결하는 것 생각해서 우선 new, delete로 바꾸기
 + 뉴 연산자 오버라이딩해서 로그찍을 수도 있다 (댕글링을 만든게 있는지)
 할당기는 너무 다 가려져있고 복잡

3. && 함수들 구현. 키워드 :
rvalue
Immutable lvalue
Reference Collapsing
Move Semantics
Forwarding Reference

4. 스마트 포인터, 윅 포인터
 리스트에서 New 랑 delete 로 메모리 관리하는 와중에 스마트 포인터를 받게 되면 메모리 관리는 어떻게 될것인가 
 Safe delete ( 무의식적으로 쓰고 있을 것 )

5. 스레드 풀링
6. 메모리 풀 
7. 서비스 되고 있는 수준의 코드를 만드는 공부