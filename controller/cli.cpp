/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2014  Lucas Arbiza - Inventati <lucas.arbiza@inventati.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>

#include "application.hpp"
#include "cli.hpp"

derailleur::CLI::CLI ( derailleur::Application* application )
{
     this->application_ = application;
     this->switches_copies_ = new std::map< int, derailleur::Switch* >;
}


void derailleur::CLI::shell()
{

     bool stay = true;
     std::string input;
     std::vector<std::string> commands;

     /* allocate vector for at least 3 positions */
     commands.reserve ( 3 );

     // get the name of the running app to show in the prompt
     std::string app_name = this->application_->get_name();


     std::cout << "\n\nDerailleur OpenFlow Controller"
               << "\nhttp://unexisting.page.com.br"
               << "\n______________________________"
               << "\n\nType your commands; help to see possibilities."
               << "\nType 'quit' to exit.\n"
               << std::endl;


     while ( stay ) {

          std::cout << app_name << " $> ";
          std::getline ( std::cin, input );


          if ( input.empty() ) {
               // do nothing
          } else {
               boost::split ( commands, input, boost::is_any_of ( " " ) );


               if ( commands[0] == "show" )
                    show ( commands );
               else if ( commands[0] == "help" )
                    help();
               else if ( commands[0] == "quit" )
                    stay = false;
               else
                    std::cout << "Unknown command; use help to see options."
                              << std::endl;

               /* clean vector up */
               for ( std::string& s : commands )
                    s.clear();
          }
     }

     std::cout << "\n\nBye!\n\n" << std::endl;
}



void derailleur::CLI::help()
{
     std::cout << "\n\nHELP\n"
               << "\nshow switches - list connected switches."
               << "\nshow switch <ID> network - show switch info and its LAN."
               << "\nshow switch <ID> flows - show running flows."
               << "\n\nquit - stop controller and exit.\n"
               << std::endl;
}



void derailleur::CLI::show ( std::vector< std::string >& commands )
{
     if ( commands.size() < 2 ) {

          std::cout << "'show' requires arguments; use help to see options."
                    << std::endl;

     }
     /* list switches */
     else if ( commands[1] == "switches" ) {

          this->application_->get_switches_copies ( switches_copies_ );

          std::cout << "\nSwitches connected: "
                    << this->switches_copies_->size()
                    << "\n" << std::endl;

          if ( this->switches_copies_->size() > 0 ) {

               std::cout << std::setw ( 6 ) << "ID"
                         << std::setw ( 24 ) << "MAC"
                         << std::setw ( 20 ) << "OF Version"
                         << std::setw ( 24 ) << "Name"
                         << std::endl;

               std::map< int, derailleur::Switch* >::iterator it;

               for ( it = this->switches_copies_->begin();
                         it != this->switches_copies_->end();
                         ++it ) {

                    std::cout << std::setw ( 6 ) << it->first
                              << std::setw ( 24 ) << it->second->get_mac_address()
                              << std::setw ( 20 ) << ( int ) it->second->get_version()
                              << std::setw ( 24 ) << it->second->get_name()
                              << std::endl;
               }

               std::cout << std::endl;
          }

     }
     /* Show one switch */
     else if ( commands[1] == "switch" ) {

          /* check if there is the number of required arguments */
          if ( commands.size() < 4 ) {

               std::cout << "Wrong usage of 'show switch'; use help to see options."
                         << std::endl;
          }
          /* access selected switch if user informed correctly */
          else {
               /* check if argument is a number */
               if ( std::all_of (
                              commands[2].begin(), commands[2].end(), ::isdigit ) ) {

                    /* check if switches_copies is populated */
                    if ( this->switches_copies_->empty() )
                         this->application_->get_switches_copies ( switches_copies_ );


                    int id = std::atoi ( commands[2].c_str() );

                    std::map< int, derailleur::Switch* >::iterator it;

                    /* search for the switch in switches container */
                    it = this->switches_copies_->find ( id );
                    if ( it != this->switches_copies_->end() ) {


                         if ( commands[3] == "lan" )
                              print_switch_lan ( it );
                         else
                              print_switch_flows ( it );


                    }
                    /* if switch was not found a helping message is printed */
                    else {
                         std::cout << "There is no switch with ID "
                                   << id <<  std::endl;
                    }

               }
               /* if is not a number print helping information */
               else {
                    std::cout << "'show switch' requires a valid switch ID."
                              << std::endl;
               }
          }
     } else
          std::cout << "Unknown option for 'show'; use help to see options."
                    << std::endl;
}



void derailleur::CLI::print_switch_lan (
     std::map< int, derailleur::Switch* >::iterator& it )
{
     std::cout << "\nSwitch ID:" << std::setfill ( '.' ) << std::setw ( 64 ) << it->first
               << "\nName:" << std::setfill ( '.' ) << std::setw ( 69 ) << it->second->get_name()
               << "\nMAC:" << std::setfill ( '.' ) << std::setw ( 70 ) << it->second->get_mac_address()
               << "\nManufacturer:" << std::setfill ( '.' ) << std::setw ( 61 ) << it->second->get_manufacturer()
               << "\nOpenFlow version:" << std::setfill ( '.' ) << std::setw ( 57 ) << ( int ) it->second->get_version()
               << "\nHardware:"  << std::setfill ( '.' ) << std::setw ( 65 ) << it->second->get_hardware()
               << "\nSoftware:" << std::setfill ( '.' ) << std::setw ( 65 ) << it->second->get_software()
               << "\nSerial number:" << std::setfill ( '.' ) << std::setw ( 60 ) << it->second->get_serial_number()
               << "\n\nLAN"
               << std::endl;
}



void derailleur::CLI::print_switch_flows (
     std::map< int, derailleur::Switch* >::iterator& it )
{

}



