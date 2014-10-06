
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
          
          fluid_msg::of13::FlowMod fm;
          //fm.xid(pi.xid());
          fm.cookie(30);
          fm.cookie_mask(0xffffffffffffffff);
          fm.table_id(0);
          fm.command(fluid_msg::of13::OFPFC_ADD);
          fm.idle_timeout(0);
          fm.hard_timeout(0);
          fm.priority(100);
          fm.buffer_id( 0xffffffff );
          fm.out_port( fluid_msg::of13::OFPP_ANY );
          fm.out_group( fluid_msg::of13::OFPG_ANY );
          fm.flags(0);
          of13::EthSrc fsrc(((uint8_t*) &src) + 2);
          of13::EthDst fdst(((uint8_t*) &dst) + 2);
          fm.add_oxm_field(fsrc);
          fm.add_oxm_field(fdst);
          of13::OutputAction act(out_port, 1024);
          
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


