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
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <sstream>

#include "log.hpp"

// Initialize instance_
derailleur::Log* derailleur::Log::instance_ = NULL;


derailleur::Log* derailleur::Log::Instance ()
{
     // TODO: check if file is set
     if ( !instance_ )
          instance_ = new derailleur::Log;

     return instance_;
}



void derailleur::Log::open_log_file ( const char* path )
{
     if ( this->file_.is_open() )
          derailleur::Log::log ( "Log", "WARNING: Attempting to open a log file, but the file is already open." );
     else {
          // File is opened in append mode, it means the content will be wrote after
          // existing content.
          this->file_.open ( path, std::fstream::app );

          if ( !this->file_.is_open() ) {
               std::cout << "\n\n************************************"
                         << "\nError opening log file. Program exited."
                         << "\n************************************\n\n"
                         <<  std::endl;

               exit ( EXIT_FAILURE );
          } else {
               this->file_ << "\n" << std::endl;
               log ( "Log", "NEW CONTROLLER EXECUTION." );
          }
     }
}



derailleur::Log::~Log()
{
     if ( this->file_.is_open() )
          this->file_.close();
}



const char* derailleur::Log::get_time()
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
     if ( now->tm_mday < 10 )
          output <<  "0" <<  now->tm_mday;
     else
          output << now->tm_mday << " ";

     // Hour
     if ( now->tm_hour < 10 )
          output <<  "0" <<  now->tm_hour << ":";
     else
          output << now->tm_hour << ":";

     // Minutes
     if ( now->tm_min < 10 )
          output <<  "0" <<  now->tm_min << ":";
     else
          output << now->tm_min << ":";

     // Seconds
     if ( now->tm_sec < 10 )
          output <<  "0" <<  now->tm_sec;
     else
          output << now->tm_sec;

     return output.str().c_str();
}



void derailleur::Log::log ( const char* logger, const char* message )
{
     this->mutex_.lock();

     this->file_ << std::setw ( 15 ) << setiosflags ( std::ios::left ) << logger
                 << " | "
                 << get_time()
                 << " - " << message
                 << std::endl;

     this->mutex_.unlock();
}


