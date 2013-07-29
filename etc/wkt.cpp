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
/* program to process WKT strings                                            */
/* ------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libogc.h"
using namespace OGC;

/*------------------------------------------------------------------------
 * options
 */
static const char *  pgm;
static const char *  inp_file   = "-";
static int           wkt_opts   = OGC_WKT_OPT_NONE;
static bool          multi_line = false;
static bool          name_only  = false;
static bool          strict     = false;
static FILE *        fp         = NULL;

/*------------------------------------------------------------------------
 * display usage
 */
static void usage(int level)
{
   if (level)
   {
      printf("Usage: %s [options] [filename]\n", pgm);
      printf("Options:\n");
      printf("  -?   Display usage\n");
      printf("  -m   Process multi-line input\n");
      printf("  -n   Show name only\n");
      printf("  -o   Display in old syntax\n");

      printf("  -i   Don't show any ID attributes\n");
      printf("  -t   Show top-level ID attribute only\n");
      printf("  -p   Show () instead of [] in strings\n");

      printf("  -x   Expand with spaces\n");
      printf("  -X   Expand with tabs\n");

      if ( strict )
      {
         printf("  -r   Relax strict parsing\n");
         printf("  -s   Set   strict parsing (default)\n");
      }
      else
      {
         printf("  -r   Relax strict parsing (default)\n");
         printf("  -s   Set   strict parsing\n");
      }
   }
   else
   {
      fprintf(stderr,
         "Usage: %s [-m] [-n] [-o] [-i|-t] [-p] [-x|-X] [-r|-s] [filename]\n",
         pgm);
   }
}

/*------------------------------------------------------------------------
 * process command-line options
 */
static int process_options (int argc, const char **argv)
{
   int optcnt;

                  pgm = strrchr(argv[0], '/');
   if (pgm == 0)  pgm = strrchr(argv[0], '\\');
   if (pgm == 0)  pgm = argv[0];
   else           pgm++;

   strict = ogc_object :: get_strict_parsing();

   for (optcnt = 1; optcnt < argc; optcnt++)
   {
      const char * arg = argv[optcnt];

      if (*arg != '-')
         break;

      while (*arg == '-')
         arg++;
      if ( !*arg )
      {
         optcnt++;
         break;
      }

      else if (strcmp(arg, "?")    == 0 ||
               strcmp(arg, "help") == 0)
      {
         usage(1);
         exit(EXIT_SUCCESS);
      }

      else if ( strcmp(arg, "m")   == 0 ) multi_line  = true;
      else if ( strcmp(arg, "n")   == 0 ) name_only   = true;
      else if ( strcmp(arg, "o")   == 0 ) wkt_opts   |= OGC_WKT_OPT_OLD_SYNTAX;
      else if ( strcmp(arg, "i")   == 0 ) wkt_opts   |= OGC_WKT_OPT_NO_IDS;
      else if ( strcmp(arg, "t")   == 0 ) wkt_opts   |= OGC_WKT_OPT_TOP_ID_ONLY;
      else if ( strcmp(arg, "p")   == 0 ) wkt_opts   |= OGC_WKT_OPT_PARENS;
      else if ( strcmp(arg, "x")   == 0 ) wkt_opts   |= OGC_WKT_OPT_EXPAND_SP;
      else if ( strcmp(arg, "X")   == 0 ) wkt_opts   |= OGC_WKT_OPT_EXPAND_TAB;

      else if ( strcmp(arg, "r")   == 0 ) ogc_object::set_strict_parsing(false);
      else if ( strcmp(arg, "s")   == 0 ) ogc_object::set_strict_parsing(true);

      else
      {
         fprintf(stderr, "%s: Invalid option -- %s\n", pgm, argv[optcnt]);
         usage(0);
         exit(EXIT_FAILURE);
      }
   }

   if ( optcnt < argc )
   {
      inp_file = argv[optcnt++];
   }

   if ( strcmp(inp_file, "-") == 0 )
   {
      fp = stdin;
   }
   else
   {
      fp = fopen(inp_file, "r");
      if ( fp == OGC_NULL )
      {
         fprintf(stderr, "%s: cannot open file %s\n", pgm, inp_file);
         exit(EXIT_FAILURE);
      }
   }

   return optcnt;
}

/*------------------------------------------------------------------------
 * error call-back routine
 */
static void error_rtn(
   void *       data,
   ogc_err_code err_code,
   const char * err_msg)
{
   (void)(data);
   (void)(err_code);

   printf("%s\n", err_msg);
}

/*------------------------------------------------------------------------
 * read in a WKT buffer to process
 *
 * The multi-line option is designed to read in WKT strings continued
 * in multiple lines and also WKT strings that have a trailing comma
 * at the end of a line.
 *
 * This is so we can cut-and-paste example WKT strings from Roger's
 * WKT-CRS doc.
 */
static int get_inp_buffer(
   OGC_BUFF inp_buffer)
{
   if ( multi_line )
   {
      char * b     = inp_buffer;
      int    count = 0;
      int    c;

      for (;;)
      {
         c = fgetc(fp);
         if ( c == EOF )
            return EOF;

         *b++ = static_cast<char>(c);

         if ( c == '[' )
         {
            count++;
         }
         else
         if ( c == ']' )
         {
            count--;
            if ( count == 0 )
            {
               /* skip over rest of line */
               for (;;)
               {
                  c = fgetc(fp);
                  if ( c == EOF || c == '\n' )
                     break;
               }
               break;
            }
         }
      }
      *b = 0;
   }
   else
   {
      if ( fgets(inp_buffer, OGC_BUFF_MAX, fp) == NULL )
         return EOF;
   }

   return 0;
}

/*------------------------------------------------------------------------
 * main()
 */
int main(int argc, const char **argv)
{
   process_options(argc, argv);
   ogc_error::set_err_rtn( error_rtn );

   for (;;)
   {
      ogc_object * obj;
      OGC_BUFF     inp_buffer;
      OGC_BUFF     out_buffer;

      if ( get_inp_buffer(inp_buffer) == EOF )
         break;

      /* ignore blank lines */
      ogc_string::trim_buf(inp_buffer);
      if ( *inp_buffer == 0 )
         continue;

      obj = ogc_object::from_wkt(inp_buffer);
      if ( obj != OGC_NULL )
      {
         if ( name_only )
         {
            printf("%s\n", obj->name());
         }
         else
         {
            obj->to_wkt(out_buffer, wkt_opts);
            printf("%s\n", out_buffer);
         }
         delete obj;
      }
   }

   fclose(fp);
   return EXIT_SUCCESS;
}
