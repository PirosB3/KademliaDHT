#include <iostream>
#include <array>
#include <list>
#include <cstdlib>

std::array<unsigned int, 32> generateRandomUID() {
    std::array<unsigned int, 32> result;
    for (int i=0; i < 32; i++) {
        result[i] = rand() % 256;
    }
    return result;
}

class Node {
public:
    Node(std::array<unsigned int, 32> uid, const char* host, unsigned int port);
    std::array<unsigned int, 32> uid;
    friend std::array<unsigned int, 32> operator^(const Node& first, const Node& second);
    friend bool operator==(const Node& first, const Node& second);
    friend bool operator|=(const Node& first, const Node& second);
    std::array<std::list<Node>, 32> buckets;
    int distanceKey();
protected:
    unsigned int port;
    const char* host;
};


Node::Node(std::array<unsigned int, 32> uid, const char* host, unsigned int port) {
    this->uid = uid;
    this->host = host;
    this->port = port;
}

std::array<unsigned int, 32> operator^(const Node& first, const Node& second) {
    std::array<unsigned int, 32> result;
    for (int i=0; i < 32; i++) {
        result[i] = first.uid[i] ^ second.uid[i];
    }
    return result;
}

bool operator==(const Node& first, const Node& second) {
    return first.uid == second.uid;
}

bool operator!=(const Node& first, const Node& second) {
    return first.uid != second.uid;
}

int Node::distanceKey() {
    for (int i=0; i < 32; i++) {
        unsigned int current = this->uid[i];
        for (int j=7; j >= 0; j--) {
            if ((current & (1 << j)) != 0) {
                return i * 8 + (7-j);
            }
        }
    }
    return 8 * 31 + 8;
}
