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

#ifndef CLI_H
#define CLI_H


namespace derailleur {

#include <vector>
#include <map>

// TODO remove
#include <string>

// forward declaration
class Application;
class Switch;

class CLI {

public:
     CLI ( derailleur::Application* application );

     ~CLI() {
          delete switches_copies_;
     }

     /**
      * shell method starts the command line interface and waits for commands.
      */
     void shell ();

private:

     /**
      *
      */
     void help ();

     /**
      * show handles every show commands.
      */
     void show ( std::vector<std::string>& commands );

     /**
      * Print switch details and its LAN.
      * @param it Iterator at the position corresponding to the switch to be
      * printed.
      */
     void print_switch_lan (
          std::map< int, derailleur::Switch* >::iterator& it );
          
     /**
      * Print flows running on switch.
      * @param it Iterator at the position corresponding to the switch to be
      * printed.
      */
     void print_switch_flows (
          std::map< int, derailleur::Switch* >::iterator& it );

     /**
      *
      */
     derailleur::Application* application_;

     /**
      * Container where switches' copies are stored. Every time user asks for
      * switches this container is populated/updated; when user asks for an
      * specific switch only that switch is updated. It reduces the need of
      * copying switches objects, only some fields are updated.
      *
      * Why is this a pointer instead of an object such as in Controller class?
      * It is because CLI class is not instantiated in any other derailleur
      * class, this way switches_copies is not initialized causing Segmentation
      * Fault errors. This pointer is initialized in constructor.
      */
     std::map< int, derailleur::Switch* >* switches_copies_;

};

} // namespace derailleur

#endif // CLI_H
