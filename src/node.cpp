#include <iostream>
#include <list>
#include <cstdlib>
#include <sstream>

#include "node.h"

std::array<unsigned int, 32> generateRandomUID() {
    std::array<unsigned int, 32> result;
    for (int i=0; i < 32; i++) {
        result[i] = rand() % 256;
    }
    return result;
}

std::array<unsigned int, 32> makeUid(char c) {
    std::array<unsigned int, 32> result;
    for (int i=0; i < 32; i++) {
        result[i] = c;
    }
    return result;
}

UID::UID() {
}

UID::UID(std::array<unsigned int, 32> data) {
    this->data = data;
}

bool UID::operator==(const UID& first) const {
    return this->data == first.data;
}

bool UID::operator!=(const UID& first) const {
    return this->data != first.data;
}

const unsigned int& UID::operator[](int x) const {
    return this->data[x];
}

Node::Node(std::array<unsigned int, 32> uid, const char* host, unsigned int port) {
    this->uid = UID(uid);
    this->host = host;
    this->port = port;
}

int Node::distanceKey() {
    return this->uid.distanceKey();
}

UID operator^(const UID& first, const UID& second) {
    std::array<unsigned int, 32> result;
    for (int i=0; i < 32; i++) {
        result[i] = first.data[i] ^ second.data[i];
    }
    return result;
}

UID operator^(const Node& first, const Node& second) {
    return first.uid ^ second.uid;
}

std::ostream& UID::operator<<(std::ostream &output) {
    return output;
}

std::string UID::getDataString() {
    std::stringstream s;
    for (unsigned int b : this->data) {
        s << b << " ";
    }
    return s.str();
}

std::array<unsigned int, 32> UID::getData() {
    return this->data;
}

int UID::distanceKey() {
    for (int i=0; i < 32; i++) {
        unsigned int current = this->data[i];
        for (int j=7; j >= 0; j--) {
            if ((current & (1 << j)) != 0) {
                return i * 8 + (7-j);
            }
        }
    }
    return 255;
}

bool Node::operator==(const Node& first) const {
    return this->uid == first.uid;
}

bool Node::operator!=(const Node& first) const {
    return this->uid != first.uid;
}
