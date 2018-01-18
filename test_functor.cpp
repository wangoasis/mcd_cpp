#include "functor.h"
#include "type_list.h"
#include <string>
#include <iostream>
#include <cassert>
using std::string;

std::size_t concatLength(const string& s1, const string& s2) {
    return s1.length() + s2.length();
}

struct concatLengthStruct {
    std::size_t operator() (const string& s1, const string& s2) {
        return s1.length() + s2.length();
    } 
};

class Dog {
public:
    void Eat() {
        std::cout << "Dog eat" << std::endl;
    }

    void Speak() {
        std::cout << "Dog speak" << std::endl;
    }
};

int Sum(int i, int j) {
    return i + j;
}

int main() {

    string s1("test"), s2(" functor");

    /*
        test for a struct with operator() overriden
    */
    concatLengthStruct s;
    // return type size_t, parameter: two string
    Mcd::Functor<std::size_t, TYPELIST_2(string, string)> cmd(s);
    assert(cmd(s1, s2) == 12);

    /*
        test for a function pointer
    */
    Mcd::Functor<std::size_t, TYPELIST_2(string, string)> f(concatLength); 
    assert(f(s1, s2) == 12);

    /*
        calling functor object with wrong number of parameter will cause compilation error
        assert(f(s1) == 12);
    */

    /*
        test for class member functions
    */
    Dog dog;
    Mcd::Functor<void> cmd1(&dog, &Dog::Eat), cmd2(&dog, &Dog::Speak);
    cmd1();
    cmd2();

    /*
        test Binder
    */
    Mcd::Functor<int, TYPELIST_2(int, int)> SumF(Sum);
    // bind the first element to 10
    Mcd::Functor<int, TYPELIST_1(int)> SumWithTen(Mcd::BindFirst(SumF, 10));
    assert(SumWithTen(15) == 25);
    assert(SumWithTen(20) == 30);
}