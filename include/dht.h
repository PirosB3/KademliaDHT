#include "shortlist.h"
#include "socket_manager.h"

using namespace std;

class DHT {
public:
    DHT(shared_ptr<Table> table, shared_ptr<Node> bootstrap);
    thread* run();
    void set(string key, string value);
    string get(string key);
    shared_ptr<Table> table;
private:
    void _run();
    SocketManager<ZMQSocket>* socketManager;
    thread thread_id;
    unordered_map<string, string> data;
};
