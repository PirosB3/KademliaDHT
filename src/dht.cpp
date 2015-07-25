#include <chrono>
#include <sstream>
#include "dht.h"
#include "json11.hpp"

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
                    string err;
                    json11::Json::array res = json11::Json::parse(get<0>(result), err)["response"].array_items();
                    cout << "Found number of nodes: " << res.size() << endl;
                    for(auto &obj : res) {
                        shared_ptr<Node> n = Node::fromJson(obj);
                    }
                } else {
                    cout << "FAIL" << endl;
                }
            }
        }
    }
}

thread* DHT::run() {
    this->thread_id = thread(&DHT::_run, this);
    return &this->thread_id;
}

void DHT::_run() {
    ostringstream stream;
    stream << "tcp://*:" << this->table->getNode()->port;

    zmqpp::context* context = SingletonStruct<zmqpp::context_t>::getInstance();
    //zmqpp::context* context = SingletonStruct::getInstance();
    zmqpp::socket receiver(*context, zmqpp::socket_type::rep);
    receiver.bind(stream.str());

    while(true) {
        string err;
        string result;

        zmqpp::message response;
        receiver.receive(response);
        response >> result;

        auto item = json11::Json::parse(result, err);
        auto senderNode = Node::fromJson(item["sender"]);
        this->table->update(senderNode);

        auto key = item["key"].string_value();
        if (key == "FIND_NODE") {
            auto uidToSearch = UID(UID::fromDataString(item["data"]["node"].string_value()));
            vector<shared_ptr<Node> > result = this->table->findNearest(&uidToSearch);

            json11::Json::array toReturn;
            string err;
            for(shared_ptr<Node> n : result) {
                toReturn.push_back(json11::Json::object {
                    {"host", n->host},
                    {"port", n->port},
                    {"uid", n->uid.getDataString()}
                });
            }

            zmqpp::message responseMessage;
            json11::Json response = json11::Json::object{
                {"response", toReturn}
            };
            responseMessage << response.dump();

            // Send response message
            receiver.send(responseMessage);
        }
    }
}
