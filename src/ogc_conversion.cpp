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
/* CONVERSION object                                                         */
/* ------------------------------------------------------------------------- */

#include "ogc_common.h"

namespace OGC {

const char * ogc_conversion :: obj_kwd() { return OGC_OBJ_KWD_CONVERSION; }

/*------------------------------------------------------------------------
 * create
 */
ogc_conversion * ogc_conversion :: create(
   const char * name,
   ogc_method * method,
   ogc_vector * parameters,
   ogc_vector * ids,
   ogc_error *  err)
{
   ogc_conversion * p = OGC_NULL;
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

   if ( method == OGC_NULL )
   {
      ogc_error::set(err, OGC_ERR_MISSING_METHOD, obj_kwd());
      bad = true;
   }

   /*---------------------------------------------------------
    * create the object
    */
   if ( !bad )
   {
      p = new (std::nothrow) ogc_conversion();
      if ( p == OGC_NULL )
      {
         ogc_error::set(err, OGC_ERR_NO_MEMORY, obj_kwd());
         return p;
      }

      ogc_string::unescape_str(p->_name, name, OGC_NAME_MAX);
      p->_obj_type   = OGC_OBJ_TYPE_METHOD;
      p->_method     = method;
      p->_parameters = parameters;
      p->_ids        = ids;
   }

   return p;
}

/*------------------------------------------------------------------------
 * destroy
 */
ogc_conversion :: ~ogc_conversion()
{
   ogc_method :: destroy( _method     );
   ogc_vector :: destroy( _parameters );
   ogc_vector :: destroy( _ids        );
}

void ogc_conversion :: destroy(
   ogc_conversion * obj)
{
   if ( obj != OGC_NULL )
   {
      delete obj;
   }
}

/*------------------------------------------------------------------------
 * object from tokens
 */
ogc_conversion * ogc_conversion :: from_tokens(
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

   ogc_conversion * obj        = OGC_NULL;
   ogc_id *         id         = OGC_NULL;
   ogc_method *     method     = OGC_NULL;
   ogc_vector *     parameters = OGC_NULL;
   ogc_parameter *  param      = OGC_NULL;
   ogc_vector *     ids        = OGC_NULL;
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

   if ( !ogc_string::is_equal(kwd, obj_kwd()) )
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
    * There must be 1 token: METHOD[ "name" ...
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
      if ( ogc_string::is_equal(arr[i].str, ogc_method::obj_kwd()) ||
           ogc_string::is_equal(arr[i].str, ogc_method::alt_kwd()) )
      {
         if ( method != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_METHOD, obj_kwd());
            bad = true;
         }
         else
         {
            method = ogc_method::from_tokens(t, i, &next, err);
            if ( method == OGC_NULL )
               bad = true;
         }
         continue;
      }

      if ( ogc_string::is_equal(arr[i].str, ogc_parameter::obj_kwd()) )
      {
         param = ogc_parameter::from_tokens(t, i, &next, err);
         if ( param == OGC_NULL )
         {
            bad = true;
         }
         else
         {
            if ( parameters == OGC_NULL )
            {
               parameters = ogc_vector::create(1, 1);
               if ( parameters == OGC_NULL )
               {
                  ogc_error::set(err, OGC_ERR_NO_MEMORY, obj_kwd());
                  delete param;
                  bad = true;
               }
            }

            if ( parameters != OGC_NULL )
            {
               void * p = parameters->find(
                             param,
                             false,
                             ogc_utils::compare_parameter);
               if ( p != OGC_NULL )
               {
                  ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_PARAMETER,
                     obj_kwd(), param->name());
                  delete param;
                  bad = true;
               }
               else
               {
                  if ( parameters->add( param ) < 0 )
                  {
                     ogc_error::set(err, OGC_ERR_NO_MEMORY, obj_kwd());
                     delete param;
                     bad = true;
                  }
               }
            }
         }
         continue;
      }

      if ( ogc_string::is_equal(arr[i].str, ogc_id::obj_kwd()) )
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
    * Final checks
    */

   /*---------------------------------------------------------
    * Create the object
    */
   if ( !bad )
   {
      obj = create(name, method, parameters, ids, err);
   }

   if ( obj == OGC_NULL )
   {
      ogc_method :: destroy( method     );
      ogc_vector :: destroy( parameters );
      ogc_vector :: destroy( ids        );
   }

   return obj;
}

/*------------------------------------------------------------------------
 * object to WKT
 */
ogc_conversion * ogc_conversion :: from_wkt(
   const char * wkt,
   ogc_error *  err)
{
   ogc_conversion * obj = OGC_NULL;
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
bool ogc_conversion :: to_wkt(
   const ogc_conversion * obj,
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

bool ogc_conversion :: to_wkt(
   char      buffer[],
   int       options,
   size_t    buflen) const
{
   OGC_UTF8_NAME buf_name;
   OGC_TBUF      buf_hdr;
   OGC_TBUF      buf_method;
   OGC_TBUF      buf_parameter;
   OGC_TBUF      buf_id;
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

   rc &= ogc_method :: to_wkt(_method, buf_method, opts, OGC_TBUF_MAX);

   ogc_string::escape_str(buf_name, _name, OGC_UTF8_NAME_MAX);
   sprintf(buf_hdr, "%s%s\"%s\"",
      kwd, opn, buf_name);

   OGC_CPY_TO_BUF( buf_hdr    );
   OGC_ADD_TO_BUF( buf_method );

   if ( _parameters != OGC_NULL )
   {
      for (int i = 0; i < parameter_count(); i++)
      {
         rc &= ogc_parameter :: to_wkt(parameter(i), buf_parameter, opts, OGC_TBUF_MAX);
         OGC_ADD_TO_BUF( buf_parameter );
      }
   }

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
ogc_conversion * ogc_conversion :: clone(const ogc_conversion * obj)
{
   if ( obj == OGC_NULL )
      return OGC_NULL;
   return obj->clone();
}

ogc_conversion * ogc_conversion :: clone() const
{
   ogc_method * method     = ogc_method :: clone( _method     );
   ogc_vector * parameters = ogc_vector :: clone( _parameters );
   ogc_vector * ids        = ogc_vector :: clone( _ids        );

   ogc_conversion * p = create(_name,
                               method,
                               parameters,
                               ids,
                               OGC_NULL);
   if ( p == OGC_NULL )
   {
;
      ogc_method :: destroy( method     );
      ogc_vector :: destroy( parameters );
      ogc_vector :: destroy( ids        );
   }

   return p;
}

/*------------------------------------------------------------------------
 * compare for computational equality
 */
bool ogc_conversion :: is_equal(
   const ogc_conversion * p1,
   const ogc_conversion * p2)
{
   if ( p1 == OGC_NULL && p2 == OGC_NULL ) return true;
   if ( p1 == OGC_NULL || p2 == OGC_NULL ) return false;

   if ( !ogc_string :: is_equal( p1->name(),       p2->name()       ) ||
        !ogc_method :: is_equal( p1->method(),     p2->method()     ) ||
        !ogc_vector :: is_equal( p1->parameters(), p2->parameters() ) )
   {
      return false;
   }

   return true;
}

bool ogc_conversion :: is_equal(
   const ogc_conversion * p) const
{
   return is_equal(this, p);
}

/*------------------------------------------------------------------------
 * compare
 */
bool ogc_conversion :: is_identical(
   const ogc_conversion * p1,
   const ogc_conversion * p2)
{
   if ( p1 == OGC_NULL && p2 == OGC_NULL ) return true;
   if ( p1 == OGC_NULL || p2 == OGC_NULL ) return false;

   if ( !ogc_string :: is_equal    ( p1->name(),       p2->name()       ) ||
        !ogc_method :: is_identical( p1->method(),     p2->method()     ) ||
        !ogc_vector :: is_identical( p1->parameters(), p2->parameters() ) ||
        !ogc_vector :: is_identical( p1->ids(),        p2->ids()        ) )
   {
      return false;
   }

   return true;
}

bool ogc_conversion :: is_identical(
   const ogc_conversion * p) const
{
   return is_identical(this, p);
}

/*------------------------------------------------------------------------
 * get parameter count
 */
int ogc_conversion :: parameter_count() const
{
   return (_parameters == OGC_NULL) ? 0 : _parameters->length();
}

/*------------------------------------------------------------------------
 * get the nth parameter
 */
ogc_parameter * ogc_conversion :: parameter(int n) const
{
   return (_parameters == OGC_NULL) ? OGC_NULL :
                                      reinterpret_cast<ogc_parameter *>(
                                         _parameters->get(n) );
}

/*------------------------------------------------------------------------
 * get ID count
 */
int ogc_conversion :: id_count() const
{
   return (_ids == OGC_NULL) ? 0 : _ids->length();
}

/*------------------------------------------------------------------------
 * get the nth ID
 */
ogc_id * ogc_conversion :: id(int n) const
{
   return (_ids == OGC_NULL) ? OGC_NULL :
                               reinterpret_cast<ogc_id *>( _ids->get(n) );
}

} /* namespace OGC */
