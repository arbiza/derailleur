
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

          std::stringstream log;
          log << "on switch up - MAC " << s.get_mac_address();
          log << " - connection ID: " << event->get_switch_id();

          derailleur::Log::Instance()->log ( this->get_name().c_str(),
                                             log.str().c_str() );
     }



     void on_switch_down ( derailleur::Switch* switch_down ) override {

          std::stringstream log;
          log << "switch ";
          log << switch_down->get_mac_address();
          log << ", ID ";
          log << switch_down->get_switch_id();
          log << " is disconnected.";

          derailleur::Log::Instance()->log ( this->get_name().c_str(),
                                             log.str().c_str() );
     }



     void on_packet_in ( const derailleur::Event* const event ) override {


          /* learning_switch inherited method extracts link and internet
           * layers information from packet data, updates switches'
           * ARP-like tables (IPv4 and IPv6) and installs the proper
           * flow in the switch. */
          if ( this->learning_switch ( event ) ) {

               std::vector<derailleur::Arp4> arp =
                    get_IPv4_neighborhood ( event->get_switch_id() );

               std::cout << "\nn MACs: " << arp.size();


               for ( derailleur::Arp4 each : arp )
                    std::cout << "\nMAC: "
                              << derailleur::util::MAC_converter ( each.mac )
                              << " IP: "
                              << derailleur::util::ipv4_converter ( each.ip )
                              << " porta: "
                              << ( int ) each.port
                              <<  std::endl;
          }

          derailleur::Log::Instance()->log ( this->get_name().c_str(),
                                             "packet-in" );

          delete event;
     } // on_packet_in


     void message_handler ( const derailleur::Event* const event ) override {

          derailleur::Log::Instance()->log ( this->get_name().c_str(),
                                             "unknown/unhandled message" );

          delete event;
     }

};


int main ( int argc, char *argv[] )
{

     LearningSwitch app ( "Learning Switch" );
     derailleur::Controller controller ( "0.0.0.0", 6653, 4, false, &app,
                                         "log.txt" );

     controller.start();

     while ( 1 ) {
          sleep ( 1 );

     }

     return 0;
}


