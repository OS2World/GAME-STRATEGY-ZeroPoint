//poly.h    declaration of a class which holds a vector of points -*-C++-*- 
//          using the vpoint structure defined in the v gui framework

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


#ifndef POLY_H
#define POLY_H

// vPoint replaces the V GUI library struct
struct vPoint { int x; int y; };

#include "genArray.h"
#include "wlndefs.h"

//enum {nSC = 16}; //number of Standard Colors in v

class polygon {
private:
  genArray<vPoint> *pointArray;
  int myColor; //index into wlnStdColors array
 
public:
  polygon(); //no points at start--use append(vPoint) to set up polygon
  polygon(polygon &pg);
  ~polygon() {if(pointArray) delete pointArray;}
  polygon &xadd(int aDelta);
  polygon &yadd(int aDelta);
  polygon &operator=(polygon &pg);
  polygon &operator+=(polygon &pg);
  vPoint &operator[](size_t ind) { return (*pointArray)[ind]; }
  polygon &append(vPoint newVert); //puts a point at the end of the polygon
  polygon &yank(); //takes out last point in polygon
  size_t numSides() {return pointArray->tellSize() - 1;} //last point == first
  vPoint *getPoints() {return &(*pointArray)[0];}
  int getColor() {return myColor;}
  int setColor(int clr) { myColor = clr % nSC; return myColor; }
  char *toString();
};


#endif //POLY_H

