///////////////////////////////////////////////////////////
// DUNE.CPP: Main source code for Dune Adventure.
///////////////////////////////////////////////////////////
//
// by Bret Williams   
// Aeschinder Software
//
// 5/26/97
//
///////////////////////////////////////////////////////////
#include <afx.h>
#include <windows.h>
#include <stdio.h>
#include "resource.h"
#include "cmydd.h"
#include "cdirsnd.h"

// Prototype for window procedure.
LRESULT APIENTRY WndProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam);

// Structures
struct pt {
	UINT x;
	UINT y;
};

struct traitorType {
	UINT tFaction;
	BOOL tPlayed;
	UINT tLeader;
};

struct strongholdType {
	UINT territoryID;
	BOOL occupied;
	UINT faction;
};

struct TroopType {
	UINT faction;
	UINT qty;
	UINT sector;
};

struct territory {
	char name[26];
	UINT sectors[6];
	struct pt coords[6];
	struct TroopType troops[6];
};

struct men {
	char name[26];
	UINT strength;
	BOOL dead; 
};


struct traitorPick {
	UINT tFaction;
	UINT tLeader;
	BOOL tPicked;
};

struct treacheryCardType {
	UINT tType;
	UINT tID;
	char name[26];
};

struct PlayerType {
	BOOL OwnOrnithopters;
	UINT pFaction;
	UINT pSpice;
	UINT pTreacheryCount;
	UINT pTreacheryCards [8];
	UINT pMaxTreachery;
	UINT pTroops;
	UINT pRevival;
	struct traitorType pTraitors[4];
	UINT pNumTraitors;
	UINT pMaxTraitors;
	UINT pNumStrongholds;
	UINT pBattlesWon;
	UINT pTroopsReserved;
};

struct spiceBlow {
	struct pt sLocation;
	UINT sAmount;
	UINT sTerritoryID;
	UINT sSectorID;
	BOOL sSpiceHere;
};


struct SPicker {
	UINT nSectors;
	UINT Sary[5];
	UINT Selected;
};

// Enumerations
enum { Fremen, Harkonnen, Guild, Emperor, Atreides, BeneGesserit };
enum { Storm, Spice, Bidding, Movement, Battle, Collection };

// Constants
const NUMFACTIONS = 6;
const NUMSECTORS = 18;
const NUMLEADERS = 5;
const NUMTERRITORIES = 42;
const NUMSPICELOCS = 15;


// Function prototypes
BOOL InitSound();
UINT LoadSound(char* fileName);
BOOL InitGraphics();
void OnKeyDown(WPARAM wParam);
void OnDestroy();
void DisplayTreachery();
void OnTimer();
void InitNewGame();
void OnLButtonDown(LPARAM lParam);
void HandleNextTurnButton();
void HandleTreacheryButton();
void DrawView();
void OnStorm();
void OnSpice();
void DisplayPhrase(char* s, UINT x, UINT y);
void OnBidding();
void OnBattle();
void OnMovement();
void OnCollection();
int myRand(int ulimit);
void DisplaySpice();
void DisplayNumber(char* s, UINT x, UINT y);
void ShuffleSpiceDeck();
void DisplayStats();
void NextRound();
void BidForCards();
void HandleTreacheryOKButton();
void SelectTraitors();
void DisplayTraitors();
void HandleTraitorClicked(int choice);
void HandleCardSelection(int choice);
void ShuffleTreacheryCards();
void DetermineCards();
int GetCard();
char* GetCardType(int cardType);
void HandleBidButton();
int TreacheryCardAnalyticals();
void HandleConcedeBidButton();
void NextBidCard();
void HandleStatusButton();
void ShowStatus();
char* GetRoundName(int round);
void OnLButtonUp();
void DoTurnPressed();
void DoTreacheryButtonPressed();
void DoStatusPressed();
void InitializeTerritoryGraph();
BOOL IsAdjacent(UINT t1, UINT t2);
void minpath(UINT start);
void TerritoryClicked(UINT territory);
void DisplayTroops();
BOOL ValidateMove(BOOL Ornithopters, UINT source, UINT dest);
void HandleMoveCancelButton();
void HandleMoveButton();
void SectorChosen(UINT s);
void CleanSource();
void FillDest();

// Global instance handle.
HINSTANCE hInstance;

// Global variables
BOOL groundMoveDone;
BOOL transportMoveDone;
UINT sTerr;
UINT dTerr;
UINT mTroops;
BOOL MovesDone;
struct SPicker SectorList;
CDirSound* pDirSound;
UINT stormBufferNum;
struct traitorType tTraitors[2][4];
UINT destStorm;
PlayerType player;
UINT sundrySurfaceNum;
UINT biddingSurfaceNum;
UINT palSurfaceNum;
UINT treacherySurfaceNum;
UINT tBidCards[NUMFACTIONS];
PlayerType computer;
struct men Leaders [NUMFACTIONS][NUMLEADERS];
struct pt StormLocation [NUMSECTORS];
struct territory Territories [NUMTERRITORIES];
struct treacheryCardType tCards[33];
UINT tCardDeck[33];
UINT tCardTop;
UINT round;
UINT stormSector;
CMyDirDraw* pDirDraw;
BOOL ddok;
BOOL setupOK = FALSE;
HWND hWindow;
UINT boardSurfaceNum;
UINT borderSurfaceNum;
UINT currentRound;
UINT currentTurn;
struct spiceBlow spiceInfo [NUMSPICELOCS];
UINT currentSpiceCard;
UINT spiceDeck[21];
BOOL EndOfGame;
BOOL showWorm;
UINT lastSpiceBlow;
BOOL stormMoving;
BOOL biddingUp;
BOOL spiceLost;
BOOL treacheryUp;
BOOL mainUp;
BOOL choosingTraitors;
UINT tCurrentBids[NUMFACTIONS];
UINT playerBid;
UINT computerBid;
UINT biddingCard;
BOOL computerConcedes;
BOOL playerConcedes;
BOOL biddingOver;
UINT totalBidCards;
char biddingMessages[5][50];
UINT numBiddingMessages;
char moveMessages[3][50];
UINT moveLines;
struct strongholdType strongholds[5];
BOOL statusUp;
UINT statusSurfaceNum;
BOOL nextTurnPressed;
BOOL statusPressed;
BOOL treacheryPressed;
UINT tGraph[42][42];	    // the accursed ajacency matrix
UINT From[42];				// lists initial ends of edges used in optimal paths
UINT To[42];				// lists corresponding terminal ends
UINT Dist[42];				// minimum distances


///////////////////////////////////////////////////////////
// WinMain
///////////////////////////////////////////////////////////
int APIENTRY WinMain(HINSTANCE hCurrentInst,
    HINSTANCE hPreviousInst, LPSTR /*lpszCmdLine */,
    int nCmdShow)
{
    WNDCLASS wndClass;
    HWND hWnd;
    MSG msg;

    // Define and register the window class.
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hCurrentInst;
    wndClass.hIcon = LoadIcon(hCurrentInst,
        MAKEINTRESOURCE(IDI_ICON_DUNE));
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = NULL;
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = "Dune";

    RegisterClass(&wndClass);

    hInstance = hCurrentInst;

    // Create a window of the previously defined class.
    hWnd = CreateWindow(
               "Dune",          // Window class's name.
               "Dune Adventure",// Title bar text.
               WS_POPUP,         // The window's style.
               0,                // X position.
               0,                // Y position.
               640,              // Width.
               480,              // Height.
               NULL,             // Parent window's handle.
               NULL,             // Menu handle.
               hCurrentInst,     // Instance handle.
               NULL);            // No additional data.

    // Display the window on the screen.
    ShowWindow(hWnd, nCmdShow);

    // Force the window to repaint itself.
    UpdateWindow(hWnd);


    // Save the window handle.
	hWindow = hWnd;

    InitNewGame();

	// Setup DirectDraw
	setupOK = InitGraphics();
	if (!setupOK)
		return 0;
		
	// Setup DirectSound
	setupOK = InitSound();
	if(!setupOK)
		return 0;
	
	
	// Start the message loop.
    while (GetMessage(&msg, NULL, NULL, NULL))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

///////////////////////////////////////////////////////////
// WndProc()
//
// This is the main window procedure, which is called by
// Windows.
///////////////////////////////////////////////////////////
LRESULT APIENTRY WndProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{
    // Handle the messages to which the application
    // must respond.
    switch(message)
    {
	case WM_KEYDOWN:
		OnKeyDown(wParam);
		return 0;

	case WM_LBUTTONDOWN:
		OnLButtonDown(lParam);
		return 0;

	case WM_LBUTTONUP:
		OnLButtonUp();
		return 0;

	case WM_TIMER:
		OnTimer();
		return 0;
    
	case WM_DESTROY:
		OnDestroy();
        PostQuitMessage(0);
        return 0;
    }

    // Make sure all unhandled messages get returned to Windows.
    return DefWindowProc(hWnd, message, wParam, lParam);
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

BOOL InitGraphics() {
	pDirDraw = new CMyDirDraw(hWindow);
	ddok = pDirDraw->DirectDrawOK();
	if (!ddok) 
		return FALSE;
	
	boardSurfaceNum = pDirDraw->CreateOffScreenSurface(640,480);
	if (boardSurfaceNum == 0)
		return FALSE;
	borderSurfaceNum = pDirDraw->CreateOffScreenSurface(640,480);
	if (borderSurfaceNum == 0)
		return FALSE;

	sundrySurfaceNum = pDirDraw->CreateOffScreenSurface(640,480);
	if (sundrySurfaceNum == 0)
		return FALSE;

	treacherySurfaceNum = pDirDraw->CreateOffScreenSurface(640,480);
	if (treacherySurfaceNum == 0)
		return FALSE;

	biddingSurfaceNum = pDirDraw->CreateOffScreenSurface(640,480);
	if(biddingSurfaceNum == 0)
		return FALSE;

	statusSurfaceNum = pDirDraw->CreateOffScreenSurface(640,480);
	if(statusSurfaceNum == 0)
		return FALSE;

	CDib* pDib = new CDib("BOARD3.BMP");
	
	// Set the palette
	BOOL resultOK = pDirDraw->SetPalette(pDib);
	if (!resultOK)
		return FALSE;
	
	// Copy board to surface
	pDirDraw->CopyDibToSurface(boardSurfaceNum,
		&CPoint(0,0), pDib, &CRect(0,0,639,479));
	delete pDib;

	pDib = new CDib("BORDER.BMP");
	pDirDraw->CopyDibToSurface(borderSurfaceNum,
		&CPoint(0,0), pDib, &CRect(0,0,639,479));
	delete pDib;
	
	pDib = new CDib("SUNDRY1.BMP");
	pDirDraw->CopyDibToSurface(sundrySurfaceNum,
		&CPoint(0,0), pDib, &CRect(0,0,639,479));
	delete pDib;

	pDib = new CDib("TREACHERY.BMP");
	pDirDraw->CopyDibToSurface(treacherySurfaceNum,
		&CPoint(0,0), pDib, &CRect(0,0,639,479));
	delete pDib;
	
	pDib = new CDib("BIDDING.BMP");
	pDirDraw->CopyDibToSurface(biddingSurfaceNum,
		&CPoint(0,0), pDib, &CRect(0,0,639,479));
	delete pDib;

	pDib = new CDib("STATUS.BMP");
	pDirDraw->CopyDibToSurface(statusSurfaceNum,
		&CPoint(0,0), pDib, &CRect(0,0,639, 479));
	delete pDib;
	

	// before blitting things, we need to set transparency for the sundry items
	pDirDraw->SetTransparentColors(boardSurfaceNum,  0, 0);
	pDirDraw->SetTransparentColors(treacherySurfaceNum, 0, 0);
	pDirDraw->SetTransparentColors(statusSurfaceNum, 0, 0);
	pDirDraw->SetTransparentColors(sundrySurfaceNum, 0, 0);
	
    // Blit the border onto the back buffer
	resultOK = pDirDraw->BlitImage(&CPoint(0,0),
		borderSurfaceNum, &CRect(0,0,639,479));
	if(!resultOK)
		return FALSE;

    // Blit the board onto the back buffer
	resultOK = pDirDraw->BlitImage(&CPoint(125,25),
		boardSurfaceNum, &CRect(0,0,399,399));
	if(!resultOK)
		return FALSE;


    // Blit the storm to its initial spot, sector 1, by Meridian territory
	resultOK = pDirDraw->BlitImage(&CPoint(StormLocation[stormSector].x,
		StormLocation[stormSector].y),
		boardSurfaceNum, &CRect(401,0,450,49));
	if(!resultOK)
		return FALSE;

    DisplaySpice();
	DisplayStats();
	DisplayTroops();
	
	resultOK = pDirDraw->FlipSurfaces();
	pDirDraw->ClearSurface(BACKBUFFERNUM);
    
	return TRUE;

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

BOOL InitSound() {

	pDirSound = new CDirSound(hWindow);
	if(!pDirSound->DirectSoundOK())
		return FALSE;
   
	stormBufferNum = LoadSound("STORM.WAV");
	if(!stormBufferNum)
		return FALSE;

	return TRUE;

}


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

UINT LoadSound(char* fileName) {

	CWave* pWave = new CWave(fileName);
	
	if(!pWave->WaveOK()) {
		delete pWave;
		return 0;
	}

	UINT bufferNum = pDirSound->CreateSoundBuffer(pWave);
	if(!bufferNum) {
		delete pWave;
		return 0;
	}

	BOOL result = pDirSound->CopyWaveToBuffer(pWave, bufferNum);
	delete pWave;
	if(!result)
		return 0;

	return bufferNum;

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void OnLButtonDown(LPARAM lParam) {

	UINT x = LOWORD(lParam);
	UINT y = HIWORD(lParam);

	// Clicked the Next Turn Button?
	if((x>555) && (x<633) &&
	   (y>345) && (y<375) && mainUp) 
	   if ((currentTurn == 1) && (currentRound == Storm)) {
			nextTurnPressed = TRUE;
			OnStorm();
	   }
	   else
		   HandleNextTurnButton();

	// Clicked the treachery Button?
	if((x>555) && (x<633) &&
	   (y>323) && (y<340)  && mainUp)
	   HandleTreacheryButton();

	if((statusUp) && (currentRound != Bidding)) {
		statusUp = FALSE;
		mainUp = TRUE;
		DrawView();
		return;
	}

	if((statusUp) && (currentRound == Bidding)) {
		statusUp = FALSE;
		biddingUp = TRUE;
		DrawView();
		return;
	}

    if(biddingUp && (x>493) && (x<551) 
				 && (y>349) && (y<366))
		HandleStatusButton();


	// Clicked the status button?
	if((x>555) && (x<633) &&
	   (y>301) && (y<318) && mainUp)
	   HandleStatusButton();

	// Clicked the OK button on treachery screen
	if((x>583) && (x<623) &&
	   (y>446) && (y<467) && treacheryUp)
		HandleTreacheryOKButton();

	// Treachery Card selection structure
	if((x>200) && (x<400) && (!choosingTraitors) && treacheryUp) {
		if((y>160) && (y<174)) {
			HandleCardSelection(0);
		}
		if((y>175) && (y<189)) {
			HandleCardSelection(1);
		}
		if((y>190) && (y<204)) {
			HandleCardSelection(2);
		}
		if((y>205) && (y<220)) {
			HandleCardSelection(3);
		}
	}


	if((x>300) && (x<370) && choosingTraitors) {
		if((y>115) && (y<129)) {
			HandleTraitorClicked(0);
		}
		if((y>130) && (y<144)) {
			HandleTraitorClicked(1);
		}
		if((y>145) && (y<159)) {
			HandleTraitorClicked(2);
		}
		if((y>160) && (y<174)) {
			HandleTraitorClicked(3);
		}
	}

	if(biddingUp) { //bidding round
		if((x>565) && (x<623) &&
		   (y>429) && (y<464)) {  // ok clicked
			HandleConcedeBidButton();
		}
		if((x>493) && (x<551) &&
		   (y>379) && (y<414)) {  // clear clicked
			playerBid = 0;
			DrawView();
		}
		if((x>493) && (x<551) &&
		   (y>429) && (y<464)) {  // bid clicked
			HandleBidButton();
		}
		if((y>434) && (y<452)) {  // number clicked
			if((x>30) && (x<51)) {
				playerBid += 10;
				DrawView();
			}
			if((x>57) && (x<76)) {
				playerBid += 20;
				DrawView();
			}
			if((x>85) && (x<106)) {
				playerBid += 30;
				DrawView();
			}
			if((x>113) && (x<133)) {
				playerBid += 40;
				DrawView();
			}
			if((x>141) && (x<160)) {
				playerBid += 50;
				DrawView();
			}
			if((x>168) && (x<178)) {
				playerBid += 1;
				DrawView();
			}
			if((x>186) && (x<196)) {
				playerBid += 2;
				DrawView();
			}
			if((x>202) && (x<213)) {
				playerBid += 3;
				DrawView();
			}
			if((x>219) && (x<230)) {
				playerBid += 4;
				DrawView();
			}
			if((x>237) && (x<247)) {
				playerBid += 5;
				DrawView();
			}
			if((x>254) && (x<264)) {
				playerBid += 6;
				DrawView();
			}
			if((x>270) && (x<280)) {
				playerBid += 7;
				DrawView();
			}
			if((x>288) && (x<298)) {
				playerBid += 8;
				DrawView();
			}
			if((x>305) && (x<315)) {
				playerBid += 9;
				DrawView();
			}
		}
	}

	if(currentRound == Movement) {
		if((x>1) && (x<43) &&
			(y>277) && (y<396)) 
			// The spaceship was clicked
			if(sTerr == 99) {
				sTerr = 50;
				DrawView();
			}
		if((x>72) && (x<130) &&
			(y>204) && (y<239))
			// Cancel was pressed
			HandleMoveCancelButton();
		if((x>5) && (x<63) &&
			(y>204) && (y<239))			
			HandleMoveButton();
			
		if((x>8) && (x<25) &&
			(y>164) && (y<178))
			SectorChosen(0);
		if((x>8) && (x<25) &&
			(y>180) && (y<192))
			SectorChosen(1);
		if((x>31) && (x<50) &&
			(y>164) && (y<178))
			SectorChosen(2);
		if((x>31) && (x<50) &&
			(y>180) && (y<192))
			SectorChosen(3);
		if((x>54) && (x<74) &&
			(y>164) && (y<184))
			SectorChosen(4);
		if((y>130) && (y<140)) {
			// A troop number was clicked
			if((x>15) && (x<22)) 
				mTroops = 0;
			if((x>24) && (x<30))
				mTroops = 1;
			if((x>31) && (x<38))
				mTroops = 2;
			if((x>40) && (x<46))
				mTroops = 3;
			if((x>49) && (x<54))
				mTroops = 4;
			if((x>55) && (x<61))
				mTroops = 5;
			if((x>64) && (x<70))
				mTroops = 6;
			if((x>71) && (x<77))
				mTroops = 7;
			if((x>80) && (x<86))
				mTroops = 8;
			if((x>87) && (x<94))
				mTroops = 9;
			DrawView();
		}
	
	}

	if(mainUp) {
		// The territory handlers
		if((x>246) && (x<289) &&
		   (y>369) && (y<380))
		   TerritoryClicked(0);
		if((x>308) && (x<339) && 
			(y>378) && (y<401))
			TerritoryClicked(1);
		if((x>366) && (x<398) &&
			(y>368) && (y<390))
			TerritoryClicked(2);
		if((x>362) && (x<409) &&
			(y>297) && (y<324))
			TerritoryClicked(3);
		if((x>341) && (x<367) &&
			(y>262) && (y<288))
			TerritoryClicked(4);
		if((x>435) && (x<467) &&
			(y>298) && (y<321))
			TerritoryClicked(5);
		if((x>479) && (x<505) &&
			(y>252) && (y<275))
			TerritoryClicked(6);
		if((x>283) && (x<337) &&
			(y>333) && (y<356))
			TerritoryClicked(7);
		if((x>293) && (x<326) &&
			(y>282) && (y<306))
			TerritoryClicked(8);
		if((x>126) && (x<208) &&
			(y>409) && (y<420))
			TerritoryClicked(9);
		if((x>236) && (x<300) &&
			(y>404) && (y<415))
			TerritoryClicked(10);
		if((x>177) && (x<224) &&
			(y>387) && (y<410))
			TerritoryClicked(11);
		if((x>180) && (x<229) &&
			(y>341) && (y<365))
			TerritoryClicked(12);
		if((x>183) && (x<217) &&
			(y>287) && (y<308))
			TerritoryClicked(13);
		if((x>144) && (x<190) &&
			(y>249) && (y<269))
			TerritoryClicked(14);
		if((x>162) && (x<243) &&
			(y>229) && (y<241))
			TerritoryClicked(15);
		if((x>301) && (x<330) &&
			(y>221) && (y<247))
			TerritoryClicked(16);
		if((x>345) && (x<367) &&
			(y>208) && (y<245))
			TerritoryClicked(17);
		if((x>372) && (x<393) &&
			(y>232) && (y<255))
			TerritoryClicked(18);
		if((x>487) && (x<523) &&
			(y>193) && (y<219))
			TerritoryClicked(19);
		if((x>413) && (x<441) &&
			(y>199) && (y<223))
			TerritoryClicked(20);
		if((x>442) && (x<473) &&
			(y>131) && (y<153))
			TerritoryClicked(21);
		if((x>380) && (x<406) &&
			(y>163) && (y<183))
			TerritoryClicked(22);
		if((x>441) && (x<466) &&
			(y>98) && (y<125))
			TerritoryClicked(23);
		if((x>467) && (x<516) &&
			(y>88) && (y<112))
			TerritoryClicked(24);
		if((x>419) && (x<458) &&
			(y>53) && (y<72))
			TerritoryClicked(25);
		if((x>452) && (x<483) &&
			(y>73) && (y<84))
			TerritoryClicked(26);
		if((x>361) && (x<407) &&
			(y>102) && (y<114))
			TerritoryClicked(27);
		if((x>332) && (x<360) &&
			(y>147) && (y<173))
			TerritoryClicked(28);
		if((x>360) && (x<381) &&
			(y>47) && (y<73))
			TerritoryClicked(29);
		if((x>308) && (x<346) &&
			(y>113) && (y<141))
			TerritoryClicked(30);
		if((x>293) && (x<329) &&
			(y>72) && (y<83))
			TerritoryClicked(31);
		if((x>308) && (x<339) &&
			(y>188) && (y<199))
			TerritoryClicked(32);
		if((x>249) && (x<281) &&
			(y>47) && (y<75))
			TerritoryClicked(33);
		if((x>260) && (x<290) &&
			(y>148) && (y<172))
			TerritoryClicked(34);
		if((x>230) && (x<260) &&
			(y>171) && (y<199)) 
			TerritoryClicked(35);
		if((x>205) && (x<272) &&
			(y>87) && (y<109))
			TerritoryClicked(36);
		if((x>191) && (x<216) &&
			(y>137) && (y<159))
			TerritoryClicked(37);
		if((x>135) && (x<181) &&
			(y>142) && (y<168))
			TerritoryClicked(38);
		if((x>148) && (x<187) &&
			(y>183) && (y<206))
			TerritoryClicked(39);
		if((x>143) && (x<212) &&
			(y>209) && (y<221))
			TerritoryClicked(40);
		if((x>262) && (x<287) &&
			(y>207) && (y<229))
			TerritoryClicked(41);
	}

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void OnLButtonUp () {

	if(nextTurnPressed)
		DoTurnPressed();

	if(treacheryPressed)
		DoTreacheryButtonPressed();

	if(statusPressed)
		DoStatusPressed();

}


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void OnKeyDown(WPARAM wParam) {
	if (wParam == VK_ESCAPE) {
		
		DrawView();
		
		pDirDraw->FlipToGDISurface();
		UINT response = MessageBox(hWindow,
			"Do you want to quit?",
			"Quit", MB_YESNO | MB_ICONQUESTION);
		if (response == IDYES)
			PostMessage(hWindow, WM_CLOSE, 0, 0L);
		else
			DrawView();
	}

	
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void OnDestroy() {
	delete pDirDraw;
	delete pDirSound;
	KillTimer(hWindow, 1);
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void OnTimer() {

    if (destStorm == stormSector) {
		stormMoving = FALSE;// storm has arrived at destination
		KillTimer(hWindow, 1);
	    if (currentTurn == 1) 
			//HandleNextTurnButton();	
			DoTurnPressed();
	}
	else {
		stormSector++;
		if (stormSector > NUMSECTORS-1)
	    	stormSector -= 18;
		// Check for spice on exposed territories!!
		for(UINT i=0; i<NUMSPICELOCS; i++)
			if(spiceInfo[i].sSpiceHere && (stormSector == spiceInfo[i].sSectorID))
			    spiceInfo[i].sSpiceHere = FALSE;
		DrawView();
		
	}


}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void InitNewGame() {

	//seed the random number generator
	srand((unsigned)time(0));
	int dummy = myRand(6); //start the randomization

	//laboriously initialize the leaders with their names, strengths, and death status
	//Atreides
	strcpy(Leaders[Atreides][0].name, "Thurfir Hawat\0");
	strcpy(Leaders[Atreides][1].name, "Lady Jessica\0");
	strcpy(Leaders[Atreides][2].name, "Gurney Halleck\0");
	strcpy(Leaders[Atreides][3].name, "Duncan Idaho\0");
	strcpy(Leaders[Atreides][4].name, "Dr. Yueh\0");
	Leaders[Atreides][0].strength = 5;
	Leaders[Atreides][1].strength = 5;
	Leaders[Atreides][2].strength = 4;
	Leaders[Atreides][3].strength = 2;
	Leaders[Atreides][4].strength = 1;
	for (UINT i=0; i<NUMLEADERS; i++) {
		Leaders[Atreides][i].dead = FALSE;
	    Leaders[Harkonnen][i].dead = FALSE;
	}
	//Harkonnen
	strcpy(Leaders[Harkonnen][0].name, "Feyd-Rautha\0");
	strcpy(Leaders[Harkonnen][1].name, "Beast Rabban\0");
	strcpy(Leaders[Harkonnen][2].name, "Piter DeVries\0");
	strcpy(Leaders[Harkonnen][3].name, "Captain Nefud\0");
	strcpy(Leaders[Harkonnen][4].name, "Umman Kudu\0");
	Leaders[Harkonnen][0].strength = 6;
	Leaders[Harkonnen][1].strength = 4;
	Leaders[Harkonnen][2].strength = 3;
	Leaders[Harkonnen][3].strength = 2;
	Leaders[Harkonnen][4].strength = 1;

	
	for(i=0; i<8; i++) {  //zero out all treachery arrays
		player.pTreacheryCards[i] = 0;
		computer.pTreacheryCards[i] = 0;
	}

	player.OwnOrnithopters = TRUE;
	player.pFaction = Atreides;
	player.pSpice = 10;
	player.pTreacheryCount = 0;
	player.pMaxTreachery = 4;
	player.pTroops = 20;
	player.pRevival = 2;
	player.pNumTraitors = 0;
	player.pMaxTraitors = 1;
	player.pNumStrongholds = 1;
	player.pBattlesWon = 0;
	player.pTroopsReserved = 10;
	

	computer.OwnOrnithopters = TRUE;
	computer.pFaction = Harkonnen;
	computer.pSpice = 10;
	computer.pTreacheryCount = 0;
	computer.pMaxTreachery = 8;
	computer.pTroops = 20;
	computer.pRevival = 2;
	computer.pNumTraitors;
	computer.pMaxTraitors = 2;
	computer.pNumStrongholds = 1;
	computer.pTroopsReserved = 10;
	computer.pBattlesWon = 0;
	

	currentRound = Storm;
	currentTurn = 1;
	stormSector = 0;
	currentSpiceCard = 0;
	moveLines = 0;
	EndOfGame = FALSE;
	showWorm = FALSE;
	spiceLost = FALSE;
	statusUp = FALSE;
	mainUp = TRUE;  // the main screen is showing
	treacheryUp = FALSE;
	nextTurnPressed = FALSE;


	//Initialize the strongholds array
	strongholds[0].territoryID = 5;
	strongholds[0].occupied = FALSE;
	strongholds[1].territoryID = 13;
	strongholds[1].occupied = FALSE;
	strongholds[2].territoryID = 27;
	strongholds[2].occupied = TRUE;
	strongholds[2].faction = Atreides;
	strongholds[3].territoryID = 30;
	strongholds[3].occupied = TRUE;
	strongholds[3].faction = Harkonnen;
	strongholds[4].territoryID = 37;
	strongholds[4].occupied = FALSE;

    //Initialize the stormlocation array.
	StormLocation[0].x = 220;	StormLocation[0].y = 400;
	StormLocation[1].x = 160;	StormLocation[1].y = 365;
	StormLocation[2].x = 120;	StormLocation[2].y = 320;
	StormLocation[3].x = 80;	StormLocation[3].y = 240;
	StormLocation[4].x = 85;	StormLocation[4].y = 160;
	StormLocation[5].x = 120;	StormLocation[5].y = 90;
	StormLocation[6].x = 170;	StormLocation[6].y = 40;
	StormLocation[7].x = 230;	StormLocation[7].y = 0;
	StormLocation[8].x = 300;	StormLocation[8].y = 0;
	StormLocation[9].x = 370;	StormLocation[9].y = 0;
	StormLocation[10].x = 430;	StormLocation[10].y = 25;
	StormLocation[11].x = 485;	StormLocation[11].y = 80;
	StormLocation[12].x = 510;	StormLocation[12].y = 160;
	StormLocation[13].x = 505;	StormLocation[13].y = 235;
	StormLocation[14].x = 475;	StormLocation[14].y = 325;
	StormLocation[15].x = 425;	StormLocation[15].y = 380;
	StormLocation[16].x = 360;	StormLocation[16].y = 405;
	StormLocation[17].x = 290;	StormLocation[17].y = 415;

    // Initialize the territories
	for(i=0; i<NUMTERRITORIES; i++)
		for(int j=0; j<6; j++) {
		   Territories[i].sectors[j] = 99;
		   Territories[i].troops[j].qty = 0;
		}

	strcpy(Territories[0].name,"Meridian\0");
	Territories[0].sectors[0] = 0;
	Territories[0].coords[0].x = 259;	Territories[0].coords[0].y = 380;
	Territories[0].sectors[1] = 17;
	Territories[0].coords[1].x = 292;	Territories[0].coords[1].y = 380;
	strcpy(Territories[1].name,"Cielago South\0");
	Territories[1].sectors[0] = 17;
	Territories[1].coords[0].x = 315;	Territories[1].coords[0].y = 389;
	Territories[1].sectors[1] = 16;
	Territories[1].coords[1].x = 347;	Territories[1].coords[1].y = 381;
	strcpy(Territories[2].name,"Cielago East\0");
	Territories[2].sectors[0] = 16;
	Territories[2].coords[0].x = 357;	Territories[2].coords[0].y = 341;
	Territories[2].sectors[1] = 15;
	Territories[2].coords[1].x = 389;	Territories[2].coords[1].y = 358;
	strcpy(Territories[3].name,"False Wall South\0");
	Territories[3].sectors[0] = 15;
	Territories[3].coords[0].x = 389;	Territories[3].coords[0].y = 327;
	Territories[3].sectors[1] = 14;
	Territories[3].coords[1].x = 400;	Territories[3].coords[1].y = 284;
	strcpy(Territories[4].name,"Harg Pass\0");
	Territories[4].sectors[0] = 15;
	Territories[4].coords[0].x = 336;	Territories[4].coords[0].y = 253;
	Territories[4].sectors[1] = 14;
	Territories[4].coords[1].x = 361;	Territories[4].coords[1].y = 258;
	strcpy(Territories[5].name,"Tuek's Sietch\0");
	Territories[5].sectors[0] = 14;
	Territories[5].coords[0].x = 434;	Territories[5].coords[0].y = 311;
	strcpy(Territories[6].name,"South Mesa \0");
	Territories[6].sectors[0] = 15;
	Territories[6].coords[0].x = 420;	Territories[6].coords[0].y = 356;
	Territories[6].sectors[1] = 14;
	Territories[6].coords[1].x = 471;	Territories[6].coords[1].y = 306;
	Territories[6].sectors[2] = 13;
	Territories[6].coords[2].x = 486;	Territories[6].coords[2].y = 234;
	strcpy(Territories[7].name,"Cielago Depression\0");
	Territories[7].sectors[0] = 0;
	Territories[7].coords[0].x = 273;	Territories[7].coords[0].y = 339;
	Territories[7].sectors[1] = 17;
	Territories[7].coords[1].x = 319;	Territories[7].coords[1].y = 336;
	Territories[7].sectors[2] = 16;
	Territories[7].coords[2].x = 339;	Territories[7].coords[2].y = 339;
	strcpy(Territories[8].name,"Cielago North\0");
	Territories[8].sectors[0] = 0;
	Territories[8].coords[0].x = 281;	Territories[8].coords[0].y = 305;
	Territories[8].sectors[1] = 17;
	Territories[8].coords[1].x = 307;	Territories[8].coords[1].y = 311;
	Territories[8].sectors[2] = 16;
	Territories[8].coords[2].x = 332;	Territories[8].coords[2].y = 293;
	strcpy(Territories[9].name,"Wind Pass North\0");
	Territories[9].sectors[0] = 1;
	Territories[9].coords[0].x = 279;	Territories[9].coords[0].y = 266;
	Territories[9].sectors[1] = 2;
	Territories[9].coords[1].x = 281;	Territories[9].coords[1].y = 242;
	strcpy(Territories[10].name,"Cielago West\0");
	Territories[10].sectors[0] = 0;
	Territories[10].coords[0].x = 264;	Territories[10].coords[0].y = 339;
	Territories[10].sectors[1] = 1;
	Territories[10].coords[1].x = 258;	Territories[10].coords[1].y = 301;
	strcpy(Territories[11].name,"False Wall West\0");
	Territories[11].sectors[0] = 1;
	Territories[11].coords[0].x = 236;	Territories[11].coords[0].y = 309;
	Territories[11].sectors[1] = 2;
	Territories[11].coords[1].x = 238;	Territories[11].coords[1].y = 274;
	Territories[11].sectors[2] = 3;
	Territories[11].coords[2].x = 250;	Territories[11].coords[2].y = 237;
	strcpy(Territories[12].name,"Habbanya Ridge Flat\0");
	Territories[12].sectors[0] = 1;
	Territories[12].coords[0].x = 231;	Territories[12].coords[0].y = 346;
	Territories[12].sectors[1] = 2;
	Territories[12].coords[1].x = 168;	Territories[12].coords[1].y = 301;
	strcpy(Territories[13].name,"Habbanya Ridge Sietch\0");
	Territories[13].sectors[0] = 2;
	Territories[13].coords[0].x = 201;	Territories[13].coords[0].y = 311;
	strcpy(Territories[14].name,"Habbanya Erg\0");
	Territories[14].sectors[0] = 2;
	Territories[14].coords[0].x = 222;	Territories[14].coords[0].y = 261;
	Territories[14].sectors[1] = 3;
	Territories[14].coords[1].x = 165;	Territories[14].coords[1].y = 263;
	strcpy(Territories[15].name,"The Greater Flat\0");
	Territories[15].sectors[0] = 4;
	Territories[15].coords[0].x = 147;	Territories[15].coords[0].y = 227;
	strcpy(Territories[16].name,"Polar Sink\0");
	Territories[16].coords[0].x = 303;	Territories[16].coords[0].y = 244;
	strcpy(Territories[17].name,"False Wall East\0");
	Territories[17].sectors[0] = 14;
	Territories[17].coords[0].x = 349;	Territories[17].coords[0].y = 244;
	Territories[17].sectors[1] = 13;
	Territories[17].coords[1].x = 352;	Territories[17].coords[1].y = 228;
	Territories[17].sectors[2] = 12;
	Territories[17].coords[2].x = 347;	Territories[17].coords[2].y = 215;
	Territories[17].sectors[3] = 11;
	Territories[17].coords[3].x = 342;	Territories[17].coords[3].y = 207;
	Territories[17].sectors[4] = 10;
	Territories[17].coords[4].x = 335;	Territories[17].coords[4].y = 201;
	strcpy(Territories[18].name,"The Minor Erg\0");
	Territories[18].sectors[0] = 14;
	Territories[18].coords[0].x = 386;	Territories[18].coords[0].y = 260;
	Territories[18].sectors[1] = 13;
	Territories[18].coords[1].x = 388;	Territories[18].coords[1].y = 242;
	Territories[18].sectors[2] = 12;
	Territories[18].coords[2].x = 380;	Territories[18].coords[2].y = 207;
	Territories[18].sectors[3] = 11;
	Territories[18].coords[3].x = 369;	Territories[18].coords[3].y = 195;
	strcpy(Territories[19].name,"Red Chasm\0");
	Territories[19].sectors[0] = 12;
	Territories[19].coords[0].x = 482;	Territories[19].coords[0].y = 199;
	strcpy(Territories[20].name,"Pasty Mesa\0");
	Territories[20].sectors[0] = 14;
	Territories[20].coords[0].x = 418;	Territories[20].coords[0].y = 275;
	Territories[20].sectors[1] = 13;
	Territories[20].coords[1].x = 425;	Territories[20].coords[1].y = 239;
	Territories[20].sectors[2] = 12;
	Territories[20].coords[2].x = 443;	Territories[20].coords[2].y = 197;
	Territories[20].sectors[3] = 11;
	Territories[20].coords[3].x = 420;	Territories[20].coords[3].y = 164;
	strcpy(Territories[21].name,"Gara Kulon\0");
	Territories[21].sectors[0] = 11;
	Territories[21].coords[0].x = 470;	Territories[21].coords[0].y = 134;
	strcpy(Territories[22].name,"Shield Wall\0");
	Territories[22].sectors[0] = 11;
	Territories[22].coords[0].x = 409;	Territories[22].coords[0].y = 146;
	Territories[22].sectors[1] = 10;
	Territories[22].coords[1].x = 391;	Territories[22].coords[1].y = 143;
	strcpy(Territories[23].name,"Sihaya Ridge\0");
	Territories[23].sectors[0] = 10;
	Territories[23].coords[0].x = 427;	Territories[23].coords[0].y = 106;
	strcpy(Territories[24].name,"Hole in the Rock\0");
	Territories[24].sectors[0] = 10;
	Territories[24].coords[0].x = 391;	Territories[24].coords[0].y = 123;
	strcpy(Territories[25].name,"Rim Wall West\0");
	Territories[25].sectors[0] = 10;
	Territories[25].coords[0].x = 378;	Territories[25].coords[0].y = 119;
	strcpy(Territories[26].name,"Basin\0");
	Territories[26].sectors[0] = 10;
	Territories[26].coords[0].x = 409;	Territories[26].coords[0].y = 85;
	strcpy(Territories[27].name,"Arrakeen\0");
	Territories[27].sectors[0] = 9;
	Territories[27].coords[0].x = 357;	Territories[27].coords[0].y = 114;
	strcpy(Territories[28].name,"Imperial Basin\0");
	Territories[28].sectors[0] = 10;
	Territories[28].coords[0].x = 347;	Territories[28].coords[0].y = 174;
	Territories[28].sectors[1] = 9;
	Territories[28].coords[1].x = 339;	Territories[28].coords[1].y = 133;
	Territories[28].sectors[2] = 8;
	Territories[28].coords[2].x = 339;	Territories[28].coords[2].y = 97;
	strcpy(Territories[29].name,"Oh Gap\0");
	Territories[29].sectors[0] = 10;
	Territories[29].coords[0].x = 403;	Territories[29].coords[0].y = 73;
	Territories[29].sectors[1] = 9;
	Territories[29].coords[1].x = 380;	Territories[29].coords[1].y = 62;
	Territories[29].sectors[2] = 8;
	Territories[29].coords[2].x = 346;	Territories[29].coords[2].y = 53;
	strcpy(Territories[30].name,"Carthag\0");
	Territories[30].sectors[0] = 8;
	Territories[30].coords[0].x = 311;	Territories[30].coords[0].y = 127;
	strcpy(Territories[31].name,"Tsimpo\0");
	Territories[31].sectors[0] = 8;
	Territories[31].coords[0].x = 310;	Territories[31].coords[0].y = 86;
	Territories[31].sectors[1] = 7;
	Territories[31].coords[1].x = 279;	Territories[31].coords[1].y = 102;
	Territories[31].sectors[2] = 6;
	Territories[31].coords[2].x = 262;	Territories[31].coords[2].y = 128;
	strcpy(Territories[32].name,"Arsunt\0");
	Territories[32].sectors[0] = 8;
	Territories[32].coords[0].x = 313;	Territories[32].coords[0].y = 160;
	Territories[32].sectors[1] = 7;
	Territories[32].coords[1].x = 307;	Territories[32].coords[1].y = 189;
	strcpy(Territories[33].name,"Broken Land\0");
	Territories[33].sectors[0] = 8;
	Territories[33].coords[0].x = 308;	Territories[33].coords[0].y = 45;
	Territories[33].sectors[1] = 7;
	Territories[33].coords[1].x = 278;	Territories[33].coords[1].y = 60;
	strcpy(Territories[34].name,"Hagga Basin\0");
	Territories[34].sectors[0] = 7;
	Territories[34].coords[0].x = 287;	Territories[34].coords[0].y = 133;
	Territories[34].sectors[1] = 6;
	Territories[34].coords[1].x = 275;	Territories[34].coords[1].y = 173;
	strcpy(Territories[35].name,"Plastic Basin\0");
	Territories[35].sectors[0] = 7;
	Territories[35].coords[0].x = 259;	Territories[35].coords[0].y = 87;
	Territories[35].sectors[1] = 6;
	Territories[35].coords[1].x = 232;	Territories[35].coords[1].y = 117;
	Territories[35].sectors[2] = 5;
	Territories[35].coords[2].x = 231;	Territories[35].coords[2].y = 151;
	strcpy(Territories[36].name,"Rock Outcroppings\0");
	Territories[36].sectors[0] = 6;
	Territories[36].coords[0].x = 224;	Territories[36].coords[0].y = 77;
	Territories[36].sectors[1] = 5;
	Territories[36].coords[1].x = 187;	Territories[36].coords[1].y = 111;
	strcpy(Territories[37].name,"Sietch Tabr\0");
	Territories[37].sectors[0] = 5;
	Territories[37].coords[0].x = 196;	Territories[37].coords[0].y = 127;
	strcpy(Territories[38].name,"Bight of the Cliff\0");
	Territories[38].sectors[0] = 5;
	Territories[38].coords[0].x = 172;	Territories[38].coords[0].y = 126;
	Territories[38].sectors[1] = 4;
	Territories[38].coords[1].x = 167;	Territories[38].coords[1].y = 168;
	strcpy(Territories[39].name,"Funeral Plain\0");
	Territories[39].sectors[0] = 4;
	Territories[39].coords[0].x = 198;	Territories[39].coords[0].y = 191;
	strcpy(Territories[40].name,"The Great Flat\0");
	Territories[40].sectors[0] = 4;
	Territories[40].coords[0].x = 217;	Territories[40].coords[0].y = 211;
	strcpy(Territories[41].name,"Wind Pass\0");
	Territories[41].sectors[0] = 5;
	Territories[41].coords[0].x = 287;	Territories[41].coords[0].y = 203;
	Territories[41].sectors[1] = 4;
	Territories[41].coords[1].x = 274;	Territories[41].coords[1].y = 212;
	Territories[41].sectors[2] = 3;
	Territories[41].coords[2].x = 266;	Territories[41].coords[2].y = 229;
	Territories[41].sectors[3] = 2;
	Territories[41].coords[3].x = 263;	Territories[41].coords[3].y = 253;


	// Initialize spiceDeck and related structures
	for(i=0; i<NUMSPICELOCS; i++)
		spiceInfo[i].sSpiceHere = FALSE;
	
	spiceInfo[0].sLocation.x = 165;	spiceInfo[0].sLocation.y = 268;
	spiceInfo[0].sAmount = 8;
	spiceInfo[0].sTerritoryID = 14;
	spiceInfo[0].sSectorID = 3;
	
	spiceInfo[1].sLocation.x = 385;   spiceInfo[1].sLocation.y = 55;
	spiceInfo[1].sAmount = 6;
	spiceInfo[1].sTerritoryID = 29;
	spiceInfo[1].sSectorID = 9;
	
	spiceInfo[2].sLocation.x = 245;	spiceInfo[2].sLocation.y = 210;
	spiceInfo[2].sAmount = 10;
	spiceInfo[2].sTerritoryID = 40;
	spiceInfo[2].sSectorID = 4;
	
	spiceInfo[3].sLocation.x = 280;	spiceInfo[3].sLocation.y = 242;
	spiceInfo[3].sAmount = 6;
	spiceInfo[3].sTerritoryID = 9;
	spiceInfo[3].sSectorID = 2;
	
	spiceInfo[4].sLocation.x = 215;	spiceInfo[4].sLocation.y = 195;
	spiceInfo[4].sAmount = 6;
	spiceInfo[4].sTerritoryID = 39;
	spiceInfo[4].sSectorID = 4;

	spiceInfo[5].sLocation.x = 225;	spiceInfo[5].sLocation.y = 345;
	spiceInfo[5].sAmount = 10;
	spiceInfo[5].sTerritoryID = 12;
	spiceInfo[5].sSectorID = 1;
	
	spiceInfo[6].sLocation.x = 185;	spiceInfo[6].sLocation.y = 105;
	spiceInfo[6].sTerritoryID = 36;
	spiceInfo[6].sAmount = 6;
	spiceInfo[6].sSectorID = 5;
	
	spiceInfo[7].sLocation.x = 425;	spiceInfo[7].sLocation.y = 105;
	spiceInfo[7].sAmount = 6;
	spiceInfo[7].sTerritoryID = 23;
	spiceInfo[7].sSectorID = 10;
	
	spiceInfo[8].sLocation.x = 310;	spiceInfo[8].sLocation.y = 366;
	spiceInfo[8].sAmount = 12;
	spiceInfo[8].sTerritoryID = 1;
	spiceInfo[8].sSectorID = 17;
	
	spiceInfo[9].sLocation.x = 340;	spiceInfo[9].sLocation.y = 300;
	spiceInfo[9].sAmount = 8;
	spiceInfo[9].sTerritoryID = 8;
	spiceInfo[9].sSectorID = 16;
	
	spiceInfo[10].sLocation.x = 470;	spiceInfo[10].sLocation.y = 300;
	spiceInfo[10].sAmount = 10;
	spiceInfo[10].sTerritoryID = 6;
	spiceInfo[10].sSectorID = 14;
	
	spiceInfo[11].sLocation.x = 493;	spiceInfo[11].sLocation.y = 180;
	spiceInfo[11].sAmount = 8;
	spiceInfo[11].sTerritoryID = 19;
	spiceInfo[11].sSectorID = 12;
	
	spiceInfo[12].sLocation.x = 285;	spiceInfo[12].sLocation.y = 57;
	spiceInfo[12].sAmount = 8;
	spiceInfo[12].sTerritoryID = 33;
	spiceInfo[12].sSectorID = 7;
	
	spiceInfo[13].sLocation.x = 275;	spiceInfo[13].sLocation.y = 170;
	spiceInfo[13].sAmount = 6;
	spiceInfo[13].sTerritoryID = 34;
	spiceInfo[13].sSectorID = 6;
	
	spiceInfo[14].sLocation.x = 370;	spiceInfo[14].sLocation.y = 193;
	spiceInfo[14].sAmount = 8;
	spiceInfo[14].sTerritoryID = 18;
	spiceInfo[14].sSectorID = 11;

	for(i=0; i<4; i++) {
		tCards[i].tType = 1;  // projectile weapon
		tCards[i].tID = i;
	}
	strcpy(tCards[0].name, "Stunner\0");
	strcpy(tCards[1].name, "Slip tip\0");
	strcpy(tCards[2].name, "Crysnife\0");
	strcpy(tCards[3].name, "Maula Pistol\0");
	for(i=4; i<8; i++) {
		tCards[i].tType = 2; // poison weapon
		tCards[i].tID = i-4;
	}
	strcpy(tCards[4].name,"Gom jabbar\0");
	strcpy(tCards[5].name,"Ellaca Drug\0");
	strcpy(tCards[6].name,"Chaumas\0");
	strcpy(tCards[7].name,"Chaumurky\0");
    for(i=8; i<12; i++) {
		tCards[i].tType = 3; // shield
		tCards[i].tID = i-8;
		strcpy(tCards[i].name, "Shield\0");
	}
	for(i=12; i<16; i++) {
		tCards[i].tType = 4; // poison defense
		tCards[i].tID = i-12;
		strcpy(tCards[i].name, "Snooper\0");
	}
	for(i=16; i<21; i++) {
		tCards[i].tType = 5; // worthless haha
		tCards[i].tID = i-16;
	}
	strcpy(tCards[16].name,"Kulon\0");
	strcpy(tCards[17].name,"La, La, La\0");
	strcpy(tCards[18].name,"Jubba Cloak\0");
	strcpy(tCards[19].name,"Baliset\0");
	strcpy(tCards[20].name,"Trip to Gamont\0");
	tCards[21].tType = 6; // TruthTrance
	tCards[21].tID = 0;
	strcpy(tCards[21].name, "Truthtrance\0");
	tCards[22].tType = 6; 
	tCards[22].tID = 1;
	strcpy(tCards[22].name, "Truthtrance\0");
    for(i=23; i<26; i++) {
		tCards[i].tType = 7;  // Cheap Hero
		tCards[i].tID = i-23;
		strcpy(tCards[i].name, "Cheap Hero\0");
	}
	for(i=26; i<28; i++) {
		tCards[i].tType = 8; // Holy Karma, Bat-man!
		tCards[i].tID = i-26;
		strcpy(tCards[i].name, "Karama\0");
	}
	tCards[28].tType = 9; //Hajr
	tCards[28].tID = 0;
	strcpy(tCards[28].name, "Hajr\0");
	tCards[29].tType = 10; // Ghola
	tCards[29].tID = 0;
	strcpy(tCards[29].name, "Tleilaxu Ghola\0");
	tCards[30].tType = 11;  // Family Atomics
	tCards[30].tID = 0;
	strcpy(tCards[30].name, "Family Atomics\0");
	tCards[31].tType = 12; // Weather Control
	tCards[31].tID = 0;
	strcpy(tCards[31].name, "Weather Control\0");
	tCards[32].tType = 13; //lasegun
	tCards[32].tID = 0;
	strcpy(tCards[32].name, "Lasegun\0");
	

	ShuffleSpiceDeck();
    SelectTraitors();
	InitializeTerritoryGraph();
	ShuffleTreacheryCards();

    player.pTreacheryCards[0] = tCardDeck[tCardTop++];
	player.pTreacheryCount++;
    computer.pTreacheryCards[0] = tCardDeck[tCardTop++];
	computer.pTreacheryCards[1] = tCardDeck[tCardTop++];
	computer.pTreacheryCount += 2;

	Territories[27].troops[0].faction = Atreides;
	Territories[27].troops[0].qty = 10;
	Territories[27].troops[0].sector = 9;
	Territories[30].troops[0].faction = Harkonnen;
	Territories[30].troops[0].qty = 10;
	Territories[30].troops[0].sector = 8;
	
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void DrawView() {

	char s[10];
	UINT z;
	UINT y;

	if(mainUp) {

		// Blit the border onto the back buffer
		pDirDraw->BlitImage(&CPoint(0,0),
			borderSurfaceNum, &CRect(0,0,639,479));

		// Blit the depressed button
		if(nextTurnPressed) {
			pDirDraw->BlitImage(&CPoint(555,345),
				sundrySurfaceNum, &CRect(1,30,79,60));
		}
		
		if(treacheryPressed) {// blit the depressed button image
			pDirDraw->BlitImage(&CPoint(555,323),
				sundrySurfaceNum, &CRect(1,79,79,96));
		}

		if(statusPressed) {
			pDirDraw->BlitImage(&CPoint(555,301),
				sundrySurfaceNum, &CRect(1,61,79,78));
		}

		// Blit the Board
		pDirDraw->BlitImage(&CPoint(125,25),
			boardSurfaceNum, &CRect(0,0,400,400));

		// Blit the Storm
		pDirDraw->BlitImage(&CPoint(StormLocation[stormSector].x,
			StormLocation[stormSector].y),
			boardSurfaceNum, &CRect(401,0,450,49));

		
		// Draw spice on the board, if any is present
		DisplaySpice();

		// Display Round and Turn information
		DisplayStats();

		// Draw all on-planet troops
		DisplayTroops();


		switch (currentRound) {   // display round-specific messages in the message panel
		case Storm:  break;
		case Spice: 	
			if (showWorm)
				pDirDraw->BlitImage(&CPoint(457,381),
					boardSurfaceNum, &CRect(400,302,578,396));
			if (spiceLost) {
				DisplayPhrase("spice lost", 460,410);
				DisplayPhrase("due to storm",460,420);
				spiceLost = FALSE;
			}
			break;
		case Bidding:  break;
		case Movement:
				// Blit special Atreides Message
				pDirDraw->BlitImage(&CPoint(485, 390),
					sundrySurfaceNum, &CRect(0,0,91,14));
				// debugging
				// Blit Off-Planet move spaceship
				pDirDraw->BlitImage(&CPoint(1,277),
					sundrySurfaceNum, &CRect(31,104,77,227));
				// Blit the movement panel
				pDirDraw->BlitImage(&CPoint(1,1),
						sundrySurfaceNum, &CRect(0,239,174,343));
				// Blit the Troop Ct.
				pDirDraw->BlitImage(&CPoint(1,103),
					sundrySurfaceNum, &CRect(1,342,111,385));
				// Blit the Sector Picker
				pDirDraw->BlitImage(&CPoint(1,146),
					sundrySurfaceNum, &CRect(1,385,76,438));
				// Blit the Move and Cancel Buttons
				pDirDraw->BlitImage(&CPoint(5,204),
					sundrySurfaceNum, &CRect(1,439,132,475));
				// Blit the source territory, if selected
				if(sTerr != 99) {
					if(sTerr!=50) { // 50 is off-planet
						DisplayPhrase(Territories[sTerr].name, 12, 24);
					} 
					else if(sTerr == 50) {
						DisplayPhrase("off planet", 12, 24);
					}
					if (dTerr != 99) {
						DisplayPhrase(Territories[dTerr].name, 12, 52);
						// Display Sector List
						wsprintf(s, "%d", SectorList.Sary[0]);
						DisplayNumber(s, 8,164);
						if(SectorList.nSectors > 1) {
							wsprintf(s, "%d", SectorList.Sary[1]);
							DisplayNumber(s, 8,180);
						}
						if(SectorList.nSectors > 2) {
							wsprintf(s, "%d", SectorList.Sary[2]);
							DisplayNumber(s, 31,164);
						}
						if(SectorList.nSectors > 3) {
							wsprintf(s, "%d", SectorList.Sary[3]);
							DisplayNumber(s, 31,180);
						}
						if(SectorList.nSectors > 4) {
							wsprintf(s, "%d", SectorList.Sary[4]);
							DisplayNumber(s, 54,164);
						}

					}
					else DisplayPhrase("not selected", 12,52);
				}
				else { DisplayPhrase("not selected", 12,24);
					   DisplayPhrase("not selected", 12,52);
				}

				// Display Number of Troops
				wsprintf(s, "%d", mTroops);
				DisplayNumber(s, 45, 80);
				// Display Sector
				if(SectorList.Selected != 99) {
					wsprintf(s, "%d", SectorList.Selected);
					DisplayNumber(s, 136, 80);
				}
				else DisplayPhrase("na", 136, 80);

				if(moveLines > 0) { 
					y = 435;
					for(z=0; z<moveLines; z++) {
						DisplayPhrase(moveMessages[z], 140, y);
						y+=15;
					}
					//moveLines = 0;
				}
		



				if(spiceDeck[currentSpiceCard]==99)  {// Great Shai-Hulud
					DisplayPhrase("using prescience", 460,410);
					DisplayPhrase("you forsee", 460, 420);
					DisplayPhrase("shai hulud", 460,430);
				}
				else {
					DisplayPhrase("using prescience", 460,410);
					DisplayPhrase("you forsee spice at", 460, 420);
					DisplayPhrase(Territories[spiceInfo[spiceDeck[currentSpiceCard]].sTerritoryID].name, 460,430);
				}
				break;
		case Battle:   
			break;
		case Collection:  
			break;
		}
			// Display Round and Turn information
		DisplayStats();
	}  // mainUp

	if (treacheryUp) {
		pDirDraw->BlitImage(&CPoint(0,0),
			treacherySurfaceNum, &CRect(0,0,639,479));
		if(choosingTraitors)
			DisplayTraitors();
		else 
		    DisplayTreachery();
	} // treacheryUp

	if (biddingUp) {
		
		pDirDraw->BlitImage(&CPoint(0,0),
			biddingSurfaceNum, &CRect(0,0,639, 479));
		BidForCards();
		if(numBiddingMessages > 0) {
			y = 300;
			for(UINT i=0; i<numBiddingMessages; i++) {
				DisplayPhrase(biddingMessages[i], 80, y);
				y+=15;
			}
		}
	}

	// show status screen
	if (statusUp) {
		pDirDraw->BlitImage(&CPoint(0,0),
			statusSurfaceNum, &CRect(0,0,639, 479));
		ShowStatus();
	}

	BOOL resultOK = pDirDraw->FlipSurfaces();
	pDirDraw->ClearSurface(BACKBUFFERNUM);
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void OnStorm() {
 
	destStorm = stormSector + myRand(6);
	if (destStorm > NUMSECTORS-1)
		destStorm -= 18;

    // Start a timer to incrementally draw the storm's movement
	stormMoving = TRUE;
	SetTimer(hWindow, 1, 1000, 0); 
    DrawView();
	pDirSound->PlaySound(stormBufferNum);

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

int myRand(int ulimit) {

	int temp;

	temp = (int)(rand()/(32767 / ulimit) + 1);
	if (temp > ulimit)
		temp = ulimit;
	if (temp < 0)
		temp = 1;

	return temp;
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void DisplaySpice() {

	char s[10];
	for(int i=0; i<NUMSPICELOCS; i++)
		if(spiceInfo[i].sSpiceHere) {
			wsprintf(s, "%d", spiceInfo[i].sAmount);
			DisplayNumber(s, spiceInfo[i].sLocation.x, spiceInfo[i].sLocation.y);
		}	
		else pDirDraw->BlitImage(&CPoint(spiceInfo[i].sLocation.x,spiceInfo[i].sLocation.y),
			boardSurfaceNum,&CRect(452,11, 460,21));


}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void DisplayNumber(char* s, UINT x, UINT y) {

	UINT len = strlen(s);

	for(UINT i=0; i<len; i++) {
		char ch = s[i];
		UINT digit = atoi(&ch);
		pDirDraw->BlitImage(&CPoint(x+i*8,y),
			boardSurfaceNum,&CRect(digit*8+453,0,digit*8+8+453,12));
	}
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void ShuffleSpiceDeck() {

	currentSpiceCard = 0;

	for(UINT i=0; i<15; i++) 
		spiceDeck[i] = i;   // Spice Territory Cards
	for(i=15; i<21; i++)
		spiceDeck[i] = 99;  // Shai Halud cards

	UINT temp, j, k;

	// Randomly swap two elements of the array 100 times.
	for(k=0; k<300; k++) {
		i = myRand(20);
		j = myRand(20);
		temp = spiceDeck[j];
		spiceDeck[j] = spiceDeck[i];
		spiceDeck[i] = temp;
	}

	//exchange top and bottom cards
	temp = spiceDeck[0];
	spiceDeck[0] = spiceDeck[20];
	spiceDeck[20] = temp;
	if(spiceDeck[0] == 99) {
		spiceDeck[0] = spiceDeck[7];
		spiceDeck[7] = 99;
	}
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void DisplayStats() {

	UINT y1, y2;

	switch (currentRound) {
	case Storm: y1 = 51;	y2 = 85; break;
	case Spice: y1 = 86;	y2 = 120; break;
	case Bidding: y1 = 121;	y2 = 154; break;
	case Movement: y1 = 155; y2 = 193; break;
	case Battle: y1 = 194; y2 = 229; break;
	case Collection: y1 = 230; y2 = 265; break;
	}

	char s[5];
	wsprintf(s, "%d", currentTurn);
	DisplayNumber(s, 55, 411);
	
	// Blit the graphic that represents which round we're on
	pDirDraw->BlitImage(&CPoint(0,440),
		boardSurfaceNum,&CRect(400,y1,539,y2));

	// Blit the turn
    pDirDraw->BlitImage(&CPoint(0,400),
		boardSurfaceNum,&CRect(400,266,539,301));

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void NextRound() {

	currentRound++;
	if(currentRound > Collection) {
		currentTurn++;
		currentRound = Storm;
		if(currentTurn > 15)
			EndOfGame = TRUE;
	}

	switch (currentRound) {
	case Storm: OnStorm(); break;
	case Spice: OnSpice(); break;
	case Bidding:  OnBidding(); break;
	case Movement:  OnMovement(); break;
	case Battle:  OnBattle(); break;
	case Collection:  OnCollection(); break;
	}
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void HandleNextTurnButton() {
	
	
	nextTurnPressed = TRUE;
	DrawView();
	
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void DoTurnPressed() {

   nextTurnPressed = FALSE;
   if(!stormMoving) 
		NextRound();

}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void OnSpice() {

	if(spiceDeck[currentSpiceCard] == 99) { // Great Shai-Hulud Batman!
		showWorm = TRUE;
		DrawView();

		//  Include code to wipe out previous spice blow and troops in territory!!
		spiceInfo[lastSpiceBlow].sSpiceHere = FALSE;

		currentSpiceCard++;
		if(currentSpiceCard>20)
			ShuffleSpiceDeck();
		while (spiceDeck[currentSpiceCard] == 99) {  // note: make this better so Shai Hulud's aren't wasted
			currentSpiceCard++;
			if(currentSpiceCard>20)
				ShuffleSpiceDeck();
		}
	}
    if(spiceInfo[spiceDeck[currentSpiceCard]].sSectorID == stormSector) 
		spiceLost = TRUE;
	else
		spiceInfo[spiceDeck[currentSpiceCard]].sSpiceHere = TRUE;
	DrawView();
	lastSpiceBlow = spiceDeck[currentSpiceCard];
	currentSpiceCard++;
	if(currentSpiceCard>20)
		ShuffleSpiceDeck();
	showWorm = FALSE;
}
		
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void OnBidding() {

	// initialize bidding structures
	for(UINT i=0; i<NUMFACTIONS; i++) 
		tCurrentBids[i] = 0;
	computerConcedes = FALSE;
	playerConcedes = FALSE;
	biddingOver = FALSE;
	playerBid = 0;
	biddingCard = 0;
	numBiddingMessages = 0;

	biddingUp = TRUE;
	mainUp = FALSE;
	DetermineCards();
	DrawView();
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void OnBattle() {

		DrawView();
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void OnMovement() {

	sTerr = 99;
	dTerr = 99;
	mTroops = 0;
	moveLines = 0;
	groundMoveDone = FALSE;
	transportMoveDone = (player.pSpice == 0);
	SectorList.nSectors = 0;
	SectorList.Selected = 99;
	MovesDone = FALSE;
	DrawView();

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

BOOL ValidateMove(BOOL Ornithopters, UINT source, UINT dest) {

	UINT i,j;

	if((source == 50) || (IsAdjacent(source, dest))) { 
		return TRUE;
	}

	minpath(source);  // generate arrays

	for(i=0; i<42; i++) {
		if(To[i] == dest) {
			j=i;
		}
	}

	if(Ornithopters) {
		if(Dist[To[j]]<4)
			return TRUE;
		else return FALSE;
	} // if branch
	else	{
		if(Dist[To[j]]<2)
			return TRUE;
		else return FALSE;
	} // else branch

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void OnCollection() {

		DrawView();
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void DisplayPhrase(char* s, UINT x, UINT y) {

	UINT len = strlen(s);
	UINT idx;
	for(UINT i=0;i<len; i++) {
		char ch = tolower(s[i]);
		if(ch != ' ') {
			idx = (UINT)ch - 97;
			pDirDraw->BlitImage(&CPoint(x+9*i,y),
				boardSurfaceNum, &CRect(1+(9*idx),468,9+(9*idx),478));
		}

	}
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void HandleTreacheryButton() {

	treacheryPressed = TRUE;
	DrawView();
   
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void DoTreacheryButtonPressed() {

   treacheryPressed = FALSE;
   mainUp = FALSE;
   treacheryUp = TRUE;
   if(player.pNumTraitors == 0) { //traitor has not been chosen
		UINT fc = 0;	UINT j;
		for(j=0; j<4; j++) {
			if(tTraitors[1][j].tFaction == player.pFaction)
				fc += 1;
		}
	    if(fc==4)   // no traitors in random sample
			choosingTraitors = FALSE;
		else
			choosingTraitors = TRUE;
   }
	
   DrawView();
   

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void HandleTreacheryOKButton() {

	mainUp = TRUE;
	treacheryUp = FALSE;
	DrawView();
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void SelectTraitors() {


	struct traitorPick TempLeaders[2*NUMLEADERS];
	

	for(UINT j=0; j<5; j++) {
		TempLeaders[j].tFaction = Harkonnen;
		TempLeaders[j].tLeader = j;
		TempLeaders[j].tPicked = FALSE;
	}
	
	UINT i=0;
	for(j=5; j<10; j++) {
		TempLeaders[j].tFaction = Atreides;
		TempLeaders[j].tLeader = i++;
		TempLeaders[j].tPicked = FALSE;
	}
	for(i=0; i<2; i++) {
		for(j=0; j<4; j++) {
			tTraitors[i][j].tFaction=0;
			tTraitors[i][j].tLeader = 0;
		}
	} 

	i=0;
	j=0;
	UINT tselection;
    
	while ((i<2) && (j<4)) {
		tselection = (myRand(10)) - 1;
		if(!TempLeaders[tselection].tPicked) {
			tTraitors[i][j].tFaction = 
				TempLeaders[tselection].tFaction;
			tTraitors[i][j].tLeader = 
				TempLeaders[tselection].tLeader;
			j++;
			TempLeaders[tselection].tPicked = TRUE;
		}
		if(j==4) {
			j=0;
			i++;
		}
	}

	// Select computer traitors
	for(i=0; i<4; i++) {
		if((tTraitors[0][i].tFaction != computer.pFaction) &&
		   (computer.pNumTraitors < computer.pMaxTraitors)) {
			computer.pNumTraitors++;
			computer.pTraitors[computer.pNumTraitors-1].tFaction = tTraitors[0][i].tFaction;
			computer.pTraitors[computer.pNumTraitors-1].tLeader = tTraitors[0][i].tLeader;
			computer.pTraitors[computer.pNumTraitors-1].tPlayed = FALSE;
		}
	}

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void DisplayTraitors() {

	UINT i,j,x,y;
	char s[10];

	DisplayPhrase("traitor selection", 250, 80);

	x=300;
	y=115;
	i=1;
	for(j=0; j<4; j++) {
		DisplayPhrase(Leaders[tTraitors[i][j].tFaction][tTraitors[i][j].tLeader].name,x,y);
		wsprintf(s, "%d",Leaders[tTraitors[i][j].tFaction][tTraitors[i][j].tLeader].strength);
		DisplayNumber(s, x+140, y-3);
		y += 15;
	}

	UINT fc = 0;
	for(j=0; j<4; j++) {
		if(tTraitors[1][j].tFaction == player.pFaction)
			fc += 1;
	}
	
    if(fc==4) {  // no traitors in random sample
		choosingTraitors = FALSE;
	}
	else {
		DisplayPhrase("You may choose one leader from these", 250, 200);
		DisplayPhrase("to be in your pay as a traitor      ", 250, 215);
		DisplayPhrase("click to choose", 250, 230);
	}

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void HandleTraitorClicked(int choice) {


	if(tTraitors[1][choice].tFaction == player.pFaction)
		return;

	player.pNumTraitors++;
	player.pTraitors[0].tFaction = tTraitors[1][choice].tFaction;
	player.pTraitors[0].tLeader = tTraitors[1][choice].tLeader;
	player.pTraitors[0].tPlayed = FALSE;
	choosingTraitors = FALSE;
	DrawView();

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void ShuffleTreacheryCards () {

	tCardTop = 0;

	UINT i, j, k, temp;

	for(i=0; i<33; i++) {
		tCardDeck[i] = i;
	}

	for(k=0; k<30000; k++) {
		i = myRand(33) - 1;
		j = myRand(33) - 1;
		temp = tCardDeck[j];
		tCardDeck[j] = tCardDeck[i];
		tCardDeck[i] = temp;
	}

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void DisplayTreachery() {

	char s[10];

	DisplayPhrase("enemy leader in your pay", 200, 100);

	if(player.pNumTraitors == 0) {
		DisplayPhrase("no enemy leaders are traitors", 250, 115); 
	}
	else {
		DisplayPhrase(Leaders[player.pTraitors[0].tFaction] [player.pTraitors[0].tLeader].name, 250,115);
		wsprintf(s, "%d", Leaders[player.pTraitors[0].tFaction] [player.pTraitors[0].tLeader].strength);
		DisplayNumber(s, 400,112);
	}

	DisplayPhrase("your treachery cards", 200, 145);

	UINT y=160;
	for(UINT i=0; i<player.pTreacheryCount; i++) {
		DisplayPhrase(tCards[player.pTreacheryCards[0]].name, 250, y);
		wsprintf(s, "%d", tCards[player.pTreacheryCards[0]].tType);
		DisplayNumber(s, 400, y-3);
		y += 15;
	}

	DisplayPhrase("click to use card", 200, 300);


}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void HandleCardSelection(int choice) {

	HandleTreacheryOKButton();

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void DetermineCards() {

	for(UINT i=0; i<NUMFACTIONS; i++)
		tBidCards[i] = 99;
	
	i=0;
	if(player.pMaxTreachery > player.pTreacheryCount)
		i++;
	if(computer.pMaxTreachery > computer.pTreacheryCount)
		i++;

	UINT j;
	for(j=0; j<i; j++) 
		tBidCards[j] = GetCard();

	totalBidCards = i;

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

int GetCard() {

	UINT temp;
	while (tCardDeck[tCardTop] == 99) {
		tCardTop++;
		if(tCardTop > 32) { // sigh, shuffle the damn deck
			ShuffleTreacheryCards();
			for(UINT i=0; i<33; i++) {
				for(UINT j=0; j<player.pTreacheryCount; j++) {
					if(tCardDeck[i] == player.pTreacheryCards[j]) {
						tCardDeck[i] = 99;
					}
				}
				for(j=0; j<computer.pTreacheryCount; j++) {
					if(tCardDeck[i] == computer.pTreacheryCards[j]) {
						tCardDeck[i] = 99;
					}
				}
			}
		}
	}

	temp = tCardDeck[tCardTop];
	tCardTop++;
	return temp;
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void BidForCards() {

	UINT i=0;
	UINT y=160;
	UINT x=137;
	char s[10];

	while((tBidCards[i] != 99) && (i<NUMFACTIONS)) {
		if(i==biddingCard)
			DisplayPhrase("x", 30, y);
		wsprintf(s, "%d", i);
		DisplayNumber(s, 43, y-3);
		DisplayPhrase(tCards[tBidCards[i]].name, 137, y);
		DisplayPhrase(GetCardType(tCards[tBidCards[i]].tType), 340, y);
		wsprintf(s, "%d", tCurrentBids[i]);
		DisplayNumber(s, 508, y-3);
		y+=15;
		i++;
	}

	wsprintf(s, "%d", playerBid);
	DisplayNumber(s, 365, 433);
	wsprintf(s, "%d", player.pSpice);
	DisplayNumber(s, 416,379);

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

char* GetCardType(int cardType) {

   switch(cardType) {
   case 1:
	   return "projectile weapon";
	   break;
   case 2:
	   return "poison weapon";
	   break;
   case 3:
	   return "shield";
	   break;
   case 4:
	   return "poison defense";
	   break;
   case 5:
	   return "worthless";
	   break;
   default:
	   return "special";
   }
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void HandleBidButton() {

    if(playerBid <= tCurrentBids[biddingCard]) {
		numBiddingMessages = 0;
		strcpy(biddingMessages[numBiddingMessages++], "bid higher atreides\0");
		DrawView();
		return;
	}

	if(playerBid > player.pSpice) {
		numBiddingMessages = 0;
		strcpy(biddingMessages[numBiddingMessages++], "you do not have enough spice\0");
		DrawView();
		return;
	}

	if((playerBid > tCurrentBids[biddingCard]) && (playerBid <= player.pSpice) &&
	   (player.pTreacheryCount < player.pMaxTreachery)) {
		tCurrentBids[biddingCard] = playerBid;
	}


    numBiddingMessages = 0;			
	computerBid = TreacheryCardAnalyticals();

    if(computerBid > 0) {
		strcpy(biddingMessages[numBiddingMessages++], "harkonnen bids \0");
		tCurrentBids[biddingCard] = computerBid;
	}
	
	playerBid = 0;
	DrawView();

    if(player.pTreacheryCount == player.pMaxTreachery)
		playerConcedes = TRUE;

	if(computerConcedes || playerConcedes) 
		NextBidCard();
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

int TreacheryCardAnalyticals() {

	int tally = 0;
	UINT CurrentBid = tCurrentBids[biddingCard];

	if(player.pNumStrongholds > 2)
		tally++;
	if(2*CurrentBid > ((UINT)(computer.pSpice * 0.5)))
		tally-=2;
	if(computer.pTreacheryCount < 5)
		tally++;
	if(currentTurn < 7)
		tally++;
	if(computer.pTreacheryCount == computer.pMaxTreachery)
		computerConcedes = TRUE;
	if((computer.pTroops < 10) && (computer.pSpice < 5))
		tally-=2;
	if(totalBidCards > (biddingCard + 1))
		tally--;
	if(tally < 0)
		computerConcedes = TRUE;
	if(computer.pSpice < playerBid)
		computerConcedes = TRUE;

	if(computerConcedes)
		return 0;
	else
		return (playerBid + 1);

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void HandleConcedeBidButton() {  // OK was pressed

  playerConcedes = TRUE;
  if(biddingOver) {
	  biddingUp = FALSE;
	  mainUp = TRUE;
	  DrawView();
  }
  else
	  NextBidCard();

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void NextBidCard() {

	numBiddingMessages = 0;
	if(playerConcedes) { // give the card to the computer
	  if(tCurrentBids[biddingCard] == 0)
		  tCurrentBids[biddingCard] = 1;
	  strcpy(biddingMessages[numBiddingMessages++], "harkonnen gets card\0");
      computer.pTreacheryCount++;
	  computer.pTreacheryCards[computer.pTreacheryCount-1] = tBidCards[biddingCard];
	  computer.pSpice -= tCurrentBids[biddingCard];
	  if(computer.pTreacheryCount < 8) {  // bastard gets another
		  computer.pTreacheryCount++;
		  computer.pTreacheryCards[computer.pTreacheryCount-1] = GetCard();
	  }
	}

	if(computerConcedes) { // give me the money!
		strcpy(biddingMessages[numBiddingMessages++], "atreides gets card\0");
		player.pTreacheryCount++;
		player.pTreacheryCards[player.pTreacheryCount-1] = tBidCards[biddingCard];
		player.pSpice -= tCurrentBids[biddingCard];
	}

	biddingCard++;
	if((biddingCard+1) > totalBidCards) {
		biddingOver = TRUE;
		strcpy(biddingMessages[numBiddingMessages++], "bidding over click ok to go on\0");
	}
	else {
		playerConcedes = FALSE;
		computerConcedes = FALSE;
	}

	DrawView();


}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void HandleStatusButton() {


	statusPressed = TRUE;
	DrawView();

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void DoStatusPressed() {

	statusPressed = FALSE;
	mainUp = FALSE;
	biddingUp = FALSE;
	statusUp = TRUE;
	DrawView();


}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void ShowStatus() {


	char s[10];

	DisplayPhrase(GetRoundName(currentRound), 87,22);
	wsprintf(s, "%d", currentTurn);
	DisplayNumber(s, 590,22);

	//player stats
	wsprintf(s, "%d", player.pSpice);
	DisplayNumber(s, 285, 164);
	wsprintf(s, "%d", player.pTroops);
	DisplayNumber(s, 285, 187);
	wsprintf(s, "%d", player.pTroopsReserved);
	DisplayNumber(s, 285, 209);
	wsprintf(s, "%d", 20 - player.pTroops);
	DisplayNumber(s, 285, 232);
	wsprintf(s, "%d", player.pNumStrongholds);
	DisplayNumber(s, 285, 256);
	wsprintf(s, "%d", player.pTreacheryCount);
	DisplayNumber(s, 285, 284);
	UINT i;
	UINT tUsed = 0;
	for(i=0;i<player.pNumTraitors; i++) 
		if(player.pTraitors[i].tPlayed == TRUE)
			tUsed++;
	wsprintf(s, "%d", tUsed);
	DisplayNumber(s, 285, 304);
	wsprintf(s, "%d", player.pBattlesWon);
	DisplayNumber(s, 285, 328);
		


	// Computer Stats
	// DisplayPhrase("na", 427, 164);
	wsprintf(s, "%d", computer.pSpice);
	DisplayNumber(s, 427, 164);
	wsprintf(s, "%d", computer.pTroops);
	DisplayNumber(s, 427, 187);
	wsprintf(s, "%d", computer.pTroopsReserved);
	DisplayNumber(s, 427, 209);
	wsprintf(s, "%d", 20 - computer.pTroops);
	DisplayNumber(s, 427, 232);
	wsprintf(s, "%d", computer.pNumStrongholds);
	DisplayNumber(s, 427, 256);
	wsprintf(s, "%d", computer.pTreacheryCount);
	DisplayNumber(s, 427, 284);
	tUsed = 0;
	for(i=0;i<computer.pNumTraitors; i++) 
		if(computer.pTraitors[i].tPlayed == TRUE)
			tUsed++;
	wsprintf(s, "%d", tUsed);
	DisplayNumber(s, 427, 304);
	wsprintf(s, "%d", computer.pBattlesWon);
	DisplayNumber(s, 427, 328);

	// debugging lines follow to end of function
	UINT y = 350;
	for(i=0; i<computer.pTreacheryCount; i++) {
	    DisplayPhrase(tCards[computer.pTreacheryCards[i]].name, 15, y);
		y+=15;
	}
	y=350;
	for(i=0; i<computer.pNumTraitors; i++) {
		DisplayPhrase(Leaders[computer.pTraitors[i].tFaction][computer.pTraitors[i].tLeader].name,
			285, y);
		y+=15;
	}


}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

char* GetRoundName(int round) {

   switch(round) {
   case 0:
	   return "storm";
	   break;
   case 1:
	   return "spice";
	   break;
   case 2:
	   return "bidding";
	   break;
   case 3:
	   return "movement";
	   break;
   case 4:
	   return "battle";
	   break;
   case 5:
	   return "collection";
	   break;
   default:
	   return "error";
	   break;
   }
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void InitializeTerritoryGraph() {

	UINT i,j;
	FILE *fout;

	for(i=0; i<42; i++)
		for(j=0; j<42; j++)
			tGraph[i][j] = 0;

	tGraph[0][7]=1;		tGraph[0][1]=1;		tGraph[0][10]=1;	tGraph[0][12]=1;
	tGraph[1][2]=1;		tGraph[1][0]=1;		tGraph[1][7]=1;
	tGraph[2][1]=1;		tGraph[2][7]=1;		tGraph[2][8]=1;		tGraph[2][3]=1;		tGraph[2][6]=1;
	tGraph[3][2]=1;		tGraph[3][8]=1;		tGraph[3][4]=1;		tGraph[3][18]=1;	tGraph[3][20]=1;	tGraph[3][5]=1;		tGraph[3][6]=1;
	tGraph[4][3]=1;		tGraph[4][8]=1;		tGraph[4][16]=1;	tGraph[4][17]=1;	tGraph[4][18]=1;
	tGraph[5][6]=1;		tGraph[5][3]=1;		tGraph[5][20]=1;
	tGraph[6][2]=1;		tGraph[6][3]=1;		tGraph[6][5]=1;		tGraph[6][20]=1;	tGraph[6][19]=1;
	tGraph[7][0]=1;		tGraph[7][1]=1;		tGraph[7][10]=1;	tGraph[7][8]=1;		tGraph[7][2]=1;
	tGraph[8][7]=1;		tGraph[8][10]=1;	tGraph[8][9]=1;		tGraph[8][16]=1;	tGraph[8][4]=1;		tGraph[8][3]=1;		tGraph[8][2]=1;
	tGraph[9][8]=1;		tGraph[9][10]=1;	tGraph[9][41]=1;	tGraph[9][16]=1;
	tGraph[10][0]=1;	tGraph[10][12]=1;	tGraph[10][11]=1;	tGraph[10][41]=1;	tGraph[10][9]=1;	tGraph[10][8]=1;	tGraph[10][7]=1;
	tGraph[11][12]=1;	tGraph[11][14]=1;	tGraph[11][15]=1;	tGraph[11][41]=1;	tGraph[11][10]=1;
	tGraph[12][0]=1;	tGraph[12][10]=1;	tGraph[12][11]=1;	tGraph[12][14]=1;	tGraph[12][13]=1;
	tGraph[13][12]=1;
	tGraph[14][12]=1;	tGraph[14][15]=1;	tGraph[14][11]=1;
	tGraph[15][14]=1;	tGraph[15][11]=1;	tGraph[15][41]=1;	tGraph[15][40]=1;
	tGraph[16][8]=1;	tGraph[16][9]=1;	tGraph[16][41]=1;	tGraph[16][34]=1;	tGraph[16][22]=1;	tGraph[16][28]=1;	tGraph[16][17]=1;	tGraph[16][4]=1;
	tGraph[17][4]=1;	tGraph[17][16]=1;	tGraph[17][28]=1;	tGraph[17][22]=1;	tGraph[17][18]=1;
	tGraph[18][22]=1;	tGraph[18][3]=1;	tGraph[18][4]=1;	tGraph[18][17]=1;	tGraph[18][20]=1;
	tGraph[19][6]=1;	tGraph[19][20]=1;
	tGraph[20][5]=1;	tGraph[20][3]=1;	tGraph[20][18]=1;	tGraph[20][22]=1;	tGraph[20][21]=1;	tGraph[20][19]=1;	tGraph[20][6]=1;
	tGraph[21][20]=1;	tGraph[21][22]=1;	tGraph[21][23]=1;
	tGraph[22][20]=1;	tGraph[22][18]=1;	tGraph[22][17]=1;	tGraph[22][28]=1;	tGraph[22][24]=1;	tGraph[22][23]=1;	tGraph[22][21]=1;
	tGraph[23][21]=1;	tGraph[23][22]=1;	tGraph[23][24]=1;	tGraph[23][26]=1;
	tGraph[24][22]=1;	tGraph[24][28]=1;	tGraph[24][25]=1;	tGraph[24][26]=1;	tGraph[24][23]=1;
	tGraph[25][24]=1;	tGraph[25][28]=1;	tGraph[25][27]=1;	tGraph[25][29]=1;	tGraph[25][26]=1;
	tGraph[26][23]=1;	tGraph[26][24]=1;	tGraph[26][25]=1;	tGraph[26][29]=1;
	tGraph[27][25]=1;	tGraph[27][28]=1;	tGraph[27][29]=1;
	tGraph[28][16]=1;	tGraph[28][17]=1;	tGraph[28][22]=1;	tGraph[28][24]=1;	tGraph[28][25]=1;	tGraph[28][27]=1;	tGraph[28][32]=1;	tGraph[28][29]=1;	tGraph[28][30]=1;	tGraph[28][31]=1;
	tGraph[29][26]=1;	tGraph[29][25]=1;	tGraph[29][27]=1;	tGraph[29][28]=1;	tGraph[29][31]=1;	tGraph[29][33]=1;
	tGraph[30][32]=1;	tGraph[30][34]=1;	tGraph[30][31]=1;	tGraph[30][28]=1;
	tGraph[31][30]=1;	tGraph[31][34]=1;	tGraph[31][35]=1;	tGraph[31][33]=1;	tGraph[31][38]=1;	tGraph[31][29]=1;	tGraph[31][28]=1;
	tGraph[32][16]=1;	tGraph[32][34]=1;	tGraph[32][30]=1;	tGraph[32][28]=1;
	tGraph[33][31]=1;	tGraph[33][35]=1;	tGraph[33][36]=1;	tGraph[33][29]=1;
	tGraph[34][41]=1;	tGraph[34][35]=1;	tGraph[34][30]=1;	tGraph[34][31]=1;	tGraph[34][32]=1;	tGraph[34][16]=1;
	tGraph[35][38]=1;	tGraph[35][39]=1;	tGraph[35][40]=1;	tGraph[35][41]=1;	tGraph[35][37]=1;	tGraph[35][36]=1;	tGraph[35][33]=1;	tGraph[35][31]=1;	tGraph[35][34]=1;
	tGraph[36][35]=1;	tGraph[36][37]=1;	tGraph[36][38]=1;	tGraph[36][33]=1;
	tGraph[37][35]=1;	tGraph[37][36]=1;	tGraph[37][38]=1;
	tGraph[38][35]=1;	tGraph[38][36]=1;	tGraph[38][37]=1;	tGraph[38][39]=1;
	tGraph[39][40]=1;	tGraph[39][38]=1;	tGraph[39][35]=1;
	tGraph[40][15]=1;	tGraph[40][39]=1;	tGraph[40][35]=1;	tGraph[40][41]=1;
	tGraph[41][34]=1;	tGraph[41][35]=1;	tGraph[41][40]=1;	tGraph[41][15]=1;	tGraph[41][11]=1;	tGraph[41][10]=1;	tGraph[41][9]=1;	tGraph[41][16]=1;


	// following lines are debugging.  They print out the contents of the matrix
	// and also the territories[].name array

	if((fout=fopen("output.txt","w"))!=NULL) {
		fprintf(fout,"   ");
		for(i=0; i<42; i++) {
			fprintf(fout, "%d ", i);
			if(i<10)
				fprintf(fout," ");
		}
		fprintf(fout,"\n");
		for(i=0; i<42; i++) {
			fprintf(fout, "%d ", i);
			if(i<10)
				fprintf(fout, " ");
			for(j=0; j<42; j++)
				fprintf(fout, "%d  ",tGraph[i][j]);
			fprintf(fout,"\n");
		}
		fprintf(fout,"\n\n\n");
		for(i=0; i<42; i++) 
			fprintf(fout, "%d  %s\n", i, Territories[i].name);

		fprintf(fout,"\n\n\n");
		if(IsAdjacent(20,17))
			fprintf(fout, "%s is adjacent to %s\n", Territories[20].name, Territories[17].name);
		else
			fprintf(fout, "%s is NOT adjacent to %s\n", Territories[20].name, Territories[17].name);
		if(IsAdjacent(20,22))
			fprintf(fout, "%s is adjacent to %s\n", Territories[20].name, Territories[22].name);
		else
			fprintf(fout, "%s is NOT adjacent to %s\n", Territories[20].name, Territories[22].name);
		fclose(fout);
	}

	minpath(0);

}


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

BOOL IsAdjacent(UINT t1, UINT t2) {

	return (tGraph[t1][t2]==1);

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void minpath(UINT start) {

	UINT v=42,			// number of vertices
		 at,			// most recently closed vertex
		 e,				// number of edges recorded for optimal paths
		 d,				// temporary variable holding distance between two vertices
		 min,			// minimum value between two vertices
		 i,j,			// looping counters
		 Open[42],		// defines open(1) and closed(1) vertices
	     A[42][42],		// weights
		 Tree[42];		// indicates for each vertex as it is closed the vertex from which it came
		 
	FILE *fp;			// file pointer for debugging output

	for(i=0; i<v; i++) {
		for(j=0; j<v; j++) {
			if(tGraph[i][j]==1)
				A[i][j]=1;
			else A[i][j]=1000;
			if(i==j)  // zero out the diagonal
				A[i][j]=0;
		}
	}

	for(i=0; i<v; i++) {
		Dist[i]=1000;
		Tree[i]=start;
		Open[i]=1;
	}

	at=start;
	Open[start]=0;
	Dist[start]=0;
	e=0;

	while(e<(v-1)) {
		for(i=0; i<v; i++) {
			if(Open[i]==1) {
				d=Dist[at] + A[at][i];
				if(d<Dist[i]) {
					Dist[i]=d;
					Tree[i]=at;
				} //if
			} // if
		} // for
		min=1000;
		for(i=0; i<v; i++) {
			if((Dist[i]<min) && (Open[i]==1)) {
				min=Dist[i];
				at=i;
			} // if
		} // for
		e++;
		From[e]=Tree[at];
		To[e]=at;
		Open[at]=0;
	} // while

	if((fp=fopen("minpath.txt","w"))!=NULL) {
		fprintf(fp, "From To Distance\n");
		for(i=0; i<v; i++)
			fprintf(fp, "%d    %d    %d\n", From[i], To[i], Dist[To[i]]);
		fclose(fp);
	} //if

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void TerritoryClicked(UINT territory) {

	
	if(currentRound == Movement) {
		if(sTerr == 99) { // source not chosen yet
			sTerr = territory;
		}
		else if(dTerr == 99) {
			dTerr = territory;
			// Fill the SectorList array
			UINT i=0;
			while(Territories[dTerr].sectors[i]!=99) {
				SectorList.Sary[i] =
					Territories[dTerr].sectors[i];
				i++;
			} // while
			SectorList.nSectors = i;
		} // else 
	} // if

	DrawView();

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void DisplayTroops() {

	UINT i,j,k,x,y;  

	// Note:  need to add support to display troops at the polar sink

	for(i=0; i<NUMTERRITORIES; i++) {
		for(j=0; j<6; j++) {
			if(Territories[i].troops[j].qty > 0) {  // Troops are here
				k=0;
				while((Territories[i].sectors[k] !=99) && (k<6)) {
					if(Territories[i].sectors[k] == Territories[i].troops[j].sector) {
						x = Territories[i].coords[k].x;
						y = Territories[i].coords[k].y;
					}
					k++;
				}
				if(k>5) // error!!
					return; 
				if(Territories[i].troops[j].faction == Atreides) {
					pDirDraw->BlitImage(&CPoint(x,y), sundrySurfaceNum,
						&CRect(1,97,18,114));
				}
				if(Territories[i].troops[j].faction == Harkonnen) {
					pDirDraw->BlitImage(&CPoint(x,y), sundrySurfaceNum,
						&CRect(1,116,18,133));
				}
			} // if
		
		} // inner for
	} // Outer for

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void HandleMoveCancelButton() {

	sTerr = 99;
	dTerr = 99;
	mTroops = 0;
	SectorList.Selected = 99;
	moveLines = 1;
	strcpy(moveMessages[0],"move parameters reset\0");
	DrawView();
}


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void HandleMoveButton() {

	moveLines = 0;
	// Here is lots of checking for movement constraint violations
	if(mTroops == 0) { 
		moveLines = 3;
		strcpy(moveMessages[0],"movement error\0");
		strcpy(moveMessages[1],"              \0");
		strcpy(moveMessages[2],"  you must move at least one troop\0");
		DrawView();
		return;
	}
	if(sTerr == 99) {
		moveLines = 3;
		strcpy(moveMessages[0],"movement error\0");
		strcpy(moveMessages[1],"              \0");
		strcpy(moveMessages[2],"  you need to select a source territory\0");
		DrawView();
		return;
	}
	if(dTerr == 99) {
		moveLines = 3;
		strcpy(moveMessages[0],"movement error\0");
		strcpy(moveMessages[1],"              \0");
		strcpy(moveMessages[2],"  you need to select a destination\0");
		DrawView();
		return;
	}
	if(groundMoveDone && transportMoveDone) {
		if(sTerr == 99) 
		moveLines = 3;
		strcpy(moveMessages[0],"movement error\0");
		strcpy(moveMessages[1],"              \0");
		strcpy(moveMessages[2],"  only one ground and one space move allowed\0");
		DrawView();
		return;
	}
	if(transportMoveDone && (sTerr == 50)) {
		if(sTerr == 99) 
		moveLines = 3;
		strcpy(moveMessages[0],"movement error\0");
		strcpy(moveMessages[1],"              \0");
		strcpy(moveMessages[2],"  only one space move allowed\0");
		DrawView();
		return;
	}
	if((sTerr == 50) && (!transportMoveDone) && (player.pSpice < mTroops)) {
		if(sTerr == 99) 
		moveLines = 3;
		strcpy(moveMessages[0],"movement error\0");
		strcpy(moveMessages[1],"              \0");
		strcpy(moveMessages[2],"  not enough spice to ship your men\0");
		DrawView();
		return;
	}
	if(sTerr == dTerr) {
		moveLines = 3;
		strcpy(moveMessages[0],"movement error\0");
		strcpy(moveMessages[1],"              \0");
		strcpy(moveMessages[2],"  the source and destination are the same\0");
		DrawView();
		return;
	}
	if(!ValidateMove(player.OwnOrnithopters, sTerr, dTerr)) {
		moveLines = 3;
		strcpy(moveMessages[0],"movement error\0");
		strcpy(moveMessages[1],"              \0");
		strcpy(moveMessages[2],"  the distance is too great, atreides\0");
		DrawView();
		return;
	}
	if(sTerr == 50) {
		if(player.pTroopsReserved >= mTroops) {
			FillDest();
		}
		else {
			moveLines = 3;
			strcpy(moveMessages[0],"movement error\0");
			strcpy(moveMessages[1],"              \0");
			strcpy(moveMessages[2],"  not enough reserve troops\0");
			DrawView();
			return;
		} //else
	}
	else {
		FillDest();
	}
	
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void FillDest() {

	UINT i=0;

	while(i<5) {
		if(Territories[dTerr].troops[i].qty>0) {
			// troops are already here
			if(Territories[dTerr].troops[i].sector == SectorList.Selected) {
				if(Territories[dTerr].troops[i].faction == player.pFaction) { // are they the player's?
					Territories[dTerr].troops[i].qty += mTroops;
					CleanSource();
					break;
				} // if
				else {
					if(i<5) { // they are the computer's troops, so put ours in the next slot
						Territories[dTerr].troops[i+1].faction = player.pFaction;
						Territories[dTerr].troops[i+1].qty = mTroops;
						Territories[dTerr].troops[i+1].sector = SectorList.Selected;
						CleanSource();
						break;
					} // if
				} //else
			} // if
			else {
				if(i<5) { // they are the computer's troops, so put ours in the next slot
					Territories[dTerr].troops[i+1].faction = player.pFaction;
					Territories[dTerr].troops[i+1].qty = mTroops;
					Territories[dTerr].troops[i+1].sector = SectorList.Selected;
					CleanSource();
					break;
				} // if
			} // else
		} // if
		else { // we're all alone here
			if(i<5) { 
				Territories[dTerr].troops[i].faction = player.pFaction;
				Territories[dTerr].troops[i].qty = mTroops;
				Territories[dTerr].troops[i].sector = SectorList.Selected;
				CleanSource();
				break;
			} // if
		} // else
		i++;
	} // while

}


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void CleanSource() {

	UINT j=0;

	if(sTerr!=50) { // make sure it's not an offplanet move
		while(j<6) {
			if((Territories[sTerr].troops[j].qty > 0) &&
				(Territories[sTerr].troops[j].faction == player.pFaction))
				Territories[sTerr].troops[j].qty -= mTroops;
			j++;
		} // while
	} // if
	else {
		player.pTroopsReserved -= mTroops;
	} // else
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void SectorChosen(UINT s) {

	if(dTerr == 99)	// exit if destination not chosen yet
		return;

	switch (s) {
	case 0:
		SectorList.Selected = SectorList.Sary[0];
		break;
	case 1:
		if(SectorList.nSectors > 1)
			SectorList.Selected = SectorList.Sary[1];
		break;
	case 2:
		if(SectorList.nSectors > 2)
			SectorList.Selected = SectorList.Sary[2];
		break;
	case 3:
		if(SectorList.nSectors > 3)
			SectorList.Selected = SectorList.Sary[3];
		break;
	case 4:
		if(SectorList.nSectors > 4)
			SectorList.Selected = SectorList.Sary[4];
		break;
	}	
	DrawView();
}

