/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Cantarell:size=13", "JoyPixels:pixelsize=12:antialias=true:autohint:true" };
static const char dmenufont[]       = "Cantarell:size=15";

#include "/home/mamba/.cache/wal/colors-wal-dwm.h"
#include <X11/XF86keysym.h>

/* tagging */
static const char *tags[] = { ">_", "www", "IVI", "o-o", "BD", ":L" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]     = { "dmenu_run", "-l", "7", "-g", "2", "-h", "40", "-bw", "5", "-fn", dmenufont, "-p", "Executar: ", "-sb", sel_bg, "-sf", sel_fg, NULL };
static const char *clipmenucmd[]  = { "clipmenu", "-l", "7", "-g", "2", "-h", "40", "-bw", "5", "-i", "-fn", dmenufont, "-sb", sel_bg, "-sf", sel_fg, NULL };
static const char *termcmd[]      = { "st", NULL };
static const char *upvol[]        = { "pactl", "set-sink-volume", "0", "+5%",     NULL };
static const char *downvol[]      = { "pactl", "set-sink-volume", "0", "-5%",     NULL };
static const char *mutevol[]      = { "pactl", "set-sink-mute",   "0", "toggle",  NULL };
static const char *music[]        = { "dev.alextren.Spot", NULL };
static const char *playpause[]    = { "playerctl", "play-pause", NULL };
static const char *next[]         = { "playerctl", "next", NULL };
static const char *previous[]     = { "playerctl", "previous", NULL };
static const char *stop[]         = { "playerctl", "stop", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,                       spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return,                  spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_v,                       spawn,          {.v = clipmenucmd } },
	{ MODKEY,                       XK_b,                       togglebar,      {0} },
	{ MODKEY,                       XK_Right,                   focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_Left,                    focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_Up,                      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Down,                    incnmaster,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_Left,                    setmfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_Right,                   setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return,                  zoom,           {0} },
	{ MODKEY,                       XK_Tab,                     view,           {0} },
	{ MODKEY,                       XK_q,                       killclient,     {0} },
	{ MODKEY,                       XK_t,                       setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,                       setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,                       setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,                   setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,                   togglefloating, {0} },
	{ MODKEY,                       XK_0,                       view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                       tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,                   focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,                  focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,                   tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,                  tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_q,                       quit,           {0} },
	{ 0,                            XF86XK_AudioLowerVolume,    spawn,          {.v = downvol } },
	{ 0,                            XF86XK_AudioMute,           spawn,          {.v = mutevol } },
	{ 0,                            XF86XK_AudioRaiseVolume,    spawn,          {.v = upvol } },
	{ 0,                            XF86XK_Tools,               spawn,          {.v = music } },
	{ 0,                            XF86XK_AudioNext,           spawn,          {.v = next } },
	{ 0,                            XF86XK_AudioPlay,           spawn,          {.v = playpause } },
	{ 0,                            XF86XK_AudioPrev,           spawn,          {.v = previous } },
	{ 0,                            XF86XK_AudioStop,           spawn,          {.v = stop } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

