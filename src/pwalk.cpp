//pwalk.cpp      definition of an array of polygons class

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
#include "pwalk.h"


polyWalk::polyWalk(size_t nSides, size_t nPoly, size_t vLim, size_t del, vPoint sizer){
  int lastVx = 0, lastVy = 0;
  size_t i;
  int x;
  xMin = 0; yMin = 0;
  xMax = sizer.x; yMax = sizer.y;
  closeDown = 0;
  vPoint tempvp;
  polyArray = new genArray<polygon>();
  if(!polyArray){
		cerr << "Trouble allocating memory for polyArray in polyWalk constructor.";
		exit(1);
  }
  velArray = new polygon();
	if(!velArray){
		cerr << "Trouble allocating memory for velArray in polyWalk constructor.";
		exit(1);
  }
  currPoly = 0;
  numSides = nSides;
  numPoly = nPoly;
  velLim = vLim;
  delay = del;
  time_t now = time(NULL);
  srand(now);
  for(i = 0; i < numSides; i++){
    //set start points
    x = rand();
    x = x % xMax;
    tempvp.x = x;
    x = rand();
    x = x % yMax;
    tempvp.y = x;
    (*polyArray)[0].append(tempvp); //set random vertices in 0'th polygon 
    //set start velocities
    x = 0;
    while(!x){
      x = rand();
      x = x % (vLim * 2);
      x -= vLim;
      if(i > 0) x += (lastVx + x) % 2; //make diff from prev point % 2 == 0
    }
    tempvp.x = x;
    x = 0;
    while(!x){
      x = rand();
      x = x % (vLim * 2);
      x -= vLim;
      if(i > 0) x += (lastVy + x) % 2;
    }
    tempvp.y = x;
    velArray->append(tempvp);  //set random velocities 
    lastVx = tempvp.x;
    lastVy = tempvp.y;
  }//for(step through sides)
  endVal = numPoly +1;
}//polyWalk constructor  
 
polygon &polyWalk::backPoly(){
  iter = currPoly + 2;
  if(iter >= numPoly){
    iter -= numPoly;
  }
  if(currPoly < numPoly - 1){
    return (*polyArray)[currPoly + 1];
  } else {
    return (*polyArray)[0];
  }
}

polygon &polyWalk::nextPoly(){
  if(iter != currPoly) {
    iter++;                        //increment first to allow testing
    if(iter >= numPoly) iter = 0;  //reset it if too large
  }
  return (*polyArray)[iter];     //we've gotten to currPoly, stay here
}

int polyWalk::step(int theColor){
  size_t i;
  int tempx, tempy, vflag = 0;
  if(endVal){
    backPoly() = frontPoly();
    if(!closeDown){
      backPoly() += *velArray;
      endVal = numPoly;
    }
    for(i = 0; i < numSides; i++){
      tempx = backPoly()[i].x;
      tempy = backPoly()[i].y;
      if((size_t)tempx >= xMax || tempx <= (int)xMin){
	backPoly()[i].x -= (*velArray)[i].x * 2;
	(*velArray)[i].x = - (*velArray)[i].x;
      }
      if((size_t)tempy >= yMax || tempy <= (int)yMin){
	backPoly()[i].y -= (*velArray)[i].y * 2;
	(*velArray)[i].y = - (*velArray)[i].y;
      }
      if(!vflag && ((*velArray)[i].x || (*velArray)[i].y)){  //if any velocity 
	vflag = 1;                         //flag true
      }
    }//for(step through sides of backPoly)
    backPoly()[numSides] = backPoly()[0]; //close the polygon
    if(!closeDown) backPoly().setColor(theColor); //leave color same if closing 
    (*velArray)[numSides] = (*velArray)[0]; // keep this congruent
    if(currPoly < numPoly - 1){
      currPoly++;
    } else {
      currPoly = 0;
    }
    endVal--;
  }//if(endVal)
  return vflag;
}//polyWalk::step()

void polyWalk::setSize(vPoint minSize, vPoint maxSize){
  polygon *tempPoly = &frontPoly();  //working directly with the points
  size_t i;
  int minX = minSize.x,
    minY = minSize.y,
    maxX = maxSize.x,
    maxY = maxSize.y;
  if(minX >= 0 && minY >= 0 && maxX > minX && maxY > minY){ //sanity
    xMin = minX;
    yMin = minY;
    xMax = maxX;
    yMax = maxY;
  }
  size_t n = tempPoly->numSides();
  for(i = 0; i < n; i++){
    if((*tempPoly)[i].x < (int)xMin) (*tempPoly)[i].x = (int)xMin;
    if((*tempPoly)[i].y < (int)yMin) (*tempPoly)[i].y = (int)yMin;
    if((size_t)(*tempPoly)[i].x > xMax) (*tempPoly)[i].x = (int)xMax;
    if((size_t)(*tempPoly)[i].y > yMax) (*tempPoly)[i].y = (int)yMax;
  }
  (*tempPoly)[i].x = (*tempPoly)[0].x;
  (*tempPoly)[i].y = (*tempPoly)[0].y;
}//polyWalk::setSize(vPoint vPoint)

#ifdef POLYTEST
int main(void){
  size_t i, n;
  polygon aPoly;
  size_t a = 3, b = 10;
  vPoint aSize = {100,100};
  polyWalk *polyW;
  polyW = new polyWalk(a, a, b, b, aSize);
  aPoly = (*polyW)[2];
  aPoly += (*polyW)[1];
  cerr << "\nThis is the poly: \n" << aPoly.toString() << endl;
  cerr << "\nThis is the polyWalk:\n" ;
  for(n = 0; n < 3; n++){
    cerr << "polygon " << n << ":\n" << (*polyW)[n].toString() << endl;
  }
  cerr << "\nHere is the front poly:\n " << polyW->frontPoly().toString() << 
    endl << "And here is the back poly:\n" << polyW->backPoly().toString() <<
    endl;
  polyW->step();
  polyW->step();
  polyW->step();
  polyW->step();
  cerr << "\nAfter 4 steps, this is the front poly:\n" << 
    polyW->frontPoly().toString() << endl <<
    "And here is the back poly:\n" << polyW->backPoly().toString() << endl;
  cerr << "\nThe whole polyWalk at this point is:\n";
  for(n = 0; n < 3; n++){
    cerr << "polygon " << n << ":\n" << (*polyW)[n].toString() << endl;
  }
  cerr << "\nThe velocity polygon is:\n" << polyW->getVels().toString() <<
    endl;
}//int main(void)

#endif //POLYTEST





