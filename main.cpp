#include <iostream>
#include "Collection.h"

#include <algorithm>

int main() {
    struct foo {
        int bar;
    };

    Collection<foo> myCollection;

    // "array" with 10 elements
    for (int i = 0; i < 10; i++) {
        foo *bar = new foo();
        bar->bar = i;
        myCollection.push(bar);
    }

    Collection<foo> myOtherCollection;
    // "array" with 10 elements
    for (int i = 10; i < 20; i++) {
        foo *bar = new foo();
        bar->bar = i;
        myOtherCollection.push(bar);
    }

    Collection<foo> myCombinedCollection = myCollection.concat(myOtherCollection);

    myCombinedCollection.forEach([](foo a){
        std::cout << a.bar << ",";
    });
    std::cout << std::endl;


    // array with 7,8,9
    auto splicedCollection = myCollection.splice(-3);

    // print those (prints 7,8,9)
    splicedCollection.forEach([](foo &a) {
        std::cout << a.bar << ",";
    });
    std::cout << std::endl;

    // print those left on original array, (prints 0,1,2,3,4,5,6)
    myCollection.forEach([](foo &a) {
        std::cout << a.bar << ",";
    });
    std::cout << std::endl;

    // filter out the odd ones
    auto everyTest = myOtherCollection.filter([](foo &a) -> bool{
        return a.bar % 2;
    }).every([](foo &b)->bool{
        return b.bar % 2;
    });

    std::cout << everyTest << "=O" << std::endl;

    Collection<double> floated = myOtherCollection.map<double>([](foo &a){
        return (double) a.bar;
    });


    int mult = 0;
    const auto total = myCollection.reduce<double>([&](double total, foo &a) {
        ++mult;
        return total + a.bar * mult;
    }, 0);

    std::cout << total;
    return 0;
}
