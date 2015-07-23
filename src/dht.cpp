#include "dht.h"

using namespace std;

DHT::DHT(shared_ptr<Table> table, shared_ptr<Node> bootstrap) {
    this->table = table;
    if (bootstrap != nullptr) {
        // Add node and run iterative find nodes
        this->table->update(bootstrap);
        UID uid = this->table->getNode()->uid;
        vector<shared_ptr<Node> > nearest = this->table->findNearest(&this->table->getNode()->uid);
        shared_ptr<Table> t = this->table;
        Shortlist s(
            uid,
            t, 
            nearest
        );
    }
}
