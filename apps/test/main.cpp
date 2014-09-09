
// Nome do arquivo
// Data

// Licença e link para lincença

// Author: Nome <email>

/* Descrição:

 */

// ___________________________________________________________________


#include "../../derailleur.hpp"

#include <iostream>


class MyApp : public derailleur::Application {
    
public:
    MyApp(const char* address,
          const int port,
          const int n_workers,
          const bool secure)
        : derailleur::Application(address, port, n_workers, secure) {}

    void on_switch_up () override {

    }

    void on_switch_down() override {

    }

    void handler(derailleur::Message* message) override {

    }
};


int main(int argc, char *argv[])
{
    
    // derailleur::Controller controller("0.0.0.0", 6653, 4, false);

    // controller.start();
    // while(1);

    MyApp myapp("0.0.0.0", 6653, 4, false);


    while(1);
    
    return 0;
}


