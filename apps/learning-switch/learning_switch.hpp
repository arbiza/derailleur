
#include <string>
#include <derailleur/application.hpp>


// forward declarations
namespace derailleur {
class Event;
class Switch;
}


class LearningSwitch : public derailleur::Application {

public:
     LearningSwitch ( std::string app_name )
          : derailleur::Application ( app_name ) {}


     void on_switch_up ( const derailleur::Event* const event ) override;
     void on_switch_down ( derailleur::Switch* switch_down ) override;
     void on_packet_in ( const derailleur::Event* const event ) override;
     void message_handler ( const derailleur::Event* const event ) override;

};
