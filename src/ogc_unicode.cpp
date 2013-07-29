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
/* UNICODE UTF8 encoding and decoding methods                                */
/* ------------------------------------------------------------------------- */

#include "ogc_common.h"

namespace OGC {

/*------------------------------------------------------------------------
 * convert a unicode string to UTF8
 */
int ogc_unicode :: unicode_to_utf8(
   char              target[],
   const OGC_UCHAR * source,
   int               n)
{
   const OGC_UCHAR * s = source;
   char *            t = target;

   /* sanity checks */

   if ( target == OGC_NULL )
   {
      return 0;
   }

   if ( source == OGC_NULL )
   {
      *target = 0;
      return 0;
   }

   /* do the actual conversion */
   {
      for (; n && *s; s++)
      {
         OGC_UCHAR u = *s;

         if (u < 0x0080)
         {
            /*----------------------------------------------
             * 00000000 0xxxxxxx   ->   0xxxxxxx
             */
            if (--n != 0)
               *t++ = static_cast<char>(u);
         }

         else
         if (u < 0x0800)
         {
            /*----------------------------------------------
             * 00000yyy yxxxxxxx   ->   110yyyyx
             *                          10xxxxxx
             */
            if (--n != 0)
               *t++ = static_cast<char>(((u         ) >>  6) | 0xc0);

            if (n != 0 && --n != 0)
               *t++ = static_cast<char>(((u & 0x003f)      ) | 0x80);
         }

         else
         {
            /*----------------------------------------------
             * zzzzzyyy yxxxxxxx   ->   1110zzzz
             *                          10zyyyyx
             *                          10xxxxxx
             */
            if (--n != 0)
               *t++ = static_cast<char>(((u         ) >> 12) | 0xe0);

            if (n != 0 && --n != 0)
               *t++ = static_cast<char>(((u & 0x0fc0) >>  6) | 0x80);

            if (n != 0 && --n != 0)
               *t++ = static_cast<char>(((u & 0x003f)      ) | 0x80);
         }
      }
      *t = 0;
   }

   return static_cast<int>(t - target);
}

/*------------------------------------------------------------------------
 * convert a UTF8 string to unicode
 */
int ogc_unicode :: utf8_to_unicode(
   OGC_UCHAR    target[],
   const char * source,
   int          n)
{
   const unsigned char * s = reinterpret_cast<const unsigned char *>(source);
   int i = 0;

   /* sanity checks */

   if ( target == OGC_NULL )
   {
      return 0;
   }

   if ( source == OGC_NULL )
   {
      *target = 0;
      return 0;
   }

   /* do the actual conversion */
   {
      typedef enum chartype
         { REG, OPEN1, OPEN2, OPEN3, CONT, INVALID } CHARTYPE;
      typedef enum state
         { NORMAL, INSEQ } STATE;

      OGC_UCHAR bad_char = '?';
      STATE    state     = NORMAL;
      int      current   = 0;
      int      need      = 0;

#     define RESET()  state = NORMAL; current = 0; need = 0

      n--;
      for (; *s; s++)
      {
         int        c;
         CHARTYPE   char_type;

         if ( n > 0 && i >= n )
            break;

         c = *s;

         if      ((c & 0x80) == 0x00) char_type = REG;
         else if ((c & 0xe0) == 0xc0) char_type = OPEN1;
         else if ((c & 0xf0) == 0xe0) char_type = OPEN2;
         else if ((c & 0xf8) == 0xf0) char_type = OPEN3;
         else if ((c & 0xc0) == 0x80) char_type = CONT;
         else                         char_type = INVALID;

         switch (char_type)
         {
         case REG:
            if (state == NORMAL)
            {
               target[i++] = (OGC_UCHAR)c;
            }
            else
            {
               target[i++] = bad_char;
               RESET();
            }
            break;

         case OPEN1:
            if (state == NORMAL)
            {
               state   = INSEQ;
               current = c & 0x1f;
               need    = 1;
            }
            else
            {
               target[i++] = bad_char;
               RESET();
            }
            break;

         case OPEN2:
            if (state == NORMAL)
            {
               state   = INSEQ;
               current = c & 0x0f;
               need    = 2;
            }
            else
            {
               target[i++] = bad_char;
               RESET();
            }
            break;

         case OPEN3:
            if (state == NORMAL)
            {
               state   = INSEQ;
               current = c & 0x07;
               need    = 3;
            }
            else
            {
               target[i++] = bad_char;
               RESET();
            }
            break;

         case CONT:
            if (state == INSEQ)
            {
               c &= 0x3f;
               current = (current << 6) | c;
               if (--need == 0)
               {
                  target[i++] = (OGC_UCHAR)current;
                  RESET();
               }
            }
            break;

         case INVALID:
            target[i++] = bad_char;
            RESET();
            break;
         }
      }

#     undef RESET

      target[i] = 0;
   }

   return i;
}

/*------------------------------------------------------------------------
 * like strcpy(), but returns number of chars copied
 */
int ogc_unicode :: strcpy_u(
   OGC_UCHAR         target[],
   const OGC_UCHAR * source)
{
   OGC_UCHAR * b = target;
   const OGC_UCHAR *s;

   if ( target == OGC_NULL )
   {
      return 0;
   }

   if ( source == OGC_NULL )
   {
      *target = 0;
      return 0;
   }

   for (s = source; *s; s++)
      *b++ = *s;
   *b = 0;

   return static_cast<int>(b - target);
}

/*------------------------------------------------------------------------
 * like strncpy(), but returns number of chars copied,
 * and guarantees a null-terminated string.
 */
int ogc_unicode :: strncpy_u(
   OGC_UCHAR         target[],
   const OGC_UCHAR * source,
   int               n)
{
   OGC_UCHAR * b = target;
   const OGC_UCHAR *s;

   if ( target == OGC_NULL )
   {
      return 0;
   }

   if ( source == OGC_NULL )
   {
      *target = 0;
      return 0;
   }

   for (s = source; --n && *s; s++)
      *b++ = *s;
   *b = 0;

   return static_cast<int>(b - target);
}

/*------------------------------------------------------------------------
 * get number of unicode chars
 */
int ogc_unicode :: strlen_u(const OGC_UCHAR *str)
{
   int len = 0;

   if ( str != OGC_NULL )
   {
      for (; *str; str++)
         len++;
   }

   return len;
}

/*------------------------------------------------------------------------
 * get the UTF8 length of a unicode string
 */
int ogc_unicode :: unicode_to_utf8_len (const OGC_UCHAR *source)
{
   const OGC_UCHAR * s = source;
   int num = 0;

   if (s != OGC_NULL && *s != 0 )
   {
      for (; *s; s++)
      {
         OGC_UCHAR u = *s;

         if (u < 0x0080)
         {
            /*----------------------------------------------
             * 00000000 0xxxxxxx   ->   0xxxxxxx
             */
            num += 1;
         }

         else
         if (u < 0x0800)
         {
            /*----------------------------------------------
             * 00000yyy yxxxxxxx   ->   110yyyyx
             *                          10xxxxxx
             */
            num += 2;
         }

         else
         {
            /*----------------------------------------------
             * zzzzzyyy yxxxxxxx   ->   1110zzzz
             *                          10zyyyyx
             *                          10xxxxxx
             */
           num += 3;
         }
      }
   }

   return num;
}

/*------------------------------------------------------------------------
 * get the unicode length of a UTF8-encoded string
 */
int ogc_unicode :: utf8_to_unicode_len(const char *str)
{
   const char * s = str;
   int len = 0;

   /*---------------------------------------------------------------------
    * sanity checks
    */
   if (str == OGC_NULL || *str == 0)
   {
      return 0;
   }

   /*---------------------------------------------------------------------
    * do the actual conversion
    */
   {
      typedef enum chartype
         { REG, OPEN1, OPEN2, OPEN3, CONT, INVALID } CHARTYPE;
      typedef enum state
         { NORMAL, INSEQ } STATE;

      STATE    state     = NORMAL;
      int      current   = 0;
      int      need      = 0;

#     define RESET()  state = NORMAL; current = 0; need = 0

      for (; *s; s++)
      {
         int        c = *((const unsigned char *)s);
         CHARTYPE   char_type;

         if      ((c & 0x80) == 0x00) char_type = REG;
         else if ((c & 0xe0) == 0xc0) char_type = OPEN1;
         else if ((c & 0xf0) == 0xe0) char_type = OPEN2;
         else if ((c & 0xf8) == 0xf0) char_type = OPEN3;
         else if ((c & 0xc0) == 0x80) char_type = CONT;
         else                         char_type = INVALID;

         switch (char_type)
         {
         case REG:
            if (state == NORMAL)
            {
               len++;
            }
            else
            {
               len++;
               RESET();
            }
            break;

         case OPEN1:
            if (state == NORMAL)
            {
               state   = INSEQ;
               current = c & 0x1f;
               need    = 1;
            }
            else
            {
               len++;
               RESET();
            }
            break;

         case OPEN2:
            if (state == NORMAL)
            {
               state   = INSEQ;
               current = c & 0x0f;
               need    = 2;
            }
            else
            {
               len++;
               RESET();
            }
            break;

         case OPEN3:
            if (state == NORMAL)
            {
               state   = INSEQ;
               current = c & 0x07;
               need    = 3;
            }
            else
            {
               len++;
               RESET();
            }
            break;

         case CONT:
            if (state == INSEQ)
            {
               c &= 0x3f;
               current = (current << 6) | c;
               if (--need == 0)
               {
                  len++;
                  RESET();
               }
            }
            break;

         case INVALID:
            len++;
            RESET();
            break;
         }
      }

#     undef RESET
   }

   return len;
}

} /* namespace OGC */
