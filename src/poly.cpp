//poly.cpp      definition of a polygon class

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


#include <time.h>
#include <sstream>
#include "poly.h"

polygon::polygon(){
  pointArray = new genArray<vPoint>();
  if(!pointArray){
		cerr << "Trouble allocating memory in polygon constructor.";
		exit(1);
	}
	myColor = 0;
}

polygon::polygon(polygon &pg){
	size_t i, n;
	pointArray = new genArray<vPoint>();
	if(!pointArray){
		cerr << "Trouble allocating memory in polygon copy constructor.";
		exit(1);
	}
	n = pg.numSides();
	for(i = 0; i < n; i++){
		(*pointArray)[i] = (*pg.pointArray)[i];
	}
}//polygon copy constructor

polygon &polygon::xadd(int aDelta){
	size_t i, n;
	n = pointArray->tellSize();
	for(i = 0; i < n; i++){
		(*pointArray)[i].x += aDelta;
	}
	return *this;
}

polygon &polygon::yadd(int aDelta){
	size_t i, n;
	n = pointArray->tellSize();
	for(i = 0; i < n; i++){
		(*pointArray)[i].y += aDelta;
	}
	return *this;
}

polygon &polygon::operator=(polygon &pg){
	size_t i, n;
	n = pg.pointArray->tellSize();
	pointArray->setSize(n);
	for(i = 0; i < n; i++){
		(*pointArray)[i] = pg[i];
	}
	myColor = pg.myColor;
	return *this;
}//polygon::operator=

polygon &polygon::operator+=(polygon &pg){
	size_t i, n;
	n = pointArray->tellSize();
	i = pg.pointArray->tellSize();
	if(n > i){
		n = i;
	}
	for(i = 0; i < n; i++){
		(*pointArray)[i].x += (*pg.pointArray)[i].x;
		(*pointArray)[i].y += (*pg.pointArray)[i].y;
	}
	return *this;
}//polygon::operator+=

polygon &polygon::append(vPoint newVert){
	size_t n;
	n = pointArray->tellSize();
	if(n > 1){   //since at 0 we add an extra, never a case of n == 1
		(*pointArray)[n-1] = newVert;
		(*pointArray)[n] = (*pointArray)[0];
	} else {
		(*pointArray)[0] = newVert;
		(*pointArray)[1] = newVert;
	}
	//cerr << "\nIn .append, Polygon is now: " << toString();
	return *this;
}//polygon::append()

polygon &polygon::yank(){
	size_t n;
	n = pointArray->tellSize();
	if(n > 1){
		(*pointArray)[n-2] = (*pointArray)[n-1];
		pointArray->setSize(n - 1);
	} else {
		if(n == 1) {
			pointArray->setSize(0);
		}
	}
	return *this;
}//polygon::yank()

char *polygon::toString(){
	std::ostringstream output;
	size_t i, n;
	n = pointArray->tellSize();
	for(i = 0; i < n; i++){
		output << (*pointArray)[i].x << "," << (*pointArray)[i].y << " ";
	}
	static std::string result;
	result = output.str();
	return const_cast<char*>(result.c_str());
}

