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
#include <ctime>
#include <sstream>

#include "log.hpp"

// Initialize instance_
Log* Log::instance_ = NULL;


Log* derailleur::Log::Instance ()
{
     if ( !instance_ ) {
          std::cout << "\n\n************************************"
                    << "\nError: Controller must be started before to try to log anything."
                    << "\nController starts the Log object properly."
                    << "\nProgram aborted."
                    << "\n************************************\n\n"
                    <<  std::endl;

          exit ( EXIT_FAILURE );
     }

     return instance_;
}



derailleur::Log::Log ( const char* path )
{
     // File is opened in append mode, it means the content will be wrote after
     // existing content.
     if ( this->file_.open ( path, std::fstream::app ) ==  nullptr ) {
          std::cout << "\n\n************************************"
                    << "\nError opening log file. Program exited."
                    << "\nCheck your configurations."
                    << "\n************************************\n\n"
                    <<  std::endl;

          exit ( EXIT_FAILURE );
     }

     this->file_ << "\n\n_________________________\n" << std::endl;
     log ( "log instance", "NEW EXECUTION." );

     this->instance_ = new Log;
}



derailleur::Log::~Log()
{
     if ( this->file_.is_open() )
          this->file_.close();
}



char* derailleur::Log::get_time()
{
     std::stringstream output; // Time format example: Oct 19 18:01:41

     std::time_t t = time ( 0 );
     struct tm * now = std::localtime ( &t );

     //  Month
     switch ( now->tm_mon ) {
     case 0:
          output << "Jan ";
          break;
     case 1:
          output << "Feb ";
          break;
     case 2:
          output << "Mar ";
          break;
     case 3:
          output << "Apr ";
          break;
     case 4:
          output << "May ";
          break;
     case 5:
          output << "Jun ";
          break;
     case 6:
          output << "Jul ";
          break;
     case 7:
          output << "Aug ";
          break;
     case 8:
          output << "Sep ";
          break;
     case 9:
          output << "Oct ";
          break;
     case 10:
          output << "Nov ";
          break;
     case 11:
          output << "Dec ";
          break;
     }
     
     // Day
     output << now->tm_mday << " ";
     
     // Hour
     output << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec;

     return output.str().c_str();
}



void derailleur::Log::log ( const char* logger, const char* message )
{
     this->mutex_.lock();


     this->file_ << "[" <<  logger << "] "
                 <<  get_time()
                 << " - " << message
                 << std::endl;

     this->mutex_.unlock();
}



// void derailleur::Log::message_log ( const char* logger,
//                                     const int connection_id,
//                                     const int type )
// {
//      std::cout << "[" <<  logger << " - Message] connection: " << connection_id
//                << " - type: " << type
//                << std::endl;
// }

void derailleur::Log::log ( const char* message )
{
     std::cout << message << std::endl;
}

