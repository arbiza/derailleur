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

#ifndef LOG_H
#define LOG_H


#include <fstream>
#include <mutex>


namespace derailleur {


// TODO Review comments; there may be references to the previous implementation where the instance must be started by Controller.

/**
 * Log class provide a logging feature (obviously). It is implemented using the
 * Singleton design pattern; log object is instantiated by controller class; 
 * when any other class tries to instantiate it a pointer to the instantiated 
 * object will be returned. 
 * Using a mutex Log class prevents logs to be printed wrong. It provides 
 * different logs methods to log. 
 * 
 * Usage: 
 *      At first, open the log file:
 *              derailleur::Log::Instance()->open_log_file ( "file path" );
 *      To log use the logging methods through the Instance method:
 *              derailleur::Log::Instance()->log ( ... );
 *  
 * @see Controller
 */
class Log {
     
public:
     
     /**
      * Returns the Log instance (instance_). This method checks if controller
      * already called Log constructor, if not program is aborted; Controller
      * must call constructor first to start it properly.
      * @see Controller
      * @return Log* pointer to the Log instance.
      */
     static Log* Instance();
     
     void open_log_file ( const char* path );
     
     /**
      * This methos provides the default logging format: [who] when: what
      * Class informs who is the logger (e.g. controller) and the message to be
      * logged, this method provided the "when".
      * @param logger Who is logging
      * @param message The message to be logged.
      */
     void log ( const char* logger,
                const char* message );
                
     /**
      * This method should be used for custom logs.
      * @param message A custom message (e.g. "Unknown error,  probably ...")
      */
     void log ( const char* message);

private:
     /**
      * Constructor is private due to the Singleton design patter; it will be
      * called by the Controller class (it is a friend). Constructor opens the
      * log file and instantiate instance_ pointer. Constructor writes the first
      * log to differentiate each Controller execution.
      * @param path path to the log file; Controller reads the path from the
      *                 configuration file
      * @see Controller
      */
     Log() {}
     
     /**
      * Destructor closes the log file; It is called from Controller destructor.
      */
     ~Log();
     
     /**
      * Return the time (now) to be used in log messages.
      * Time format example: Oct 19 18:01:41
      */
     static const char* get_time ();

     
     /**
      * instance_ attribute in set NULL when program starts (see the source 
      * file). It is used to indicate if there is already a Log object 
      * instantiated.
      */
     static Log* instance_;
     
     /**
      * The log file. 
      */
     std::fstream file_;
     
     /**
      * Used to avoid different threads write to the log file at the same time.
      */
     std::mutex mutex_;
     
     /**
      * Controller is a friend class because Controller calls the constructor 
      * who opens log file and instantiate instance_ attribute.
      * Log destructor is called from Controller destructor when controller is
      * shut down.
      */
};

} // namespace derailleur

#endif // LOG_H
