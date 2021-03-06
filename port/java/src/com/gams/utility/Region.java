/*********************************************************************
 * Copyright (c) 2013-2015 Carnegie Mellon University. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following acknowledgments and disclaimers.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 
 * 3. The names "Carnegie Mellon University," "SEI" and/or
 * "Software Engineering Institute" shall not be used to endorse or promote
 * products derived from this software without prior written permission. For
 * written permission, please contact permission@sei.cmu.edu.
 * 
 * 4. Products derived from this software may not be called "SEI" nor may "SEI"
 * appear in their names without prior written permission of
 * permission@sei.cmu.edu.
 *
 * 5. Redistributions of any form whatsoever must retain the following
 * acknowledgment:
 *
 * This material is based upon work funded and supported by the Department of
 * Defense under Contract No. FA8721-05-C-0003 with Carnegie Mellon University
 * for the operation of the Software Engineering Institute, a federally funded
 * research and development center. Any opinions, findings and conclusions or
 * recommendations expressed in this material are those of the author(s) and
 * do not necessarily reflect the views of the United States Department of
 * Defense.
 * 
 * NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING
 * INSTITUTE MATERIAL IS FURNISHED ON AN "AS-IS" BASIS. CARNEGIE MELLON
 * UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR IMPLIED,
 * AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF FITNESS FOR
 * PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS OBTAINED FROM USE OF THE
 * MATERIAL. CARNEGIE MELLON UNIVERSITY DOES NOT MAKE ANY WARRANTY OF ANY KIND
 * WITH RESPECT TO FREEDOM FROM PATENT, TRADEMARK, OR COPYRIGHT INFRINGEMENT.
 * 
 * This material has been approved for public release and unlimited
 * distribution.
 * 
 * @author James Edmondson <jedmondson@gmail.com>
 *********************************************************************/
package com.gams.utility;

import com.gams.GamsJNI;
import com.madara.containers.StringVector;

/**
 * A utility class that acts as a facade for a region
 **/
public class Region extends GamsJNI
{
  private native long jni_Region();
  private static native void jni_freeRegion(long cptr);
  private native void jni_fromContainer(long cptr,long container); 
  private native java.lang.String jni_toString(long cptr);
  private native void jni_addGpsVertex(long cptr, long coord); 
  private native long[] jni_getVertices(long cptr); 
  private native double jni_getArea(long cptr); 
  private native long jni_getBoundingBox(long cptr); 
  private native boolean jni_containsGps(long cptr, long coord);  
  private native double jni_getGpsDistance(long cptr, long coord); 
  private native double jni_getMaxAlt(long cptr); 
  private native double jni_getMinAlt(long cptr); 
  private native double jni_getMaxLat(long cptr); 
  private native double jni_getMinLat(long cptr); 
  private native double jni_getMaxLong(long cptr); 
  private native double jni_getMinLong(long cptr); 
  
  private boolean manageMemory = true;

  /**
   * Default constructor
   **/
  public Region()
  {
    setCPtr(jni_Region());
  }

  /**
   * Converts the position into a string
   * @return position as a string
   **/
  public java.lang.String toString()
  {
    return jni_toString(getCPtr());
  }

  
  /**
   * Adds a GPS vertex to this region
   * @param   point     point to check
   **/
  public void addVertex(GpsPosition point)
  {
    jni_addGpsVertex(getCPtr(),point.getCPtr());
  }
  
  /**
   * Gets area of the region
   * @return area of this region
   **/
  public double getArea()
  {
    return jni_getArea(getCPtr()); 
  }
  
  /**
   * Gets bounding box
   * @return Region object corresponding to bounding box
   **/
  public Region getBoundingBox()
  {
    Region result = new Region();
    result.setCPtr(jni_getBoundingBox(getCPtr()));
    return result;
  }
  
  /**
   * Checks to see if the point is contained in this region
   * @param   point     point to check
   * @return 0 if in region, otherwise distance from region
   **/
  public boolean contains(GpsPosition point)
  {
    return jni_containsGps(getCPtr(),point.getCPtr());
  } 
  
  /**
   * Get distance from any point in this region
   * @param   point     point to check
   * @return 0 if in region, otherwise distance from region
   **/
  public double getDistance(GpsPosition point)
  {
    return jni_getGpsDistance(getCPtr(),point.getCPtr());
  }
  
  /**
   * Gets the GPS vertices that form the region boundary
   * @return vertices that form the region boundary
   **/
  public GpsPosition[] getVertices()
  {
    long[] vertices = jni_getVertices(getCPtr());
    GpsPosition[] result = new GpsPosition[vertices.length];
    
    if(vertices.length > 0)
    {
      result = new GpsPosition[vertices.length];
      for (int i = 0; i < vertices.length; ++i)
        result[i].fromPointer(vertices[i]);
    }
    return result;
  }
  
  /**
   * Gets the maximum altitude
   * @return maximum altitude in the region
   **/
  public double getMaxAlt()
  {
    return jni_getMaxAlt(getCPtr());
  }
  
  /**
   * Gets the minimum altitude
   * @return minimum altitude in the region
   **/
  public double getMinAlt()
  {
    return jni_getMinAlt(getCPtr());
  }
    
  /**
   * Gets the maximum latitude
   * @return maximum latitude in the region
   **/
  public double getMaxLat()
  {
    return jni_getMaxLat(getCPtr());
  }
      
  /**
   * Gets the minimum latitude
   * @return minimum latitude in the region
   **/
  public double getMinLat()
  {
    return jni_getMinLat(getCPtr());
  }
      
  /**
   * Gets the maximum longitude
   * @return maximum longitude in the region
   **/
  public double jni_getMaxLong()
  {
    return jni_getMaxLong(getCPtr());
  }
      
  /**
   * Gets the minimum longitude
   * @return minimum longitude in the region
   **/
  public double getMinLong()
  {
    return jni_getMinLong(getCPtr());
  }
  
  /**
   * Creates a java object instance from a C/C++ pointer
   *
   * @param cptr C pointer to the object
   * @return a new java instance of the underlying pointer
   */
  public static Region fromPointer(long cptr)
  {
    Region ret = new Region();
    ret.manageMemory = true;
    ret.setCPtr(cptr);
    return ret;
  }
  
  /**
   * Creates a java object instance from a C/C++ pointer
   *
   * @param cptr C pointer to the object
   * @param shouldManage  if true, manage the pointer
   * @return a new java instance of the underlying pointer
   */
  public static Region fromPointer(long cptr, boolean shouldManage)
  {
    Region ret = new Region();
    ret.manageMemory=shouldManage;
    ret.setCPtr(cptr);
    return ret;
  }

  /**
   * Helper function for copying values from a MADARA string array
   * @param source     source container to copy values from
   **/
  void fromContainer(StringVector source)
  {
    jni_fromContainer(getCPtr(),source.getCPtr()); 
  }

  /**
   * Deletes the C instantiation. To prevent memory leaks, this <b>must</b> be
   * called before an instance of WaitSettings gets garbage collected
   */
  public void free()
  {
    if(manageMemory)
    {
      jni_freeRegion(getCPtr());
    }
  }                
}

