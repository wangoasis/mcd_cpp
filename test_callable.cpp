#include <iostream> 
using namespace std;

void Foo() {
    cout << "Foo called" << endl;
}

void Bar(int i) {
    cout << "Bar called " << i << " " << endl;
}

int main() {
    // defines a pointer to function Foo, the return type is void, 
    // and it takes no parameters
    void (*pF)() = &Foo;

    // call directly
    Foo();
    Bar(5);

    // call via pointer
    (*pF)();

    // defines a pointer to function Bar, the return type is void, 
    // and it takes 1 parameter of int type
    void (*pB)(int) = &Bar;
    (*pB)(3);

    // the function name can be seen as the pointer to function
    void (*pB2)(int) = Bar;
    (*pB2)(3);
}