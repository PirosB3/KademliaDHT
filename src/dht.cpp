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
                if (std::get<1>(result) == true) {
                    string err;
                    json11::Json::array res = json11::Json::parse(std::get<0>(result), err)["response"].array_items();

                    vector<shared_ptr<Node> > newNodes;
                    for(auto &obj : res) {
                        shared_ptr<Node> n = Node::fromJson(obj);
                        newNodes.push_back(n);
                    }
                    s.addToFronteer(newNodes);
                } else {
                    cout << "FAIL FIND_NODE" << endl;
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
        } else if (key == "STORE_VALUE") {
            string hashKey = item["data"]["hashKey"].string_value();
            string value = item["data"]["value"].string_value();
            this->data[hashKey] = value;

            zmqpp::message responseMessage;
            json11::Json response = json11::Json::object{
                {"response", "OK"}
            };
            responseMessage << response.dump();
            receiver.send(responseMessage);
        } else if (key == "FIND_VALUE") {
            json11::Json response;

            UID uid = UID::fromDataString(item["data"]["hashKey"].string_value());
            string hashKey = uid.getDataString();
            if (this->data.find(hashKey) != this->data.end()) {
                response = json11::Json::object{
                    {"response", json11::Json::object {
                        {"value", this->data[hashKey]}
                    }}
                };
            } else {
                vector<shared_ptr<Node> > result = this->table->findNearest(&uid);

                json11::Json::array toReturn;
                string err;
                for(shared_ptr<Node> n : result) {
                    toReturn.push_back(json11::Json::object {
                        {"host", n->host},
                        {"port", n->port},
                        {"uid", n->uid.getDataString()}
                    });
                }
                response = json11::Json::object{
                    {"response", toReturn}
                };
            }

            zmqpp::message responseMessage;
            responseMessage << response.dump();
            receiver.send(responseMessage);
        }
    }
}

tuple<string, bool> DHT::get(string key) {
    UID uid = UID::fromHash(key);
    string hashKey = uid.getDataString();
    if (this->data.find(hashKey) != this->data.end()) {
        return forward_as_tuple(this->data[hashKey], true);
    }

    // Value is not local, start iterative find value
    Shortlist s(
        uid,
        this->table, 
        this->table->findNearest(&uid)
    );
    while (s.canContinue()) {
        vector<shared_ptr<Node> > alphaNodes = s.getAlpha();
        for (shared_ptr<Node>& node : alphaNodes) {
            auto result = this->socketManager->getSocket(node)->send(
                "FIND_VALUE",
                json11::Json::object {
                    {"hashKey", uid.getDataString()}
                }
            );
            if (std::get<1>(result) == true) {
                string err;
                json11::Json res = json11::Json::parse(std::get<0>(result), err)["response"];
                if (res.is_object()) {
                    return forward_as_tuple(res["value"].string_value(), true);
                } else if (res.is_array()) {
                    vector<shared_ptr<Node> > newNodes;
                    for(auto &obj : res.array_items()) {
                        shared_ptr<Node> n = Node::fromJson(obj);
                        newNodes.push_back(n);
                    }
                    s.addToFronteer(newNodes);
                }
            } else {
                cout << "FAIL FIND_VALUE" << endl;
            }
        }
    }
    return forward_as_tuple("", false);
}

void DHT::set(string key, string value) {
    UID uid = UID::fromHash(key);
    string hashKey = uid.getDataString();
    auto results = this->table->findNearest(&uid);
    if (results.size() == 0) {
        this->data[hashKey] = value;
        return;
    }
    for(shared_ptr<Node> node : results) {
        auto result = this->socketManager->getSocket(node)->send(
            "STORE_VALUE",
            json11::Json::object {
                {"hashKey", hashKey},
                {"value", value}
            }
        );
    }
}
