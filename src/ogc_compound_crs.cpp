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
/* COMPOUND CRS object                                                       */
/* ------------------------------------------------------------------------- */

#include "ogc_common.h"

namespace OGC {

const char * ogc_compound_crs :: obj_kwd() { return OGC_OBJ_KWD_COMPOUND_CRS; }
const char * ogc_compound_crs :: old_kwd() { return OGC_OLD_KWD_COMPDCS;      }

bool ogc_compound_crs :: is_kwd(const char * kwd)
{
   return ogc_string::is_equal(kwd, obj_kwd()) ||
          ogc_string::is_equal(kwd, old_kwd());
}

/*------------------------------------------------------------------------
 * create
 */
ogc_compound_crs * ogc_compound_crs :: create(
   const char *       name,
   ogc_crs *          first_crs,
   ogc_crs *          second_crs,
   ogc_time_crs *     third_crs,
   ogc_vector *       ids,
   ogc_remark *       remark,
   ogc_error *        err)
{
   ogc_compound_crs * p = OGC_NULL;
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

   if ( first_crs == OGC_NULL )
   {
      ogc_error::set(err, OGC_ERR_MISSING_FIRST_CRS, obj_kwd());
      bad = true;
   }
   else
   {
      switch ( first_crs->crs_type() )
      {
         case OGC_CRS_TYPE_GEOD:
         case OGC_CRS_TYPE_PROJ:
         case OGC_CRS_TYPE_ENGR:
            break;

         default:
            ogc_error::set(err, OGC_ERR_INVALID_FIRST_CRS, obj_kwd());
            bad = true;
            break;
      }
   }

   if ( second_crs == OGC_NULL )
   {
      ogc_error::set(err, OGC_ERR_MISSING_SECOND_CRS, obj_kwd());
      bad = true;
   }
   else
   {
      switch ( second_crs->crs_type() )
      {
         case OGC_CRS_TYPE_VERT:
         case OGC_CRS_TYPE_PARAM:
            break;

         case OGC_CRS_TYPE_TIME:
            if ( third_crs != OGC_NULL )
            {
               ogc_error::set(err, OGC_ERR_INVALID_THIRD_CRS, obj_kwd());
               bad = true;
            }
            break;

         default:
            ogc_error::set(err, OGC_ERR_INVALID_SECOND_CRS, obj_kwd());
            bad = true;
            break;
      }
   }

   /*---------------------------------------------------------
    * create the object
    */
   if ( !bad )
   {
      p = new (std::nothrow) ogc_compound_crs();
      if ( p == OGC_NULL )
      {
         ogc_error::set(err, OGC_ERR_NO_MEMORY, obj_kwd());
         return p;
      }

      ogc_string::unescape_str(p->_name, name, OGC_NAME_MAX);
      p->_obj_type   = OGC_OBJ_TYPE_COMPOUND_CRS;
      p->_visible    = true;
      p->_crs_type   = OGC_CRS_TYPE_COMPOUND;
      p->_cs         = OGC_NULL;
      p->_axis_1     = OGC_NULL;
      p->_axis_2     = OGC_NULL;
      p->_axis_3     = OGC_NULL;
      p->_unit       = OGC_NULL;
      p->_first_crs  = first_crs;
      p->_second_crs = second_crs;
      p->_third_crs  = third_crs;
      p->_scope      = OGC_NULL;
      p->_extents    = OGC_NULL;
      p->_ids        = ids;
      p->_remark     = remark;
   }

   return p;
}

/*------------------------------------------------------------------------
 * destroy
 */
ogc_compound_crs :: ~ogc_compound_crs()
{
   _first_crs  = ogc_crs      :: destroy( _first_crs  );
   _second_crs = ogc_crs      :: destroy( _second_crs );
   _third_crs  = ogc_time_crs :: destroy( _third_crs  );
   _ids        = ogc_vector   :: destroy( _ids        );
   _remark     = ogc_remark   :: destroy( _remark     );
}

ogc_compound_crs * ogc_compound_crs :: destroy(
   ogc_compound_crs * obj)
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
ogc_compound_crs * ogc_compound_crs :: from_tokens(
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

   ogc_compound_crs * obj        = OGC_NULL;
   ogc_crs *          first_crs  = OGC_NULL;
   ogc_crs *          second_crs = OGC_NULL;
   ogc_time_crs *     third_crs  = OGC_NULL;
   ogc_id *           id         = OGC_NULL;
   ogc_vector *       ids        = OGC_NULL;
   ogc_remark *       remark     = OGC_NULL;
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

   if ( ogc_string::is_equal(kwd, old_kwd()) )
      return from_tokens_old(t, start, pend, err);

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
    * There must be 1 token: COMPOUNDCRS[ "name" ...
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
      if ( ogc_geod_crs::is_kwd(arr[i].str) ||
           ogc_proj_crs::is_kwd(arr[i].str) ||
           ogc_engr_crs::is_kwd(arr[i].str) )
      {
         if ( first_crs != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_FIRST_CRS, obj_kwd());
            bad = true;
         }
         else
         {
            first_crs = ogc_crs::from_tokens(t, i, &next, err);
            if ( first_crs == OGC_NULL )
               bad = true;
         }
         continue;
      }

      if ( ogc_vert_crs ::is_kwd(arr[i].str) ||
           ogc_param_crs::is_kwd(arr[i].str) )
      {
         if ( second_crs != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_SECOND_CRS, obj_kwd());
            bad = true;
         }
         else
         {
            second_crs = ogc_crs::from_tokens(t, i, &next, err);
            if ( second_crs == OGC_NULL )
               bad = true;
         }
         continue;
      }

      if ( ogc_time_crs::is_kwd(arr[i].str) )
      {
         if ( (second_crs != OGC_NULL &&
               second_crs->crs_type() == OGC_CRS_TYPE_TIME) ||
              third_crs != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_TIME_CRS, obj_kwd());
            bad = true;
         }
         else if ( second_crs == OGC_NULL )
         {
            second_crs = ogc_time_crs::from_tokens(t, i, &next, err);
            if ( second_crs == OGC_NULL )
               bad = true;
         }
         else if ( third_crs == OGC_NULL )
         {
            third_crs = ogc_time_crs::from_tokens(t, i, &next, err);
            if ( third_crs == OGC_NULL )
               bad = true;
         }
         continue;
      }

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

      if ( ogc_remark::is_kwd(arr[i].str) )
      {
         if ( remark != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_REMARK, obj_kwd());
            bad = true;
         }
         else
         {
            remark = ogc_remark::from_tokens(t, i, &next, err);
            if ( remark == OGC_NULL )
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
      obj = create(name, first_crs, second_crs, third_crs, ids, remark, err);
   }

   if ( obj == OGC_NULL )
   {
      ogc_crs    :: destroy( first_crs  );
      ogc_crs    :: destroy( second_crs );
      ogc_crs    :: destroy( third_crs  );
      ogc_vector :: destroy( ids        );
      ogc_remark :: destroy( remark     );
   }

   return obj;
}

/*------------------------------------------------------------------------
 * object from tokens (using old syntax)
 */
ogc_compound_crs * ogc_compound_crs :: from_tokens_old(
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

   ogc_compound_crs * obj        = OGC_NULL;
   ogc_crs *          first_crs  = OGC_NULL;
   ogc_crs *          second_crs = OGC_NULL;
   ogc_id *           id         = OGC_NULL;
   ogc_vector *       ids        = OGC_NULL;
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

   if ( !ogc_string::is_equal(kwd, old_kwd()) )
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
    * There must be 1 token: COMPOUNDCRS[ "name" ...
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
      if ( ogc_geod_crs::is_kwd(arr[i].str) ||
           ogc_proj_crs::is_kwd(arr[i].str) )
      {
         if ( first_crs != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_FIRST_CRS, obj_kwd());
            bad = true;
         }
         else
         {
            first_crs = ogc_crs::from_tokens(t, i, &next, err);
            if ( first_crs == OGC_NULL )
               bad = true;
         }
         continue;
      }

      if ( ogc_vert_crs ::is_kwd(arr[i].str) )
      {
         if ( second_crs != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_SECOND_CRS, obj_kwd());
            bad = true;
         }
         else
         {
            second_crs = ogc_crs::from_tokens(t, i, &next, err);
            if ( second_crs == OGC_NULL )
               bad = true;
         }
         continue;
      }

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
      obj = create(name, first_crs, second_crs, OGC_NULL, ids, OGC_NULL, err);
   }

   if ( obj == OGC_NULL )
   {
      ogc_crs    :: destroy( first_crs  );
      ogc_crs    :: destroy( second_crs );
      ogc_vector :: destroy( ids        );
   }

   return obj;
}

/*------------------------------------------------------------------------
 * object from WKT
 */
ogc_compound_crs * ogc_compound_crs :: from_wkt(
   const char * wkt,
   ogc_error *  err)
{
   ogc_compound_crs * obj = OGC_NULL;
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
bool ogc_compound_crs :: to_wkt(
   const ogc_compound_crs * obj,
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

bool ogc_compound_crs :: to_wkt(
   char      buffer[],
   int       options,
   size_t    buflen) const
{
   OGC_UTF8_NAME buf_name;
   OGC_TBUF      buf_hdr;
   OGC_BUFF      buf_first;
   OGC_BUFF      buf_second;
   OGC_BUFF      buf_third;
   OGC_TBUF      buf_id;
   OGC_TBUF      buf_remark;
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
      kwd = old_kwd();

   rc &= ogc_crs      :: to_wkt(_first_crs,  buf_first,  opts, OGC_BUFF_MAX);
   rc &= ogc_crs      :: to_wkt(_second_crs, buf_second, opts, OGC_BUFF_MAX);
   rc &= ogc_time_crs :: to_wkt(_third_crs,  buf_third,  opts, OGC_BUFF_MAX);
   rc &= ogc_remark   :: to_wkt(_remark,     buf_remark, opts, OGC_TBUF_MAX);

   ogc_string::escape_str(buf_name, _name, OGC_UTF8_NAME_MAX);
   sprintf(buf_hdr, "%s%s\"%s\"",
      kwd, opn, buf_name);

   OGC_CPY_TO_BUF( buf_hdr    );
   OGC_ADD_TO_BUF( buf_first  );
   OGC_ADD_TO_BUF( buf_second );
   OGC_ADD_TO_BUF( buf_third  );

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

   OGC_ADD_TO_BUF( buf_remark );
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
ogc_compound_crs * ogc_compound_crs :: clone(const ogc_compound_crs * obj)
{
   if ( obj == OGC_NULL )
      return OGC_NULL;
   return obj->clone();
}

ogc_compound_crs * ogc_compound_crs :: clone() const
{
   ogc_crs *      first_crs  = ogc_crs      :: clone( _first_crs  );
   ogc_crs *      second_crs = ogc_crs      :: clone( _second_crs );
   ogc_time_crs * third_crs  = ogc_time_crs :: clone( _third_crs  );
   ogc_vector *   ids        = ogc_vector   :: clone( _ids        );
   ogc_remark *   remark     = ogc_remark   :: clone( _remark     );

   ogc_compound_crs * p = create(_name,
                                 first_crs,
                                 second_crs,
                                 third_crs,
                                 ids,
                                 remark,
                                 OGC_NULL);
   if ( p == OGC_NULL )
   {
      ogc_crs      :: destroy( first_crs  );
      ogc_crs      :: destroy( second_crs );
      ogc_time_crs :: destroy( third_crs  );
      ogc_vector   :: destroy( ids        );
      ogc_remark   :: destroy( remark     );
   }

   return p;
}

/*------------------------------------------------------------------------
 * compare for computational equality
 */
bool ogc_compound_crs :: is_equal(
   const ogc_compound_crs * p1,
   const ogc_compound_crs * p2)
{
   if ( p1 == OGC_NULL && p2 == OGC_NULL ) return true;
   if ( p1 == OGC_NULL || p2 == OGC_NULL ) return false;

   if ( !ogc_string :: is_equal( p1->name(),       p2->name()       ) ||
        !ogc_crs    :: is_equal( p1->first_crs(),  p2->first_crs()  ) ||
        !ogc_crs    :: is_equal( p1->second_crs(), p2->second_crs() ) ||
        !ogc_crs    :: is_equal( p1->third_crs(),  p2->third_crs()  ) )
   {
      return false;
   }

   return true;
}

bool ogc_compound_crs :: is_equal(
   const ogc_compound_crs * p) const
{
   return is_equal(this, p);
}

/*------------------------------------------------------------------------
 * compare
 */
bool ogc_compound_crs :: is_identical(
   const ogc_compound_crs * p1,
   const ogc_compound_crs * p2)
{
   if ( p1 == OGC_NULL && p2 == OGC_NULL ) return true;
   if ( p1 == OGC_NULL || p2 == OGC_NULL ) return false;

   if ( !ogc_string :: is_equal    ( p1->name(),       p2->name()       ) ||
        !ogc_crs    :: is_identical( p1->first_crs(),  p2->first_crs()  ) ||
        !ogc_crs    :: is_identical( p1->second_crs(), p2->second_crs() ) ||
        !ogc_crs    :: is_identical( p1->third_crs(),  p2->third_crs()  ) ||
        !ogc_vector :: is_identical( p1->ids(),        p2->ids()        ) ||
        !ogc_remark :: is_identical( p1->remark(),     p2->remark()     ) )
   {
      return false;
   }

   return true;
}

bool ogc_compound_crs :: is_identical(
   const ogc_compound_crs * p) const
{
   return is_identical(this, p);
}

} /* namespace OGC */
