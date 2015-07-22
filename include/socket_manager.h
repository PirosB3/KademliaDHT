#include "node.h"
#include <memory>

#include "zmqpp/zmqpp.hpp"

using namespace std;

//class DistroSocket {
//public:
    //DistroSocket(shared_ptr<Node> node);
    //virtual string send(string data) = 0;
//};

//class StringSocket : public DistroSocket {
class StringSocket {
public:
    StringSocket(shared_ptr<Node> node);
    string dataSent;
    string send(string key, string data);
protected:
    shared_ptr<Node> rootNode;
};

template<class SOCKET_TYPE>
class SocketManager {
public:
    SocketManager(shared_ptr<Node> rootNode);
    shared_ptr<SOCKET_TYPE> getSocket(shared_ptr<Node> node);
private:
    shared_ptr<Node> rootNode;
};

template<class SOCKET_TYPE>
SocketManager<SOCKET_TYPE>::SocketManager(shared_ptr<Node> node) {
    this->rootNode = node;
}

template<class SOCKET_TYPE>
shared_ptr<SOCKET_TYPE> SocketManager<SOCKET_TYPE>::getSocket(shared_ptr<Node> node) {
    shared_ptr<SOCKET_TYPE> result(new SOCKET_TYPE(node));
    return result;
}
