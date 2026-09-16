//genArray.h  A template-based generic array of T's that can grow  -*-C++-*-
//            if needed

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


#ifndef __GENARRAY__
#define __GENARRAY__

#include <iostream>
#include <stdlib.h>
using std::cerr;

template < class T >
class genArray {
protected:
  T * theArray;
  size_t currSize;
  size_t currStorage;
  size_t growRate;
public:
  genArray (size_t growth = 256) {
    theArray = new T[growth];
    if (theArray == NULL) {
      cerr << "Unable to allocate memory in genArray constructor\n";
      exit (1);
    }
    growRate = growth;
    currSize = 0;
    currStorage = growth;
  }

  virtual ~ genArray () {
    delete[]theArray;
  }

  size_t tellSize () const {
    return currSize;
  } 

  void setSize (size_t newSize) {	//throws away items after newSize
    if (newSize > currSize) {
      operator[](newSize - 1);
    } else {
      currSize = newSize;
    }
  }
  
  genArray & operator=(genArray & gA) {
    for (size_t i = 0; i < gA.currSize; i++) {
      operator[](i) = gA[i];
    }
    return *this;
  }

  T *getArray() {
    return theArray;
  } 
  
  size_t append(T newT) {	//adds new T at end and returns its index
    operator[] (currSize) = newT;
    return currSize - 1;
  } 

  T & operator[](const size_t anIndex) {
    if (anIndex < currSize) {	//is the index within the current array?
      return theArray[anIndex];
    } else {			//not in the current array
      if (anIndex < currStorage - 1) {	//is it in the current storage?
	currSize = anIndex + 1;
	return theArray[anIndex];
      } else {			//not in the current storage--allocate more 
	if (anIndex + 1 < currStorage + growRate) {
	  currStorage += growRate;
	} else {
	  currStorage = anIndex + 1;
	}
	T *temp = new T[currStorage];
	if (temp == NULL) {
	  cerr << "\nUnable to allocate memory in genArray::operator[]().\n";
	  exit (1);
	}
	for (size_t i = 0; i < currSize; i++) {
	  temp[i] = theArray[i];
	}
	delete[]theArray;
	theArray = temp;
	currSize = anIndex + 1;
	return theArray[anIndex];
      }	//else (index not in the current storage--allocate more space
    }//else (index not in the current array)
  }//T &genArray::operator[](size_t anIndex)
};//class genArray

#endif //__GENARRAY__
