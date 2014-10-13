
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
          
//           fluid_msg::of13::FlowMod fm;
//           //fm.xid(pi.xid());
//           fm.cookie(30);
//           fm.cookie_mask(0xffffffffffffffff);
//           fm.table_id(0);
//           fm.command(fluid_msg::of13::OFPFC_ADD);
//           fm.idle_timeout(0);
//           fm.hard_timeout(0);
//           fm.priority(100);
//           fm.buffer_id( 0xffffffff );
//           fm.out_port( fluid_msg::of13::OFPP_ANY );
//           fm.out_group( fluid_msg::of13::OFPG_ANY );
//           fm.flags(0);
//           of13::EthSrc fsrc(((uint8_t*) &src) + 2);
//           of13::EthDst fdst(((uint8_t*) &dst) + 2);
//           fm.add_oxm_field(fsrc);
//           fm.add_oxm_field(fdst);
//           of13::OutputAction act(out_port, 1024);
          
          
          /* Example from libfluid MsgApps.hh */
          uint8_t* buffer;
          /*You can also set the message field using
          class methods which have the same names from
          the field present on the specification*/
          fluid_msg::of13::FlowMod fm;
          //fm.xid(pi.xid());
          fm.cookie(123);
          fm.cookie_mask(0xffffffffffffffff);
          fm.table_id(0);
          fm.command(fluid_msg::of13::OFPFC_ADD);
          fm.idle_timeout(5);
          fm.hard_timeout(10);
          fm.priority(100);
          fm.buffer_id(0xffffffff);
          fm.out_port(0);
          fm.out_group(0);
          fm.flags(0);
          //of13::EthSrc fsrc(((uint8_t*) &src) + 2);
          //of13::EthDst fdst(((uint8_t*) &dst) + 2);
          fm.add_oxm_field(fsrc);
          fm.add_oxm_field(fdst);
          of13::OutputAction act(out_port, 1024);
          of13::ApplyActions inst;
          inst.add_action(act);
          fm.add_instruction(inst);
          buffer = fm.pack();
          ofconn->send(buffer, fm.length());
          OFMsg::free_buffer(buffer);
          of13::Match m;
          of13::MultipartRequestFlow rf(2, 0x0, 0, of13::OFPP_ANY, of13::OFPG_ANY,
          0x0, 0x0, m);
          buffer = rf.pack();
          ofconn->send(buffer, rf.length());
          OFMsg::free_buffer(buffer);
          
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


