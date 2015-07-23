#include "socket_manager.h"
#include <sstream>

using namespace std;

StringSocket::StringSocket(shared_ptr<Node> from, shared_ptr<Node> to, zmqpp::context_t* context) {
    this->rootNode = from;
}

string StringSocket::send(string key, json11::Json data) {
    json11::Json wrapper = json11::Json::object {
        {"sender", json11::Json::object {
            {"host", this->rootNode->host},
            {"port", this->rootNode->port},
        }},
        {"uid", this->rootNode->uid.getDataString()},
        {"key", key},
        {"data", data}
    };
    this->dataSent = wrapper.dump();
    return "";
}


ZMQSocket::ZMQSocket(shared_ptr<Node> from, shared_ptr<Node> to, zmqpp::context_t* context) {
    this->rootNode = from;
    this->node = to;
    this->context = context;
}

std::tuple<string, bool> ZMQSocket::send(string key, json11::Json data) {
    json11::Json wrapper = json11::Json::object {
        {"sender", json11::Json::object {
            {"host", this->rootNode->host},
            {"port", this->rootNode->port},
        }},
        {"uid", this->rootNode->uid.getDataString()},
        {"key", key},
        {"data", data}
    };

    // Create connect address
    ostringstream stream;
    stream << "tcp://" << string(this->node->host) << ":" << this->node->port;

    // Create socket and connect
    zmqpp::socket s(*this->context, zmqpp::socket_type::req);
    s.set(zmqpp::socket_option::linger, 0);
    s.connect(stream.str());


    // part 1: send REQ
    zmqpp::message request;
    request << wrapper.dump();
    s.send(request, true);

    // part 2: receive REP
    zmqpp::poller poller;
    poller.add(s, zmqpp::poller::poll_in);
    poller.poll(1000);

    if ((poller.events(s) & zmqpp::poller::poll_in) > 0) {
        string result;
        zmqpp::message response;
        s.receive(response);
        response >> result;
        return forward_as_tuple(result, true);
    }
    return forward_as_tuple("", false);
}
