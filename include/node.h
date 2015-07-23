#include <array>
#include <list>
#include <iostream>

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
    Node(std::array<unsigned int, 32> uid, const char* host, unsigned int port);
    UID uid;
    friend UID operator^(const Node& first, const Node& second);
    bool operator==(const Node& first) const;
    bool operator!=(const Node& first) const;
    int distanceKey();
    int port;
    const char* host;
protected:
};
