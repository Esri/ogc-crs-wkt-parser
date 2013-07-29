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
/* String handling methods                                                   */
/* ------------------------------------------------------------------------- */

#include "ogc_common.h"
#include <locale.h>

namespace OGC {

/*------------------------------------------------------------------------
 * get the decimal point character
 */
char ogc_string :: decimal_point_char()
{
   char dec = '.';

#if !defined(ANDROID)
   {
      dec = localeconv()->decimal_point[0];
   }
#endif

   return dec;
}

/*------------------------------------------------------------------------
 * convert a string to an integer
 */
int ogc_string :: atoi(const char * str)
{
   int  num   = 0;
   bool minus = false;

   if ( str == OGC_NULL )
      return 0;

   for (; isspace(*str); str++) {}
   if ( *str == 0 )
      return 0;

   if ( *str == '-' )
   {
      minus = true;
      str++;
   }
   else
   if ( *str == '+' )
   {
      str++;
   }

   for (; isdigit(*str); str++)
   {
      num = (num * 10) + (*str - '0');
   }

   if ( minus )
      num = -num;
   return num;
}

/*------------------------------------------------------------------------
 * convert a string to a double
 */
double ogc_string :: atod(const char * str)
{
   double num = 0.0;

   if ( str == OGC_NULL )
      return 0.0;

   for (; isspace(*str); str++) {}
   if ( *str == 0 )
      return 0.0;

   if ( strlen(str) < OGC_NBUF_MAX )
   {
      char     dec = decimal_point_char();
      OGC_NBUF buf;
      char *   b;

      strcpy(buf, str);
      b = strchr(buf, '.');
      if ( b != OGC_NULL )
         *b = dec;
      num = ::atof(buf);
   }

   return num;
}

/*------------------------------------------------------------------------
 * convert an integer to a string
 */
char * ogc_string :: itoa(int n, OGC_NBUF buf)
{
   if ( buf != OGC_NULL )
   {
      sprintf(buf, "%d", n);
   }

   return buf;
}

/*------------------------------------------------------------------------
 * convert a double to a string
 *
 * There is a lot of esoteric code here to "cleanup" the string.
 * For example, both:
 *
 *    0.09999999999999999
 *    0.10000000000000001
 *
 * will be displayed as:
 *
 *    0.1
 */
char * ogc_string :: dtoa(double d, OGC_NBUF buf)
{
   char dec = decimal_point_char();
   char tmp[64];
   char *pc;
   int  nsigdigits = 16;
   int  i, is16, iexp, ilen, idec;

   if ( buf == OGC_NULL )
      return OGC_NULL;

   if ( nsigdigits <= 0 )
      nsigdigits = 1;
   else if ( nsigdigits > 16 )
      nsigdigits = 16;

   is16 = (nsigdigits == 16) ? 1 : 0;
   ilen = (nsigdigits + 8 - is16);
   idec = (nsigdigits - is16);
   sprintf(tmp, "%*.*e", ilen, idec, d);

   pc = &tmp[ilen - 3];
   while ( *pc != '+' && *pc != '-' )
   {
     pc--;
   }
   iexp = atoi(pc);

   if ( is16 )
   {
      pc = pc - 4;

      if ( iexp < 12 )
      {
         /* Check the last couple of sig digits */

         if ( strncmp(pc, "00", 2) == 0 )
         {
            /* Truncate */
            /* Drop the number of significant digits */
            nsigdigits--;
         }
         else if ( strncmp(pc, "99", 2) == 0 )
         {
            /* Round up */
            /* Drop the number of significant digits */
            nsigdigits--;
         }
      }
   }

   if ( iexp < 0 )
   {
      idec = nsigdigits - 1 + (iexp * -1);
      ilen = idec + 3;

      if ( ilen > 63 )
      {
         idec = nsigdigits;
         ilen = idec + 8;
         sprintf(tmp, "%*.*g", ilen, idec, d);
      }
      else
      {
         sprintf(tmp, "%*.*f", ilen, idec, d);
         for (i = (int)strlen(tmp) - 1; i >= 0; i--)
         {
            if ( tmp[i] != '0' )
            {
               break;
            }
            tmp[i] = 0;
         }
         if ( tmp[i] == dec )
         {
            tmp[i+1] = '0';
            tmp[i+2] = 0;
         }

         if ( strlen(tmp) > 24 || iexp < -9 )
         {
            idec = nsigdigits;
            ilen = idec + 8;
            sprintf(tmp, "%*.*g", ilen, idec, d);
         }
      }
      for (pc = tmp; isspace(*pc); pc++) ;
      strcpy(buf, pc);
   }
   else
   {
      idec = nsigdigits - 1 - iexp;
      if ( idec > -1 )
      {
         ilen = nsigdigits + 2;
         sprintf(buf, "%*.*f", ilen, idec, d);

         if ( strchr(buf, dec) == OGC_NULL )
         {
            char buf2[8];
            sprintf(buf2, "%c0", dec);
            strcat(buf, buf2);
         }

         for (i = (int)strlen(buf) - 1; i >= 0; i--)
         {
            if ( buf[i] != '0' )
            {
               break;
            }
            buf[i] = 0;
         }
         if ( buf[i] == dec )
         {
            buf[i+1] = '0';
            buf[i+2] = 0;
         }
      }
      else
      {
         idec = nsigdigits;
         ilen = idec + 8;
         sprintf(buf, "%*.*g", ilen, idec, d);
      }
   }

   trim_buf(buf);
   return buf;
}

/*------------------------------------------------------------------------
 * case insensitive version of strcmp()
 *
 * This routine treats '-' as '_'.
 */
int ogc_string :: strcmp_i (const char *s1, const char *s2)
{
   const unsigned char * u1 = reinterpret_cast<const unsigned char *>(s1);
   const unsigned char * u2 = reinterpret_cast<const unsigned char *>(s2);

   if ( u1 == OGC_NULL ) u1 = reinterpret_cast<const unsigned char *>("");
   if ( u2 == OGC_NULL ) u2 = reinterpret_cast<const unsigned char *>("");

   for (;;)
   {
      int c1 = toupper(*u1);
      int c2 = toupper(*u2);
      int rc;

      /* treat '-' as '_' */
      if (c1 == '-')  c1 = '_';
      if (c2 == '-')  c2 = '_';

      rc = (c1 - c2);
      if (rc != 0 || c1 == 0 || c2 == 0)
         return rc;

      u1++;
      u2++;
   }
}

/*------------------------------------------------------------------------
 * Check if a character should be ignored in comparisons.
 */
bool ogc_string :: is_ignore_char(int c)
{
  return ( isspace(c) ||
           c == '-'   ||
           c == '_'   ||
           c == '('   ||
           c == ')'   ||
           c == '/'   );
}

/*------------------------------------------------------------------------
 * Get the number of non-ignorable chars in a string.
 */
int ogc_string :: unignore_len(const unsigned char *s)
{
   int n = 0;

   for (; *s; s++)
   {
      if ( !is_ignore_char(*s) )
         n++;
   }

   return n;
}

/*------------------------------------------------------------------------
 * These routines all compare the two strings ignoring special chars.
 *
 * The routine strncmp_ci() is not very useful to a caller because the
 * length specified is the number of non-ignore chars to compare.
 * Use strncmp_ci[12]() instead.
 */
int ogc_string :: strncmp_ci  (const char *s1, const char *s2, size_t n)
{
   const unsigned char * u1  = reinterpret_cast<const unsigned char *>(s1);
   const unsigned char * u2  = reinterpret_cast<const unsigned char *>(s2);
   int                   num = static_cast<int>(n);

   if ( u1 == OGC_NULL ) u1 = reinterpret_cast<const unsigned char *>("");
   if ( u2 == OGC_NULL ) u2 = reinterpret_cast<const unsigned char *>("");

   if ( num == 0 )
      num = -1;

   while (num--)
   {
      int c1;
      int c2;
      int rc;

      for (; is_ignore_char(*u1); u1++) {}
      for (; is_ignore_char(*u2); u2++) {}

      c1 = tolower( *u1 );
      c2 = tolower( *u2 );
      rc = (c1 - c2);

      if (rc != 0 || c1 == 0 || c2 == 0)
         return rc;

      u1++;
      u2++;
   }

   return 0;
}

int ogc_string :: strncmp_ci1 (const char *s1, const char *s2)
{
   const unsigned char * u1 = reinterpret_cast<const unsigned char *>(s1);
   int n;

   if ( u1 == OGC_NULL ) u1 = reinterpret_cast<const unsigned char *>("");
   n = unignore_len(u1);

   return strncmp_ci(s1, s2, n);
}

int ogc_string :: strncmp_ci2 (const char *s1, const char *s2)
{
   const unsigned char * u2 = reinterpret_cast<const unsigned char *>(s2);
   int n;

   if ( u2 == OGC_NULL ) u2 = reinterpret_cast<const unsigned char *>("");
   n = unignore_len(u2);

   return strncmp_ci(s1, s2, n);
}

int ogc_string :: strcmp_ci   (const char *s1, const char *s2)
{
   return strncmp_ci(s1, s2, 0);
}

/*------------------------------------------------------------------------
 * case-insensitive version of strstr()
 */
const char * ogc_string :: strstr_ci (const char *str, const char *srch)
{
   const char * s;
   const char * e;
   int first;
   int len_str;
   int len_srch;

   if ( str   == OGC_NULL ) return OGC_NULL;
   if ( srch  == OGC_NULL ) srch = "";

   if ( *srch == 0 )        return str;
   if ( *str  == 0 )        return OGC_NULL;

   len_str  = unignore_len( reinterpret_cast<const unsigned char *>(str)  );
   len_srch = unignore_len( reinterpret_cast<const unsigned char *>(srch) );

   if (len_srch > len_str)  return OGC_NULL;

   e = (str + (len_str - len_srch));
   first = tolower( *reinterpret_cast<const unsigned char *>(srch) );

   for (s = str; s <= e; s++)
   {
      if ( first == tolower( *reinterpret_cast<const unsigned char *>(s) ) )

      {
         if (strncmp_ci(s, srch, len_srch) == 0)
            return s;
      }
   }

   return OGC_NULL;
}

/*------------------------------------------------------------------------
 * copy a string & make it all upper-case or lower-case
 *
 * returns the number of chars processed
 */
int ogc_string :: strupr (char *buf, const char *str)
{
   const char * s;
   char *       b = buf;

   if ( buf == OGC_NULL ) return 0;
   if ( str == OGC_NULL ) str = "";

   for (s = str; *s; s++)
   {
      *b++ = static_cast<char>
             ( toupper( *reinterpret_cast<const unsigned char *>(s) ) );
   }
   *b = 0;

   return static_cast<int>(b - buf);
}

int ogc_string :: strlwr (char *buf, const char *str)
{
   const char * s;
   char *       b = buf;

   if ( buf == OGC_NULL ) return 0;
   if ( str == OGC_NULL ) str = "";

   for (s = str; *s; s++)
   {
      *b++ = static_cast<char>
             ( tolower( *reinterpret_cast<const unsigned char *>(s) ) );
   }
   *b = 0;

   return static_cast<int>(b - buf);
}

/*------------------------------------------------------------------------
 * like strcpy(), but returns number of chars copied
 */
int ogc_string :: strcpy (char *buf, const char *str)
{
   char * b = buf;
   const char *s;

   if ( buf == OGC_NULL )
   {
      return 0;
   }

   if ( str == OGC_NULL )
   {
      *buf = 0;
      return 0;
   }

   for (s = str; *s; s++)
      *b++ = *s;
   *b = 0;

   return static_cast<int>(b - buf);
}

/*------------------------------------------------------------------------
 * like strncpy(), but returns number of chars copied,
 * and guarantees a null-terminated string.
 */
int ogc_string :: strncpy (char *buf, const char *str, size_t n)
{
   char * b = buf;
   const char *s;

   if ( buf == OGC_NULL || n == 0 )
   {
      return 0;
   }

   if ( str == OGC_NULL || *str == 0 )
   {
      *buf = 0;
      return 0;
   }

   for (s = str; --n && *s; s++)
      *b++ = *s;
   *b = 0;

   return static_cast<int>(b - buf);
}

/*------------------------------------------------------------------------
 * remove all trailing whitespace from a given string
 *
 * Returns: pointer to the string.
 */
char * ogc_string :: strip (char *str)
{
   if ( str != OGC_NULL )
   {
      char * p = OGC_NULL;
      char * s = str;

      for (; *s; s++)
      {
         if ( !isspace( *reinterpret_cast<unsigned char *>(s) ) )
            p = s;
      }

      if (p != OGC_NULL)
         p[1] = 0;
      else
         *str = 0;
   }

   return str;
}

/*------------------------------------------------------------------------
 * remove any leading/trailing whitespace from a string
 *
 * Returns: pointer to the first non-space character in the string.
 */
char * ogc_string :: trim (char *str)
{
   if ( str != OGC_NULL )
   {
      for (; isspace( *reinterpret_cast<unsigned char *>(str) ); str++) {}
      strip(str);
   }

   return str;
}

/*------------------------------------------------------------------------
 * remove any leading/trailing whitespace in place
 *
 * Returns: pointer to the first non-space character in the string.
 */
char * ogc_string :: trim_buf (char *str)
{
   if ( str != OGC_NULL )
   {
      char * p = trim(str);

      if ( p > str )
      {
         char * s = str;

         for (; *p; p++)
            *s++ = *p;
         *s = 0;
      }
   }

   return str;
}

/*------------------------------------------------------------------------
 * escape a string
 *
 * Currently, this consists only of doubling any embedded " chars.
 */
char * ogc_string :: escape_str(
   char *       buf,
   const char * str,
   size_t       buflen)
{
   if ( buf != OGC_NULL )
   {
      if ( str == OGC_NULL )
         str = "";

      char * b = buf;
      char * e = (buf + buflen -1);
      const char * s = str;

      for (; *s && b < e; s++)
      {
         switch (*s)
         {
            case '"':
               *b++ = *s;
               *b++ = *s;
               break;

            default:
               *b++ = *s;
               break;
         }
      }
      *b = 0;
   }

   return buf;
}

/*------------------------------------------------------------------------
 * unescape a string
 *
 * Currently, this consists only of converting any embedded ""
 * sequences to a single " char.
 */
char * ogc_string :: unescape_str(
   char *       buf,
   const char * str,
   size_t       buflen)
{
   if ( buf != OGC_NULL )
   {
      if ( str == OGC_NULL )
         str = "";

      char * b = buf;
      char * e = (buf + buflen -1);
      const char * s = str;

      for (; *s && b < e; s++)
      {
         if ( *s == '"' )
         {
            if ( s == str || s[-1] != '"' )
               *b++ = *s;
         }
         else
         {
            *b++ = *s;
         }
      }
      *b = 0;
   }

   return buf;
}

/*------------------------------------------------------------------------
 * get the unescape length of a string
 */
int ogc_string :: unescape_len(
   const char * str)
{
   if ( str == OGC_NULL || *str == 0 )
      return 0;

   int len = 0;
   for (const char * s = str; *s; s++)
   {
      if ( *s == '"' )
      {
         if ( s == str || s[-1] != '"' )
            len++;
      }
      else
      {
         len++;
      }
   }

   return len;
}

} /* namespace OGC */
