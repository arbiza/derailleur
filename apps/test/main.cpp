
// Nome do arquivo
// Data

// Licença e link para lincença

// Author: Nome <email>

/* Descrição:

 */

// ___________________________________________________________________


#include <iostream>

#include <string>


#include "../../derailleur.hpp"

#include <unistd.h>


class MyApp : public derailleur::Application {

public:
     MyApp ( std::string app_name )
          : derailleur::Application ( app_name ) {}

     void on_switch_up ( const derailleur::Event* const event ) override {
          const derailleur::Switch* s = get_switch( event->get_switch_id() );

     }

     void on_switch_down ( const int switch_id ) override {

     }

     void message_handler ( const derailleur::Event* const event ) override {

          switch ( event->get_type() ) {

          case 10: // packet-in

//                uint64_t dst = 0, src = 0;
//                void* ofpip;
//                uint8_t* data;
//                uint16_t in_port;
//
//                if ( event->get_version() == fluid_msg::of10::OFP_VERSION ) {
//
//                } else if ( event->get_version() == fluid_msg::of13::OFP_VERSION ) {
//
//                     fluid_msg::of13::PacketIn *packet_in =
//                          new fluid_msg::of13::PacketIn();
//
//                     packet_in->unpack ( event->get_data() );
//                }
//                break;
               derailleur::Log::Instance()->log ( "Test", "packet_in" );
               break;

          default:
               derailleur::Log::Instance()->log ( "Test", "unknown" );
               break;
          }

     }
};


int main ( int argc, char *argv[] )
{

     MyApp myapp ( "Test app" );
     derailleur::Controller controller ( "0.0.0.0", 6653, 4, false, &myapp,
                                         "log.txt" );

     controller.start();


     while ( 1 ) {
          sleep(1);
          std::cout << "stack size: " << controller.get_stack_size() << std::endl;
     }

     return 0;
}


