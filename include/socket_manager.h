#include "node.h"
#include <memory>

#include "zmqpp/zmqpp.hpp"
#include "json11.hpp"
#include "context.h"

using namespace std;

class ZMQSocket {
public:
    ZMQSocket(shared_ptr<Node> from, shared_ptr<Node> to, zmqpp::context_t* context);
    tuple<string, bool> send(string key, json11::Json data);
protected:
    shared_ptr<Node> rootNode;
    shared_ptr<Node> node;
    zmqpp::context_t* context;
};

class StringSocket {
public:
    StringSocket(shared_ptr<Node> from, shared_ptr<Node> to, zmqpp::context_t* context);
    string dataSent;
    string send(string key, json11::Json data);
protected:
    shared_ptr<Node> rootNode;
};

template<class SOCKET_TYPE>
class SocketManager {
public:
    SocketManager(shared_ptr<Node> node);
    shared_ptr<SOCKET_TYPE> getSocket(shared_ptr<Node> node);
    zmqpp::context_t* context;
private:
    shared_ptr<Node> rootNode;
};

template<class SOCKET_TYPE>
SocketManager<SOCKET_TYPE>::SocketManager(shared_ptr<Node> node) {
    this->rootNode = node;
    this->context = SingletonStruct<zmqpp::context_t>::getInstance();
}

template<class SOCKET_TYPE>
shared_ptr<SOCKET_TYPE> SocketManager<SOCKET_TYPE>::getSocket(shared_ptr<Node> node) {
    shared_ptr<SOCKET_TYPE> result(new SOCKET_TYPE(this->rootNode, node, this->context));
    return result;
}
