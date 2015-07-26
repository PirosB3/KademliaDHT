#include "table.h"
#include <set>

using namespace std;

class Shortlist {
public:
    Shortlist(UID target, shared_ptr<Table> table, vector<shared_ptr<Node> > initial_shortlist);
    std::set<shared_ptr<Node>> seen;
    vector<tuple<int, shared_ptr<Node> > > fronteer;
    bool canContinue();
    vector<shared_ptr<Node> > getAlpha();
    void addToFronteer(vector<shared_ptr<Node> > nodes);
private:
    UID target;
    bool atLeastOneWorked;
    shared_ptr<Table> table;
};
