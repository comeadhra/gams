/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/
 
#ifndef _SENSORS_MODULE_H
#define _SENSORS_MODULE_H

#include <stdlib.h>
#include "madara/knowledge_engine/Knowledge_Base.h"

namespace SMASH
{
	namespace Sensors
	{
		void initialize(Madara::Knowledge_Engine::Knowledge_Base&);
		std::string main_logic();
	}
}

#endif