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

#include "Base_Controller.h"

#include <iostream>

#include "ace/High_Res_Timer.h"
#include "ace/OS_NS_sys_time.h"
#include "madara/utility/Utility.h"
#include "gams/platforms/Platform_Factory.h"
#include "gams/algorithms/Algorithm_Factory.h"

using std::cerr;
using std::endl;

typedef  Madara::Knowledge_Record::Integer  Integer;

gams::controllers::Base::Base (
  Madara::Knowledge_Engine::Knowledge_Base & knowledge)
  : algorithm_ (0), knowledge_ (knowledge), platform_ (0)
{
}

gams::controllers::Base::~Base ()
{
  delete algorithm_;
  delete platform_;

  for (algorithms::Algorithms::iterator i = accents_.begin ();
       i != accents_.end (); ++i)
  {
    delete *i;
  }
}

int
gams::controllers::Base::monitor (void)
{
  return platform_->sense ();
}

int
gams::controllers::Base::system_analyze (void)
{
  int return_value (0);
  bool error (false);

  /**
   * note that certain device variables like command are kept local only.
   * @see gams::variables::Device::init_vars
   * @see gams::variables::Swarm::init_vars
   **/

  if (this->self_.device.command == "land" || this->swarm_.command == "land")
  {
    init_algorithm ("land");
  }

  else if (this->self_.device.command == "takeoff" || 
    this->swarm_.command == "takeoff")
  {
    init_algorithm ("takeoff");
  }

  else if (this->self_.device.command == "cover" ||
    this->swarm_.command == "cover")
  {
    Madara::Knowledge_Record type;
    Madara::Knowledge_Record area;
    
    if (self_.device.command == "cover")
    {
      self_.device.command_args.resize ();

      if (this->self_.device.command_args.size () == 2)
      {
        type = self_.device.command_args[0];
        area = self_.device.command_args[1];
      }
      else
      {
        error = true;
      }
    }
    else
    {
      swarm_.command_args.resize ();

      // check if proper number of arguments were given
      if (swarm_.command_args.size () == 2)
      {
        type = swarm_.command_args[0];
        area = swarm_.command_args[1];
      }
      else
      {
        error = true;
      }
    }

    if (error)
    {
      std::cerr << "ERROR: Area coverage requested with improper args.\n";
    }
    else
    {
      if (!type.is_string_type ())
      {
        std::cerr << "ERROR: Area coverage type (first arg) == "
          << type <<
          ". Type should be a string.\n";
        error = true;
      }
      if (!area.is_string_type ())
      {
        std::cerr << "ERROR: Area coverage area (second arg) == "
          << area <<
          ". Area should be a string.\n";
        error = true;
      }

      knowledge_.print ();
    }

    // check if args were of the right type
    if (!error)
    {
      delete algorithm_;
      algorithms::Factory factory (&knowledge_, &sensors_,
        platform_, &self_, &devices_);

      Madara::Knowledge_Vector args (1);
      args[0] = area;

      algorithm_ = factory.create (type.to_string (), args);
    }
  }
  
  else if (this->self_.device.command == "move" ||
    this->swarm_.command == "move")
  {
    Madara::Knowledge_Record arg1;
    Madara::Knowledge_Record arg2;
    utility::Position target;
    
    if (self_.device.command == "move")
    {
      self_.device.command_args.resize ();

      if (this->self_.device.command_args.size () == 2)
      {
        arg1 = self_.device.command_args[0];
        arg2 = self_.device.command_args[1];
      }
      else if (this->self_.device.command_args.size () == 1)
      {
        arg1 = self_.device.command_args[0];
      }
      else if (this->self_.device.command_args.size () == 0)
      {
        arg1 = "target";
      }
      else
      {
        error = true;
      }
    }
    else
    {
      swarm_.command_args.resize ();

      // check if proper number of arguments were given
      if (swarm_.command_args.size () == 2)
      {
        arg1 = swarm_.command_args[0];
        arg2 = swarm_.command_args[1];
      }
      else if (swarm_.command_args.size () == 1)
      {
        arg1 = swarm_.command_args[0];
      }
      else
      {
        error = true;
      }
    }

    if (error)
    {
      std::cerr << "ERROR: Move requested with improper args.\n";
    }
    else
    {
      delete algorithm_;
      algorithms::Factory factory (&knowledge_, &sensors_,
        platform_, &self_, &devices_);

      Madara::Knowledge_Vector args (2);
      args[0] = arg1;
      args[1] = arg2;

      algorithm_ = factory.create ("move", args);
    }
  }
 
  else if (this->self_.device.command == "formation" ||
    this->swarm_.command == "formation")
  {
    Madara::Knowledge_Record target_id;
    Madara::Knowledge_Record cylindrical_offset;
    Madara::Knowledge_Record destination;
    Madara::Knowledge_Record members;
    Madara::Knowledge_Record modifier;

    if (self_.device.command == "formation")
    {
      self_.device.command_args.resize ();

      if (this->self_.device.command_args.size () == 4)
      {
        target_id = self_.device.command_args[0];
        cylindrical_offset = self_.device.command_args[1];
        destination = self_.device.command_args[2];
        members = self_.device.command_args[3];
      }
      else if (this->self_.device.command_args.size () == 5)
      {
        target_id = self_.device.command_args[0];
        cylindrical_offset = self_.device.command_args[1];
        destination = self_.device.command_args[2];
        members = self_.device.command_args[3];
        modifier = self_.device.command_args[4];
      }
      else
      {
        error = true;
      }
    }
    else // swarm command
    {
    }

    if (error)
    {
      std::cerr << "ERROR: Formation requested with improper args.\n";
    }
    else
    {
      delete algorithm_;
      algorithms::Factory factory (&knowledge_, &sensors_,
        platform_, &self_, &devices_);

      Madara::Knowledge_Vector args (5);
      args[0] = target_id;
      args[1] = cylindrical_offset;
      args[2] = destination;
      args[3] = members;
      args[4] = modifier;

      algorithm_ = factory.create ("formation", args);
    }
  }

  else if (this->self_.device.command == "follow")
  {
    Madara::Knowledge_Record target_id;
    Madara::Knowledge_Record delay;

    if (self_.device.command == "follow")
    {
      self_.device.command_args.resize ();

      if (this->self_.device.command_args.size () == 1)
      {
        target_id = self_.device.command_args[0];
        delay.set_value ((Madara::Knowledge_Record::Integer)5);
      }
      else if (this->self_.device.command_args.size () == 2)
      {
        target_id = self_.device.command_args[0];
        delay = self_.device.command_args[1];
      }
      else
      {
        error = true;
      }
    }

    if (error)
    {
      std::cerr << "ERROR: Formation requested with improper args.\n";
    }
    else
    {
      delete algorithm_;
      algorithms::Factory factory (&knowledge_, &sensors_,
        platform_, &self_, &devices_);

      Madara::Knowledge_Vector args (2);
      args[0] = target_id;
      args[1] = delay;

      algorithm_ = factory.create ("follow", args);
    }
  }
  
  self_.device.command = "";
  swarm_.command = "";
  self_.device.command_args.resize (0);
  swarm_.command_args.resize (0);

  return return_value;
}

int
gams::controllers::Base::analyze (void)
{
  int return_value (0);

  if (platform_)
    return_value |= platform_->analyze ();

  return_value |= system_analyze ();

  if (algorithm_)
    return_value |= algorithm_->analyze ();

  if (accents_.size () > 0)
  {
    for (algorithms::Algorithms::iterator i = accents_.begin ();
      i != accents_.end (); ++i)
    {
      (*i)->analyze ();
    }
  }

  return return_value;
}

int
gams::controllers::Base::plan (void)
{
  int return_value (0);
  
  if (algorithm_)
    return_value |= algorithm_->plan ();
  
  if (accents_.size () > 0)
  {
    for (algorithms::Algorithms::iterator i = accents_.begin ();
      i != accents_.end (); ++i)
    {
      (*i)->plan ();
    }
  }

  return return_value;
}

int
gams::controllers::Base::execute (void)
{
  int return_value (0);
  
  if (algorithm_)
    return_value |= algorithm_->execute ();

  if (accents_.size () > 0)
  {
    for (algorithms::Algorithms::iterator i = accents_.begin ();
      i != accents_.end (); ++i)
    {
      (*i)->plan ();
    }
  }

  return return_value;
}

int
gams::controllers::Base::run (double period, double max_runtime)
{
  // return value
  int return_value (0);

  // loop every period until a max run time has been reached
  ACE_Time_Value current = ACE_OS::gettimeofday ();  
  ACE_Time_Value max_wait, sleep_time, next_epoch;
  ACE_Time_Value poll_frequency, last = current;
  
  if (period > 0)
  {
    max_wait.set (max_runtime);
    max_wait = current + max_wait;
    
    poll_frequency.set (period);
    next_epoch = current + poll_frequency;

    unsigned int iterations = 0;
    while (current < max_wait)
    {
      cerr << "iteration: " << iterations << endl;

      // some debug output, just not every iteration
      if ((++iterations) % 15 == 0)
        knowledge_.print();

      // send modified values through network
      knowledge_.send_modifieds();

      // return value should be last return value of mape loop
      return_value = 0;
      return_value |= monitor ();
      return_value |= analyze ();
      return_value |= plan ();
      return_value |= execute ();
    
      current = ACE_OS::gettimeofday ();

      if (current < next_epoch)
      {
        Madara::Utility::sleep (next_epoch - current);  
      }

      next_epoch = next_epoch + poll_frequency;
    }
  }

  return return_value;
}

void
gams::controllers::Base::init_accent (const std::string & algorithm,
  const Madara::Knowledge_Vector & args)
{
  if (algorithm == "")
  {
    std::cerr << "Accent is empty. ";
    std::cerr << "Please provide a supported accent algorithm.\n\n";
  }
  else
  {
    // create new accent pointer and algorithm factory
    algorithms::Base * new_accent (0);
    algorithms::Factory factory (&knowledge_, &sensors_,
      platform_, &self_, &devices_);

    new_accent = factory.create (algorithm, args);

    if (new_accent)
    {
      accents_.push_back (new_accent);
    }
  }
}

void gams::controllers::Base::clear_accents (void)
{
  for (unsigned int i = 0; i < accents_.size (); ++i)
  {
    delete accents_[i];
  }

  accents_.clear ();
}
void
gams::controllers::Base::init_algorithm (
const std::string & algorithm, const Madara::Knowledge_Vector & args)
{
  // initialize the algorithm
  
  if (algorithm == "")
  {
    std::cerr << "Algorithm is empty. Here is a list of supported algorithms.";
    std::cerr << "\n\n";
    std::cerr << "SUPPORTED ALGORITHMS:\n";
    std::cerr << "  bridge | bridging\n";
    std::cerr << "  random area coverage\n";
    std::cerr << "  priotized area coverage\n";
  }
  else
  {
    delete algorithm_;
    algorithms::Factory factory (&knowledge_, &sensors_,
      platform_, &self_, &devices_);
    algorithm_ = factory.create (algorithm, args);
  }
}

void
gams::controllers::Base::init_platform (
  const std::string & platform,
  const Madara::Knowledge_Vector & args)
{
  // initialize the platform
  
  if (platform == "")
  {
    std::cerr << "Platform is empty. Here is a list of supported platforms.";
    std::cerr << "\n\n";
    std::cerr << "SUPPORTED PLATFORMS:\n";
    std::cerr << "  drone-rk\n";
    std::cerr << "  vrep\n";
  }
  else
  {
    delete platform_;
    platforms::Factory factory (&knowledge_, &sensors_, &platforms_, &self_);
    platform_ = factory.create (platform);

    if (algorithm_)
      algorithm_->set_platform (platform_);
  }
}

void
gams::controllers::Base::init_vars (
  const Integer & id,
  const Integer & processes)
{
  // initialize the devices, swarm, and self variables
  variables::init_vars (devices_, knowledge_, processes);
  swarm_.init_vars (knowledge_, processes);
  self_.init_vars (knowledge_, id);
}