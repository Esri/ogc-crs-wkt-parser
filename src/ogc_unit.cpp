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
/* UNIT (Generic unit) object                                                */
/* ------------------------------------------------------------------------- */

#include "ogc_common.h"

namespace OGC {

const char * ogc_unit :: obj_kwd() { return OGC_OBJ_KWD_UNIT; }

bool ogc_unit :: is_kwd(const char * kwd)
{
   return ogc_string::is_equal(kwd, obj_kwd());
}

/*------------------------------------------------------------------------
 * create
 */
ogc_unit * ogc_unit :: create(
   const char * name,
   double       factor,
   ogc_vector * ids,
   ogc_error *  err)
{
   ogc_unit * p = OGC_NULL;
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

   if ( factor <= 0.0 )
   {
      ogc_error::set(err, OGC_ERR_INVALID_UNIT_FACTOR, obj_kwd(), factor);
      bad = true;
   }

   /*---------------------------------------------------------
    * create the object
    */
   if ( !bad )
   {
      p = new (std::nothrow) ogc_unit();
      if ( p == OGC_NULL )
      {
         ogc_error::set(err, OGC_ERR_NO_MEMORY, obj_kwd());
         return p;
      }

      ogc_string::unescape_str(p->_name, name, OGC_NAME_MAX);
      p->_obj_type  = OGC_OBJ_TYPE_UNIT;
      p->_visible   = true;
      p->_unit_type = OGC_UNIT_TYPE_GENERIC;
      p->_factor    = factor;
      p->_ids       = ids;
   }

   return p;
}

/*------------------------------------------------------------------------
 * destroy
 */
ogc_unit :: ~ogc_unit()
{
   _ids = ogc_vector :: destroy( _ids );
}

ogc_unit * ogc_unit :: destroy(
   ogc_unit * obj)
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
ogc_unit * ogc_unit :: from_tokens(
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

   ogc_unit *   obj = OGC_NULL;
   ogc_id *     id  = OGC_NULL;
   ogc_vector * ids = OGC_NULL;
   const char * name;
   double       factor;

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

   if ( ogc_angunit  ::is_kwd(kwd) )
      return ogc_angunit   :: from_tokens(t, start, pend, err);

   if ( ogc_lenunit  ::is_kwd(kwd) )
      return ogc_lenunit   :: from_tokens(t, start, pend, err);

   if ( ogc_paramunit::is_kwd(kwd) )
      return ogc_paramunit :: from_tokens(t, start, pend, err);

   if ( ogc_scaleunit::is_kwd(kwd) )
      return ogc_scaleunit :: from_tokens(t, start, pend, err);

   if ( ogc_timeunit ::is_kwd(kwd) )
      return ogc_timeunit  :: from_tokens(t, start, pend, err);

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
    * There must be 2 tokens: UNIT[ "name", factor ...
    */
   if ( same < 2 )
   {
      ogc_error::set(err, OGC_ERR_WKT_INSUFFICIENT_TOKENS, obj_kwd(), same);
      return OGC_NULL;
   }

   if ( same > 2 && get_strict_parsing() )
   {
      ogc_error::set(err, OGC_ERR_WKT_TOO_MANY_TOKENS,     obj_kwd(), same);
      return OGC_NULL;
   }

   start++;

   /*---------------------------------------------------------
    * Process all non-object tokens.
    * They come first and are syntactically fixed.
    */
   name   = arr[start++].str;
   factor = ogc_string::atod( arr[start++].str );

   /*---------------------------------------------------------
    * Now process all sub-objects
    */
   int  next = 0;
   for (int i = start; i < end; i = next)
   {
      if ( ogc_id::is_kwd(arr[i].str) )
      {
         id = ogc_id::from_tokens(t, i, &next, err);
         if ( id == OGC_NULL )
         {
            bad = true;
         }
         else
         {
            if ( ids == OGC_NULL )
            {
               ids = ogc_vector::create(1, 1);
               if ( ids == OGC_NULL )
               {
                  ogc_error::set(err, OGC_ERR_NO_MEMORY, obj_kwd());
                  delete id;
                  bad = true;
               }
            }

            if ( ids != OGC_NULL )
            {
               void * p = ids->find(
                             id,
                             false,
                             ogc_utils::compare_id);
               if ( p != OGC_NULL )
               {
                  ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_ID,
                     obj_kwd(), id->name());
                  delete id;
                  bad = true;
               }
               else
               {
                  if ( ids->add( id ) < 0 )
                  {
                     ogc_error::set(err, OGC_ERR_NO_MEMORY, obj_kwd());
                     delete id;
                     bad = true;
                  }
               }
            }
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
      obj = create(name, factor, ids, err);
   }

   if ( obj == OGC_NULL )
   {
      ogc_vector :: destroy( ids );
   }

   return obj;
}

/*------------------------------------------------------------------------
 * object from WKT
 */
ogc_unit * ogc_unit :: from_wkt(
   const char * wkt,
   ogc_error *  err)
{
   ogc_unit * obj = OGC_NULL;
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
bool ogc_unit :: to_wkt(
   const ogc_unit * obj,
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

bool ogc_unit :: to_wkt(
   char      buffer[],
   int       options,
   size_t    buflen) const
{
   OGC_UTF8_NAME buf_name;
   OGC_TBUF      buf_hdr;
   OGC_TBUF      buf_id;
   OGC_NBUF      buf_factor;
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

   ogc_string :: dtoa(_factor, buf_factor);

   if ( (options & OGC_WKT_OPT_OLD_SYNTAX) == 0 )
   {
      switch (_unit_type)
      {
         case OGC_UNIT_TYPE_ANGLE:  kwd = ogc_angunit  ::obj_kwd(); break;
         case OGC_UNIT_TYPE_LENGTH: kwd = ogc_lenunit  ::obj_kwd(); break;
         case OGC_UNIT_TYPE_PARAM:  kwd = ogc_paramunit::obj_kwd(); break;
         case OGC_UNIT_TYPE_SCALE:  kwd = ogc_scaleunit::obj_kwd(); break;
         case OGC_UNIT_TYPE_TIME:   kwd = ogc_timeunit ::obj_kwd(); break;

         default:
            break;
      }
   }

   ogc_string::escape_str(buf_name, _name, OGC_UTF8_NAME_MAX);
   sprintf(buf_hdr, "%s%s\"%s\",%s",
      kwd, opn, buf_name, buf_factor);

   OGC_CPY_TO_BUF( buf_hdr );

   if ( _ids != OGC_NULL && (options & OGC_WKT_OPT_NO_IDS) == 0 )
   {
      for (int i = 0; i < id_count(); i++)
      {
         rc &= ogc_id :: to_wkt(id(i), buf_id, opts, OGC_TBUF_MAX);
         OGC_ADD_TO_BUF( buf_id );
         if ( (options & OGC_WKT_OPT_OLD_SYNTAX) != 0 )
            break;
      }
   }

   OGC_CPY_TO_BUF( cls );

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
ogc_unit * ogc_unit :: clone(const ogc_unit * obj)
{
   if ( obj == OGC_NULL )
      return OGC_NULL;
   return obj->clone();
}

ogc_unit * ogc_unit :: clone() const
{
   ogc_vector * ids = ogc_vector :: clone( _ids );

   ogc_unit * p = create(_name,
                         _factor,
                         ids,
                         OGC_NULL);
   if ( p == OGC_NULL)
   {
      ogc_vector :: destroy( ids );
   }

   return p;
}

/*------------------------------------------------------------------------
 * compare for computational equality
 */
bool ogc_unit :: is_equal(
   const ogc_unit * p1,
   const ogc_unit * p2)
{
   if ( p1 == OGC_NULL && p2 == OGC_NULL ) return true;
   if ( p1 == OGC_NULL || p2 == OGC_NULL ) return false;

   if ( !ogc_string :: is_equal( p1->name(),   p2->name()   ) ||
        !ogc_macros :: eq      ( p1->factor(), p2->factor() ) )
   {
      return false;
   }

   return true;
}

bool ogc_unit :: is_equal(
   const ogc_unit * p) const
{
   return is_equal(this, p);
}

/*------------------------------------------------------------------------
 * compare
 */
bool ogc_unit :: is_identical(
   const ogc_unit * p1,
   const ogc_unit * p2)
{
   if ( p1 == OGC_NULL && p2 == OGC_NULL ) return true;
   if ( p1 == OGC_NULL || p2 == OGC_NULL ) return false;

   if ( !ogc_string :: is_equal    ( p1->name(),   p2->name()   ) ||
        !ogc_macros :: eq          ( p1->factor(), p2->factor() ) ||
        !ogc_vector :: is_identical( p1->ids(),    p2->ids()    ) )
   {
      return false;
   }

   return true;
}

bool ogc_unit :: is_identical(
   const ogc_unit * p) const
{
   return is_identical(this, p);
}

/*------------------------------------------------------------------------
 * get ID count
 */
int ogc_unit :: id_count() const
{
   return (_ids == OGC_NULL) ? 0 : _ids->length();
}

/*------------------------------------------------------------------------
 * get the nth ID
 */
ogc_id * ogc_unit :: id(int n) const
{
   return (_ids == OGC_NULL) ? OGC_NULL :
                               reinterpret_cast<ogc_id *>( _ids->get(n) );
}

} /* namespace OGC */
