
// Nome do arquivo
// Data

// Licença e link para lincença

// Author: Nome <email>

/* Descrição:

 */

// ___________________________________________________________________


#include <iostream>
#include <vector>
#include <string>


#include "../../derailleur.hpp"

#include <unistd.h>


class MyApp : public derailleur::Application {

public:
     MyApp ( std::string app_name )
          : derailleur::Application ( app_name ) {}

     void on_switch_up ( const derailleur::Event* const event ) override {
          derailleur::Switch* s = get_switch_copy ( event->get_switch_id() );

          std::string log ( "on switch up event - MAC " );
          log += s->get_mac_address();

          derailleur::Log::Instance()->log ( "MyApp", log.c_str() );

          std::vector<int> v = get_switches_IDs();

          std::cout << "IDs: ";
          for ( int i : v )
               std::cout << i << " ";
          std::cout << std::endl;
     }

     void on_switch_down ( const int switch_id ) override {

     }

     void message_handler ( const derailleur::Event* const event ) override {

          switch ( event->get_type() ) {

          case 10: // packet-in

               derailleur::Log::Instance()->log ( "Test", "packet_in" );

               std::cout << derailleur::util::get_link_layer_protocol ( event )
                         << std::endl;

//                std::cout << "código arp: "
//                          << derailleur::util::Protocols.link_layer.arp
//                          << std::endl;

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
          sleep ( 1 );

          //std::cout << "stack size: " << controller.get_stack_size() << std::endl;
     }

     return 0;
}


