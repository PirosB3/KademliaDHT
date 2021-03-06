#include "table.h"
#include <array>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


Table::Table(std::shared_ptr<Node> node) {
    this->node = node;
}

shared_ptr<Node> Table::getNode() {
    return this->node;
}

vector<shared_ptr<Node> > Table::findNearestFromBucket(int startingBucket, UID* source) {
    vector<shared_ptr<Node> > results;
    for (int i=0; i < 256; i++) {
        bool finishedLeft = false;
        bool finishedRight = false;

        int idxRight = startingBucket + i;
        int idxLeft = startingBucket - i;
        finishedLeft = idxLeft < 0;
        if (!finishedLeft) {
            for (shared_ptr<Node> node : this->buckets[idxLeft]) {
                results.push_back(node);
            }
        }
        finishedRight = idxRight >= this->buckets.size();
        if (!finishedRight && (i > 0)) {
            for (shared_ptr<Node> node : this->buckets[idxRight]) {
                results.push_back(node);
            }
        }
        if ((finishedRight && finishedLeft) || (results.size() >= 20)) {
            break;
        }
    }
    while (results.size() > 20) {
        results.pop_back();
    }
    return results;
}

void Table::update(shared_ptr<Node> new_node) {
    if (this->node == new_node) {
        return;
    }

    UID res = *this->node ^ *new_node;
    int bucketN = res.distanceKey();

    // Check if value is already there. if it isn't, add it
    for(shared_ptr<Node> n : this->buckets[bucketN]) {
        if (n->uid == new_node->uid) {
            return;
        }
    }
    this->buckets[bucketN].push_back(new_node);
}

vector<shared_ptr<Node> > Table::findNearest(UID* source) {
    int bucketN = (this->node->uid ^ *source).distanceKey();
    return this->findNearestFromBucket(bucketN, source);
}
