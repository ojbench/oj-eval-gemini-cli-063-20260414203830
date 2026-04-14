#include "MemoryRiver.hpp"
#include <iostream>
#include <cassert>

struct MyStruct {
    int a;
    double b;
    bool operator==(const MyStruct& other) const {
        return a == other.a && b == other.b;
    }
};

int main() {
    MemoryRiver<MyStruct, 2> mr("test.bin");
    mr.initialise();

    mr.write_info(42, 1);
    mr.write_info(100, 2);

    int info1, info2;
    mr.get_info(info1, 1);
    mr.get_info(info2, 2);
    assert(info1 == 42);
    assert(info2 == 100);

    MyStruct s1{1, 1.1};
    MyStruct s2{2, 2.2};
    MyStruct s3{3, 3.3};

    int idx1 = mr.write(s1);
    int idx2 = mr.write(s2);
    int idx3 = mr.write(s3);

    MyStruct r1, r2, r3;
    mr.read(r1, idx1);
    mr.read(r2, idx2);
    mr.read(r3, idx3);

    assert(r1 == s1);
    assert(r2 == s2);
    assert(r3 == s3);

    mr.Delete(idx2);

    MyStruct s4{4, 4.4};
    int idx4 = mr.write(s4);
    assert(idx4 == idx2); // Should reuse the space

    mr.read(r2, idx4);
    assert(r2 == s4);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}