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
#include <vector>
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
          if ( commands.size() != 3 )
               std::cout << "Wrong usage of 'show switch'; use help to see options."
                         << std::endl;
          else {
               std::cout << "show switch" << std::endl;
          }
     } else
          std::cout << "Unknown option for 'show'; use help to see options."
                    << std::endl;
}



void derailleur::CLI::help()
{
     std::cout << "\n\nHELP\n"
               << "\nshow switches - list connected switches."
               << "\nshow switch <number> - show switch's info and its LAN."
               << "\n\nquit - stop controller and exit.\n"
               << std::endl;
}


