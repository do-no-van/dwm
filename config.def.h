/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx = 3;        /* border pixel of windows */
static const unsigned int snap     = 32;       /* snap pixel */
static const unsigned int gappx    = 7;        /* gap between windows */
static const int showbar           = 1;        /* 0 means no bar */
static const int topbar            = 1;        /* 0 means bottom bar */
static const char *fonts[]         = {
    "FiraCode Nerd Font:pixelsize=13:antialias=true:autohint=true",
    "JoyPixels:pixelsize=10:antialias=true:autohint=true"
};
static const char normfgcolor[]     = "#777777";
static const char normbgcolor[]     = "#222222";
static const char normbordercolor[] = "#444444";
static const char selfgcolor[]      = "#eeeeee";
static const char selbgcolor[]      = "#1965c1";
static const char selbordercolor[]  = "#0f94d2";

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

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
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY  Mod4Mask
#define AltMask Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

#include "movestack.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ 0,         XF86XK_AudioRaiseVolume,      spawn,          SHCMD("amixer set Master 5%+ && pkill -RTMIN+3 dwmblocks &") },
	{ 0,         XF86XK_AudioLowerVolume,      spawn,          SHCMD("amixer set Master 5%- && pkill -RTMIN+3 dwmblocks &") },
	{ 0,         XF86XK_AudioMute,             spawn,          SHCMD("amixer set Master toggle && pkill -RTMIN+3 dwmblocks &") },
	{ 0,         XF86XK_MonBrightnessUp,       spawn,          SHCMD("xbacklight -inc 15") },
	{ 0,         XF86XK_MonBrightnessDown,     spawn,          SHCMD("xbacklight -dec 15") },
	{ 0,                         XK_Print,     spawn,          SHCMD("cd ~/media; scrot --select") },
	{ MODKEY,                    XK_BackSpace, spawn,          SHCMD("power-dmenu") },
	{ MODKEY|ShiftMask,          XK_b,         spawn,          SHCMD("$BROWSER") },
	{ MODKEY|AltMask,            XK_b,         spawn,          SHCMD("$PRIVATE_BROWSER") },
	{ MODKEY|ShiftMask,          XK_s,         spawn,          SHCMD("steam") },
	{ MODKEY|AltMask,            XK_s,         spawn,          SHCMD("dmenu-steam") },
	{ MODKEY,                    XK_Return,    spawn,          SHCMD("$TERMINAL") },
	{ MODKEY|ShiftMask,          XK_p,         spawn,          SHCMD("keepassxc") },
	{ MODKEY,                    XK_d,         spawn,          SHCMD("dmenu_run") },
	{ MODKEY|ShiftMask,          XK_m,         spawn,          SHCMD("dmenu-mount") },
	{ MODKEY|AltMask,            XK_m,         spawn,          SHCMD("dmenu-unmount") },
	{ MODKEY,                    XK_b,         togglebar,      {0} },
	{ MODKEY,                    XK_j,         focusstack,     {.i = +1 } },
	{ MODKEY,                    XK_k,         focusstack,     {.i = -1 } },
	{ MODKEY,                    XK_i,         incnmaster,     {.i = +1 } },
	{ MODKEY,                    XK_p,         incnmaster,     {.i = -1 } },
	{ MODKEY,                    XK_h,         setmfact,       {.f = -0.05} },
	{ MODKEY,                    XK_l,         setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,          XK_j,         movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,          XK_k,         movestack,      {.i = -1 } },
	{ MODKEY|AltMask,            XK_h,         incrgaps,       {.i = +1 } },
	{ MODKEY|AltMask,            XK_l,         incrgaps,       {.i = -1 } },
	{ MODKEY|AltMask|ShiftMask,  XK_0,         defaultgaps,    {0} },
	{ MODKEY,                    XK_Tab,       zoom,           {0} },
	{ MODKEY|ShiftMask,          XK_Return,    view,           {0} },
	{ MODKEY,                    XK_q,         killclient,     {0} },
	{ MODKEY,                    XK_t,         setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                    XK_f,         setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                    XK_m,         setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                    XK_space,     setlayout,      {0} },
	{ MODKEY|ShiftMask,          XK_space,     togglefloating, {0} },
	{ MODKEY,                    XK_0,         view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,          XK_0,         tag,            {.ui = ~0 } },
	{ MODKEY,                    XK_comma,     focusmon,       {.i = -1 } },
	{ MODKEY,                    XK_period,    focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,          XK_comma,     tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,          XK_period,    tagmon,         {.i = +1 } },
	TAGKEYS(                     XK_1,                         0)
	TAGKEYS(                     XK_2,                         1)
	TAGKEYS(                     XK_3,                         2)
	TAGKEYS(                     XK_4,                         3)
	TAGKEYS(                     XK_5,                         4)
	TAGKEYS(                     XK_6,                         5)
	TAGKEYS(                     XK_7,                         6)
	TAGKEYS(                     XK_8,                         7)
	TAGKEYS(                     XK_9,                         8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigstatusbar,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigstatusbar,   {.i = 5} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

