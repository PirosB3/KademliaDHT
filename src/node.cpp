#include <iostream>
#include <list>
#include <cstdlib>
#include <cstring>
#include <sstream>

#include "node.h"
#include "openssl/sha.h"

void sha256(const char *string, char outputBuffer[65]) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, string, strlen(string));
    SHA256_Final(hash, &sha256);
    int i = 0;
    for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = 0;
}

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

std::array<unsigned int, 32> UID::fromDataString(std::string data) {
    std::array<unsigned int, 32> uidArray;
    std::istringstream stream(data);
    unsigned int byte;
    for(int i=0; i < 32; i++) {
        stream >> byte;
        uidArray[i] = byte;
    }
    return uidArray;
}

std::array<unsigned int, 32> UID::fromHash(std::string data) {
    char output[65];
    sha256(data.c_str(), output);

    std::array<unsigned int, 32> result;
    int counter = 0;

    for (int i=0; i < 64; i+=2) {
        std::stringstream ss;
        ss << std::hex << output[i] << output[i+1];
        unsigned int k;
        ss >> k;
        result[counter] = k;
        counter++;
    }
    return result;
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

Node::Node(std::array<unsigned int, 32> uid, std::string host, unsigned int port) {
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


std::shared_ptr<Node> Node::fromJson(json11::Json object) {
    // port and host
    int port = object["port"].int_value();
    std::string host = object["host"].string_value();

    std::array<unsigned int, 32> uidArray = UID::fromDataString(object["uid"].string_value());
    //std::istringstream stream();
    //auto uidArray = makeUid(0);
    //unsigned int byte;
    //for(int i=0; i < 32; i++) {
        //stream >> byte;
        //uidArray[i] = byte;
    //}
    return std::shared_ptr<Node>(new Node(
        uidArray,
        host,
        port
    ));
}
