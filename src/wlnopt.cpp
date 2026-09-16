//wlnopt.cpp   Defines a class for storing game options

////////////////////////////////////////////////////////////////////////////
// This file is a module in the program Zero Point, a video game program  //
// for Linux X11 and Windows.                                             //
//                                                                        //
// Copyright (C) 1996, 1997, 1998 by Enumerate Inc.                       //
//                                                                        //
// Enumerate Inc. can be contacted at enumerate@rocketmail.com            //
//                                                                        //
// This file is licensed within the terms of the GNU General Public       //
// License version 2 or later. This source code may freely redistributed  //
// or modified within the terms of that license, as described in the file //
// 'COPYING' which should be included with any redistribution of this     //
// file. If the file COPYING is not available, a copy of the General      //
// Public License can be obtained from the Free Software Foundation, Inc. //
// 675 Mass Ave., Cambridge, MA 02139.                                    //
//                                                                        //
// THIS PROGRAM IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER  //
// EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED       //
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.    //
// THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS    //
// WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF  //
// ALL NECESSARY SERVICING, REPAIR OR CORRECTION.                         //
//////////////////////////////////////////////////////////////////////////// 


#include <stdlib.h>
#include "wlnopt.h"

int wlnOpts::instances;

int wlnOpts::theOpts[] = {opSt, opLm, opDl, opNp, opSp, opMx, opMy,
	opSl, opNs, opNh, opEn};
int wlnOpts::theMins[] = {miSt, miLm, miDl, miNp, miSp, miMx, miMy,
	miSl, miNs, miNh, miEn};
int wlnOpts::theMaxs[] = {maSt, maLm, maDl, maNp, maSp, maMx, maMy,
	maSl, maNs, maNh, maEn};
int wlnOpts::theDefs[] = {dfSt, dfLm, dfDl, dfNp, dfSp, dfMx, dfMy,
	dfSl, dfNs, dfNh, dfEn};
const char* wlnOpts::theList[] = {"DefStartLevel", "Speedlim", "MaxDelay",
			    "NPollies", "StepPClr", "MaxX", "MaxY",
			    "StartLives", "NSaved", "NHigh", NULL};

wlnOpts optInts;
