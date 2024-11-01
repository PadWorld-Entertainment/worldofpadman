/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
// console.c

#include "client.h"

int g_console_field_width = 78;
static fontSize_t fontSize;

#define MAX_CONSOLE_WIDTH 120
#define NUM_CON_TIMES 4

#define CON_TEXTSIZE 32768
typedef struct {
	qboolean initialized;

	short text[CON_TEXTSIZE];
	int current; // line where next message will be printed
	int x;		 // offset in current line for next print
	int display; // bottom of console displays this line

	int linewidth;	// characters across screen
	int totallines; // total lines in console scrollback
	int viswidth;

	float xadjust; // for wide aspect screens

	float displayFrac; // aproaches finalFrac at scr_conspeed
	float finalFrac;   // 0.0 to 1.0 lines of console to display

	int vislines; // in scanlines

	int times[NUM_CON_TIMES]; // cls.realtime time the line was generated
							  // for transparent notify lines
	vec4_t color;
} console_t;

static console_t con;

static cvar_t *con_conspeed;
static cvar_t *con_autoclear;
static cvar_t *con_notifytime;
static cvar_t *con_scale;

#define DEFAULT_CONSOLE_WIDTH 78

/*
================
Con_ToggleConsole_f
================
*/
void Con_ToggleConsole_f(void) {
	// Can't toggle the console when it's the only thing available
	if (clc.state == CA_DISCONNECTED && Key_GetCatcher() == KEYCATCH_CONSOLE) {
		return;
	}

	if (con_autoclear->integer) {
		Field_Clear(&g_consoleField);
	}

	g_consoleField.widthInChars = g_console_field_width;

	Con_ClearNotify();
	Key_SetCatcher(Key_GetCatcher() ^ KEYCATCH_CONSOLE);
}

/*
===================
Con_ToggleMenu_f
===================
*/
static void Con_ToggleMenu_f(void) {
	CL_KeyEvent(K_ESCAPE, qtrue, Sys_Milliseconds());
	CL_KeyEvent(K_ESCAPE, qfalse, Sys_Milliseconds());
}

/*
================
Con_MessageMode_f
================
*/
static void Con_MessageMode_f(void) {
	chat_playerNum = -1;
	chat_team = qfalse;
	Field_Clear(&chatField);
	chatField.widthInChars = 30;

	Key_SetCatcher(Key_GetCatcher() ^ KEYCATCH_MESSAGE);
}

/*
================
Con_MessageMode2_f
================
*/
static void Con_MessageMode2_f(void) {
	chat_playerNum = -1;
	chat_team = qtrue;
	Field_Clear(&chatField);
	chatField.widthInChars = 25;
	Key_SetCatcher(Key_GetCatcher() ^ KEYCATCH_MESSAGE);
}

/*
================
Con_MessageMode3_f
================
*/
static void Con_MessageMode3_f(void) {
	chat_playerNum = VM_Call(cgvm, CG_CROSSHAIR_PLAYER);
	if (chat_playerNum < 0 || chat_playerNum >= MAX_CLIENTS) {
		chat_playerNum = -1;
		return;
	}
	chat_team = qfalse;
	Field_Clear(&chatField);
	chatField.widthInChars = 30;
	Key_SetCatcher(Key_GetCatcher() ^ KEYCATCH_MESSAGE);
}

/*
================
Con_MessageMode4_f
================
*/
static void Con_MessageMode4_f(void) {
	chat_playerNum = VM_Call(cgvm, CG_LAST_ATTACKER);
	if (chat_playerNum < 0 || chat_playerNum >= MAX_CLIENTS) {
		chat_playerNum = -1;
		return;
	}
	chat_team = qfalse;
	Field_Clear(&chatField);
	chatField.widthInChars = 30;
	Key_SetCatcher(Key_GetCatcher() ^ KEYCATCH_MESSAGE);
}

/*
================
Con_Clear_f
================
*/
static void Con_Clear_f(void) {
	int i;

	for (i = 0; i < CON_TEXTSIZE; i++) {
		con.text[i] = (ColorIndex(COLOR_WHITE) << 8) | ' ';
	}

	Con_Bottom(); // go to end
}

/*
================
Con_Dump_f

Save the console contents out to a file
================
*/
static void Con_Dump_f(void) {
	int l, x, i;
	short *line;
	fileHandle_t f;
	int bufferlen;
	char *buffer;
	char filename[MAX_QPATH];

	if (Cmd_Argc() != 2) {
		Com_Printf("usage: condump <filename>\n");
		return;
	}

	Q_strncpyz(filename, Cmd_Argv(1), sizeof(filename));
	COM_DefaultExtension(filename, sizeof(filename), ".txt");

	if (!COM_CompareExtension(filename, ".txt")) {
		Com_Printf("Con_Dump_f: Only the \".txt\" extension is supported by this command!\n");
		return;
	}

	f = FS_FOpenFileWrite(filename);
	if (!f) {
		Com_Printf("ERROR: couldn't open %s.\n", filename);
		return;
	}

	Com_Printf("Dumped console text to %s.\n", filename);

	// skip empty lines
	for (l = con.current - con.totallines + 1; l <= con.current; l++) {
		line = con.text + (l % con.totallines) * con.linewidth;
		for (x = 0; x < con.linewidth; x++)
			if ((line[x] & 0xff) != ' ')
				break;
		if (x != con.linewidth)
			break;
	}

#ifdef _WIN32
	bufferlen = con.linewidth + 3 * sizeof(char);
#else
	bufferlen = con.linewidth + 2 * sizeof(char);
#endif

	buffer = Hunk_AllocateTempMemory(bufferlen);

	// write the remaining lines
	buffer[bufferlen - 1] = 0;
	for (; l <= con.current; l++) {
		line = con.text + (l % con.totallines) * con.linewidth;
		for (i = 0; i < con.linewidth; i++)
			buffer[i] = line[i] & 0xff;
		for (x = con.linewidth - 1; x >= 0; x--) {
			if (buffer[x] == ' ')
				buffer[x] = 0;
			else
				break;
		}
#ifdef _WIN32
		Q_strcat(buffer, bufferlen, "\r\n");
#else
		Q_strcat(buffer, bufferlen, "\n");
#endif
		FS_Write(buffer, strlen(buffer), f);
	}

	Hunk_FreeTempMemory(buffer);
	FS_FCloseFile(f);
}

/*
================
Con_ClearNotify
================
*/
void Con_ClearNotify(void) {
	int i;

	for (i = 0; i < NUM_CON_TIMES; i++) {
		con.times[i] = 0;
	}
}

/*
================
Con_CheckResize

If the line width has changed, reformat the buffer.
================
*/
void Con_CheckResize(void) {
	float scale;

	if (con.viswidth == cls.glconfig.vidWidth && !con_scale->modified) {
		return;
	}

	scale = con_scale->value;

	con.viswidth = cls.glconfig.vidWidth;

	fontSize.w = SMALLCHAR_WIDTH * scale;
	fontSize.h = SMALLCHAR_HEIGHT * scale;

	if (cls.glconfig.vidWidth == 0) // video hasn't been initialized yet
	{
		int width;
		g_console_field_width = DEFAULT_CONSOLE_WIDTH;
		width = DEFAULT_CONSOLE_WIDTH * scale;
		con.linewidth = width;
		con.totallines = CON_TEXTSIZE / con.linewidth;

		Con_Clear_f();
	} else {
		int oldcurrent, oldwidth, oldtotallines;
		short tbuf[CON_TEXTSIZE];
		int i, j, width, numlines, numchars;

		width = ((cls.glconfig.vidWidth / fontSize.w) - 2);

		g_console_field_width = width;
		g_consoleField.widthInChars = g_console_field_width;

		if (width > MAX_CONSOLE_WIDTH)
			width = MAX_CONSOLE_WIDTH;

		oldwidth = con.linewidth;
		oldtotallines = con.totallines;
		oldcurrent = con.current;

		con.linewidth = width;
		con.totallines = CON_TEXTSIZE / con.linewidth;

		numchars = oldwidth;
		if (numchars > con.linewidth)
			numchars = con.linewidth;

		if (oldcurrent > oldtotallines)
			numlines = oldtotallines;
		else
			numlines = oldcurrent + 1;

		if (numlines > con.totallines)
			numlines = con.totallines;

		Com_Memcpy(tbuf, con.text, CON_TEXTSIZE * sizeof(short));

		for (i = 0; i < CON_TEXTSIZE; i++)
			con.text[i] = (ColorIndex(COLOR_WHITE) << 8) | ' ';

		for (i = 0; i < numlines; i++) {
			const short *src = &tbuf[((oldcurrent - i + oldtotallines) % oldtotallines) * oldwidth];
			short *dst = &con.text[(numlines - 1 - i) * con.linewidth];
			for (j = 0; j < numchars; j++)
				*dst++ = *src++;
		}

		Con_ClearNotify();

		con.current = numlines - 1;
	}

	con.display = con.current;
	con_scale->modified = qfalse;
}

/*
==================
Cmd_CompleteTxtName
==================
*/
static void Cmd_CompleteTxtName(const char *args, int argNum) {
	if (argNum == 2) {
		Field_CompleteFilename("", "txt", qfalse, qtrue);
	}
}

/*
================
Con_Init
================
*/
void Con_Init(void) {
	int i;

	con_notifytime = Cvar_Get("con_notifytime", "-4", 0);
	con_conspeed = Cvar_Get("scr_conspeed", "3", 0);
	con_autoclear = Cvar_Get("con_autoclear", "1", CVAR_ARCHIVE);
	con_scale = Cvar_Get("con_scale", "2", CVAR_ARCHIVE);
	Cvar_CheckRange(con_scale, 1, 4, qfalse);

	fontSize.w = SMALLCHAR_WIDTH * con_scale->value;
	fontSize.h = SMALLCHAR_HEIGHT * con_scale->value;

	Field_Clear(&g_consoleField);
	g_consoleField.widthInChars = g_console_field_width;
	for (i = 0; i < COMMAND_HISTORY; i++) {
		Field_Clear(&historyEditLines[i]);
		historyEditLines[i].widthInChars = g_console_field_width;
	}
	CL_LoadConsoleHistory();

	Cmd_AddCommand("toggleconsole", Con_ToggleConsole_f);
	Cmd_AddCommand("togglemenu", Con_ToggleMenu_f);
	Cmd_AddCommand("messagemode", Con_MessageMode_f);
	Cmd_AddCommand("messagemode2", Con_MessageMode2_f);
	Cmd_AddCommand("messagemode3", Con_MessageMode3_f);
	Cmd_AddCommand("messagemode4", Con_MessageMode4_f);
	Cmd_AddCommand("clear", Con_Clear_f);
	Cmd_AddCommand("condump", Con_Dump_f);
	Cmd_SetCommandCompletionFunc("condump", Cmd_CompleteTxtName);
}

/*
================
Con_Shutdown
================
*/
void Con_Shutdown(void) {
	Cmd_RemoveCommand("toggleconsole");
	Cmd_RemoveCommand("togglemenu");
	Cmd_RemoveCommand("messagemode");
	Cmd_RemoveCommand("messagemode2");
	Cmd_RemoveCommand("messagemode3");
	Cmd_RemoveCommand("messagemode4");
	Cmd_RemoveCommand("clear");
	Cmd_RemoveCommand("condump");
}

/*
===============
Con_Linefeed
===============
*/
static void Con_Linefeed(qboolean skipnotify) {
	int i;

	// mark time for transparent overlay
	if (con.current >= 0) {
		if (skipnotify)
			con.times[con.current % NUM_CON_TIMES] = 0;
		else
			con.times[con.current % NUM_CON_TIMES] = cls.realtime;
	}

	con.x = 0;
	if (con.display == con.current)
		con.display++;
	con.current++;
	for (i = 0; i < con.linewidth; i++)
		con.text[(con.current % con.totallines) * con.linewidth + i] = (ColorIndex(COLOR_WHITE) << 8) | ' ';
}

/*
================
CL_ConsolePrint

Handles cursor positioning, line wrapping, etc
All console printing must go through this in order to be logged to disk
If no console is visible, the text will appear at the top of the game window
================
*/
void CL_ConsolePrint(const char *txt) {
	int y, l;
	unsigned char c;
	unsigned short color;
	qboolean skipnotify = qfalse; // NERVE - SMF
	int prev;					  // NERVE - SMF

	// TTimo - prefix for text that shows up in console but not in notify
	// backported from RTCW
	if (!Q_strncmp(txt, "[skipnotify]", 12)) {
		skipnotify = qtrue;
		txt += 12;
	}

	// for some demos we don't want to ever show anything on the console
	if (cl_noprint && cl_noprint->integer) {
		return;
	}

	if (!con.initialized) {
		static cvar_t null_cvar = { 0 };
		fontSize.w = SMALLCHAR_WIDTH;
		fontSize.h = SMALLCHAR_HEIGHT;
		con.color[0] = con.color[1] = con.color[2] = con.color[3] = 1.0f;
		con.viswidth = -9999;
		con.linewidth = -1;
		con_scale = &null_cvar;
		con_scale->value = 1.0f;
		con_scale->modified = qtrue;
		Con_CheckResize();
		con.initialized = qtrue;
	}

	color = ColorIndex(COLOR_WHITE);

	while ((c = *((const unsigned char *)txt)) != 0) {
		if (Q_IsColorString(txt)) {
			color = ColorIndex(*(txt + 1));
			txt += 2;
			continue;
		}

		// count word length
		for (l = 0; l < con.linewidth; l++) {
			if (txt[l] <= ' ') {
				break;
			}
		}

		// word wrap
		if (l != con.linewidth && (con.x + l >= con.linewidth)) {
			Con_Linefeed(skipnotify);
		}

		txt++;

		switch (c) {
		case '\n':
			Con_Linefeed(skipnotify);
			break;
		case '\r':
			con.x = 0;
			break;
		default: // display character and advance
			y = con.current % con.totallines;
			con.text[y * con.linewidth + con.x] = (color << 8) | c;
			con.x++;
			if (con.x >= con.linewidth)
				Con_Linefeed(skipnotify);
			break;
		}
	}

	// mark time for transparent overlay
	if (con.current >= 0) {
		// NERVE - SMF
		if (skipnotify) {
			prev = con.current % NUM_CON_TIMES - 1;
			if (prev < 0)
				prev = NUM_CON_TIMES - 1;
			con.times[prev] = 0;
		} else
			// -NERVE - SMF
			con.times[con.current % NUM_CON_TIMES] = cls.realtime;
	}
}

/*
==============================================================================

DRAWING

==============================================================================
*/

/*
================
Con_DrawInput

Draw the editline after a ] prompt
================
*/
static void Con_DrawInput(void) {
	int y;

	if (clc.state != CA_DISCONNECTED && !(Key_GetCatcher() & KEYCATCH_CONSOLE)) {
		return;
	}

	y = con.vislines - (fontSize.h * 2);

	re.SetColor(con.color);

	SCR_DrawChar(con.xadjust + 1 * fontSize.w, y, ']', fontSize);

	Field_Draw(&g_consoleField, con.xadjust + 2 * fontSize.w, y, SCREEN_WIDTH - 3 * fontSize.w, qtrue, qtrue, fontSize);
}

/*
================
Con_DrawNotify

Draws the last few lines of output transparently over the game top
================
*/
void Con_DrawNotify(void) {
	int x, v;
	short *text;
	int i;
	int time;
	int skip;
	int currentColor;

	currentColor = 7;
	re.SetColor(g_color_table[currentColor]);

	v = 0;
	for (i = con.current - NUM_CON_TIMES + 1; i <= con.current; i++) {
		if (i < 0)
			continue;
		time = con.times[i % NUM_CON_TIMES];
		if (time == 0)
			continue;
		time = cls.realtime - time;
		if (time > con_notifytime->value * 1000)
			continue;
		text = con.text + (i % con.totallines) * con.linewidth;

		if (cl.snap.ps.pm_type != PM_INTERMISSION && Key_GetCatcher() & (KEYCATCH_UI | KEYCATCH_CGAME)) {
			continue;
		}

		for (x = 0; x < con.linewidth; x++) {
			if ((text[x] & 0xff) == ' ') {
				continue;
			}
			if (ColorIndexForNumber(text[x] >> 8) != currentColor) {
				currentColor = ColorIndexForNumber(text[x] >> 8);
				re.SetColor(g_color_table[currentColor]);
			}
			SCR_DrawChar(cl_conXOffset->integer + con.xadjust + (x + 1) * fontSize.w, v, text[x] & 0xff, fontSize);
		}

		v += fontSize.h;
	}

	re.SetColor(NULL);

	if (Key_GetCatcher() & (KEYCATCH_UI | KEYCATCH_CGAME)) {
		return;
	}

	// draw the chat line
	if (Key_GetCatcher() & KEYCATCH_MESSAGE) {
		if (chat_team) {
			SCR_DrawString(8, v, "say_team:", qfalse, FONT_BIG);
			skip = 10;
		} else {
			SCR_DrawString(8, v, "say:", qfalse, FONT_BIG);
			skip = 5;
		}

		Field_Draw(&chatField, skip * FONT_BIG.w, v, SCREEN_WIDTH - (skip + 1) * FONT_BIG.w, qtrue, qtrue, FONT_BIG);
	}
}

/*
================
Con_DrawSolidConsole

Draws the console with the solid background
================
*/
static void Con_DrawSolidConsole(float frac) {
	int i, x, y;
	int rows;
	short *text;
	int row;
	int lines;
	//	qhandle_t		conShader;
	int currentColor;
	vec4_t color;

	lines = cls.glconfig.vidHeight * frac;
	if (lines <= 0)
		return;

	if (lines > cls.glconfig.vidHeight)
		lines = cls.glconfig.vidHeight;

	// on wide screens, we will center the text
	con.xadjust = 0;
	SCR_AdjustFrom640(&con.xadjust, NULL, NULL, NULL);

	// draw the background
	y = frac * SCREEN_HEIGHT;
	if (y < 1) {
		y = 0;
	} else {
		SCR_DrawPic(0, 0, SCREEN_WIDTH, y, cls.consoleShader);
	}

	color[0] = 1.0f;
	color[1] = 0.8f;
	color[2] = 0.0f;
	color[3] = 1;
	SCR_FillRect(0, y - 2, SCREEN_WIDTH, 4, color);
	// SCR_FillRect( 0, y+2, SCREEN_WIDTH, 8, tblack );

	// draw the version number

	re.SetColor(g_color_table[ColorIndex(COLOR_RED)]);

	i = strlen(Q3_VERSION);

	for (x = 0; x < i; x++) {
		SCR_DrawChar(cls.glconfig.vidWidth - (i - x + 1) * fontSize.w, lines - fontSize.h,
						  Q3_VERSION[x], fontSize);
	}

	// draw the text
	con.vislines = lines;
	rows = (lines - fontSize.h) / fontSize.h; // rows of text to draw

	y = lines - (fontSize.h * 3);

	// draw from the bottom up
	if (con.display != con.current) {
		// draw arrows to show the buffer is backscrolled
		re.SetColor(g_color_table[ColorIndex(COLOR_RED)]);
		for (x = 0; x < con.linewidth; x += 4)
			SCR_DrawChar(con.xadjust + (x + 1) * fontSize.w, y, '^', fontSize);
		y -= fontSize.h;
		rows--;
	}

	row = con.display;

	if (con.x == 0) {
		row--;
	}

	currentColor = 7;
	re.SetColor(g_color_table[currentColor]);

	for (i = 0; i < rows; i++, y -= fontSize.h, row--) {
		if (row < 0)
			break;
		if (con.current - row >= con.totallines) {
			// past scrollback wrap point
			continue;
		}

		text = con.text + (row % con.totallines) * con.linewidth;

		for (x = 0; x < con.linewidth; x++) {
			if ((text[x] & 0xff) == ' ') {
				continue;
			}

			if (ColorIndexForNumber(text[x] >> 8) != currentColor) {
				currentColor = ColorIndexForNumber(text[x] >> 8);
				re.SetColor(g_color_table[currentColor]);
			}
			SCR_DrawChar(con.xadjust + (x + 1) * fontSize.w, y, text[x] & 0xff, fontSize);
		}
	}

	// draw the input prompt, user text, and cursor if desired
	Con_DrawInput();

	re.SetColor(NULL);
}

/*
==================
Con_DrawConsole
==================
*/
void Con_DrawConsole(void) {
	// check for console width changes from a vid mode change
	Con_CheckResize();

	// if disconnected, render console full screen
	if (clc.state == CA_DISCONNECTED) {
		if (!(Key_GetCatcher() & (KEYCATCH_UI | KEYCATCH_CGAME))) {
			Con_DrawSolidConsole(1.0);
			return;
		}
	}

	if (con.displayFrac) {
		Con_DrawSolidConsole(con.displayFrac);
	} else {
		// draw notify lines
		if (clc.state == CA_ACTIVE) {
			Con_DrawNotify();
		}
	}
}

//================================================================

/*
==================
Con_RunConsole

Scroll it up or down
==================
*/
void Con_RunConsole(void) {
	// decide on the destination height of the console
	if (Key_GetCatcher() & KEYCATCH_CONSOLE)
		con.finalFrac = 0.5; // half screen
	else
		con.finalFrac = 0; // none visible

	// scroll towards the destination height
	if (con.finalFrac < con.displayFrac) {
		con.displayFrac -= con_conspeed->value * cls.realFrametime * 0.001;
		if (con.finalFrac > con.displayFrac)
			con.displayFrac = con.finalFrac;

	} else if (con.finalFrac > con.displayFrac) {
		con.displayFrac += con_conspeed->value * cls.realFrametime * 0.001;
		if (con.finalFrac < con.displayFrac)
			con.displayFrac = con.finalFrac;
	}
}

void Con_PageUp(void) {
	con.display -= 2;
	if (con.current - con.display >= con.totallines) {
		con.display = con.current - con.totallines + 1;
	}
}

void Con_PageDown(void) {
	con.display += 2;
	if (con.display > con.current) {
		con.display = con.current;
	}
}

void Con_Top(void) {
	con.display = con.totallines;
	if (con.current - con.display >= con.totallines) {
		con.display = con.current - con.totallines + 1;
	}
}

void Con_Bottom(void) {
	con.display = con.current;
}

void Con_Close(void) {
	if (!com_cl_running->integer) {
		return;
	}
	Field_Clear(&g_consoleField);
	Con_ClearNotify();
	Key_SetCatcher(Key_GetCatcher() & ~KEYCATCH_CONSOLE);
	con.finalFrac = 0; // none visible
	con.displayFrac = 0;
}
