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


// r-value 는 오른쪽에 올 수 있는 상수를 의미
// = 등호에서 왼쪽, 오른쪽 모두에 올 수 있는 값은 l-value, 오른쪽에만 올 수 있는 값은 r-value
// ex) int a = 10;
// a 는 l-value 이고 10 은 r-value 이다 ( 10 = a 는 불가능 )
// const int a = 10; a 는 왼쪽에 올 수 없으니 r-value 라고 할 수 있는가? 
// 아니다.  immutable l-value 로 값을 변경할 수 없는 l-value 로 분류된다
// r-value 레퍼런스
// int& a = 10 << 레퍼런스는 r-value 를 참조할 수 없기때문에
// const 를 이용하게 되면 사용이 가능하다
// const int& a = 10 가능
// 지역 변수 레퍼런스 처리( Most Important const ) 에 따라 임시 객체의 수명이 const 의 수명으로 늘어나게 됨
// r-value 레퍼런스
// int&& a = 10;

// Reference Collapsing
// Universal reference l-value 레퍼런스 일 수도 r-value 레퍼런스 일수도있는 상태
// 즉 타입 추론에 의한 레퍼런스인 상태, 이 경우 Reference Collapsing 에 따라 l-value 레퍼런스, r-value 레퍼런스인지 결정된다
// 타입 추론은 auto 키워드, 템플릿 인자와 같이 타입을 명시하지 않는 케이스에서 타입을 추론하는 것을 의미한다
// l-value(&) l-value(&) => l-value(&)
// l-value(&) r-value(&&) => l-value(&)
// r-value(&&) l-value(&) => l-value(&)
// r-value(&&) r-value(&&) => r-value(&&)
// l-value 레퍼런스가 하나라도 있으면 l-value 레퍼런스

// Move Semantics 자원 복사가 아닌 이동
// move 생성자 혹은 move 대입 연산자
// class C
// C( C&& a ) 임시 객체로 초기화할때 호출
// std::move 함수 - r-value 로 캐스팅해서 리턴
// C a = static_cast<C&&>(b); == C a = std:move(b); // move 생성자 호출
// 클래스 C 에 move 생성자가 있다면 move 생성자 호출 없다면 복사 생성자 호출

// Forwarding Reference 전달 참조

// allocator 는 나중에

class C {
public:
    C() {}
    C( C&& a ) // move 생성자
        : m( a.m )
    {}
    C& operator =(C&& a) { // move 대입연산
        m = std::move(a.m);
        return *this;
    }

    int m = 0;
};

template< typename T>
void func(T&& f) {

}

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetBreakAlloc(217);

    test_JVector();
    test_JList();
    
    // Reference Collapsing
    int a = 0;
    typedef int& lr;    
    lr&& r1 = a;        // int& && r1 => int& r1 이 됨

    typedef int&& lr2;
    lr2&& r2 = 0;        // int&& && r2 => int&& r-value 레퍼런스가 되기 때문에 가능 ( 레퍼런스는 r-value 를 참조할수 없음 )

    // move semantics 자원 복사가 아닌 이동
    // move 생성자 혹은 move 대입 연산자
    // C( C&& a ) , C& operator =(C&& a)
    // 복사가 아닌 이동의 개념 ( 임시객체 복사가 발생하지 않도록 )

    
    // forwarding reference
    C b;
    func(b);    // l-value 로 넘겼기에 func(T&& f) 는 l-value 를 받음
    func(std::move(b)); // r-value 로 넘겼기에 func(T&& f) 는 r-value 를 받음
    /*
    template< typename T>
    void func(T&& f) {
        forward<T>(f) // r-value 일때만 r-value 로 캐스팅하는 키워드
    }
    */
    _CrtDumpMemoryLeaks();
}