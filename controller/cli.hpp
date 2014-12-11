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

// forward declaration
class Application;

class CLI {
     
public:
     CLI( derailleur::Application* application ) {
          this->application_ = application;
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
     void show ( std::vector<std::string>& commands);
     
     /**
      * 
      */
     derailleur::Application* application_;
     
     /**
      * Container where switches' copies are stored. Every time user asks for
      * switches this container is populated/updated; when user asks for an 
      * specific switch only that switch is updated. It reduces the need of
      * copying switches objects, only some fields are updated.
      */
     std::map< int, derailleur::Switch* > switches_copies_;
     
};

} // namespace derailleur

#endif // CLI_H
