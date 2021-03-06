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
 * 3. The names Carnegie Mellon University, "SEI and/or Software
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
 *      INSTITUTE MATERIAL IS FURNISHED ON AN AS-IS BASIS. CARNEGIE MELLON
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

/**
 * @file Formation_Coverage.cpp
 * @author Anton Dukeman <anton.dukeman@gmail.com>
 *
 * Definitions for Formation_Coverage class
 * - The head agent runs some area coverage algorithm
 * - Followers perform formation flying around the head agent
 **/

#include "gams/algorithms/Formation_Coverage.h"
#include "gams/algorithms/Controller_Algorithm_Factory.h"

#include <sstream>
#include <string>
#include <iostream>

#include "gams/algorithms/Algorithm_Factory.h"

using std::string;
using std::stringstream;
using std::cerr;
using std::endl;

gams::algorithms::Base_Algorithm *
gams::algorithms::Formation_Coverage_Factory::create (
  const Madara::Knowledge_Vector & args,
  Madara::Knowledge_Engine::Knowledge_Base * knowledge,
  platforms::Base_Platform * platform,
  variables::Sensors * sensors,
  variables::Self * self,
  variables::Devices * devices)
{
  Base_Algorithm * result (0);
  
  if (knowledge && sensors && platform && self && args.size () >= 5)
  {
    Madara::Knowledge_Vector cover_args;
    for(size_t i = 5; i < args.size(); ++i)
      cover_args.push_back (args[i]);

    result = new Formation_Coverage (
      args[0] /* head */,
      args[1] /* offset */,
      args[2] /* members */,
      args[3] /* modifier */,
      args[4] /* coverage */,
      cover_args,
      knowledge, platform, sensors, self);
  }

  return result;
}

gams::algorithms::Formation_Coverage::Formation_Coverage (
  const Madara::Knowledge_Record & head_id,
  const Madara::Knowledge_Record & offset,
  const Madara::Knowledge_Record & members,
  const Madara::Knowledge_Record & modifier,
  const Madara::Knowledge_Record & coverage,
  const Madara::Knowledge_Vector & cover_args,
  Madara::Knowledge_Engine::Knowledge_Base * knowledge,
  platforms::Base_Platform * platform,
  variables::Sensors * sensors,
  variables::Self * self)
  : Base_Algorithm (knowledge, platform, sensors, self), is_covering_(false)
{
  cerr << "Creating Formation Coverage Algorithm" << endl;

  Madara::Knowledge_Record rec("0,0,0");
  my_formation_ = new Formation_Flying (head_id, offset, rec, members, 
    modifier, knowledge, platform, sensors, self);

  if (my_formation_->is_head ())
  {
    Controller_Algorithm_Factory factory (knowledge, sensors, platform, self);
    Base_Algorithm * base_algo = factory.create (coverage.to_string (), cover_args);
    head_algo_ = dynamic_cast<area_coverage::Base_Area_Coverage*>(base_algo);

    cerr << "Creating Area Coverage Algorithm" << endl;

    stringstream head_destination_str;
    head_destination_str << "device." << self->id.to_integer () << ".destination";
    string dest_str = head_destination_str.str ();
    head_destination_.set_name(dest_str, *knowledge, 3);
  }
}

gams::algorithms::Formation_Coverage::~Formation_Coverage ()
{
  delete my_formation_;
  delete head_algo_;
}

void
gams::algorithms::Formation_Coverage::operator= (const Formation_Coverage & rhs)
{
}
      
int
gams::algorithms::Formation_Coverage::analyze (void)
{
  if (my_formation_->is_head ())
  {
    if (is_covering_)
    {
      head_algo_->analyze ();
    }
    else
    {
      is_covering_ = my_formation_->analyze ();
    }
  }
  else // follower
  {
    my_formation_->analyze ();
  }

  return 0;
}
      
int
gams::algorithms::Formation_Coverage::execute (void)
{
  if (my_formation_->is_head ())
  {
    if (is_covering_)
      head_algo_->execute ();
  }
  else // follower
  {
    my_formation_->execute();
  }

  return 0;
}

int
gams::algorithms::Formation_Coverage::plan (void)
{
  if (my_formation_->is_head ())
  {
    if (is_covering_)
      head_algo_->plan ();
    head_algo_->get_next_position ().to_container (head_destination_);
  }
  else // follower
  {
    my_formation_->plan ();
  }

  return 0;
}
