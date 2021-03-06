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
/* OPACCURACY (Operation accuracy) object                                    */
/* ------------------------------------------------------------------------- */

#include "ogc_common.h"

namespace OGC {

const char * ogc_op_accuracy :: obj_kwd() { return OGC_OBJ_KWD_OP_ACCURACY; }

bool ogc_op_accuracy :: is_kwd(const char * kwd)
{
   return ogc_string::is_equal(kwd, obj_kwd());
}

/*------------------------------------------------------------------------
 * create
 */
ogc_op_accuracy * ogc_op_accuracy :: create(
   double        accuracy,
   ogc_error *   err)
{
   ogc_op_accuracy * p = OGC_NULL;
   bool bad = false;

   /*---------------------------------------------------------
    * error checks
    */

   /*---------------------------------------------------------
    * create the object
    */
   if ( !bad )
   {
      p = new (std::nothrow) ogc_op_accuracy();
      if ( p == OGC_NULL )
      {
         ogc_error::set(err, OGC_ERR_NO_MEMORY, obj_kwd());
         return p;
      }

      p->_obj_type = OGC_OBJ_TYPE_OP_ACCURACY;
      p->_visible  = true;
      p->_accuracy = accuracy;
   }

   return p;
}

/*------------------------------------------------------------------------
 * destroy
 */
ogc_op_accuracy :: ~ogc_op_accuracy()
{
}

ogc_op_accuracy * ogc_op_accuracy :: destroy(
   ogc_op_accuracy * obj)
{
   if ( obj != OGC_NULL )
   {
      delete obj;
   }
   return OGC_NULL;
}

/*------------------------------------------------------------------------
 * object from tokens
 */
ogc_op_accuracy * ogc_op_accuracy :: from_tokens(
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

   ogc_op_accuracy * obj     = OGC_NULL;
   double accuracy;

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
    * There must be 1 token: OPACCURACY[ accuracy ...
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
   accuracy = ogc_string::atod( arr[start++].str );

   /*---------------------------------------------------------
    * Now process all sub-objects
    */
#if 0 /* who cares? */
   int  next = 0;
   for (int i = start; i < end; i = next)
   {
      /* unknown object, skip over it */
      for (next = i+1; next < end; next++)
      {
         if ( (arr[next].lvl <= arr[i].lvl) )
            break;
      }
   }
#endif

   /*---------------------------------------------------------
    * Create the object
    */
   if ( !bad )
   {
      obj = create(accuracy, err);
   }

   if ( obj == OGC_NULL )
   {
   }

   return obj;
}

/*------------------------------------------------------------------------
 * object from WKT
 */
ogc_op_accuracy * ogc_op_accuracy :: from_wkt(
   const char * wkt,
   ogc_error *  err)
{
   ogc_op_accuracy * obj = OGC_NULL;
   ogc_token t;

   if ( t.tokenize(wkt, obj_kwd(), err) )
   {
      obj = from_tokens(&t, 0, OGC_NULL, err);
   }

   return obj;
}

/*------------------------------------------------------------------------
 * object to WKT
 */
bool ogc_op_accuracy :: to_wkt(
   const ogc_op_accuracy * obj,
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

bool ogc_op_accuracy :: to_wkt(
   char      buffer[],
   int       options,
   size_t    buflen) const
{
   OGC_TBUF     buf_hdr;
   OGC_NBUF     buf_accuracy;
   size_t       len   = 0;
   bool         rc    = true;
   const char * opn   = "[";
   const char * cls   = "]";
   const char *  kwd   = obj_kwd();

   if ( (options & OGC_WKT_OPT_PARENS) != 0 )
   {
      opn = "(";
      cls = ")";
   }

   if ( buffer == OGC_NULL )
      return false;
   *buffer = 0;

   if ( !is_visible() )
      return true;

   if ( (options & OGC_WKT_OPT_OLD_SYNTAX) != 0 )
      return true;

   ogc_string :: dtoa(_accuracy, buf_accuracy);

   sprintf(buf_hdr, "%s%s%s",
      kwd, opn, buf_accuracy);

   OGC_CPY_TO_BUF( buf_hdr );
   OGC_CPY_TO_BUF( cls     );

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
ogc_op_accuracy * ogc_op_accuracy :: clone(const ogc_op_accuracy * obj)
{
   if ( obj == OGC_NULL )
      return OGC_NULL;
   return obj->clone();
}

ogc_op_accuracy * ogc_op_accuracy :: clone() const
{
   ogc_op_accuracy * p = create(_accuracy, OGC_NULL);
   return p;
}

/*------------------------------------------------------------------------
 * compare for computational equality
 */
bool ogc_op_accuracy :: is_equal(
   const ogc_op_accuracy * p1,
   const ogc_op_accuracy * p2)
{
   if ( p1 == OGC_NULL && p2 == OGC_NULL ) return true;
   if ( p1 == OGC_NULL || p2 == OGC_NULL ) return false;

   if ( !ogc_macros :: eq( p1->accuracy(), p2->accuracy() ) )
   {
      return false;
   }

   return true;
}

bool ogc_op_accuracy :: is_equal(
   const ogc_op_accuracy * p) const
{
   return is_equal(this, p);
}

/*------------------------------------------------------------------------
 * compare
 */
bool ogc_op_accuracy :: is_identical(
   const ogc_op_accuracy * p1,
   const ogc_op_accuracy * p2)
{
   if ( p1 == OGC_NULL && p2 == OGC_NULL ) return true;
   if ( p1 == OGC_NULL || p2 == OGC_NULL ) return false;

   if ( !ogc_macros :: eq( p1->accuracy(), p2->accuracy() ) )
   {
      return false;
   }

   return true;
}

bool ogc_op_accuracy :: is_identical(
   const ogc_op_accuracy * p) const
{
   return is_identical(this, p);
}

} /* namespace OGC */
