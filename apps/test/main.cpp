
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
          derailleur::Switch* s = get_switch ( event->get_switch_id() );

          std::cout << "FEATURES REPLY:"
                    << "\nDatapath id: " <<  s->get_datapath_id()
                    << "\nn buffers: " << s->get_n_buffers()
                    << "\nn tables: " << s->get_n_tables()
//                     << "\nauxiliary id: " << s->get_auxiliary_id()
//                     << "\ncapabilities: " << s->get_capabilities()
                    << "\n\nDESCRIPTION:"
                    << "\nManufacturer: " << s->get_manufacturer()
                    << "\nHardware: " << s->get_hardware()
                    << "\nSoftware: " << s->get_software()
                    << "\nSerial: " << s->get_serial_number()
                    << "\nDatapath: " << s->get_datapath()
                    << std::endl;

          std::cout << "MAC: " << s->get_mac_address() << std::endl;
     }

     void on_switch_down ( const int switch_id ) override {

     }

     void message_handler ( const derailleur::Event* const event ) override {

     }
};


int main ( int argc, char *argv[] )
{

     MyApp myapp ( "Test app" );
     derailleur::Controller controller ( "0.0.0.0", 6653, 4, false, &myapp );

     controller.start();


     while ( 1 ) {
          std::cout << "size: " << controller.get_threads_size() << std::endl;
          sleep ( 5 );
     }

     return 0;
}


