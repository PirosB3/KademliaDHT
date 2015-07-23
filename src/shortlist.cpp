#include "shortlist.h"

using namespace std;

Shortlist::Shortlist(UID target, shared_ptr<Table> table, vector<shared_ptr<Node> > initial_shortlist) {
    this->target = target;
    this->atLeastOneWorked = true;
    this->addToFronteer(initial_shortlist);
}

void Shortlist::addToFronteer(vector<shared_ptr<Node> > nodes) {
    for(shared_ptr<Node> item : nodes) {
        UID xorr = item->uid ^ target;
        tuple<int, shared_ptr<Node> > pair = forward_as_tuple(xorr.distanceKey(), item);
        this->fronteer.push_back(pair);
    }
}

bool Shortlist::canContinue() {
    return (this->fronteer.size() < 20) && this->atLeastOneWorked;
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
    return result;
}
