// minifmod xm player

#include "stdafx.h"

#define USEMEMLOAD
#define USEMEMLOADRESOURCE

#define USEFMOD TRUE

#ifdef USEFMOD
	#include "minifmod.h"
#endif

#ifdef USEFMOD
	FMUSIC_MODULE *mod;


#ifndef USEMEMLOAD

unsigned int fileopen(char *name)
{
	return (unsigned int)fopen(name, "rb");
}

void fileclose(unsigned int handle)
{
	fclose((FILE *)handle);
}

int fileread(void *buffer, int size, unsigned int handle)
{
	return fread(buffer, 1, size, (FILE *)handle);
}

void fileseek(unsigned int handle, int pos, signed char mode)
{
	fseek((FILE *)handle, pos, mode);
}

int filetell(unsigned int handle)
{
	return ftell((FILE *)handle);
}

#else

typedef struct 
{
	int length;
	int pos;
	void *data;
} MEMFILE;


unsigned int memopen(char *name)
{
	MEMFILE *memfile;

	memfile = (MEMFILE *)calloc(sizeof(MEMFILE),1);

#ifndef USEMEMLOADRESOURCE
	{	// load an external file and read it
		FILE *fp;
		fp = fopen(name, "rb");
		if (fp)
		{
			fseek(fp, 0, SEEK_END);
			memfile->length = ftell(fp);
			memfile->data = calloc(memfile->length,1);
			memfile->pos = 0;

			fseek(fp, 0, SEEK_SET);
			fread(memfile->data, 1, memfile->length, fp);
			fclose(fp);
		}
	}
#else
	{	// hey look some load from resource code!
		HRSRC		rec;
		HGLOBAL		handle;

		rec = FindResource(NULL, name, RT_RCDATA);
		handle = LoadResource(NULL, rec);
		
		memfile->data = LockResource(handle);
		memfile->length = SizeofResource(NULL, rec);
		memfile->pos = 0;
	}
#endif

	return (unsigned int)memfile;
}

void memclose(unsigned int handle)
{
	MEMFILE *memfile = (MEMFILE *)handle;

#ifndef USEMEMLOADRESOURCE
	free(memfile->data);			// dont free it if it was initialized with LockResource
#endif

	free(memfile);
}

int memread(void *buffer, int size, unsigned int handle)
{
	MEMFILE *memfile = (MEMFILE *)handle;

	if (memfile->pos + size >= memfile->length)
		size = memfile->length - memfile->pos;

	memcpy(buffer, (char *)memfile->data+memfile->pos, size);
	memfile->pos += size;
	
	return size;
}

void memseek(unsigned int handle, int pos, signed char mode)
{
	MEMFILE *memfile = (MEMFILE *)handle;

	if (mode == SEEK_SET) 
		memfile->pos = pos;
	else if (mode == SEEK_CUR) 
		memfile->pos += pos;
	else if (mode == SEEK_END)
		memfile->pos = memfile->length + pos;

	if (memfile->pos > memfile->length)
		memfile->pos = memfile->length;
}

int memtell(unsigned int handle)
{
	MEMFILE *memfile = (MEMFILE *)handle;

	return memfile->pos;
}
#endif


void Init_Music()
{

#ifndef USEMEMLOAD
	FSOUND_File_SetCallbacks(fileopen, fileclose, fileread, fileseek, filetell);
#else
	FSOUND_File_SetCallbacks(memopen, memclose, memread, memseek, memtell);
#endif
#endif

	
	FSOUND_File_SetCallbacks(memopen, memclose, memread, memseek, memtell);
	
	// ==========================================================================================
	// INITIALIZE
	// ==========================================================================================
	if (!FSOUND_Init(44100, NULL))
	{
		MessageBox(NULL, "sound player error!", "sound player can't initialize default device", MB_OK);
		return;
	}

	// ==========================================================================================
	// LOAD SONG
	// ==========================================================================================
	mod = FMUSIC_LoadSong("tune", NULL); //sampleloadcallback);
	if (!mod)
	{
		MessageBox(NULL, "can't locate song!", "module not found!", MB_OK);
//		return;
	}

	// ==========================================================================================
	// PLAY SONG
	// ==========================================================================================
	FMUSIC_PlaySong(mod);
}


void Stop_Music()
{
	FMUSIC_FreeSong(mod);
	FSOUND_Close();
}

