#include "gtest/gtest.h"
#include "shortlist.h"
#include <memory>

using namespace std;

TEST(Shortlist, exists) {
    shared_ptr<Node> current(new Node(makeUid('z'), "localhost", 3002));
    UID target(makeUid('a'));

    vector<shared_ptr<Node> > nodes = {
        shared_ptr<Node>(new Node(makeUid('b'), "localhost", 3002)),
        shared_ptr<Node>(new Node(makeUid('c'), "localhost", 3003)),
        shared_ptr<Node>(new Node(makeUid('d'), "localhost", 3004)),
        shared_ptr<Node>(new Node(makeUid('e'), "localhost", 3005)),
        shared_ptr<Node>(new Node(makeUid('f'), "localhost", 3006)),
    };
    shared_ptr<Table> t(new Table(current));
    Shortlist s(target, t, nodes);
    ASSERT_EQ(s.seen.size(), 0);
    ASSERT_EQ(s.fronteer.size(), 5);
    ASSERT_EQ(s.canContinue(), true);

    ASSERT_EQ(s.getAlpha().size(), 5);
    ASSERT_EQ(s.seen.size(), 5);
    ASSERT_EQ(s.canContinue(), true);

    ASSERT_EQ(s.getAlpha().size(), 0);
    ASSERT_EQ(s.seen.size(), 5);
    ASSERT_EQ(s.canContinue(), false);
};
