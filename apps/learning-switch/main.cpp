
// Nome do arquivo
// Data

// Licença e link para lincença

// Author: Nome <email>

/* Descrição:

 */

// ___________________________________________________________________


#include <iostream>
#include <vector>
#include <sstream>


#include "../../derailleur.hpp"

#include <unistd.h>


class LearningSwitch : public derailleur::Application {

public:
     LearningSwitch ( std::string app_name )
          : derailleur::Application ( app_name ) {}

          
          
     void on_switch_up ( const derailleur::Event* const event ) override {
          derailleur::Switch* s = get_switch_copy ( event->get_switch_id() );

          std::stringstream      log;
          log << "on switch up - MAC " << s->get_mac_address();
          log << " - connection ID: " << event->get_switch_id();

          derailleur::Log::Instance()->log ( "LearningSwitch",
                                             log.str().c_str() );
     }

     
     
     void on_switch_down ( const int switch_id ) override {

     }

     
     
     void message_handler ( const derailleur::Event* const event ) override {

          // packet-in
          if ( event->get_type() == 10 ) {


               fluid_msg::PacketInCommon* packet_in = nullptr;

               if ( event->get_version() == fluid_msg::of13::OFP_VERSION ) {
                    packet_in = new fluid_msg::of13::PacketIn();
                    packet_in->unpack ( event->get_data() );
               } else {
                    packet_in = new fluid_msg::of10::PacketIn();
                    packet_in->unpack ( event->get_data() );
               }

               uint16_t l1_protocol = derailleur::util::get_link_layer_protocol (
                                           ( uint8_t* ) packet_in->data()
                                      );

               // Check if neighborhood discovery protocols are used in this
               // packet: NDP for IPv6 and ARP for IPv4.
               if ( l1_protocol == derailleur::util::Protocols.link_layer.ipv6 ||
                         l1_protocol == derailleur::util::Protocols.link_layer.arp )




                    std::cout << "protocol: "
                              << derailleur::util::get_link_layer_protocol (
                                   ( uint8_t* ) packet_in->data() )
                              << std::endl;


          } else {
               derailleur::Log::Instance()->log ( "Test", "unknown" );
          }

     }

};


int main ( int argc, char *argv[] )
{

     LearningSwitch app ( "Learning Switch app" );
     derailleur::Controller controller ( "0.0.0.0", 6653, 4, false, &app,
                                         "log.txt" );

     controller.start();

     while ( 1 ) {
          sleep ( 1 );

          //std::cout << "stack size: " << controller.get_stack_size() << std::endl;
     }

     return 0;
}


