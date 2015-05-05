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
/* utility methods                                                           */
/* ------------------------------------------------------------------------- */

#include "ogc_common.h"

namespace OGC {

/* ------------------------------------------------------------------------- */
/* object types                                                              */
/* ------------------------------------------------------------------------- */

ogc_obj_type ogc_utils :: obj_kwd_to_type (const char * kwd)
{
   const ogc_tables::obj_tbl * t = ogc_tables::ogc_obj_types;

   if ( kwd == OGC_NULL )
      return OGC_OBJ_TYPE_UNKNOWN;

   for (; t->type != OGC_OBJ_TYPE_UNKNOWN; t++)
   {
      if ( ogc_string::strcmp_i(t->kwd, kwd) == 0 )
         return t->type;
   }

   return OGC_OBJ_TYPE_UNKNOWN;
}

const char * ogc_utils :: obj_type_to_kwd (ogc_obj_type type)
{
   const ogc_tables::obj_tbl * t = ogc_tables::ogc_obj_types;

   for (; t->type != OGC_OBJ_TYPE_UNKNOWN; t++)
   {
      if ( t->type == type )
         return t->kwd;
   }

   return OGC_OBJ_KWD_UNKNOWN;
}

/* ------------------------------------------------------------------------- */
/* coordinate reference system types                                         */
/* ------------------------------------------------------------------------- */

ogc_crs_type ogc_utils :: crs_kwd_to_type (const char * kwd)
{
   const ogc_tables::crs_tbl * t = ogc_tables::ogc_crs_types;

   if ( kwd == OGC_NULL )
      return OGC_CRS_TYPE_UNKNOWN;

   for (; t->type != OGC_CRS_TYPE_UNKNOWN; t++)
   {
      if ( ogc_string::strcmp_i(t->kwd, kwd) == 0 )
         return t->type;
   }

   return OGC_CRS_TYPE_UNKNOWN;
}

const char * ogc_utils :: crs_type_to_kwd (ogc_crs_type type)
{
   const ogc_tables::crs_tbl * t = ogc_tables::ogc_crs_types;

   for (; t->type != OGC_CRS_TYPE_UNKNOWN; t++)
   {
      if ( t->type == type )
         return t->kwd;
   }

   return OGC_CRS_KWD_UNKNOWN;
}

/* ------------------------------------------------------------------------- */
/* unit types                                                                */
/* ------------------------------------------------------------------------- */

ogc_unit_type ogc_utils :: unit_kwd_to_type (const char * kwd)
{
   const ogc_tables::unit_tbl * t = ogc_tables::ogc_unit_types;

   if ( kwd == OGC_NULL )
      return OGC_UNIT_TYPE_UNKNOWN;

   for (; t->type != OGC_UNIT_TYPE_UNKNOWN; t++)
   {
      if ( ogc_string::strcmp_i(t->kwd, kwd) == 0 )
         return t->type;
   }

   return OGC_UNIT_TYPE_UNKNOWN;
}

const char * ogc_utils :: unit_type_to_kwd (ogc_unit_type type)
{
   const ogc_tables::unit_tbl * t = ogc_tables::ogc_unit_types;

   for (; t->type != OGC_UNIT_TYPE_UNKNOWN; t++)
   {
      if ( t->type == type )
         return t->kwd;
   }

   return OGC_UNIT_KWD_UNKNOWN;
}

/* ------------------------------------------------------------------------- */
/* datum types                                                               */
/* ------------------------------------------------------------------------- */

ogc_datum_type ogc_utils :: datum_kwd_to_type (const char * kwd)
{
   const ogc_tables::datum_tbl * t = ogc_tables::ogc_datum_types;

   if ( kwd == OGC_NULL )
      return OGC_DATUM_TYPE_UNKNOWN;

   for (; t->type != OGC_DATUM_TYPE_UNKNOWN; t++)
   {
      if ( ogc_string::strcmp_i(t->kwd, kwd) == 0 )
         return t->type;
   }

   return OGC_DATUM_TYPE_UNKNOWN;
}

const char * ogc_utils :: datum_type_to_kwd (ogc_datum_type type)
{
   const ogc_tables::datum_tbl * t = ogc_tables::ogc_datum_types;

   for (; t->type != OGC_DATUM_TYPE_UNKNOWN; t++)
   {
      if ( t->type == type )
         return t->kwd;
   }

   return OGC_DATUM_KWD_UNKNOWN;
}

/* ------------------------------------------------------------------------- */
/* coordinate system types                                                   */
/* ------------------------------------------------------------------------- */

ogc_cs_type ogc_utils :: cs_kwd_to_type (const char * kwd)
{
   const ogc_tables::cs_tbl * t = ogc_tables::ogc_cs_types;

   if ( kwd == OGC_NULL )
      return OGC_CS_TYPE_UNKNOWN;

   for (; t->type != OGC_CS_TYPE_UNKNOWN; t++)
   {
      if ( ogc_string::strcmp_i(t->kwd, kwd) == 0 )
         return t->type;
   }

   return OGC_CS_TYPE_UNKNOWN;
}

const char * ogc_utils :: cs_type_to_kwd (ogc_cs_type type)
{
   const ogc_tables::cs_tbl * t = ogc_tables::ogc_cs_types;

   for (; t->type != OGC_CS_TYPE_UNKNOWN; t++)
   {
      if ( t->type == type )
         return t->kwd;
   }

   return OGC_CS_KWD_UNKNOWN;
}

bool ogc_utils :: cs_type_valid (ogc_cs_type type)
{
   const ogc_tables::cs_tbl * t = ogc_tables::ogc_cs_types;

   for (; t->type != OGC_CS_TYPE_UNKNOWN; t++)
   {
      if ( t->type == type )
         return true;
   }

   return false;
}

/* ------------------------------------------------------------------------- */
/* axis direction types                                                      */
/* ------------------------------------------------------------------------- */

ogc_axis_direction ogc_utils :: axis_kwd_to_direction (const char * kwd)
{
   const ogc_tables::axis_tbl * t = ogc_tables::ogc_axis_types;

   if ( kwd == OGC_NULL )
      return OGC_AXIS_DIR_TYPE_UNKNOWN;

   for (; t->type != OGC_AXIS_DIR_TYPE_UNKNOWN; t++)
   {
      if ( ogc_string::strcmp_i(t->kwd, kwd) == 0 )
         return t->type;
   }

   return OGC_AXIS_DIR_TYPE_UNKNOWN;
}

const char * ogc_utils :: axis_direction_to_kwd (ogc_axis_direction dir)
{
   const ogc_tables::axis_tbl * t = ogc_tables::ogc_axis_types;

   for (; t->type != OGC_AXIS_DIR_TYPE_UNKNOWN; t++)
   {
      if ( t->type == dir )
         return t->kwd;
   }

   return OGC_AXIS_DIR_KWD_UNKNOWN;
}

bool ogc_utils :: axis_direction_valid (ogc_axis_direction dir)
{
   const ogc_tables::axis_tbl * t = ogc_tables::ogc_axis_types;

   for (; t->type != OGC_AXIS_DIR_TYPE_UNKNOWN; t++)
   {
      if ( t->type == dir )
         return true;
   }

   return false;
}

/* ------------------------------------------------------------------------- */
/* pixel-in-cell types                                                       */
/* ------------------------------------------------------------------------- */

ogc_pixel_type ogc_utils :: pixel_kwd_to_type (const char * kwd)
{
   const ogc_tables::pixel_tbl * t = ogc_tables::ogc_pixel_types;

   if ( kwd == OGC_NULL )
      return OGC_PIXEL_TYPE_UNKNOWN;

   for (; t->type != OGC_PIXEL_TYPE_UNKNOWN; t++)
   {
      if ( ogc_string::strcmp_i(t->kwd, kwd) == 0 )
         return t->type;
   }

   return OGC_PIXEL_TYPE_UNKNOWN;
}

const char * ogc_utils :: pixel_type_to_kwd (ogc_pixel_type type)
{
   const ogc_tables::pixel_tbl * t = ogc_tables::ogc_pixel_types;

   for (; t->type != OGC_PIXEL_TYPE_UNKNOWN; t++)
   {
      if ( t->type == type )
         return t->kwd;
   }

   return OGC_PIXEL_KWD_UNKNOWN;
}

bool ogc_utils :: pixel_type_valid (ogc_pixel_type type)
{
   const ogc_tables::pixel_tbl * t = ogc_tables::ogc_pixel_types;

   for (; t->type != OGC_PIXEL_TYPE_UNKNOWN; t++)
   {
      if ( t->type == type )
         return true;
   }

   return false;
}

/* ------------------------------------------------------------------------- */
/* other utility methods                                                     */
/* ------------------------------------------------------------------------- */

/*------------------------------------------------------------------------
 * validate the coordinate system, axes, and unit
 *
 * This code assumes a valid object type and CS object.
 *
 * returns true if OK
 */
bool ogc_utils :: validate_cs(
   ogc_obj_type obj_type,
   ogc_cs *     cs,
   ogc_axis *   axis_1,
   ogc_axis *   axis_2,
   ogc_axis *   axis_3,
   ogc_unit *   unit,
   ogc_error *  err)
{
   ogc_cs_type  cs_type = cs->cs_type();
   int          dim     = cs->dimension();
   const char * obj_kwd = ogc_utils::obj_type_to_kwd(obj_type);
   const char * cs_kwd  = ogc_utils:: cs_type_to_kwd( cs_type);

   /* First validate the CS type against the CRS type and
      the number of dimensions.

         CRS type        Permitted CS type(s)   Dimension
                                                (# axes)
         -------------   --------------------   ---------
         geodetic        Cartesian                  3
                         ellipsoidal              2 3
                         spherical                  3

         projected       Cartesian                2

         vertical        vertical               1

         engineering     affine                   2 3
                         Cartesian                2 3
                         cylindrical                3
                         linear                 1
                         polar                    2
                         spherical                  3

         image           affine                   2
                         Cartesian                2

         parametric      parametric             1

         temporal        time                   1
   */
   switch (obj_type)
   {
      case OGC_OBJ_TYPE_GEOD_CRS:
         switch (cs_type)
         {
            case OGC_CS_TYPE_CARTESIAN:
            case OGC_CS_TYPE_SPHERICAL:
               if ( dim != 3 )
               {
                  ogc_error::set(err, OGC_ERR_INVALID_DIMENSION, obj_kwd, dim);
                  return false;
               }
               break;

            case OGC_CS_TYPE_ELLIPSOIDAL:
               if ( dim != 2 && dim != 3 )
               {
                  ogc_error::set(err, OGC_ERR_INVALID_DIMENSION, obj_kwd, dim);
                  return false;
               }
               break;

            default:
               ogc_error::set(err, OGC_ERR_INVALID_CS_TYPE, obj_kwd, cs_kwd);
               return false;
         }
         break;

      case OGC_OBJ_TYPE_PROJ_CRS:
         switch (cs_type)
         {
            case OGC_CS_TYPE_CARTESIAN:
               if ( dim != 2 )
               {
                  ogc_error::set(err, OGC_ERR_INVALID_DIMENSION, obj_kwd, dim);
                  return false;
               }
               break;

            default:
               ogc_error::set(err, OGC_ERR_INVALID_CS_TYPE, obj_kwd, cs_kwd);
               return false;
         }
        break;

      case OGC_OBJ_TYPE_VERT_CRS:
         switch (cs_type)
         {
            case OGC_CS_TYPE_VERTICAL:
               if ( dim != 1 )
               {
                  ogc_error::set(err, OGC_ERR_INVALID_DIMENSION, obj_kwd, dim);
                  return false;
               }
               break;

            default:
               ogc_error::set(err, OGC_ERR_INVALID_CS_TYPE, obj_kwd, cs_kwd);
               return false;
         }
        break;

      case OGC_OBJ_TYPE_ENGR_CRS:
         switch (cs_type)
         {
            case OGC_CS_TYPE_AFFINE:
            case OGC_CS_TYPE_CARTESIAN:
               if ( dim != 2 && dim != 3 )
               {
                  ogc_error::set(err, OGC_ERR_INVALID_DIMENSION, obj_kwd, dim);
                  return false;
               }
               break;

            case OGC_CS_TYPE_CYLINDRICAL:
            case OGC_CS_TYPE_SPHERICAL:
               if ( dim != 3 )
               {
                  ogc_error::set(err, OGC_ERR_INVALID_DIMENSION, obj_kwd, dim);
                  return false;
               }
               break;

            case OGC_CS_TYPE_LINEAR:
               if ( dim != 1 )
               {
                  ogc_error::set(err, OGC_ERR_INVALID_DIMENSION, obj_kwd, dim);
                  return false;
               }
               break;

            case OGC_CS_TYPE_POLAR:
               if ( dim != 2 )
               {
                  ogc_error::set(err, OGC_ERR_INVALID_DIMENSION, obj_kwd, dim);
                  return false;
               }
               break;

            default:
               ogc_error::set(err, OGC_ERR_INVALID_CS_TYPE, obj_kwd, cs_kwd);
               return false;
         }
        break;

      case OGC_OBJ_TYPE_IMAGE_CRS:
         switch (cs_type)
         {
            case OGC_CS_TYPE_AFFINE:
            case OGC_CS_TYPE_CARTESIAN:
               if ( dim != 2 )
               {
                  ogc_error::set(err, OGC_ERR_INVALID_DIMENSION, obj_kwd, dim);
                  return false;
               }
               break;

            default:
               ogc_error::set(err, OGC_ERR_INVALID_CS_TYPE, obj_kwd, cs_kwd);
               return false;
         }
        break;

      case OGC_OBJ_TYPE_TIME_CRS:
         switch (cs_type)
         {
            case OGC_CS_TYPE_TEMPORAL:
               if ( dim != 1 )
               {
                  ogc_error::set(err, OGC_ERR_INVALID_DIMENSION, obj_kwd, dim);
                  return false;
               }
               break;

            default:
               ogc_error::set(err, OGC_ERR_INVALID_CS_TYPE, obj_kwd, cs_kwd);
               return false;
         }
        break;

      case OGC_OBJ_TYPE_PARAM_CRS:
         switch (cs_type)
         {
            case OGC_CS_TYPE_PARAMETRIC:
               if ( dim != 1 )
               {
                  ogc_error::set(err, OGC_ERR_INVALID_DIMENSION, obj_kwd, dim);
                  return false;
               }
               break;

            default:
               ogc_error::set(err, OGC_ERR_INVALID_CS_TYPE, obj_kwd, cs_kwd);
               return false;
         }
        break;

      default:
        ogc_error::set(err, OGC_ERR_INVALID_CS_TYPE, obj_kwd, cs_kwd);
        return false;
   }

   /* Get the unit types for the unit arg and all axis args. */

   ogc_unit_type  u_type = OGC_UNIT_TYPE_UNKNOWN;   /* type of unit   arg  */
   ogc_unit_type u1_type = OGC_UNIT_TYPE_UNKNOWN;   /* type of axis-1 unit */
   ogc_unit_type u2_type = OGC_UNIT_TYPE_UNKNOWN;   /* type of axis-2 unit */
   ogc_unit_type u3_type = OGC_UNIT_TYPE_UNKNOWN;   /* type of axis-3 unit */

   if ( unit != OGC_NULL )
   {
      u_type = unit->unit_type();
   }

   if ( axis_1 != OGC_NULL )
   {
      ogc_unit * u = axis_1->unit();
      if ( u != OGC_NULL )
         u1_type = u->unit_type();
   }

   if ( axis_2 != OGC_NULL )
   {
      ogc_unit * u = axis_2->unit();
      if ( u != OGC_NULL )
         u2_type = u->unit_type();
   }

   if ( axis_3 != OGC_NULL )
   {
      ogc_unit * u = axis_3->unit();
      if ( u != OGC_NULL )
         u3_type = u->unit_type();
   }

   /* Now check axis entries and unit. */
   /* TODO : not yet done */

   switch (obj_type)
   {
      case OGC_OBJ_TYPE_GEOD_CRS:
         break;

      case OGC_OBJ_TYPE_PROJ_CRS:
        break;

      case OGC_OBJ_TYPE_VERT_CRS:
        break;

      case OGC_OBJ_TYPE_ENGR_CRS:
        break;

      case OGC_OBJ_TYPE_IMAGE_CRS:
        break;

      case OGC_OBJ_TYPE_TIME_CRS:
        break;

      case OGC_OBJ_TYPE_PARAM_CRS:
        break;

      default:
        ogc_error::set(err, OGC_ERR_INVALID_CS_TYPE, obj_kwd, cs_kwd);
        return false;
   }

   return true;
}

/*------------------------------------------------------------------------
 * place an axis in the proper slot
 *
 * returns true if OK
 */
bool ogc_utils :: place_axis(
   ogc_axis *   axis,
   ogc_axis **  paxis_1,
   ogc_axis **  paxis_2,
   ogc_axis **  paxis_3,
   const char * obj_kwd,
   ogc_error *  err)
{
   ogc_order * order = axis->order();

   /* If no order was specified, then
      put it in the first empty slot.
   */
   if ( order == OGC_NULL )
   {
      if ( paxis_1 != OGC_NULL && *paxis_1 == OGC_NULL )
      {
         *paxis_1 = axis;
         return true;
      }

      if ( paxis_2 != OGC_NULL && *paxis_2 == OGC_NULL )
      {
         *paxis_2 = axis;
         return true;
      }

      if ( paxis_3 != OGC_NULL && *paxis_3 == OGC_NULL )
      {
         *paxis_3 = axis;
         return true;
      }

      ogc_error::set(err, OGC_ERR_WKT_TOO_MANY_AXES,
         obj_kwd);
      return false;
   }

   /* If an order was specified, check first that the specified
      entry is currently empty. If so, use it.
   */
   int value = order->value();

   switch (value)
   {
      case 1:
         if ( paxis_1 == OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_ORDER_OUT_OF_RANGE,
               obj_kwd, value);
            return false;
         }
         if ( *paxis_1 != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_AXIS_ALREADY_USED,
               obj_kwd, value);
            return false;
         }
         *paxis_1 = axis;
         return true;

      case 2:
         if ( paxis_2 == OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_ORDER_OUT_OF_RANGE,
               obj_kwd, value);
            return false;
         }
         if ( *paxis_2 != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_AXIS_ALREADY_USED,
               obj_kwd, value);
            return false;
         }
         *paxis_2 = axis;
         return true;

      case 3:
         if ( paxis_3 == OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_ORDER_OUT_OF_RANGE,
               obj_kwd, value);
            return false;
         }
         if ( *paxis_3 != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_AXIS_ALREADY_USED,
               obj_kwd, value);
            return false;
         }
         *paxis_3 = axis;
         return true;
   }

   ogc_error::set(err, OGC_ERR_INVALID_ORDER_VALUE,
      obj_kwd, value);
   return false;
}

/*------------------------------------------------------------------------
 * expand a WKT string
 *
 * This routine inserts appropriate spaces and newlines to make the
 * string more readable.
 *
 * Thus, the string:
 *   PROJCRS[...,BASECRS[...,DATUM[...,ELLIPSOID[...]],PRIMEM[...],...]]
 * becomes:
 *   PROJCRS[...,
 *    BASECRS[...,
 *     DATUM[...,
 *      ELLIPSOID[...]],
 *     PRIMEM[...],
 *    ...]]
 *
 * Current limitations:
 *
 *   It doesn't handle "[]()," chars in quoted strings.
 *
 * <prefix> is an optional string that will prefix each line of output.
 *
 * Note that the resultant string is still a valid input to any
 * from_wkt() method.
 *
 * Note also that this code is fairly abstruse. Sorry.
 */
bool ogc_utils :: expand_wkt(
      char         out_buffer[],
      const char * inp_buffer,
      const char * prefix,
      int          opts,
      size_t       buflen)
{
   char * tmp_buffer = OGC_NULL;

   if ( prefix == OGC_NULL )
      prefix = "";

   if ( out_buffer == OGC_NULL )
      return false;

   if ( inp_buffer == OGC_NULL || *inp_buffer == 0 )
   {
      *out_buffer = 0;
      return true;
   }

   if ( inp_buffer == out_buffer )
   {
      tmp_buffer = new (std::nothrow) char [strlen(inp_buffer)+1];
      if ( tmp_buffer == OGC_NULL )
      {
         return false;
      }
      strcpy(tmp_buffer, inp_buffer);
      inp_buffer = tmp_buffer;
   }

   if ( (opts & OGC_WKT_OPT_EXPAND) == 0 )
   {
      ogc_string::strncpy(out_buffer, inp_buffer, buflen);
   }
   else
   {
      const char * p;
      const char * b;
      const char * s      = inp_buffer;
      const char * bs     = inp_buffer;
      const char * be     = inp_buffer;
      char *       op     = out_buffer;
      int          tab    = 0;
      int          otb    = 0;
      bool         cr     = false;
      char         indent = ((opts & OGC_WKT_OPT_EXPAND_SP) != 0) ? ' ' : '\t';
      char         opn    = '[';
      char         cls    = ']';

      if ( (opts & OGC_WKT_OPT_PARENS) != 0 )
      {
         opn = '(';
         cls = ')';
      }

      for (p = prefix; *p; p++)
         *op++ = *p;

      for (; ; s++)
      {
         be++;

         if ( *s != ',' && *s != 0 )
         {
            if ( *s == opn )
            {
               tab++;
               cr = true;
            }
            else
            if ( *s == cls )
            {
               tab--;
            }
         }
         else
         {
            if ( cr )
            {
               if ( otb > 0 )
               {
                  *op++ = '\n';
                  for (p = prefix; *p; p++)
                     *op++ = *p;
                  for (; otb; otb--)
                     *op++ = indent;
               }
            }
            else
            {
               *op++ = ' ';
            }

            for (b = bs; b < be; b++)
               *op++ = *b;
            bs = be = (s+1);

            cr = false;
            otb = tab;

            if ( *s == 0 )
               break;
         }
      }
      *op = 0;
   }

   if ( tmp_buffer != OGC_NULL )
      delete [] tmp_buffer;

   return true;
}

/* ------------------------------------------------------------------------- */
/* compare routines for sorting/finding                                      */
/* ------------------------------------------------------------------------- */

int _CDECL ogc_utils :: compare_extent(
   const void * v1,
   const void * v2)
{
   const ogc_extent * p1 = reinterpret_cast<const ogc_extent *>( v1 );
   const ogc_extent * p2 = reinterpret_cast<const ogc_extent *>( v2 );
   int t1 = static_cast<int>( p1->obj_type() );
   int t2 = static_cast<int>( p2->obj_type() );

   return (t1 - t2);
}

int _CDECL ogc_utils :: compare_id(
   const void * v1,
   const void * v2)
{
   const ogc_id * p1 = reinterpret_cast<const ogc_id *>( v1 );
   const ogc_id * p2 = reinterpret_cast<const ogc_id *>( v2 );

   return ogc_string::strcmp_ci( p1->name(), p2->name() );
}

int _CDECL ogc_utils :: compare_parameter(
   const void * v1,
   const void * v2)
{
   const ogc_parameter * p1 = reinterpret_cast<const ogc_parameter *>( v1 );
   const ogc_parameter * p2 = reinterpret_cast<const ogc_parameter *>( v2 );

   return ogc_string::strcmp_ci( p1->name(), p2->name() );
}

int _CDECL ogc_utils :: compare_param_file(
   const void * v1,
   const void * v2)
{
   const ogc_param_file * p1 = reinterpret_cast<const ogc_param_file *>( v1 );
   const ogc_param_file * p2 = reinterpret_cast<const ogc_param_file *>( v2 );

   return ogc_string::strcmp_ci( p1->name(), p2->name() );
}

} /* namespace OGC */
