#include "shortlist.h"

using namespace std;

class DHT {
public:
    DHT(shared_ptr<Table> table, shared_ptr<Node> bootstrap);
private:
    shared_ptr<Table> table;
};
