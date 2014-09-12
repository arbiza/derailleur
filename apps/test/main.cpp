
// Nome do arquivo
// Data

// Licença e link para lincença

// Author: Nome <email>

/* Descrição:

 */

// ___________________________________________________________________


#include "../../derailleur.hpp"

#include <iostream>
#include <string>


class MyApp : public derailleur::Application {

public:
     MyApp ( std::string app_name )
          : derailleur::Application ( app_name ) {}

     void on_switch_up ( const int switch_id ) override {
          derailleur::Switch* s = get_switch ( switch_id );

          std::cout << "Manufacturer: " << s->get_manufacturer()
                    << "\nHardware: " << s->get_hardware()
                    << "\nSoftware: " << s->get_software()
                    << "\nSerial: " << s->get_serial_number()
                    << "\nDatapath: " << s->get_datapath()
                    << std::endl;
     }

     void on_switch_down ( const int switch_id ) override {

     }

     void message_handler ( const int switch_id,
                            derailleur::Message* const message ) override {

     }
};


int main ( int argc, char *argv[] )
{

     MyApp myapp ( "Test app" );
     derailleur::Controller controller ( "0.0.0.0", 6653, 4, false, &myapp );

     controller.start();


     while ( 1 );

     return 0;
}


