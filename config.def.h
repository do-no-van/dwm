/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx = 3;        /* border pixel of windows */
static const unsigned int snap     = 32;       /* snap pixel */
static const unsigned int gappx    = 7;        /* gap between windows */
static const int swallowfloating   = 0;        /* 1 means swallow floating windows by default */
static const int showbar           = 1;        /* 0 means no bar */
static const int topbar            = 1;        /* 0 means bottom bar */
static const char *fonts[]         = {
    "FiraCode Nerd Font:pixelsize=13:antialias=true:autohint=true",
    "JoyPixels:pixelsize=10:antialias=true:autohint=true"
};
static char normbgcolor[]          = "tmp_val";
static char normbordercolor[]      = "tmp_val";
static char normfgcolor[]          = "tmp_val";
static char selbordercolor[]       = "tmp_val";
static char selbgcolor[]           = "tmp_val";
static char unoccfgcolor[]         = "tmp_val";
static char *colors[][3] = {
       /*                fg            bg           border   */
       [SchemeNorm]  = { normfgcolor,  normbgcolor, normbordercolor },
       [SchemeSel]   = { normfgcolor,  selbgcolor,  selbordercolor  },
       [SchemeUnOcc] = { unoccfgcolor, normbgcolor, normbordercolor },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class         instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",        NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox",     NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "st-256color", NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,          NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
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

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-nb", normbgcolor, "-nf", unoccfgcolor, "-sb", selbordercolor, "-sf", normfgcolor, NULL };

#include "movestack.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ 0,         XF86XK_AudioRaiseVolume,      spawn,          SHCMD("amixer set Master 5%+") },
	{ 0,         XF86XK_AudioLowerVolume,      spawn,          SHCMD("amixer set Master 5%-") },
	{ 0,         XF86XK_AudioMute,             spawn,          SHCMD("amixer set Master toggle") },
	{ 0,         XF86XK_MonBrightnessUp,       spawn,          SHCMD("xbacklight -inc 15") },
	{ 0,         XF86XK_MonBrightnessDown,     spawn,          SHCMD("xbacklight -dec 15") },
	{ 0,                         XK_Print,     spawn,          SHCMD("cd ~/media; scrot --select") },
	{ MODKEY,                    XK_BackSpace, spawn,          SHCMD("powermenu") },
	{ MODKEY|ShiftMask,          XK_b,         spawn,          SHCMD("$BROWSER") },
	{ MODKEY|AltMask,            XK_b,         spawn,          SHCMD("$PRIVATE_BROWSER") },
	{ MODKEY|ShiftMask,          XK_c,         spawn,          SHCMD("code") },
	{ MODKEY,                    XK_Return,    spawn,          SHCMD("$TERMINAL") },
	{ MODKEY|ShiftMask,          XK_p,         spawn,          SHCMD("keepassxc") },
	{ MODKEY,                    XK_d,         spawn,          {.v = dmenucmd } },
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
	{ ClkStatusText,        0,              Button2,        spawn,          SHCMD("$TERMINAL") },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

