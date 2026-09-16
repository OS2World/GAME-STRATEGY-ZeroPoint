//	wlnapp.h:	Header for wlnApp class -*-C++-*-
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


#ifndef wlnAPP_H
#define wlnAPP_H

// Include standard V files as needed

#ifdef vDEBUG
#include <v/vdebug.h>
#endif

//#define DBG

#include <v/vapp.h>
#include <v/vawinfo.h>
#include <v/vnotice.h>

#include "wlncmdw.h"	// we use wlnCommandWindow
#include "poly.h"       // and polygon and polyWalk def's
#include "pwalk.h"
#include "wlnopt.h"     //options stored here
#include "wlndefs.h"

    class wlnApp : public vApp
      {
	friend int AppMain(int, char**);	// allow AppMain access

      public:		//---------------------------------------- public

	wlnApp(char* name, int sdi = 1, int h = 0, int w = 0);
	virtual ~wlnApp();

	// Routines from vApp that are normally overridden

	virtual vWindow* NewAppWin(vWindow* win, char* name, int w, int h,
		vAppWinInfo* winInfo);

	virtual void Exit(void);

	virtual int CloseAppWin(vWindow*);

	virtual void AppCommand(vWindow* win, ItemVal id, ItemVal val, CmdType cType);

	virtual void KeyIn(vWindow*, vKey, unsigned int);
	void KeyFake(vKey key, unsigned int shift);

	virtual void WorkSlice(){step();}

	// New routines for this particular app
	polyWalk *getPWalk() { return aWalk;}
	void setPWalk(polyWalk *aNewWalk) { if(aNewWalk){ aWalk = aNewWalk;}}
	int getLevel() {return level;}
	void setLevel(int aLevel) {level = aLevel - 1; nextLevel();}
	int nextLevel(); //returns the level and sets it up
	long getScore() {return score;}
	void setScore(long aScore) {score = aScore;}
	void stepOn()  {stepping = 1;}
	void stepOff() {stepping = 0;}
	int stepQ() {return stepping;}
	int pause(bool on);
	int step(); //returns 0 if all vels are 0
	int restartLevel();
	int startAtLevel(int);
	void writeStatus();
	int loseALife();     //returns # of lives left, ends game if <0
	int demoing() {return inDemo;}    //are we playing aby ourselves?
	int demodiag(int open = -1) {if(open > -1)demoopen = open;return open;}
	void runDemo(bool truefalse);    //turns on the automatic self-play
	int practicing() {return inPractice;}
	void practiceMode(int onOrOff);
	void debStat(char* msg);
	int writerc();  //returns 0 on success
	bool highScrQ(long aScore = 0); //returns true for on the list
	
	void tellIt(char* msg) {if(Notifier)Notifier->Notice(msg);}

	genArray<saved> &getSaves(void) {return saves;}
	genArray<saved> &getHigh(void) {return highscores;}

      protected:	//--------------------------------------- protected

      private:		//--------------------------------------- private

	wlnCmdWindow* _wlnCmdWin;	// Pointer to instance of first window
	polyWalk *aWalk;
	int level, stepping, lives, inDemo, inPractice, demoopen;
	
	//state flag: start pgm: 3; start level: 1; after start: -1; else 0
	int starting;    
	long score, counter, bonus;
	genArray<option> options;
	genArray<saved> saves;
	genArray<saved> highscores;
	int readrc();
	int parseOptions();
	//sets the bonus at the start of play
	long setStart() {return (level * 150) + 850 + (200 * ((level - 1) % 3)); }
	vNoticeDialog* Notifier;
      };
#endif


