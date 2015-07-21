#include <iostream>
#include <list>
#include <cstdlib>

#include "node.h"

std::array<unsigned int, 32> generateRandomUID() {
    std::array<unsigned int, 32> result;
    for (int i=0; i < 32; i++) {
        result[i] = rand() % 256;
    }
    return result;
}

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

bool Node::operator==(const Node& first) const {
    return this->uid == first.uid;
}

bool Node::operator!=(const Node& first) const {
    return this->uid != first.uid;
}
