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