#ifndef __CONTROLLER_HH__ 
#define __CONTROLLER_HH__

#include <fluid/OFServer.hh>

#include <iostream> 
//#include <unordered_map> 
//#include <list>

namespace derailleur {
    
class Controller : public fluid_base::OFServer {

    bool running;
    
public:

    Controller(const char* address = "0.0.0.0", const int port = 6653,
			   const int n_workers = 4, bool secure =
			   false) : fluid_base::OFServer(address,
			   port, n_workers, secure,
			   fluid_base::OFServerSettings().supported_version(1).
			   supported_version(4).keep_data_ownership(false))
			   { this->running = true; }

    // void register_for_event(fluid_base::Application* app, int
    // event_type) {
//
    //  }

    void stop() { this->running = false; fluid_base::OFServer::stop();
        }


    virtual void message_callback(fluid_base::OFConnection* ofconn,
				  uint8_t type, void* data, size_t
				  len) { std::cout << "message" <<
				  std::endl; }

    virtual void connection_callback(fluid_base::OFConnection* ofconn,
				     fluid_base::OFConnection::Event
				     type) {

    }


    void teste();
    
};
    
} // namespace derailleur

#endif
