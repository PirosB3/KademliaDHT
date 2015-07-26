#include <iostream>
#include <cassert>
#include <chrono>
#include <thread>
#include <fstream>

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
        cout << "Spinned up: " << core->table->getNode()->uid.getDataString() << endl;
        dhts.push_back(core);
        if (anyThread == nullptr) {
            anyThread = t;
        }
    }

    this_thread::sleep_for(chrono::seconds(2));
    ifstream stream("/Users/danielpiros/DHT/lipsum.txt");
    string word;
    vector<string> wordVec;

    while(stream >> word) {
        int randomItem = rand() % dhts.size();
        dhts[randomItem]->set(word, word);
        cout << "Wrote " << word << endl;
        wordVec.push_back(word);
    }

    this_thread::sleep_for(chrono::seconds(2));
    for (string &word : wordVec) {
        int randomItem = rand() % dhts.size();
        shared_ptr<DHT> randomDht = dhts[randomItem];
        auto result = randomDht->get(word);
        assert(get<1>(result) == true);
        cout << word << " == " << get<0>(result) << endl;
    }
    anyThread->join();
}
