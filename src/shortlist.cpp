#include "shortlist.h"

using namespace std;

Shortlist::Shortlist(UID target, shared_ptr<Table> table, vector<shared_ptr<Node> > initial_shortlist) {
    this->target = target;
    this->table = table;
    this->atLeastOneWorked = true;
    this->hasDoneAtLeastOneIteration = false;
    this->addToFronteer(initial_shortlist);
}

void Shortlist::addToFronteer(vector<shared_ptr<Node> > nodes) {
    for(shared_ptr<Node> item : nodes) {
        bool isIn = this->seen.find(item) != this->seen.end();
        bool isCurrentNode = this->table->getNode()->uid == item->uid;
        if (!isIn && !isCurrentNode) {
            UID xorr = item->uid ^ target;
            tuple<int, shared_ptr<Node> > pair = forward_as_tuple(xorr.distanceKey(), item);
            this->fronteer.push_back(pair);
            this->table->update(item);
        }
    }
}

bool Shortlist::canContinue() {
    return ((this->fronteer.size() < 20) && this->atLeastOneWorked) || !hasDoneAtLeastOneIteration;
}

vector<shared_ptr<Node> > Shortlist::getAlpha() {
    sort(this->fronteer.begin(), this->fronteer.end(), 
         [](const tuple<int, shared_ptr<Node>>& a,
            const tuple<int, shared_ptr<Node>>& b) -> bool
         {
            return get<0>(a) < get<0>(b);
         });

    bool atLeastOneWorked = false;
    vector<shared_ptr<Node> > result;
    for(tuple<int, shared_ptr<Node> >& pair : this->fronteer) {
        shared_ptr<Node> n = get<1>(pair);
        bool is_in = this->seen.find(n) != this->seen.end();
        if (!is_in) {
            seen.insert(n);
            result.push_back(n);
            atLeastOneWorked = true;
        }
    }
    this->atLeastOneWorked = atLeastOneWorked;
    this->hasDoneAtLeastOneIteration = true;
    return result;
}
