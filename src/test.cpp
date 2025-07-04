#include <iostream>
#include <functional>

int num = 10;

class Obj{
public:
    void print() { std::cout << "print in Obj " << this << std::endl; }
};


void print(){
    std::cout << "print!" << std::endl;
}

int main(void){
    Obj obj;
    std::cout << "전역 num: " << &num << std::endl;

    //std::function<void(char a)> a = [](char a) { std::cout << a << std::endl; };
    auto aFunc = [=]() { std::cout << &num << std::endl; };
    std::function<void(Obj&)> bFunc = &Obj::print;

    aFunc();
    bFunc(obj);
}