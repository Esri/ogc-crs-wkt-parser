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
/* tables                                                                    */
/* ------------------------------------------------------------------------- */

#include "ogc_common.h"

namespace OGC {

/* ------------------------------------------------------------------------- */
/* object types                                                              */
/* ------------------------------------------------------------------------- */

const ogc_tables::obj_tbl ogc_tables::ogc_obj_types[] =
{
   { OGC_OBJ_TYPE_CITATION,               OGC_OBJ_KWD_CITATION         },
   { OGC_OBJ_TYPE_URI,                    OGC_OBJ_KWD_URI              },
   { OGC_OBJ_TYPE_ID,                     OGC_OBJ_KWD_ID               },

   { OGC_OBJ_TYPE_REMARK,                 OGC_OBJ_KWD_REMARK           },

   { OGC_OBJ_TYPE_UNIT,                   OGC_OBJ_KWD_UNIT             },
   { OGC_OBJ_TYPE_ANGUNIT,                OGC_OBJ_KWD_ANGUNIT          },
   { OGC_OBJ_TYPE_LENUNIT,                OGC_OBJ_KWD_LENUNIT          },
   { OGC_OBJ_TYPE_SCALEUNIT,              OGC_OBJ_KWD_SCALEUNIT        },
   { OGC_OBJ_TYPE_TIMEUNIT,               OGC_OBJ_KWD_TIMEUNIT         },
   { OGC_OBJ_TYPE_PARAMUNIT,              OGC_OBJ_KWD_PARAMUNIT        },

   { OGC_OBJ_TYPE_SCOPE,                  OGC_OBJ_KWD_SCOPE            },
   { OGC_OBJ_TYPE_AREA_EXTENT,            OGC_OBJ_KWD_AREA_EXTENT      },
   { OGC_OBJ_TYPE_BBOX_EXTENT,            OGC_OBJ_KWD_BBOX_EXTENT      },
   { OGC_OBJ_TYPE_TIME_EXTENT,            OGC_OBJ_KWD_TIME_EXTENT      },
   { OGC_OBJ_TYPE_VERT_EXTENT,            OGC_OBJ_KWD_VERT_EXTENT      },

   { OGC_OBJ_TYPE_PARAMETER,              OGC_OBJ_KWD_PARAMETER        },
   { OGC_OBJ_TYPE_PARAM_FILE,             OGC_OBJ_KWD_PARAM_FILE       },

   { OGC_OBJ_TYPE_ELLIPSOID,              OGC_OBJ_KWD_ELLIPSOID        },

   { OGC_OBJ_TYPE_ANCHOR,                 OGC_OBJ_KWD_ANCHOR           },
   { OGC_OBJ_TYPE_TIME_ORIGIN,            OGC_OBJ_KWD_TIME_ORIGIN      },

   { OGC_OBJ_TYPE_ENGR_DATUM,             OGC_OBJ_KWD_ENGR_DATUM       },
   { OGC_OBJ_TYPE_GEOD_DATUM,             OGC_OBJ_KWD_GEOD_DATUM       },
   { OGC_OBJ_TYPE_IMAGE_DATUM,            OGC_OBJ_KWD_IMAGE_DATUM      },
   { OGC_OBJ_TYPE_PARAM_DATUM,            OGC_OBJ_KWD_PARAM_DATUM      },
   { OGC_OBJ_TYPE_TIME_DATUM,             OGC_OBJ_KWD_TIME_DATUM       },
   { OGC_OBJ_TYPE_VERT_DATUM,             OGC_OBJ_KWD_VERT_DATUM       },

   { OGC_OBJ_TYPE_PRIMEM,                 OGC_OBJ_KWD_PRIMEM           },

   { OGC_OBJ_TYPE_ORDER,                  OGC_OBJ_KWD_ORDER            },
   { OGC_OBJ_TYPE_MERIDIAN,               OGC_OBJ_KWD_MERIDIAN         },
   { OGC_OBJ_TYPE_BEARING,                OGC_OBJ_KWD_BEARING          },
   { OGC_OBJ_TYPE_AXIS,                   OGC_OBJ_KWD_AXIS             },

   { OGC_OBJ_TYPE_CS,                     OGC_OBJ_KWD_CS               },

   { OGC_OBJ_TYPE_CONVERSION,             OGC_OBJ_KWD_CONVERSION       },
   { OGC_OBJ_TYPE_DERIVING_CONV,          OGC_OBJ_KWD_DERIVING_CONV    },
   { OGC_OBJ_TYPE_METHOD,                 OGC_OBJ_KWD_METHOD           },

   { OGC_OBJ_TYPE_ENGR_CRS,               OGC_OBJ_KWD_ENGR_CRS         },
   { OGC_OBJ_TYPE_GEOD_CRS,               OGC_OBJ_KWD_GEOD_CRS         },
   { OGC_OBJ_TYPE_IMAGE_CRS,              OGC_OBJ_KWD_IMAGE_CRS        },
   { OGC_OBJ_TYPE_PARAM_CRS,              OGC_OBJ_KWD_PARAM_CRS        },
   { OGC_OBJ_TYPE_PROJ_CRS,               OGC_OBJ_KWD_PROJ_CRS         },
   { OGC_OBJ_TYPE_TIME_CRS,               OGC_OBJ_KWD_TIME_CRS         },
   { OGC_OBJ_TYPE_VERT_CRS,               OGC_OBJ_KWD_VERT_CRS         },
   { OGC_OBJ_TYPE_COMPOUND_CRS,           OGC_OBJ_KWD_COMPOUND_CRS     },

   { OGC_OBJ_TYPE_BASE_ENGR_CRS,          OGC_OBJ_KWD_BASE_ENGR_CRS    },
   { OGC_OBJ_TYPE_BASE_GEOD_CRS,          OGC_OBJ_KWD_BASE_GEOD_CRS    },
   { OGC_OBJ_TYPE_BASE_PARAM_CRS,         OGC_OBJ_KWD_BASE_PARAM_CRS   },
   { OGC_OBJ_TYPE_BASE_PROJ_CRS,          OGC_OBJ_KWD_BASE_PROJ_CRS    },
   { OGC_OBJ_TYPE_BASE_TIME_CRS,          OGC_OBJ_KWD_BASE_TIME_CRS    },
   { OGC_OBJ_TYPE_BASE_VERT_CRS,          OGC_OBJ_KWD_BASE_VERT_CRS    },

   { OGC_OBJ_TYPE_OP_ACCURACY,            OGC_OBJ_KWD_OP_ACCURACY      },
   { OGC_OBJ_TYPE_COORD_OP,               OGC_OBJ_KWD_COORD_OP         },

   { OGC_OBJ_TYPE_UNKNOWN,                OGC_OBJ_KWD_UNKNOWN          }
};

/* ------------------------------------------------------------------------- */
/* coordinate reference system types                                         */
/* ------------------------------------------------------------------------- */

const ogc_tables::crs_tbl ogc_tables::ogc_crs_types[] =
{
   { OGC_CRS_TYPE_ENGR,                   OGC_CRS_KWD_ENGR             },
   { OGC_CRS_TYPE_PROJ,                   OGC_CRS_KWD_PROJ             },
   { OGC_CRS_TYPE_VERT,                   OGC_CRS_KWD_VERT             },
   { OGC_CRS_TYPE_GEOD,                   OGC_CRS_KWD_GEOD             },
   { OGC_CRS_TYPE_IMAGE,                  OGC_CRS_KWD_IMAGE            },
   { OGC_CRS_TYPE_PARAM,                  OGC_CRS_KWD_PARAM            },
   { OGC_CRS_TYPE_TIME,                   OGC_CRS_KWD_TIME             },

   { OGC_CRS_TYPE_ENGR_DERIVED,           OGC_CRS_KWD_ENGR_DERIVED,    },
   { OGC_CRS_TYPE_VERT_DERIVED,           OGC_CRS_KWD_VERT_DERIVED,    },
   { OGC_CRS_TYPE_GEOD_DERIVED,           OGC_CRS_KWD_GEOD_DERIVED,    },
   { OGC_CRS_TYPE_PARAM_DERIVED,          OGC_CRS_KWD_PARAM_DERIVED,   },
   { OGC_CRS_TYPE_TIME_DERIVED,           OGC_CRS_KWD_TIME_DERIVED,    },

   { OGC_CRS_TYPE_COMPOUND,               OGC_CRS_KWD_COMPOUND         },

   { OGC_CRS_TYPE_UNKNOWN,                OGC_CRS_KWD_UNKNOWN          }
};

/* ------------------------------------------------------------------------- */
/* unit types                                                                */
/* ------------------------------------------------------------------------- */

const ogc_tables::unit_tbl ogc_tables::ogc_unit_types[] =
{
   { OGC_UNIT_TYPE_ANGLE,                 OGC_UNIT_KWD_ANGLE           },
   { OGC_UNIT_TYPE_LENGTH,                OGC_UNIT_KWD_LENGTH          },
   { OGC_UNIT_TYPE_PARAM,                 OGC_UNIT_KWD_PARAM           },
   { OGC_UNIT_TYPE_SCALE,                 OGC_UNIT_KWD_SCALE           },
   { OGC_UNIT_TYPE_TIME,                  OGC_UNIT_KWD_TIME            },
   { OGC_UNIT_TYPE_GENERIC,               OGC_UNIT_KWD_GENERIC         },

   { OGC_UNIT_TYPE_UNKNOWN,               OGC_UNIT_KWD_UNKNOWN         }
};

/* ------------------------------------------------------------------------- */
/* datum types                                                               */
/* ------------------------------------------------------------------------- */

const ogc_tables::datum_tbl ogc_tables::ogc_datum_types[] =
{
   { OGC_DATUM_TYPE_ENGR,                 OGC_DATUM_KWD_ENGR           },
   { OGC_DATUM_TYPE_GEOD,                 OGC_DATUM_KWD_GEOD           },
   { OGC_DATUM_TYPE_IMAGE,                OGC_DATUM_KWD_IMAGE          },
   { OGC_DATUM_TYPE_PARAM,                OGC_DATUM_KWD_PARAM          },
   { OGC_DATUM_TYPE_TIME,                 OGC_DATUM_KWD_TIME           },
   { OGC_DATUM_TYPE_VERT,                 OGC_DATUM_KWD_VERT           },

   { OGC_DATUM_TYPE_UNKNOWN,              OGC_DATUM_KWD_UNKNOWN        }
};

/* ------------------------------------------------------------------------- */
/* coordinate system types                                                   */
/* ------------------------------------------------------------------------- */

const ogc_tables::cs_tbl ogc_tables::ogc_cs_types[] =
{
   { OGC_CS_TYPE_AFFINE,                  OGC_CS_KWD_AFFINE            },
   { OGC_CS_TYPE_CARTESIAN,               OGC_CS_KWD_CARTESIAN         },
   { OGC_CS_TYPE_CYLINDRICAL,             OGC_CS_KWD_CYLINDRICAL       },
   { OGC_CS_TYPE_ELLIPSOIDAL,             OGC_CS_KWD_ELLIPSOIDAL       },
   { OGC_CS_TYPE_LINEAR,                  OGC_CS_KWD_LINEAR            },
   { OGC_CS_TYPE_PARAMETRIC,              OGC_CS_KWD_PARAMETRIC        },
   { OGC_CS_TYPE_POLAR,                   OGC_CS_KWD_POLAR             },
   { OGC_CS_TYPE_SPHERICAL,               OGC_CS_KWD_SPHERICAL         },
   { OGC_CS_TYPE_TEMPORAL,                OGC_CS_KWD_TEMPORAL          },
   { OGC_CS_TYPE_VERTICAL,                OGC_CS_KWD_VERTICAL          },

   { OGC_CS_TYPE_UNKNOWN,                 OGC_CS_KWD_UNKNOWN           }
};

/* ------------------------------------------------------------------------- */
/* axis direction types                                                      */
/* ------------------------------------------------------------------------- */

const ogc_tables::axis_tbl ogc_tables::ogc_axis_types[] =
{
   { OGC_AXIS_DIR_TYPE_NORTH,             OGC_AXIS_DIR_KWD_NORTH             },
   { OGC_AXIS_DIR_TYPE_NORTH_NORTH_EAST,  OGC_AXIS_DIR_KWD_NORTH_NORTH_EAST  },
   { OGC_AXIS_DIR_TYPE_NORTH_EAST,        OGC_AXIS_DIR_KWD_NORTH_EAST        },
   { OGC_AXIS_DIR_TYPE_EAST_NORTH_EAST,   OGC_AXIS_DIR_KWD_EAST_NORTH_EAST   },
   { OGC_AXIS_DIR_TYPE_EAST,              OGC_AXIS_DIR_KWD_EAST              },
   { OGC_AXIS_DIR_TYPE_EAST_SOUTH_EAST,   OGC_AXIS_DIR_KWD_EAST_SOUTH_EAST   },
   { OGC_AXIS_DIR_TYPE_SOUTH_EAST,        OGC_AXIS_DIR_KWD_SOUTH_EAST        },
   { OGC_AXIS_DIR_TYPE_SOUTH_SOUTH_EAST,  OGC_AXIS_DIR_KWD_SOUTH_SOUTH_EAST  },
   { OGC_AXIS_DIR_TYPE_SOUTH,             OGC_AXIS_DIR_KWD_SOUTH             },
   { OGC_AXIS_DIR_TYPE_SOUTH_WEST,        OGC_AXIS_DIR_KWD_SOUTH_WEST        },
   { OGC_AXIS_DIR_TYPE_WEST_SOUTH_WEST,   OGC_AXIS_DIR_KWD_WEST_SOUTH_WEST   },
   { OGC_AXIS_DIR_TYPE_WEST,              OGC_AXIS_DIR_KWD_WEST              },
   { OGC_AXIS_DIR_TYPE_WEST_NORTH_WEST,   OGC_AXIS_DIR_KWD_WEST_NORTH_WEST   },
   { OGC_AXIS_DIR_TYPE_NORTH_WEST,        OGC_AXIS_DIR_KWD_NORTH_WEST        },
   { OGC_AXIS_DIR_TYPE_NORTH_NORTH_WEST,  OGC_AXIS_DIR_KWD_NORTH_NORTH_WEST  },

   { OGC_AXIS_DIR_TYPE_GEOCENTRIC_X,      OGC_AXIS_DIR_KWD_GEOCENTRIC_X      },
   { OGC_AXIS_DIR_TYPE_GEOCENTRIC_Y,      OGC_AXIS_DIR_KWD_GEOCENTRIC_Y      },
   { OGC_AXIS_DIR_TYPE_GEOCENTRIC_Z,      OGC_AXIS_DIR_KWD_GEOCENTRIC_Z      },

   { OGC_AXIS_DIR_TYPE_UP,                OGC_AXIS_DIR_KWD_UP                },
   { OGC_AXIS_DIR_TYPE_DOWN,              OGC_AXIS_DIR_KWD_DOWN              },

   { OGC_AXIS_DIR_TYPE_FORWARD,           OGC_AXIS_DIR_KWD_FORWARD           },
   { OGC_AXIS_DIR_TYPE_AFT,               OGC_AXIS_DIR_KWD_AFT               },
   { OGC_AXIS_DIR_TYPE_STARBOARD,         OGC_AXIS_DIR_KWD_STARBOARD         },
   { OGC_AXIS_DIR_TYPE_PORT,              OGC_AXIS_DIR_KWD_PORT              },

   { OGC_AXIS_DIR_TYPE_CLOCKWISE,         OGC_AXIS_DIR_KWD_CLOCKWISE         },
   { OGC_AXIS_DIR_TYPE_COUNTER_CLOCKWISE, OGC_AXIS_DIR_KWD_COUNTER_CLOCKWISE },

   { OGC_AXIS_DIR_TYPE_COLUMN_POSITIVE,   OGC_AXIS_DIR_KWD_COLUMN_POSITIVE   },
   { OGC_AXIS_DIR_TYPE_COLUMN_NEGATIVE,   OGC_AXIS_DIR_KWD_COLUMN_NEGATIVE   },
   { OGC_AXIS_DIR_TYPE_ROW_POSITIVE,      OGC_AXIS_DIR_KWD_ROW_POSITIVE      },
   { OGC_AXIS_DIR_TYPE_ROW_NEGATIVE,      OGC_AXIS_DIR_KWD_ROW_NEGATIVE      },

   { OGC_AXIS_DIR_TYPE_DISPLAY_RIGHT,     OGC_AXIS_DIR_KWD_DISPLAY_RIGHT     },
   { OGC_AXIS_DIR_TYPE_DISPLAY_LEFT,      OGC_AXIS_DIR_KWD_DISPLAY_LEFT      },
   { OGC_AXIS_DIR_TYPE_DISPLAY_UP,        OGC_AXIS_DIR_KWD_DISPLAY_UP        },
   { OGC_AXIS_DIR_TYPE_DISPLAY_DOWN,      OGC_AXIS_DIR_KWD_DISPLAY_DOWN      },

   { OGC_AXIS_DIR_TYPE_FUTURE,            OGC_AXIS_DIR_KWD_FUTURE            },
   { OGC_AXIS_DIR_TYPE_PAST,              OGC_AXIS_DIR_KWD_PAST              },

   { OGC_AXIS_DIR_TYPE_TOWARDS,           OGC_AXIS_DIR_KWD_TOWARDS           },
   { OGC_AXIS_DIR_TYPE_AWAY_FROM,         OGC_AXIS_DIR_KWD_AWAY_FROM         },

   { OGC_AXIS_DIR_TYPE_UNSPECIFIED,       OGC_AXIS_DIR_KWD_UNSPECIFIED       },

   { OGC_AXIS_DIR_TYPE_UNKNOWN,           OGC_AXIS_DIR_KWD_UNKNOWN           }
};

/* ------------------------------------------------------------------------- */
/* pixel-in-cell types                                                       */
/* ------------------------------------------------------------------------- */

const ogc_tables::pixel_tbl ogc_tables::ogc_pixel_types[] =
{
   { OGC_PIXEL_TYPE_CENTER,              OGC_PIXEL_KWD_CENTER                },
   { OGC_PIXEL_TYPE_CENTRE,              OGC_PIXEL_KWD_CENTRE                },
   { OGC_PIXEL_TYPE_CORNER,              OGC_PIXEL_KWD_CORNER                },

   { OGC_PIXEL_TYPE_UNKNOWN,             OGC_PIXEL_KWD_UNKNOWN               }
};

/* ------------------------------------------------------------------------- */
/* error codes                                                               */
/* ------------------------------------------------------------------------- */

const ogc_tables::error_tbl ogc_tables :: ogc_errors[] =
{
   /* general errors */

   { OGC_ERR_NONE,                          "no error"                 },
   { OGC_ERR_NO_MEMORY,                     "no memory"                },

   /* WKT syntax errors */

   { OGC_ERR_WKT_EMPTY_STRING,              "empty string"             },
   { OGC_ERR_WKT_EXPECTING_TOKEN,           "expecting token"          },
   { OGC_ERR_WKT_INSUFFICIENT_TOKENS,       "insufficient tokens"      },
   { OGC_ERR_WKT_INVALID_KEYWORD,           "invalid keyword"          },
   { OGC_ERR_WKT_INVALID_ESCAPE,            "invalid escape"           },
   { OGC_ERR_WKT_INVALID_SYNTAX,            "invalid syntax"           },
   { OGC_ERR_WKT_MAX_TOKENS_EXCEEDED,       "max tokens exceeded"      },
   { OGC_ERR_WKT_MISSING_TOKEN,             "missing token"            },
   { OGC_ERR_WKT_TOO_LONG,                  "too long"                 },
   { OGC_ERR_WKT_UNBALANCED_QUOTES,         "unbalanced quotes"        },
   { OGC_ERR_WKT_TOO_MANY_OPEN_TOKENS,      "too many open tokens"     },
   { OGC_ERR_WKT_TOO_MANY_CLOSE_TOKENS,     "too many close tokens"    },
   { OGC_ERR_WKT_UNKNOWN_KEYWORD,           "unknown keyword"          },

   /* WKT duplicate object errors */

   { OGC_ERR_WKT_DUPLICATE_ABRTRANS,        "duplicate abrtrans"       },
   { OGC_ERR_WKT_DUPLICATE_ANCHOR,          "duplicate anchor"         },
   { OGC_ERR_WKT_DUPLICATE_BASECRS,         "duplicate basecrs"        },
   { OGC_ERR_WKT_DUPLICATE_BEARING,         "duplicate bearing"        },
   { OGC_ERR_WKT_DUPLICATE_CONVERSION,      "duplicate conversion"     },
   { OGC_ERR_WKT_DUPLICATE_CS,              "duplicate cs"             },
   { OGC_ERR_WKT_DUPLICATE_DATUM,           "duplicate datum"          },
   { OGC_ERR_WKT_DUPLICATE_CITATION,        "duplicate citation"       },
   { OGC_ERR_WKT_DUPLICATE_ELLIPSOID,       "duplicate ellipsoid"      },
   { OGC_ERR_WKT_DUPLICATE_EXTENT,          "duplicate extent"         },
   { OGC_ERR_WKT_DUPLICATE_FIRST_CRS,       "duplicate first crs"      },
   { OGC_ERR_WKT_DUPLICATE_ID,              "duplicate id"             },
   { OGC_ERR_WKT_DUPLICATE_INTERP_CRS,      "duplicate interp crs"     },
   { OGC_ERR_WKT_DUPLICATE_MERIDIAN,        "duplicate meridian"       },
   { OGC_ERR_WKT_DUPLICATE_METHOD,          "duplicate method"         },
   { OGC_ERR_WKT_DUPLICATE_PARAMETER,       "duplicate parameter"      },
   { OGC_ERR_WKT_DUPLICATE_PARAM_FILE,      "duplicate parameter file" },
   { OGC_ERR_WKT_DUPLICATE_OPACCURACY,      "duplicate opaccuracy"     },
   { OGC_ERR_WKT_DUPLICATE_ORDER,           "duplicate order"          },
   { OGC_ERR_WKT_DUPLICATE_PRIMEM,          "duplicate primem"         },
   { OGC_ERR_WKT_DUPLICATE_REMARK,          "duplicate remark"         },
   { OGC_ERR_WKT_DUPLICATE_SCOPE,           "duplicate scope"          },
   { OGC_ERR_WKT_DUPLICATE_SECOND_CRS,      "duplicate second crs"     },
   { OGC_ERR_WKT_DUPLICATE_SOURCE_CRS,      "duplicate source crs"     },
   { OGC_ERR_WKT_DUPLICATE_TARGET_CRS,      "duplicate target crs"     },
   { OGC_ERR_WKT_DUPLICATE_TIME_CRS,        "duplicate time crs"       },
   { OGC_ERR_WKT_DUPLICATE_UNIT,            "duplicate unit"           },
   { OGC_ERR_WKT_DUPLICATE_URI,             "duplicate uri"            },

   /* WKT axis errors */

   { OGC_ERR_WKT_TOO_MANY_AXES,             "too many axes"            },
   { OGC_ERR_WKT_ORDER_OUT_OF_RANGE,        "order out of range"       },
   { OGC_ERR_WKT_AXIS_ALREADY_USED,         "axis already used"        },

   /* other WKT errors */

   { OGC_ERR_WKT_TOO_MANY_TOKENS,           "too many tokens"          },
   { OGC_ERR_WKT_INDEX_OUT_OF_RANGE,        "index out of range"       },

   /* missing object errors */

   { OGC_ERR_MISSING_ABRTRANS,              "missing abrtrans"         },
   { OGC_ERR_MISSING_BASE_CRS,              "missing base crs"         },
   { OGC_ERR_MISSING_CONVERSION,            "missing conversion"       },
   { OGC_ERR_MISSING_CS,                    "missing cs"               },
   { OGC_ERR_MISSING_DATUM,                 "missing datum"            },
   { OGC_ERR_MISSING_ELLIPSOID,             "missing ellipsoid"        },
   { OGC_ERR_MISSING_FIRST_CRS,             "missing first crs"        },
   { OGC_ERR_MISSING_METHOD,                "missing method"           },
   { OGC_ERR_MISSING_SECOND_CRS,            "missing second crs"       },
   { OGC_ERR_MISSING_SOURCE_CRS,            "missing source crs"       },
   { OGC_ERR_MISSING_TARGET_CRS,            "missing target crs"       },
   { OGC_ERR_MISSING_UNIT,                  "missing unit"             },

   /* invalid value errors */

   { OGC_ERR_INVALID_AXIS_DIRECTION,        "invalid axis direction"   },
   { OGC_ERR_INVALID_BEARING_VALUE,         "invalid bearing value"    },
   { OGC_ERR_INVALID_CS_TYPE,               "invalid cs type"          },
   { OGC_ERR_INVALID_DIMENSION,             "invalid dimension"        },
   { OGC_ERR_INVALID_FIRST_CRS,             "invalid first crs"        },
   { OGC_ERR_INVALID_FLATTENING,            "invalid flattening"       },
   { OGC_ERR_INVALID_LATITUDE,              "invalid latitude"         },
   { OGC_ERR_INVALID_LONGITUDE,             "invalid longitude"        },
   { OGC_ERR_INVALID_MERIDIAN_VALUE,        "invalid meridian value"   },
   { OGC_ERR_INVALID_ORDER_VALUE,           "invalid order value"      },
   { OGC_ERR_INVALID_SECOND_CRS,            "invalid second crs"       },
   { OGC_ERR_INVALID_SEMIMAJOR_AXIS,        "invalid semimajor axis"   },
   { OGC_ERR_INVALID_THIRD_CRS,             "invalid third crs"        },
   { OGC_ERR_INVALID_UNIT_FACTOR,           "invalid unit factor"      },
   { OGC_ERR_INVALID_PIXEL_TYPE,            "invalid pixel type"       },

   /* other errors */

   { OGC_ERR_EMPTY_FILENAME,                "empty filename"           },
   { OGC_ERR_EMPTY_NAME,                    "empty name"               },

   { OGC_ERR_MISMATCHED_UNITS,              "mismatched units"         },

   { OGC_ERR_ABBR_TOO_LONG,                 "abbr too long"            },
   { OGC_ERR_FILENAME_TOO_LONG,             "filename too long"        },
   { OGC_ERR_IDENTIFIER_TOO_LONG,           "identifier too long"      },
   { OGC_ERR_NAME_TOO_LONG,                 "name too long"            },
   { OGC_ERR_TEXT_TOO_LONG,                 "text too long"            },
   { OGC_ERR_TIME_TOO_LONG,                 "time too long"            },
   { OGC_ERR_VERSION_TOO_LONG,              "version too long"         },

   { OGC_ERR_LAST,                          OGC_NULL                   }
};

} /* namespace OGC */
