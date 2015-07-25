#include "gtest/gtest.h"

#include "context.h"

struct test {
    int number;
};

TEST(SingletonStruct, makesSingletons) {
    ASSERT_EQ(
        SingletonStruct<test>::getInstance(),
        SingletonStruct<test>::getInstance()
    );

    SingletonStruct<test>::getInstance()->number = 22;
    ASSERT_EQ(
        SingletonStruct<test>::getInstance()->number,
        22
    );

}

