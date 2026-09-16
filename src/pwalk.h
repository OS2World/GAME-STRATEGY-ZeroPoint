//pwalk.h    declaration of a class which represents a vector of -*-C++-*- 
//           polygons as defined in poly.h.

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


#ifndef PWALK_H
#define PWALK_H

#include "poly.h"
#include "genArray.h"


class polyWalk {
private:
  genArray<polygon> *polyArray;
  polygon *velArray;
  size_t currPoly, iter;  //lines per color
  size_t numSides, numPoly, velLim, delay, endVal;
  size_t xMin, yMin, xMax, yMax;
  int closeDown;  //flag for ending

public:
  polyWalk(size_t nSides, size_t nPoly, size_t vLim, size_t del, vPoint sizer);
  ~polyWalk(){if(polyArray) delete polyArray; if(velArray) delete velArray;}
  polygon &frontPoly() {return (*polyArray)[currPoly];}
  polygon &backPoly();
  polygon &nextPoly(); //for iterating from backPoly to frontPoly
  int iterDone() { if(iter == currPoly) {return 1;} else {return 0;}}
  //void normalize() {} //checks to make sure all velocities sum to %2 == 0
  int step(int theColor);  //returns non-zero if any vel is non-zero
  size_t tellNumSides(){return numSides;}
  //  void setNumSides(size_t newNum);
  polygon &getVels() {return *velArray;}
  //void setVels(polygon newVels);
  size_t tellNumPoly() {return numPoly;}
  size_t tellCurrPoly() {return currPoly;}
  void setSize(vPoint minSize, vPoint maxSize);
	vPoint tellSize() {vPoint aSize = {(int)xMax, (int)yMax}; return aSize;}
	size_t tellDel() { return delay; }
  void setDel(size_t del) { if (delay > 0) delay = del; }
  int closing(int newValue = -1) {  //with no parameter, returns curr. sett'g
    if(newValue != -1) closeDown = newValue;
    return closeDown;
  }
  size_t tellVelLim() {return velLim;}
  size_t tellDelay() {return delay;}
  int allDone() {return endVal <= 0; }
  void restart() {endVal = numPoly;}
};

#endif //PWALK_H

