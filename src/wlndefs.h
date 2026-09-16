// Common definitions for the wln classes -*-C++-*-

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


#ifndef WLNDEF_H
#define WLNDEF_H

#include <string.h>

//	Start defines for the main window with 100

//command item ID def's
      enum {
	m_FirstCmd = 100, // Dummy Command
	m_TestDialog,	// TestDialog menu
	m_ModalDialog,	// TestDialog menu
	lblTestStat,	// Status Bar test
	lblLvlStat,   // Status Bar Level label
	lvlStat,      // Actual Status Bar Level readout
	lblScrStat,   // Status bar score label
	scrStat,      // Score readout for status bar
	lblBonStat,   // Status bar bonus label
	bonStat,      // Bonus readout for status bar
	clrStat,      // Color readout on status bar (for debug)
	lblLivStat,   // Status bar lives label
	liveStat,     // Lives readout on status bar
	lblEscStat,   // Status bar label for quitting
	M_Game,       // Identifies game menu
	WLN_NewGame,  // Get a new game
	WLN_NewGame10, //Get a new game starting on level 10
	WLN_RestartLevel, //Start current level over
	WLN_LevelDone, //message that level has finished (broadcast)
	pLFrame,      //frame for level pick radio buttons
	pLFLabel,     //label for that frame
	lSpinner,    //or a spinner for picking the level
	lblStartMsg, //message warning no warrantee etc.
	cancelStart, //cancel button in start dialog
	lblShare,    //main label in shareware dialog
	btnPractice, //button for starting practice play
	mnuPause,    //menu item for pausing
	mnuHigh,     //menu item to show high scores
	mnuSetOptions,
	mnuSaveOpts,
	frmOptDlg,
	optSpin0,    //option spinners in dialog
	optSpin1,
	optSpin2,
	optSpin3,
	optSpin4,
	optSpin5,
	optSpin6,
	optSpin7,
	optSpin8,
	optSpin9,
	optSpin10,
	lblOpt0,
	lblOpt1,
	lblOpt1a,
	lblOpt2,
	lblOpt2a,
	lblOpt3,
	lblOpt3a,
	lblOpt4,
	lblOpt4a,
	lblOpt5,
	lblOpt6,
	lblOpt7,
	lblOpt7a,
	lblOpt8,
	lblOpt9,
	lblOpt10,
	lblHid1,
	M_Demo,
	btnDemo,     //button for running demo
	btnStartDemo,//button for running demo from Demo menu
	btnDemoNext, //
	btnDemoPrev, //
	demoText,    //text in the demo dialog
	demoKeyPress, //the demo is running (message to itself)
	demoClrKeys,  //clear key pictures in demo dialog
	arrowFrame,  //frame for arrows showing motion in demo
	arrowUp,     //arrow icons
	arrowDown,
	arrowLeft,
	arrowRight,
	blankIcon1,
	blankIcon2,
	blankIcon3,
	blankIcon4,
	blankIcon5,
	btnPlay,     //button for going back to game from demo
	mnuPractice,  //menu item to start practice mode
	frmHiScore,   //frame for high score texts
	txtHiName,    //high score texts
	txtHiLevel,
	txtHiScore,
	blkLast		// Last item
      };

enum {rcnamesz = 25, numHi = 10};

struct option{
	char* name;
	long value;
	option(){ name = new char[rcnamesz]; }
	~option() { delete[] name;}
};

struct saved{
	char* name;
	int aLevel;
	long aScore;
	int nLives;
	saved() { name = new char[rcnamesz];
		name[0] = 'R'; name[1] = 'o'; name[2] = 'n'; name[3] = '\0';
		aLevel = 1;
		aScore = 10;
	}
	saved(saved& sv) {
		name = new char[rcnamesz];
		strcpy(name, sv.name);
		aLevel = sv.aLevel;
		aScore = sv.aScore;
		nLives = sv.nLives;
	}
	~saved() { delete[] name; }
};

typedef option option;
typedef saved saved;

#ifdef __WINDOWS__
enum bool {false, true};
#endif

enum{nSC = 19};  //number of standard colors in wln
enum{minRun = 50}; //smallest size for running
enum {blank, Left, Right, Down, Up};  //directions
//this enum is for the demo/help dialog text
enum {intro1, intro2, intro3, descrip1, descrip2, descrip3, action1, action2, extra1,scoring1,scoring2,endlist };



/************************************************************
This is the format of the .rc file used for storing configuration
info. and saved games between invocations.

[CONFIG]
DefStartLevel=1
Speedlim=15
MaxDelay=90
NPollies=50
StepPClr=13
MaxX=600
MaxY=400
Startlives=4
NSaved=15
NHigh=15

[SAVED]
Saved~Name=aName~Level=1~Score=0~Lives=0

[HIGH]
High~Name=aName~Level=1~Score=0

*************************************************************/
#endif //WLNDEF_H
