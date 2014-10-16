
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

          switch ( event->get_type() ) {

          case 10: // packet-in

               uint64_t dst = 0, src = 0;
               void* ofpip;
               uint8_t* data;
               uint16_t in_port;

               if ( event->get_version() == fluid_msg::of10::OFP_VERSION ) {

               }
               else if ( event->get_version() == fluid_msg::of13::OFP_VERSION ) {
                    
                    fluid_msg::of13::PacketIn *packet_in =
                         new fluid_msg::of13::PacketIn();

                    packet_in->unpack ( event->get_data() );

                    //data = ( uint8_t* ) packet_in->data();

//                     memcpy ( ( ( uint8_t* ) &dst ) + 2, ( uint8_t* ) ofpi->data(), 6 );
//                     memcpy ( ( ( uint8_t* ) &src ) + 2, ( uint8_t* ) ofpi->data() + 6, 6 );



//                     std::bitset<64> bits ( this->get_datapath_id() );
//                     this->mac_address_ =
//                     this->convert_bits_to_mac_address ( bits.to_string() );



                    if ( ofpi->match().in_port() == NULL ) {
                         return;
                    }

                    in_port = ofpi->match().in_port()->value();

//                     fluid_msg::of13::FlowMod* flow =
//                          static_cast<fluid_msg::of13::FlowMod*> (
//                               derailleur::flow::create_flow (
//                                    fluid_msg::of13::OFP_VERSION,
//                                    fluid_msg::of13::OFPFC_ADD )
//                          );
                    auto* flow = derailleur::flow::create_flow (
                                      fluid_msg::of13::OFP_VERSION,
                                      fluid_msg::of13::OFPFC_ADD );

//                     std::cout << "Packet-in: \n"
//                               << "dst: " << derailleur::Flow::convert_bits_to_MAC ( ( uint8_t* ) ofpi->data() )
//                     << "\nsrc: " << derailleur::Flow::convert_bits_to_MAC ( ( uint8_t* ) ofpi->data() + 6 )
//                               << "\nin port: " << in_port
//                               << "\ndata: " << ofpi->data()
//                               << std:: endl;

//                     std::string mac = derailleur::flow::util::convert_bits_to_MAC ( ( uint8_t* ) ofpi->data() );
//
//                     std::cout << "\n\nConversões:"
//                               << "\nMAC destino: " << mac
//                               << "\nuint8* original: " << ( ( uint8_t* ) ofpi->data() )
//                               << "\nuint8* pós-conversão: " << derailleur::flow::util::convert_MAC_to_bits ( &mac )
//                               << "\nMAC reconvertido: " << derailleur::flow::util::convert_bits_to_MAC ( derailleur::Flow::convert_MAC_to_bits ( &mac ) )
//                               << std::endl;
               }

               break;
          }

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


