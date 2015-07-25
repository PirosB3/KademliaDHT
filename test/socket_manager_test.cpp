#include "gtest/gtest.h"

#include "socket_manager.h"

using namespace std;

TEST(SocketManager, sendsMessages) {
    shared_ptr<Node> n1(new Node(makeUid('a'), "localhost", 3000));
    SocketManager<StringSocket> sm(n1);

    shared_ptr<Node> n2(new Node(makeUid('b'), "localhost", 3000));
    shared_ptr<StringSocket> ds = sm.getSocket(n2);

    json11::Json payload = json11::Json::object {
        { "node", "12345" },
    };
    ds->send("FIND_NODE", payload);
    string err;
    ASSERT_EQ(
         json11::Json::parse(ds->dataSent, err),
         json11::Json::parse("{\"data\": {\"node\": \"12345\"}, \"key\": \"FIND_NODE\", \"sender\": {\"host\": \"localhost\", \"port\": 3000, \"uid\": \"97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 \"}}", err)
    );
}

TEST(SocketManager, sendsMessages2) {
    shared_ptr<Node> n1(new Node(makeUid('a'), "localhost", 3000));
    SocketManager<ZMQSocket> sm(n1);

    shared_ptr<Node> n2(new Node(makeUid('b'), "localhost", 3001));
    shared_ptr<ZMQSocket> ds = sm.getSocket(n2);

    json11::Json payload = json11::Json::object {
        { "node", "12345" },
    };
    tuple<string, bool> result = ds->send("FIND_NODE", payload);
    ASSERT_EQ(get<1>(result), false);
}
