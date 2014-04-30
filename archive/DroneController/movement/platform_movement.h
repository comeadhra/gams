/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/

#ifndef _PLATFORM_MOVEMENT_H
#define _PLATFORM_MOVEMENT_H

bool platform_init_control_functions();

void platform_takeoff();
void platform_land();

// Simple movements, used to move for a certain amount of time in the given direction.
void platform_move_up();
void platform_move_down();
void platform_move_left();
void platform_move_right();
void platform_move_forward();
void platform_move_backward();

// Functions to move to a certain location or altitude, and to poll and see if such target has been reached (respectively).
void platform_move_to_location(double, double, double);
void platform_move_to_altitude(double);
bool platform_location_reached();
bool platform_altitude_reached();

void platform_stop_movement();

#endif