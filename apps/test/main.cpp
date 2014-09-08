
// Nome do arquivo
// Data

// Licença e link para lincença

// Author: Nome <email>

/* Descrição:

 */

// ___________________________________________________________________


#include "../../derailleur.hpp"

#include <iostream>

int main(int argc, char *argv[])
{
    
    derailleur::Controller controller("0.0.0.0", 6653, 4, false);

    controller.start();
    while(1);
    
    return 0;
}


