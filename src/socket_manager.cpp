#include "socket_manager.h"

using namespace std;

//DistroSocket::DistroSocket(shared_ptr<Node> node) {
    //this->rootNode = node;
//}

StringSocket::StringSocket(shared_ptr<Node> node) {
    this->rootNode = node;
}

string StringSocket::send(string key, string data) {
    this->dataSent = data;
    return "";
}
