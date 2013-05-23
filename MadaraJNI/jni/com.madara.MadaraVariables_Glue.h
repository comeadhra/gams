/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/

#ifndef _Included_com_madara_MadaraVariables_Glue
#define _Included_com_madara_MadaraVariables_Glue

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     com_madara_MadaraVariables
 * Method:    jni_evaluate
 * Signature: (JJJ)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_MadaraVariables_jni_1evaluate__JJJ
  (JNIEnv *, jobject, jlong, jlong, jlong);

/*
 * Class:     com_madara_MadaraVariables
 * Method:    jni_compile
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_MadaraVariables_jni_1compile
  (JNIEnv *, jobject, jlong, jstring);
  
/*
 * Class:     com_madara_MadaraVariables
 * Method:    jni_get
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_MadaraVariables_jni_1get
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     com_madara_MadaraVariables
 * Method:    jni_set
 * Signature: (JLjava/lang/String;J)V
 */
JNIEXPORT void JNICALL Java_com_madara_MadaraVariables_jni_1set
  (JNIEnv *, jobject, jlong, jstring, jlong);

#ifdef __cplusplus
}
#endif
#endif