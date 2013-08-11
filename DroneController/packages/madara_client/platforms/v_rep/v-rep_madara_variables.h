/********************************************************************* 
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/

/*********************************************************************
 * v-rep_madara_variables.h - Madara variables used for simulation between
 * the V-Rep platform on the Drone and the plugin for that in V-Rep.
 *********************************************************************/

#ifndef _VREP_MADARA_VARIABLES_H
#define _VREP_MADARA_VARIABLES_H

#include <string>

// Multicast IP group.
const std::string DEFAULT_MULTICAST_ADDRESS ("239.255.0.2:4250");

// Domain
const std::string VREP_DOMAIN ("v_rep");

// For simulation purposes.
#define MS_SIM_PREFIX           "sim"                   // Prefix used to disseminate local device variables for simulator.
#define MS_SIM_DEVICES_PREFIX   "sim.device."           // Prefix used to disseminate local device variables for simulator.
#define MS_SIM_CMD_SENT_ID      ".sent_command_id"      // Suffix used to indicate the id of the last command sent, to resend for lost ones.
#define MS_SIM_CMD_RCVD_ID      ".received_command_id"  // Suffix used to indicate the id of the command received, to resend for lost ones.

// Movement commands variables.
#define MV_MOVEMENT_REQUESTED   ".movement_command"                             // Internal command to tell that we want certain movement.
#define MV_MOVEMENT_CMD_ARG(i)  ".movement_command."  + std::string(i) + ""     // The i argument for the command, starting at i=0.

// Movement commands.
#define MO_MOVE_TO_GPS_CMD      "move_to_gps"           // Command used to tell drone to move to that a location.
#define MO_MOVE_TO_ALTITUDE_CMD "move_to_altitude"      // Command used to tell drone to move to a specific altitude.
#define MO_LAND_CMD             "land"                  // Command used to tell drone to land at current location
#define MO_TAKEOFF_CMD          "takeoff"               // Command used to tell drone to takeoff

// Sensor variables.
#define MV_LATITUDE             ".location.latitude"    // Latitude.
#define MV_LONGITUDE            ".location.longitude"   // Longitude.
#define MV_ALTITUDE             ".location.altitude"    // Altitude.

#endif