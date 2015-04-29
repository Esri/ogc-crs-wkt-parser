## ogc-crs-wkt-parser

### Introduction

This package contains sample code for a library that implements the
OGC WKT specification titled "Geographic information - Well-known text
representation of coordinate reference systems" (referred to as the WKT-SPEC).
This specification is for defining a Well-Known-Text (WKT) syntax
for describing coordinate reference systems as defined in the following
standards:

    ISO 19111:2007    "Geographic information -- Spatial referencing by coordinates"
    ISO 19111-2:2009  "Geographic information -- Spatial referencing by coordinates --
                       Part 2: Extension for parametric values".

This package also contains some sample programs that use this library.

This package is not an official offering from the OGC, but is provided
by Esri as a proof-of-concept implementation of the WKT-SPEC, with
the intent of testing the proposed specification and highlighting potential
issues with the WKT-SPEC.

Included in this package is the document ogc_wkt_crs.pdf,
which been submitted by ISO TC211 for publication as an International
Standard document. The version incorporates comments made during both the
OGC Public Comment Period as well as the ISO ballot for DIS (ISO TC211
document N3750).

### Contents

This package consists of a library and sample programs using the library.
Only source code is provided. A user will have to build the package
if he wants to try using it. To aid in building this package,
we have included GNU makefiles as well as solution/project files
for Visual Studio versions 2008, 2012, and 2013.

This code is provided to the committee charged with developing and
approving the WKT-SPEC with the intent that they can study it to
determine the usability of the WKT spec.

### Usability

This code is provided to help evaluate the specification, and is 
not intended to be used in any production environment at this time.
This library provides the following functions only:

*  Convert WKT strings to internal objects.
*  Convert internal objects to WKT strings.
*  Create and destroy internal objects.
*  Clone internal objects.
*  Determine whether two internal objects are identical or are
   computationally equivalent.

There is no attempt here to provide any code to actually use these objects.

### Implementation

Although this code closely follows the WKT-SPEC, there are areas
where this implementation deviates from or exceeds the spec. The
following is a list of these areas. This list may not be complete.

   1.  This implementation only handles WKT strings that are ASCII
       (single-byte character) strings. The WKT-SPEC does not specify
       any encoding of WKT strings. This implementation assumes that any
       encoding other than simple ASCII encoding is processed into
       ASCII prior to the calling of these routines.

       Conversion routines are provided to convert to and from Unicode
       strings, with any non-ASCII Unicode characters being UTF8 encoded
       or decoded.

   2.  The WKT-SPEC mandates that only the REMARK attribute may contain
       non-ASCII characters, but this implementation does nothing to
       check that any other string part contains strictly ASCII characters.

   3.  The WKT-SPEC does not address any error reporting of incorrect
       WKT strings nor any action that should be taken. This code
       attempts to stringently assure that any violation of the
       WKT-SPEC is caught and reported. It is possible for multiple
       error messages to be issued for a single WKT string.

       The ability is provided to choose between extremely strict parsing
       of strings or relaxing some of the rules.

       If strict parsing is relaxed, the following happens:
    
          1. Missing trailing ] characters is allowed.
          2. Token parsing stops at the matching ] character to the opening
             [ character. (ie. any characters after the closing ]
             character are ignored.)
          3. Extra atomic (ie. non sub-object) tokens in an object are ignored.

   4.  The WKT-SPEC specifies an order for all sub-objects in a WKT string.
       This implementation follows that order when outputting sub-objects
       into a WKT string, but allows them to appear in any order when reading
       WKT strings.

       This is done regardless of whether strict-parsing is enabled or not.

   5.  Object names are allowed to be empty, except for the following objects:

          METHOD (PROJECTION)
          PARAMETER
          PARAMETERFILE
          VDATUM

       Most importantly, a CONVERSION object is allowed to have an empty name,
       as without that rule an old-style WKT cannot be converted into a
       new-style WKT without additional input. However, currently when
       processing an old-style PROJCS WKT string the PROJECTION name
       is copied into the CONVERSION name.

   6.  Underscores are allowed in keywords, but they are ignored.

   7.  WKT strings in the old syntax identified by the following
       keywords will be processed into a new-style object:

          GEOGCS     -> GEODCRS
          PROJCS     -> PROJCRS
          VERT_DATUM -> VDATUM
          VERT_CS    -> VERTCRS
          COMPD_CS   -> COMPOUNDCRS

   8.  The ability to output objects in the old (19125) syntax is provided,
       although no checks are done to insure that the created WKT will be
       usable to create an equivalent object.

### Sample programs

Three sample programs are provided:

    parse     This program tests the token parsing of WKT strings.

              Usage: parse [options] [filename]
              Options:
                -?, -help     Display usage
                -V, -version  Display version
                -v            Verbose
              Arguments:
                filename      File of WKT strings to read (default is stdin)

    datetime  This program tests the parsing of date-time strings.

              Usage: datetime [options] [filename]
              Options:
                -?, -help     Display usage
                -V, -version  Display version
                -v            Verbose
                -n ndig       Specify number of digits in seconds
              Arguments:
                filename      File of date-time strings to read (default is stdin)

    wkt       This program will read and write WKT strings.

              Usage: wkt [options] [filename]
              Options:
                -?, -help     Display usage
                -V, -version  Display version
                -m            Process multi-line input
                -n            Show name only
                -o            Display in old syntax
                -i            Don't show any ID attributes
                -t            Show top-level ID attribute only
                -p            Show () instead of [] in strings
                -x            Expand with spaces
                -X            Expand with tabs
                -r            Relax strict parsing
                -s            Set   strict parsing (default)
              Arguments:
                filename      File of WKT strings to read (default is stdin)

### To do

The following is a list of things still to do to fully flesh out this
sample API:

*  The method ogc_utils::validate_cs() needs to be expanded to catch
   more errors with CS, AXIS, and UNIT attributes.

*  The ogc_vector::is_equal() and ogc_vector::is_identical() methods
   could be expanded to take into account the vectors having the same
   objects but in a different order.

### Issues

Find a bug or want to request a new feature?  Please let us know by submitting
an issue.

Comments and issues logged through this Github project will be available to
the ISO and OGC Standards Working Group. However, this Github project is not
an official repository of public comment for the standard - you may want
to submit comments through the formal ISO and OGC channels to ensure that they
are logged and responded to.

### Contributing

Anyone and everyone is welcome to contribute.

### Licensing

Copyright 2010-2015 Esri

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

A copy of the license is available in the repository's
[license.txt](https://raw.github.com/Esri/ogc-crs-wkt-parser/master/license.txt) file.

[](Esri Tags: OGC WKT)
[](Esri Language: C-plus-plus)
