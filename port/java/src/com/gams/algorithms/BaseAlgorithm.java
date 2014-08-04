/**
 * Copyright (c) 2013-2014 Carnegie Mellon University. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *  this list of conditions and the following acknowledgments and disclaimers.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution.
 * 
 * 3. The names �Carnegie Mellon University,� "SEI� and/or �Software
 *  Engineering Institute" shall not be used to endorse or promote products
 *  derived from this software without prior written permission. For written
 *  permission, please contact permission@sei.cmu.edu.
 * 
 * 4. Products derived from this software may not be called "SEI" nor may "SEI"
 *  appear in their names without prior written permission of
 *  permission@sei.cmu.edu.
 * 
 * 5. Redistributions of any form whatsoever must retain the following
 *  acknowledgment:
 * 
 *  This material is based upon work funded and supported by the Department
 *  of Defense under Contract No. FA8721-05-C-0003 with Carnegie Mellon
 *  University for the operation of the Software Engineering Institute, a
 *  federally funded research and development center. Any opinions,
 *  findings and conclusions or recommendations expressed in this material
 *  are those of the author(s) and do not necessarily reflect the views of
 *  the United States Department of Defense.
 * 
 *  NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING
 *  INSTITUTE MATERIAL IS FURNISHED ON AN �AS-IS� BASIS. CARNEGIE MELLON
 *  UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR
 *  IMPLIED, AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF
 *  FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS
 *  OBTAINED FROM USE OF THE MATERIAL. CARNEGIE MELLON UNIVERSITY DOES
 *  NOT MAKE ANY WARRANTY OF ANY KIND WITH RESPECT TO FREEDOM FROM PATENT,
 *  TRADEMARK, OR COPYRIGHT INFRINGEMENT.
 * 
 *  This material has been approved for public release and unlimited
 *  distribution.
 **/
package com.gams.algorithms;

import com.gams.GamsJNI;
import com.madara.KnowledgeBase;
import com.gams.platforms.BasePlatform;
import com.gams.variables.Self;
import com.gams.variables.Algorithm;
import com.gams.controllers.BaseController;

/**
 * Base class that should be extended when creating a Java algorithm for
 * usage in the GAMS controller
 */
public abstract class BaseAlgorithm extends GamsJNI implements AlgorithmInterface
{
  private native long jni_Base(Object sub);
  private native long jni_getKnowledgeBase(long cptr);
  private native long jni_getSelf(long cptr);
  private native long jni_getPlatform(long cptr);
  private native long jni_getAlgorithmStatus(long cptr);

  public BaseAlgorithm(BaseAlgorithm sub)
  {
    setCPtr(jni_Base(sub));
  }

  /**
   * Initialize the platform with controller variables. Use this
   * method to synchronize user-defined algorithms with the controller.
   **/
  public void init (BaseController controller)
  {
    controller.initVars (this);
    platform = new BasePlatform(jni_getPlatform(getCPtr()));
    knowledge = KnowledgeBase.fromPointer(jni_getKnowledgeBase(getCPtr()),false);
    self = Self.fromPointer(jni_getSelf(getCPtr()),false);
    status = Algorithm.fromPointer(jni_getAlgorithmStatus(getCPtr()),false);
  }
  
  /**
   * The controller's current knowledge base
   **/
  KnowledgeBase knowledge;
  
  /**
   * The platform currently in use by the controller
   **/
  BasePlatform platform;
  
  /**
   * Self-identifying variables like id and device properties
   **/
  Self self;
  
  /**
   * The status of the algorithm
   **/
  Algorithm status;
}

