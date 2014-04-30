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
#include "Swarm.h"

typedef  Madara::Knowledge_Record::Integer  Integer;


gams::variables::Swarm::Swarm ()
{
}

gams::variables::Swarm::~Swarm ()
{
}

void
gams::variables::Swarm::operator= (const Swarm & rhs)
{
  if (this != &rhs)
  {
    this->command = rhs.command;
    this->command_args = rhs.command_args;
    this->min_alt = rhs.min_alt;
  }
}


void
gams::variables::Swarm::init_vars (
  Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  Madara::Knowledge_Engine::Knowledge_Update_Settings keep_local (true);

  // swarm commands are prefixed with "swarm.movement_command"
  std::string prefix ("swarm.command");

  // initialize the variable containers
  min_alt.set_name ("swarm.min_alt", knowledge);
  command.set_name (prefix, knowledge);
  command_args.set_name (prefix + ".size", knowledge);

  // keep certain varaible changes as local only
  command.set_settings (keep_local);
  command_args.set_settings (keep_local);
}

void
gams::variables::Swarm::init_vars (
  Madara::Knowledge_Engine::Variables & knowledge)
{
  Madara::Knowledge_Engine::Knowledge_Update_Settings keep_local (true);

  // swarm commands are prefixed with "swarm.movement_command"
  std::string prefix ("swarm.command");

  // initialize the variable containers
  min_alt.set_name ("swarm.min_alt", knowledge);
  command.set_name (prefix, knowledge);
  command_args.set_name (prefix + ".size", knowledge);

  // keep certain varaible changes as local only
  command.set_settings (keep_local);
  command_args.set_settings (keep_local);
}

void gams::variables::init_vars (Swarm & variables,
      Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  variables.init_vars (knowledge);
}