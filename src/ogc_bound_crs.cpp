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
/* BOUND CRS object                                                          */
/* ------------------------------------------------------------------------- */

#include "ogc_common.h"

namespace OGC {

const char * ogc_bound_crs :: obj_kwd() { return OGC_OBJ_KWD_BOUND_CRS; }

bool ogc_bound_crs :: is_kwd(const char * kwd)
{
   return ogc_string::is_equal(kwd, obj_kwd());
}

/*------------------------------------------------------------------------
 * create
 */
ogc_bound_crs * ogc_bound_crs :: create(
   ogc_crs *        source_crs,
   ogc_crs *        target_crs,
   ogc_abrtrans *   abrtrans,
   ogc_vector *     ids,
   ogc_remark *     remark,
   ogc_error *      err)
{
   ogc_bound_crs * p = OGC_NULL;
   bool bad = false;

   /*---------------------------------------------------------
    * error checks
    */
   if ( source_crs == OGC_NULL )
   {
      ogc_error::set(err, OGC_ERR_MISSING_SOURCE_CRS, obj_kwd());
      bad = true;
   }

   if ( target_crs == OGC_NULL )
   {
      ogc_error::set(err, OGC_ERR_MISSING_TARGET_CRS, obj_kwd());
      bad = true;
   }

   if ( abrtrans == OGC_NULL )
   {
      ogc_error::set(err, OGC_ERR_MISSING_ABRTRANS, obj_kwd());
      bad = true;
   }

   /*---------------------------------------------------------
    * create the object
    */
   if ( !bad )
   {
      p = new (std::nothrow) ogc_bound_crs();
      if ( p == OGC_NULL )
      {
         ogc_error::set(err, OGC_ERR_NO_MEMORY, obj_kwd());
         return p;
      }

      p->_obj_type   = OGC_OBJ_TYPE_BOUND_CRS;
      p->_source_crs = source_crs;
      p->_target_crs = target_crs;
      p->_abrtrans   = abrtrans;
      p->_ids        = ids;
      p->_remark     = remark;
   }

   return p;
}

/*------------------------------------------------------------------------
 * destroy
 */
ogc_bound_crs :: ~ogc_bound_crs()
{
   ogc_crs        :: destroy( _source_crs );
   ogc_crs        :: destroy( _target_crs );
   ogc_abrtrans   :: destroy( _abrtrans   );
   ogc_vector     :: destroy( _ids        );
   ogc_remark     :: destroy( _remark     );
}

void ogc_bound_crs :: destroy(
   ogc_bound_crs * obj)
{
   if ( obj != OGC_NULL )
   {
      delete obj;
   }
}

/*------------------------------------------------------------------------
 * object from tokens
 */
ogc_bound_crs * ogc_bound_crs :: from_tokens(
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

   ogc_bound_crs * obj        = OGC_NULL;
   ogc_crs *       source_crs = OGC_NULL;
   ogc_crs *       target_crs = OGC_NULL;
   ogc_abrtrans *  abrtrans   = OGC_NULL;
   ogc_id *        id         = OGC_NULL;
   ogc_vector *    ids        = OGC_NULL;
   ogc_remark *    remark     = OGC_NULL;

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
    * There must be 1 token: BOUNDCRS[ ...
    */
   if ( same < 0 )
   {
      ogc_error::set(err, OGC_ERR_WKT_INSUFFICIENT_TOKENS, obj_kwd(), same);
      return OGC_NULL;
   }

   if ( same > 0 && get_strict_parsing() )
   {
      ogc_error::set(err, OGC_ERR_WKT_TOO_MANY_TOKENS,     obj_kwd(), same);
      return OGC_NULL;
   }

   start++;

   /*---------------------------------------------------------
    * Process all non-object tokens.
    * They come first and are syntactically fixed.
    */

   /*---------------------------------------------------------
    * Now process all sub-objects
    */
   int  next = 0;
   for (int i = start; i < end; i = next)
   {
      if ( ogc_string::is_equal(arr[i].str, OGC_OBJ_KWD_SOURCE_CRS) )
      {
         if ( source_crs != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_SOURCE_CRS, obj_kwd());
            bad = true;
         }
         else
         {
            source_crs = ogc_crs::from_tokens(t, i+1, &next, err);
            if ( source_crs == OGC_NULL )
               bad = true;
         }
         continue;
      }

      if ( ogc_string::is_equal(arr[i].str, OGC_OBJ_KWD_TARGET_CRS) )
      {
         if ( target_crs != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_TARGET_CRS, obj_kwd());
            bad = true;
         }
         else
         {
            target_crs = ogc_crs::from_tokens(t, i+1, &next, err);
            if ( target_crs == OGC_NULL )
               bad = true;
         }
         continue;
      }

      if ( ogc_abrtrans::is_kwd(arr[i].str) )
      {
         if ( abrtrans != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_ABRTRANS, obj_kwd());
            bad = true;
         }
         else
         {
            abrtrans = ogc_abrtrans::from_tokens(t, i, &next, err);
            if ( abrtrans == OGC_NULL )
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
      obj = create(source_crs, target_crs, abrtrans, ids, remark, err);
   }

   if ( obj == OGC_NULL )
   {
      ogc_crs        :: destroy( source_crs );
      ogc_crs        :: destroy( target_crs );
      ogc_abrtrans   :: destroy( abrtrans   );
      ogc_vector     :: destroy( ids        );
      ogc_remark     :: destroy( remark     );
   }

   return obj;
}

/*------------------------------------------------------------------------
 * object from WKT
 */
ogc_bound_crs * ogc_bound_crs :: from_wkt(
   const char * wkt,
   ogc_error *  err)
{
   ogc_bound_crs * obj = OGC_NULL;
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
bool ogc_bound_crs :: to_wkt(
   const ogc_bound_crs * obj,
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

bool ogc_bound_crs :: to_wkt(
   char      buffer[],
   int       options,
   size_t    buflen) const
{
   OGC_TBUF      buf_hdr;
   OGC_BUFF      buf_source_crs;
   OGC_BUFF      buf_source;
   OGC_BUFF      buf_target_crs;
   OGC_BUFF      buf_target;
   OGC_TBUF      buf_abrtrans;
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

   if ( (options & OGC_WKT_OPT_OLD_SYNTAX) != 0 )
      return true;

   rc &= ogc_crs :: to_wkt(_source_crs,    buf_source_crs, opts, OGC_BUFF_MAX-8);
   rc &= ogc_crs :: to_wkt(_target_crs,    buf_target_crs, opts, OGC_BUFF_MAX-8);
   rc &= ogc_abrtrans :: to_wkt(_abrtrans, buf_abrtrans,   opts, OGC_TBUF_MAX);
   rc &= ogc_remark   :: to_wkt(_remark,   buf_remark,     opts, OGC_TBUF_MAX);

   sprintf(buf_source, "%s%s%s%s",
      OGC_OBJ_KWD_SOURCE_CRS, opn, buf_source_crs, cls);

   sprintf(buf_target, "%s%s%s%s",
      OGC_OBJ_KWD_TARGET_CRS, opn, buf_target_crs, cls);

   sprintf(buf_hdr, "%s%s",
      kwd, opn);

   OGC_CPY_TO_BUF( buf_hdr      );
   OGC_CPY_TO_BUF( buf_source   );
   OGC_ADD_TO_BUF( buf_target   );
   OGC_ADD_TO_BUF( buf_abrtrans );

   if ( _ids != OGC_NULL && (options & OGC_WKT_OPT_NO_IDS) == 0 )
   {
      for (int i = 0; i < id_count(); i++)
      {
         rc &= ogc_id :: to_wkt(id(i), buf_id, opts, OGC_TBUF_MAX);
         OGC_ADD_TO_BUF( buf_id );
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
ogc_bound_crs * ogc_bound_crs :: clone(const ogc_bound_crs * obj)
{
   if ( obj == OGC_NULL )
      return OGC_NULL;
   return obj->clone();
}

ogc_bound_crs * ogc_bound_crs :: clone() const
{
   ogc_crs *      source_crs = ogc_crs      :: clone( _source_crs );
   ogc_crs *      target_crs = ogc_crs      :: clone( _target_crs );
   ogc_abrtrans * abrtrans   = ogc_abrtrans :: clone( _abrtrans   );
   ogc_vector *   ids        = ogc_vector   :: clone( _ids        );
   ogc_remark *   remark     = ogc_remark   :: clone( _remark     );

   ogc_bound_crs * p = create(source_crs,
                              target_crs,
                              abrtrans,
                              ids,
                              remark,
                              OGC_NULL);
   if ( p == OGC_NULL )
   {
      ogc_crs      :: destroy( source_crs );
      ogc_crs      :: destroy( target_crs );
      ogc_abrtrans :: destroy( abrtrans   );
      ogc_vector   :: destroy( ids        );
      ogc_remark   :: destroy( remark     );
   }

   return p;
}

/*------------------------------------------------------------------------
 * compare for computational equality
 */
bool ogc_bound_crs :: is_equal(
   const ogc_bound_crs * p1,
   const ogc_bound_crs * p2)
{
   if ( p1 == OGC_NULL && p2 == OGC_NULL ) return true;
   if ( p1 == OGC_NULL || p2 == OGC_NULL ) return false;

   if ( !ogc_crs      :: is_equal( p1->source_crs(), p2->source_crs() ) ||
        !ogc_crs      :: is_equal( p1->target_crs(), p2->target_crs() ) ||
        !ogc_abrtrans :: is_equal( p1->abrtrans(),   p2->abrtrans()   ) )
   {
      return false;
   }

   return true;
}

bool ogc_bound_crs :: is_equal(
   const ogc_bound_crs * p) const
{
   return is_equal(this, p);
}

/*------------------------------------------------------------------------
 * compare
 */
bool ogc_bound_crs :: is_identical(
   const ogc_bound_crs * p1,
   const ogc_bound_crs * p2)
{
   if ( p1 == OGC_NULL && p2 == OGC_NULL ) return true;
   if ( p1 == OGC_NULL || p2 == OGC_NULL ) return false;

   if ( !ogc_crs      :: is_identical( p1->source_crs(), p2->source_crs() ) ||
        !ogc_crs      :: is_identical( p1->target_crs(), p2->target_crs() ) ||
        !ogc_abrtrans :: is_identical( p1->abrtrans(),   p2->abrtrans()   ) ||
        !ogc_vector   :: is_identical( p1->ids(),        p2->ids()        ) ||
        !ogc_remark   :: is_identical( p1->remark(),     p2->remark()     ) )
   {
      return false;
   }

   return true;
}

bool ogc_bound_crs :: is_identical(
   const ogc_bound_crs * p) const
{
   return is_identical(this, p);
}

/*------------------------------------------------------------------------
 * get ID count
 */
int ogc_bound_crs :: id_count() const
{
   return (_ids == OGC_NULL) ? 0 : _ids->length();
}

/*------------------------------------------------------------------------
 * get the nth ID
 */
ogc_id * ogc_bound_crs :: id(int n) const
{
   return (_ids == OGC_NULL) ? OGC_NULL :
                               reinterpret_cast<ogc_id *>( _ids->get(n) );
}

} /* namespace OGC */
