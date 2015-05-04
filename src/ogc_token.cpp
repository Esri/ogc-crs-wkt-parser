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
/* WKT tokenizing                                                            */
/* ------------------------------------------------------------------------- */

#include "ogc_common.h"

namespace OGC {

/*------------------------------------------------------------------------
 * TOKEN constructor
 */
ogc_token :: ogc_token()
{
   _buffer = OGC_NULL;
   _num    = 0;
}

/*------------------------------------------------------------------------
 * TOKEN destructor
 */
ogc_token :: ~ogc_token()
{
   if ( _buffer != OGC_NULL )
   {
      delete [] _buffer;
   }
}

/*------------------------------------------------------------------------
 * 1st pass through the string
 *
 * Copy the string into our internal buffer.
 * While doing this, we convert any () chars to [], and
 * remove any whitespace outside of quotes.
 * We also strip leading & trailing whitespace from quoted sub-strings.
 * We also check that all quotes and [] are balanced.
 *
 * Notes:
 *    We do everything as unsigned characters, so the ctype
 *    functions work properly.
 *
 *    We know that the string is non-empty with no leading whitespace.
 */
bool ogc_token :: pass1(
   const char * str,
   const char * start,
   const char * obj_kwd,
   ogc_error *  err)
{
   unsigned char *       ubuf = reinterpret_cast<      unsigned char *>(_buffer);
   const unsigned char * ustr = reinterpret_cast<const unsigned char *>(str);
   const unsigned char * ubeg = reinterpret_cast<const unsigned char *>(start);
   unsigned char *       end  = (ubuf + OGC_BUFF_MAX - 1);
   const unsigned char * s    = ustr;
   unsigned char *       b    = ubuf;
   bool in_quotes     = false;
   int  bracket_count = 0;
   int  pos;

   if ( !isalpha(*s) )
   {
      pos = static_cast<int>(s-ubeg);
      ogc_error::set(err, OGC_ERR_WKT_INVALID_SYNTAX, obj_kwd, pos);
      return false;
   }

   for (; *s; )
   {
      unsigned char c = *s++;

      if ( b >= end )
      {
         pos = static_cast<int>(s-ubeg);
         ogc_error::set(err, OGC_ERR_WKT_TOO_LONG, obj_kwd, pos);
         return false;
      }

#if 0 /* reverse-solidus (\) is a valid text character */
      if ( c == '\\' )
      {
         /* deal with escape chars */
         if ( *s == 0 )
         {
            pos = static_cast<int>(s-ubeg);
            ogc_error::set(err, OGC_ERR_WKT_INVALID_ESCAPE, obj_kwd, pos);
            return false;
         }
         *b++ = *s++;
         continue;
      }
#endif

      /* fix delimiters */
      if ( !in_quotes )
      {
         if ( c == '(' ) c = '[';
         else
         if ( c == ')' ) c = ']';
      }

      if ( c == '"' )
      {
         /* "" treated as single " */
         if ( in_quotes && *s == '"' )
         {
            *b++ = c;
            *b++ = c;
            s++;
            continue;
         }

         in_quotes = !in_quotes;
         if ( in_quotes )
         {
            *b++ = c;
            /* remove leading whitespace */
            while ( isspace(*s) )
               s++;
         }
         else
         {
            /* remove trailing whitespace */
            while ( isspace(b[-1]) )
               b--;
            *b++ = c;
            for (; isspace(*s); s++)
               ;
            if ( *s != ',' && *s != ']' )
            {
               pos = static_cast<int>(s-ubeg);
               ogc_error::set(err, OGC_ERR_WKT_EXPECTING_TOKEN, obj_kwd, pos);
               return false;
            }
         }
         continue;
      }

      /* convert any quoted whitespace to a space */
      if ( in_quotes )
      {
         if ( isspace(c) )
         {
            /* convert multiple WS to single space */
            *b++ = ' ';
            for (; isspace(*s); s++)
               ;
         }
         else
         {
            *b++ = c;
         }
         continue;
      }

      if ( isspace(c) )
         continue;

      if (c == '[')
      {
         bracket_count++;
      }
      else if (c == ']')
      {
         if ( ! ogc_object::get_strict_parsing() )
         {
            if ( bracket_count == 0 )
               break;
         }

         if ( --bracket_count < 0 )
         {
            pos = static_cast<int>(s-ubeg);
            ogc_error::set(err, OGC_ERR_WKT_TOO_MANY_CLOSE_TOKENS, obj_kwd,
               pos);
            return false;
         }
      }

      if (b[-1] == ']')
      {
         if (c != ']' && c != ',')
         {
            pos = static_cast<int>(s-ubeg);
            ogc_error::set(err, OGC_ERR_WKT_EXPECTING_TOKEN, obj_kwd, pos);
            return false;
         }
      }

      *b++ = c;
   }
   *b = 0;

   if ( in_quotes )
   {
      pos = static_cast<int>(s-ubeg);
      ogc_error::set(err, OGC_ERR_WKT_UNBALANCED_QUOTES, obj_kwd, pos);
      return false;
   }

   if ( bracket_count > 0 )
   {
      if ( ogc_object::get_strict_parsing() )
      {
         pos = static_cast<int>(s-ubeg);
         ogc_error::set(err, OGC_ERR_WKT_TOO_MANY_OPEN_TOKENS, obj_kwd, pos);
         return false;
      }
      else
      {
         while (bracket_count--)
            *b++ = ']';
         *b = 0;
      }
   }

   return true;
}

/*------------------------------------------------------------------------
 * parse a sub-substring
 *
 *   input:
 *      pb      address of pointer to start of string
 *
 *   output:
 *      pb      address of pointer to actual start of string
 *              Actual string will be null-terminated.
 *      pe      address of next string to process (or to null)
 *
 *      returns delimiter char found (,[] or 0).
 */
int ogc_token :: parse_substring(
   char ** pb,
   char ** pe)
{
   char * b = *pb;
   char * e;
   int    c;

   if ( *b == '"' )
   {
      /* Token is in form "...".
       * The string is everything within the quotes, and the char
       * following the closing quote is the delimiter.
       *
       * We also have to deal with "" chars.
       */
      for (e = ++b; *e; e++)
      {
         if ( *e == '"' )
         {
            if ( e[1] == '"' )
            {
               e++;
               continue;
            }
            break;
         }
      }
      *e++ = 0;

      c = *e;
      if ( *e != 0 )
         e++;
   }
   else
   {
      /* No quotes.
       * The string is everything up to the first ",[]" char,
       * which is the delimiter.
       */
      for (e = b; *e; e++)
      {
         if ( *e == '[' || *e == ']' || *e == ',' )
            break;
      }

      c = *e;
      if ( *e != 0 )
         *e++ = 0;
   }

   *pb = b;
   *pe = e;

   return c;
}

/*------------------------------------------------------------------------
 * tokenize a string
 * returns: true if successful
 */
bool ogc_token :: tokenize(
   const char * str,
   const char * obj_kwd,
   ogc_error *  err)
{
   const char * start = str;

   ogc_error::clear(err);

   /* sanity checks */
   if ( str == OGC_NULL )
   {
      ogc_error::set(err, OGC_ERR_WKT_EMPTY_STRING, obj_kwd);
      return false;
   }

   while ( isspace(*str) )
      str++;

   if ( *str == 0 )
   {
      ogc_error::set(err, OGC_ERR_WKT_EMPTY_STRING, obj_kwd);
      return false;
   }

   /* Allocate a buffer to store a copy of the string.
      We double the length to deal with possible extra ]
      chars we may have to add at the end.
   */
   _buffer = new (std::nothrow) char [strlen(str) * 2];
   if ( _buffer == OGC_NULL )
   {
      ogc_error::set(err, OGC_ERR_NO_MEMORY, obj_kwd);
      return false;
   }
   *_buffer = 0;

   /* do the first pass through the string */
   if ( !pass1(str, start, obj_kwd, err) )
   {
      return false;
   }

   /* Now do the 2nd pass.
    *
    * At this time, we know the following:
    *   1.  String is non-empty.
    *   2.  String starts with an alphanumeric char.
    *   3.  All non-essential whitespace is gone, including leading
    *       and trailing whitespace in quoted strings.
    *   4.  Quotes and "[]" are matched.
    *   5.  Any internal "]"  is followed by a "," or a "]" char.
    *   6.  Any quoted string is followed by a "," or a "]" char.
    */
   {
      int     level = 0;
      int     count = 0;
      char *  b     = _buffer;
      char *  e;
      int     delim;
      int     prev_delim = 0;
      int     index = 0;

      for (;;)
      {
         delim = parse_substring(&b, &e);

         switch (delim)
         {
            case '[':
               /*---------------------------------------------------------
                * add this name as a new sub-object
                */
               {
                  if ( count >= OGC_TOKENS_MAX )
                  {
                     ogc_error::set(err, OGC_ERR_WKT_MAX_TOKENS_EXCEEDED,
                        obj_kwd);
                     return false;
                  }
                  _arr[count].str = b;
                  _arr[count].lvl = level;
                  _arr[count].idx = 0;
                  count++;
               }
               index = 1;
               level++;
               break;

            case ']':
            case 0:
               /*---------------------------------------------------------
                * end this object
                */
               if ( *b != 0 )
               {
                  if ( count >= OGC_TOKENS_MAX )
                  {
                     ogc_error::set(err, OGC_ERR_WKT_MAX_TOKENS_EXCEEDED,
                        obj_kwd);
                     return false;
                  }
                  _arr[count].str = b;
                  _arr[count].lvl = level;
                  _arr[count].idx = index++;
                  count++;
               }
               level--;
               break;

            case ',':
               /*---------------------------------------------------------
                * entry delimiter
                * We add this string if it is non-empty or if it doesn't
                * follow a object-close.
                */
               if (*b != 0 || prev_delim != ']')
               {
                  if ( count >= OGC_TOKENS_MAX )
                  {
                     ogc_error::set(err, OGC_ERR_WKT_MAX_TOKENS_EXCEEDED,
                        obj_kwd);
                     return false;
                  }
                  _arr[count].str = b;
                  _arr[count].lvl = level;
                  _arr[count].idx = index++;
                  count++;
               }
               break;
         }

         b = e;
         prev_delim = delim;

         if ( level == 0 || delim == 0 )
            break;
      }

      _num = count;
   }

   return true;
}

} /* namespace OGC */
