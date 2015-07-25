#include <iostream>

#include "dht.h"

using namespace std;

int main() {
    shared_ptr<Node> node(new Node(makeUid('a'), "localhost", 3001));
    shared_ptr<Node> bst(new Node(makeUid('b'), "localhost", 3002));
    shared_ptr<Table> table(new Table(node));
    DHT core(table, bst);
    cout << "OK" << endl;
}
