
// Nome do arquivo
// Data

// Licença e link para lincença

// Author: Nome <email>

/* Descrição:

 */

// ___________________________________________________________________


#include <iostream>
#include <sstream>
#include <list>


#include "../../derailleur.hpp"

#include <unistd.h>


class LearningSwitch : public derailleur::Application {

public:
     LearningSwitch ( std::string app_name )
          : derailleur::Application ( app_name ) {}



     void on_switch_up ( const derailleur::Event* const event ) override {
          derailleur::Switch13 s;
          get_switch_copy ( event->get_switch_id(), s );

          std::stringstream      log;
          log << "on switch up - MAC " << s.get_mac_address();
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


               /* learning_switch inherited method extracts link and internet
                * layers information from packet data, updates switches'
                * ARP-like tables (IPv4 and IPv6) and installs the proper
                * flow in the switch. */
               if ( this->learning_switch (
                              event->get_switch_id(), event ) ) {

                    std::list<derailleur::Arp4> arp =
                         get_IPv4_neighborhood ( event->get_switch_id() );
                         
                    std::cout << "\nn MACs: " << arp.size();

                    for ( derailleur::Arp4 each : arp )
                         std::cout << "\nMAC: "
                                   << derailleur::util::MAC_converter ( each.mac )
                                   <<  std::endl;
               }

          } else {
               derailleur::Log::Instance()->log ( "Learning Switch app",
                                                  "unknown or unhandled packet" );
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

     }

     return 0;
}


