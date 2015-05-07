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
/* TIME CRS (Temporal CRS) object                                            */
/* ------------------------------------------------------------------------- */

#include "ogc_common.h"

namespace OGC {

const char * ogc_base_time_crs :: obj_kwd() { return OGC_OBJ_KWD_BASE_TIME_CRS; }

bool ogc_base_time_crs :: is_kwd(const char * kwd)
{
   return ogc_string::is_equal(kwd, obj_kwd());
}

/*------------------------------------------------------------------------
 * create
 */
ogc_base_time_crs * ogc_base_time_crs :: create(
   const char *      name,
   ogc_time_datum *  datum,
   ogc_unit *        unit,
   ogc_error *       err)
{
   ogc_base_time_crs * p = OGC_NULL;
   bool bad = false;

   /*---------------------------------------------------------
    * error checks
    */
   if ( name == OGC_NULL )
   {
      name = "";
   }
   else
   {
      int len = ogc_string::unescape_len(name);
      if ( len >= OGC_NAME_MAX )
      {
         ogc_error::set(err, OGC_ERR_NAME_TOO_LONG, obj_kwd(), len);
         bad = true;
      }
   }

   if ( datum == OGC_NULL )
   {
      ogc_error::set(err, OGC_ERR_MISSING_DATUM, obj_kwd());
      bad = true;
   }

   /*---------------------------------------------------------
    * create the object
    */
   if ( !bad )
   {
      p = new (std::nothrow) ogc_base_time_crs();
      if ( p == OGC_NULL )
      {
         ogc_error::set(err, OGC_ERR_NO_MEMORY, obj_kwd());
         return p;
      }

      ogc_string::unescape_str(p->_name, name, OGC_NAME_MAX);
      p->_obj_type = OGC_OBJ_TYPE_BASE_TIME_CRS;
      p->_visible  = true;
      p->_crs_type = OGC_CRS_TYPE_BASE_TIME;
      p->_datum    = datum;
      p->_unit     = unit;
   }

   return p;
}

/*------------------------------------------------------------------------
 * destroy
 */
ogc_base_time_crs :: ~ogc_base_time_crs()
{
}

ogc_base_time_crs * ogc_base_time_crs :: destroy(
   ogc_base_time_crs * obj)
{
   if ( obj != OGC_NULL )
   {
#if 0 /* this will be deleted by the parent */
      delete obj;
#endif
   }
   return OGC_NULL;
}

/*------------------------------------------------------------------------
 * object from tokens
 */
ogc_base_time_crs * ogc_base_time_crs :: from_tokens(
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

   ogc_base_time_crs *    obj     = OGC_NULL;
   ogc_time_datum *   datum   = OGC_NULL;
   ogc_timeunit *     unit    = OGC_NULL;
   const char * name;

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
    * There must be 1 token: BASETIMECRS[ "name" ...
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
   name = arr[start++].str;

   /*---------------------------------------------------------
    * Now process all sub-objects
    */
   int  next = 0;
   for (int i = start; i < end; i = next)
   {
      if ( ogc_time_datum::is_kwd(arr[i].str) )
      {
         if ( datum != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_DATUM, obj_kwd());
            bad = true;
         }
         else
         {
            datum = ogc_time_datum::from_tokens(t, i, &next, err);
            if ( datum == OGC_NULL )
               bad = true;
         }
         continue;
      }

      if ( ogc_timeunit::is_kwd(arr[i].str) )
      {
         if ( unit != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_UNIT, obj_kwd());
            bad = true;
         }
         else
         {
            unit = ogc_timeunit::from_tokens(t, i, &next, err);
            if ( unit == OGC_NULL )
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
      obj = create(name, datum, unit, err);
   }

   if ( obj == OGC_NULL )
   {
      ogc_time_datum  :: destroy( datum   );
      ogc_unit        :: destroy( unit    );
   }

   return obj;
}

/*------------------------------------------------------------------------
 * object from WKT
 */
ogc_base_time_crs * ogc_base_time_crs :: from_wkt(
   const char * wkt,
   ogc_error *  err)
{
   ogc_base_time_crs * obj = OGC_NULL;
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
bool ogc_base_time_crs :: to_wkt(
   const ogc_base_time_crs * obj,
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

bool ogc_base_time_crs :: to_wkt(
   char      buffer[],
   int       options,
   size_t    buflen) const
{
   OGC_UTF8_NAME buf_name;
   OGC_TBUF      buf_hdr;
   OGC_TBUF      buf_datum;
   OGC_TBUF      buf_unit;
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

   if ( !is_visible() )
      return true;

   if ( (opts & OGC_WKT_OPT_OLD_SYNTAX) != 0 )
      return true;

   rc &= ogc_time_datum :: to_wkt(_datum, buf_datum, opts, OGC_TBUF_MAX);
   rc &= ogc_unit       :: to_wkt(_unit,  buf_unit,  opts, OGC_TBUF_MAX);

   ogc_string::escape_str(buf_name, _name, OGC_UTF8_NAME_MAX);
   sprintf(buf_hdr, "%s%s\"%s\"",
      kwd, opn, buf_name);

   OGC_CPY_TO_BUF( buf_hdr    );
   OGC_ADD_TO_BUF( buf_datum  );
   OGC_ADD_TO_BUF( buf_unit   );
   OGC_CPY_TO_BUF( cls        );

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
ogc_base_time_crs * ogc_base_time_crs :: clone(const ogc_base_time_crs * obj)
{
   if ( obj == OGC_NULL )
      return OGC_NULL;
   return obj->clone();
}

ogc_base_time_crs * ogc_base_time_crs :: clone() const
{
   ogc_timeunit * u = reinterpret_cast<ogc_timeunit *>(_unit);

   ogc_time_datum *  datum    = ogc_time_datum :: clone( _datum   );
   ogc_timeunit *    timeunit = ogc_timeunit   :: clone( u        );

   ogc_base_time_crs * p = create(_name,
                                 datum,
                                 timeunit,
                                 OGC_NULL);
   if ( p == OGC_NULL )
   {
      ogc_time_datum :: destroy( datum    );
      ogc_timeunit   :: destroy( timeunit );
   }

   return p;
}

/*------------------------------------------------------------------------
 * compare for computational equality
 */
bool ogc_base_time_crs :: is_equal(
   const ogc_base_time_crs * p1,
   const ogc_base_time_crs * p2)
{
   if ( p1 == OGC_NULL && p2 == OGC_NULL ) return true;
   if ( p1 == OGC_NULL || p2 == OGC_NULL ) return false;

   if ( !ogc_string     :: is_equal( p1->name(),     p2->name()     ) ||
        !ogc_time_datum :: is_equal( p1->datum(),    p2->datum()    ) ||
        !ogc_timeunit   :: is_equal( p1->timeunit(), p2->timeunit() ) )
   {
      return false;
   }

   return true;
}

bool ogc_base_time_crs :: is_equal(
   const ogc_base_time_crs * p) const
{
   return is_equal(this, p);
}

/*------------------------------------------------------------------------
 * compare
 */
bool ogc_base_time_crs :: is_identical(
   const ogc_base_time_crs * p1,
   const ogc_base_time_crs * p2)
{
   if ( p1 == OGC_NULL && p2 == OGC_NULL ) return true;
   if ( p1 == OGC_NULL || p2 == OGC_NULL ) return false;

   if ( !ogc_string     :: is_equal    ( p1->name(),     p2->name()     ) ||
        !ogc_time_datum :: is_identical( p1->datum(),    p2->datum()    ) ||
        !ogc_timeunit   :: is_identical( p1->timeunit(), p2->timeunit() ) )
   {
      return false;
   }

   return true;
}

bool ogc_base_time_crs :: is_identical(
   const ogc_base_time_crs * p) const
{
   return is_identical(this, p);
}

} /* namespace OGC */
