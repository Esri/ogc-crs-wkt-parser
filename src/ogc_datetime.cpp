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
 *
 * String format is: "[YYYY-MM-DD][T][hh:mm:ss.sss]".
 * Either part or both parts may be present.
 * If both parts are present, they are separated by 'T'.
 *
 * Note that we are ignoring any trailing timezone info.
 */
bool ogc_datetime :: parse(
   const char * str)
{
   clear();

   if ( strchr(str, 'T') != OGC_NULL )
      return parse_timestamp(str);

   if ( strchr(str, '-') != OGC_NULL )
      return parse_date(str);

   if ( strchr(str, ':') != OGC_NULL )
      return parse_time(str);

   return false;
}

/*------------------------------------------------------------------------
 * parse a datetime string
 *
 * String format is: "YYYY-MM-DDThh:mm:ss.sss [Z|+-h[:m]]".
 */
bool ogc_datetime :: parse_timestamp(
   const char * str)
{
   const char * T = strchr(str, 'T');
   if ( T == OGC_NULL )
      return false;

   if ( ! parse_date(str) )
      return false;

   if ( ! parse_time(T+1) )
      return false;

   if ( ! parse_timezone(T+1) )
      return false;

   return true;
}

/*------------------------------------------------------------------------
 * parse a date string
 *
 * String format is: "YYYY-MM-DD".
 */
bool ogc_datetime :: parse_date(
   const char * str)
{
   const int days[] =
      /* jan  feb  mar  apr  may  jun  jul  aug  sep  oct  nov  dec */
      {   31,  29,  31,  30,  31,  30,  31,  31,  30,  31,  30,  31 };

   for (; isdigit(*str); str++)
   {
      _year = (_year * 10) + (*str - '0');
   }
   if ( _year > 9999 )
      return false;

   for (; !isdigit(*str); str++)
      ;

   for (; isdigit(*str); str++)
   {
      _month = (_month * 10) + (*str - '0');
   }
   if ( _month > 12 )
      return false;

   for (; !isdigit(*str); str++)
      ;

   for (; isdigit(*str); str++)
   {
      _day = (_day * 10) + (*str - '0');
   }
   if ( _month > 0 && _day > days[_month-1] )
      return false;

   return true;
}

/*------------------------------------------------------------------------
 * parse a time string
 *
 * String format is: "hh:mm:ss.sss".
 *
 * Note that we ignore the case where we may have leap seconds (60-62).
 * Note also that this code will ignore any trailing timezone info.
 */
bool ogc_datetime :: parse_time(
   const char * str)
{
   for (; isdigit(*str); str++)
   {
      _hour = (_hour * 10) + (*str - '0');
   }
   if ( _hour > 23 )
      return false;

   for (; !isdigit(*str); str++)
      ;

   for (; isdigit(*str); str++)
   {
      _min = (_min * 10) + (*str - '0');
   }
   if ( _min > 59 )
      return false;

   for (; !isdigit(*str); str++)
      ;

   _sec = ogc_string::atod(str);
   if ( _sec >= 60.0 )
      return false;

   return true;
}

/*------------------------------------------------------------------------
 * parse a timezone string
 *
 * String format is: ... "Z" | [+_hh[:mm]]"
 */
bool ogc_datetime :: parse_timezone(
   const char * str)
{
   const char * s;
   bool  minus = false;
   int   h = 0;
   int   m = 0;

   s = strchr(str, 'Z');
   if ( s != OGC_NULL )
      return true;

   s = strpbrk(str, "+-");
   if ( s == OGC_NULL )
      return true;
   minus = (*s == '-');

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

   return true;
}

/*------------------------------------------------------------------------
 * number of seconds since Jan 1, 1970 UTC
 *
 * Note that we are ignoring the fact that years divisible by 100
 * are not leap years unless they are also divisible by 400.
 * This is valid until the year 2100 (by then someone else can do this).
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

      if ( _tz == 0 )
      {
         strcat(timebuf, "Z");
      }
      else
      {
         char * t = timebuf + strlen(timebuf);
         char pm = '+';
         int  tz = _tz;
         int  h;
         int  m;

         if ( tz < 0 )
         {
            pm = '-';
            tz = -tz;
         }

         h = (tz % 60);
         m = (tz / 60);

         t += sprintf(t, "%c%d", pm, h);
         if ( m != 0 )
            t += sprintf(t, ":%d", m);
      }
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

} /* namespace OGC */
