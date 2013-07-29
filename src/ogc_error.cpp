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
/* Error handling methods                                                    */
/* ------------------------------------------------------------------------- */

#include "ogc_common.h"

namespace OGC {

/*------------------------------------------------------------------------
 * user call-back routine and data
 */
ogc_error :: OGC_ERR_RTN * ogc_error :: _usr_rtn  = OGC_NULL;
void *                     ogc_error :: _usr_data = OGC_NULL;

/*------------------------------------------------------------------------
 * constructor
 */
ogc_error :: ogc_error()
{
   clear();
}

/*------------------------------------------------------------------------
 * static set the error call-back routine
 */
void ogc_error :: set_err_rtn(
   OGC_ERR_RTN * rtn,
   void *        data)
{
   _usr_rtn  = rtn;
   _usr_data = data;
}

/*------------------------------------------------------------------------
 * internal send the message
 */
void ogc_error :: send()
{
   if ( _usr_rtn != OGC_NULL )
   {
      (_usr_rtn)(_usr_data, _err_code, _err_msg);
   }
}

/*------------------------------------------------------------------------
 * clear the error code
 */
void ogc_error :: clear()
{
   _err_code   = OGC_ERR_NONE;
   _err_msg[0] = 0;
}

/*------------------------------------------------------------------------
 * set the error code to a value
 */
void ogc_error :: set(ogc_err_code err_code)
{
   _err_code = err_code;
   strcpy(_err_msg, err_str(err_code));

   send();
}

/*------------------------------------------------------------------------
 * set the error code to a value
 */
void ogc_error :: set(
   ogc_err_code err_code,
   const char * obj_kwd)
{
   char * s = _err_msg;

   _err_code = err_code;
   if ( obj_kwd != OGC_NULL && *obj_kwd != 0 )
   {
      s += ogc_string::strlwr(s, obj_kwd);
      s += ogc_string::strcpy(s, ": ");
   }
   s += ogc_string::strcpy(s, err_str(err_code));

   send();
}

/*------------------------------------------------------------------------
 * set the error code and string
 */
void ogc_error :: set(
   ogc_err_code err_code,
   const char * obj_kwd,
   const char * arg)
{
   char * s = _err_msg;

   _err_code = err_code;
   if ( obj_kwd != OGC_NULL && *obj_kwd != 0 )
   {
      s += ogc_string::strlwr(s, obj_kwd);
      s += ogc_string::strcpy(s, ": ");
   }
   s += ogc_string::strcpy(s, err_str(err_code));
   s += ogc_string::strcpy(s, ": ");

   size_t len = strlen(_err_msg);
   ogc_string::strncpy(s, arg, OGC_ERR_MSG_MAX-len);

   send();
}

/*------------------------------------------------------------------------
 * set the error code and int
 */
void ogc_error :: set(
   ogc_err_code err_code,
   const char * obj_kwd,
   int          arg)
{
   char * s = _err_msg;
   OGC_NBUF buf;

   _err_code = err_code;
   if ( obj_kwd != OGC_NULL && *obj_kwd != 0 )
   {
      s += ogc_string::strlwr(s, obj_kwd);
      s += ogc_string::strcpy(s, ": ");
   }
   s += ogc_string::strcpy(s, err_str(err_code));
   s += ogc_string::strcpy(s, ": ");
   s += ogc_string::strcpy(s, ogc_string::itoa(arg, buf));

   send();
}

/*------------------------------------------------------------------------
 * set the error code and double
 */
void ogc_error :: set(
   ogc_err_code err_code,
   const char * obj_kwd,
   double       arg)
{
   char * s = _err_msg;
   OGC_NBUF buf;

   _err_code = err_code;
   if ( obj_kwd != OGC_NULL && *obj_kwd != 0 )
   {
      s += ogc_string::strlwr(s, obj_kwd);
      s += ogc_string::strcpy(s, ": ");
   }
   s += ogc_string::strcpy(s, err_str(err_code));
   s += ogc_string::strcpy(s, ": ");
   s += ogc_string::strcpy(s, ogc_string::dtoa(arg, buf));

   send();
}

/* ------------------------------------------------------------------------- */
/* static methods                                                            */
/* ------------------------------------------------------------------------- */

/*------------------------------------------------------------------------
 * static clear the error code
 */
void ogc_error :: clear(
   ogc_error * err)
{
   if ( err != OGC_NULL )
      err->clear();
}

/*------------------------------------------------------------------------
 * static set the error code to a value
 */
void ogc_error :: set(
   ogc_error *  err,
   ogc_err_code err_code)
{
   ogc_error e;
   if ( err == OGC_NULL ) err = &e;
   err->set(err_code);
}

/*------------------------------------------------------------------------
 * static set the error code to a value
 */
void ogc_error :: set(
   ogc_error *  err,
   ogc_err_code err_code,
   const char * obj_kwd)
{
   ogc_error e;
   if ( err == OGC_NULL ) err = &e;
   err->set(err_code, obj_kwd);
}

/*------------------------------------------------------------------------
 * static set the error code and string
 */
void ogc_error :: set(
   ogc_error *  err,
   ogc_err_code err_code,
   const char * obj_kwd,
   const char * arg)
{
   ogc_error e;
   if ( err == OGC_NULL ) err = &e;
   err->set(err_code, obj_kwd, arg);
}

/*------------------------------------------------------------------------
 * static set the error code and int
 */
void ogc_error :: set(
   ogc_error *  err,
   ogc_err_code err_code,
   const char * obj_kwd,
   int          arg)
{
   ogc_error e;
   if ( err == OGC_NULL ) err = &e;
   err->set(err_code, obj_kwd, arg);
}

/*------------------------------------------------------------------------
 * static set the error code and double
 */
void ogc_error :: set(
   ogc_error *  err,
   ogc_err_code err_code,
   const char * obj_kwd,
   double       arg)
{
   ogc_error e;
   if ( err == OGC_NULL ) err = &e;
   err->set(err_code, obj_kwd, arg);
}

/*------------------------------------------------------------------------
 * static get the error code
 */
ogc_err_code ogc_error :: err_code(
   const ogc_error *  err)
{
   if ( err == OGC_NULL )
      return OGC_ERR_NONE;
   else
      return err->_err_code;
}

/*------------------------------------------------------------------------
 * static get the error message
 */
const char * ogc_error :: err_msg(
   const ogc_error *  err)
{
   if ( err == OGC_NULL )
      return "?";
   else
      return err->_err_msg;
}

/*------------------------------------------------------------------------
 * static convert an error code to a string
 */
const char * ogc_error :: err_str(ogc_err_code err_code)
{
   const ogc_tables::error_tbl * t = ogc_tables::ogc_errors;

   for (; t->err_code != OGC_ERR_LAST; t++)
   {
      if ( t->err_code == err_code )
         return t->err_str;
   }

   return "?";
}

} /* namespace OGC */
