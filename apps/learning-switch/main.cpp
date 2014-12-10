
// Nome do arquivo
// Data

// Licença e link para lincença

// Author: Nome <email>

/* Descrição:

 */

// ___________________________________________________________________

#include <unistd.h> // included due to sleep

#include <derailleur.hpp>
#include "learning_switch.hpp"

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


