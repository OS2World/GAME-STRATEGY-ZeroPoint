//	wlncnv.h:	Header for wlnCanvasPane class -*-C++-*-
//=======================================================================

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


#ifndef wlnCNV_H
#define wlnCNV_H

#include <v/vcanvas.h>
#include <v/vcolor.h>
#include <v/vapp.h>     // theApp
#include "poly.h"
#include "pwalk.h"
#include "wlndefs.h"

class wlnCanvasPane : public vCanvasPane
{
public:		//---------------------------------------- public
  wlnCanvasPane();
  virtual ~wlnCanvasPane();
  
  // Scrolling
  virtual void HPage(int shown, int top);
  virtual void VPage(int shown, int top);
  
  virtual void HScroll(int step);
  virtual void VScroll(int step);
  
  // Events
  virtual void MouseDown(int x, int y, int button);
  virtual void MouseUp(int x, int y, int button);
  virtual void MouseMove(int x, int y, int button);
  
  virtual void Redraw(int x, int y, int width, int height);
  virtual void Resize(int newW, int newH);
  
  void drawWlnPoly(polygon &pg);

protected:	//--------------------------------------- protected
  
private:		//--------------------------------------- private

  vColor *wlnStdColors;
};
#endif


