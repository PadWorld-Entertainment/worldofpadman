/*
####################### ####################### ####################### ####################### #######################

  WOP Music Menu

  by #@

####################### ####################### ####################### ####################### #######################
*/


#include "ui_local.h"

#define MUSIC_BG_ARM "wopmusic/bg_arm.tga"

#define MAX_TRACKS		15
#define MAX_LPS			10
#define MAX_TITLELENGHT	64
#define MAX_LPNAME		32

#define ID_TRACK1				10
#define ID_EXIT					ID_TRACK1+MAX_TRACKS
#define ID_SWITCH				ID_EXIT+1
#define ID_CLEARLIST			ID_SWITCH+1
#define ID_PLAYALL				ID_CLEARLIST+1

typedef struct
{
	menuframework_s	menu;

	menutext_s		tracks[MAX_TRACKS];
//	menutext_s		exit;
//	menutext_s		switchLPs;
//	menutext_s		clearlist;
//	menutext_s		playall;
	menubitmap1024s_s	exit;
	menubitmap1024s_s	switchLPs;
	menubitmap1024s_s	clearlist;
	menubitmap1024s_s	playall;

	qhandle_t		bg_arm;
	int				switching; // -1 = out , 0 = nothing , 1 = in, -2 = quit menu
	int				switchtimer; // switch(in/out) started

	int				currentLP;
} wopmusicmenu_t;

static wopmusicmenu_t	wopmusicmenu;

typedef struct
{
	char			title[MAX_TITLELENGHT];
	char			file[MAX_QPATH];
	int				lenght;
}TrackInfo_t;

typedef struct
{
//	char			title[MAX_TRACKS][MAX_TITLELENGHT];
//	char			trackfile[MAX_TRACKS][MAX_QPATH];
//	int				tracklenght[MAX_TRACKS];
	TrackInfo_t		tracks[MAX_TRACKS];
	qhandle_t		bg;
	char			lpname[MAX_LPNAME];

	int				numTracks;
} LPInfos_t;

typedef struct playOrder_s
{
	struct playOrder_s	*next;
	int					LP;
	int					Track;
} playOrder_t;

static playOrder_t playOrderMEM[MAX_TRACKS*MAX_LPS];
static playOrder_t *freeOrderItems;

typedef struct
{
	LPInfos_t		LPs[MAX_LPS];

	playOrder_t		*playOrder;
	playOrder_t		*lastplayOrderItem;
	int				numLPs;
	int				songstarted;
} wopmusicInfos_t;

static wopmusicInfos_t	wopmusicInfos;

/*
#######################
  ReadWaveLenght
#######################
*/
typedef struct
{
	char				RIFF[4];
	 unsigned long		sizeofRIFF;

	 char				WAVE[4];

	 char				FMT[4];
	  unsigned long		sizeofFMT;
	  unsigned short	DataFormatCode;
	  unsigned short	numChannels;
	  unsigned long		SamplesPerSecond;
	  unsigned long		BytesPerSecond;
	  unsigned short	BytesPerSample;
	  unsigned short	BitesPerSample;

} WaveInfo_t;

typedef struct
{
	 char				DATA[4];
	  unsigned long		sizeofDATA;
} WaveDATAInfo_t;

//#define SWAPSHOT(x) (((x&0x00ff)<<8)|((x&0xff00)>>8)) //the vm-compiler dosen't like this one ... I don't know why -.-
//... but we don't need any short-int-info ^^
#define SWAPLONG(x) ((x<<24)|((x & 0xff00)<<8)|((x & 0xff0000)>>8)|(x>>24))

int ReadWaveLenght(const char* filename)
{
	fileHandle_t	f;
	WaveInfo_t		waveInfo;
	WaveDATAInfo_t	WaveDataInfo;
	unsigned long	ms, LeftBytes;
	int				i;
	char			tmpc;
	qboolean		mustswap;

	if(!trap_FS_FOpenFile(filename,&f,FS_READ)) return 0;

	trap_FS_Read(&waveInfo,sizeof(WaveInfo_t),f);

	if(waveInfo.sizeofFMT & 0xff000000)//noch nicht ideal ... soll überprüfen ob geswapt werden muss
	{
		waveInfo.sizeofFMT			= SWAPLONG(waveInfo.sizeofFMT);
		waveInfo.SamplesPerSecond	= SWAPLONG(waveInfo.SamplesPerSecond);
		waveInfo.BytesPerSecond		= SWAPLONG(waveInfo.BytesPerSecond);
		//can't link this macro with vm-linker -.- ... but we don't need this values ^^
//		waveInfo.DataFormatCode		= SWAPSHOT(waveInfo.DataFormatCode);
//		waveInfo.numChannels		= SWAPSHOT(waveInfo.numChannels);
//		waveInfo.BytesPerSample		= SWAPSHOT(waveInfo.BytesPerSample);
//		waveInfo.BitesPerSample		= SWAPSHOT(waveInfo.BitesPerSample);
		mustswap=qtrue;
	}
	else
		mustswap=qfalse;

	if(waveInfo.sizeofFMT<16)// don't know if this can happen ...
		return 0;

	//if FMT is bigger than normal ... we can't use fseek, because this will crash q3 if the file is in a zip-file
	for(i=0;i<(waveInfo.sizeofFMT-16);i++)
		trap_FS_Read(&tmpc,1,f);

	trap_FS_Read(&WaveDataInfo,sizeof(WaveDATAInfo_t),f);
	if(mustswap) WaveDataInfo.sizeofDATA = SWAPLONG(WaveDataInfo.sizeofDATA);

	while(!(WaveDataInfo.DATA[0]=='d' && WaveDataInfo.DATA[1]=='a' && WaveDataInfo.DATA[2]=='t' && WaveDataInfo.DATA[3]=='a'))
	{
//		printf("%4s\n",WaveDataInfo.DATA);
//fseek doesn't work with ziped files !!! ...
//		trap_FS_Seek(f,WaveDataInfo.sizeofDATA,FS_SEEK_CUR);

		for(i=0;i<WaveDataInfo.sizeofDATA;i++)
			trap_FS_Read(&tmpc,1,f);

		memset(&WaveDataInfo,0,sizeof(WaveDATAInfo_t));//weiß nicht ob ich das brauch
		trap_FS_Read(&WaveDataInfo,sizeof(WaveDATAInfo_t),f);
		if(mustswap) WaveDataInfo.sizeofDATA = SWAPLONG(WaveDataInfo.sizeofDATA);

		if(WaveDataInfo.DATA[0]=='\0')//ich hoffe das geht so ... aber sollte eigentlich nie gebraucht werden
		{
//			printf("file error, before finding DATA chunk\n");
			return 0;
		}
	}

	trap_FS_FCloseFile(f);


	ms=WaveDataInfo.sizeofDATA/waveInfo.BytesPerSecond;
	LeftBytes=WaveDataInfo.sizeofDATA-waveInfo.BytesPerSecond*ms;
	ms=ms*1000+LeftBytes*1000/waveInfo.BytesPerSecond;

//	Com_Printf("Wave is %i ms long.\n",ms);

	return ms;
}

/*
#######################
  AddSongToPlayList
#######################
*/
void AddSongToPlayList(int LP,int Track,qboolean loadingPlayList)
{
	playOrder_t		*playOrderPTR,*playOrderPTR2;


	if(wopmusicInfos.playOrder!=NULL)
	{
		if(wopmusicInfos.playOrder->LP==LP && wopmusicInfos.playOrder->Track==Track)
		{
			wopmusicInfos.songstarted=0;
			trap_S_StopBackgroundTrack();

			if(wopmusicInfos.playOrder==wopmusicInfos.lastplayOrderItem)
			{
				wopmusicInfos.lastplayOrderItem=NULL;
			}
			playOrderPTR=wopmusicInfos.playOrder;
			wopmusicInfos.playOrder=playOrderPTR->next;
			playOrderPTR->next=freeOrderItems;
			freeOrderItems=playOrderPTR;

			return;
		}
		else
		{
			for(playOrderPTR=(playOrderPTR2=wopmusicInfos.playOrder)->next; playOrderPTR!=NULL; playOrderPTR=playOrderPTR->next)
			{
				if(playOrderPTR->LP==LP && playOrderPTR->Track==Track)
				{
					if(playOrderPTR==wopmusicInfos.lastplayOrderItem)
						wopmusicInfos.lastplayOrderItem=playOrderPTR2;
					playOrderPTR2->next=playOrderPTR->next;
					playOrderPTR->next=freeOrderItems;
					freeOrderItems=playOrderPTR;
					return;
				}

				playOrderPTR2=playOrderPTR;
			}
		}
	}


	if(freeOrderItems==NULL)
	{
		Com_Printf("[WoP Music] too much selected tracks\n");
		return;
	}

	playOrderPTR=freeOrderItems;
	freeOrderItems=freeOrderItems->next;

	if(wopmusicInfos.playOrder==NULL)
	{
		wopmusicInfos.playOrder =
		wopmusicInfos.lastplayOrderItem = playOrderPTR;
	}
	else
	{
		wopmusicInfos.lastplayOrderItem->next = playOrderPTR;
		wopmusicInfos.lastplayOrderItem = playOrderPTR;
	}

	playOrderPTR->next=NULL;
	playOrderPTR->LP=LP;
	playOrderPTR->Track=Track;

	if(!loadingPlayList)
		CheckInGameMusic();
}

/*
#######################
  WOPMusicMenu_ClearList
#######################
*/
void WOPMusicMenu_ClearList(void)
{
	playOrder_t		*playOrderPTR;

	if(freeOrderItems==NULL)
	{
		freeOrderItems=wopmusicInfos.playOrder;
	}
	else
	{
		for(playOrderPTR=freeOrderItems;playOrderPTR->next!=NULL;playOrderPTR=playOrderPTR->next);

		playOrderPTR->next=wopmusicInfos.playOrder;
	}

	wopmusicInfos.playOrder=wopmusicInfos.lastplayOrderItem=NULL;

	wopmusicInfos.songstarted=0;
	trap_S_StopBackgroundTrack();
}

/*
#######################
  WOPMusicMenu_PlayAll
#######################
*/
void WOPMusicMenu_PlayAll(void)
{
	int i,j;

	WOPMusicMenu_ClearList();

	for(i=0;i<wopmusicInfos.numLPs;i++)
		for(j=0;j<wopmusicInfos.LPs[i].numTracks;j++)
			AddSongToPlayList(i,j,qfalse);
}

/*
#######################
  sortWoPMusicTracks
#######################
*/
int sortWoPMusicTracks(const void *TrackA, const void *TrackB)
{
	return Q_stricmp(((TrackInfo_t*)TrackA)->file,((TrackInfo_t*)TrackB)->file);
}

static qboolean wopmusic_inited=qfalse; // durch den sound-restart kommt er in den shutdown ohne den init gemacht zu haben =/

/*
#######################
  WOPMusicMenu_Init
#######################
*/
#define LPNAME_SIZE 32
void WOPMusicMenu_Init(void)
{
	char			lpdirlist[1024];
	char			trackdirlist[1024];
	char			*lpstrptr,*trstrptr;
	int				lpdirlen, numlpdirs, i;
	int				trdirlen, numtrdirs, j;
	char			lpname[LPNAME_SIZE];//reicht scho ;)
//	char			Stats[MAX_CVAR_VALUE_STRING];

	wopmusic_inited=qtrue;
	memset(&wopmusicInfos,0,sizeof(wopmusicInfos));

	memset(&playOrderMEM,0,sizeof(playOrderMEM));
	freeOrderItems=playOrderMEM;
	for(i=0;i<(MAX_TRACKS*MAX_LPS)-1;i++)
	{
		playOrderMEM[i].next=&playOrderMEM[i+1];
	}

	numlpdirs = trap_FS_GetFileList("wopmusic", "/", lpdirlist, 1024 );
	lpstrptr  = lpdirlist;
	if(numlpdirs>MAX_LPS) numlpdirs=MAX_LPS;
	for (i = 0; i < numlpdirs; i++, lpstrptr += lpdirlen+1)
	{
		int numTracks;

		if(*lpstrptr=='\0')
		{
			lpdirlen=0;
			continue;
		}

		lpdirlen = strlen(lpstrptr);
		if(!Q_stricmp(lpstrptr,".") || !Q_stricmp(lpstrptr,".."))
			continue;

		// '/' is missing if the directory isn't in a pk3 (using winXP)
		if(lpstrptr[lpdirlen-1]=='/')
			Q_strncpyz(lpname,lpstrptr,LPNAME_SIZE);
		else
			Com_sprintf(lpname,LPNAME_SIZE,"%s/",lpstrptr);
//		Q_strncpyz(wopmusicInfos.LPs[wopmusicInfos.numLPs].lpname,lpstrptr,lpdirlen<MAX_LPNAME?lpdirlen:MAX_LPNAME);
		Q_strncpyz(wopmusicInfos.LPs[wopmusicInfos.numLPs].lpname,lpstrptr,MAX_LPNAME);

		wopmusicInfos.LPs[wopmusicInfos.numLPs].bg = trap_R_RegisterShaderNoMip(va("wopmusic/%sbg.tga",lpname));

		numtrdirs = trap_FS_GetFileList(va("wopmusic/%s",lpname), ".wav", trackdirlist, 1024 );
		trstrptr  = trackdirlist;
		if(numtrdirs>MAX_TRACKS) numtrdirs=MAX_TRACKS;
		numTracks=0;
		for (j = 0; j < numtrdirs; j++, trstrptr += trdirlen+1)
		{
			if(*trstrptr=='\0') //komischer bug -.-
			{
				trdirlen=0;
				continue;
			}

			trdirlen = strlen(trstrptr);

			Com_sprintf(wopmusicInfos.LPs[wopmusicInfos.numLPs].tracks[numTracks].file,MAX_QPATH,"wopmusic/%s%s",lpname,trstrptr);

			wopmusicInfos.LPs[wopmusicInfos.numLPs].tracks[numTracks].lenght = ReadWaveLenght(wopmusicInfos.LPs[wopmusicInfos.numLPs].tracks[numTracks].file);

			Q_strncpyz(wopmusicInfos.LPs[wopmusicInfos.numLPs].tracks[numTracks].title,(char *)(trstrptr+3),MAX_TITLELENGHT);
			*strstr(wopmusicInfos.LPs[wopmusicInfos.numLPs].tracks[numTracks].title,".wav")='\0';
			numTracks++;
		}

//ogg-finder
		numtrdirs = trap_FS_GetFileList(va("wopmusic/%s",lpname), ".ogg", trackdirlist, 1024 );
		trstrptr  = trackdirlist;
		if(numtrdirs>MAX_TRACKS-numTracks) numtrdirs=MAX_TRACKS-numTracks; // numTracks is the number of wave-files that were found

		for (j = 0; j < numtrdirs; j++, trstrptr += trdirlen+1)
		{
			if(*trstrptr=='\0') //komischer bug -.-
			{
				trdirlen=0;
				continue;
			}

			trdirlen = strlen(trstrptr);

			Com_sprintf(wopmusicInfos.LPs[wopmusicInfos.numLPs].tracks[numTracks].file,MAX_QPATH,"wopmusic/%s%s",lpname,trstrptr);

//			wopmusicInfos.LPs[wopmusicInfos.numLPs].tracks[numTracks].lenght = ReadWaveLenght(wopmusicInfos.LPs[wopmusicInfos.numLPs].tracks[numTracks].file);
			wopmusicInfos.LPs[wopmusicInfos.numLPs].tracks[numTracks].lenght = -1;

			Q_strncpyz(wopmusicInfos.LPs[wopmusicInfos.numLPs].tracks[numTracks].title,(char *)(trstrptr+3),MAX_TITLELENGHT);
			*strstr(wopmusicInfos.LPs[wopmusicInfos.numLPs].tracks[numTracks].title,".ogg")='\0';
			numTracks++;
		}
//ogg-end
//mp3-finder
		numtrdirs = trap_FS_GetFileList(va("wopmusic/%s",lpname), ".mp3", trackdirlist, 1024 );
		trstrptr  = trackdirlist;
		if(numtrdirs>MAX_TRACKS-numTracks) numtrdirs=MAX_TRACKS-numTracks; // numTracks is the number of wave-files that were found

		for (j = 0; j < numtrdirs; j++, trstrptr += trdirlen+1)
		{
			if(*trstrptr=='\0') //komischer bug -.-
			{
				trdirlen=0;
				continue;
			}

			trdirlen = strlen(trstrptr);

			Com_sprintf(wopmusicInfos.LPs[wopmusicInfos.numLPs].tracks[numTracks].file,MAX_QPATH,"wopmusic/%s%s",lpname,trstrptr);

//			wopmusicInfos.LPs[wopmusicInfos.numLPs].tracks[numTracks].lenght = ReadWaveLenght(wopmusicInfos.LPs[wopmusicInfos.numLPs].tracks[numTracks].file);
			wopmusicInfos.LPs[wopmusicInfos.numLPs].tracks[numTracks].lenght = -1;

			Q_strncpyz(wopmusicInfos.LPs[wopmusicInfos.numLPs].tracks[numTracks].title,(char *)(trstrptr+3),MAX_TITLELENGHT);
			*strstr(wopmusicInfos.LPs[wopmusicInfos.numLPs].tracks[numTracks].title,".mp3")='\0';
			numTracks++;
		}
//mp3-end

		qsort(wopmusicInfos.LPs[wopmusicInfos.numLPs].tracks,numTracks,sizeof(TrackInfo_t),sortWoPMusicTracks);

		if(numTracks)
		{
			wopmusicInfos.LPs[wopmusicInfos.numLPs].numTracks=numTracks;
			wopmusicInfos.numLPs++;
		}
	}

/*
	for(i=0;i<wopmusicInfos.numLPs;i++)
	{
		Com_Printf("%i. LP: bghandle=%i\n",i+1,wopmusicInfos.LPs[i].bg);
		for(j=0;j<wopmusicInfos.LPs[i].numTracks;j++)
		{
//			Com_Printf("  %i. %-32s %i (%s)\n",j+1,wopmusicInfos.LPs[i].title[j],wopmusicInfos.LPs[i].tracklenght[j],wopmusicInfos.LPs[i].trackfile[j]);
			Com_Printf("  %i. %-32s %i\n",j+1,wopmusicInfos.LPs[i].title[j],wopmusicInfos.LPs[i].tracklenght[j]);
		}
	}
*/

/*	trap_Cvar_VariableStringBuffer("wop_musicStats",Stats,MAX_CVAR_VALUE_STRING);

	for(i=0;i+2<MAX_CVAR_VALUE_STRING;i+=3)
	{
		int LP, Track;

		if(!Stats[i] || !Stats[i+1] || !Stats[i+2])
			break;

		Track=(Stats[i]-'0')*10+(Stats[i+1]-'0');
		LP=(Stats[i+2]-'0');

		AddSongToPlayList(LP,Track,qtrue);
	}
*/

	if(wopmusicInfos.playOrder==NULL)
	{
		fileHandle_t f;
		char playlist[2048];//alle momentanen tracks zusammen sind ca. 700 byte
		char TrackName[MAX_QPATH];
		char *chrptr;
		int	i,j,k;

		trap_FS_FOpenFile( "playlist.dat", &f, FS_READ );
		if(f)
		{
			memset(playlist,0,sizeof(playlist));
			trap_FS_Read(playlist,2047,f);
			trap_FS_FCloseFile(f);

			i=0;
			for(chrptr=playlist;*chrptr!='\0';chrptr++)
			{
				if(*chrptr!='\n')
					TrackName[i++]=*chrptr;
				else
				{
					TrackName[i]='\0';
					for(j=0;j<wopmusicInfos.numLPs;j++)
						for(k=0;k<wopmusicInfos.LPs[j].numTracks;k++)
							if(!Q_stricmp(wopmusicInfos.LPs[j].tracks[k].file,TrackName))
							{
								AddSongToPlayList(j,k,qtrue);
								goto doubleBrake;
							}

doubleBrake:
					i=0;
				}
			}
		}
		else //only if there is realy no playlist
		{	// no playlist -> play all (so new players can hear the music feature ;P )
			WOPMusicMenu_PlayAll();
		}
	}
}

/*
#######################
  WOPMusicMenu_Shutdown

  [TT][L] -> 3 ... 15*5*3=225, die maximale liste passt in eine cvar(MAX_CVAR_VALUE_STRING=256)
#######################
*/
void WOPMusicMenu_Shutdown(void)
{
	char		Stats[MAX_CVAR_VALUE_STRING];
	playOrder_t *playOrderPTR;
	int			i;
	int			fileopen,size;
	fileHandle_t f;

	if(!wopmusic_inited)	return; //ohne init macht der shutdown keinen sinn ...

	Stats[0]='\0';

	fileopen=trap_FS_FOpenFile("playlist.dat", &f, FS_WRITE);

	for(playOrderPTR=wopmusicInfos.playOrder,i=0;playOrderPTR!=NULL;playOrderPTR=playOrderPTR->next,i+=3)
	{
/*		if(i+3>255)//string zu kurz
		{
			Stats[i]='\0';
			break;
		}

		Stats[i]=((playOrderPTR->Track/10)%10)+'0';
		Stats[i+1]=(playOrderPTR->Track%10)+'0';
		Stats[i+2]=(playOrderPTR->LP%10)+'0';
*/
		if (fileopen >= 0) {
			size = strlen(wopmusicInfos.LPs[playOrderPTR->LP].tracks[playOrderPTR->Track].file)+1;// +1 => '\n'
			trap_FS_Write(va("%s\n",wopmusicInfos.LPs[playOrderPTR->LP].tracks[playOrderPTR->Track].file), size, f);
		}

//		if(playOrderPTR->next==NULL)
//			Stats[i+3]='\0';
	}

	if(fileopen >= 0)
		trap_FS_FCloseFile(f);

//	trap_Cvar_Set("wop_musicStats",Stats);
}

/*
#######################
  WOPMusicNextSong
#######################
*/
void WOPMusicNextSong(void)
{
	playOrder_t	*playOrderPTR;

	if(wopmusicInfos.playOrder==NULL)
		return;

	// wenn es mehr als einen Track gibt, müssen wir weiter rotieren
	if(wopmusicInfos.playOrder!=wopmusicInfos.lastplayOrderItem)
	{
		playOrderPTR=wopmusicInfos.playOrder;
		wopmusicInfos.playOrder=playOrderPTR->next;
		playOrderPTR->next=NULL;
		wopmusicInfos.lastplayOrderItem->next=playOrderPTR;
		wopmusicInfos.lastplayOrderItem=playOrderPTR;
	}

	Com_Printf("[WoP Music] Playing \"%s\"\n",wopmusicInfos.LPs[wopmusicInfos.playOrder->LP].tracks[wopmusicInfos.playOrder->Track].title);

	trap_S_StartBackgroundTrack(wopmusicInfos.LPs[wopmusicInfos.playOrder->LP].tracks[wopmusicInfos.playOrder->Track].file,"<nextsongCMD>");
}

void TriggerInGameMusicRestart() {
	wopmusicInfos.songstarted=qfalse;
}

/*
#######################
  CheckInGameMusic
#######################
*/
void CheckInGameMusic(void)
{
//	int			ms;
//	playOrder_t	*playOrderPTR;
	int tracktime;

	if(uis.musicbool==qtrue) return;

	if(wopmusicInfos.playOrder==NULL) return;

//	ms=trap_Milliseconds();

	if(wopmusicInfos.songstarted==0)
	{
		if(wop_AutoswitchSongByNextMap.integer) {
			WOPMusicNextSong();
		}
		else {
			trap_S_StartBackgroundTrack(wopmusicInfos.LPs[wopmusicInfos.playOrder->LP].tracks[wopmusicInfos.playOrder->Track].file,"<nextsongCMD>");
			tracktime=wopmusicInfos.LPs[wopmusicInfos.playOrder->LP].tracks[wopmusicInfos.playOrder->Track].lenght;
	//		Com_Printf("[WoP Music] Playing \"%s\" (%i:%i%i)\n",wopmusicInfos.LPs[wopmusicInfos.playOrder->LP].tracks[wopmusicInfos.playOrder->Track].title,(tracktime/1000/60)%10,(tracktime/10000)%6,(tracktime/1000)%10);
			Com_Printf("[WoP Music] Playing \"%s\"\n",wopmusicInfos.LPs[wopmusicInfos.playOrder->LP].tracks[wopmusicInfos.playOrder->Track].title);
		}
//		wopmusicInfos.songstarted=ms;
		wopmusicInfos.songstarted=qtrue;
	}
/* old-track-end detection ... now we will use "WOPMusicNextSong" (started by engine ... via cmd)

	else if((ms-wopmusicInfos.songstarted)+500>=(wopmusicInfos.LPs[wopmusicInfos.playOrder->LP].tracks[wopmusicInfos.playOrder->Track].lenght))
	{
		if(wopmusicInfos.playOrder==wopmusicInfos.lastplayOrderItem)
			wopmusicInfos.songstarted=ms;
		else
		{
			playOrderPTR=wopmusicInfos.playOrder;
			wopmusicInfos.playOrder=playOrderPTR->next;
			playOrderPTR->next=NULL;
			wopmusicInfos.lastplayOrderItem->next=playOrderPTR;
			wopmusicInfos.lastplayOrderItem=playOrderPTR;

			trap_S_StartBackgroundTrack(wopmusicInfos.LPs[wopmusicInfos.playOrder->LP].tracks[wopmusicInfos.playOrder->Track].file,"");
			wopmusicInfos.songstarted=ms;
		}

		tracktime=wopmusicInfos.LPs[wopmusicInfos.playOrder->LP].tracks[wopmusicInfos.playOrder->Track].lenght;
		Com_Printf("[WoP Music] Playing \"%s\" (%i:%i%i)\n",wopmusicInfos.LPs[wopmusicInfos.playOrder->LP].tracks[wopmusicInfos.playOrder->Track].title,(tracktime/1000/60)%10,(tracktime/10000)%6,(tracktime/1000)%10);
	}
*/
}

/*
#######################
  WOPMusicMenu_Event
#######################
*/
void WOPMusicMenu_Event( void *ptr, int notification ) {
//	int i;

	if( notification != QM_ACTIVATED ) {
		return;
	}

	switch( ((menucommon_s*)ptr)->id ) {
	case ID_EXIT:
//		UI_PopMenu();
		if(wopmusicmenu.switching!=-2)
			wopmusicmenu.switchtimer=uis.realtime;
		wopmusicmenu.switching=-2;
		break;
	case ID_SWITCH:
		if(wopmusicInfos.numLPs<2 || wopmusicmenu.switching!=0) return;

		wopmusicmenu.switching=-1;
		wopmusicmenu.switchtimer=uis.realtime;

/* old (ohne animation):
		if(wopmusicmenu.currentLP+1>=wopmusicInfos.numLPs)
			wopmusicmenu.currentLP=0;
		else
			wopmusicmenu.currentLP++;

		for(i=0;i<MAX_TRACKS;i++)
		{
			if(i>=wopmusicInfos.LPs[wopmusicmenu.currentLP].numTracks)
				wopmusicmenu.tracks[i].generic.flags	|= QMF_HIDDEN|QMF_INACTIVE;
			else
			{
				wopmusicmenu.tracks[i].generic.flags	&= ~(QMF_HIDDEN|QMF_INACTIVE);
				wopmusicmenu.tracks[i].generic.right	= wopmusicmenu.tracks[i].generic.left + strlen(wopmusicInfos.LPs[wopmusicmenu.currentLP].tracks[i].title)*SMALLCHAR_WIDTH;
			}
			wopmusicmenu.tracks[i].string			= wopmusicInfos.LPs[wopmusicmenu.currentLP].tracks[i].title;
		}
*/

		break;
	case ID_CLEARLIST:
		WOPMusicMenu_ClearList();

		break;
	case ID_PLAYALL:
		WOPMusicMenu_PlayAll();
		break;
	default:
		if(((menucommon_s*)ptr)->id>=ID_TRACK1 && ((menucommon_s*)ptr)->id<ID_TRACK1+MAX_TRACKS)
		{
			AddSongToPlayList(wopmusicmenu.currentLP,((menucommon_s*)ptr)->id-ID_TRACK1,qfalse);
			break;
		}
	}
}

/*
#######################
  WOPMusicMenu_Draw
#######################
*/
#define FACTOR1024 (1024.0f/640.0f)
static void WOPMusicMenu_Draw( void )
{
	int i;
	float switchoffset = 0;

	if(wopmusicmenu.switching!=0)
	{
		if(wopmusicmenu.switching<0)//==-1(switch out) and -2(popmenu)
			switchoffset=uis.realtime-wopmusicmenu.switchtimer;
		else if(wopmusicmenu.switching>0)//==1
			switchoffset=1000-(uis.realtime-wopmusicmenu.switchtimer);
		switchoffset*=switchoffset;
		switchoffset*=0.001f;

		for(i=0;i<MAX_TRACKS;i++)
			wopmusicmenu.tracks[i].generic.x	= 188+switchoffset;
//		wopmusicmenu.switchLPs.generic.x		= 640+switchoffset;
//		wopmusicmenu.clearlist.generic.x		= 640+switchoffset;
//		wopmusicmenu.playall.generic.x			= 640+switchoffset;
//		wopmusicmenu.exit.generic.x				= 640+switchoffset;
		wopmusicmenu.switchLPs.x		= 925+switchoffset*FACTOR1024;
		wopmusicmenu.clearlist.x		= 958+switchoffset*FACTOR1024;
		wopmusicmenu.playall.x			= 925+switchoffset*FACTOR1024;
		wopmusicmenu.exit.x				= 954+switchoffset*FACTOR1024;

		if((uis.realtime-wopmusicmenu.switchtimer)>1000)
		{
			if(wopmusicmenu.switching==-1)
			{
				trap_S_StartLocalSound( menu_switch_sound, CHAN_LOCAL_SOUND );
				wopmusicmenu.switching=1;
				wopmusicmenu.switchtimer=uis.realtime;

				if(wopmusicmenu.currentLP+1>=wopmusicInfos.numLPs)
					wopmusicmenu.currentLP=0;
				else
					wopmusicmenu.currentLP++;

				for(i=0;i<MAX_TRACKS;i++)
				{
					if(i>=wopmusicInfos.LPs[wopmusicmenu.currentLP].numTracks)
						wopmusicmenu.tracks[i].generic.flags	|= QMF_HIDDEN|QMF_INACTIVE;
					else
					{
						wopmusicmenu.tracks[i].generic.flags	&= ~(QMF_HIDDEN|QMF_INACTIVE);
						wopmusicmenu.tracks[i].generic.right	= wopmusicmenu.tracks[i].generic.left + strlen(wopmusicInfos.LPs[wopmusicmenu.currentLP].tracks[i].title)*SMALLCHAR_WIDTH;
					}
					wopmusicmenu.tracks[i].string			= wopmusicInfos.LPs[wopmusicmenu.currentLP].tracks[i].title;
				}
			}
			else if(wopmusicmenu.switching==1)
				wopmusicmenu.switching=0;
			else if(wopmusicmenu.switching==-2)
				UI_PopMenu();
		}
	}
	else
	{
		for(i=0;i<MAX_TRACKS;i++)
			wopmusicmenu.tracks[i].generic.x	= 188;
		wopmusicmenu.switchLPs.x		= 925;//640;
		wopmusicmenu.clearlist.x		= 958;//640;
		wopmusicmenu.playall.x			= 925;//640;
		wopmusicmenu.exit.x				= 954;//640;
	}

	// right 495 ...
	if(wopmusicInfos.LPs[wopmusicmenu.currentLP].bg)
//		UI_DrawHandlePic( 130+switchoffset, 61, 510, 338, wopmusicInfos.LPs[wopmusicmenu.currentLP].bg );
		UI_DrawHandlePic( 130+switchoffset, 61, 366, 338, wopmusicInfos.LPs[wopmusicmenu.currentLP].bg );
	else
	{
		UI_FillRect(160+switchoffset, 60, 335, 340,colorBlack);
		UI_DrawStringNS(170+switchoffset,62,wopmusicInfos.LPs[wopmusicmenu.currentLP].lpname,0,16.0f,colorWhite);
	}

	UI_DrawHandlePic(433+switchoffset,68,500,315,wopmusicmenu.bg_arm);

	if(wopmusicInfos.playOrder!=NULL)
	{
		playOrder_t		*playOrderPTR;
		int				i;

		for(playOrderPTR=wopmusicInfos.playOrder,i=1;playOrderPTR!=NULL;playOrderPTR=playOrderPTR->next,i++)
		{
			if(playOrderPTR->LP==wopmusicmenu.currentLP)
				UI_DrawString(188+switchoffset,82+playOrderPTR->Track*18,va("%2i.",i),UI_RIGHT|UI_SMALLFONT,colorWhite);
		}
	}

	// standard menu drawing
	Menu_Draw( &wopmusicmenu.menu );
}

/*
#######################
  WOPMusicMenu_Key
#######################
*/
static sfxHandle_t WOPMusicMenu_Key( int key )
{

	switch(key)
	{
	case K_ESCAPE:
	case K_MOUSE2:
	case 'm':
	case 'M':
//		UI_PopMenu();
//		return menu_out_sound;
		if(wopmusicmenu.switching!=-2)
			wopmusicmenu.switchtimer=uis.realtime;
		wopmusicmenu.switching=-2;
		return 0;
		break;
	case 's':
	case 'S':
		if(wopmusicInfos.numLPs>1 && wopmusicmenu.switching==0)
		{
			wopmusicmenu.switching=-1;
			wopmusicmenu.switchtimer=uis.realtime;
		}
		return 0;
		break;
	}

	return ( Menu_DefaultKey( &wopmusicmenu.menu, key ) );
}


/*
#######################
  WOPMusicMenu_Open
#######################
*/
void WOPMusicMenu_Open( void )
{
	int		y, i;

	memset( &wopmusicmenu, 0 ,sizeof(wopmusicmenu) );
	uis.musicbool=2;//weil beim cmd aufruf cl_paused nicht gesetzt wird
					//2->music kommt vom music-menu ... 1->music ist ui-bgmusic

	trap_Cvar_Set( "cl_paused", "1" );//wenn musicbool auf 2 ist sollt das nicht's ausmachen

	WOPMusicMenu_Cache();

	wopmusicmenu.menu.wrapAround = qtrue;
	wopmusicmenu.menu.fullscreen = qfalse;
	wopmusicmenu.menu.draw		 = WOPMusicMenu_Draw;
	wopmusicmenu.menu.key		 = WOPMusicMenu_Key;

	y = 82;
	for(i=0;i<MAX_TRACKS;i++)
	{
		wopmusicmenu.tracks[i].generic.type		= MTYPE_TEXTS;
		wopmusicmenu.tracks[i].fontHeight		= 16.0f;
//		wopmusicmenu.tracks[i].generic.flags	= QMF_LEFT_JUSTIFY;
		if(i>=wopmusicInfos.LPs[wopmusicmenu.currentLP].numTracks)
			wopmusicmenu.tracks[i].generic.flags	= QMF_HIDDEN|QMF_INACTIVE;
		else
			wopmusicmenu.tracks[i].generic.flags	= 0;
		wopmusicmenu.tracks[i].generic.x		= 188;
		wopmusicmenu.tracks[i].generic.y		= y;
		wopmusicmenu.tracks[i].generic.id		= ID_TRACK1+i;
		wopmusicmenu.tracks[i].generic.callback	= WOPMusicMenu_Event;
		wopmusicmenu.tracks[i].string			= wopmusicInfos.LPs[wopmusicmenu.currentLP].tracks[i].title;
		wopmusicmenu.tracks[i].color			= color_white;
		wopmusicmenu.tracks[i].style			= UI_LEFT|UI_SMALLFONT;

		Menu_AddItem( &wopmusicmenu.menu, &wopmusicmenu.tracks[i] );

		y+=18;
	}

	wopmusicmenu.switchLPs.x		= 925;
	wopmusicmenu.clearlist.x		= 958;
	wopmusicmenu.playall.x			= 925;
	wopmusicmenu.exit.x				= 954;

	wopmusicmenu.switchLPs.generic.type		= MTYPE_BITMAP1024S;
	if(wopmusicInfos.numLPs<2)
		wopmusicmenu.switchLPs.generic.flags = QMF_HIDDEN|QMF_INACTIVE;
	wopmusicmenu.switchLPs.y				= 348;//y;
	wopmusicmenu.switchLPs.generic.id		= ID_SWITCH;
	wopmusicmenu.switchLPs.generic.callback	= WOPMusicMenu_Event;
	wopmusicmenu.switchLPs.w				= 76;
	wopmusicmenu.switchLPs.h				= 19;
	wopmusicmenu.switchLPs.shader			= trap_R_RegisterShaderNoMip("wopmusic/switch_button_off.tga");
	wopmusicmenu.switchLPs.mouseovershader	= trap_R_RegisterShaderNoMip("wopmusic/switch_button_on.tga");
	Menu_AddItem( &wopmusicmenu.menu, &wopmusicmenu.switchLPs );

	wopmusicmenu.clearlist.generic.type		= MTYPE_BITMAP1024S;
	wopmusicmenu.clearlist.y				= 369;//y;
	wopmusicmenu.clearlist.generic.id		= ID_CLEARLIST;
	wopmusicmenu.clearlist.generic.callback	= WOPMusicMenu_Event;
	wopmusicmenu.clearlist.w				= 43;
	wopmusicmenu.clearlist.h				= 19;
	wopmusicmenu.clearlist.shader			= trap_R_RegisterShaderNoMip("wopmusic/clear_button_off.tga");
	wopmusicmenu.clearlist.mouseovershader	= trap_R_RegisterShaderNoMip("wopmusic/clear_button_on.tga");
	Menu_AddItem( &wopmusicmenu.menu, &wopmusicmenu.clearlist );

	wopmusicmenu.playall.generic.type	= MTYPE_BITMAP1024S;
	wopmusicmenu.playall.y				= 369;//y;
	wopmusicmenu.playall.generic.id		= ID_PLAYALL;
	wopmusicmenu.playall.generic.callback	= WOPMusicMenu_Event;
	wopmusicmenu.playall.w				= 31;
	wopmusicmenu.playall.h				= 19;
	wopmusicmenu.playall.shader			= trap_R_RegisterShaderNoMip("wopmusic/all_button_off.tga");
	wopmusicmenu.playall.mouseovershader= trap_R_RegisterShaderNoMip("wopmusic/all_button_on.tga");
	Menu_AddItem( &wopmusicmenu.menu, &wopmusicmenu.playall );

	wopmusicmenu.exit.generic.type		= MTYPE_BITMAP1024S;
	wopmusicmenu.exit.y					= 413;//y;
	wopmusicmenu.exit.generic.id		= ID_EXIT;
	wopmusicmenu.exit.generic.callback	= WOPMusicMenu_Event;
	wopmusicmenu.exit.w					=
	wopmusicmenu.exit.h					= 55;
	wopmusicmenu.exit.shader			= trap_R_RegisterShaderNoMip("wopmusic/exit_button_off.tga");
	wopmusicmenu.exit.mouseovershader	= trap_R_RegisterShaderNoMip("wopmusic/exit_button_on.tga");
	Menu_AddItem( &wopmusicmenu.menu, &wopmusicmenu.exit );

	UI_PushMenu( &wopmusicmenu.menu );

	//switch in ^^
	wopmusicmenu.switching=1;
	wopmusicmenu.switchtimer=uis.realtime+1000;
}


/*
#######################
  WOPMusicMenu_Cache
#######################
*/
void WOPMusicMenu_Cache( void )
{
	wopmusicmenu.bg_arm = trap_R_RegisterShaderNoMip( MUSIC_BG_ARM );
}
