#include "node.h"
#include <vector>
#include <memory>
#include <algorithm>

using namespace std;

class Table {
public:
    Table(std::shared_ptr<Node> node);
    shared_ptr<Node> getNode();
    void update(shared_ptr<Node> new_node);
    array<list<shared_ptr<Node> >, 256> buckets;
    vector<shared_ptr<Node> > findNearestFromBucket(int bucket, UID* source);
    vector<shared_ptr<Node> > findNearest(UID* source);
private:
    std::shared_ptr<Node> node;
};
