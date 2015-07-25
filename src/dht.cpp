#include "dht.h"

using namespace std;

DHT::DHT(shared_ptr<Table> table, shared_ptr<Node> bootstrap) {
    this->table = table;
    this->socketManager = new SocketManager<ZMQSocket>(this->table->getNode());
    if (bootstrap != nullptr) {
        // Add node and run iterative find nodes
        UID uid = this->table->getNode()->uid;
        this->table->update(bootstrap);

        Shortlist s(
            uid,
            this->table, 
            this->table->findNearest(&uid)
        );
        while (s.canContinue()) {
            vector<shared_ptr<Node> > alphaNodes = s.getAlpha();
            for (shared_ptr<Node>& node : alphaNodes) {
                auto result = this->socketManager->getSocket(node)->send(
                    "FIND_NODE",
                    json11::Json::object {
                        {"node", uid.getDataString()}
                    }
                );
                
                // check if success
                if (get<1>(result) == true) {
                    cout << "SUCCESS" << endl;
                } else {
                    cout << "FAIL" << endl;
                }
            }
        }
    }
}
