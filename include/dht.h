#include "shortlist.h"
#include "socket_manager.h"

using namespace std;

class DHT {
public:
    DHT(shared_ptr<Table> table, shared_ptr<Node> bootstrap);
private:
    shared_ptr<Table> table;
    SocketManager<ZMQSocket>* socketManager;
};
