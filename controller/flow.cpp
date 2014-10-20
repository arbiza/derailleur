

#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>
#include <iomanip>

#include <fluid/of10msg.hh>
#include <fluid/of13msg.hh>

#include "flow.hpp"
#include "event.hpp"

derailleur::Flow::Flow ( const derailleur::Event* const event )
{

     /* Set the proper OpenFlow message version */
     if ( event->get_version() == fluid_msg::of13::OFP_VERSION )
          flow = new fluid_msg::of13::FlowMod;
     else if ( event->get_version() == fluid_msg::of10::OFP_VERSION )
          flow = new fluid_msg::of10::FlowMod;
     else {
          this->log_.custom_log ( "Flow destructed: unsupported or unknown OpenFlow version." );
          ~Flow();
     }


     /* Idle time before discarding (seconds).
      * Set default to 0 (do not expire). */
     this->flow_mod_->idle_timeout ( 0 );


     /* Max time before discarding (seconds).
      * Set default to 0 (do not expire). */
     this->flow_mod_->hard_timeout ( 0 );

     /* Buffered packet to apply to, or OFP_NO_BUFFER. Not meaningful for
      * OFPFC_DELETE.
      * It is set to 0 above. */
     this->flow_mod_->buffer_id ( 0xffffffff );
}


bool derailleur::Flow::set_command_ADD ( uint16_t priority,
          derailleur::flow_mod_commands command,
          derailleur::flow_mod_flags flag )
{
        
}


/**
 * Create a basic flow with some default values set. The flow is created
 * according OpenFlow version.
 * Buffered packet to apply to, or OFP_NO_BUFFER. Not meaningful for
// * OFPFC_DELETE.
// * It is set to 0 above. */
// flow13->buffer_id ( 0xffffffff );
// 
// /* Default table is set to 0. */
// flow13->table_id ( 0 );
// * @param version OpenFlow version
// * @param command OpenFlow command enumerated in ofp_flow_mod_command ( described
//           *                      bellow );
// I used enumerator from of13 because it is the
// *                      same as of10.
// * @return fluid_msg::FlowModCommon* pointer;
// you need to static_cast the
// *              returning pointer to a pointer of flow mod of the version you
// *              are using;
// you may also use auto ptr* ( recommended ).
// *
// * enum ofp_flow_mod_command {
//      *      OFPFC_ADD = 0,          // New flow.
//      *      OFPFC_MODIFY = 1,       // Modify all matching flows.
//      *      OFPFC_MODIFY_STRICT = 2,// Modify entry strictly matching wildcards
//      *                              // and priority.
//      *      OFPFC_DELETE = 3,       // Delete all matching flows.
//      *      OFPFC_DELETE_STRICT = 4,// Delete entry strictly matching wildcards and
//      *                              //priority.
//      *
// };
// */

static fluid_msg::FlowModCommon* create_flow_from_packet_in (
     uint8_t version,
     const fluid_msg::of13::PacketIn* packet_in )
{

     fluid_msg::FlowModCommon* flow;

     // OpenFlow 1.3 messages
     if ( version == fluid_msg::of13::OFP_VERSION ) {

          fluid_msg::of13::FlowMod* flow13 = new fluid_msg::of13::FlowMod;

          packet_in->unpack ( event->get_data() );


          flow13->xid ( packet_in->xid() );


          /* Mask used to restrict the cookie bits that must match when the
           * command is OFPFC_MODIFY* or OFPFC_DELETE*. A value of 0 indicates
           * no restriction.
           * It is set to 0 above. */
          flow13->cookie_mask ( 0xffffffffffffffff );


          /* Default table is set to 0. */
          flow13->table_id ( 0 );





          // fm.out_port ( 0 );
          // fm.out_group ( 0 );
          // fm.flags ( 0 );
          // //of13::EthSrc fsrc(((uint8_t*) &src) + 2);
          // //of13::EthDst fdst(((uint8_t*) &dst) + 2);
          // fm.add_oxm_field ( fsrc );
          // fm.add_oxm_field ( fdst );
          // of13::OutputAction act ( out_port, 1024 );
          // of13::ApplyActions inst;
          // inst.add_action ( act );
          // fm.add_instruction ( inst );

          flow = flow13;
     }
     // OpenFlow 1.0 messages
     else if ( version == fluid_msg::of10::OFP_VERSION ) {

          fluid_msg::of10::FlowMod* flow10 = new fluid_msg::of10::FlowMod;

          flow = flow10;
     } else {
          flow = nullptr;
     }

     return flow;
}
