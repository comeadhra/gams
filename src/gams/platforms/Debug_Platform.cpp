/**
 * Copyright (c) 2014 Carnegie Mellon University. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following acknowledgments and disclaimers.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * 3. The names "Carnegie Mellon University," "SEI" and/or "Software
 *    Engineering Institute" shall not be used to endorse or promote products
 *    derived from this software without prior written permission. For written
 *    permission, please contact permission@sei.cmu.edu.
 * 
 * 4. Products derived from this software may not be called "SEI" nor may "SEI"
 *    appear in their names without prior written permission of
 *    permission@sei.cmu.edu.
 * 
 * 5. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 * 
 *      This material is based upon work funded and supported by the Department
 *      of Defense under Contract No. FA8721-05-C-0003 with Carnegie Mellon
 *      University for the operation of the Software Engineering Institute, a
 *      federally funded research and development center. Any opinions,
 *      findings and conclusions or recommendations expressed in this material
 *      are those of the author(s) and do not necessarily reflect the views of
 *      the United States Department of Defense.
 * 
 *      NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING
 *      INSTITUTE MATERIAL IS FURNISHED ON AN "AS-IS" BASIS. CARNEGIE MELLON
 *      UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR
 *      IMPLIED, AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF
 *      FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS
 *      OBTAINED FROM USE OF THE MATERIAL. CARNEGIE MELLON UNIVERSITY DOES
 *      NOT MAKE ANY WARRANTY OF ANY KIND WITH RESPECT TO FREEDOM FROM PATENT,
 *      TRADEMARK, OR COPYRIGHT INFRINGEMENT.
 * 
 *      This material has been approved for public release and unlimited
 *      distribution.
 **/
#include "Debug_Platform.h"
#include "gams/utility/Logging.h"

gams::platforms::Base_Platform *
gams::platforms::Debug_Platform_Factory::create (
        const Madara::Knowledge_Vector & args,
        Madara::Knowledge_Engine::Knowledge_Base * knowledge,
        variables::Sensors * sensors,
        variables::Platforms * platforms,
        variables::Self * self)
{
  Base_Platform * result (0);
  
  if (knowledge && sensors && platforms && self)
  {
    result = new Debug_Platform (knowledge, sensors, platforms, self);
  }

  return result;
}

gams::platforms::Debug_Platform::Debug_Platform (
  Madara::Knowledge_Engine::Knowledge_Base * knowledge,
  variables::Sensors * sensors,
  variables::Platforms * platforms,
  variables::Self * self,
  const std::string & executions_location)
  : Base_Platform (knowledge, sensors, self)
{
  if (platforms && knowledge)
  {
    (*platforms)[get_id ()].init_vars (*knowledge, get_id ());
    status_ = (*platforms)[get_id ()];
    executions_.set_name (executions_location, *knowledge);
  }
}

gams::platforms::Debug_Platform::~Debug_Platform ()
{
}

void
gams::platforms::Debug_Platform::operator= (const Debug_Platform & rhs)
{
  if (this != &rhs)
  {
    platforms::Base_Platform * dest = dynamic_cast <platforms::Base_Platform *> (this);
    const platforms::Base_Platform * source =
      dynamic_cast <const platforms::Base_Platform *> (&rhs);

    *dest = *source;

    this->position_ = rhs.position_;
    this->executions_ = rhs.executions_;
  }
}
 
int
gams::platforms::Debug_Platform::analyze (void)
{
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q: platform.analyze ()\n",
    *self_->id, *executions_));

  if (sensors_)
  {
    GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
      "%q:%q:   platform.sensors_ is set\n",
      *self_->id, *executions_));
  }
  else
  {
    GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
      "%q:%q:   ERROR: platform.sensors_ is not set\n",
      *self_->id, *executions_));
  }

  if (self_)
  {
    GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
      "%q:%q:   platform.self_ is set\n",
      *self_->id, *executions_));
  }
  else
  {
    GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
      "%q:%q:   ERROR: platform.self_ is not set\n",
      *self_->id, *executions_));
  }

  status_.moving = 0;
  status_.communication_available = 1;
  status_.movement_available = 1;
  status_.sensors_available = 1;
  status_.waiting = 1;
  
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.ok == %q\n",
    *self_->id, *executions_, *status_.ok));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.waiting == %q\n",
    *self_->id, *executions_, *status_.waiting));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.deadlocked == %q\n",
    *self_->id, *executions_, *status_.deadlocked));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.failed == %q\n",
    *self_->id, *executions_, *status_.failed));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.moving == %q\n",
    *self_->id, *executions_, *status_.moving));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.reduced_sensing == %q\n",
    *self_->id, *executions_, *status_.reduced_sensing));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.reduced_movement == %q\n",
    *self_->id, *executions_, *status_.reduced_movement));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.communication_available == %q\n",
    *self_->id, *executions_, *status_.communication_available));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.sensors_available == %q\n",
    *self_->id, *executions_, *status_.sensors_available));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.movement_available == %q\n",
    *self_->id, *executions_, *status_.movement_available));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.gps_spoofed == %q\n",
    *self_->id, *executions_, *status_.gps_spoofed));
  
  return 0;
}

std::string
gams::platforms::Debug_Platform::get_id () const
{
  return "debug_printer";
}

std::string
gams::platforms::Debug_Platform::get_name () const
{
  return "Debug Printer";
}

double
gams::platforms::Debug_Platform::get_accuracy () const
{
  return 0.0;
}

double
gams::platforms::Debug_Platform::get_move_speed () const
{
  return 0.0;
}

int
gams::platforms::Debug_Platform::home (void)
{
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q: platform.home ()\n",
    *self_->id, *executions_));
  
  if (sensors_)
  {
    GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
      "%q:%q:   platform.sensors_ is set\n",
      *self_->id, *executions_));
  }
  else
  {
    GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
      "%q:%q:   ERROR: platform.sensors_ is not set\n",
      *self_->id, *executions_));
  }

  if (self_)
  {
    GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
      "%q:%q:   platform.self_ is set\n",
      *self_->id, *executions_));
  }
  else
  {
    GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
      "%q:%q:   ERROR: platform.self_ is not set\n",
      *self_->id, *executions_));
  }
  status_.waiting = 0;
  status_.moving = 1;
  
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.ok == %q\n",
    *self_->id, *executions_, *status_.ok));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.waiting == %q\n",
    *self_->id, *executions_, *status_.waiting));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.deadlocked == %q\n",
    *self_->id, *executions_, *status_.deadlocked));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.failed == %q\n",
    *self_->id, *executions_, *status_.failed));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.moving == %q\n",
    *self_->id, *executions_, *status_.moving));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.reduced_sensing == %q\n",
    *self_->id, *executions_, *status_.reduced_sensing));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.reduced_movement == %q\n",
    *self_->id, *executions_, *status_.reduced_movement));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.communication_available == %q\n",
    *self_->id, *executions_, *status_.communication_available));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.sensors_available == %q\n",
    *self_->id, *executions_, *status_.sensors_available));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.movement_available == %q\n",
    *self_->id, *executions_, *status_.movement_available));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.gps_spoofed == %q\n",
    *self_->id, *executions_, *status_.gps_spoofed));
  
  return 0;
}

int
gams::platforms::Debug_Platform::land (void)
{
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q: platform.land ()\n",
    *self_->id, *executions_));

  if (sensors_)
  {
    GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
      "%q:%q:   platform.sensors_ is set\n",
      *self_->id, *executions_));
  }
  else
  {
    GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
      "%q:%q:   ERROR: platform.sensors_ is not set\n",
      *self_->id, *executions_));
  }

  if (self_)
  {
    GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
      "%q:%q:   platform.self_ is set\n",
      *self_->id, *executions_));
  }
  else
  {
    GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
      "%q:%q:   ERROR: platform.self_ is not set\n",
      *self_->id, *executions_));
  }

  status_.waiting = 0;
  status_.moving = 1;
  
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.ok == %q\n",
    *self_->id, *executions_, *status_.ok));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.waiting == %q\n",
    *self_->id, *executions_, *status_.waiting));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.deadlocked == %q\n",
    *self_->id, *executions_, *status_.deadlocked));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.failed == %q\n",
    *self_->id, *executions_, *status_.failed));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.moving == %q\n",
    *self_->id, *executions_, *status_.moving));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.reduced_sensing == %q\n",
    *self_->id, *executions_, *status_.reduced_sensing));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.reduced_movement == %q\n",
    *self_->id, *executions_, *status_.reduced_movement));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.communication_available == %q\n",
    *self_->id, *executions_, *status_.communication_available));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.sensors_available == %q\n",
    *self_->id, *executions_, *status_.sensors_available));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.movement_available == %q\n",
    *self_->id, *executions_, *status_.movement_available));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.gps_spoofed == %q\n",
    *self_->id, *executions_, *status_.gps_spoofed));
  
  return 0;
}

int
gams::platforms::Debug_Platform::move (const utility::Position & position,
  const double & /*epsilon*/)
{
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q: platform.move (%s)\n",
    *self_->id, *executions_, position.to_string (", ").c_str ()));

  if (sensors_)
  {
    GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
      "%q:%q:   platform.sensors_ is set\n",
      *self_->id, *executions_));
  }
  else
  {
    GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
      "%q:%q:   ERROR: platform.sensors_ is not set\n",
      *self_->id, *executions_));
  }

  if (self_)
  {
    GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
      "%q:%q:   platform.self_ is set\n",
      *self_->id, *executions_));
  }
  else
  {
    GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
      "%q:%q:   ERROR: platform.self_ is not set\n",
      *self_->id, *executions_));
  }

  status_.waiting = 0;
  status_.moving = 1;
  
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.ok == %q\n",
    *self_->id, *executions_, *status_.ok));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.waiting == %q\n",
    *self_->id, *executions_, *status_.waiting));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.deadlocked == %q\n",
    *self_->id, *executions_, *status_.deadlocked));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.failed == %q\n",
    *self_->id, *executions_, *status_.failed));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.moving == %q\n",
    *self_->id, *executions_, *status_.moving));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.reduced_sensing == %q\n",
    *self_->id, *executions_, *status_.reduced_sensing));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.reduced_movement == %q\n",
    *self_->id, *executions_, *status_.reduced_movement));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.communication_available == %q\n",
    *self_->id, *executions_, *status_.communication_available));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.sensors_available == %q\n",
    *self_->id, *executions_, *status_.sensors_available));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.movement_available == %q\n",
    *self_->id, *executions_, *status_.movement_available));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.gps_spoofed == %q\n",
    *self_->id, *executions_, *status_.gps_spoofed));
  
  return 0;
}

int
gams::platforms::Debug_Platform::sense (void)
{
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q: platform.sense ()\n",
    *self_->id, *executions_));

  if (sensors_)
  {
    GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
      "%q:%q:   platform.sensors_ is set\n",
      *self_->id, *executions_));
  }
  else
  {
    GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
      "%q:%q:   ERROR: platform.sensors_ is not set\n",
      *self_->id, *executions_));
  }

  if (self_)
  {
    GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
      "%q:%q:   platform.self_ is set\n",
      *self_->id, *executions_));
  }
  else
  {
    GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
      "%q:%q:   ERROR: platform.self_ is not set\n",
      *self_->id, *executions_));
  }

  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.ok == %q\n",
    *self_->id, *executions_, *status_.ok));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.waiting == %q\n",
    *self_->id, *executions_, *status_.waiting));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.deadlocked == %q\n",
    *self_->id, *executions_, *status_.deadlocked));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.failed == %q\n",
    *self_->id, *executions_, *status_.failed));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.moving == %q\n",
    *self_->id, *executions_, *status_.moving));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.reduced_sensing == %q\n",
    *self_->id, *executions_, *status_.reduced_sensing));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.reduced_movement == %q\n",
    *self_->id, *executions_, *status_.reduced_movement));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.communication_available == %q\n",
    *self_->id, *executions_, *status_.communication_available));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.sensors_available == %q\n",
    *self_->id, *executions_, *status_.sensors_available));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.movement_available == %q\n",
    *self_->id, *executions_, *status_.movement_available));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.gps_spoofed == %q\n",
    *self_->id, *executions_, *status_.gps_spoofed));
  
  return 0;
}

void
gams::platforms::Debug_Platform::set_move_speed (const double& /*speed*/)
{
}

int
gams::platforms::Debug_Platform::takeoff (void)
{
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q: platform.takeoff ()\n",
    *self_->id, *executions_));

  if (sensors_)
  {
    GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
      "%q:%q:   platform.sensors_ is set\n",
      *self_->id, *executions_));
  }
  else
  {
    GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
      "%q:%q:   ERROR: platform.sensors_ is not set\n",
      *self_->id, *executions_));
  }

  if (self_)
  {
    GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
      "%q:%q:   platform.self_ is set\n",
      *self_->id, *executions_));
  }
  else
  {
    GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
      "%q:%q:   ERROR: platform.self_ is not set\n",
      *self_->id, *executions_));
  }

  status_.waiting = 0;
  status_.moving = 1;
  
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.ok == %q\n",
    *self_->id, *executions_, *status_.ok));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.waiting == %q\n",
    *self_->id, *executions_, *status_.waiting));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.deadlocked == %q\n",
    *self_->id, *executions_, *status_.deadlocked));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.failed == %q\n",
    *self_->id, *executions_, *status_.failed));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.moving == %q\n",
    *self_->id, *executions_, *status_.moving));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.reduced_sensing == %q\n",
    *self_->id, *executions_, *status_.reduced_sensing));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.reduced_movement == %q\n",
    *self_->id, *executions_, *status_.reduced_movement));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.communication_available == %q\n",
    *self_->id, *executions_, *status_.communication_available));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.sensors_available == %q\n",
    *self_->id, *executions_, *status_.sensors_available));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.movement_available == %q\n",
    *self_->id, *executions_, *status_.movement_available));
  GAMS_DEBUG (gams::utility::LOG_EMERGENCY, (LM_DEBUG,
    "%q:%q:     platform.status_.gps_spoofed == %q\n",
    *self_->id, *executions_, *status_.gps_spoofed));
  
  return 0;
}
