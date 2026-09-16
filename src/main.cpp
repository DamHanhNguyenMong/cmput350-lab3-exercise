#include <cstdlib>
#include <iostream>
#include "SharedPtr.h"

class Pirate {
    public:
        virtual void create() {
            std::cout << "Pirate created!" << std::endl;
        }
        virtual ~Pirate() {
            std::cout << "Pirate destroyed!" << std::endl;
        }
};

class StrawHatPirate : public Pirate {
    public:
        void create() override {
            std::cout << "Straw Hat Pirate created!" << std::endl;
        }
};

class Thingy {
    public:
        Thingy(int a, int b) : x(a), y(b) {
            std::cout << "x: " << x << ", y: " << y << std::endl;
        }
private:
        int x, y;
};

int main() {

    SharedPtr<int> ptr1;
    std::cout << "ptr1: " << ptr1.get() << std::endl;
    std::cout << "ptr1 useCount: " << ptr1.useCount() << '\n' << std::endl;
    if (!ptr1) {
        std::cout << "ptr1 is empty\n" << std::endl;
    }

    SharedPtr<int> ptr2(new int(10));
    std::cout << "ptr2: " << *ptr2 << '\n' << std::endl;
    std::cout << "ptr2 useCount: " << ptr2.useCount() << '\n' << std::endl;

    *ptr2 = 20;
    std::cout << "ptr2 after dereference: " << *ptr2 << '\n' << std::endl;

    std::cout << "ptr2 raw pointer: " << ptr2.get() << '\n' << std::endl;

    SharedPtr<int> ptr3(ptr2);
    std::cout << "ptr2 after copy: " << *ptr2 << '\n' << std::endl;
    std::cout << "ptr3 after copy: " << *ptr3 << '\n' << std::endl;
    std::cout << "ptr2 useCount after copy: " << ptr2.useCount() << '\n' << std::endl;
    std::cout << "ptr3 useCount after copy: " << ptr3.useCount() << '\n' << std::endl;

    SharedPtr<int> ptr4(std::move(ptr3));
    std::cout << "ptr3 after move: " << ptr3.get() << '\n' << std::endl;
    std::cout << "ptr4 after move: " << *ptr4 << '\n' << std::endl;
    std::cout << "ptr4 useCount after move: " << ptr4.useCount() << '\n' << std::endl;
    if (!ptr3) {
        std::cout << "ptr3 is empty after move\n" << std::endl;
    }

    SharedPtr<int> ptr5(new int(50));
    SharedPtr<int> ptr6;
    ptr6 = ptr5;
    std::cout << "ptr5 after copy assignment: " << *ptr5 << '\n' << std::endl;
    std::cout << "ptr6 after copy assignment: " << *ptr6 << '\n' << std::endl;
    std::cout << "ptr5 useCount after copy assignment: " << ptr5.useCount() << '\n' << std::endl;
    std::cout << "ptr6 useCount after copy assignment: " << ptr6.useCount() << '\n' << std::endl;

    SharedPtr<int> ptr7(new int(70));
    SharedPtr<int> ptr8;
    ptr8 = std::move(ptr7);
    std::cout << "ptr7 after move assignment: " << ptr7.get() << '\n' << std::endl;
    std::cout << "ptr8 after move assignment: " << *ptr8 << '\n' << std::endl;
    std::cout << "ptr8 useCount after move assignment: " << ptr8.useCount() << '\n' << std::endl;
    if (!ptr7) {
        std::cout << "ptr7 is empty after move assignment\n" << std::endl;
    }

    SharedPtr<int> ptr9(new int(90));
    std::cout << "ptr9 before reset: " << *ptr9 << '\n' << std::endl;
    ptr9.reset();
    std::cout << "ptr9 after reset: " << ptr9.get() << '\n' << std::endl;
    if (!ptr9) {
        std::cout << "ptr9 is empty after reset\n" << std::endl;
    }

    SharedPtr<int> ptr10(new int(100));
    std::cout << "ptr10 before reset: " << *ptr10 << '\n' << std::endl;
    ptr10.reset(new int(110));
    std::cout << "ptr10 after reset: " << *ptr10 << '\n' << std::endl;

    SharedPtr<int> ptr11(new int(110));
    SharedPtr<int> ptr12(new int(120));
    std::cout << "ptr11 before swap: " << *ptr11 << '\n' << std::endl;
    std::cout << "ptr12 before swap: " << *ptr12 << '\n' << std::endl;
    ptr11.swap(ptr12);
    std::cout << "ptr11 after swap: " << *ptr11 << '\n' << std::endl;
    std::cout << "ptr12 after swap: " << *ptr12 << '\n' << std::endl;

    SharedPtr<int> ptr13(new int(100));
    SharedPtr<int> ptr14(ptr13);
    if (ptr13 == ptr14) {
        std::cout << "ptr13 and ptr14 point to the same raw pointer\n" << std::endl;
    } else {
        std::cout << "ptr13 and ptr14 point to different raw pointers\n" << std::endl;
    }

    SharedPtr<int> ptr15(new int(100));
    if (ptr13 == ptr15) {
        std::cout << "ptr13 and ptr15 point to the same raw pointer\n" << std::endl;
    } else {
        std::cout << "ptr13 and ptr15 point to different raw pointers\n" << std::endl;
    }

    SharedPtr<Pirate> piratePtr(new StrawHatPirate());
    piratePtr->create();

    SharedPtr<Pirate> piratePtr2(new StrawHatPirate());
    (*piratePtr2).create();

    SharedPtr<Pirate> piratePtr3(new StrawHatPirate());
    SharedPtr<Pirate> piratePtr4(piratePtr3);
    std::cout << "piratePtr3 useCount: " << piratePtr3.useCount() << '\n' << std::endl;
    std::cout << "piratePtr4 useCount: " << piratePtr4.useCount() << '\n' << std::endl;

    SharedPtr<int> ptr16 = makeSharedBasic<int>(120);
    std::cout << "ptr16 after makeSharedBasic: " << *ptr16 << '\n' << std::endl;
    std::cout << "ptr16 useCount: " << ptr16.useCount() << '\n' << std::endl;

    SharedPtr<Thingy> thingyPtr = makeSharedBasic<Thingy>(1, 2);
    std::cout << "thingyPtr after makeSharedBasic: " << thingyPtr.get() << '\n' << std::endl;
    std::cout << "thingyPtr useCount: " << thingyPtr.useCount() << '\n' << std::endl;

    return EXIT_SUCCESS;
}
