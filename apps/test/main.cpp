
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
          derailleur::Switch* s = get_switch_copy( event->get_switch_id() );
          
          
          
          std::cout << "on_switch_up: " << s->get_mac_address() << std::endl;
     }

     void on_switch_down ( const int switch_id ) override {

     }

     void message_handler ( const derailleur::Event* const event ) override {

          switch ( event->get_type() ) {

          case 10: // packet-in

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


