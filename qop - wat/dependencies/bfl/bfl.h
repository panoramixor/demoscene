//
// Copyright (c) 2000, Bernhard Tschirren
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// 1. Redistributions of the source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
// 2. Any redistribution solely in binary form must conspicuously
//    reproduce the following disclaimer in documentation provided with
//    the binary redistribution.
//
// THIS SOFTWARE IS PROVIDED ``AS IS'', WITHOUT ANY WARRANTIES, EXPRESS
// OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  LICENSOR SHALL
// NOT BE LIABLE FOR ANY LOSS OR DAMAGES RESULTING FROM THE USE OF THIS
// SOFTWARE, EITHER ALONE OR IN COMBINATION WITH ANY OTHER SOFTWARE.
//

#ifndef _BT_BFL_H_
#define _BT_BFL_H_

#ifdef _WIN32
#  define  WIN32_LEAN_AND_MEAN
#  include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>


#ifdef __cplusplus
extern "C" {
#endif


#define BFL_LIBRARY_VERSION       0x0100
#define BFL_NUM_CHARACTERS        128


/* define BFL_API */
#ifdef _WIN32
#  ifdef BFDLL_EXPORTS
#    define BFL_API __declspec(dllexport)
#  else
#    define BFL_API __declspec(dllimport)
#  endif
#else
#  define BFL_API
#endif


/* define bool */
#if !defined(__cplusplus) && !defined(bool)
#  define false (0)
#  define true  (!false)
#  define bool  int
#endif


typedef struct
  {
  GLuint  name;
  GLfloat minS;
  GLfloat maxS;
  GLfloat minT;
  GLfloat maxT;
  } BFL_GLYPH_TEXTURE;

typedef struct
  {
  int               character;
  int               width;
  int               height;
  int               depth;  /* in bits  */
  int               originX;
  int               originY;
  int               advanceX;
  int               advanceY;
  unsigned char    *data;
  BFL_GLYPH_TEXTURE texture;
  } BFL_GLYPH;

typedef struct
  {
  char       description[32];
  BFL_GLYPH *glyphs[BFL_NUM_CHARACTERS];
  } BFL_FONT;

/* Callback function type */
typedef void (*BFL_CALLBACK)(const char *message, ...);

/* Callback functions */
extern BFL_API BFL_CALLBACK BFL_Fatal;
extern BFL_API BFL_CALLBACK BFL_Error;

/* function protoypes */
extern BFL_API void       BFL_SetFatalCallback(BFL_CALLBACK function);
extern BFL_API void       BFL_SetErrorCallback(BFL_CALLBACK function);
extern BFL_API void      *BFL_Malloc(int size);
extern BFL_API void       BFL_Free(void *ptr);

extern BFL_API BFL_GLYPH *BFL_LoadGlyph(FILE *file);
extern BFL_API bool       BFL_SaveGlyph(FILE *file, const BFL_GLYPH *glyph);
extern BFL_API void       BFL_DestroyGlyph(BFL_GLYPH *glyph);

extern BFL_API BFL_FONT  *BFL_LoadFont(FILE *file);
extern BFL_API bool       BFL_SaveFont(FILE *file, const BFL_FONT *font);
extern BFL_API void       BFL_DestroyFont(BFL_FONT *font);

extern BFL_API int        BFL_GetNumGlyphs(const BFL_FONT *font);
extern BFL_API BFL_GLYPH *BFL_GetGlyph(const BFL_FONT *font, int character);
extern BFL_API bool       BFL_AddGlyph(BFL_FONT *font, BFL_GLYPH *glyph);
extern BFL_API bool       BFL_RemoveGlyph(BFL_FONT *font, BFL_GLYPH *glyph);
extern BFL_API bool       BFL_CharacterExists(const BFL_FONT *font, int character);

extern BFL_API bool       BFL_GetGlyphExtents(const BFL_GLYPH *glyph,                          int *x1, int *y1, int *x2, int *y2, int *advanceX, int *advanceY);
extern BFL_API bool       BFL_GetCharacterExtents(const BFL_FONT *font, int character,         int *x1, int *y1, int *x2, int *y2, int *advanceX, int *advanceY);
extern BFL_API bool       BFL_GetStringExtents(const BFL_FONT *font, char *string, int length, int *x1, int *y1, int *x2, int *y2, int *advanceX, int *advanceY);

extern BFL_API bool       BFL_GenerateGlyphTexture(BFL_GLYPH *glyph);
extern BFL_API bool       BFL_GenerateFontTexture(BFL_FONT *font);

extern BFL_API bool       BFL_SetBlendMode(int blendMode);
extern BFL_API BFL_GLYPH *BFL_BindCharacter(BFL_FONT *font, int character);
extern BFL_API bool       BFL_DrawCharacter(BFL_FONT *font, int character);
extern BFL_API bool       BFL_DrawString(BFL_FONT *font, const char *string, int length);
extern BFL_API bool       BFL_DrawColorCharacter(BFL_FONT *font, int character, GLfloat *colors);
extern BFL_API bool       BFL_DrawColorString(BFL_FONT *font, const char *string, int length, GLfloat *colors);

#ifdef __cplusplus
}
#endif

#endif /* _BT_BFL_H_ */