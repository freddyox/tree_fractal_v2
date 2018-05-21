// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME mydict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "Tree.hh"
#include "Box.hh"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *Tree_Dictionary();
   static void Tree_TClassManip(TClass*);
   static void delete_Tree(void *p);
   static void deleteArray_Tree(void *p);
   static void destruct_Tree(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Tree*)
   {
      ::Tree *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Tree));
      static ::ROOT::TGenericClassInfo 
         instance("Tree", "Tree.hh", 18,
                  typeid(::Tree), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &Tree_Dictionary, isa_proxy, 0,
                  sizeof(::Tree) );
      instance.SetDelete(&delete_Tree);
      instance.SetDeleteArray(&deleteArray_Tree);
      instance.SetDestructor(&destruct_Tree);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Tree*)
   {
      return GenerateInitInstanceLocal((::Tree*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Tree*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *Tree_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Tree*)0x0)->GetClass();
      Tree_TClassManip(theClass);
   return theClass;
   }

   static void Tree_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *Box_Dictionary();
   static void Box_TClassManip(TClass*);
   static void delete_Box(void *p);
   static void deleteArray_Box(void *p);
   static void destruct_Box(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Box*)
   {
      ::Box *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Box));
      static ::ROOT::TGenericClassInfo 
         instance("Box", "Box.hh", 4,
                  typeid(::Box), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &Box_Dictionary, isa_proxy, 0,
                  sizeof(::Box) );
      instance.SetDelete(&delete_Box);
      instance.SetDeleteArray(&deleteArray_Box);
      instance.SetDestructor(&destruct_Box);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Box*)
   {
      return GenerateInitInstanceLocal((::Box*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Box*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *Box_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Box*)0x0)->GetClass();
      Box_TClassManip(theClass);
   return theClass;
   }

   static void Box_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrapper around operator delete
   static void delete_Tree(void *p) {
      delete ((::Tree*)p);
   }
   static void deleteArray_Tree(void *p) {
      delete [] ((::Tree*)p);
   }
   static void destruct_Tree(void *p) {
      typedef ::Tree current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Tree

namespace ROOT {
   // Wrapper around operator delete
   static void delete_Box(void *p) {
      delete ((::Box*)p);
   }
   static void deleteArray_Box(void *p) {
      delete [] ((::Box*)p);
   }
   static void destruct_Box(void *p) {
      typedef ::Box current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Box

namespace {
  void TriggerDictionaryInitialization_mydict_Impl() {
    static const char* headers[] = {
"Tree.hh",
"Box.hh",
0
    };
    static const char* includePaths[] = {
"/home/obrecht/Documents/ROOT/build/include",
"/home/obrecht/Documents/Projects_New/root/tree_program/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "mydict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$Tree.hh")))  Tree;
class __attribute__((annotate("$clingAutoload$Box.hh")))  Box;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "mydict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "Tree.hh"
#include "Box.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"Box", payloadCode, "@",
"Tree", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("mydict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_mydict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_mydict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_mydict() {
  TriggerDictionaryInitialization_mydict_Impl();
}
