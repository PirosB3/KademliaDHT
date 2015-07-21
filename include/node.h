#include <array>
#include <list>

std::array<unsigned int, 32> generateRandomUID();
std::array<unsigned int, 32> makeUid(char c);

class UID {
public:
    UID();
    UID(std::array<unsigned int, 32> data);
    bool operator==(const UID& first) const;
    bool operator!=(const UID& first) const;
    const unsigned int& operator[](int x) const;
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
protected:
    unsigned int port;
    const char* host;
};
