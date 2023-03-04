1. protected 소멸자

객체를 스택에 만들 수 없게 할때 사용하는 기법

힙에다가는 만들 수 있다
멤버함수를 통해서만 delete 가능

public:
 void Destroy() { delete this; }

 참조 계수 기반의 객체 수명 관리 기법에서 주로 사용된다


2. Upcast

Composite 패턴 관점에서 중요한 특성이다

파워포인트 그룹을 또 그룹으로 만들려면 기반 클래스가 있어야 한다

3. COUPLING

Camera
HDCamera

OCP 개발 폐쇄의 법칙

4. 프로토타입 패턴

가상함수 를 이용해
Clone 만드는 법 

기존의 객체 복사본을 만들때

5. 
mutex lock
do
unlock 
 : 템플릿 구조

  do 를 protected로 놓는다 



중급

decltype deduction 

() 안에 심볼 이름 외에 연산자 등을 포함한 표현식이 있는 경우

왼쪽에 올수 있으면 int& 아니면 intㄴ

decltype(n) 는 심볼 이외 다른 표현식 없으니까 그대로 int


auto 는 우변의 참조 속성을 제거한다


decltype(auto)
이거쓰면 참조도 가져오는듯

템플릿 메소드보다

strategy 가 더 좋다

중간에 ㅓㅈㅇ책 바꿀수있고
같은 정책 다른 클래스에서 쓰고싶을 수 있으니까

템플릿메소드는 
mutex lock
do
unlock 

do 재사용성이 필요없다

do 를 훅 이라고 함



menu event

생성자 상속:

자식이 디폴트 생성자쓰면 부모 생성자 파라미터로 만들수있음

class MenuItem
{
    int id;
public:
    MenuItem(std::string s, int n) : BaseMenu(n), id(n) {}
}

class AddStudentMenu : public MenuItem
{
public:
    using MenuItem::MenuItem;
};

AddStudentMenu m1( "Add Student ", 11);


방법 1 : 하는 일을 가상함수로 분리
단점 : 파생클래스의 개수가 너무 많아진다

virtual void command()
{
    doCommand(); <- 변해야하는 것만 가상함수로
}

방법 2 : strategy 패턴



state # 1 상태와 전략의 차이

State Pattern : 객체 자신의 내부 상태에 따라 행위(동작)을 변경하도록 한다
객체는 마치 클래스를 바꾸는 것 처럼 보인다

Strategy : 다양한 알고리즘이 존재하면 이들 각각을 하나의 클래스로 캡슐화 하여
알고리즘의 대체가 가능하도록 한다

클래스 다이어그램 상으론 완벽히 동일하게 그려진다

state는 동작을, strategy 는 알고리즘을 바꿀때 그렇게 불린다.


[State Pattern] summery
변하지 않은 코드에서 변해야 하는 부분은 분리 되어야 한다.


1. 일반 함우세어 변하는 건 함수 인자로 분리 : 함수 포인터, 함수 객체, 람다

2. 멤버 함수에서 변하는 건
템플릿 메소드 : 상속 기반, 재사용 불가, 실행 시간 교체 불가
 : 근데 바뀌는 거 여러가지면 상속해서 바뀌는 게 좋음

정책 클래스 교체
전략, 상태 : 인터페이스로, 실행시간이지만 가상함수라서 느림
Policy Base Design : 템플리 인자로, 실행시간 교체 불가능, 인라인 치환 가능, 빠름


[PolicyBase & Application Framwork] policy base


List<int> s; // 멀티 스레드에 안전하지 않다

template< typename T> class List
{
    ISync* pSync = 0;
public:
    void push_front(const T& a)
    {
        if ( pSync ) pSync->Lock();
        // ...
        if ( pSync ) pSync->Unlock(); // 괜히 부르면 성능이 저하된다 : 사용자가 쓸지 안쓸지 선택할 수 있게 한다
    }
}

전략패턴과 단위기반패턴 차이

단위 기반 패턴은 컴파일 시간 정책이라서 실행시간에 교체할 수 없고,
전략처럼 순수 가상함수가 있는 기반 클래스가 없어서
구현이 되어야할 걸 문서화 해놔야한다



menu event # 2

하나의 포인터에 멤버함수, 일반함수 모두 담기

void(*f1)() = &foo;

void(Dialog::*f2)() = &Dialog::Close;

클래스 템플릿을 생성하는 함수 템플릿을 만든다



menu event # 3

function<void()> f;
f = &foo;
f();

Dialog dlg;

f = bind(&Dialog::Close, &dlg);
f();

f = bind(&goo, 5);
f();

You can pass extra parameters to the function object returned from bind. They are silently ignored


menu event # 2

function 템플릿 : 

일반 함수, 멤버 함수, 람다 표현식, 함수 객체 등을 모두 담을 수 있다



[Adapter 패턴 개념]

Adapter 패턴 : 

한 클래스(또는 객체)의 인터페이스를 클라이언트가 사용하고자 하는 다른 인터페이스로 변환 한다

호환성 대문에 사용할 수 없었던 클래스들을 연결해서 사용할 수 있다


class Text : public TextView, public Shape
{
    public:
        Text( std::string s ) : TextView(s) {}

        virtual void Draw() { TextView::Show(); }
};


Text t("Hello");

vector<Shape*> v;

v.push_back(&t);

Textview tv("world")' // 이게 들어가려면?
v.push_back(&tv); // error

어댑터 종류: 클래스 어댑터, 객체 어댑터



[Adapter 패턴] STL Container Adapter

Sequence Container 의 인터페이스를 수정

ZArray = std::vector 가능하려면?



[Proxy 패턴] Proxy #1

Proxy 패턴을 사용한 IPC 계산기 예제 첫번째


어떤 객체에 대한 접근을 제어하기 위한 용도로
대리인이나 대변인에 해당하는 객체를 제공하는 패턴

