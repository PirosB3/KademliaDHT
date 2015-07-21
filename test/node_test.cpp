#include "gtest/gtest.h"
#include <array>

#include "node.h"

std::array<unsigned int, 32> makeUid(char c) {
    std::array<unsigned int, 32> result;
    for (int i=0; i < 32; i++) {
        result[i] = c;
    }
    return result;
}

TEST(Node, canGenerateUIDs) {
    std::array<unsigned int, 32> result = generateRandomUID();
    ASSERT_EQ(result.size(), 32);
}

TEST(Node, exists) {
    std::array<unsigned int, 32> result = generateRandomUID();
    Node n(result, "localhost", 3001);
    ASSERT_EQ(n.buckets.size(), 32);
}

TEST(Node, xor) {
    std::array<unsigned int, 32> first = makeUid('a');
    std::array<unsigned int, 32> second = makeUid('b');
    Node n1(first, "localhost", 3001);
    Node n2(second, "localhost", 3002);

    std::array<unsigned int, 32> result = n1 ^ n2;
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

TEST(Node, distanceKey) {
    std::array<unsigned int, 32> first = {
        0, 0, 0, 0, 0, 31, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255
    };
    Node n1(first, "localhost", 3001);
    ASSERT_EQ(n1.distanceKey(),  (8 * 5) + 3);

}

TEST(Node, distanceKeyZeros) {
    std::array<unsigned int, 32> first = makeUid(0);
    Node n1(first, "localhost", 3001);
    ASSERT_EQ(n1.distanceKey(), 256);
}
