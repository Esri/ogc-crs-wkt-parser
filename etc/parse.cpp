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
/* program to test token parsing                                             */
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
static FILE *        fp       = NULL;

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
      printf("  -v   Verbose\n");
   }
   else
   {
      fprintf(stderr, "Usage: %s [-v] [filename]\n", pgm);
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

      else if ( strcmp(arg, "v") == 0 )  verbose = true;

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
      OGC_BUFF buffer;
      ogc_token t;
      ogc_error e;

      if ( fgets(buffer, sizeof(buffer), fp) == NULL )
         break;

      if ( ! t.tokenize(buffer, OGC_NULL, &e) )
      {
         printf("invalid: %s\n", e.err_msg());
      }
      else
      {
         for (int i = 0; i < t._num; i++)
         {
            printf("%2d: %2d: ", i, t._arr[i].lvl);
            if ( verbose )
               printf("%2d: ", t._arr[i].idx);

            for (int j = 0; j < t._arr[i].lvl; j++)
               printf("  ");
            printf("%s\n", t._arr[i].str);
         }
         printf("\n");
      }
   }

   fclose(fp);
   return EXIT_SUCCESS;
}
