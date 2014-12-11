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

#include "application.hpp"
#include "cli.hpp"

void derailleur::CLI::shell()
{
     bool stay = true;

     std::cout << "\n\nDerailleur OpenFlow Controller"
               << "\nhttp://unexisting.page.com.br"
               << "\n______________________________"
               << "\n\nType your commands; help to see possibilities."
               << "\nType 'quit' to exit.\n"
               << std::endl;

               
     while ( stay ) {
          
          std::cout << this->application_->get_name() << " > ";
          std::cin >> this->input_;
          
          if ( this->input_.find( "quit" ) != std::string::npos )
               stay = false;
     }

     
     std::cout << "\n\nBye!\n\n" << std::endl;
}


