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
/* COORDOP (Coordinate operation) object                                     */
/* ------------------------------------------------------------------------- */

#include "ogc_common.h"

namespace OGC {

const char * ogc_coord_op :: obj_kwd() { return OGC_OBJ_KWD_COORD_OP; }

/*------------------------------------------------------------------------
 * create
 */
ogc_coord_op * ogc_coord_op :: create(
   const char *      name,
   ogc_crs *         source_crs,
   ogc_crs *         target_crs,
   ogc_crs *         interp_crs,
   ogc_method *      method,
   ogc_vector *      parameters,
   ogc_vector *      param_files,
   ogc_op_accuracy * op_accuracy,
   ogc_scope *       scope,
   ogc_vector *      extents,
   ogc_vector *      ids,
   ogc_remark *      remark,
   ogc_error *       err)
{
   ogc_coord_op * p = OGC_NULL;
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
      p = new (std::nothrow) ogc_coord_op();
      if ( p == OGC_NULL )
      {
         ogc_error::set(err, OGC_ERR_NO_MEMORY, obj_kwd());
         return p;
      }

      ogc_string::unescape_str(p->_name, name, OGC_NAME_MAX);
      p->_obj_type        = OGC_OBJ_TYPE_COORD_OP;
      p->_source_crs      = source_crs;
      p->_target_crs      = target_crs;
      p->_interp_crs      = interp_crs;
      p->_method          = method;
      p->_parameters      = parameters;
      p->_param_files     = param_files;
      p->_op_accuracy     = op_accuracy;
      p->_scope           = scope;
      p->_extents         = extents;
      p->_ids             = ids;
      p->_remark          = remark;
   }

   return p;
}

/*------------------------------------------------------------------------
 * destroy
 */
ogc_coord_op :: ~ogc_coord_op()
{
   ogc_crs         :: destroy( _source_crs  );
   ogc_crs         :: destroy( _target_crs  );
   ogc_crs         :: destroy( _interp_crs  );
   ogc_method      :: destroy( _method      );
   ogc_vector      :: destroy( _parameters  );
   ogc_vector      :: destroy( _param_files );
   ogc_op_accuracy :: destroy( _op_accuracy );
   ogc_scope       :: destroy( _scope       );
   ogc_vector      :: destroy( _extents     );
   ogc_vector      :: destroy( _ids         );
   ogc_remark      :: destroy( _remark      );
}

void ogc_coord_op :: destroy(
   ogc_coord_op * obj)
{
   if ( obj != OGC_NULL )
   {
      delete obj;
   }
}

/*------------------------------------------------------------------------
 * object from tokens
 */
ogc_coord_op * ogc_coord_op :: from_tokens(
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

   ogc_coord_op *    obj         = OGC_NULL;
   ogc_crs *         source_crs  = OGC_NULL;
   ogc_crs *         target_crs  = OGC_NULL;
   ogc_crs *         interp_crs  = OGC_NULL;
   ogc_method *      method      = OGC_NULL;
   ogc_vector *      parameters  = OGC_NULL;
   ogc_vector *      param_files = OGC_NULL;
   ogc_op_accuracy * op_accuracy = OGC_NULL;
   ogc_scope *       scope       = OGC_NULL;
   ogc_extent *      extent      = OGC_NULL;
   ogc_vector *      extents     = OGC_NULL;
   ogc_id *          id          = OGC_NULL;
   ogc_vector *      ids         = OGC_NULL;
   ogc_remark *      remark      = OGC_NULL;
   ogc_parameter *   param;
   ogc_param_file *  param_file;
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
    * There must be 1 token: COORDOP[ "name" ...
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

      if ( ogc_string::is_equal(arr[i].str, OGC_OBJ_KWD_INTERP_CRS) )
      {
         if ( interp_crs != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_INTERP_CRS, obj_kwd());
            bad = true;
         }
         else
         {
            interp_crs = ogc_crs::from_tokens(t, i+1, &next, err);
            if ( interp_crs == OGC_NULL )
               bad = true;
         }
         continue;
      }

      if ( ogc_string::is_equal(arr[i].str, ogc_method::obj_kwd()) )
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

      /* must check PARAMETERFILE before PARAMETER */
      if ( ogc_string::is_equal(arr[i].str, ogc_param_file::obj_kwd()) )
      {
         param_file = ogc_param_file::from_tokens(t, i, &next, err);
         if ( param_file == OGC_NULL )
         {
            bad = true;
         }
         else
         {
            if ( param_files == OGC_NULL )
            {
               param_files = ogc_vector::create(1, 1);
               if ( param_files == OGC_NULL )
               {
                  ogc_error::set(err, OGC_ERR_NO_MEMORY, obj_kwd());
                  delete param_file;
                  bad = true;
               }
            }

            if ( param_files != OGC_NULL )
            {
               void * p = param_files->find(
                             param_file,
                             false,
                             ogc_utils::compare_param_file);
               if ( p != OGC_NULL )
               {
                  ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_PARAM_FILE,
                     obj_kwd(), param_file->name());
                  delete param_file;
                  bad = true;
               }
               else
               {
                  if ( param_files->add( param_file ) < 0 )
                  {
                     ogc_error::set(err, OGC_ERR_NO_MEMORY, obj_kwd());
                     delete param_file;
                     bad = true;
                  }
               }
            }
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

      if ( ogc_string::is_equal(arr[i].str, ogc_op_accuracy::obj_kwd()) )
      {
         if ( op_accuracy != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_OPACCURACY, obj_kwd());
            bad = true;
         }
         else
         {
            op_accuracy = ogc_op_accuracy::from_tokens(t, i, &next, err);
            if ( op_accuracy == OGC_NULL )
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
      obj = create(name, source_crs, target_crs, interp_crs,
                   method, parameters, param_files,
                   op_accuracy, scope, extents, ids, remark, err);
   }

   if ( obj == OGC_NULL )
   {
      ogc_crs         :: destroy( source_crs  );
      ogc_crs         :: destroy( target_crs  );
      ogc_crs         :: destroy( interp_crs  );
      ogc_method      :: destroy( method      );
      ogc_vector      :: destroy( parameters  );
      ogc_vector      :: destroy( param_files );
      ogc_op_accuracy :: destroy( op_accuracy );
      ogc_scope       :: destroy( scope       );
      ogc_vector      :: destroy( extents     );
      ogc_vector      :: destroy( ids         );
      ogc_remark      :: destroy( remark      );
   }

   return obj;
}

/*------------------------------------------------------------------------
 * object from WKT
 */
ogc_coord_op * ogc_coord_op :: from_wkt(
   const char * wkt,
   ogc_error *  err)
{
   ogc_coord_op * obj = OGC_NULL;
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
bool ogc_coord_op :: to_wkt(
   const ogc_coord_op * obj,
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

bool ogc_coord_op :: to_wkt(
   char      buffer[],
   int       options,
   size_t    buflen) const
{
   OGC_UTF8_NAME buf_name;
   OGC_TBUF      buf_hdr;
   OGC_BUFF      buf_source_crs;
   OGC_BUFF      buf_source;
   OGC_BUFF      buf_target_crs;
   OGC_BUFF      buf_target;
   OGC_BUFF      buf_interp_crs;
   OGC_BUFF      buf_interp;
   OGC_TBUF      buf_method;
   OGC_TBUF      buf_parameter;
   OGC_TBUF      buf_op_accuracy;
   OGC_TBUF      buf_scope;
   OGC_TBUF      buf_extent;
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

   rc &= ogc_crs    :: to_wkt(_source_crs, buf_source_crs, opts, OGC_BUFF_MAX-8);
   rc &= ogc_crs    :: to_wkt(_target_crs, buf_target_crs, opts, OGC_BUFF_MAX-8);
   rc &= ogc_crs    :: to_wkt(_interp_crs, buf_interp_crs, opts, OGC_BUFF_MAX-8);
   rc &= ogc_method :: to_wkt(_method, buf_method,         opts, OGC_TBUF_MAX);
   rc &= ogc_op_accuracy  :: to_wkt(_op_accuracy,  buf_op_accuracy, opts, OGC_TBUF_MAX);
   rc &= ogc_scope  :: to_wkt(_scope,  buf_scope,          opts, OGC_TBUF_MAX);
   rc &= ogc_remark :: to_wkt(_remark, buf_remark,         opts, OGC_TBUF_MAX);

   sprintf(buf_source, "%s%s%s%s",
      OGC_OBJ_KWD_SOURCE_CRS, opn, buf_source_crs, cls);

   sprintf(buf_target, "%s%s%s%s",
      OGC_OBJ_KWD_TARGET_CRS, opn, buf_target_crs, cls);

   if ( _interp_crs != OGC_NULL )
   {
      sprintf(buf_interp, "%s%s%s%s",
         OGC_OBJ_KWD_TARGET_CRS, opn, buf_interp_crs, cls);
   }
   else
   {
      *buf_interp = 0;
   }

   ogc_string::escape_str(buf_name, _name, OGC_UTF8_NAME_MAX);
   sprintf(buf_hdr, "%s%s\"%s\"",
      kwd, opn, buf_name);

   OGC_CPY_TO_BUF( buf_hdr    );
   OGC_ADD_TO_BUF( buf_source );
   OGC_ADD_TO_BUF( buf_target );
   OGC_ADD_TO_BUF( buf_method );

   if ( _parameters != OGC_NULL )
   {
      for (int i = 0; i < parameter_count(); i++)
      {
         rc &= ogc_parameter :: to_wkt(parameter(i),
                                       buf_parameter, opts, OGC_TBUF_MAX);
         OGC_ADD_TO_BUF( buf_parameter );
      }
   }

   if ( _param_files != OGC_NULL )
   {
      for (int i = 0; i < param_file_count(); i++)
      {
         rc &= ogc_param_file :: to_wkt(param_file(i),
                                        buf_parameter, opts, OGC_TBUF_MAX);
         OGC_ADD_TO_BUF( buf_parameter );
      }
   }

   OGC_ADD_TO_BUF( buf_interp      );
   OGC_ADD_TO_BUF( buf_op_accuracy );
   OGC_ADD_TO_BUF( buf_scope       );

   if ( _extents != OGC_NULL )
   {
      for (int i = 0; i < extent_count(); i++)
      {
         rc &= ogc_extent :: to_wkt(extent(i),
                                    buf_extent, opts, OGC_TBUF_MAX);
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

   return rc;
}

/*------------------------------------------------------------------------
 * clone
 */
ogc_coord_op * ogc_coord_op :: clone(const ogc_coord_op * obj)
{
   if ( obj == OGC_NULL )
      return OGC_NULL;
   return obj->clone();
}

ogc_coord_op * ogc_coord_op :: clone() const
{
   ogc_crs *         source_crs  = ogc_crs         :: clone( _source_crs  );
   ogc_crs *         target_crs  = ogc_crs         :: clone( _target_crs  );
   ogc_crs *         interp_crs  = ogc_crs         :: clone( _interp_crs  );
   ogc_method *      method      = ogc_method      :: clone( _method      );
   ogc_vector *      parameters  = ogc_vector      :: clone( _parameters  );
   ogc_vector *      param_files = ogc_vector      :: clone( _param_files );
   ogc_op_accuracy * op_accuracy = ogc_op_accuracy :: clone( _op_accuracy );
   ogc_scope *       scope       = ogc_scope       :: clone( _scope       );
   ogc_vector *      extents     = ogc_vector      :: clone( _extents     );
   ogc_vector *      ids         = ogc_vector      :: clone( _ids         );
   ogc_remark *      remark      = ogc_remark      :: clone( _remark      );

   ogc_coord_op * p = create(_name,
                            source_crs,
                            target_crs,
                            interp_crs,
                            method,
                            parameters,
                            param_files,
                            op_accuracy,
                            scope,
                            extents,
                            ids,
                            remark,
                            OGC_NULL);
   if ( p == OGC_NULL )
   {
      ogc_crs         :: destroy( source_crs  );
      ogc_crs         :: destroy( target_crs  );
      ogc_crs         :: destroy( interp_crs  );
      ogc_method      :: destroy( method      );
      ogc_vector      :: destroy( parameters  );
      ogc_vector      :: destroy( param_files );
      ogc_op_accuracy :: destroy( op_accuracy );
      ogc_scope       :: destroy( scope       );
      ogc_vector      :: destroy( extents     );
      ogc_vector      :: destroy( ids         );
      ogc_remark      :: destroy( remark      );
   }

   return p;
}

/*------------------------------------------------------------------------
 * compare for computational equality
 */
bool ogc_coord_op :: is_equal(
   const ogc_coord_op * p1,
   const ogc_coord_op * p2)
{
   if ( p1 == OGC_NULL && p2 == OGC_NULL ) return true;
   if ( p1 == OGC_NULL || p2 == OGC_NULL ) return false;

   if ( !ogc_string :: is_equal( p1->name(),        p2->name()        ) ||
        !ogc_crs    :: is_equal( p1->source_crs(),  p2->source_crs()  ) ||
        !ogc_crs    :: is_equal( p1->target_crs(),  p2->target_crs()  ) ||
        !ogc_crs    :: is_equal( p1->interp_crs(),  p2->interp_crs()  ) ||
        !ogc_method :: is_equal( p1->method(),      p2->method()      ) ||
        !ogc_vector :: is_equal( p1->parameters(),  p2->parameters()  ) ||
        !ogc_vector :: is_equal( p1->param_files(), p2->param_files() ) )
   {
      return false;
   }

   return true;
}

bool ogc_coord_op :: is_equal(
   const ogc_coord_op * p) const
{
   return is_equal(this, p);
}

/*------------------------------------------------------------------------
 * compare
 */
bool ogc_coord_op :: is_identical(
   const ogc_coord_op * p1,
   const ogc_coord_op * p2)
{
   if ( p1 == OGC_NULL && p2 == OGC_NULL ) return true;
   if ( p1 == OGC_NULL || p2 == OGC_NULL ) return false;

   if ( !ogc_string      :: is_equal    ( p1->name(),        p2->name()        ) ||
        !ogc_crs         :: is_identical( p1->source_crs(),  p2->source_crs()  ) ||
        !ogc_crs         :: is_identical( p1->target_crs(),  p2->target_crs()  ) ||
        !ogc_crs         :: is_identical( p1->interp_crs(),  p2->interp_crs()  ) ||
        !ogc_method      :: is_identical( p1->method(),      p2->method()      ) ||
        !ogc_vector      :: is_identical( p1->parameters(),  p2->parameters()  ) ||
        !ogc_vector      :: is_identical( p1->param_files(), p2->param_files() ) ||
        !ogc_op_accuracy :: is_identical( p1->op_accuracy(), p2->op_accuracy() ) ||
        !ogc_scope       :: is_identical( p1->scope(),       p2->scope()       ) ||
        !ogc_vector      :: is_identical( p1->extents(),     p2->extents()     ) ||
        !ogc_vector      :: is_identical( p1->ids(),         p2->ids()         ) ||
        !ogc_remark      :: is_identical( p1->remark(),      p2->remark()      ) )
   {
      return false;
   }

   return true;
}

bool ogc_coord_op :: is_identical(
   const ogc_coord_op * p) const
{
   return is_identical(this, p);
}

/*------------------------------------------------------------------------
 * get parameter count
 */
int ogc_coord_op :: parameter_count() const
{
   return (_parameters == OGC_NULL) ? 0 : _parameters->length();
}

/*------------------------------------------------------------------------
 * get the nth parameter
 */
ogc_parameter * ogc_coord_op :: parameter(int n) const
{
   return (_parameters == OGC_NULL) ? OGC_NULL :
                                      reinterpret_cast<ogc_parameter *>
                                         ( _parameters->get(n) );
}

/*------------------------------------------------------------------------
 * get param_file count
 */
int ogc_coord_op :: param_file_count() const
{
   return (_param_files == OGC_NULL) ? 0 : _param_files->length();
}

/*------------------------------------------------------------------------
 * get the nth param_file
 */
ogc_param_file * ogc_coord_op :: param_file(int n) const
{
   return (_param_files == OGC_NULL) ? OGC_NULL :
                               reinterpret_cast<ogc_param_file *>
                                  ( _param_files->get(n) );
}

/*------------------------------------------------------------------------
 * get extent count
 */
int ogc_coord_op :: extent_count() const
{
   return (_extents == OGC_NULL) ? 0 : _extents->length();
}

/*------------------------------------------------------------------------
 * get the nth extent
 */
ogc_extent * ogc_coord_op :: extent(int n) const
{
   return (_extents == OGC_NULL) ? OGC_NULL :
                           reinterpret_cast<ogc_extent *>( _extents->get(n) );
}

/*------------------------------------------------------------------------
 * get ID count
 */
int ogc_coord_op :: id_count() const
{
   return (_ids == OGC_NULL) ? 0 : _ids->length();
}

/*------------------------------------------------------------------------
 * get the nth ID
 */
ogc_id * ogc_coord_op :: id(int n) const
{
   return (_ids == OGC_NULL) ? OGC_NULL :
                               reinterpret_cast<ogc_id *>( _ids->get(n) );
}

} /* namespace OGC */
