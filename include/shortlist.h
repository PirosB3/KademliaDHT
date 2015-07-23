#include "table.h"
#include <set>

using namespace std;

class Shortlist {
public:
    Shortlist(UID current, UID target, Table* table, vector<shared_ptr<Node> > initial_shortlist);
    std::set<shared_ptr<Node>> seen;
    vector<tuple<int, shared_ptr<Node> > > fronteer;
    bool canContinue();
    vector<shared_ptr<Node> > getAlpha();
private:
    void addToFronteer(vector<shared_ptr<Node> > nodes);
    UID target;
    bool atLeastOneWorked;
};
