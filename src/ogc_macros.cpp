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
/* math macros                                                               */
/* ------------------------------------------------------------------------- */

#include "ogc_common.h"

namespace OGC {

/* Get the absolute value of a number */
double ogc_macros :: abs (double a)
{
   return (a < 0) ? -a : a;
}

int    ogc_macros :: abs (int a)
{
   return (a < 0) ? -a : a;
}

/* Assign the sign of the second number to the first */
double ogc_macros :: sgn (double a, double b)
{
   return (b >= 0) ? abs(a) : -abs(a);
}

double ogc_macros :: sgn (double a, int    b)
{
   return (b >= 0) ? abs(a) : -abs(a);
}

int    ogc_macros :: sgn (int    a, int    b)
{
   return (b >= 0) ? abs(a) : -abs(a);
}

/* Return the maximum value of two numbers */
double ogc_macros :: max (double a, double b)
{
   return (a > b) ? a : b;
}

int    ogc_macros :: max (int    a, int    b)
{
   return (a > b) ? a : b;
}

/* Return the minimum value of two numbers */
double ogc_macros :: min (double a, double b)
{
   return (a < b) ? a : b;
}

int    ogc_macros :: min (int    a, int    b)
{
   return (a < b) ? a : b;
}

/* Determine if a double is == another within a given   tolerance */
bool ogc_macros :: eq_eps (double a, double b, double e)
{
   return (a == b) || abs(a-b) <= e*(1+(abs(a)+abs(b))/2);
}

/* Determine if a double is == another within a default tolerance */
bool ogc_macros :: eq (double a, double b)
{
   return eq_eps(a, b, OGC_EPS);
}

/* Determine if a double is != another within a given   tolerance */
bool ogc_macros :: ne_eps (double a, double b, double e)
{
   return !eq_eps(a, b, e);
}

/* Determine if a double is != another within a default tolerance */
bool ogc_macros :: ne (double a, double b)
{
   return !eq(a, b);
}

/* Determine if a double is <= another within a given   tolerance */
bool ogc_macros :: le_eps (double a, double b, double e)
{
   return (a < b) || eq_eps(a, b, e);
}

/* Determine if a double is <= another within a default tolerance */
bool ogc_macros :: le (double a, double b)
{
   return le_eps(a, b, OGC_EPS);
}

/* Determine if a double is >= another within a given   tolerance */
bool ogc_macros :: ge_eps (double a, double b, double e)
{
   return (a > b) || eq_eps(a, b, e);
}

/* Determine if a double is >= another within a default tolerance */
bool ogc_macros :: ge (double a, double b)
{
   return ge_eps(a, b, OGC_EPS);
}

/* Determine if a double is <  another within a given   tolerance */
bool ogc_macros :: lt_eps (double a, double b, double e)
{
   return !ge_eps(a, b, e);
}

/* Determine if a double is <  another within a default tolerance */
bool ogc_macros :: lt (double a, double b)
{
   return !ge(a, b);
}

/* Determine if a double is >  another within a given   tolerance */
bool ogc_macros :: gt_eps (double a, double b, double e)
{
   return !le_eps(a, b, e);
}

/* Determine if a double is >  another within a default tolerance */
bool ogc_macros :: gt (double a, double b)
{
   return !le(a, b);
}

/* Determine if a double is == 0       within a given   tolerance */
bool ogc_macros :: zero_eps (double d, double e)
{
   return (d == 0.0) || (abs(d) <= e);
}

/* Determine if a double is == 0       within a default tolerance */
bool ogc_macros :: zero (double d)
{
   return zero_eps(d, OGC_EPS);
}

/* Determine if an e-squared represents a sphere */
bool ogc_macros :: is_e2_sphere (double e2)
{
   return (e2 < OGC_EPS);
}

/* Convert degrees to radians (assumes -360 to 360) */
double ogc_macros :: deg_to_rad (double d)
{
   return (d * OGC_PI180);
}

/* Convert radians to degrees (assumes -2PI to 2PI) */
double ogc_macros :: rad_to_deg (double r)
{
   return (r * OGC_180PI);
}

/* Convert deg-min-sec to decimal degrees */
double ogc_macros :: dms_to_dd (double d, double m, double s)
{
   return ( d + m / 60.0 + s / (60.0 * 60.0) );
}

} /* namespace OGC */
