#include "gtest/gtest.h"
#include <array>
#include "json11.hpp"

#include "node.h"


TEST(Node, canGenerateUIDs) {
    std::array<unsigned int, 32> result = generateRandomUID();
    ASSERT_EQ(result.size(), 32);
}

TEST(Node, xor) {
    std::array<unsigned int, 32> first = makeUid('a');
    std::array<unsigned int, 32> second = makeUid('b');
    Node n1(first, "localhost", 3001);
    Node n2(second, "localhost", 3002);

    UID result = n1 ^ n2;
    for (int i=0; i < 32; i++) {
        ASSERT_EQ(result[i], 3);
    }
}

TEST(Node, equality) {
    std::array<unsigned int, 32> first = makeUid('a');
    std::array<unsigned int, 32> second = makeUid('b');
    Node n1(first, "localhost", 3001);
    Node n2(first, "localhost", 3002);
    Node n3(second, "localhost", 3002);
    ASSERT_EQ(n1,  n2);
    ASSERT_NE(n1,  n3);
}

TEST(UID, distanceKey) {
    std::array<unsigned int, 32> first = {
        0, 0, 0, 0, 0, 31, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255
    };
    UID n1(first);
    ASSERT_EQ(n1.distanceKey(),  (8 * 5) + 3);

}

TEST(Node, distanceKeyZeros) {
    std::array<unsigned int, 32> first = makeUid(0);
    Node n1(first, "localhost", 3001);
    ASSERT_EQ(n1.distanceKey(), 255);
}

TEST(Node, canDeserializeAndSerialize) {
    std::string err;
    auto data = json11::Json::parse("{\"data\": {\"node\": \"12345\"}, \"key\": \"FIND_NODE\", \"sender\": {\"host\": \"localhost\", \"port\": 3000, \"uid\": \"97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 97 \"}}", err);
    std::shared_ptr<Node> node = Node::fromJson(data["sender"]);
    ASSERT_EQ(node->host, "localhost");
    ASSERT_EQ(node->port, 3000);
    for(unsigned int &i : node->uid.getData()) {
        ASSERT_EQ(i, 97);
    }
}

TEST(UID, canBeGeneratedFromHash) {
    auto res = UID::fromHash("Hello World!");
    std::array<unsigned int, 32> expected = {
        127, 131, 177, 101, 127, 241, 252, 83, 185, 45, 193,
        129, 72, 161, 214, 93, 252, 45, 75, 31, 163, 214, 119,
        40, 74, 221, 210, 0, 18, 109, 144, 105
    };
    ASSERT_EQ(res, expected);
}
