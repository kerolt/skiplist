#include <iostream>
#include "skip_list.h"

int main(int argc, char** argv) {
    SkipList<int, std::string> skip_list(6);

    skip_list.InsertElement(1, "I");
    skip_list.InsertElement(2, "Love");
    skip_list.InsertElement(3, "C++");
    skip_list.InsertElement(4, "And");
    skip_list.InsertElement(5, "Linux");
    skip_list.InsertElement(6, "Keep");
    skip_list.InsertElement(7, "Fighting!");

    std::cout << "skip_list size:" << skip_list.Size() << std::endl;

    skip_list.DumpFile();

    skip_list.SearchElement(9);
    skip_list.SearchElement(18);

    skip_list.PrintList();

    skip_list.DeleteElement(3);
    skip_list.DeleteElement(7);

    std::cout << "skip_list size:" << skip_list.Size() << std::endl;

    skip_list.PrintList();
}
