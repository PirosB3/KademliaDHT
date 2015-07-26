#include "gtest/gtest.h"
#include <array>
#include <vector>
#include <memory>

#include "table.h"

TEST(TableTest, exists) {
    std::shared_ptr<Node> node(new Node(generateRandomUID(), "localhost", 3001));
    Table t(node);
    ASSERT_EQ(t.getNode(), node);
};

TEST(TableTest, updateNode) {
    std::shared_ptr<Node> n1(new Node(makeUid('a'), "localhost", 3001));
    std::shared_ptr<Node> n2(new Node(makeUid('b'), "localhost", 3002));
    std::shared_ptr<Node> n3(new Node(n2->uid.getData(), "localhost", 3003));
    Table t(n1);

    // Increases bucket 6 by 1
    t.update(n2);
    ASSERT_EQ(t.buckets[6].size(), 1);

    // Value already exists
    t.update(n2);
    t.update(n3);
    ASSERT_EQ(t.buckets[6].size(), 1);

    // Adding itself is illegal
    t.update(n1);
    ASSERT_EQ(t.buckets[255].size(), 0);
};

TEST(TableTest, findNearest) {
    std::shared_ptr<Node> n1(new Node(makeUid('a'), "localhost", 3001));
    Table t(n1);

    std::shared_ptr<Node> nodes[] = {
        std::shared_ptr<Node>(new Node(makeUid('a'), "localhost", 3001)),
        std::shared_ptr<Node>(new Node(makeUid('b'), "localhost", 3002)),
        std::shared_ptr<Node>(new Node(makeUid('c'), "localhost", 3003)),
        std::shared_ptr<Node>(new Node(makeUid('d'), "localhost", 3004)),
        std::shared_ptr<Node>(new Node(makeUid('e'), "localhost", 3005)),
        std::shared_ptr<Node>(new Node(makeUid('f'), "localhost", 3006)),
    };

    t.buckets[4].push_back(nodes[0]);
    t.buckets[4].push_back(nodes[1]);
    t.buckets[3].push_back(nodes[2]);
    t.buckets[5].push_back(nodes[3]);
    t.buckets[5].push_back(nodes[4]);
    t.buckets[10].push_back(nodes[5]);

    UID uid(generateRandomUID());
    std::vector<std::shared_ptr<Node> > result = t.findNearestFromBucket(4, &uid);
    ASSERT_EQ(result.size(), 6);
    for (int i=0; i < 6; i++) {
        ASSERT_EQ(*result[i], *nodes[i]);
    }
}
