
#include <fluid/OFServer.hh>

#include <iostream>
#include <unordered_map>
#include <list>

namespace controller {
    
class Controller : public fluid_base::OFServer
{
    public:

public:
    std::unordered_map<int, std::vector<Application*> > event_listeners;
    // We assume that after we start the controller no event listeners will be
    // added. This way, we don't need to lock this map.
    bool running;

    Controller(const char* address = "0.0.0.0",
               const int port = 6653,
               const int n_workers = 4,
               bool secure = false) : 
    OFServer(address, port, n_workers, secure, OFServerSettings().supported_version(1).
                        supported_version(4).keep_data_ownership(false)) {
        this->running = true;
    }

    void register_for_event(Application* app, int event_type) {

    }

    void stop() {
    }

    virtual void message_callback(OFConnection* ofconn, uint8_t type, void* data, size_t len) {
        
    }

    virtual void connection_callback(OFConnection* ofconn, OFConnection::Event type) {

    }


    void teste();
    
};
    
} // namespace controller
