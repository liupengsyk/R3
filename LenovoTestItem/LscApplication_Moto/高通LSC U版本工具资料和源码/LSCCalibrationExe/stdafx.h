// Copyright (c) 2014 Qualcomm Technologies, Inc.  All Rights Reserved.
// Qualcomm Technologies Proprietary and Confidential.

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#define RGGB_PATTERN	0
#define GRBG_PATTERN	1
#define BGGR_PATTERN	2
#define GBRG_PATTERN	3

typedef struct CalibrationDataStruct
{
	unsigned short int * R_LSC;
	unsigned short int * Gr_LSC;
	unsigned short int * Gb_LSC;
	unsigned short int * B_LSC;
	unsigned short int * AWB;
} CalibrationDataStruct;

// TODO: reference additional headers your program requires here
