#include <array>
#include <list>
#include <iostream>
#include <memory>
#include "json11.hpp"

#pragma once
std::array<unsigned int, 32> generateRandomUID();
std::array<unsigned int, 32> makeUid(char c);

class UID {
public:
    UID();
    UID(std::array<unsigned int, 32> data);
    friend UID operator^(const UID& first, const UID& second);
    bool operator==(const UID& first) const;
    bool operator!=(const UID& first) const;
    std::ostream& operator<<(std::ostream &output);
    const unsigned int& operator[](int x) const;
    int distanceKey();
    std::array<unsigned int, 32> getData();
    std::string getDataString();
private:
    std::array<unsigned int, 32> data;
};

class Node {
public:
    static std::shared_ptr<Node> fromJson(json11::Json object);
    Node(std::array<unsigned int, 32> uid, std::string host, unsigned int port);
    UID uid;
    friend UID operator^(const Node& first, const Node& second);
    bool operator==(const Node& first) const;
    bool operator!=(const Node& first) const;
    int distanceKey();
    int port;
    std::string host;
protected:
};
