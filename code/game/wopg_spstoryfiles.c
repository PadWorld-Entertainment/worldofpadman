
// some infos can be found in the header file ( "wop_spstoryfiles.h" )
#include "wopg_spstoryfiles.h"

#if defined(UI)
#include "../ui/ui_local.h"

static void executeCmd(const char* cmd) {
	trap_Cmd_ExecuteText(EXEC_APPEND, cmd);
}
#else
#include "g_local.h"

static void executeCmd(const char* cmd) {
	trap_SendConsoleCommand(EXEC_APPEND, cmd);
}
#endif

#define STORYFILES_DIR "story/"
#define STORYFILES_EXT ".story"


#define PFM "[wopSP] " // Printf "Module" info ;P

#define WOPSP_BUFFERSIZE (4*1024)
typedef struct {
	char	filename[MAX_QPATH];

	fileHandle_t	f;
	int				filesize;
//	int				currentPosInFile;
	qboolean		noDataRead; // are we at the start of this file?

	int				elementCnt; // counts the read Elements

	char	buffer[WOPSP_BUFFERSIZE];
	int		usedBuffer; // how many bytes are in the buffer
	char*	buffPtr;	// current parsing position
} wop_StoryFile_t;

static wop_StoryFile_t wop_StoryFile;

void wopSP_initStoryElement(wop_StoryElement_t* e) {
	memset(e,0,sizeof(wop_StoryElement_t));
	e->elementID = -1;
}

qboolean wopSP_validSE(const wop_StoryElement_t* e) {
	return (e->elementID >= 0);
}

static qboolean isFileAtTop(void) {
	return (wop_StoryFile.buffPtr==NULL); //FIXME? maybe this should be solved a better way
}

/*
  wopSP_execStoryElement

  Should be used to:
  ... start a Story(from UI)
  ... go to next Element(game ... intermission)
  it sets all values and change/restart map ... if it is needed
*/
void wopSP_execStoryElement(const wop_StoryElement_t* se) {
	char tmpBuff[MAX_CVAR_VALUE_STRING];

	trap_Cvar_Set("timelimit",va("%d",se->timelimit));
	trap_Cvar_Set("pointlimit",va("%d",se->pointlimit));

	trap_Cvar_VariableStringBuffer("g_gametype",tmpBuff,sizeof(tmpBuff));
	if(atoi(tmpBuff)!=se->gametype) {
		trap_Cvar_Set("g_gametype",va("%d",se->gametype));
	}

	//TODO: other options (LPSflags/lives, instaGib,...)

	trap_Cvar_VariableStringBuffer("mapname",tmpBuff,sizeof(tmpBuff));
	if(Q_stricmp(tmpBuff,se->map)) {
		executeCmd(va("map %s\n",se->map));
	}
	else {
		executeCmd("map_restart 0\n");
	}
}

/*
  wopSP_addBots

  TODO: call this funktion ... after init (or in the first frames)
*/
void wopSP_addBots(const wop_StoryElement_t* se) {
	//TODO: add bots ... like they are configured in storyElement
}

static qboolean isNum(char c) {
	return (c>='0' && c<='9');
}

/*
  wopSP_compareWithNextString

  compares with "#[ID]" and "[elementName]"
  return: true -> the nextStr points at this element
*/
qboolean wopSP_compareWithNextString(const char* nextStr, const wop_StoryElement_t* element) {
	if(!nextStr || !nextStr[0])
		return qfalse;

	if(nextStr[0] == '#' && isNum(nextStr[1]))
		return (element->elementID == atoi(nextStr+1));

	if(!Q_stricmp(nextStr,element->name))
		return qtrue;

	return qfalse;
}

static int bytesLeftInBuffer(void) {
	if(wop_StoryFile.buffPtr)
		return wop_StoryFile.usedBuffer - (wop_StoryFile.buffPtr-wop_StoryFile.buffer);
	else
		return wop_StoryFile.usedBuffer;
}

int wopSP_openStory(const char* storyname) {
	return wopSP_openFile(va(STORYFILES_DIR"%s"STORYFILES_EXT,storyname));
}

int wopSP_openFile(const char* filename) {
	if(wop_StoryFile.f)
		wopSP_closeFile();

	if(!filename || !filename[0])
		return -1;

	if(strlen(filename) >= sizeof(wop_StoryFile.filename)) {
		Com_Printf(PFM"filename of storyfile is to long\n");
		return -2;
	}

	strcpy(wop_StoryFile.filename,filename);

	wop_StoryFile.filesize = trap_FS_FOpenFile(wop_StoryFile.filename, &wop_StoryFile.f, FS_READ);
	if ( !wop_StoryFile.f ) {
		Com_Printf(PFM"cannot open file(%s)\n",wop_StoryFile.filename);
		return -3;
	}

	return 0;
}

int wopSP_resetFile(void) {
	char	filename[MAX_QPATH];
	strcpy(filename,wop_StoryFile.filename);

	wopSP_closeFile();
	return wopSP_openFile(filename);
}

typedef enum
{
	NT_NORMAL=0,

	NT_STRING,
	NT_INT,
	NT_GAMETYPE,
	NT_SPECIAL
} next_token_t;

/*

  newElement: output!
  return:	<0  -> error
			>=0 -> ElementNumber
*/
int wopSP_loadNextStoryElement( wop_StoryElement_t* newElement ) {
	qboolean	noElementRead = qtrue; // false if a whole Element was read
	int			lvl=0; // { => ++, } => --
	next_token_t nextT=NT_NORMAL;
	int*		nextInt=NULL; // NT_INT
	char*		nextStr=NULL; // NT_STRING
	int			nextStrMaxLen = 128; // NT_STRING

	wopSP_initStoryElement(newElement);

	if(newElement==NULL)
		return -1;

	while((wop_StoryFile.f || bytesLeftInBuffer()) && noElementRead) {
		char* token;

		// refill buffer ...
		if(bytesLeftInBuffer()<WOPSP_BUFFERSIZE/2 && wop_StoryFile.f) {
			char* read;
			int tmpi = wop_StoryFile.buffPtr-wop_StoryFile.buffer;

			if(tmpi>=0) {
				strcpy(wop_StoryFile.buffer,wop_StoryFile.buffPtr);
				wop_StoryFile.usedBuffer -= tmpi;
				wop_StoryFile.buffPtr = wop_StoryFile.buffer;
			}

			tmpi = (WOPSP_BUFFERSIZE-wop_StoryFile.usedBuffer)-1;
			read = (wop_StoryFile.buffer+wop_StoryFile.usedBuffer);
			memset(read,0,tmpi+1);
			trap_FS_Read(read,tmpi,wop_StoryFile.f);
			if(*read == '\0') {
				trap_FS_FCloseFile(wop_StoryFile.f);
				wop_StoryFile.f = 0;
			}
			else {
				wop_StoryFile.usedBuffer += strlen(read);
			}
		}

		// init buffPtr ...
		if(wop_StoryFile.buffPtr==NULL)
			wop_StoryFile.buffPtr=wop_StoryFile.buffer;

		// get a token
		token = COM_Parse(&wop_StoryFile.buffPtr);
		if(wop_StoryFile.buffPtr==NULL) // "skip whitespace" ran into the end (maybe only a temporary end, with this buffering...)
			wop_StoryFile.buffPtr = wop_StoryFile.buffer+wop_StoryFile.usedBuffer;

		// analyse the token
		if(token && token[0]) {
			if(!strcmp(token,"{")) {
				++lvl;
				if(lvl>1)
					Com_Printf(PFM"unexpected Number of '}'(in %s)\n",wop_StoryFile.filename);

				if(nextT!=NT_NORMAL) {
					nextT=NT_NORMAL;
				}
			}
			else if(!strcmp(token,"}")) {
				--lvl;
				if(lvl==0) { // we are back at level 0, so we just left the element boundings
					noElementRead=qfalse;
					newElement->elementID = wop_StoryFile.elementCnt;
					++wop_StoryFile.elementCnt;
				}
				else if(lvl<0)
					Com_Printf(PFM"unexpected Number of '{'(in %s)\n",wop_StoryFile.filename);

				if(nextT!=NT_NORMAL) {
					nextT=NT_NORMAL;
				}
			}
			else if(lvl==0) { // new name for the nextElement
				Q_strncpyz(newElement->name,token,sizeof(newElement->name));
				if(token[0]=='#' && isNum(token[1]))
					Com_Printf(PFM"Warning: Elementname \"%s\" contains Elementnumber syntax\n",token);
			}
			else if(lvl==1) {
				if(nextT!=NT_NORMAL){
					switch(nextT) {
					case NT_STRING:
						Q_strncpyz(nextStr,token,nextStrMaxLen);
						nextT=NT_NORMAL;
						break;
					case NT_GAMETYPE:
						if(isNum(token[0]))
							newElement->gametype = atoi(token);
						else
							newElement->gametype = convertGTStringToGTNumber(token);
						nextT=NT_NORMAL;
						break;
					case NT_SPECIAL:
						if(!Q_stricmp(token,"startmap")) {
							newElement->flags |= WSEF_STARTMAP;
						}
						else if(!Q_stricmp(token,"endmap")) {
							newElement->flags |= WSEF_ENDMAP;
						}
						break;
					case NT_INT:
						*nextInt = atoi(token);
						nextT=NT_NORMAL;
						break;
					default:
						nextT=NT_NORMAL;
						break;
					}
				}
				else if(!Q_stricmp(token,"map")) {
					nextT=NT_STRING;
					nextStr = newElement->map;
					nextStrMaxLen = sizeof(newElement->map);
				}
				else if(!Q_stricmp(token,"gametype")) {
					nextT=NT_GAMETYPE;
				}
				else if(!Q_stricmp(token,"special")) {
					nextT=NT_SPECIAL;
				}
				else if(!Q_stricmp(token,"timelimit")) {
					nextT=NT_INT;
					nextInt=&newElement->timelimit;
				}
				else if(!Q_stricmp(token,"pointlimit")) {
					nextT=NT_INT;
					nextInt=&newElement->pointlimit;
				}
				else if(!Q_stricmp(token,"intro")) {
					nextT=NT_STRING;
					nextStr = newElement->intro;
					nextStrMaxLen = sizeof(newElement->intro);
				}
				else if(!Q_stricmp(token,"extroWin")) {
					nextT=NT_STRING;
					nextStr = newElement->extroWin;
					nextStrMaxLen = sizeof(newElement->extroWin);
				}
				else if(!Q_stricmp(token,"extroLoos")) {
					nextT=NT_STRING;
					nextStr = newElement->extroLoos;
					nextStrMaxLen = sizeof(newElement->extroLoos);
				}
				else if(!Q_stricmp(token,"nextAfterWin")) {
					nextT=NT_STRING;
					nextStr = newElement->nextAfterWin;
					nextStrMaxLen = sizeof(newElement->nextAfterWin);
				}
				else if(!Q_stricmp(token,"nextAfterLoos")) {
					nextT=NT_STRING;
					nextStr = newElement->nextAfterLoos;
					nextStrMaxLen = sizeof(newElement->nextAfterLoos);
				}
				else if(!Q_stricmp(token,"bots")) {
					nextT=NT_STRING;
					nextStr = newElement->bots;
					nextStrMaxLen = sizeof(newElement->bots);
				}
				else if(!Q_stricmp(token,"forceToTeam")) {
					nextT=NT_STRING;
					nextStr = newElement->forceToTeam;
					nextStrMaxLen = sizeof(newElement->forceToTeam);
				}
				//TODO: add more options

				else Com_Printf(PFM"Unknown Token \"%s\"(in %s)\n",token,wop_StoryFile.filename);
			}
			else Com_Printf(PFM"Unknown Token \"%s\"(in %s)\n",token,wop_StoryFile.filename);
		}
	}

	if(noElementRead==qfalse)
		return (wop_StoryFile.elementCnt-1);

	return -1;
}

/*
  wopSP_findStoryElement

  storyElementName: could be a real name or "#[ElementID]"
  newElement: output!
  return:	<0	error
			>=0	ElementID
*/
int wopSP_findStoryElement( const char* storyElementName, wop_StoryElement_t* newElement ) {
	qboolean startedFromTop=qtrue;
	qboolean notDone=qtrue;
	qboolean foundElement=qfalse;

	startedFromTop=isFileAtTop();

	while(notDone) {
		if(wopSP_loadNextStoryElement(newElement) >= 0) {
			if(wopSP_compareWithNextString(storyElementName,newElement)) {
				foundElement=qtrue;
				notDone=qfalse;
			}
		}
		else {
			if(!startedFromTop) {
				wopSP_resetFile();
				startedFromTop=qtrue;
			}
			else
				notDone=qfalse;
		}
	}

	if(foundElement)
		return newElement->elementID;

	return -1;
}

/*
  wopSP_closeFile

  Close File and reset wop_StoryFile
*/
void wopSP_closeFile(void) {
	if(wop_StoryFile.f)
		trap_FS_FCloseFile(wop_StoryFile.f);

	memset(&wop_StoryFile,0,sizeof(wop_StoryFile));
}

/*
####################### #######################

  savefiles

  one story <-> one savefile
  one line <-> one GameState/savename
  line: "[name] [first won SE] [2nd won SE] ..."
  (example: "default 1 3")

####################### #######################
*/
#define GAMESTATE_DIR "save/"
#define GAMESTATE_EXT ".gs.dat"

#define SIZEOFGSBUFFER 4*1024

/*
  wopSP_loadGameState

  return: qtrue -> found and loaded a GameState
*/
qboolean wopSP_loadGameState(const char* storyname, const char* savename, char* gsStr, int gsStrSize) {
	fileHandle_t	f;
	char tmpBuff[SIZEOFGSBUFFER];
	char* parsePtr;

	memset(gsStr,0,gsStrSize);

	Com_sprintf(tmpBuff,sizeof(tmpBuff),GAMESTATE_DIR"%s"GAMESTATE_EXT,storyname);
	if(trap_FS_FOpenFile(tmpBuff, &f, FS_READ) >=  SIZEOFGSBUFFER)
		Com_Printf(PFM"Warning: GameState file(%s) is bigger than the used buffer\n",tmpBuff);

	if(f) {
		memset(tmpBuff,0,sizeof(tmpBuff));
		trap_FS_Read(tmpBuff,sizeof(tmpBuff)-1,f);

		trap_FS_FCloseFile(f);

		parsePtr = tmpBuff;
		while(parsePtr!=NULL && parsePtr[0]!='\0') {
			char* token;

			token = COM_Parse(&parsePtr);
			if(!Q_stricmp(token,savename)) {
				char* endlPtr;
				// make sure we didn't hit eof (malformed save file)
				if(NULL == parsePtr){
					trap_Error( va("malformed gamestate %s in file %s", savename, storyname) );
					return qfalse;
				}

				Q_strncpyz(gsStr,parsePtr,gsStrSize);
				endlPtr = strchr(gsStr,'\n');
				if(endlPtr)
					*endlPtr = '\0';
				break;
			}

			//going to end of line
			if(parsePtr!=NULL)
				while(*(++parsePtr) != '\n' && *parsePtr != '\0') ;
			
			// end of line? move on!
			if(*parsePtr == '\n')
				parsePtr++;
		}
	}

	return (gsStr[0]!='\0');
}

/*
  wopSP_loadLastWonSE

  return: last entered WonSE-Number
*/
int wopSP_loadLastWonSE(const char* storyname, const char* savename) {
	int r=-1;
	char tmpBuff[1024];
	char *token, *parsePtr;

	if(!wopSP_loadGameState(storyname, savename, tmpBuff, sizeof(tmpBuff)))
		Com_Printf(PFM"Couldn't open GameState file(story: %s, savename: %s)\n",storyname, savename);

	parsePtr = tmpBuff;
	token = COM_Parse(&parsePtr);
	while( token[0] != '\0' ){
		r = atoi(token);
		token = COM_Parse(&parsePtr);
	}

	return r;
}

/*
  wopSP_WonSEalreadySaved

  return: true if the given WonSE-Number is already saved to this savegame
*/
qboolean wopSP_WonSEalreadySaved(const char* storyname, const char* savename,int WonSE) {
	qboolean r=qfalse;
	char tmpBuff[1024];
	char *token, *parsePtr;

	wopSP_loadGameState(storyname, savename, tmpBuff, sizeof(tmpBuff));
	parsePtr = tmpBuff;
	token = COM_Parse(&parsePtr);
	while( token[0] != '\0' ) {
		if(WonSE == atoi(token))
			r = qtrue;
		token = COM_Parse(&parsePtr);
	}

	return r;
}

/*
  wopSP_addWonSEtoGameState

  adds a WonSE-Number to this storyname+savename
*/
void wopSP_addWonSEtoGameState(const char* storyname, const char* savename, int wonSE) {
	fileHandle_t	f;
	char tmpBuff[SIZEOFGSBUFFER];

	Com_sprintf(tmpBuff,sizeof(tmpBuff),GAMESTATE_DIR"%s"GAMESTATE_EXT,storyname);
	if(trap_FS_FOpenFile(tmpBuff, &f, FS_READ) >=  SIZEOFGSBUFFER)
		Com_Printf(PFM"Warning: GameState file(%s) is bigger than the used buffer\n",tmpBuff);

	memset(tmpBuff,0,sizeof(tmpBuff));
	if(f) {
		trap_FS_Read(tmpBuff,sizeof(tmpBuff)-1,f);

		trap_FS_FCloseFile(f);
	}

	trap_FS_FOpenFile(va(GAMESTATE_DIR"%s"GAMESTATE_EXT,storyname), &f, FS_WRITE);
	if(!f)
		Com_Printf(PFM"Couldn't open GameState file(%s), for writting\n",tmpBuff);
	else
	{
		char *token, *parsePtr;
		char *lineStart, *lineEnd;
		qboolean saveLine = qfalse;
		qboolean noSaveLineFound = qtrue;

		parsePtr = tmpBuff;
		while(parsePtr && *parsePtr) {
			lineStart=parsePtr;
			token=COM_Parse(&parsePtr);

			if(!Q_stricmp(token,savename))
				saveLine = qtrue;

			lineEnd = strchr(lineStart,'\n');
			if(!lineEnd)
				lineEnd = lineStart+strlen(lineStart)-1;

			if(saveLine && lineEnd[0]=='\n')
				trap_FS_Write(lineStart, (lineEnd-lineStart), f);
			else
				trap_FS_Write(lineStart, (lineEnd-lineStart)+1, f);

			if(saveLine) {
				char* tmpPtr;
				tmpPtr = va(" %d\n",wonSE);
				trap_FS_Write(tmpPtr, strlen(tmpPtr), f);

				noSaveLineFound = qfalse;
			}


			saveLine = qfalse;
			parsePtr = lineEnd+1;
		}

		if(noSaveLineFound) {
			char* tmpPtr;
			tmpPtr = va("%s %d\n",savename,wonSE);
			trap_FS_Write(tmpPtr, strlen(tmpPtr), f);
		}

		trap_FS_FCloseFile(f);
	}
}


