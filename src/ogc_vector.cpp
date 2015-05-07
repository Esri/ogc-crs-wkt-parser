/* ------------------------------------------------------------------------- */
/* Copyright 2013 Esri                                                       */
/*                                                                           */
/* Licensed under the Apache License, Version 2.0 (the "License");           */
/* you may not use this file except in compliance with the License.          */
/* You may obtain a copy of the License at                                   */
/*                                                                           */
/*     http://www.apache.org/licenses/LICENSE-2.0                            */
/*                                                                           */
/* Unless required by applicable law or agreed to in writing, software       */
/* distributed under the License is distributed on an "AS IS" BASIS,         */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  */
/* See the License for the specific language governing permissions and       */
/* limitations under the License.                                            */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* Vector object and methods                                                 */
/* ------------------------------------------------------------------------- */

#include "ogc_common.h"

namespace OGC {

/*------------------------------------------------------------------------
 * destructor
 */
ogc_vector :: ~ogc_vector()
{
   if ( _array != OGC_NULL )
   {
      for (int i = 0; i < _length; i++)
      {
         _array[i] = ogc_object :: destroy(_array[i]);
      }
      delete [] _array;
   }
}

/*------------------------------------------------------------------------
 * static create
 */
ogc_vector * ogc_vector :: create(
   int capacity,
   int increment)
{
   ogc_vector * v = OGC_NULL;

   v = new (std::nothrow) ogc_vector();
   if ( v == OGC_NULL )
   {
      v = OGC_NULL;
   }

   if ( v != OGC_NULL )
   {
      v->_length    = 0;
      v->_capacity  = capacity;
      v->_increment = increment;
      v->_array     = OGC_NULL;

      if ( capacity > 0 )
      {
         v->_array = new (std::nothrow) ogc_object * [capacity];
         if ( v->_array == OGC_NULL )
         {
            v->_capacity = 0;
         }
      }
   }

   return v;
}

/*------------------------------------------------------------------------
 * static clone
 */
ogc_vector * ogc_vector :: clone(const ogc_vector * obj)
{
   if ( obj == OGC_NULL )
      return OGC_NULL;
   return obj->clone();
}

/*------------------------------------------------------------------------
 * clone
 */
ogc_vector * ogc_vector :: clone() const
{
   ogc_vector * v = create(_capacity, _increment);

   if ( v != OGC_NULL )
   {
      for (int i = 0; i < _length; i++)
      {
         ogc_object * p = _array[i]->clone();
         if ( p != OGC_NULL )
            v->add(p);
      }
   }

   return v;
}

/*------------------------------------------------------------------------
 * static destroy
 */
ogc_vector * ogc_vector :: destroy( ogc_vector * obj )
{
   if ( obj != OGC_NULL )
   {
      delete obj;
   }
   return OGC_NULL;
}

/*------------------------------------------------------------------------
 * get the nth element
 */
ogc_object * ogc_vector :: get(int n) const
{
   if ( n < 0 || n >= _length )
      return OGC_NULL;

   return _array[n];
}

/*------------------------------------------------------------------------
 * add an element
 */
int ogc_vector :: add(ogc_object * element)
{
   if ( _length == _capacity )
   {
      if ( _increment == 0 )
         return -1;

      int new_capacity = _capacity + _increment;
      ogc_object ** new_array = new (std::nothrow) ogc_object * [new_capacity];

      if ( new_array == OGC_NULL )
      {
         return -1;
      }

      for (int i = 0; i < _length; i++)
        new_array[i] = _array[i];
      delete [] _array;

      _array    = new_array;
      _capacity = new_capacity;
   }

   _array[_length++] = element;
   return 0;
}

/*------------------------------------------------------------------------
 * sort the array
 */
void ogc_vector :: sort(
   OGC_COMPARE_RTN * rtn)
{
   if ( _length > 1 )
   {
      qsort(_array, _length, sizeof(*_array), rtn);
   }
}

/*------------------------------------------------------------------------
 * find an entry in the array
 */
void * ogc_vector :: find(
   ogc_object *      key,
   bool              sorted,
   OGC_COMPARE_RTN * rtn) const
{
   ogc_object * p = OGC_NULL;

   if ( _length > 0 )
   {
#if !defined(OGC_NO_BSEARCH)
      if ( sorted )
      {
         p = reinterpret_cast<ogc_object *>(
            bsearch(key, _array, _length, sizeof(*_array), rtn) );
      }
      else
#endif
      {
         for (int i = 0; i < _length; i++)
         {
            int rc = (rtn)(key, _array[i]);

            if ( rc == 0 )
            {
               p = _array[i];
               break;
            }

            if ( sorted && rc < 0 )
            {
               break;
            }
         }
      }
   }

   return p;
}

/*------------------------------------------------------------------------
 * static determine whether two vectors are equal
 *
 * TODO: Take into account that the vectors may contain the same
 *       objects but they may be in a different order.
 */
bool ogc_vector :: is_equal(
   const ogc_vector * v1,
   const ogc_vector * v2)
{
   if ( v1 == OGC_NULL && v2 == OGC_NULL )  return true;
   if ( v1 == OGC_NULL || v2 == OGC_NULL )  return false;
   if ( v1->length()   != v2->length()   )  return false;

   for (int i = 0; i < v1->length(); i++)
   {
      if ( !ogc_object :: is_equal(v1->get(i), v2->get(i)) )
         return false;
   }

   return true;
}

/*------------------------------------------------------------------------
 * determine whether two vectors are equal
 */
bool ogc_vector :: is_equal(
   const ogc_vector * v) const
{
   return is_equal(this, v);
}

/*------------------------------------------------------------------------
 * static determine whether two vectors are identical
 *
 * TODO: Take into account that the vectors may contain the same
 *       objects but they may be in a different order.
 */
bool ogc_vector :: is_identical(
   const ogc_vector * v1,
   const ogc_vector * v2)
{
   if ( v1 == OGC_NULL && v2 == OGC_NULL )  return true;
   if ( v1 == OGC_NULL || v2 == OGC_NULL )  return false;
   if ( v1->length()   != v2->length()   )  return false;

   for (int i = 0; i < v1->length(); i++)
   {
      if ( !ogc_object :: is_identical(v1->get(i), v2->get(i)) )
         return false;
   }

   return true;
}

/*------------------------------------------------------------------------
 * determine whether two vectors are identical
 */
bool ogc_vector :: is_identical(
   const ogc_vector * v) const
{
   return is_identical(this, v);
}

} /* namespace OGC */
