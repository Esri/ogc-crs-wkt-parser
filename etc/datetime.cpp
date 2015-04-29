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
/* program to test date-time string parsing                                  */
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
static const char *  inp_file = "-";
static bool          verbose  = false;
static int           ndigits  = 0;
static FILE *        fp       = NULL;

/*------------------------------------------------------------------------
 * display usage
 */
static void usage(int level)
{
   if (level)
   {
      printf("%s: Test the parsing of date-time strings\n", pgm);
      printf("Usage: %s [options] [filename]\n", pgm);
      printf("Options:\n");
      printf("  -?, -help     Display usage\n");
      printf("  -V, -version  Display version\n");
      printf("  -v            Verbose\n");
      printf("  -n ndig       Specify number of digits in seconds\n");

      printf("Arguments:\n");
      printf("  filename      File of date-time strings to read (default is stdin)\n");
   }
   else
   {
      fprintf(stderr, "Usage: %s [-v] [-n num] [filename]\n", pgm);
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

      else if (strcmp(arg, "V")       == 0 ||
               strcmp(arg, "version") == 0)
      {
         printf("%s: version %s\n", pgm, OGC_VERSION_STR);
         exit(EXIT_SUCCESS);
      }

      else if ( strcmp(arg, "v") == 0 )  verbose = true;

      else if ( strcmp(arg, "n") == 0 )
      {
         if ( ++optcnt >= argc )
         {
            fprintf(stderr, "%s: option needs an argument -- -%s\n",
               pgm, "n");
            usage(0);
            exit(EXIT_FAILURE);
         }
         ndigits = atoi( argv[optcnt] );
         if ( ndigits > 9 )
            ndigits = 9;
      }

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
 * main()
 */
int main(int argc, const char **argv)
{
   process_options(argc, argv);

   for (;;)
   {
      OGC_TIME inp_buf;
      OGC_TIME out_buf;
      ogc_datetime t;

      if ( fgets(inp_buf, sizeof(inp_buf), fp) == NULL )
         break;

      ogc_string::trim_buf(inp_buf);
      if ( *inp_buf == 0 )
      {
         t.now();
      }
      else
      {
         if ( ! t.parse(inp_buf) )
         {
            printf("invalid\n");
            continue;
         }
      }

      if ( verbose )
      {
         printf("year  = %d\n",            t.year () );
         printf("month = %d\n",            t.month() );
         printf("day   = %d\n",            t.day  () );
         printf("hour  = %d\n",            t.hour () );
         printf("min   = %d\n",            t.min  () );
         printf("sec   = %.*f\n", ndigits, t.sec  () );

         int  tz = t.tz();
         if ( tz == 0 )
         {
            printf("tz    = UTC\n");
         }
         else
         {
            bool minus = (tz < 0 );
            if ( minus )
               tz = -tz;

            int h = (tz / 60);
            int m = (tz % 60);

            printf("tz    = %c%d.%d\n", (minus ? '-' : '+'), h, m);
         }
      }

      t.timestamp_str(out_buf, ndigits);
      printf("%s\n", out_buf);

      printf("\n");
   }

   fclose(fp);
   return EXIT_SUCCESS;
}
