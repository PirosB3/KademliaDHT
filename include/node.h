#include <array>
#include <list>

std::array<unsigned int, 32> generateRandomUID();

class Node {
public:
    Node(std::array<unsigned int, 32> uid, const char* host, unsigned int port);
    std::array<unsigned int, 32> uid;
    friend std::array<unsigned int, 32> operator^(const Node& first, const Node& second);
    bool operator==(const Node& first) const;
    bool operator!=(const Node& first) const;
    std::array<std::list<Node>, 32> buckets;
    int distanceKey();
protected:
    unsigned int port;
    const char* host;
};
