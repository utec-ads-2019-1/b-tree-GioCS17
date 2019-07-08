#include <iostream>
#include "btree.h"

using namespace std;

int main(int argc, char *argv[]) {
  //define numero de keys
    BTree t(5);

    t.insert(10);
    t.insert(1);
    t.insert(11);
    t.insert(7);
    t.insert(37);
    t.insert(12);
    t.insert(107);
    t.insert(207);
    t.insert(1007);
    t.insert(2007);
    t.insert(1027);
    t.insert(2047);
    t.print();

    t.insert(2);
    t.print();
    t.remove(207);
    t.remove(1007);
    t.remove(2007);
    t.remove(1027);
    t.remove(2047);
    t.print();
    return EXIT_SUCCESS;
}
