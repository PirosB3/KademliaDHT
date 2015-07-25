#include <iostream>
#include <cassert>

#include "dht.h"

using namespace std;

int main(int argc, char* argv[]) {
    thread* anyThread = nullptr;
    int port = 3000;
    vector<shared_ptr<DHT>> dhts;

    for (int i=0; i < 50; i++) {
        shared_ptr<Node> bootstrapNode = nullptr;
        if (dhts.size() > 0) {
            int randomItem = rand() % dhts.size();
            bootstrapNode = dhts[randomItem]->table->getNode();
        }

        shared_ptr<Node> node(new Node(generateRandomUID(), "localhost", ++port));
        shared_ptr<Table> table(new Table(node));
        auto core = shared_ptr<DHT>(new DHT(table, bootstrapNode));
        thread* t = core->run();
        dhts.push_back(core);
        if (anyThread == nullptr) {
            anyThread = t;
        }
    }
    anyThread->join();
}
