#include <mutex>
#include <memory>
#include "zmqpp/zmqpp.hpp"

using namespace std;

template<class ITEM>
class SingletonStruct {
public:
    SingletonStruct() {}
    static ITEM* getInstance();
private:
    static ITEM* instance;
    static mutex* m;
};

template<class ITEM>
ITEM* SingletonStruct<ITEM>::instance = nullptr;

template<class ITEM>
mutex* SingletonStruct<ITEM>::m = new mutex();

template<class ITEM>
ITEM* SingletonStruct<ITEM>::getInstance() {
    m->lock();
    if (instance == nullptr) {
        instance = new ITEM();
    }
    m->unlock();
    return instance;
}


//class SingletonStruct {
//public:
    //static zmqpp::context* getInstance();
    //SingletonStruct() {}
//private:
    //static zmqpp::context* instance;
//};

//zmqpp::context* SingletonStruct::instance = nullptr;

//zmqpp::context* SingletonStruct::getInstance() {
    //return instance;
//}

