#include "shortlist.h"
#include "socket_manager.h"

using namespace std;

class DHT {
public:
    DHT(shared_ptr<Table> table, shared_ptr<Node> bootstrap);
    thread* run();
    shared_ptr<Table> table;
private:
    void _run();
    SocketManager<ZMQSocket>* socketManager;
    thread thread_id;
};
