
#include "com_gams_variables_Region.h"
#include "gams/variables/Region.h"

namespace containers = Madara::Knowledge_Engine::Containers;
namespace engine = Madara::Knowledge_Engine;
namespace variables = gams::variables;

/*
 * Class:     com_gams_variables_Region
 * Method:    jni_Region
 * Signature: ()J
 */
GAMS_Export jlong JNICALL Java_com_gams_variables_Region_jni_1Region__
  (JNIEnv * env, jobject)
{
  return (jlong) new variables::Region ();
}

/*
 * Class:     com_gams_variables_Region
 * Method:    jni_Region
 * Signature: (J)J
 */
GAMS_Export jlong JNICALL Java_com_gams_variables_Region_jni_1Region__J
  (JNIEnv * env, jobject, jlong cptr)
{
  return (jlong) new variables::Region (*(variables::Region *)cptr);
}

/*
 * Class:     com_gams_variables_Region
 * Method:    jni_freeRegion
 * Signature: (J)V
 */
GAMS_Export void JNICALL Java_com_gams_variables_Region_jni_1freeRegion
  (JNIEnv * env, jclass, jlong cptr)
{
  delete (variables::Region *) cptr;
}

/*
 * Class:     com_gams_variables_Region
 * Method:    jni_getName
 * Signature: (J)Ljava/lang/String;
 */
GAMS_Export jstring JNICALL Java_com_gams_variables_Region_jni_1getName
  (JNIEnv * env, jobject, jlong cptr)
{
  jstring result;

  variables::Region * current = (variables::Region *) cptr;
  if (current)
    result = env->NewStringUTF(current->name.c_str ());

  return result;
}

/*
 * Class:     com_gams_variables_Region
 * Method:    jni_init
 * Signature: (JJJLjava/lang/String;)V
 */
GAMS_Export void JNICALL Java_com_gams_variables_Region_jni_1init
  (JNIEnv * env, jobject, jlong cptr, jlong type, jlong context, jstring name)
{
  variables::Region * current = (variables::Region *) cptr;

  if (current)
  {
    const char * str_name = env->GetStringUTFChars(name, 0);

    if (type == 0)
    {
      engine::Knowledge_Base * kb = (engine::Knowledge_Base *) context;
      current->init_vars (*kb, str_name);
    }
    else if (type == 1)
    {
      engine::Variables * vars = (engine::Variables *) context;
      current->init_vars (*vars, str_name);
    }

    env->ReleaseStringUTFChars(name, str_name);
  }
}

/*
 * Class:     com_gams_variables_Region
 * Method:    jni_toString
 * Signature: (J)Ljava/lang/String;
 */
GAMS_Export jstring JNICALL Java_com_gams_variables_Region_jni_1toString
  (JNIEnv * env, jobject, jlong cptr)
{
  jstring result;

  variables::Region * current = (variables::Region *) cptr;
  if (current)
    result = env->NewStringUTF(current->name.c_str ());

  return result;
}

/*
 * Class:     com_gams_variables_Region
 * Method:    jni_getVertices
 * Signature: (J)J
 */
GAMS_Export jlong JNICALL Java_com_gams_variables_Region_jni_1getVertices
  (JNIEnv * env, jobject, jlong cptr)
{
  variables::Region * current = (variables::Region *) cptr;

  return (jlong) &current->vertices;
}
