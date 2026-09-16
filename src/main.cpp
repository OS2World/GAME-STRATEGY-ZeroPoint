// main.cpp -- ZeroPoint SDL2 port for ArcaOS
// Replaces the V GUI library (wlnapp.cpp, wlncmdw.cpp, wlncnv.cpp, all dialogs)

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>

#include "poly.h"
#include "pwalk.h"
#include "wlnopt.h"
#include "wlndefs.h"

// ---------------------------------------------------------------------------
// Color palette (index 0 = black/erase, 1-18 = rainbow)  [from wlncnv.cpp]
// ---------------------------------------------------------------------------
static SDL_Color gColors[nSC] = {
    {  0,   0,   0, 255},   // 0  black (background / erase)
    {254,   0,   0, 255},   // 1  red
    {254,  88,   0, 255},   // 2
    {254, 176,   0, 255},   // 3
    {254, 254,   0, 255},   // 4  yellow
    {176, 254,   0, 255},   // 5
    { 88, 254,   0, 255},   // 6
    {  0, 254,   0, 255},   // 7  green
    {  0, 254,  88, 255},   // 8
    {  0, 254, 176, 255},   // 9
    {  0, 254, 254, 255},   // 10 cyan
    {  0, 176, 254, 255},   // 11
    {  0,  88, 254, 255},   // 12
    {  0,   0, 254, 255},   // 13 blue
    { 88,   0, 254, 255},   // 14
    {176,   0, 254, 255},   // 15
    {254,   0, 254, 255},   // 16 magenta
    {254,   0, 176, 255},   // 17
    {254,   0,  88, 255},   // 18
};

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
static SDL_Window*   gWin  = NULL;
static SDL_Renderer* gRen  = NULL;
static TTF_Font*     gFont = NULL;

static polyWalk* gWalk      = NULL;
static int   gLevel         = 1;
static int   gLives         = 5;
static long  gScore         = 0;
static long  gBonus         = 0;
static long  gCounter       = 0;
static int   gStepping      = 0;
static int   gInPractice    = 0;
static bool  gQuit          = false;
static int   gStepDelay     = 100;

static genArray<saved>  gHighscores;
static genArray<option> gOptions;

#define STATUS_H  22
#define MENU_H    22

// ---------------------------------------------------------------------------
// Embedded 8x8 bitmap font (public domain, IBM CP437 glyphs 32-127)
// ---------------------------------------------------------------------------
static const Uint8 font8x8[96][8] = {
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, // 32 space
{0x18,0x3C,0x3C,0x18,0x18,0x00,0x18,0x00}, // 33 !
{0x6C,0x6C,0x24,0x00,0x00,0x00,0x00,0x00}, // 34 "
{0x6C,0x6C,0xFE,0x6C,0xFE,0x6C,0x6C,0x00}, // 35 #
{0x18,0x3E,0x60,0x3C,0x06,0x7C,0x18,0x00}, // 36 $
{0x00,0xC6,0xCC,0x18,0x30,0x66,0xC6,0x00}, // 37 %
{0x38,0x6C,0x38,0x76,0xDC,0xCC,0x76,0x00}, // 38 &
{0x18,0x18,0x30,0x00,0x00,0x00,0x00,0x00}, // 39 '
{0x0C,0x18,0x30,0x30,0x30,0x18,0x0C,0x00}, // 40 (
{0x30,0x18,0x0C,0x0C,0x0C,0x18,0x30,0x00}, // 41 )
{0x00,0x66,0x3C,0xFF,0x3C,0x66,0x00,0x00}, // 42 *
{0x00,0x18,0x18,0x7E,0x18,0x18,0x00,0x00}, // 43 +
{0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x30}, // 44 ,
{0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00}, // 45 -
{0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00}, // 46 .
{0x06,0x0C,0x18,0x30,0x60,0xC0,0x80,0x00}, // 47 /
{0x3C,0x66,0x6E,0x76,0x66,0x66,0x3C,0x00}, // 48 0
{0x18,0x38,0x18,0x18,0x18,0x18,0x7E,0x00}, // 49 1
{0x3C,0x66,0x06,0x1C,0x30,0x66,0x7E,0x00}, // 50 2
{0x3C,0x66,0x06,0x1C,0x06,0x66,0x3C,0x00}, // 51 3
{0x0E,0x1E,0x36,0x66,0x7F,0x06,0x06,0x00}, // 52 4
{0x7E,0x60,0x7C,0x06,0x06,0x66,0x3C,0x00}, // 53 5
{0x1C,0x30,0x60,0x7C,0x66,0x66,0x3C,0x00}, // 54 6
{0x7E,0x06,0x0C,0x18,0x30,0x30,0x30,0x00}, // 55 7
{0x3C,0x66,0x66,0x3C,0x66,0x66,0x3C,0x00}, // 56 8
{0x3C,0x66,0x66,0x3E,0x06,0x0C,0x38,0x00}, // 57 9
{0x00,0x18,0x18,0x00,0x18,0x18,0x00,0x00}, // 58 :
{0x00,0x18,0x18,0x00,0x18,0x18,0x30,0x00}, // 59 ;
{0x0C,0x18,0x30,0x60,0x30,0x18,0x0C,0x00}, // 60 <
{0x00,0x00,0x7E,0x00,0x7E,0x00,0x00,0x00}, // 61 =
{0x60,0x30,0x18,0x0C,0x18,0x30,0x60,0x00}, // 62 >
{0x3C,0x66,0x06,0x1C,0x18,0x00,0x18,0x00}, // 63 ?
{0x3E,0x63,0x6F,0x69,0x6F,0x60,0x3E,0x00}, // 64 @
{0x18,0x3C,0x66,0x7E,0x66,0x66,0x66,0x00}, // 65 A
{0x7C,0x66,0x66,0x7C,0x66,0x66,0x7C,0x00}, // 66 B
{0x3C,0x66,0x60,0x60,0x60,0x66,0x3C,0x00}, // 67 C
{0x78,0x6C,0x66,0x66,0x66,0x6C,0x78,0x00}, // 68 D
{0x7E,0x60,0x60,0x7C,0x60,0x60,0x7E,0x00}, // 69 E
{0x7E,0x60,0x60,0x7C,0x60,0x60,0x60,0x00}, // 70 F
{0x3C,0x66,0x60,0x6E,0x66,0x66,0x3C,0x00}, // 71 G
{0x66,0x66,0x66,0x7E,0x66,0x66,0x66,0x00}, // 72 H
{0x3C,0x18,0x18,0x18,0x18,0x18,0x3C,0x00}, // 73 I
{0x1E,0x06,0x06,0x06,0x66,0x66,0x3C,0x00}, // 74 J
{0x66,0x6C,0x78,0x70,0x78,0x6C,0x66,0x00}, // 75 K
{0x60,0x60,0x60,0x60,0x60,0x60,0x7E,0x00}, // 76 L
{0x63,0x77,0x7F,0x6B,0x63,0x63,0x63,0x00}, // 77 M
{0x66,0x76,0x7E,0x7E,0x6E,0x66,0x66,0x00}, // 78 N
{0x3C,0x66,0x66,0x66,0x66,0x66,0x3C,0x00}, // 79 O
{0x7C,0x66,0x66,0x7C,0x60,0x60,0x60,0x00}, // 80 P
{0x3C,0x66,0x66,0x66,0x6E,0x3C,0x0E,0x00}, // 81 Q
{0x7C,0x66,0x66,0x7C,0x78,0x6C,0x66,0x00}, // 82 R
{0x3C,0x66,0x60,0x3C,0x06,0x66,0x3C,0x00}, // 83 S
{0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x00}, // 84 T
{0x66,0x66,0x66,0x66,0x66,0x66,0x3C,0x00}, // 85 U
{0x66,0x66,0x66,0x66,0x66,0x3C,0x18,0x00}, // 86 V
{0x63,0x63,0x63,0x6B,0x7F,0x77,0x63,0x00}, // 87 W
{0x66,0x66,0x3C,0x18,0x3C,0x66,0x66,0x00}, // 88 X
{0x66,0x66,0x66,0x3C,0x18,0x18,0x18,0x00}, // 89 Y
{0x7E,0x06,0x0C,0x18,0x30,0x60,0x7E,0x00}, // 90 Z
{0x3C,0x30,0x30,0x30,0x30,0x30,0x3C,0x00}, // 91 [
{0xC0,0x60,0x30,0x18,0x0C,0x06,0x02,0x00}, // 92 backslash
{0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,0x3C,0x00}, // 93 ]
{0x10,0x38,0x6C,0x00,0x00,0x00,0x00,0x00}, // 94 ^
{0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00}, // 95 _
{0x18,0x18,0x0C,0x00,0x00,0x00,0x00,0x00}, // 96 `
{0x00,0x00,0x3C,0x06,0x3E,0x66,0x3E,0x00}, // 97  a
{0x60,0x60,0x7C,0x66,0x66,0x66,0x7C,0x00}, // 98  b
{0x00,0x00,0x3C,0x60,0x60,0x66,0x3C,0x00}, // 99  c
{0x06,0x06,0x3E,0x66,0x66,0x66,0x3E,0x00}, // 100 d
{0x00,0x00,0x3C,0x66,0x7E,0x60,0x3C,0x00}, // 101 e
{0x1C,0x30,0x30,0x7C,0x30,0x30,0x30,0x00}, // 102 f
{0x00,0x00,0x3E,0x66,0x66,0x3E,0x06,0x3C}, // 103 g
{0x60,0x60,0x7C,0x66,0x66,0x66,0x66,0x00}, // 104 h
{0x18,0x00,0x38,0x18,0x18,0x18,0x3C,0x00}, // 105 i
{0x06,0x00,0x06,0x06,0x06,0x06,0x3C,0x00}, // 106 j
{0x60,0x60,0x66,0x6C,0x78,0x6C,0x66,0x00}, // 107 k
{0x38,0x18,0x18,0x18,0x18,0x18,0x3C,0x00}, // 108 l
{0x00,0x00,0x66,0x7F,0x7F,0x6B,0x63,0x00}, // 109 m
{0x00,0x00,0x7C,0x66,0x66,0x66,0x66,0x00}, // 110 n
{0x00,0x00,0x3C,0x66,0x66,0x66,0x3C,0x00}, // 111 o
{0x00,0x00,0x7C,0x66,0x66,0x7C,0x60,0x60}, // 112 p
{0x00,0x00,0x3E,0x66,0x66,0x3E,0x06,0x06}, // 113 q
{0x00,0x00,0x6C,0x76,0x60,0x60,0x60,0x00}, // 114 r
{0x00,0x00,0x3C,0x60,0x3C,0x06,0x7C,0x00}, // 115 s
{0x30,0x30,0x7C,0x30,0x30,0x30,0x1C,0x00}, // 116 t
{0x00,0x00,0x66,0x66,0x66,0x66,0x3E,0x00}, // 117 u
{0x00,0x00,0x66,0x66,0x66,0x3C,0x18,0x00}, // 118 v
{0x00,0x00,0x63,0x6B,0x7F,0x3E,0x36,0x00}, // 119 w
{0x00,0x00,0x66,0x3C,0x18,0x3C,0x66,0x00}, // 120 x
{0x00,0x00,0x66,0x66,0x66,0x3E,0x06,0x3C}, // 121 y
{0x00,0x00,0x7E,0x0C,0x18,0x30,0x7E,0x00}, // 122 z
{0x0E,0x18,0x18,0x70,0x18,0x18,0x0E,0x00}, // 123 {
{0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x00}, // 124 |
{0x70,0x18,0x18,0x0E,0x18,0x18,0x70,0x00}, // 125 }
{0x76,0xDC,0x00,0x00,0x00,0x00,0x00,0x00}, // 126 ~
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, // 127
};

static void blit_char(int cx, int cy, char ch, Uint8 r, Uint8 g, Uint8 b)
{
    int idx = (unsigned char)ch - 32;
    if (idx < 0 || idx >= 96) idx = 0;
    const Uint8* row = font8x8[idx];
    SDL_SetRenderDrawColor(gRen, r, g, b, 255);
    for (int py = 0; py < 8; py++) {
        Uint8 bits = row[py];
        for (int px = 0; px < 8; px++) {
            if (bits & (0x80 >> px))
                SDL_RenderDrawPoint(gRen, cx + px, cy + py);
        }
    }
}

static void blit_str(int x, int y, const char* s, Uint8 r, Uint8 g, Uint8 b)
{
    for (; *s; s++, x += 8)
        blit_char(x, y, *s, r, g, b);
}

// ---------------------------------------------------------------------------
// Menu system (drawn in SDL, no PM required)
// ---------------------------------------------------------------------------
static int gOpenMenu = -1;  // -1=none open

struct MenuItem { const char* label; SDL_Keycode key; bool sep; const char* shortcut; };
struct MenuDef  { const char* name; int hx, hw; MenuItem items[8]; int nItems; };

static MenuDef gMenus[3] = {
    {"Game", 0, 0, {
        {"New Game",    SDLK_n,       false, "Ctrl+N"},
        {"",           SDLK_UNKNOWN,  true,  ""},
        {"High Scores", SDLK_h,      false,  "H"},
        {"",           SDLK_UNKNOWN,  true,  ""},
        {"Pause",      SDLK_p,        false, "P"},
        {"Restart",    SDLK_r,        false, "R"},
        {"",           SDLK_UNKNOWN,  true,  ""},
        {"Quit",       SDLK_ESCAPE,  false,  "Ctrl+X"},
    }, 8},
    {"Options", 0, 0, {
        {"Keys Reference", SDLK_F1,  false,  "F1"},
    }, 1},
    {"Help", 0, 0, {
        {"Instructions", SDLK_F1,    false,  "F1"},
        {"",           SDLK_UNKNOWN,  true,  ""},
        {"About",      SDLK_F2,       false,  ""},
    }, 3},
};
#define MITEM_H  16
#define MITEM_W  180

// ---------------------------------------------------------------------------
// Forward declarations
// ---------------------------------------------------------------------------
static void zpNextLevel(bool showLevelMsg);
static int  zpLoseALife();
static bool zpHighScrQ();
static int  doWriterc();
static void redrawAll();
static void drawStatus();
static void drawMenuBar();
static long setStart();

static void setGameViewport()
{
    SDL_Rect vp = {0, MENU_H, optInts[MaxX], optInts[MaxY]};
    SDL_RenderSetViewport(gRen, &vp);
}
static void clearViewport()
{
    SDL_RenderSetViewport(gRen, NULL);
}
static void zpPresent()
{
    drawMenuBar();
    drawStatus();
    SDL_RenderPresent(gRen);
}

// ---------------------------------------------------------------------------
// Native OS/2 PM message boxes (no font required)
// ---------------------------------------------------------------------------

static void zpNativeMsg(const char* title, const char* msg)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, title, msg, gWin);
}

static int zpNativeAsk(const char* title, const char* msg)
{
    SDL_MessageBoxButtonData buttons[] = {
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Yes" },
        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "No"  }
    };
    SDL_MessageBoxData mbd = {
        SDL_MESSAGEBOX_WARNING, gWin, title, msg, 2, buttons, NULL
    };
    int r = 0;
    SDL_ShowMessageBox(&mbd, &r);
    return r;
}

// ---------------------------------------------------------------------------
// Text rendering helpers
// ---------------------------------------------------------------------------

static void renderText(const char* text, SDL_Color color, int x, int y)
{
    if (!gFont || !text || !text[0]) return;
    SDL_Surface* s = TTF_RenderText_Solid(gFont, text, color);
    if (!s) return;
    SDL_Texture* t = SDL_CreateTextureFromSurface(gRen, s);
    SDL_Rect dst = {x, y, s->w, s->h};
    SDL_RenderCopy(gRen, t, NULL, &dst);
    SDL_DestroyTexture(t);
    SDL_FreeSurface(s);
}

static int textW(const char* text)
{
    if (!gFont || !text) return (int)strlen(text) * 8;
    int w = 0;
    TTF_SizeText(gFont, text, &w, NULL);
    return w;
}

static int lineH()
{
    return gFont ? TTF_FontHeight(gFont) + 4 : 18;
}

// Draw a semi-transparent overlay over the game area
static void drawOverlayRect()
{
    SDL_SetRenderDrawBlendMode(gRen, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(gRen, 0, 0, 0, 175);
    SDL_Rect full = {0, MENU_H, optInts[MaxX], optInts[MaxY]};
    SDL_RenderFillRect(gRen, &full);
    SDL_SetRenderDrawBlendMode(gRen, SDL_BLENDMODE_NONE);
}


// Text-input dialog for high score name entry
// Returns true if user pressed Enter, false for Escape/quit
static bool zpGetName(const char* prompt, char* buf, int maxLen)
{
    buf[0] = '\0';
    int bufLen = 0;
    bool done = false, ok = false;

    SDL_StartTextInput();

    SDL_Color white  = {255, 255, 200, 255};
    SDL_Color yellow = {255, 255,   0, 255};
    SDL_Color gray   = {140, 140, 140, 255};

    SDL_Event e;
    while (!done) {
        drawOverlayRect();
        int lh = lineH();
        int y  = optInts[MaxY] / 2 - lh * 2;

        renderText(prompt, white, (optInts[MaxX] - textW(prompt)) / 2, y);
        y += lh + 4;

        {
            std::string inp = "> ";
            inp += buf;
            inp += "_";
            renderText(inp.c_str(), yellow,
                       (optInts[MaxX] - textW(inp.c_str())) / 2, y);
        }
        {
            const char* h = "[ Enter to confirm   Esc to skip ]";
            renderText(h, gray, (optInts[MaxX] - textW(h)) / 2, y + lh + 8);
        }
        SDL_RenderPresent(gRen);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) { done = true; break; }
            if (e.type == SDL_KEYDOWN) {
                SDL_Keycode k = e.key.keysym.sym;
                if (k == SDLK_RETURN)            { ok = true; done = true; }
                else if (k == SDLK_ESCAPE)       { done = true; }
                else if (k == SDLK_BACKSPACE && bufLen > 0)
                    buf[--bufLen] = '\0';
            }
            if (e.type == SDL_TEXTINPUT) {
                int addLen = (int)strlen(e.text.text);
                if (bufLen + addLen < maxLen) {
                    strcat(buf, e.text.text);
                    bufLen += addLen;
                }
            }
        }
        SDL_Delay(16);
    }
    SDL_StopTextInput();
    return ok;
}

// ---------------------------------------------------------------------------
// Drawing
// ---------------------------------------------------------------------------

static void drawPoly(polygon& pg, const SDL_Color& c)
{
    int n = (int)pg.numSides() + 1;
    if (n < 2) return;
    vPoint* pts = pg.getPoints();
    SDL_Point sdlPts[512];
    if (n > 512) n = 512;
    for (int i = 0; i < n; i++) {
        sdlPts[i].x = pts[i].x;
        sdlPts[i].y = pts[i].y;
    }
    SDL_SetRenderDrawColor(gRen, c.r, c.g, c.b, 255);
    SDL_RenderDrawLines(gRen, sdlPts, n);
}

static void drawPolyColored(polygon& pg)
{
    int ci = pg.getColor();
    if (ci < 0 || ci >= nSC) ci = 0;
    drawPoly(pg, gColors[ci]);
}

static void redrawAll()
{
    setGameViewport();
    SDL_SetRenderDrawColor(gRen, 0, 0, 0, 255);
    SDL_Rect r = {0, 0, optInts[MaxX], optInts[MaxY]};
    SDL_RenderFillRect(gRen, &r);
    if (gWalk) {
        drawPolyColored(gWalk->backPoly());
        while (!gWalk->iterDone())
            drawPolyColored(gWalk->nextPoly());
        drawPolyColored(gWalk->frontPoly());
    }
    clearViewport();
}

static void drawStatus()
{
    int W = optInts[MaxX];
    int Y = MENU_H + optInts[MaxY];

    // Background: light grey like Windows status bar
    SDL_SetRenderDrawColor(gRen, 212, 208, 200, 255);
    SDL_Rect bg = {0, Y, W, STATUS_H};
    SDL_RenderFillRect(gRen, &bg);

    // Top sunken border (3D look)
    SDL_SetRenderDrawColor(gRen, 128, 128, 128, 255);
    SDL_RenderDrawLine(gRen, 0, Y,   W, Y);
    SDL_SetRenderDrawColor(gRen, 255, 255, 255, 255);
    SDL_RenderDrawLine(gRen, 0, Y+1, W, Y+1);

    // Text vertically centered in STATUS_H
    int ty = Y + (STATUS_H - 8) / 2;

    char buf[512];
    if (!gInPractice)
        snprintf(buf, sizeof(buf),
                 "Level: %d  Total:  %ld  This level:  %ld  Lives:  %d  Esc: Quit, P: Pause",
                 gLevel, gScore, gBonus < 0 ? 0L : gBonus, gLives < 0 ? 0 : gLives);
    else
        snprintf(buf, sizeof(buf),
                 "Level: %d  [Practice]  Lives:  %d  Esc: Quit, P: Pause",
                 gLevel, gLives < 0 ? 0 : gLives);

    blit_str(6, ty, buf, 0, 0, 0);
}

static void drawMenuBar()
{
    int W = optInts[MaxX];
    // Background: Windows classic menu bar grey
    SDL_SetRenderDrawColor(gRen, 212, 208, 200, 255);
    SDL_Rect bg = {0, 0, W, MENU_H};
    SDL_RenderFillRect(gRen, &bg);
    // Bottom border
    SDL_SetRenderDrawColor(gRen, 128, 128, 128, 255);
    SDL_RenderDrawLine(gRen, 0, MENU_H-1, W, MENU_H-1);

    int x = 2;
    for (int m = 0; m < 3; m++) {
        int tw = (int)strlen(gMenus[m].name) * 8;
        int padx = 6;
        gMenus[m].hx = x;
        gMenus[m].hw = tw + padx * 2;

        if (gOpenMenu == m) {
            SDL_SetRenderDrawColor(gRen, 0, 0, 128, 255);
            SDL_Rect hi = {x, 1, gMenus[m].hw, MENU_H - 2};
            SDL_RenderFillRect(gRen, &hi);
            blit_str(x + padx, (MENU_H - 8) / 2, gMenus[m].name, 255, 255, 255);
        } else {
            blit_str(x + padx, (MENU_H - 8) / 2, gMenus[m].name, 0, 0, 0);
        }
        x += gMenus[m].hw + 2;
    }

    // Draw open dropdown
    if (gOpenMenu >= 0) {
        MenuDef& md = gMenus[gOpenMenu];
        int dy = MENU_H;
        // Background
        SDL_SetRenderDrawColor(gRen, 212, 208, 200, 255);
        SDL_Rect mbg = {md.hx, dy, MITEM_W, md.nItems * MITEM_H};
        SDL_RenderFillRect(gRen, &mbg);
        // Border (sunken)
        SDL_SetRenderDrawColor(gRen, 128, 128, 128, 255);
        SDL_RenderDrawRect(gRen, &mbg);

        for (int i = 0; i < md.nItems; i++) {
            int iy = dy + i * MITEM_H;
            if (md.items[i].sep) {
                SDL_SetRenderDrawColor(gRen, 128, 128, 128, 255);
                SDL_RenderDrawLine(gRen, md.hx + 4, iy + MITEM_H/2,
                                         md.hx + MITEM_W - 4, iy + MITEM_H/2);
            } else {
                blit_str(md.hx + 8, iy + (MITEM_H - 8)/2,
                         md.items[i].label, 0, 0, 0);
                if (md.items[i].shortcut && md.items[i].shortcut[0]) {
                    int sw = (int)strlen(md.items[i].shortcut) * 8;
                    blit_str(md.hx + MITEM_W - sw - 6, iy + (MITEM_H - 8)/2,
                             md.items[i].shortcut, 80, 80, 80);
                }
            }
        }
    }
}

// ---------------------------------------------------------------------------
// Game logic
// ---------------------------------------------------------------------------

static long setStart()
{
    return (gLevel * 150L) + 850L + (200L * ((gLevel - 1) % 3));
}

static void zpRestartLevel()
{
    gCounter = 0;

    setGameViewport();
    SDL_SetRenderDrawColor(gRen, 0, 0, 0, 255);
    SDL_Rect r = {0, 0, optInts[MaxX], optInts[MaxY]};
    SDL_RenderFillRect(gRen, &r);
    clearViewport();

    int sides = (int)gWalk->tellNumSides();
    int delay = optInts[MaxDelay] - (((gLevel - 1) % 3) * 25);
    if (delay < 10) delay = 10;
    vPoint sz  = gWalk->tellSize();
    gBonus     = setStart();

    delete gWalk;
    gWalk = new polyWalk(sides, optInts[NPollies], optInts[Speedlim], delay, sz);
    gStepDelay = delay;
    gStepping  = 1;
    zpPresent();
}

static bool zpHighScrQ()
{
    if (gScore <= 0) return false;

    int n = optInts[NHigh];

    // Find insertion position (sorted descending by score)
    int insertAt = n; // means "not in top-N"
    for (int i = 0; i < n; i++) {
        if ((int)gHighscores.tellSize() <= i || gScore > gHighscores[i].aScore) {
            insertAt = i;
            break;
        }
    }
    if (insertAt >= n) return false;

    // Shift entries from insertAt..n-2 down one slot (in reverse)
    for (int j = n - 2; j >= insertAt; j--) {
        snprintf(gHighscores[j+1].name, rcnamesz, "%s", gHighscores[j].name);
        gHighscores[j+1].aLevel = gHighscores[j].aLevel;
        gHighscores[j+1].aScore = gHighscores[j].aScore;
    }

    // Get name from player
    char nameInput[rcnamesz];
    nameInput[0] = '\0';
    zpGetName("High Score!  Enter your name:", nameInput, rcnamesz - 1);
    // Sanitize rc delimiters
    for (int j = 0; nameInput[j]; j++)
        if (nameInput[j] == '~' || nameInput[j] == '=') nameInput[j] = '_';
    if (!nameInput[0]) { nameInput[0] = '-'; nameInput[1] = '\0'; }

    snprintf(gHighscores[insertAt].name, rcnamesz, "%s", nameInput);
    gHighscores[insertAt].aLevel = gLevel;
    gHighscores[insertAt].aScore = gScore;

    doWriterc();

    // Display high score table
    static char hdr[64];
    snprintf(hdr, sizeof(hdr), "--- High Scores ---");
    static char hlines[12][64];
    const char* msgs[14];
    int cnt = 0;
    msgs[cnt++] = hdr;

    int hn = (int)gHighscores.tellSize();
    if (hn > n) hn = n;
    for (int j = 0; j < hn && cnt < 12; j++) {
        if (gHighscores[j].aScore > 0) {
            snprintf(hlines[cnt-1], 64, "%2d.  %-16s  L%d  %ld",
                     j+1, gHighscores[j].name,
                     gHighscores[j].aLevel, gHighscores[j].aScore);
            msgs[cnt] = hlines[cnt-1];
            cnt++;
        }
    }
    // Build one string for native dialog
    std::string hsText;
    for (int j = 0; j < cnt; j++) { hsText += msgs[j]; hsText += "\n"; }
    zpNativeMsg("High Scores", hsText.c_str());
    redrawAll();
    zpPresent();
    return true;
}

static int zpLoseALife()
{
    --gLives;
    zpPresent();

    if (gLives < 0) {
        zpHighScrQ();
        char msg[128];
        snprintf(msg, sizeof(msg), "Game Over!\n\nFinal score: %ld\n\nStart a new game?", gScore);
        int r = zpNativeAsk("Game Over", msg);
        if (r == 1) {
            gLevel = optInts[DefStartLevel];
            gLives = optInts[StartLives];
            gScore = 0;
            redrawAll();
            zpNextLevel(false);
        } else {
            gQuit = true;
        }
    } else {
        char msg[80];
        if (gLives == 0)
            snprintf(msg, sizeof(msg), "Last life!  Make it count.");
        else
            snprintf(msg, sizeof(msg), "Lost a life!  %d left.", gLives);
        zpNativeMsg("Oops", msg);
        redrawAll();
        zpRestartLevel();
    }
    return gLives;
}

static void zpNextLevel(bool showLevelMsg)
{
    gStepping = 0;

    if (showLevelMsg && gLevel > 0 && !gInPractice) {
        gScore += gBonus;
        char msg[160];
        snprintf(msg, sizeof(msg),
                 "Level %d complete!\n\nScore: %ld  (bonus: %ld)",
                 gLevel, gScore, gBonus);
        zpNativeMsg("Level Done", msg);
        redrawAll();
    }

    gLevel++;
    gBonus   = setStart();
    gCounter = 0;

    int sides = ((gLevel - 1) / 3) + 2;
    int delay = optInts[MaxDelay] - (((gLevel - 1) % 3) * 25);
    if (delay < 10) delay = 10;
    gStepDelay = delay;

    vPoint sz = { optInts[MaxX], optInts[MaxY] };
    if (gWalk) sz = gWalk->tellSize();
    delete gWalk;
    gWalk = new polyWalk(sides, optInts[NPollies], optInts[Speedlim], delay, sz);

    setGameViewport();
    SDL_SetRenderDrawColor(gRen, 0, 0, 0, 255);
    SDL_Rect r = {0, 0, optInts[MaxX], optInts[MaxY]};
    SDL_RenderFillRect(gRen, &r);
    clearViewport();

    zpPresent();
    gStepping = 1;
}

static void doStep()
{
    if (!gWalk || !gStepping) return;

    setGameViewport();
    drawPoly(gWalk->backPoly(), gColors[0]);

    long cIdx  = (gCounter / optInts[StepPClr]) % (long)(nSC - 1);
    int colorIx = (int)(cIdx + 1);
    gCounter++;
    if (gCounter > 200000L)
        gCounter = gCounter % (long)(nSC * optInts[StepPClr]);

    int anyVel = gWalk->step(colorIx);
    drawPoly(gWalk->frontPoly(), gColors[colorIx]);
    clearViewport();

    if (!(gCounter % 10) && gCounter >= 10 && !gInPractice) {
        gBonus -= 10;
        if (gBonus <= 0) {
            gStepping = 0;
            zpPresent();
            zpLoseALife();
            return;
        }
    }

    zpPresent();

    if (!anyVel) gWalk->closing(1);

    if (gWalk->allDone()) {
        setGameViewport();
        drawPoly(gWalk->frontPoly(), gColors[0]);
        clearViewport();
        zpPresent();
        gStepping = 0;
        zpNextLevel(true);
    }
}

// ---------------------------------------------------------------------------
// RC file  (zeropt.rc — INI-style with [CONFIG] and [HIGH] sections)
// ---------------------------------------------------------------------------

static int doWriterc()
{
    std::ofstream rc("zeropt.rc");
    if (!rc) return 1;

    rc << "# ZeroPoint configuration\n\n[CONFIG]\n";
    const char** list = wlnOpts::getList();
    for (int i = 0; list[i]; i++)
        rc << list[i] << "=" << optInts[i] << "\n";

    rc << "\n[HIGH]\n";
    int hn = (int)gHighscores.tellSize();
    for (int i = 0; i < hn; i++) {
        if (gHighscores[i].aScore > 0 && gHighscores[i].name[0])
            rc << "High~Name=" << gHighscores[i].name
               << "~Level=" << gHighscores[i].aLevel
               << "~Score=" << gHighscores[i].aScore << "\n";
    }
    return 0;
}

static int doReadrc()
{
    std::ifstream rc("zeropt.rc");
    if (!rc) return 0;

    std::string line;
    while (std::getline(rc, line)) {
        // Strip comment and trailing whitespace
        size_t cpos = line.find('#');
        if (cpos != std::string::npos) line.erase(cpos);
        while (!line.empty() &&
               (line.back() == '\r' || line.back() == ' ' || line.back() == '\t'))
            line.pop_back();
        if (line.empty()) continue;

        if (line.compare(0, 5, "High~") == 0) {
            // High~Name=xxx~Level=N~Score=N
            int hi = (int)gHighscores.tellSize();
            size_t pName  = line.find("Name=");
            size_t pLevel = line.find("~Level=");
            size_t pScore = line.find("~Score=");
            if (pName != std::string::npos && pLevel != std::string::npos) {
                std::string nm = line.substr(pName + 5, pLevel - (pName + 5));
                if (nm.size() >= (size_t)rcnamesz) nm.resize(rcnamesz - 1);
                snprintf(gHighscores[hi].name, rcnamesz, "%s", nm.c_str());
            }
            if (pLevel != std::string::npos)
                gHighscores[hi].aLevel = atoi(line.c_str() + pLevel + 7);
            if (pScore != std::string::npos)
                gHighscores[hi].aScore = atol(line.c_str() + pScore + 7);

        } else if (line.find('=') != std::string::npos && line[0] != '[') {
            // key=value (config options)
            size_t eq = line.find('=');
            std::string key = line.substr(0, eq);
            long val = atol(line.c_str() + eq + 1);
            int os = (int)gOptions.tellSize();
            snprintf(gOptions[os].name, rcnamesz, "%s", key.c_str());
            gOptions[os].value = val;
        }
    }
    return 0;
}

static void doParseOptions()
{
    const char** list = wlnOpts::getList();
    int*   mins = wlnOpts::getMins();
    int*   maxs = wlnOpts::getMaxs();
    int    n    = (int)gOptions.tellSize();

    for (int i = 0; list[i]; i++) {
        for (int j = 0; j < n; j++) {
            if (!strcmp(gOptions[j].name, list[i])) {
                int v = (int)gOptions[j].value;
                if (v < mins[i]) v = mins[i];
                if (v > maxs[i]) v = maxs[i];
                optInts[i] = v;
                break;
            }
        }
    }
}

// ---------------------------------------------------------------------------
// Welcome screen
// ---------------------------------------------------------------------------

static void showWelcome()
{
    zpNativeMsg("ZeroPoint",
        "Slow and stop the bouncing polygon using the arrow keys.\n"
        "\n"
        "Arrow keys      Adjust velocity\n"
        "P / Pause       Pause / Resume\n"
        "N               New game\n"
        "R               Restart level\n"
        "H               High scores\n"
        "Esc             Quit");
}

// ---------------------------------------------------------------------------
// Key handler
// ---------------------------------------------------------------------------

static void handleKey(SDL_Keycode k)
{
    if (!gWalk) return;

    switch (k) {
    case SDLK_UP:    case SDLK_KP_8:
        gWalk->getVels().yadd(-1); gStepping = 1; break;
    case SDLK_DOWN:  case SDLK_KP_2:
        gWalk->getVels().yadd( 1); gStepping = 1; break;
    case SDLK_RIGHT: case SDLK_KP_6:
        gWalk->getVels().xadd( 1); gStepping = 1; break;
    case SDLK_LEFT:  case SDLK_KP_4:
        gWalk->getVels().xadd(-1); gStepping = 1; break;
    case SDLK_KP_7:
        gWalk->getVels().xadd(-1); gWalk->getVels().yadd(-1); gStepping = 1; break;
    case SDLK_KP_9:
        gWalk->getVels().xadd( 1); gWalk->getVels().yadd(-1); gStepping = 1; break;
    case SDLK_KP_3:
        gWalk->getVels().xadd( 1); gWalk->getVels().yadd( 1); gStepping = 1; break;
    case SDLK_KP_1:
        gWalk->getVels().xadd(-1); gWalk->getVels().yadd( 1); gStepping = 1; break;

    case SDLK_p: case SDLK_PAUSE: {
        int was = gStepping;
        gStepping = 0;
        zpNativeMsg("Paused", "Game paused.\n\nPress OK to resume.");
        redrawAll();
        zpPresent();
        gStepping = was;
        break;
    }
    case SDLK_n: {
        int was = gStepping;
        gStepping = 0;
        if (zpNativeAsk("New Game", "Abandon current game and start over?") == 1) {
            zpHighScrQ();
            gLevel = optInts[DefStartLevel];
            gLives = optInts[StartLives];
            gScore = 0;
            redrawAll();
            zpNextLevel(false);
        } else {
            redrawAll();
            zpPresent();
            gStepping = was;
        }
        break;
    }
    case SDLK_r: {
        int was = gStepping;
        gStepping = 0;
        if (zpNativeAsk("Restart", "Restart current level?") == 1) {
            redrawAll();
            zpRestartLevel();
        } else {
            redrawAll();
            zpPresent();
            gStepping = was;
        }
        break;
    }
    case SDLK_h: {
        int was = gStepping;
        gStepping = 0;
        zpHighScrQ();
        {
            std::string hs = "High Scores\n\n";
            int n = optInts[NHigh];
            int cnt = 0;
            for (int i = 0; i < n && i < (int)gHighscores.tellSize(); i++) {
                if (gHighscores[i].aScore > 0) {
                    char line[80];
                    snprintf(line, sizeof(line), "%2d.  %-16s  L%d  %ld\n",
                             i+1, gHighscores[i].name,
                             gHighscores[i].aLevel, gHighscores[i].aScore);
                    hs += line;
                    cnt++;
                }
            }
            if (!cnt) hs += "(no scores yet)";
            zpNativeMsg("High Scores", hs.c_str());
        }
        redrawAll();
        zpPresent();
        gStepping = was;
        break;
    }
    case SDLK_ESCAPE: {
        int was = gStepping;
        gStepping = 0;
        if (zpNativeAsk("Quit", "Quit ZeroPoint?") == 1) {
            zpHighScrQ();
            gQuit = true;
        } else {
            redrawAll();
            zpPresent();
            gStepping = was;
        }
        break;
    }
    case SDLK_F1: {
        int was = gStepping;
        gStepping = 0;
        showWelcome();
        redrawAll();
        zpPresent();
        gStepping = was;
        break;
    }
    case SDLK_F2: {
        int was = gStepping;
        gStepping = 0;
        zpNativeMsg("About ZeroPoint",
            "ZeroPoint\n"
            "Version 0.9.5\n"
            "\n"
            "Original game by Enumerate Inc.\n"
            "Copyright (C) 1996, 1997, 1998\n"
            "\n"
            "ArcaOS SDL2 port");
        redrawAll();
        zpPresent();
        gStepping = was;
        break;
    }
    case SDLK_x:
        if (SDL_GetModState() & KMOD_CTRL)
            handleKey(SDLK_ESCAPE);
        break;
    default: break;
    }
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    (void)argc; (void)argv;

    doReadrc();
    doParseOptions();

    int W = optInts[MaxX];
    int H = optInts[MaxY] + STATUS_H + MENU_H;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() < 0) {
        fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    gWin = SDL_CreateWindow("ZeroPoint",
                            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            W, H,
                            SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    if (!gWin) {
        fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
        TTF_Quit(); SDL_Quit(); return 1;
    }

    gRen = SDL_CreateRenderer(gWin, -1,
                              SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!gRen)
        gRen = SDL_CreateRenderer(gWin, -1, SDL_RENDERER_SOFTWARE);
    if (!gRen) {
        fprintf(stderr, "SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(gWin); TTF_Quit(); SDL_Quit(); return 1;
    }

    // Try font paths in priority order (ArcaOS + common Linux paths)
    static const char* fontPaths[] = {
        "/@unixroot/usr/share/fonts/dejavu/DejaVuSans.ttf",
        "/@unixroot/usr/share/fonts/ttf-dejavu/DejaVuSans.ttf",
        "/@unixroot/usr/share/fonts/truetype/ttf-dejavu/DejaVuSans.ttf",
        "/@unixroot/usr/share/fonts/liberation/LiberationSans-Regular.ttf",
        "/@unixroot/usr/share/fonts/liberation-fonts/LiberationSans-Regular.ttf",
        "/@unixroot/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
        "/@unixroot/usr/share/fonts/freefont/FreeSans.ttf",
        "/@unixroot/usr/share/fonts/gnu-free/FreeSans.ttf",
        "/@unixroot/usr/share/fonts/open-sans/OpenSans-Regular.ttf",
        "/@unixroot/usr/share/fonts/truetype/freefont/FreeSans.ttf",
        "font.ttf",
        NULL
    };
    for (int i = 0; fontPaths[i] && !gFont; i++)
        gFont = TTF_OpenFont(fontPaths[i], 13);
    if (!gFont)
        fprintf(stderr, "Warning: no font loaded; text overlays disabled.\n");

    srand((unsigned int)time(NULL));

    // Initialize game state
    gLevel    = optInts[DefStartLevel];
    gLives    = optInts[StartLives];
    gScore    = 0;
    gBonus    = 0;
    gCounter  = 0;
    gStepping = 0;

    vPoint sz = { optInts[MaxX], optInts[MaxY] };
    gWalk = new polyWalk(2, optInts[NPollies], optInts[Speedlim],
                         optInts[MaxDelay], sz);

    setGameViewport();
    SDL_SetRenderDrawColor(gRen, 0, 0, 0, 255);
    SDL_RenderFillRect(gRen, NULL);
    clearViewport();
    drawMenuBar();
    drawStatus();
    SDL_RenderPresent(gRen);

    zpNextLevel(false);

    Uint32 lastStep = SDL_GetTicks();

    while (!gQuit) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                handleKey(SDLK_ESCAPE);
            } else if (e.type == SDL_KEYDOWN) {
                handleKey(e.key.keysym.sym);
            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int mx = e.button.x, my = e.button.y;
                if (my < MENU_H) {
                    // Click on menu bar header
                    int hit = -1;
                    for (int m = 0; m < 3; m++) {
                        if (mx >= gMenus[m].hx && mx < gMenus[m].hx + gMenus[m].hw)
                            { hit = m; break; }
                    }
                    gOpenMenu = (gOpenMenu == hit) ? -1 : hit;
                    redrawAll();
                    drawMenuBar();
                    drawStatus();
                    SDL_RenderPresent(gRen);
                } else if (gOpenMenu >= 0) {
                    MenuDef& md = gMenus[gOpenMenu];
                    int dy = MENU_H;
                    if (mx >= md.hx && mx < md.hx + MITEM_W && my >= dy) {
                        int idx = (my - dy) / MITEM_H;
                        if (idx >= 0 && idx < md.nItems && !md.items[idx].sep) {
                            SDL_Keycode k = md.items[idx].key;
                            gOpenMenu = -1;
                            redrawAll();
                            drawMenuBar();
                            drawStatus();
                            SDL_RenderPresent(gRen);
                            handleKey(k);
                        }
                    } else {
                        gOpenMenu = -1;
                        redrawAll();
                        drawMenuBar();
                        drawStatus();
                        SDL_RenderPresent(gRen);
                    }
                }
            } else if (e.type == SDL_WINDOWEVENT) {
                if (e.window.event == SDL_WINDOWEVENT_RESIZED ||
                    e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    int newW = 0, newH = 0;
                    SDL_GetWindowSize(gWin, &newW, &newH);
                    newH -= STATUS_H + MENU_H;
                    if (newW >= minRun && newH >= minRun && gWalk) {
                        int was = gStepping;
                        gStepping = 0;
                        optInts[MaxX] = newW;
                        optInts[MaxY] = newH;
                        vPoint mini = {0, 0}, maxi = {newW, newH};
                        gWalk->setSize(mini, maxi);
                        redrawAll();
                        drawMenuBar();
                        drawStatus();
                        SDL_RenderPresent(gRen);
                        gStepping = was;
                    }
                }
            }
        }

        if (gStepping && !gQuit) {
            Uint32 now = SDL_GetTicks();
            if (now - lastStep >= (Uint32)gStepDelay) {
                lastStep = now;
                doStep();
            }
        }

        SDL_Delay(1);
    }

    doWriterc();

    delete gWalk;
    if (gFont) TTF_CloseFont(gFont);
    TTF_Quit();
    SDL_DestroyRenderer(gRen);
    SDL_DestroyWindow(gWin);
    SDL_Quit();
    return 0;
}
