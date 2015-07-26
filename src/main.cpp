#include <iostream>
#include <cassert>
#include <chrono>
#include <thread>

#include "dht.h"

using namespace std;

int main(int argc, char* argv[]) {
    thread* anyThread = nullptr;
    int port = 3000;
    vector<shared_ptr<DHT>> dhts;

    for (int i=0; i < 20; i++) {
        shared_ptr<Node> bootstrapNode = nullptr;
        if (dhts.size() > 0) {
            int randomItem = rand() % dhts.size();
            bootstrapNode = dhts[randomItem]->table->getNode();
        }

        shared_ptr<Node> node(new Node(generateRandomUID(), "localhost", ++port));
        shared_ptr<Table> table(new Table(node));
        auto core = shared_ptr<DHT>(new DHT(table, bootstrapNode));
        cout << "Spinning up " << core->table->getNode()->uid.getDataString() << " with bootstrap " << bootstrapNode << endl;
        thread* t = core->run();
        cout << "Spinned up: " << core->table->getNode()->uid.getDataString() << endl;
        dhts.push_back(core);
        if (anyThread == nullptr) {
            anyThread = t;
        }
    }

    this_thread::sleep_for(chrono::seconds(2));
    int randomItem = rand() % dhts.size();
    dhts[randomItem]->set("Hello", "World");
    this_thread::sleep_for(chrono::seconds(2));
    cout << "NOW" << endl;
    cout << get<0>(dhts[randomItem]->get("Hello")) << endl;
    anyThread->join();
}
