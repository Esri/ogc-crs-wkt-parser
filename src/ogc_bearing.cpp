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
/* BEARING object                                                            */
/* ------------------------------------------------------------------------- */

#include "ogc_common.h"

namespace OGC {

const char * ogc_bearing :: obj_kwd() { return OGC_OBJ_KWD_BEARING; }

bool ogc_bearing :: is_kwd(const char * kwd)
{
   return ogc_string::is_equal(kwd, obj_kwd());
}

/*------------------------------------------------------------------------
 * create
 */
ogc_bearing * ogc_bearing :: create(
   double        value,
   ogc_angunit * angunit,
   ogc_error *   err)
{
   ogc_bearing * p = OGC_NULL;
   bool bad = false;

   /*---------------------------------------------------------
    * error checks
    */
   if ( value <= -360.0 || value >= 360.0 )
   {
      ogc_error::set(err, OGC_ERR_INVALID_BEARING_VALUE, obj_kwd(), value);
      bad = true;
   }

   /*---------------------------------------------------------
    * create the object
    */
   if ( !bad )
   {
      p = new (std::nothrow) ogc_bearing();
      if ( p == OGC_NULL )
      {
         ogc_error::set(err, OGC_ERR_NO_MEMORY, obj_kwd());
         return p;
      }

      p->_obj_type = OGC_OBJ_TYPE_BEARING;
      p->_value    = value;
      p->_angunit  = angunit;
   }

   return p;
}

/*------------------------------------------------------------------------
 * destroy
 */
ogc_bearing :: ~ogc_bearing()
{
   ogc_angunit :: destroy( _angunit );
}

void ogc_bearing :: destroy(
   ogc_bearing * obj)
{
   if ( obj != OGC_NULL )
   {
      delete obj;
   }
}

/*------------------------------------------------------------------------
 * object from tokens
 */
ogc_bearing * ogc_bearing :: from_tokens(
   const ogc_token * t,
   int               start,
   int *             pend,
   ogc_error *       err)
{
   const ogc_token_entry * arr;
   const char * kwd;
   bool bad = false;
   int  level;
   int  end;
   int  same;
   int  num;

   ogc_bearing * obj     = OGC_NULL;
   ogc_angunit * angunit = OGC_NULL;
   double value;

   /*---------------------------------------------------------
    * sanity checks
    */
   if ( t == OGC_NULL )
   {
      ogc_error::set(err, OGC_ERR_WKT_EMPTY_STRING, obj_kwd());
      return OGC_NULL;
   }
   arr = t->_arr;

   if ( start < 0 || start >= t->_num )
   {
      ogc_error::set(err, OGC_ERR_WKT_INDEX_OUT_OF_RANGE, obj_kwd(), start);
      return OGC_NULL;
   }
   kwd = arr[start].str;

   if ( !is_kwd(kwd) )
   {
      ogc_error::set(err, OGC_ERR_WKT_INVALID_KEYWORD, obj_kwd(), kwd);
      return OGC_NULL;
   }

   /*---------------------------------------------------------
    * Get the level for this object,
    * the number of tokens at that level,
    * and the total number of tokens.
    */
   level = arr[start].lvl;
   for (end = start+1; end < t->_num; end++)
   {
      if ( arr[end].lvl <= level )
         break;
   }

   if ( pend != OGC_NULL )
      *pend = end;
   num = (end - start);

   for (same = 0; same < num; same++)
   {
      if ( arr[start+same+1].lvl != level+1 || arr[start+same+1].idx == 0 )
         break;
   }

   /*---------------------------------------------------------
    * There must be 1 token: BEARING[ value ...
    */
   if ( same < 1 )
   {
      ogc_error::set(err, OGC_ERR_WKT_INSUFFICIENT_TOKENS, obj_kwd(), same);
      return OGC_NULL;
   }

   if ( same > 1 && get_strict_parsing() )
   {
      ogc_error::set(err, OGC_ERR_WKT_TOO_MANY_TOKENS,     obj_kwd(), same);
      return OGC_NULL;
   }

   start++;

   /*---------------------------------------------------------
    * Process all non-object tokens.
    * They come first and are syntactically fixed.
    */
   value = ogc_string::atod( arr[start++].str );

   /*---------------------------------------------------------
    * Now process all sub-objects
    */
   int  next = 0;
   for (int i = start; i < end; i = next)
   {
      if ( ogc_id::is_kwd(arr[i].str) )
      {
         if ( angunit != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_ANCHOR, obj_kwd());
            bad = true;
         }
         else
         {
            angunit = ogc_angunit::from_tokens(t, i, &next, err);
            if ( angunit == OGC_NULL )
               bad = true;
         }
         continue;
      }

      /* unknown object, skip over it */
      for (next = i+1; next < end; next++)
      {
         if ( (arr[next].lvl <= arr[i].lvl) )
            break;
      }
   }

   /*---------------------------------------------------------
    * Create the object
    */
   if ( !bad )
   {
      obj = create(value, angunit, err);
   }

   if ( obj == OGC_NULL )
   {
      ogc_angunit :: destroy( angunit );
   }

   return obj;
}

/*------------------------------------------------------------------------
 static object from WKT
 */
ogc_bearing * ogc_bearing :: from_wkt(
   const char * wkt,
   ogc_error *  err)
{
   ogc_bearing * obj = OGC_NULL;
   ogc_token t;

   if ( t.tokenize(wkt, obj_kwd(), err) )
   {
      obj = from_tokens(&t, 0, OGC_NULL, err);
   }

   return obj;
}

bool ogc_bearing :: to_wkt(
   const ogc_bearing * obj,
   char      buffer[],
   int       options,
   size_t    buflen)
{
   if ( buffer == OGC_NULL )
      return false;
   *buffer = 0;

   if ( obj == OGC_NULL )
      return true;

   return obj->to_wkt(buffer, options, buflen);
}

/*------------------------------------------------------------------------
 * object to WKT
 */
bool ogc_bearing :: to_wkt(
   char      buffer[],
   int       options,
   size_t    buflen) const
{
   OGC_TBUF      buf_hdr;
   OGC_TBUF      buf_angunit;
   OGC_NBUF      buf_value;
   int           opts  =  (options | OGC_WKT_OPT_INTERNAL);
   size_t        len   = 0;
   bool          rc    = true;
   const char *  opn   = "[";
   const char *  cls   = "]";
   const char *  kwd   = obj_kwd();

   if ( (options & OGC_WKT_OPT_PARENS) != 0 )
   {
      opn = "(";
      cls = ")";
   }

   if ( (opts & OGC_WKT_OPT_TOP_ID_ONLY) != 0 )
      opts |= OGC_WKT_OPT_NO_IDS;

   if ( buffer == OGC_NULL )
      return false;
   *buffer = 0;

   if ( (options & OGC_WKT_OPT_OLD_SYNTAX) != 0 )
      return true;

   ogc_string :: dtoa(_value, buf_value);

   rc &= ogc_angunit :: to_wkt(_angunit, buf_angunit, opts, OGC_TBUF_MAX);

   sprintf(buf_hdr, "%s%s%s",
      kwd, opn, buf_value);

   OGC_CPY_TO_BUF( buf_hdr     );
   OGC_ADD_TO_BUF( buf_angunit );
   OGC_CPY_TO_BUF( cls         );

   if ( (options & OGC_WKT_OPT_INTERNAL) == 0 &&
        (options & OGC_WKT_OPT_EXPAND)   != 0 )
   {
      rc &= ogc_utils :: expand_wkt(buffer, buffer, "", options, buflen);
   }

   return rc;
}

/*------------------------------------------------------------------------
 * clone
 */
ogc_bearing * ogc_bearing :: clone(const ogc_bearing * obj)
{
   if ( obj == OGC_NULL )
      return OGC_NULL;
   return obj->clone();
}

ogc_bearing * ogc_bearing :: clone() const
{
   ogc_angunit * angunit = ogc_angunit :: clone( _angunit );

   ogc_bearing * p = create(_value,
                            angunit,
                            OGC_NULL);
   if ( p == OGC_NULL )
   {
      ogc_angunit :: destroy( angunit );
   }

   return p;
}

/*------------------------------------------------------------------------
 * compare for computational equality
 */
bool ogc_bearing :: is_equal(
   const ogc_bearing * p1,
   const ogc_bearing * p2)
{
   if ( p1 == OGC_NULL && p2 == OGC_NULL ) return true;
   if ( p1 == OGC_NULL || p2 == OGC_NULL ) return false;

   if ( !ogc_macros  :: eq      ( p1->value(),   p2->value()   ) ||
        !ogc_angunit :: is_equal( p1->angunit(), p2->angunit() ) )
   {
      return false;
   }

   return true;
}

bool ogc_bearing :: is_equal(
   const ogc_bearing * p) const
{
   return is_equal(this, p);
}

/*------------------------------------------------------------------------
 * compare
 */
bool ogc_bearing :: is_identical(
   const ogc_bearing * p1,
   const ogc_bearing * p2)
{
   if ( p1 == OGC_NULL && p2 == OGC_NULL ) return true;
   if ( p1 == OGC_NULL || p2 == OGC_NULL ) return false;

   if ( !ogc_macros  :: eq          ( p1->value(),   p2->value()   ) ||
        !ogc_angunit :: is_identical( p1->angunit(), p2->angunit() ) )
   {
      return false;
   }

   return true;
}

bool ogc_bearing :: is_identical(
   const ogc_bearing * p) const
{
   return is_identical(this, p);
}

} /* namespace OGC */
