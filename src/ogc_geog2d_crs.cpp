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
/* GEOG2D CRS (2D geodetic CRS) object                                       */
/* ------------------------------------------------------------------------- */

#include "ogc_common.h"

namespace OGC {

const char * ogc_geog2d_crs :: obj_kwd() { return OGC_OBJ_KWD_GEOG2D_CRS; }
const char * ogc_geog2d_crs :: alt_kwd() { return OGC_OBJ_KWD_BASE_CRS;   }
const char * ogc_geog2d_crs :: old_kwd() { return OGC_OBJ_KWD_GEOGCS;     }

/*------------------------------------------------------------------------
 * create
 */
ogc_geog2d_crs * ogc_geog2d_crs :: create(
   const char *         name,
   ogc_geodetic_datum * datum,
   ogc_primem *         primem,
   ogc_cs *             cs,
   ogc_axis *           axis_1,
   ogc_axis *           axis_2,
   ogc_angunit *        angunit,
   ogc_scope *          scope,
   ogc_vector *         extents,
   ogc_vector *         ids,
   ogc_remark *         remark,
   ogc_error *          err)
{
   ogc_geog2d_crs * p = OGC_NULL;
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

   if ( cs == OGC_NULL )
   {
      ogc_error::set(err, OGC_ERR_MISSING_CS, obj_kwd());
      bad = true;
   }
   else if ( !ogc_utils::validate_cs(OGC_OBJ_TYPE_GEOG2D_CRS, cs,
                                     axis_1, axis_2, OGC_NULL, angunit, err) )
   {
      bad = true;
   }

   /*---------------------------------------------------------
    * create the object
    */
   if ( !bad )
   {
      p = new (std::nothrow) ogc_geog2d_crs();
      if ( p == OGC_NULL )
      {
         ogc_error::set(err, OGC_ERR_NO_MEMORY, obj_kwd());
         return p;
      }

      ogc_string::unescape_str(p->_name, name, OGC_NAME_MAX);
      p->_obj_type = OGC_OBJ_TYPE_GEOG2D_CRS;
      p->_crs_type = OGC_CRS_TYPE_GEOG2D;
      p->_datum    = datum;
      p->_primem   = primem;
      p->_cs       = cs;
      p->_axis_1   = axis_1;
      p->_axis_2   = axis_2;
      p->_axis_3   = OGC_NULL;
      p->_unit     = angunit;
      p->_scope    = scope;
      p->_extents  = extents;
      p->_ids      = ids;
      p->_remark   = remark;
   }

   return p;
}

/*------------------------------------------------------------------------
 * destroy
 */
ogc_geog2d_crs :: ~ogc_geog2d_crs()
{
   ogc_geodetic_datum :: destroy( _datum   );
   ogc_primem         :: destroy( _primem  );
}

void ogc_geog2d_crs :: destroy(
   ogc_geog2d_crs * obj)
{
   if ( obj != OGC_NULL )
   {
      delete obj;
   }
}

/*------------------------------------------------------------------------
 * object from tokens
 */
ogc_geog2d_crs * ogc_geog2d_crs :: from_tokens(
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

   ogc_geog2d_crs *      obj     = OGC_NULL;
   ogc_geodetic_datum *  datum   = OGC_NULL;
   ogc_primem *          primem  = OGC_NULL;
   ogc_cs *              cs      = OGC_NULL;
   ogc_axis *            axis    = OGC_NULL;
   ogc_axis *            axis_1  = OGC_NULL;
   ogc_axis *            axis_2  = OGC_NULL;
   ogc_angunit *         unit    = OGC_NULL;
   ogc_scope *           scope   = OGC_NULL;
   ogc_extent *          extent  = OGC_NULL;
   ogc_vector *          extents = OGC_NULL;
   ogc_id *              id      = OGC_NULL;
   ogc_vector *          ids     = OGC_NULL;
   ogc_remark *          remark  = OGC_NULL;
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
   {
      return from_tokens_geogcs(t, start, pend, err);
   }

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
    * There must be 1 token: GEOGCRS[ "name" ...
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
      if ( ogc_string::is_equal(arr[i].str, ogc_geodetic_datum::obj_kwd()) )
      {
         if ( datum != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_DATUM, obj_kwd());
            bad = true;
         }
         else
         {
            datum = ogc_geodetic_datum::from_tokens(t, i, &next, err);
            if ( datum == OGC_NULL )
               bad = true;
         }
         continue;
      }

      if ( ogc_string::is_equal(arr[i].str, ogc_primem::obj_kwd()) )
      {
         if ( primem != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_PRIMEM, obj_kwd());
            bad = true;
         }
         else
         {
            primem = ogc_primem::from_tokens(t, i, &next, err);
            if ( primem == OGC_NULL )
               bad = true;
         }
         continue;
      }

      if ( ogc_string::is_equal(arr[i].str, ogc_cs::obj_kwd()) )
      {
         if ( cs != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_CS, obj_kwd());
            bad = true;
         }
         else
         {
            cs = ogc_cs::from_tokens(t, i, &next, err);
            if ( cs == OGC_NULL )
               bad = true;
         }
         continue;
      }

      if ( ogc_string::is_equal(arr[i].str, ogc_axis::obj_kwd()) )
      {
         axis = ogc_axis::from_tokens(t, i, &next, err);
         if ( axis == OGC_NULL )
         {
            bad = true;
         }
         else
         {
            if ( !ogc_utils::place_axis(axis, &axis_1, &axis_2, OGC_NULL,
                                        obj_kwd(), err) )
            {
               delete axis;
               bad = true;
            }
         }
         continue;
      }

      if ( ogc_string::is_equal(arr[i].str, ogc_angunit::obj_kwd()) ||
           ogc_string::is_equal(arr[i].str, ogc_angunit::alt_kwd()) )
      {
         if ( unit != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_UNIT, obj_kwd());
            bad = true;
         }
         else
         {
            unit = ogc_angunit::from_tokens(t, i, &next, err);
            if ( unit == OGC_NULL )
               bad = true;
         }
         continue;
      }

      if ( ogc_string::is_equal(arr[i].str, ogc_scope::obj_kwd()) )
      {
         if ( scope != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_SCOPE, obj_kwd());
            bad = true;
         }
         else
         {
            scope = ogc_scope::from_tokens(t, i, &next, err);
            if ( scope == OGC_NULL )
               bad = true;
         }
         continue;
      }

      if ( ogc_string::is_equal(arr[i].str, ogc_area_extent::obj_kwd()) ||
           ogc_string::is_equal(arr[i].str, ogc_bbox_extent::obj_kwd()) ||
           ogc_string::is_equal(arr[i].str, ogc_time_extent::obj_kwd()) ||
           ogc_string::is_equal(arr[i].str, ogc_vert_extent::obj_kwd()) )
      {
         extent = ogc_extent::from_tokens(t, i, &next, err);
         if ( extent == OGC_NULL )
         {
            bad = true;
         }
         else
         {
            if ( extents == OGC_NULL )
            {
               extents = ogc_vector::create(1, 1);
               if ( extents == OGC_NULL )
               {
                  ogc_error::set(err, OGC_ERR_NO_MEMORY, obj_kwd());
                  delete extent;
                  bad = true;
               }
            }

            if ( extents != OGC_NULL )
            {
               void * p = extents->find(
                             extent,
                             false,
                             ogc_utils::compare_extent);
               if ( p != OGC_NULL )
               {
                  ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_EXTENT,
                     obj_kwd(),
                     ogc_utils::obj_type_to_kwd(extent->obj_type()));
                  delete extent;
                  bad = true;
               }
               else
               {
                  if ( extents->add( extent ) < 0 )
                  {
                     ogc_error::set(err, OGC_ERR_NO_MEMORY, obj_kwd());
                     delete extent;
                     bad = true;
                  }
               }
            }
         }
         continue;
      }

      if ( ogc_string::is_equal(arr[i].str, ogc_id::obj_kwd()) ||
           ogc_string::is_equal(arr[i].str, ogc_id::alt_kwd()) )
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

      if ( ogc_string::is_equal(arr[i].str, ogc_remark::obj_kwd()) )
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
      obj = create(name, datum, primem, cs, axis_1, axis_2, unit,
                   scope, extents, ids, remark, err);
   }

   if ( obj == OGC_NULL )
   {
      ogc_geodetic_datum :: destroy( datum   );
      ogc_primem         :: destroy( primem  );
      ogc_cs             :: destroy( cs      );
      ogc_axis           :: destroy( axis_1  );
      ogc_axis           :: destroy( axis_2  );
      ogc_unit           :: destroy( unit    );
      ogc_scope          :: destroy( scope   );
      ogc_vector         :: destroy( extents );
      ogc_vector         :: destroy( ids     );
      ogc_remark         :: destroy( remark  );
   }

   return obj;
}

/*------------------------------------------------------------------------
 * object from WKT
 */
ogc_geog2d_crs * ogc_geog2d_crs :: from_wkt(
   const char * wkt,
   ogc_error *  err)
{
   ogc_geog2d_crs * obj = OGC_NULL;
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
bool ogc_geog2d_crs :: to_wkt(
   const ogc_geog2d_crs * obj,
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

bool ogc_geog2d_crs :: to_wkt(
   char      buffer[],
   int       options,
   size_t    buflen) const
{
   OGC_UTF8_NAME buf_name;
   OGC_TBUF      buf_hdr;
   OGC_TBUF      buf_datum;
   OGC_TBUF      buf_primem;
   OGC_TBUF      buf_cs;
   OGC_TBUF      buf_axis_1;
   OGC_TBUF      buf_axis_2;
   OGC_TBUF      buf_unit;
   OGC_TBUF      buf_extent;
   OGC_TBUF      buf_id;
   OGC_TBUF      buf_remark;
   int           opts  =  (options | OGC_WKT_OPT_INTERNAL);
   size_t        len   = 0;
   bool          rc    = true;
   const char *  opn   = "[";
   const char *  cls   = "]";
   const char *  kwd   = obj_kwd();
   ogc_primem *  primem = _primem;

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
   {
      kwd = old_kwd();
      if ( primem == OGC_NULL )
      {
         primem = ogc_primem::create("Greenwich", 0.0,
            OGC_NULL, OGC_NULL, OGC_NULL);
      }
   }

   rc &= ogc_geodetic_datum :: to_wkt(_datum,  buf_datum,  opts, OGC_TBUF_MAX);
   rc &= ogc_primem         :: to_wkt( primem, buf_primem, opts, OGC_TBUF_MAX);
   rc &= ogc_cs             :: to_wkt(_cs,     buf_cs,     opts, OGC_TBUF_MAX);
   rc &= ogc_axis           :: to_wkt(_axis_1, buf_axis_1, opts, OGC_TBUF_MAX);
   rc &= ogc_axis           :: to_wkt(_axis_2, buf_axis_2, opts, OGC_TBUF_MAX);
   rc &= ogc_unit           :: to_wkt(_unit,   buf_unit,   opts, OGC_TBUF_MAX);
   rc &= ogc_remark         :: to_wkt(_remark, buf_remark, opts, OGC_TBUF_MAX);

   ogc_string::escape_str(buf_name, _name, OGC_UTF8_NAME_MAX);
   sprintf(buf_hdr, "%s%s\"%s\"",
      kwd, opn, buf_name);

   OGC_CPY_TO_BUF( buf_hdr    );
   OGC_ADD_TO_BUF( buf_datum  );
   OGC_ADD_TO_BUF( buf_primem );
   OGC_ADD_TO_BUF( buf_cs     );
   OGC_ADD_TO_BUF( buf_axis_1 );
   OGC_ADD_TO_BUF( buf_axis_2 );
   OGC_ADD_TO_BUF( buf_unit   );

   if ( _extents != OGC_NULL && (options & OGC_WKT_OPT_OLD_SYNTAX) == 0 )
   {
      for (int i = 0; i < extent_count(); i++)
      {
         rc &= ogc_extent :: to_wkt(extent(i), buf_extent, opts, OGC_TBUF_MAX);
         OGC_ADD_TO_BUF( buf_extent );
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

   OGC_ADD_TO_BUF( buf_remark );
   OGC_CPY_TO_BUF( cls        );

   if ( (options & OGC_WKT_OPT_INTERNAL) == 0 &&
        (options & OGC_WKT_OPT_EXPAND)   != 0 )
   {
      rc &= ogc_utils :: expand_wkt(buffer, buffer, "", options, buflen);
   }

   if ( primem != _primem )
      ogc_primem::destroy(primem);

   return rc;
}

/*------------------------------------------------------------------------
 * clone
 */
ogc_geog2d_crs * ogc_geog2d_crs :: clone(const ogc_geog2d_crs * obj)
{
   if ( obj == OGC_NULL )
      return OGC_NULL;
   return obj->clone();
}

ogc_geog2d_crs * ogc_geog2d_crs :: clone() const
{
   ogc_angunit * u = reinterpret_cast<ogc_angunit *>(_unit);

   ogc_geodetic_datum * datum   = ogc_geodetic_datum :: clone( _datum   );
   ogc_primem *         primem  = ogc_primem         :: clone( _primem  );
   ogc_cs *             cs      = ogc_cs             :: clone( _cs      );
   ogc_axis *           axis_1  = ogc_axis           :: clone( _axis_1  );
   ogc_axis *           axis_2  = ogc_axis           :: clone( _axis_2  );
   ogc_angunit *        angunit = ogc_angunit        :: clone( u        );
   ogc_scope *          scope   = ogc_scope          :: clone( _scope   );
   ogc_vector *         extents = ogc_vector         :: clone( _extents );
   ogc_vector *         ids     = ogc_vector         :: clone( _ids     );
   ogc_remark *         remark  = ogc_remark         :: clone( _remark  );

   ogc_geog2d_crs * p = create(_name,
                               datum,
                               primem,
                               cs,
                               axis_1,
                               axis_2,
                               angunit,
                               scope,
                               extents,
                               ids,
                               remark,
                               OGC_NULL);
   if ( p == OGC_NULL )
   {
      ogc_geodetic_datum :: destroy( datum   );
      ogc_primem         :: destroy( primem  );
      ogc_cs             :: destroy( cs      );
      ogc_axis           :: destroy( axis_1  );
      ogc_axis           :: destroy( axis_2  );
      ogc_angunit        :: destroy( angunit );
      ogc_scope          :: destroy( scope   );
      ogc_vector         :: destroy( extents );
      ogc_vector         :: destroy( ids     );
      ogc_remark         :: destroy( remark  );
   }

   return p;
}

/*------------------------------------------------------------------------
 * compare for computational equality
 */
bool ogc_geog2d_crs :: is_equal(
   const ogc_geog2d_crs * p1,
   const ogc_geog2d_crs * p2)
{
   if ( p1 == OGC_NULL && p2 == OGC_NULL ) return true;
   if ( p1 == OGC_NULL || p2 == OGC_NULL ) return false;

   if ( !ogc_string         :: is_equal( p1->name(),    p2->name()    ) ||
        !ogc_geodetic_datum :: is_equal( p1->datum(),   p2->datum()   ) ||
        !ogc_primem         :: is_equal( p1->primem(),  p2->primem()  ) ||
        !ogc_cs             :: is_equal( p1->cs(),      p2->cs()      ) ||
        !ogc_axis           :: is_equal( p1->axis_1(),  p2->axis_1()  ) ||
        !ogc_axis           :: is_equal( p1->axis_2(),  p2->axis_2()  ) ||
        !ogc_angunit        :: is_equal( p1->angunit(), p2->angunit() ) )
   {
      return false;
   }

   return true;
}

bool ogc_geog2d_crs :: is_equal(
   const ogc_geog2d_crs * p) const
{
   return is_equal(this, p);
}

/*------------------------------------------------------------------------
 * compare
 */
bool ogc_geog2d_crs :: is_identical(
   const ogc_geog2d_crs * p1,
   const ogc_geog2d_crs * p2)
{
   if ( p1 == OGC_NULL && p2 == OGC_NULL ) return true;
   if ( p1 == OGC_NULL || p2 == OGC_NULL ) return false;

   if ( !ogc_string         :: is_equal    ( p1->name(),    p2->name()    ) ||
        !ogc_geodetic_datum :: is_identical( p1->datum(),   p2->datum()   ) ||
        !ogc_primem         :: is_identical( p1->primem(),  p2->primem()  ) ||
        !ogc_cs             :: is_identical( p1->cs(),      p2->cs()      ) ||
        !ogc_axis           :: is_identical( p1->axis_1(),  p2->axis_1()  ) ||
        !ogc_axis           :: is_identical( p1->axis_2(),  p2->axis_2()  ) ||
        !ogc_angunit        :: is_identical( p1->angunit(), p2->angunit() ) ||
        !ogc_scope          :: is_identical( p1->scope(),   p2->scope()   ) ||
        !ogc_vector         :: is_identical( p1->extents(), p2->extents() ) ||
        !ogc_vector         :: is_identical( p1->ids(),     p2->ids()     ) ||
        !ogc_remark         :: is_identical( p1->remark(),  p2->remark()  ) )
   {
      return false;
   }

   return true;
}

bool ogc_geog2d_crs :: is_identical(
   const ogc_geog2d_crs * p) const
{
   return is_identical(this, p);
}

/* ------------------------------------------------------------------------- */
/* BASE_CRS routines                                                         */
/* ------------------------------------------------------------------------- */

/*------------------------------------------------------------------------
 * static create (BASE_CRS)
 */
ogc_geog2d_crs * ogc_geog2d_crs :: create_base_crs(
   const char *         name,
   ogc_geodetic_datum * datum,
   ogc_primem *         primem,
   ogc_angunit *        angunit,
   ogc_error *          err)
{
   ogc_geog2d_crs * p = OGC_NULL;
   bool bad = false;

   /*---------------------------------------------------------
    * sanity checks
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
         ogc_error::set(err, OGC_ERR_NAME_TOO_LONG, alt_kwd(), len);
         bad = true;
      }
   }

   if ( datum == OGC_NULL )
   {
      ogc_error::set(err, OGC_ERR_MISSING_DATUM, alt_kwd());
      bad = true;
   }

   /*---------------------------------------------------------
    * create the object
    */
   if ( !bad )
   {
      p = new (std::nothrow) ogc_geog2d_crs();
      if ( p == OGC_NULL )
      {
         ogc_error::set(err, OGC_ERR_NO_MEMORY, alt_kwd());
         return p;
      }

      ogc_string::strncpy(p->_name, name, OGC_NAME_MAX);
      p->_obj_type = OGC_OBJ_TYPE_GEOG2D_CRS;
      p->_crs_type = OGC_CRS_TYPE_GEOG2D;
      p->_datum    = datum;
      p->_primem   = primem;
      p->_cs       = OGC_NULL;
      p->_axis_1   = OGC_NULL;
      p->_axis_2   = OGC_NULL;
      p->_axis_3   = OGC_NULL;
      p->_unit     = angunit;
      p->_scope    = OGC_NULL;
      p->_extents  = OGC_NULL;
      p->_ids      = OGC_NULL;
      p->_remark   = OGC_NULL;
   }

   return p;
}

/*------------------------------------------------------------------------
 * static object from tokens (BASE_CRS)
 */
ogc_geog2d_crs * ogc_geog2d_crs :: from_tokens_base_crs(
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

   ogc_geog2d_crs *      obj    = OGC_NULL;
   ogc_geodetic_datum *  datum  = OGC_NULL;
   ogc_primem *          primem = OGC_NULL;
   ogc_angunit *         unit   = OGC_NULL;
   const char * name;

   /*---------------------------------------------------------
    * sanity checks
    */
   if ( t == OGC_NULL )
   {
      ogc_error::set(err, OGC_ERR_WKT_EMPTY_STRING, alt_kwd());
      return OGC_NULL;
   }
   arr = t->_arr;

   if ( start < 0 || start >= t->_num )
   {
      ogc_error::set(err, OGC_ERR_WKT_INDEX_OUT_OF_RANGE, alt_kwd(), start);
      return OGC_NULL;
   }
   kwd = arr[start].str;

   if ( !ogc_string::is_equal(kwd, alt_kwd()) &&
        !ogc_string::is_equal(kwd, old_kwd()) )
   {
      ogc_error::set(err, OGC_ERR_WKT_INVALID_KEYWORD, alt_kwd(), kwd);
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
    * There must be 1 token: BASECRS[ "name" ...
    */
   if ( same < 1 )
   {
      ogc_error::set(err, OGC_ERR_WKT_INSUFFICIENT_TOKENS, alt_kwd(), same);
      return OGC_NULL;
   }

   if ( same > 1 && ogc_object::get_strict_parsing() )
   {
      ogc_error::set(err, OGC_ERR_WKT_TOO_MANY_TOKENS,     alt_kwd(), same);
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
      if ( ogc_string::is_equal(arr[i].str, ogc_geodetic_datum::obj_kwd()) )
      {
         if ( datum != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_DATUM, alt_kwd());
            bad = true;
         }
         else
         {
            datum = ogc_geodetic_datum::from_tokens(t, i, &next, err);
            if ( datum == OGC_NULL )
               bad = true;
         }
         continue;
      }

      if ( ogc_string::is_equal(arr[i].str, ogc_primem::obj_kwd()) )
      {
         if ( primem != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_PRIMEM, alt_kwd());
            bad = true;
         }
         else
         {
            if ( ogc_string::is_equal(kwd, old_kwd()) )
               primem = ogc_primem::from_tokens_geogcs(t, i, &next, err);
            else
               primem = ogc_primem::from_tokens       (t, i, &next, err);
            if ( primem == OGC_NULL )
               bad = true;
         }
         continue;
      }

      if ( ogc_string::is_equal(arr[i].str, ogc_angunit::obj_kwd()) ||
           ogc_string::is_equal(arr[i].str, ogc_angunit::alt_kwd()) )
      {
         if ( unit != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_UNIT, alt_kwd());
            bad = true;
         }
         else
         {
            unit = ogc_angunit::from_tokens(t, i, &next, err);
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
      obj = create_base_crs(name, datum, primem, unit, err);
   }

   if ( obj == OGC_NULL )
   {
      ogc_geodetic_datum :: destroy( datum  );
      ogc_primem         :: destroy( primem );
      ogc_unit           :: destroy( unit   );
   }

   return obj;
}

/*------------------------------------------------------------------------
 * static object to WKT (BASE_CRS)
 */
bool ogc_geog2d_crs :: to_wkt_base_crs(
   const ogc_geog2d_crs * obj,
   char      buffer[],
   int       options,
   size_t    buflen)
{
   if ( buffer == OGC_NULL )
      return false;
   *buffer = 0;

   if ( obj == OGC_NULL )
      return false;

   return obj->to_wkt_base_crs(buffer, options, buflen);
}

/*------------------------------------------------------------------------
 * object to WKT (BASE_CRS)
 */
bool ogc_geog2d_crs :: to_wkt_base_crs(
   char      buffer[],
   int       options,
   size_t    buflen) const
{
   OGC_UTF8_NAME buf_name;
   OGC_TBUF      buf_hdr;
   OGC_TBUF      buf_datum;
   OGC_TBUF      buf_primem;
   OGC_TBUF      buf_unit;
   int           opts   =  (options | OGC_WKT_OPT_INTERNAL);
   size_t        len    = 0;
   bool          rc     = true;
   const char *  opn    = "[";
   const char *  cls    = "]";
   const char *  kwd    = alt_kwd();
   ogc_primem *  primem = _primem;

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
   {
      kwd = old_kwd();
      if ( primem == OGC_NULL )
      {
         primem = ogc_primem::create("Greenwich", 0.0,
            OGC_NULL, OGC_NULL, OGC_NULL);
      }
   }

   rc &= ogc_geodetic_datum :: to_wkt(_datum,  buf_datum,  opts, OGC_TBUF_MAX);
   rc &= ogc_primem         :: to_wkt( primem, buf_primem, opts, OGC_TBUF_MAX);
   rc &= ogc_unit           :: to_wkt(_unit,   buf_unit,   opts, OGC_TBUF_MAX);

   ogc_string::escape_str(buf_name, _name, OGC_UTF8_NAME_MAX);
   sprintf(buf_hdr, "%s%s\"%s\"",
      kwd, opn, buf_name);

   OGC_CPY_TO_BUF( buf_hdr    );
   OGC_ADD_TO_BUF( buf_datum  );
   OGC_ADD_TO_BUF( buf_primem );
   OGC_ADD_TO_BUF( buf_unit   );
   OGC_CPY_TO_BUF( cls        );

   if ( (options & OGC_WKT_OPT_INTERNAL) == 0 &&
        (options & OGC_WKT_OPT_EXPAND)   != 0 )
   {
      rc &= ogc_utils::expand_wkt(buffer, buffer, "", options, buflen);
   }

   if ( primem != _primem )
      ogc_primem::destroy(primem);

   return rc;
}

/* ------------------------------------------------------------------------- */
/* GEOGCS routines                                                           */
/* ------------------------------------------------------------------------- */

/*------------------------------------------------------------------------
 * static object from tokens (GEOGCS)
 */
ogc_geog2d_crs * ogc_geog2d_crs :: from_tokens_geogcs(
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

   ogc_geog2d_crs *      obj     = OGC_NULL;
   ogc_geodetic_datum *  datum   = OGC_NULL;
   ogc_primem *          primem  = OGC_NULL;
   ogc_cs *              cs      = OGC_NULL;
   ogc_axis *            axis_1  = OGC_NULL;
   ogc_axis *            axis_2  = OGC_NULL;
   ogc_angunit *         unit    = OGC_NULL;
   ogc_id *              id      = OGC_NULL;
   ogc_vector *          ids     = OGC_NULL;
   ogc_remark *          remark  = OGC_NULL;
   const char * name;

   /*---------------------------------------------------------
    * sanity checks
    */
   if ( t == OGC_NULL )
   {
      ogc_error::set(err, OGC_ERR_WKT_EMPTY_STRING, old_kwd());
      return OGC_NULL;
   }
   arr = t->_arr;

   if ( start < 0 || start >= t->_num )
   {
      ogc_error::set(err, OGC_ERR_WKT_INDEX_OUT_OF_RANGE, old_kwd(), start);
      return OGC_NULL;
   }
   kwd = arr[start].str;

   if ( !ogc_string::is_equal(kwd, old_kwd()) )
   {
      ogc_error::set(err, OGC_ERR_WKT_INVALID_KEYWORD, old_kwd(), kwd);
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
    * There must be 1 token: GEOGCS[ "name" ...
    */
   if ( same < 1 )
   {
      ogc_error::set(err, OGC_ERR_WKT_INSUFFICIENT_TOKENS, old_kwd(), same);
      return OGC_NULL;
   }

   if ( same > 1 && ogc_object::get_strict_parsing() )
   {
      ogc_error::set(err, OGC_ERR_WKT_TOO_MANY_TOKENS,     old_kwd(), same);
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
      if ( ogc_string::is_equal(arr[i].str, ogc_geodetic_datum::obj_kwd()) )
      {
         if ( datum != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_DATUM, old_kwd());
            bad = true;
         }
         else
         {
            datum = ogc_geodetic_datum::from_tokens(t, i, &next, err);
            if ( datum == OGC_NULL )
               bad = true;
         }
         continue;
      }

      if ( ogc_string::is_equal(arr[i].str, ogc_primem::obj_kwd()) )
      {
         if ( primem != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_PRIMEM, old_kwd());
            bad = true;
         }
         else
         {
            primem = ogc_primem::from_tokens_geogcs(t, i, &next, err);
            if ( primem == OGC_NULL )
               bad = true;
         }
         continue;
      }

      if ( ogc_string::is_equal(arr[i].str, ogc_angunit::obj_kwd()) ||
           ogc_string::is_equal(arr[i].str, ogc_angunit::alt_kwd()) )
      {
         if ( unit != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_UNIT, old_kwd());
            bad = true;
         }
         else
         {
            unit = ogc_angunit::from_tokens(t, i, &next, err);
            if ( unit == OGC_NULL )
               bad = true;
         }
         continue;
      }

      if ( ogc_string::is_equal(arr[i].str, ogc_id::obj_kwd()) ||
           ogc_string::is_equal(arr[i].str, ogc_id::alt_kwd()) )
      {
         if ( ids == OGC_NULL )
         {
            ids = ogc_vector::create(1,1);
            if ( ids == OGC_NULL )
            {
               ogc_error::set(err, OGC_ERR_NO_MEMORY, old_kwd());
               bad = true;
            }
         }

         if ( ids != OGC_NULL )
         {
            id = ogc_id::from_tokens(t, i, &next, err);
            if ( id == OGC_NULL )
            {
               bad = true;
            }
            else
            {
               if ( ids->add( id ) < 0 )
               {
                  delete id;
                  ogc_error::set(err, OGC_ERR_NO_MEMORY, old_kwd());
                  bad = true;
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
    * Create a default CS
    */
   if ( !bad )
   {
      cs = ogc_cs::create(OGC_CS_TYPE_ELLIPSOIDAL, 2, OGC_NULL, err);
      if ( cs == OGC_NULL )
      {
         bad = true;
      }
   }

   /*---------------------------------------------------------
    * Create the object
    */
   if ( !bad )
   {
      obj = create(name, datum, primem, cs, axis_1, axis_2, unit,
                   OGC_NULL, OGC_NULL, ids, remark, err);
   }

   if ( obj == OGC_NULL )
   {
      ogc_geodetic_datum :: destroy( datum   );
      ogc_primem         :: destroy( primem  );
      ogc_cs             :: destroy( cs      );
      ogc_axis           :: destroy( axis_1  );
      ogc_axis           :: destroy( axis_2  );
      ogc_unit           :: destroy( unit    );
      ogc_vector         :: destroy( ids     );
      ogc_remark         :: destroy( remark  );
   }

   return obj;
}

} /* namespace OGC */
