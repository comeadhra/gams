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
 * 3. The names �Carnegie Mellon University,� "SEI� and/or �Software
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
 *      INSTITUTE MATERIAL IS FURNISHED ON AN �AS-IS� BASIS. CARNEGIE MELLON
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
#include "Algorithm_Factory.h"
#include "Land.h"
#include "Move.h"
#include "Printer_Algorithm.h"
#include "Random_Area_Coverage.h"
#include "Snake_Area_Coverage.h"
#include "Takeoff.h"


gams::algorithms::Factory::Factory (
  Madara::Knowledge_Engine::Knowledge_Base * knowledge,
  variables::Sensors * sensors,
  platforms::Base * platform,
  variables::Self * self,
  variables::Devices * devices)
: knowledge_ (knowledge), sensors_ (sensors),
  platform_ (platform), self_ (self),
  devices_ (devices)
{
}

gams::algorithms::Factory::~Factory ()
{
}

void
gams::algorithms::Factory::operator= (const Factory & rhs)
{
}

gams::algorithms::Base *
gams::algorithms::Factory::create (const std::string & type,
  const Madara::Knowledge_Record & arg1,
  const Madara::Knowledge_Record & arg2)
{
  if (type == "debug" || type == "print" || type == "printer")
  {
    if (knowledge_ && sensors_ && self_)
      return new Printer_Algorithm (knowledge_, platform_, sensors_, self_);
  }
  if (type == "random area coverage" || type == "rac")
  {
    if (knowledge_ && sensors_  && self_)
      return new Random_Area_Coverage (knowledge_, platform_, sensors_, self_);
  }
  else if (type == "snake" || type == "sac")
  {
    if (knowledge_ && sensors_ && platform_ && self_)
      return new Snake_Area_Coverage (knowledge_, platform_, sensors_, self_);
  }
  else if (type == "takeoff")
  {
    if (knowledge_ && sensors_ && platform_ && self_)
      return new Takeoff (knowledge_, platform_, sensors_, self_);
  }
  else if (type == "land")
  {
    if (knowledge_ && sensors_ && platform_ && self_)
      return new Land (knowledge_, platform_, sensors_, self_);
  }
  else if (type == "move")
  {
    if (knowledge_ && sensors_ && platform_ && self_ && arg1.is_string_type ())
      return new Move (arg1.to_string ());
  }

  return 0;
}

void
gams::algorithms::Factory::set_knowledge (
  Madara::Knowledge_Engine::Knowledge_Base * knowledge)
{
  knowledge_ = knowledge;
}
      

void
gams::algorithms::Factory::set_sensors (variables::Sensors * sensors)
{
  sensors_ = sensors;
}
      

void
gams::algorithms::Factory::set_platform (platforms::Base * platform)
{
  platform_ = platform;
}
      

void
gams::algorithms::Factory::set_self (variables::Self * self)
{
  self_ = self;
}

void
gams::algorithms::Factory::set_devices (variables::Devices * devices)
{
  devices_ = devices;
}