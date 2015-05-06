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
/* SQL-like date-time object                                                 */
/* ------------------------------------------------------------------------- */

#include "ogc_common.h"

namespace OGC {

/*------------------------------------------------------------------------
 * constructors
 */
ogc_datetime :: ogc_datetime()
{
   clear();
}

ogc_datetime :: ogc_datetime(int year, int month, int    day,
                             int hour, int min,   double sec,
                             int tz)
{
   _year  = year;
   _month = month;
   _day   = day;
   _hour  = hour;
   _min   = min;
   _sec   = sec;
   _tz    = tz;
}

ogc_datetime :: ogc_datetime(int year, int month, int    day,
                             int hour, int min,   double sec)
{
   _year  = year;
   _month = month;
   _day   = day;
   _hour  = hour;
   _min   = min;
   _sec   = sec;
   _tz    = 0;
}

ogc_datetime :: ogc_datetime(int year, int month, int day)
{
   _year  = year;
   _month = month;
   _day   = day;
   _hour  = 0;
   _min   = 0;
   _sec   = 0.0;
   _tz    = 0;
}

ogc_datetime :: ogc_datetime(int hour, int min, double sec)
{
   _year  = 0;
   _month = 0;
   _day   = 0;
   _hour  = hour;
   _min   = min;
   _sec   = sec;
   _tz    = 0;
}

/*------------------------------------------------------------------------
 * clear all values
 */
void ogc_datetime :: clear()
{
   _year  = 0;
   _month = 0;
   _day   = 0;
   _hour  = 0;
   _min   = 0;
   _sec   = 0.0;
   _tz    = 0;
}

/*------------------------------------------------------------------------
 * set to today
 */
void ogc_datetime :: today()
{
   now();
   _hour    = 0;
   _min     = 0;
   _sec     = 0.0;
}

/*------------------------------------------------------------------------
 * set to now
 */
void ogc_datetime :: now()
{
   time_t      t  = time(OGC_NULL);
   struct tm * tm = gmtime(&t);

   _year  = tm->tm_year + 1900;
   _month = tm->tm_mon;
   _day   = tm->tm_mday;
   _hour  = tm->tm_hour;
   _min   = tm->tm_min;
   _sec   = tm->tm_sec;
   _tz    = 0;
}

/*------------------------------------------------------------------------
 * parse a datetime string
 */
bool ogc_datetime :: parse(
   const char * str)
{
   OGC_TIME timebuf;
   const char * s;
   char * t;

   clear();

   if ( str == OGC_NULL )
      return false;

   /* copy string, remove WS, uppercase all chars */
   t = timebuf;
   for (s = str; *s; s++)
   {
      if ( !isspace(*s) )
         *t++ = (char)toupper(*s);
   }
   *t = 0;

   s = timebuf;
   if ( *s == 0 )
      return false;

   if ( !parse_date(&s) )
      return false;
   if ( *s == 0 )
      return true;

   if ( *s++ != 'T' )
      return false;

   if ( !parse_time(&s) )
      return false;
   if ( *s == 0 )
      return true;

   if ( !parse_tz(&s) )
      return false;

   return true;
}

/*------------------------------------------------------------------------
 * collect digits
 */
static int collect (const char ** str, int *num_dig)
{
   const char * s = *str;
   int num = 0;

   *num_dig = 0;
   for (; isdigit(*s); s++)
   {
      num = (num * 10) + (*s - '0');
      (*num_dig)++;
   }

   *str = s;
   return num;
}

/*------------------------------------------------------------------------
 * parse a date string
 *
 * String format is:
 *    YYYY
 *    YYYY-MM
 *    YYYY-MM-DD
 *    YYYY-DDD
 */
bool ogc_datetime :: parse_date(
   const char ** str)
{
   const int month_days_norm[] =
      /* jan  feb  mar  apr  may  jun  jul  aug  sep  oct  nov  dec */
      {   31,  28,  31,  30,  31,  30,  31,  31,  30,  31,  30,  31 };
   const int month_days_leap[] =
      /* jan  feb  mar  apr  may  jun  jul  aug  sep  oct  nov  dec */
      {   31,  29,  31,  30,  31,  30,  31,  31,  30,  31,  30,  31 };
   const int * month_days;

   const char * s = *str;
   int num;
   int num_digs;

   /* get year */

   num = collect(&s, &num_digs);
   if ( num_digs != 4 )
      return false;
   _year = num;

   month_days = (is_leapyear() ? month_days_leap : month_days_norm);

   if ( *s == 0 )
   {
      *str = s;
      return true;
   }
   if ( *s++ != '-' )
      return false;

   /* get month or ordinal-day */

   num = collect(&s, &num_digs);
   if ( num_digs < 2 || num_digs > 3)
      return false;
   if ( num_digs == 3 )
   {
      /* ordinal day */

      int i;

      if ( num == 0 || num > (365 + (is_leapyear() ? 1 : 0)) )
         return false;
      for (i = 0; i < 11; i++)
      {
         if ( num <= month_days[i] )
            break;
         num -= month_days[i];
      }
      _month = i+1;
      _day   = num;

      *str = s;
      return true;
   }
   else
   {
      if ( num == 0 || num > 12 )
         return false;
      _month = num;

      if ( *s == 0 )
      {
         *str = s;
         return true;
      }
      if ( *s++ != '-' )
         return false;

      /* get day */

      num = collect(&s, &num_digs);
      if ( num_digs != 2 )
         return false;
      if ( num == 0 || num > month_days[_month-1] )
         return false;
      _day = num;

      *str = s;
      return true;
   }
}

/*------------------------------------------------------------------------
 * parse a time string
 *
 * String format is: "hh:mm:ss.sss".
 *
 * Note that we ignore the case where we may have leap seconds (60-62).
 */
bool ogc_datetime :: parse_time(
   const char ** str)
{
   const char * s = *str;
   double dbl;
   int num;
   int num_digs;

   /* get hour */

   num = collect(&s, &num_digs);
   if ( num_digs != 2 )
      return false;
   if ( num > 23 )
      return false;
   _hour = num;

   if ( *s == 0 )
   {
      *str = s;
      return true;
   }
   if ( *s++ != ':' )
      return false;

   /* get minute */

   num = collect(&s, &num_digs);
   if ( num_digs != 2 )
      return false;
   if ( num > 59 )
      return false;
   _min = num;

   if ( *s == 0 )
   {
      *str = s;
      return true;
   }
   if ( *s++ != ':' )
      return false;

   /* get second (as a double) */

   char buf[24];
   char * b = buf;
   for (; *s; s++)
   {
      if (*s != '.' && !isdigit(*s) )
         break;
      *b = *s;
   }
   *b = 0;

   dbl = atof(buf);
   if ( dbl >= 60.0 )
      return false;
   _sec = dbl;

   *str = s;
   return true;
}

/*------------------------------------------------------------------------
 * parse a timezone string
 *
 * String format is: ... "Z" | [+_hh[:mm]]"
 */
bool ogc_datetime :: parse_tz(
   const char ** str)
{
   const char * s = *str;
   bool  minus = false;
   int   h = 0;
   int   m = 0;

   if ( *s == 'Z' )
      return true;

   if ( *s == '+' )
      minus = false;
   else if (*s == '-' )
      minus = true;
   else
      return false;

   for (s++; isdigit(*s); s++)
      h = (10 * h) + (*s - '0');
   if ( h > 23 )
      return false;

   if ( *s == ':' )
   {
      for (s++; isdigit(*s); s++)
         m = (10 * m) + (*s - '0');
      if ( m > 59 )
         return false;
   }

   _tz = (h * 60) + m;
   if ( minus )
      _tz = -_tz;

   *str = s;
   return true;
}

/*------------------------------------------------------------------------
 * number of seconds since Jan 1, 1970 UTC
 */
time_t ogc_datetime :: unixtime() const
{
   time_t secs = 0;

   if ( _year >= 1970 )
   {
      secs  = ((_year - 1970) * 365);
      secs += ((_year - 1969) / 4);
      secs += yday();
      secs *= (24 * 60 * 60);

      secs += _hour * (60 * 60);
      secs += _min  * (60);
      secs += (int)_sec;

      secs += (_tz * 60);
   }

   return secs;
}

/*------------------------------------------------------------------------
 * days since Jan 1 (0-365)
 *
 * Note that we are ignoring the fact that years divisible by 100
 * are not leap years unless they are also divisible by 400.
 * This is valid until the year 2100 (by then someone else can do this).
 */
int ogc_datetime :: yday() const
{
   const int days_leap[] =
      /* jan  feb  mar  apr  may  jun  jul  aug  sep  oct  nov  dec */
      {    0,  31,  60,  91, 121, 152, 182, 213, 244, 274, 305, 335 };

   const int days_norm[] =
      /* jan  feb  mar  apr  may  jun  jul  aug  sep  oct  nov  dec */
      {    0,  31,  59,  90, 120, 151, 181, 212, 243, 273, 304, 334 };

   int days = _day - 1;
   if ( (_year / 4) == 0 )
      days += days_leap[_month-1];
   else
      days += days_norm[_month-1];

   return days;
}

/*------------------------------------------------------------------------
 * check if leapyear
 *
 * Note that we are ignoring the fact that years divisible by 100
 * are not leap years unless they are also divisible by 400.
 * This is valid until the year 2100 (by then someone else can do this).
 */
bool ogc_datetime :: is_leapyear() const
{
   return ( (_year % 4) == 0 );
}

/*------------------------------------------------------------------------
 * create datetime string "YYYY-MM-DDThh:mm:ss.sss[Z|+-h[:m]]"
 */
char * ogc_datetime :: timestamp_str(
   OGC_TIME timebuf,
   int sec_digits) const
{
   if ( timebuf != OGC_NULL )
   {
      date_str(timebuf);
      strcat  (timebuf, "T");
      time_str(timebuf+11, sec_digits);

      char * t = timebuf + strlen(timebuf);
      tz_str(t);
   }

   return timebuf;
}

/*------------------------------------------------------------------------
 * create date string "YYYY-MM-DD"
 */
char * ogc_datetime :: date_str(
   OGC_TIME timebuf) const
{
   if ( timebuf != OGC_NULL )
   {
      sprintf(timebuf, "%04d-%02d-%02d", _year, _month, _day);
   }

   return timebuf;
}

/*------------------------------------------------------------------------
 * create time string "hh:mm:ss[.sss]"
 */
char * ogc_datetime :: time_str(
   OGC_TIME timebuf,
   int sec_digits) const
{
   if ( timebuf != OGC_NULL )
   {
      if ( sec_digits == 0 || (int)_sec == _sec )
      {
         sprintf(timebuf, "%02d:%02d:%02d",
            _hour, _min, (int)_sec);
      }
      else
      {
         if ( sec_digits < 0 ) sec_digits = 0;
         if ( sec_digits > 9 ) sec_digits = 9;

         sprintf(timebuf, "%02d:%02d:%.*f",
            _hour, _min, sec_digits, _sec);
      }
   }

   return timebuf;
}

/*------------------------------------------------------------------------
 * create timezone string "Z" or "+-hh[:mm]"
 */
char * ogc_datetime :: tz_str(
   OGC_TIME timebuf) const
{
   if ( _tz == 0 )
   {
      strcpy(timebuf, "Z");
   }
   else
   {
      char * t = timebuf;
      char pm = '+';
      int  tz = _tz;

      if ( tz < 0 )
      {
         pm = '-';
         tz = -tz;
      }

      int h = (tz / 60);
      int m = (tz % 60);

      t += sprintf(t, "%c%d", pm, h);
      if ( m != 0 )
         t += sprintf(t, ":%d", m);
   }

   return timebuf;
}

} /* namespace OGC */
