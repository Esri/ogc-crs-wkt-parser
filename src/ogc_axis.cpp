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
/* AXIS object                                                               */
/* ------------------------------------------------------------------------- */

#include "ogc_common.h"

namespace OGC {

const char * ogc_axis :: obj_kwd() { return OGC_OBJ_KWD_AXIS; }

/*------------------------------------------------------------------------
 * create
 */
ogc_axis * ogc_axis :: create(
   const char *        name,
   const char *        abbr,
   ogc_axis_direction  direction,
   ogc_bearing *       bearing,
   ogc_meridian *      meridian,
   ogc_order *         order,
   ogc_unit *          unit,
   ogc_vector *        ids,
   ogc_error *         err)
{
   ogc_axis * p = OGC_NULL;
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

   if ( abbr == OGC_NULL )
   {
      abbr = "";
   }
   else
   {
      int len = ogc_string::unescape_len(abbr);
      if ( len >= OGC_NAME_MAX )
      {
         ogc_error::set(err, OGC_ERR_ABBR_TOO_LONG, obj_kwd(), len);
         bad = true;
      }
   }

   if ( !ogc_utils::axis_direction_valid(direction) )
   {
      ogc_error::set(err, OGC_ERR_INVALID_AXIS_DIRECTION,
         obj_kwd(), static_cast<int>(direction));
      bad = true;
   }

   /*---------------------------------------------------------
    * create the object
    */
   if ( !bad )
   {
      p = new (std::nothrow) ogc_axis();
      if ( p == OGC_NULL )
      {
         ogc_error::set(err, OGC_ERR_NO_MEMORY,
            obj_kwd());
         return p;
      }

      ogc_string::unescape_str(p->_name, name, OGC_NAME_MAX);
      ogc_string::unescape_str(p->_abbr, abbr, OGC_NAME_MAX);
      p->_obj_type  = OGC_OBJ_TYPE_AXIS;
      p->_direction = direction;
      p->_bearing   = bearing;
      p->_meridian  = meridian;
      p->_order     = order;
      p->_unit      = unit;
      p->_ids       = ids;
   }

   return p;
}

/*------------------------------------------------------------------------
 * destroy
 */
ogc_axis :: ~ogc_axis()
{
   ogc_order  :: destroy( _order );
   ogc_unit   :: destroy( _unit  );
   ogc_vector :: destroy( _ids   );
}

void ogc_axis :: destroy(
   ogc_axis * obj)
{
   if ( obj != OGC_NULL )
   {
      delete obj;
   }
}

/*------------------------------------------------------------------------
 * object from tokens
 */
ogc_axis * ogc_axis :: from_tokens(
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

   ogc_axis *      obj      = OGC_NULL;
   ogc_bearing *   bearing  = OGC_NULL;
   ogc_meridian *  meridian = OGC_NULL;
   ogc_order *     order    = OGC_NULL;
   ogc_unit *      unit     = OGC_NULL;
   ogc_id *        id       = OGC_NULL;
   ogc_vector *    ids      = OGC_NULL;
   ogc_axis_direction direction;
   OGC_NAME name_buf;
   OGC_NAME abbr_buf;

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
    * There must be 2 tokens: AXIS[ "name", direction ...
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
    *
    * "name" may be:
    *    "name"
    *    "name (abbr)"
    *    "(abbr)"
    */
   ogc_string::strncpy(name_buf, arr[start++].str, OGC_NAME_MAX);

   char * s = strchr(name_buf, '(');
   if ( s != OGC_NULL )
   {
      *s++ = 0;
      strcpy(abbr_buf, s);
      char *s = strchr(abbr_buf, ')');
      if ( s != OGC_NULL )
         *s = 0;

      ogc_string::trim_buf( name_buf );
      ogc_string::trim_buf( abbr_buf );
   }
   else
   {
      *abbr_buf = 0;
   }

   direction = ogc_utils::axis_kwd_to_direction( arr[start++].str );

   if ( !ogc_utils::axis_direction_valid(direction) )
   {
      ogc_error::set(err, OGC_ERR_INVALID_AXIS_DIRECTION,
         obj_kwd(), arr[start-1].str);
      bad = true;
   }

   /*---------------------------------------------------------
    * Now process all sub-objects
    */
   int  next = 0;
   for (int i = start; i < end; i = next)
   {
      if ( ogc_string::is_equal(arr[i].str, ogc_bearing::obj_kwd()) )
      {
         if ( bearing != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_BEARING, obj_kwd());
            bad = true;
         }
         else
         {
            bearing = ogc_bearing::from_tokens(t, i, &next, err);
            if ( bearing == OGC_NULL )
               bad = true;
         }
         continue;
      }

      if ( ogc_string::is_equal(arr[i].str, ogc_meridian::obj_kwd()) )
      {
         if ( meridian != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_MERIDIAN, obj_kwd());
            bad = true;
         }
         else
         {
            meridian = ogc_meridian::from_tokens(t, i, &next, err);
            if ( meridian == OGC_NULL )
               bad = true;
         }
         continue;
      }

      if ( ogc_string::is_equal(arr[i].str, ogc_order::obj_kwd()) )
      {
         if ( order != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_ORDER, obj_kwd());
            bad = true;
         }
         else
         {
            order = ogc_order::from_tokens(t, i, &next, err);
            if ( order == OGC_NULL )
               bad = true;
         }
         continue;
      }

      if ( ogc_string::is_equal(arr[i].str, ogc_unit     ::obj_kwd()) ||
           ogc_string::is_equal(arr[i].str, ogc_angunit  ::obj_kwd()) ||
           ogc_string::is_equal(arr[i].str, ogc_lenunit  ::obj_kwd()) ||
           ogc_string::is_equal(arr[i].str, ogc_paramunit::obj_kwd()) ||
           ogc_string::is_equal(arr[i].str, ogc_scaleunit::obj_kwd()) ||
           ogc_string::is_equal(arr[i].str, ogc_timeunit ::obj_kwd()) )
      {
         if ( unit != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_UNIT, obj_kwd());
            bad = true;
         }
         else
         {
            unit = ogc_unit::from_tokens(t, i, &next, err);
            if ( unit == OGC_NULL )
               bad = true;
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
    * Create the object
    */
   if ( !bad )
   {
      obj = create(name_buf, abbr_buf, direction, bearing, meridian, order,
                   unit, ids, err);
   }

   if ( obj == OGC_NULL )
   {
      ogc_bearing  :: destroy( bearing  );
      ogc_meridian :: destroy( meridian );
      ogc_order    :: destroy( order    );
      ogc_unit     :: destroy( unit     );
      ogc_vector   :: destroy( ids      );
   }

   return obj;
}

/*------------------------------------------------------------------------
 * object from WKT
 */
ogc_axis * ogc_axis :: from_wkt(
   const char * wkt,
   ogc_error *  err)
{
   ogc_axis * obj = OGC_NULL;
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
bool ogc_axis :: to_wkt(
   const ogc_axis * obj,
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

bool ogc_axis :: to_wkt(
   char      buffer[],
   int       options,
   size_t    buflen) const
{
   OGC_TBUF      buf_hdr;
   OGC_TBUF      buf_bearing;
   OGC_TBUF      buf_meridian;
   OGC_TBUF      buf_order;
   OGC_TBUF      buf_unit;
   OGC_TBUF      buf_id;
   OGC_NAME      buf_name;
   const char *  axis_dir;
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

   name_and_abbr(buf_name);
   axis_dir = ogc_utils::axis_direction_to_kwd(_direction);

   rc &= ogc_bearing  :: to_wkt(_bearing,  buf_bearing,  opts, OGC_TBUF_MAX);
   rc &= ogc_meridian :: to_wkt(_meridian, buf_meridian, opts, OGC_TBUF_MAX);
   rc &= ogc_order    :: to_wkt(_order,    buf_order,    opts, OGC_TBUF_MAX);

   if ( (opts & OGC_WKT_OPT_OLD_SYNTAX) != 0 )
      rc &= ogc_unit :: to_wkt(_unit, buf_unit, opts, OGC_TBUF_MAX);
   else
      *buf_unit = 0;

   sprintf(buf_hdr, "%s%s\"%s\",%s",
      kwd, opn, buf_name, axis_dir);

   OGC_CPY_TO_BUF( buf_hdr      );
   OGC_ADD_TO_BUF( buf_bearing  );
   OGC_ADD_TO_BUF( buf_meridian );
   OGC_ADD_TO_BUF( buf_order    );
   OGC_ADD_TO_BUF( buf_unit     );

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
ogc_axis * ogc_axis :: clone(const ogc_axis * obj)
{
   if ( obj == OGC_NULL )
      return OGC_NULL;
   return obj->clone();
}

ogc_axis * ogc_axis :: clone() const
{
   ogc_bearing *  bearing  = ogc_bearing  :: clone( _bearing  );
   ogc_meridian * meridian = ogc_meridian :: clone( _meridian );
   ogc_order *    order    = ogc_order    :: clone( _order    );
   ogc_unit *     unit     = ogc_unit     :: clone( _unit     );
   ogc_vector *   ids      = ogc_vector   :: clone( _ids      );

   ogc_axis * p = create(_name,
                         _abbr,
                         _direction,
                         bearing,
                         meridian,
                         order,
                         unit,
                         ids,
                         OGC_NULL);
   if ( p == OGC_NULL )
   {
      ogc_bearing  :: destroy( bearing  );
      ogc_meridian :: destroy( meridian );
      ogc_order    :: destroy( order    );
      ogc_unit     :: destroy( unit     );
      ogc_vector   :: destroy( ids      );
   }

   return p;
}

/*------------------------------------------------------------------------
 * compare for computational equality
 */
bool ogc_axis :: is_equal(
   const ogc_axis * p1,
   const ogc_axis * p2)
{
   if ( p1 == OGC_NULL && p2 == OGC_NULL ) return true;
   if ( p1 == OGC_NULL || p2 == OGC_NULL ) return false;

   if ( !ogc_string   :: is_equal( p1->name(),     p2->name()     ) ||
        !ogc_bearing  :: is_equal( p1->bearing(),  p2->bearing()  ) ||
        !ogc_meridian :: is_equal( p1->meridian(), p2->meridian() ) ||
        !ogc_order    :: is_equal( p1->order(),    p2->order()    ) ||
        !ogc_unit     :: is_equal( p1->unit(),     p2->unit()     ) )
   {
      return false;
   }

   return true;
}

bool ogc_axis :: is_equal(
   const ogc_axis * p) const
{
   return is_equal(this, p);
}

/*------------------------------------------------------------------------
 * compare
 */
bool ogc_axis :: is_identical(
   const ogc_axis * p1,
   const ogc_axis * p2)
{
   if ( p1 == OGC_NULL && p2 == OGC_NULL ) return true;
   if ( p1 == OGC_NULL || p2 == OGC_NULL ) return false;

   if ( !ogc_string   :: is_equal    ( p1->name(),     p2->name()     ) ||
        !ogc_bearing  :: is_identical( p1->bearing(),  p2->bearing()  ) ||
        !ogc_meridian :: is_identical( p1->meridian(), p2->meridian() ) ||
        !ogc_order    :: is_identical( p1->order(),    p2->order()    ) ||
        !ogc_unit     :: is_identical( p1->unit(),     p2->unit()     ) ||
        !ogc_vector   :: is_identical( p1->ids(),      p2->ids()      ) )
   {
      return false;
   }

   return true;
}

bool ogc_axis :: is_identical(
   const ogc_axis * p) const
{
   return is_identical(this, p);
}

/*------------------------------------------------------------------------
 * get the name and abbreviation
 */
void ogc_axis :: name_and_abbr(OGC_NAME buf) const
{
   if ( buf != OGC_NULL )
   {
      if      ( *_name != 0 && *_abbr != 0 )
         sprintf(buf, "%s (%s)", _name, _abbr);
      else if ( *_name != 0 )
         sprintf(buf, "%s", _name);
      else if ( *_abbr != 0 )
         sprintf(buf, "(%s)", _abbr);
      else
         *buf = 0;
   }
}

/*------------------------------------------------------------------------
 * get ID count
 */
int ogc_axis :: id_count() const
{
   return (_ids == OGC_NULL) ? 0 : _ids->length();
}

/*------------------------------------------------------------------------
 * get the nth ID
 */
ogc_id * ogc_axis :: id(int n) const
{
   return (_ids == OGC_NULL) ? OGC_NULL :
                               reinterpret_cast<ogc_id *>( _ids->get(n) );
}

} /* namespace OGC */
