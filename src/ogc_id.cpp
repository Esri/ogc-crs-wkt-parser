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
/* ID (Identifier) object                                                    */
/* ------------------------------------------------------------------------- */

#include "ogc_common.h"

namespace OGC {

const char * ogc_id :: obj_kwd() { return OGC_OBJ_KWD_ID; }
const char * ogc_id :: old_kwd() { return OGC_OLD_KWD_ID; }

/*------------------------------------------------------------------------
 * create
 */
ogc_id * ogc_id :: create(
   const char *   name,
   const char *   identifier,
   const char *   version,
   ogc_citation * citation,
   ogc_uri *      uri,
   ogc_error *    err)
{
   ogc_id *  p = OGC_NULL;
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

   if ( identifier == OGC_NULL )
   {
      identifier = "";
   }
   else
   {
      int len = ogc_string::unescape_len(identifier);
      if ( len >= OGC_TEXT_MAX )
      {
         ogc_error::set(err, OGC_ERR_IDENTIFIER_TOO_LONG, obj_kwd(), len);
         bad = true;
      }
   }

   if ( version == OGC_NULL )
   {
      version = "";
   }
   else
   {
      int len = ogc_string::unescape_len(version);
      if ( len >= OGC_TEXT_MAX )
      {
         ogc_error::set(err, OGC_ERR_VERSION_TOO_LONG, obj_kwd(), len);
         bad = true;
      }
   }

   /*---------------------------------------------------------
    * create the object
    */
   if ( !bad )
   {
      p = new (std::nothrow) ogc_id();
      if ( p == OGC_NULL )
      {
         ogc_error::set(err, OGC_ERR_NO_MEMORY, obj_kwd());
         return p;
      }

      ogc_string::unescape_str(p->_name,       name,       OGC_NAME_MAX);
      ogc_string::unescape_str(p->_identifier, identifier, OGC_TEXT_MAX);
      ogc_string::unescape_str(p->_version,    version,    OGC_TEXT_MAX);
      p->_obj_type = OGC_OBJ_TYPE_ID;
      p->_citation = citation;
      p->_uri      = uri;
   }

   return p;
}

/*------------------------------------------------------------------------
 * destroy
 */
ogc_id :: ~ogc_id()
{
   ogc_citation :: destroy( _citation );
   ogc_uri      :: destroy( _uri      );
}

void ogc_id :: destroy(
   ogc_id * obj)
{
   if ( obj != OGC_NULL )
   {
      delete obj;
   }
}

/*------------------------------------------------------------------------
 * object from tokens
 */
ogc_id * ogc_id :: from_tokens(
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

   ogc_id *       obj      = OGC_NULL;
   ogc_citation * citation = OGC_NULL;
   ogc_uri *      uri      = OGC_NULL;
   const char * name;
   const char * identifier;
   const char * version;

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
    * There must be 2 or 3 tokens: ID[ "name", "identity" ...
    * (version is optional)
    */
   if ( same < 2 )
   {
      ogc_error::set(err, OGC_ERR_WKT_INSUFFICIENT_TOKENS, obj_kwd(), same);
      return OGC_NULL;
   }

   if ( same > 3 && get_strict_parsing() )
   {
      ogc_error::set(err, OGC_ERR_WKT_TOO_MANY_TOKENS,     obj_kwd(), same);
      return OGC_NULL;
   }


   start++;

   /*---------------------------------------------------------
    * Process all non-object tokens.
    * They come first and are syntactically fixed.
    */
   name       = arr[start++].str;
   identifier = arr[start++].str;

   if ( same > 2 )
      version = arr[start++].str;
   else
      version = "";

   /*---------------------------------------------------------
    * Now process all sub-objects
    */
   int  next = 0;
   for (int i = start; i < end; i = next)
   {
      if ( ogc_string::is_equal(arr[i].str, ogc_citation::obj_kwd()) )
      {
         if ( citation != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_CITATION, obj_kwd());
            bad = true;
         }
         else
         {
            citation = ogc_citation::from_tokens(t, i, &next, err);
            if ( citation == OGC_NULL )
               bad = true;
         }
         continue;
      }

      if ( ogc_string::is_equal(arr[i].str, ogc_uri::obj_kwd()) )
      {
         if ( uri != OGC_NULL )
         {
            ogc_error::set(err, OGC_ERR_WKT_DUPLICATE_URI, obj_kwd());
            bad = true;
         }
         else
         {
            uri = ogc_uri::from_tokens(t, i, &next, err);
            if ( uri == OGC_NULL )
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
      obj = create(name, identifier, version, citation, uri, err);
   }

   if ( obj == OGC_NULL )
   {
      ogc_citation :: destroy( citation );
      ogc_uri      :: destroy( uri      );
   }

   return obj;
}

/*------------------------------------------------------------------------
 * object from WKT
 */
ogc_id * ogc_id :: from_wkt(
   const char * wkt,
   ogc_error *  err)
{
   ogc_id * obj = OGC_NULL;
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
bool ogc_id :: to_wkt(
   const ogc_id * obj,
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

bool ogc_id :: to_wkt(
   char      buffer[],
   int       options,
   size_t    buflen) const
{
   OGC_UTF8_NAME buf_name;
   OGC_UTF8_TEXT buf_identifier;
   OGC_UTF8_TEXT buf_version;
   OGC_TBUF      buf_hdr;
   OGC_TBUF      buf_citation;
   OGC_TBUF      buf_uri;
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
      kwd = old_kwd();

   rc &= ogc_citation :: to_wkt(_citation, buf_citation, opts, OGC_TBUF_MAX);
   rc &= ogc_uri      :: to_wkt(_uri,      buf_uri,      opts, OGC_TBUF_MAX);

   ogc_string::escape_str(buf_name,       _name,       OGC_UTF8_NAME_MAX);
   ogc_string::escape_str(buf_identifier, _identifier, OGC_UTF8_TEXT_MAX);
   ogc_string::escape_str(buf_version,    _version,    OGC_UTF8_TEXT_MAX);

   if ( (options & OGC_WKT_OPT_OLD_SYNTAX) != 0 )
   {
      sprintf(buf_hdr, "%s%s\"%s\",%d",
         kwd, opn, buf_name, ogc_string::atoi(_identifier));
   }
   else
   {
      if ( *buf_version != 0 )
      {
         sprintf(buf_hdr, "%s%s\"%s\",\"%s\",\"%s\"",
            kwd, opn, buf_name, buf_identifier, buf_version);
      }
      else
      {
         sprintf(buf_hdr, "%s%s\"%s\",\"%s\"",
            kwd, opn, buf_name, buf_identifier);
      }
   }

   OGC_CPY_TO_BUF( buf_hdr      );
   OGC_ADD_TO_BUF( buf_citation );
   OGC_ADD_TO_BUF( buf_uri      );
   OGC_CPY_TO_BUF( cls          );

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
ogc_id * ogc_id :: clone(const ogc_id * obj)
{
   if ( obj == OGC_NULL )
      return OGC_NULL;
   return obj->clone();
}

ogc_id * ogc_id :: clone() const
{
   ogc_citation * citation = ogc_citation :: clone( _citation );
   ogc_uri *      uri      = ogc_uri      :: clone( _uri      );

   ogc_id * p = create(_name,
                       _identifier,
                       _version,
                       citation,
                       uri,
                       OGC_NULL);
   if ( p == OGC_NULL )
   {
      ogc_citation :: destroy( citation );
      ogc_uri      :: destroy( uri      );
   }

   return p;
}

/*------------------------------------------------------------------------
 * compare for computational equality
 */
bool ogc_id :: is_equal(
   const ogc_id * p1,
   const ogc_id * p2)
{
   if ( p1 == OGC_NULL && p2 == OGC_NULL ) return true;
   if ( p1 == OGC_NULL || p2 == OGC_NULL ) return false;

   if ( !ogc_string :: is_equal( p1->name(),       p2->name()       ) ||
        !ogc_string :: is_equal( p1->identifier(), p2->identifier() ) ||
        !ogc_string :: is_equal( p1->version(),    p2->version()    ) )
   {
      return false;
   }

   return true;
}

bool ogc_id :: is_equal(
   const ogc_id * p) const
{
   return is_equal(this, p);
}

/*------------------------------------------------------------------------
 * compare
 */
bool ogc_id :: is_identical(
   const ogc_id * p1,
   const ogc_id * p2)
{
   if ( p1 == OGC_NULL && p2 == OGC_NULL ) return true;
   if ( p1 == OGC_NULL || p2 == OGC_NULL ) return false;

   if ( !ogc_string   :: is_equal    ( p1->name(),       p2->name()       ) ||
        !ogc_string   :: is_equal    ( p1->identifier(), p2->identifier() ) ||
        !ogc_string   :: is_equal    ( p1->version(),    p2->version()    ) ||
        !ogc_citation :: is_identical( p1->citation(),   p2->citation()   ) ||
        !ogc_uri      :: is_identical( p1->uri(),        p2->uri()        ) )
   {
      return false;
   }

   return true;
}

bool ogc_id :: is_identical(
   const ogc_id * p) const
{
   return is_identical(this, p);
}

} /* namespace OGC */
