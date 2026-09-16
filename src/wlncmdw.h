//	wlncmdw.h:	Header for wlncmdw class -*-C++-*-
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


#ifndef wlnCMDW_H
#define wlnCMDW_H

#include <v/vcmdwin.h>	// So we can use vCmdWindow
#include <v/vmenu.h>	// For the menu pane
#include <v/vutil.h>	// For V Utilities
#include <v/vstatusp.h>	// For the status pane

#ifdef vDEBUG
#include <v/vdebug.h>
#endif

#include "wlncnv.h"	// wlnCanvasPane
#include "wlnmdlg.h"	// wlnModalDialog
#include "share.h"
#include "wDemmdlg.h"   //demo dialog
#include "wlhimdlg.h"   //hi score dialog
#include "optmdlg.h"     //dialog to set options

                                                              
  class wlnCmdWindow : public vCmdWindow
      {
	friend int AppMain(int, char**);	// allow AppMain access

      public:		//---------------------------------------- public
	wlnCmdWindow(char*, int width, int height);
	virtual ~wlnCmdWindow();
	virtual void WindowCommand(ItemVal id, ItemVal val, CmdType cType);
	virtual void KeyIn(vKey keysym, unsigned int shift);
	wlnCanvasPane *getCanvas() {return wlnCanvas;}
	int showShare(char* msg) {return wlnShareDlg->wlnAction(msg);}
	int showHi(); 
	void runTheDemo();
	int actMDlg(char* msg) { return wlnMDlg->wlnAction(msg); }
	//	int setLevel(int level) { return ((wlnApp*)theApp)->setLevel(level);}

      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private

	// Standard elements
	vMenuPane* wlnMenu;		// For the menu bar
	wlnCanvasPane* wlnCanvas;		// For the canvas
	vStatusPane* wlnStatus;		// For the status bar

	// Dialogs associated with CmdWindow

	wlnModalDialog* wlnMDlg;
	wlnShare* wlnShareDlg;
	wDemModalDialog* demoDlg;
	wlnHiModalDialog* hiDlg;
	optModalDialog* optDlg;

      };
#endif
