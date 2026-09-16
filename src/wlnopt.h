//wlnopt.h    Declares a class for storing game options -*-C++-*-

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

#ifndef _WLNOPT_H
#define _WLNOPT_H
#include "wlndefs.h"

//this enumeration allows a fake associative array
enum {DefStartLevel, Speedlim, MaxDelay, NPollies, StepPClr, MaxX, MaxY,
			StartLives, NSaved, NHigh};
#ifndef __WINDOWS__
enum {opSt = 1, opLm = 15, opDl = 100, opNp = 50, opSp = 10, opMx = 1200,
	opMy = 800, opSl = 5, opNs = 15, opNh = numHi, opEn = 0};
#else
enum {opSt = 1, opLm = 15, opDl = 60, opNp = 50, opSp = 10, opMx = 1200,
	opMy = 800, opSl = 5, opNs = 15, opNh = numHi, opEn = 0};
#endif

enum {miSt = 1, miLm = 6, miDl = 51, miNp = 6, miSp = 1, miMx = 200,
	miMy = 200, miSl = 1, miNs = 5, miNh = numHi, miEn = 0};
enum {maSt = 99, maLm = 50, maDl = 600, maNp = 200, maSp = 100, maMx = 1920,
	maMy = 1200, maSl = 12, maNs = 25, maNh = numHi, maEn = 0};

#ifndef __WINDOWS__
enum {dfSt = 1, dfLm = 15, dfDl = 100, dfNp = 50, dfSp = 10, dfMx = 1200,
	dfMy = 800, dfSl = 5, dfNs = 15, dfNh = numHi, dfEn = 0};
#else
enum {dfSt = 1, dfLm = 15, dfDl = 60, dfNp = 50, dfSp = 10, dfMx = 1200,
	dfMy = 800, dfSl = 5, dfNs = 15, dfNh = numHi, dfEn = 0};
#endif

class wlnOpts {
public:
  wlnOpts() {instances++;}
  int &operator[](const int idx) { return theOpts[idx]; }
  static int* getOpts() { return theOpts; }
  static int* getMins() { return theMins; }
  static int* getMaxs() { return theMaxs; }
  static int* getDefs() { return theDefs; }
  static const char** getList() { return theList; }
private:
  static int instances;
  static int theOpts[];
  static int theMins[];
  static int theMaxs[];
  static int theDefs[];
  static const char* theList[];

};

extern wlnOpts optInts;

#endif //_WLNOPT_H
