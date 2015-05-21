/*
	Copyright (c) 2014 Rafal "Gamer_Z" Grasman - All Rights Reserved
	* Unauthorized copying of this file, via any medium is strictly prohibited
	* Unauthorized execution of this program is strictly prohibited
	* Proprietary and confidential, if obtained without permission - destroy or delete immediately from your device
	* Written by Rafal Grasman <grasmanek94@gmail.com>, October 2014
	*/
#pragma warning(disable: 4305)

#include <sampgdk.h>

#undef GetTickCount
#undef SelectObject
#undef SetTimer
#undef KillTimer
#undef CreateMenu
#undef DestroyMenu

//----------------------------------------------------------
//
//   SA:MP Multiplayer Modification For GTA:SA
//   Copyright 2004-2007 SA:MP Team
//
//----------------------------------------------------------

//
// This provides an interface to call amx library functions
// within samp-server.
//
// To use:
//    Define the extern in your main source file:
//        extern void *pAMXFunctions;
//    And, in your Initialize function, assign:
//        pAMXFunctions = data[PLUGIN_DATA_AMX];
//
//
// WIN32: To regenerate thunks for calls from prototypes in amx.h
// Run a regex with:
//   S: ^(.*)(AMXAPI amx_)([^(]*)([^\;]*);$
//   R: NUDE \1\2\3\4\n{\n\t_asm mov eax, pAMXFunctions;\n\t_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_\3*4];\n}\n
//
// LINUX/BSD: To regenerate thunks for calls from prototypes in amx.h
// Run a regex with:
//   S: ^(.*)(AMXAPI amx_)([^(]*)([^\;]*);$
//   R: typedef \1 AMXAPI (*amx_\3_t)\4;\n\1\2\3\4\n{\n\tamx_\3_t fn = ((amx_\3_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_\3];\n\treturn fn\4;\n}\n
// Modify internal function calls as nessesary
//

//----------------------------------------------------------

#include "plugin.h"
//----------------------------------------------------------
/* Extremely inefficient but portable POSIX getch() */
#ifndef WIN32

#include <stdio.h>
#include <string.h>
#include <termios.h>    /* for tcgetattr() and tcsetattr() */
#include <unistd.h>     /* for read() */
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>

#ifndef STDIN_FILENO
# define STDIN_FILENO 0
#endif

int
getch(void)
{
	struct termios save_termios;
	struct termios ios;
	int c = 0;

	if (!isatty(STDIN_FILENO))
		return EOF;

	if (tcgetattr(STDIN_FILENO, &save_termios) < 0)
		return EOF;

	ios = save_termios;
	ios.c_lflag &= ~(ICANON | ECHO | ISIG);
	ios.c_cc[VMIN] = 1;           /* read() will return with one char */
	ios.c_cc[VTIME] = 0;          /* read() blocks forever */

	if (tcsetattr(STDIN_FILENO, TCSANOW, &ios) < 0)
		return EOF;

	if (read(STDIN_FILENO, &c, 1) != 1)
		c = EOF;

	tcsetattr(STDIN_FILENO, TCSANOW, &save_termios);

	return c;
}

int
kbhit(void)
{
	struct termios save_termios;
	struct termios ios;
	fd_set inp;
	struct timeval timeout = { 0, 0 };
	int result;

	if (!isatty(STDIN_FILENO))
		return 0;

	if (tcgetattr(STDIN_FILENO, &save_termios) < 0)
		return 0;

	ios = save_termios;
	ios.c_lflag &= ~(ICANON | ECHO | ISIG);
	ios.c_cc[VMIN] = 1;           /* read() will return with one char */
	ios.c_cc[VTIME] = 0;          /* read() blocks forever */

	if (tcsetattr(STDIN_FILENO, TCSANOW, &ios) < 0)
		return 0;

	/* set up select() args */
	FD_ZERO(&inp);
	FD_SET(STDIN_FILENO, &inp);

	result = select(STDIN_FILENO + 1, &inp, NULL, NULL, &timeout) == 1;

	tcsetattr(STDIN_FILENO, TCSANOW, &save_termios);

	return result;
}

#endif

//----------------------------------------------------------

void *pAMXFunctions;

//----------------------------------------------------------

#if (defined(WIN32) || defined(_WIN32)) && defined(_MSC_VER)

// Optimized Inline Assembly Thunks for MS VC++

#define NUDE _declspec(naked) 

NUDE uint16_t * AMXAPI amx_Align16(uint16_t *v)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_Align16 * 4];
}

NUDE uint32_t * AMXAPI amx_Align32(uint32_t *v)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_Align32 * 4];
}

#if defined _I64_MAX || defined HAVE_I64
NUDE   uint64_t * AMXAPI amx_Align64(uint64_t *v)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_Align64 * 4];
}

#endif
NUDE int AMXAPI amx_Allot(AMX *amx, int cells, cell *amx_addr, cell **phys_addr)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_Allot * 4];
}

NUDE int AMXAPI amx_Callback(AMX *amx, cell index, cell *result, cell *params)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_Callback * 4];
}

NUDE int AMXAPI amx_Cleanup(AMX *amx)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_Cleanup * 4];
}

NUDE int AMXAPI amx_Clone(AMX *amxClone, AMX *amxSource, void *data)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_Clone * 4];
}

NUDE int AMXAPI amx_Exec(AMX *amx, cell *retval, int index)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_Exec * 4];
}

NUDE int AMXAPI amx_FindNative(AMX *amx, const char *name, int *index)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_FindNative * 4];
}

NUDE int AMXAPI amx_FindPublic(AMX *amx, const char *funcname, int *index)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_FindPublic * 4];
}

NUDE int AMXAPI amx_FindPubVar(AMX *amx, const char *varname, cell *amx_addr)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_FindPubVar * 4];
}

NUDE int AMXAPI amx_FindTagId(AMX *amx, cell tag_id, char *tagname)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_FindTagId * 4];
}

NUDE int AMXAPI amx_Flags(AMX *amx, uint16_t *flags)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_Flags * 4];
}

NUDE int AMXAPI amx_GetAddr(AMX *amx, cell amx_addr, cell **phys_addr)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_GetAddr * 4];
}

NUDE int AMXAPI amx_GetNative(AMX *amx, int index, char *funcname)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_GetNative * 4];
}

NUDE int AMXAPI amx_GetPublic(AMX *amx, int index, char *funcname)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_GetPublic * 4];
}

NUDE int AMXAPI amx_GetPubVar(AMX *amx, int index, char *varname, cell *amx_addr)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_GetPubVar * 4];
}

NUDE int AMXAPI amx_GetString(char *dest, const cell *source, int use_wchar, size_t size)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_GetString * 4];
}

NUDE int AMXAPI amx_GetTag(AMX *amx, int index, char *tagname, cell *tag_id)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_GetTag * 4];
}

NUDE int AMXAPI amx_GetUserData(AMX *amx, long tag, void **ptr)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_GetUserData * 4];
}

NUDE int AMXAPI amx_Init(AMX *amx, void *program)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_Init * 4];
}

NUDE int AMXAPI amx_InitJIT(AMX *amx, void *reloc_table, void *native_code)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_InitJIT * 4];
}

NUDE int AMXAPI amx_MemInfo(AMX *amx, long *codesize, long *datasize, long *stackheap)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_MemInfo * 4];
}

NUDE int AMXAPI amx_NameLength(AMX *amx, int *length)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_NameLength * 4];
}

NUDE AMX_NATIVE_INFO * AMXAPI amx_NativeInfo(const char *name, AMX_NATIVE func)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_NativeInfo * 4];
}

NUDE int AMXAPI amx_NumNatives(AMX *amx, int *number)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_NumNatives * 4];
}

NUDE int AMXAPI amx_NumPublics(AMX *amx, int *number)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_NumPublics * 4];
}

NUDE int AMXAPI amx_NumPubVars(AMX *amx, int *number)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_NumPubVars * 4];
}

NUDE int AMXAPI amx_NumTags(AMX *amx, int *number)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_NumTags * 4];
}

NUDE int AMXAPI amx_Push(AMX *amx, cell value)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_Push * 4];
}

NUDE int AMXAPI amx_PushArray(AMX *amx, cell *amx_addr, cell **phys_addr, const cell array[], int numcells)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_PushArray * 4];
}

NUDE int AMXAPI amx_PushString(AMX *amx, cell *amx_addr, cell **phys_addr, const char *string, int pack, int use_wchar)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_PushString * 4];
}

NUDE int AMXAPI amx_RaiseError(AMX *amx, int error)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_RaiseError * 4];
}

NUDE int AMXAPI amx_Register(AMX *amx, const AMX_NATIVE_INFO *nativelist, int number)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_Register * 4];
}

NUDE int AMXAPI amx_Release(AMX *amx, cell amx_addr)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_Release * 4];
}

NUDE int AMXAPI amx_SetCallback(AMX *amx, AMX_CALLBACK callback)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_SetCallback * 4];
}

NUDE int AMXAPI amx_SetDebugHook(AMX *amx, AMX_DEBUG debug)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_SetDebugHook * 4];
}

NUDE int AMXAPI amx_SetString(cell *dest, const char *source, int pack, int use_wchar, size_t size)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_SetString * 4];
}

NUDE int AMXAPI amx_SetUserData(AMX *amx, long tag, void *ptr)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_SetUserData * 4];
}

NUDE int AMXAPI amx_StrLen(const cell *cstring, int *length)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_StrLen * 4];
}

NUDE int AMXAPI amx_UTF8Check(const char *string, int *length)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_UTF8Check * 4];
}

NUDE int AMXAPI amx_UTF8Get(const char *string, const char **endptr, cell *value)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_UTF8Get * 4];
}

NUDE int AMXAPI amx_UTF8Len(const cell *cstr, int *length)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_UTF8Len * 4];
}

NUDE int AMXAPI amx_UTF8Put(char *string, char **endptr, int maxchars, cell value)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr[eax + PLUGIN_AMX_EXPORT_UTF8Put * 4];
}

#else

// Unoptimized Thunks (Linux/BSD/non MSVC++)

typedef uint16_t *  AMXAPI(*amx_Align16_t)(uint16_t *v);
uint16_t * AMXAPI amx_Align16(uint16_t *v)
{
	amx_Align16_t fn = ((amx_Align16_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_Align16];
	return fn(v);
}

typedef uint32_t *  AMXAPI(*amx_Align32_t)(uint32_t *v);
uint32_t * AMXAPI amx_Align32(uint32_t *v)
{
	amx_Align32_t fn = ((amx_Align32_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_Align32];
	return fn(v);
}

#if defined _I64_MAX || defined HAVE_I64
typedef   uint64_t *  AMXAPI(*amx_Align64_t)(uint64_t *v);
uint64_t * AMXAPI amx_Align64(uint64_t *v)
{
	amx_Align64_t fn = ((amx_Align64_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_Align64];
	return fn(v);
}

#endif
typedef int  AMXAPI(*amx_Allot_t)(AMX *amx, int cells, cell *amx_addr, cell **phys_addr);
int AMXAPI amx_Allot(AMX *amx, int cells, cell *amx_addr, cell **phys_addr)
{
	amx_Allot_t fn = ((amx_Allot_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_Allot];
	return fn(amx, cells, amx_addr, phys_addr);
}

typedef int  AMXAPI(*amx_Callback_t)(AMX *amx, cell index, cell *result, cell *params);
int AMXAPI amx_Callback(AMX *amx, cell index, cell *result, cell *params)
{
	amx_Callback_t fn = ((amx_Callback_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_Callback];
	return fn(amx, index, result, params);
}

typedef int  AMXAPI(*amx_Cleanup_t)(AMX *amx);
int AMXAPI amx_Cleanup(AMX *amx)
{
	amx_Cleanup_t fn = ((amx_Cleanup_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_Cleanup];
	return fn(amx);
}

typedef int  AMXAPI(*amx_Clone_t)(AMX *amxClone, AMX *amxSource, void *data);
int AMXAPI amx_Clone(AMX *amxClone, AMX *amxSource, void *data)
{
	amx_Clone_t fn = ((amx_Clone_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_Clone];
	return fn(amxClone, amxSource, data);
}

typedef int  AMXAPI(*amx_Exec_t)(AMX *amx, cell *retval, int index);
int AMXAPI amx_Exec(AMX *amx, cell *retval, int index)
{
	amx_Exec_t fn = ((amx_Exec_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_Exec];
	return fn(amx, retval, index);
}

typedef int  AMXAPI(*amx_FindNative_t)(AMX *amx, const char *name, int *index);
int AMXAPI amx_FindNative(AMX *amx, const char *name, int *index)
{
	amx_FindNative_t fn = ((amx_FindNative_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_FindNative];
	return fn(amx, name, index);
}

typedef int  AMXAPI(*amx_FindPublic_t)(AMX *amx, const char *funcname, int *index);
int AMXAPI amx_FindPublic(AMX *amx, const char *funcname, int *index)
{
	amx_FindPublic_t fn = ((amx_FindPublic_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_FindPublic];
	return fn(amx, funcname, index);
}

typedef int  AMXAPI(*amx_FindPubVar_t)(AMX *amx, const char *varname, cell *amx_addr);
int AMXAPI amx_FindPubVar(AMX *amx, const char *varname, cell *amx_addr)
{
	amx_FindPubVar_t fn = ((amx_FindPubVar_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_FindPubVar];
	return fn(amx, varname, amx_addr);
}

typedef int  AMXAPI(*amx_FindTagId_t)(AMX *amx, cell tag_id, char *tagname);
int AMXAPI amx_FindTagId(AMX *amx, cell tag_id, char *tagname)
{
	amx_FindTagId_t fn = ((amx_FindTagId_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_FindTagId];
	return fn(amx, tag_id, tagname);
}

typedef int  AMXAPI(*amx_Flags_t)(AMX *amx, uint16_t *flags);
int AMXAPI amx_Flags(AMX *amx, uint16_t *flags)
{
	amx_Flags_t fn = ((amx_Flags_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_Flags];
	return fn(amx, flags);
}

typedef int  AMXAPI(*amx_GetAddr_t)(AMX *amx, cell amx_addr, cell **phys_addr);
int AMXAPI amx_GetAddr(AMX *amx, cell amx_addr, cell **phys_addr)
{
	amx_GetAddr_t fn = ((amx_GetAddr_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_GetAddr];
	return fn(amx, amx_addr, phys_addr);
}

typedef int  AMXAPI(*amx_GetNative_t)(AMX *amx, int index, char *funcname);
int AMXAPI amx_GetNative(AMX *amx, int index, char *funcname)
{
	amx_GetNative_t fn = ((amx_GetNative_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_GetNative];
	return fn(amx, index, funcname);
}

typedef int  AMXAPI(*amx_GetPublic_t)(AMX *amx, int index, char *funcname);
int AMXAPI amx_GetPublic(AMX *amx, int index, char *funcname)
{
	amx_GetPublic_t fn = ((amx_GetPublic_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_GetPublic];
	return fn(amx, index, funcname);
}

typedef int  AMXAPI(*amx_GetPubVar_t)(AMX *amx, int index, char *varname, cell *amx_addr);
int AMXAPI amx_GetPubVar(AMX *amx, int index, char *varname, cell *amx_addr)
{
	amx_GetPubVar_t fn = ((amx_GetPubVar_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_GetPubVar];
	return fn(amx, index, varname, amx_addr);
}

typedef int  AMXAPI(*amx_GetString_t)(char *dest, const cell *source, int use_wchar, size_t size);
int AMXAPI amx_GetString(char *dest, const cell *source, int use_wchar, size_t size)
{
	amx_GetString_t fn = ((amx_GetString_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_GetString];
	return fn(dest, source, use_wchar, size);
}

typedef int  AMXAPI(*amx_GetTag_t)(AMX *amx, int index, char *tagname, cell *tag_id);
int AMXAPI amx_GetTag(AMX *amx, int index, char *tagname, cell *tag_id)
{
	amx_GetTag_t fn = ((amx_GetTag_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_GetTag];
	return fn(amx, index, tagname, tag_id);
}

typedef int  AMXAPI(*amx_GetUserData_t)(AMX *amx, long tag, void **ptr);
int AMXAPI amx_GetUserData(AMX *amx, long tag, void **ptr)
{
	amx_GetUserData_t fn = ((amx_GetUserData_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_GetUserData];
	return fn(amx, tag, ptr);
}

typedef int  AMXAPI(*amx_Init_t)(AMX *amx, void *program);
int AMXAPI amx_Init(AMX *amx, void *program)
{
	amx_Init_t fn = ((amx_Init_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_Init];
	return fn(amx, program);
}

typedef int  AMXAPI(*amx_InitJIT_t)(AMX *amx, void *reloc_table, void *native_code);
int AMXAPI amx_InitJIT(AMX *amx, void *reloc_table, void *native_code)
{
	amx_InitJIT_t fn = ((amx_InitJIT_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_InitJIT];
	return fn(amx, reloc_table, native_code);
}

typedef int  AMXAPI(*amx_MemInfo_t)(AMX *amx, long *codesize, long *datasize, long *stackheap);
int AMXAPI amx_MemInfo(AMX *amx, long *codesize, long *datasize, long *stackheap)
{
	amx_MemInfo_t fn = ((amx_MemInfo_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_MemInfo];
	return fn(amx, codesize, datasize, stackheap);
}

typedef int  AMXAPI(*amx_NameLength_t)(AMX *amx, int *length);
int AMXAPI amx_NameLength(AMX *amx, int *length)
{
	amx_NameLength_t fn = ((amx_NameLength_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_NameLength];
	return fn(amx, length);
}

typedef AMX_NATIVE_INFO *  AMXAPI(*amx_NativeInfo_t)(const char *name, AMX_NATIVE func);
AMX_NATIVE_INFO * AMXAPI amx_NativeInfo(const char *name, AMX_NATIVE func)
{
	amx_NativeInfo_t fn = ((amx_NativeInfo_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_NativeInfo];
	return fn(name, func);
}

typedef int  AMXAPI(*amx_NumNatives_t)(AMX *amx, int *number);
int AMXAPI amx_NumNatives(AMX *amx, int *number)
{
	amx_NumNatives_t fn = ((amx_NumNatives_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_NumNatives];
	return fn(amx, number);
}

typedef int  AMXAPI(*amx_NumPublics_t)(AMX *amx, int *number);
int AMXAPI amx_NumPublics(AMX *amx, int *number)
{
	amx_NumPublics_t fn = ((amx_NumPublics_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_NumPublics];
	return fn(amx, number);
}

typedef int  AMXAPI(*amx_NumPubVars_t)(AMX *amx, int *number);
int AMXAPI amx_NumPubVars(AMX *amx, int *number)
{
	amx_NumPubVars_t fn = ((amx_NumPubVars_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_NumPubVars];
	return fn(amx, number);
}

typedef int  AMXAPI(*amx_NumTags_t)(AMX *amx, int *number);
int AMXAPI amx_NumTags(AMX *amx, int *number)
{
	amx_NumTags_t fn = ((amx_NumTags_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_NumTags];
	return fn(amx, number);
}

typedef int  AMXAPI(*amx_Push_t)(AMX *amx, cell value);
int AMXAPI amx_Push(AMX *amx, cell value)
{
	amx_Push_t fn = ((amx_Push_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_Push];
	return fn(amx, value);
}

typedef int  AMXAPI(*amx_PushArray_t)(AMX *amx, cell *amx_addr, cell **phys_addr, const cell array[], int numcells);
int AMXAPI amx_PushArray(AMX *amx, cell *amx_addr, cell **phys_addr, const cell array[], int numcells)
{
	amx_PushArray_t fn = ((amx_PushArray_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_PushArray];
	return fn(amx, amx_addr, phys_addr, array, numcells);
}

typedef int  AMXAPI(*amx_PushString_t)(AMX *amx, cell *amx_addr, cell **phys_addr, const char *string, int pack, int use_wchar);
int AMXAPI amx_PushString(AMX *amx, cell *amx_addr, cell **phys_addr, const char *string, int pack, int use_wchar)
{
	amx_PushString_t fn = ((amx_PushString_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_PushString];
	return fn(amx, amx_addr, phys_addr, string, pack, use_wchar);
}

typedef int  AMXAPI(*amx_RaiseError_t)(AMX *amx, int error);
int AMXAPI amx_RaiseError(AMX *amx, int error)
{
	amx_RaiseError_t fn = ((amx_RaiseError_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_RaiseError];
	return fn(amx, error);
}

typedef int  AMXAPI(*amx_Register_t)(AMX *amx, const AMX_NATIVE_INFO *nativelist, int number);
int AMXAPI amx_Register(AMX *amx, const AMX_NATIVE_INFO *nativelist, int number)
{
	amx_Register_t fn = ((amx_Register_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_Register];
	return fn(amx, nativelist, number);
}

typedef int  AMXAPI(*amx_Release_t)(AMX *amx, cell amx_addr);
int AMXAPI amx_Release(AMX *amx, cell amx_addr)
{
	amx_Release_t fn = ((amx_Release_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_Release];
	return fn(amx, amx_addr);
}

typedef int  AMXAPI(*amx_SetCallback_t)(AMX *amx, AMX_CALLBACK callback);
int AMXAPI amx_SetCallback(AMX *amx, AMX_CALLBACK callback)
{
	amx_SetCallback_t fn = ((amx_SetCallback_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_SetCallback];
	return fn(amx, callback);
}

typedef int  AMXAPI(*amx_SetDebugHook_t)(AMX *amx, AMX_DEBUG debug);
int AMXAPI amx_SetDebugHook(AMX *amx, AMX_DEBUG debug)
{
	amx_SetDebugHook_t fn = ((amx_SetDebugHook_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_SetDebugHook];
	return fn(amx, debug);
}

typedef int  AMXAPI(*amx_SetString_t)(cell *dest, const char *source, int pack, int use_wchar, size_t size);
int AMXAPI amx_SetString(cell *dest, const char *source, int pack, int use_wchar, size_t size)
{
	amx_SetString_t fn = ((amx_SetString_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_SetString];
	return fn(dest, source, pack, use_wchar, size);
}

typedef int  AMXAPI(*amx_SetUserData_t)(AMX *amx, long tag, void *ptr);
int AMXAPI amx_SetUserData(AMX *amx, long tag, void *ptr)
{
	amx_SetUserData_t fn = ((amx_SetUserData_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_SetUserData];
	return fn(amx, tag, ptr);
}

typedef int  AMXAPI(*amx_StrLen_t)(const cell *cstring, int *length);
int AMXAPI amx_StrLen(const cell *cstring, int *length)
{
	amx_StrLen_t fn = ((amx_StrLen_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_StrLen];
	return fn(cstring, length);
}

typedef int  AMXAPI(*amx_UTF8Check_t)(const char *string, int *length);
int AMXAPI amx_UTF8Check(const char *string, int *length)
{
	amx_UTF8Check_t fn = ((amx_UTF8Check_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_UTF8Check];
	return fn(string, length);
}

typedef int  AMXAPI(*amx_UTF8Get_t)(const char *string, const char **endptr, cell *value);
int AMXAPI amx_UTF8Get(const char *string, const char **endptr, cell *value)
{
	amx_UTF8Get_t fn = ((amx_UTF8Get_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_UTF8Get];
	return fn(string, endptr, value);
}

typedef int  AMXAPI(*amx_UTF8Len_t)(const cell *cstr, int *length);
int AMXAPI amx_UTF8Len(const cell *cstr, int *length)
{
	amx_UTF8Len_t fn = ((amx_UTF8Len_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_UTF8Len];
	return fn(cstr, length);
}

typedef int  AMXAPI(*amx_UTF8Put_t)(char *string, char **endptr, int maxchars, cell value);
int AMXAPI amx_UTF8Put(char *string, char **endptr, int maxchars, cell value)
{
	amx_UTF8Put_t fn = ((amx_UTF8Put_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_UTF8Put];
	return fn(string, endptr, maxchars, value);
}

#endif

//----------------------------------------------------------
// EOF

#include <sstream>
#include <vector>
#include <algorithm>
#include <map>
#include <time.h>

void TeleportPlayer(int playerid, float x, float y, float z, float a, bool vehicle, int interiorid, int virtualworldid, bool reset_weapons);

namespace Extension
{
	class Base;
	typedef std::pair<long long, Base*> extension_filler;
	extern std::vector<extension_filler> * Extensions;

	bool Prioritizer(extension_filler &i, extension_filler &j);

	class Base
	{

	public:
		//max priority == 0xFFFF (most important), minimal == 0x0000 (least important)
		Base(long long priority = 0)
		{
			if (Extensions == NULL)
				Extensions = new std::vector<extension_filler>;
			Extensions->push_back(extension_filler(priority, this));
			std::sort(Extensions->begin(), Extensions->end(), Prioritizer);
		}

		//SA-MP
		virtual bool OnGameModeInit() { return true; }
		virtual bool OnGameModeExit() { return true; }
		virtual bool OnPlayerConnect(int playerid) { return true; }
		virtual bool OnPlayerDisconnect(int playerid, int reason) { return true; }
		virtual bool OnPlayerSpawn(int playerid) { return true; }
		virtual bool OnPlayerDeath(int playerid, int killerid, int reason) { return true; }
		virtual bool OnVehicleSpawn(int vehicleid) { return true; }
		virtual bool OnVehicleDeath(int vehicleid, int killerid) { return true; }
		virtual bool OnPlayerText(int playerid, std::string text) { return true; }
		virtual bool OnPlayerCommandText(int playerid, std::string cmdtext) { return true; }
		virtual bool OnPlayerRequestClass(int playerid, int classid) { return true; }
		virtual bool OnPlayerEnterVehicle(int playerid, int vehicleid, bool ispassenger) { return true; }
		virtual bool OnPlayerExitVehicle(int playerid, int vehicleid) { return true; }
		virtual bool OnPlayerStateChange(int playerid, int newstate, int oldstate) { return true; }
		virtual bool OnPlayerEnterCheckpoint(int playerid) { return true; }
		virtual bool OnPlayerLeaveCheckpoint(int playerid) { return true; }
		virtual bool OnPlayerEnterRaceCheckpoint(int playerid) { return true; }
		virtual bool OnPlayerLeaveRaceCheckpoint(int playerid) { return true; }
		virtual bool OnRconCommand(std::string cmd) { return false; }
		virtual bool OnPlayerRequestSpawn(int playerid) { return true; }
		virtual bool OnObjectMoved(int objectid) { return true; }
		virtual bool OnPlayerObjectMoved(int playerid, int objectid) { return true; }
		virtual bool OnPlayerPickUpPickup(int playerid, int pickupid) { return true; }
		virtual bool OnVehicleMod(int playerid, int vehicleid, int componentid) { return true; }
		virtual bool OnEnterExitModShop(int playerid, int enterexit, int interiorid) { return true; }
		virtual bool OnVehiclePaintjob(int playerid, int vehicleid, int paintjobid) { return true; }
		virtual bool OnVehicleRespray(int playerid, int vehicleid, int color1, int color2) { return true; }
		virtual bool OnVehicleDamageStatusUpdate(int vehicleid, int playerid) { return true; }
		virtual bool OnUnoccupiedVehicleUpdate(int vehicleid, int playerid, int passenger_seat, float new_x, float new_y, float new_z, float vel_x, float vel_y, float vel_z) { return true; }
		virtual bool OnPlayerSelectedMenuRow(int playerid, int row) { return true; }
		virtual bool OnPlayerExitedMenu(int playerid) { return true; }
		virtual bool OnPlayerInteriorChange(int playerid, int newinteriorid, int oldinteriorid) { return true; }
		virtual bool OnPlayerKeyStateChange(int playerid, int newkeys, int oldkeys) { return true; }
		virtual bool OnRconLoginAttempt(std::string ip, std::string password, bool success) { return true; }
		virtual bool OnPlayerUpdate(int playerid) { return true; }
		virtual bool OnPlayerStreamIn(int playerid, int forplayerid) { return true; }
		virtual bool OnPlayerStreamOut(int playerid, int forplayerid) { return true; }
		virtual bool OnVehicleStreamIn(int vehicleid, int forplayerid) { return true; }
		virtual bool OnVehicleStreamOut(int vehicleid, int forplayerid) { return true; }
		virtual bool OnDialogResponse(int playerid, int dialogid, int response, int listitem, std::string inputtext) { return true; }
		virtual bool OnPlayerTakeDamage(int playerid, int issuerid, float amount, int weaponid, int bodypart) { return true; }
		virtual bool OnPlayerGiveDamage(int playerid, int damagedid, float amount, int weaponid, int bodypart) { return true; }
		virtual bool OnPlayerClickMap(int playerid, float fX, float fY, float fZ) { return true; }
		virtual bool OnPlayerClickTextDraw(int playerid, int clickedid) { return true; }
		virtual bool OnPlayerClickPlayerTextDraw(int playerid, int playertextid) { return true; }
		virtual bool OnIncomingConnection(int playerid, std::string ip_address, int port) { return true; }
		virtual bool OnTrailerUpdate(int playerid, int vehicleid) { return true; }
		virtual bool OnPlayerClickPlayer(int playerid, int clickedplayerid, int source) { return true; }
		virtual bool OnPlayerEditObject(int playerid, bool playerobject, int objectid, int response, float fX, float fY, float fZ, float fRotX, float fRotY, float fRotZ) { return true; }
		virtual bool OnPlayerEditAttachedObject(int playerid, int response, int index, int modelid, int boneid, float fOffsetX, float fOffsetY, float fOffsetZ, float fRotX, float fRotY, float fRotZ, float fScaleX, float fScaleY, float fScaleZ) { return true; }
		virtual bool OnPlayerSelectObject(int playerid, int type, int objectid, int modelid, float fX, float fY, float fZ) { return true; }
		virtual bool OnPlayerWeaponShot(int playerid, int weaponid, int hittype, int hitid, float fX, float fY, float fZ) { return true; }

		//Plugin
		virtual void Load(){}
		virtual void Unload(){}
		virtual void ProcessTick(){}
		//Dtor
		virtual ~Base() {}
	};
};


namespace ZeroDLG
{
	class Dialog;
	static bool init = false;
	struct ZeroDialogInternal
	{
		Dialog* CurrentDialog[MAX_PLAYERS];
		std::map<std::string, Dialog*> dialog_map;
		ZeroDialogInternal();
		void register_dialog(Dialog* dialog, std::string name);
	};
	extern ZeroDialogInternal * __internal_dialog_registrar;
	class Dialog
	{
	protected:
		int
			dialog_style;
		std::string
			caption,
			info,
			button1,
			button2;
	public:
		Dialog();
		~Dialog();


		virtual void process_dialog(int playerid, int response, int listitem, std::string &inputtext) = 0;
		void Show(int playerid);
		void Show(int playerid, std::string &customtext);
		void ShowCustom(int playerid, int cdialog_style, std::string &ccaption, std::string &cinfo, std::string &cbutton1, std::string &cbutton2);
		void PrepareDialog(int dialog_style, std::string &caption, std::string &info, std::string &button1, std::string &button2);
	};
	bool OnDialogResponse(int playerid, int dialogid, int response, int listitem, std::string inputtext);
	bool OnPlayerConnect(int playerid);
	bool OnPlayerDisconnect(int playerid, int reason);
};

#define ZERO_DIALOG(name) \
class dialog ## name : public ZeroDLG::Dialog\
	{\
		public:\
		dialog ## name() { if (!ZeroDLG::__internal_dialog_registrar){ZeroDLG::__internal_dialog_registrar = new ZeroDLG::ZeroDialogInternal;} ZeroDLG::__internal_dialog_registrar->register_dialog(this, #name); }\
		void process_dialog(int playerid, int response, int listitem, std::string& inputtext);\
	};\
	dialog ## name dialog ## name ## __execute;\
	inline void dialog ## name::process_dialog(int playerid, int response, int listitem, std::string& inputtext)

bool PrepareZeroDialog(std::string name, int dialog_style, std::string caption, std::string info, std::string button1, std::string button2);
bool ShowPlayerZeroDialog(int playerid, std::string name);
bool ShowPlayerZeroDialog(int playerid, std::string name, std::string text);
bool ShowPlayerCustomDialog(int playerid, std::string name, int dialog_style, std::string caption, std::string info, std::string button1, std::string button2);
bool HidePlayerZeroDialog(int playerid);
const static std::string DLG_DUMMY = "________dummy";

std::string _GetPlayerName(int playerid);
std::string _GetPlayerIP(int playerid);

namespace Extension
{
	std::vector<extension_filler> * Extensions;
	bool Prioritizer(extension_filler &i, extension_filler &j)
	{
		return (i.first>j.first);
	}
};

class DummyExtensionNotUsed : public Extension::Base
{
public:
	DummyExtensionNotUsed()
	{
	}
};
DummyExtensionNotUsed _DummyExtensionNotUsed;


/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnGameModeInit">OnGameModeInit on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnGameModeInit())
{
	static bool init = false;
	if (init)
		return true;
	init = true;
	for (auto &i : *Extension::Extensions)
		i.second->OnGameModeInit();
	return true;
}


/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerConnect">OnPlayerConnect on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerConnect(int playerid))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerConnect(playerid);
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerDisconnect">OnPlayerDisconnect on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerDisconnect(int playerid, int reason))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerDisconnect(playerid, reason);
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerRequestClass">OnPlayerRequestClass on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerRequestClass(int playerid, int classid))
{
	for (auto &i : *Extension::Extensions)
		if (!i.second->OnPlayerRequestClass(playerid, classid))
			return false;
	return true;
}

SAMPGDK_CALLBACK(bool, OnPlayerCommandText(int playerid, const char * cmdtext))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerCommandText(playerid, std::string(cmdtext));//for logging etc
	return true;
}


/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnRconCommand">OnRconCommand on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnRconCommand(const char * cmd))
{
	for (auto &i : *Extension::Extensions)
		if (i.second->OnRconCommand(std::string(cmd)))
			return true;
	return false;
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return sampgdk::Supports() | SUPPORTS_PROCESS_TICK;
}

extern void *pAMXFunctions;

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	for (auto &i : *Extension::Extensions)
		i.second->Unload();
	sampgdk::Unload();
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	bool load = sampgdk::Load(ppData);

	for (auto &i : *Extension::Extensions)
		i.second->Load();
	return load;
}


PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
	sampgdk::ProcessTick();
	for (auto &i : *Extension::Extensions)
		i.second->ProcessTick();
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnGameModeExit">OnGameModeExit on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnGameModeExit())
{
	for (auto &i : *Extension::Extensions)
		i.second->OnGameModeExit();
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerSpawn">OnPlayerSpawn on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerSpawn(int playerid))
{
	for (auto &i : *Extension::Extensions)
		if (!i.second->OnPlayerSpawn(playerid))
			return false;
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerDeath">OnPlayerDeath on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerDeath(int playerid, int killerid, int reason))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerDeath(playerid, killerid, reason);
	return false;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnVehicleSpawn">OnVehicleSpawn on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnVehicleSpawn(int vehicleid))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnVehicleSpawn(vehicleid);
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnVehicleDeath">OnVehicleDeath on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnVehicleDeath(int vehicleid, int killerid))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnVehicleDeath(vehicleid, killerid);
	return false;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerText">OnPlayerText on SA-MP Wiki</a>
*/

SAMPGDK_CALLBACK(bool, OnPlayerText(int playerid, const char * text))
{
	for (auto &i : *Extension::Extensions)
		if (!i.second->OnPlayerText(playerid, std::string(text)))
			return false;
	return false;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerEnterVehicle">OnPlayerEnterVehicle on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerEnterVehicle(int playerid, int vehicleid, bool ispassenger))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerExitVehicle">OnPlayerExitVehicle on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerExitVehicle(int playerid, int vehicleid))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerExitVehicle(playerid, vehicleid);
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerStateChange">OnPlayerStateChange on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerStateChange(int playerid, int newstate, int oldstate))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerStateChange(playerid, newstate, oldstate);
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerEnterCheckpoint">OnPlayerEnterCheckpoint on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerEnterCheckpoint(int playerid))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerEnterCheckpoint(playerid);
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerLeaveCheckpoint">OnPlayerLeaveCheckpoint on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerLeaveCheckpoint(int playerid))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerLeaveCheckpoint(playerid);
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerEnterRaceCheckpoint">OnPlayerEnterRaceCheckpoint on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerEnterRaceCheckpoint(int playerid))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerEnterRaceCheckpoint(playerid);
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerLeaveRaceCheckpoint">OnPlayerLeaveRaceCheckpoint on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerLeaveRaceCheckpoint(int playerid))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerLeaveRaceCheckpoint(playerid);
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerRequestSpawn">OnPlayerRequestSpawn on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerRequestSpawn(int playerid))
{
	for (auto &i : *Extension::Extensions)
		if (!i.second->OnPlayerRequestSpawn(playerid))
			return false;
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnObjectMoved">OnObjectMoved on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnObjectMoved(int objectid))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnObjectMoved(objectid);
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerObjectMoved">OnPlayerObjectMoved on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerObjectMoved(int playerid, int objectid))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerObjectMoved(playerid, objectid);
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerPickUpPickup">OnPlayerPickUpPickup on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerPickUpPickup(int playerid, int pickupid))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerPickUpPickup(playerid, pickupid);
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnVehicleMod">OnVehicleMod on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnVehicleMod(int playerid, int vehicleid, int componentid))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnVehicleMod(playerid, vehicleid, componentid);
	return false;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnEnterExitModShop">OnEnterExitModShop on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnEnterExitModShop(int playerid, int enterexit, int interiorid))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnEnterExitModShop(playerid, enterexit, interiorid);
	return false;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnVehiclePaintjob">OnVehiclePaintjob on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnVehiclePaintjob(int playerid, int vehicleid, int paintjobid))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnVehiclePaintjob(playerid, vehicleid, paintjobid);
	return false;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnVehicleRespray">OnVehicleRespray on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnVehicleRespray(int playerid, int vehicleid, int color1, int color2))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnVehicleRespray(playerid, vehicleid, color1, color2);
	return false;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnVehicleDamageStatusUpdate">OnVehicleDamageStatusUpdate on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnVehicleDamageStatusUpdate(int vehicleid, int playerid))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnVehicleDamageStatusUpdate(vehicleid, playerid);
	return false;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnUnoccupiedVehicleUpdate">OnUnoccupiedVehicleUpdate on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnUnoccupiedVehicleUpdate(int vehicleid, int playerid, int passenger_seat, float new_x, float new_y, float new_z, float vel_x, float vel_y, float vel_z))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat, new_x, new_y, new_z, vel_x, vel_y, vel_z);
	return false;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerSelectedMenuRow">OnPlayerSelectedMenuRow on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerSelectedMenuRow(int playerid, int row))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerSelectedMenuRow(playerid, row);
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerExitedMenu">OnPlayerExitedMenu on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerExitedMenu(int playerid))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerExitedMenu(playerid);
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerInteriorChange">OnPlayerInteriorChange on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerInteriorChange(int playerid, int newinteriorid, int oldinteriorid))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerKeyStateChange">OnPlayerKeyStateChange on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerKeyStateChange(int playerid, int newkeys, int oldkeys))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	return false;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnRconLoginAttempt">OnRconLoginAttempt on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnRconLoginAttempt(const char * ip, const char * password, bool success))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnRconLoginAttempt(std::string(ip), std::string(password), success);
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerUpdate">OnPlayerUpdate on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerUpdate(int playerid))
{
	for (auto &i : *Extension::Extensions)
		if (!i.second->OnPlayerUpdate(playerid))
			return false;
	return false;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerStreamIn">OnPlayerStreamIn on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerStreamIn(int playerid, int forplayerid))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerStreamIn(playerid, forplayerid);
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerStreamOut">OnPlayerStreamOut on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerStreamOut(int playerid, int forplayerid))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerStreamOut(playerid, forplayerid);
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnVehicleStreamIn">OnVehicleStreamIn on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnVehicleStreamIn(int vehicleid, int forplayerid))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnVehicleStreamIn(vehicleid, forplayerid);
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnVehicleStreamOut">OnVehicleStreamOut on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnVehicleStreamOut(int vehicleid, int forplayerid))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnVehicleStreamOut(vehicleid, forplayerid);
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnDialogResponse">OnDialogResponse on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnDialogResponse(int playerid, int dialogid, int response, int listitem, const char * inputtext))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnDialogResponse(playerid, dialogid, response, listitem, std::string(inputtext));
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerTakeDamage">OnPlayerTakeDamage on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerTakeDamage(int playerid, int issuerid, float amount, int weaponid, int bodypart))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerTakeDamage(playerid, issuerid, amount, weaponid, bodypart);
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerGiveDamage">OnPlayerGiveDamage on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerGiveDamage(int playerid, int damagedid, float amount, int weaponid, int bodypart))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerGiveDamage(playerid, damagedid, amount, weaponid, bodypart);
	return false;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerClickMap">OnPlayerClickMap on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerClickMap(int playerid, float fX, float fY, float fZ))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerClickMap(playerid, fX, fY, fZ);
	return false;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerClickTextDraw">OnPlayerClickTextDraw on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerClickTextDraw(int playerid, int clickedid))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerClickTextDraw(playerid, clickedid);
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerClickPlayerTextDraw">OnPlayerClickPlayerTextDraw on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerClickPlayerTextDraw(int playerid, int playertextid))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerClickPlayerTextDraw(playerid, playertextid);
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnIncomingConnection">OnIncomingConnection on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnIncomingConnection(int playerid, const char * ip_address, int port))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnIncomingConnection(playerid, std::string(ip_address), port);
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnTrailerUpdate">OnTrailerUpdate on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnTrailerUpdate(int playerid, int vehicleid))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnTrailerUpdate(playerid, vehicleid);
	return false;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerClickPlayer">OnPlayerClickPlayer on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerClickPlayer(int playerid, int clickedplayerid, int source))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerClickPlayer(playerid, clickedplayerid, source);
	return true;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerEditObject">OnPlayerEditObject on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerEditObject(int playerid, bool playerobject, int objectid, int response, float fX, float fY, float fZ, float fRotX, float fRotY, float fRotZ))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerEditObject(playerid, playerobject, objectid, response, fX, fY, fZ, fRotX, fRotY, fRotZ);
	return false;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerEditAttachedObject">OnPlayerEditAttachedObject on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerEditAttachedObject(int playerid, int response, int index, int modelid, int boneid, float fOffsetX, float fOffsetY, float fOffsetZ, float fRotX, float fRotY, float fRotZ, float fScaleX, float fScaleY, float fScaleZ))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerEditAttachedObject(playerid, response, index, modelid, boneid, fOffsetX, fOffsetY, fOffsetZ, fRotX, fRotY, fRotZ, fScaleX, fScaleY, fScaleZ);
	return false;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerSelectObject">OnPlayerSelectObject on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerSelectObject(int playerid, int type, int objectid, int modelid, float fX, float fY, float fZ))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerSelectObject(playerid, type, objectid, modelid, fX, fY, fZ);
	return false;
}

/**
* \ingroup callbacks
* \see <a href="http://wiki.sa-mp.com/wiki/OnPlayerWeaponShot">OnPlayerWeaponShot on SA-MP Wiki</a>
*/
SAMPGDK_CALLBACK(bool, OnPlayerWeaponShot(int playerid, int weaponid, int hittype, int hitid, float fX, float fY, float fZ))
{
	for (auto &i : *Extension::Extensions)
		i.second->OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
	return false;
}

namespace ZeroDLG
{
	ZeroDialogInternal * __internal_dialog_registrar;
}

class DialogProcessor : public Extension::Base
{
public:
	bool OnDialogResponse(int playerid, int dialogid, int response, int listitem, std::string inputtext)
	{
		if (ZeroDLG::__internal_dialog_registrar->CurrentDialog[playerid] != nullptr)
		{
			ZeroDLG::Dialog* copy = ZeroDLG::__internal_dialog_registrar->CurrentDialog[playerid];
			ZeroDLG::__internal_dialog_registrar->CurrentDialog[playerid] = nullptr;
			copy->process_dialog(playerid, response, listitem, inputtext);
		}
		return false;
	}
	bool OnPlayerConnect(int playerid)
	{
		ZeroDLG::__internal_dialog_registrar->CurrentDialog[playerid] = nullptr;
		return true;
	}
	bool OnPlayerDisconnect(int playerid, int reason)
	{
		ZeroDLG::__internal_dialog_registrar->CurrentDialog[playerid] = nullptr;
		return true;
	}
} _DialogProcessor;

bool PrepareZeroDialog(std::string name, int dialog_style, std::string caption, std::string info, std::string button1, std::string button2)
{
	auto dialog = ZeroDLG::__internal_dialog_registrar->dialog_map.find(name);
	if (dialog == ZeroDLG::__internal_dialog_registrar->dialog_map.end())
		return false;
	dialog->second->PrepareDialog(dialog_style, caption, info, button1, button2);
	return true;
}

bool ShowPlayerZeroDialog(int playerid, std::string name)
{
	auto dialog = ZeroDLG::__internal_dialog_registrar->dialog_map.find(name);
	if (dialog == ZeroDLG::__internal_dialog_registrar->dialog_map.end())
		return false;
	dialog->second->Show(playerid);
	return true;
}

bool ShowPlayerZeroDialog(int playerid, std::string name, std::string text)
{
	auto dialog = ZeroDLG::__internal_dialog_registrar->dialog_map.find(name);
	if (dialog == ZeroDLG::__internal_dialog_registrar->dialog_map.end())
		return false;
	dialog->second->Show(playerid, text);
	return true;
}

bool ShowPlayerCustomDialog(int playerid, std::string name, int dialog_style, std::string caption, std::string info, std::string button1, std::string button2)
{
	auto dialog = ZeroDLG::__internal_dialog_registrar->dialog_map.find(name);
	if (dialog == ZeroDLG::__internal_dialog_registrar->dialog_map.end())
		return false;
	dialog->second->ShowCustom(playerid, dialog_style, caption, info, button1, button2);
	return true;
}

bool HidePlayerZeroDialog(int playerid)
{
	ShowPlayerDialog(playerid, -1, -1, "", "", "", "");
	return true;
}

ZERO_DIALOG(________dummy){}

///////////////DIALOGPROCESSOR

namespace ZeroDLG
{
	ZeroDialogInternal::ZeroDialogInternal() {}
	void  ZeroDialogInternal::register_dialog(Dialog* dialog, std::string name)
	{
		if (!init)
		{
			init = true;
			for (size_t i = 0; i < MAX_PLAYERS; ++i)
			{
				CurrentDialog[i] = nullptr;
			}
		}
		dialog_map[name] = dialog;
	}

	Dialog::Dialog()
	{
		if (!__internal_dialog_registrar)
		{
			__internal_dialog_registrar = new ZeroDialogInternal;
		}
	}
	Dialog::~Dialog(){}
	void Dialog::Show(int playerid)
	{
		__internal_dialog_registrar->CurrentDialog[playerid] = this;
		ShowPlayerDialog(playerid, 1, dialog_style, caption.c_str(), info.c_str(), button1.c_str(), button2.c_str());
	}
	void Dialog::Show(int playerid, std::string &customtext)
	{
		__internal_dialog_registrar->CurrentDialog[playerid] = this;
		ShowPlayerDialog(playerid, 1, dialog_style, caption.c_str(), (info + customtext).c_str(), button1.c_str(), button2.c_str());
	}
	void Dialog::ShowCustom(int playerid, int cdialog_style, std::string &ccaption, std::string &cinfo, std::string &cbutton1, std::string &cbutton2)
	{
		__internal_dialog_registrar->CurrentDialog[playerid] = this;
		ShowPlayerDialog(playerid, 1, cdialog_style, ccaption.c_str(), cinfo.c_str(), cbutton1.c_str(), cbutton2.c_str());
	}
	void Dialog::PrepareDialog(int dialog_style, std::string &caption, std::string &info, std::string &button1, std::string &button2)
	{
		this->dialog_style = dialog_style;
		this->caption.assign(caption);
		this->info.assign(info);
		this->button1.assign(button1);
		this->button2.assign(button2);
	}
};

char tempname[32];
std::string _GetPlayerName(int playerid)
{
	GetPlayerName(playerid, tempname, 32);
	return tempname;
}
std::string _GetPlayerIP(int playerid)
{
	GetPlayerIp(playerid, tempname, 32);
	return tempname;
}

void TeleportPlayer(int playerid, float x, float y, float z, float a, bool vehicle, int interiorid, int virtualworldid, bool reset_weapons)
{
	if (reset_weapons)
		ResetPlayerWeapons(playerid);

	SetPlayerInterior(playerid, interiorid);
	SetPlayerVirtualWorld(playerid, virtualworldid);

	if (vehicle && GetPlayerVehicleID(playerid))
	{
		SetVehiclePos(GetPlayerVehicleID(playerid), x, y, z);
		SetVehicleZAngle(GetPlayerVehicleID(playerid), a);
		LinkVehicleToInterior(GetPlayerVehicleID(playerid), interiorid);
		SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), virtualworldid);
	}
	else
	{
		SetPlayerPos(playerid, x , y , z);
		SetPlayerFacingAngle(playerid, a);
	}
}

std::map<int, std::string> PlayerIP;

typedef std::initializer_list<std::string> vecstri;
struct Captcha
{
	struct POS
	{
		float x;
		float y;
		float z;
		POS(){}
		POS(float a, float b, float c)
			:x(a), y(b), z(c)
		{}
	};
	POS from;
	POS to;
	std::string question;
	std::vector<std::string> PossibleAnswers;
	bool MustContainAllPossibleAnswers;
	bool OrderOfAnswerIsImportant;
	Captcha(){}
	Captcha(float fx, float fy, float fz, float fa, float tx, float ty, float tz, float ta, std::string _q, std::vector<std::string> _a, bool mcaps, bool ooaii)
		: from(fx, fy, fz), to(tx, ty, tz), question(_q), PossibleAnswers(_a), MustContainAllPossibleAnswers(mcaps), OrderOfAnswerIsImportant(ooaii)
	{}
};

static const std::vector<Captcha> Captchas =
{
	Captcha(1560.0347, -1728.9708, 30.4837, 179.8081, 1559.1919, -1723.5101, 31.0120, 179.8081, "Co tutaj piszę?", vecstri({ "rampage" }), true, true ),///RampageTo
	Captcha(1691.4023,-1158.3511,25.9160,179.8081,1691.3745,-1166.6801,27.3262,179.8081,"Jaką liczbę lub numer tutaj widać?",vecstri({"659"}), true, true), ///JakaLiczba659To
	Captcha(2472.1431,-1494.4578,37.4980,97.7141,2461.4321,-1495.2190,37.4980,88.6274,"Co tutaj piszę?",vecstri({"mama's", "mamas", "mama s"}), true, true), ///MamasTo
	Captcha(2345.2649,-1547.3610,33.9385,312.3261,2353.4065,-1541.1558,36.8038,307.3126,"Jaką liczbę lub numer tutaj widać?",vecstri({"555-5300", "5555300" }), true, true), ///JakiNr555-5300To
	Captcha(1724.1974,-1330.6105,35.2078,354.3132 ,1725.1992,-1320.5502,35.2078,354.3132,"Przepisz duży napis",vecstri({"trainhard", "train hard"}), true, true), ///JakiDuzyNapisTrainHardTo prolaps
	Captcha(1410.1907,-1148.7324,28.5131,75.1540,1405.6650,-1147.0697,30.3154,69.8272,"Co jest w prawo?",vecstri({"mulholand"}), true, true), ///CoWPrawoMulholandTo
	Captcha(1324.6530,-1139.4542,30.3154,86.7474,1317.2120,-1139.0311,30.3154,86.7474,"Co jest prosto/do góry?",vecstri({"vinewood"}), true, true), ///Coprostovinewoodto
	Captcha(873.9315,-1138.4919,30.3154,90.8208,861.8248,-1138.7317,30.3154,91.1341,"Co jest w lewo?",vecstri({"rodeo"}), true, true), ///Cowleworodeoto
	Captcha(760.2118, -1210.4923, 30.3154, 167.2748, 753.7706, -1242.2318, 17.1930, 348.5282, "Jaki sport się tutaj uprawia?", vecstri({ "tennis", "tenis" }), false, false), ///CoZaSportTenisTo
	Captcha(793.1856,-1416.1311,19.7912,193.2817 ,794.3928,-1421.2449,19.7912,193.2817,"Co jest w lewo?",vecstri({"market"}), true, true), ///CowlewoMarketTo
	Captcha(1047.4915,-1522.4120,23.2891,219.2885,1050.0464,-1525.2700,24.8192,221.7952,"Jaką liczbę lub numer tutaj widać?",vecstri({"555-8684", "5558684"}), true, true), ///JakiNumer555-8684To
	Captcha(1061.4226,-1898.3774,19.8048,3.7365,1061.2825,-1893.1787,19.8048,1.5432,"Co jest prosto/do góry?",vecstri({"market"}), true, true), ///coprostoMarketTo
	Captcha(869.6863,-1769.7039,19.8048,95.8574,860.8276,-1770.6477,19.8048,86.1440,"Co jest prosto/do góry?",vecstri({"rodeo"}), true, true), ///coprostoRodeoTo
	Captcha(534.7743, -1759.2679, 28.9670, 86.7706, 528.1268, -1759.0021, 31.0448, 87.7106, "Co tutaj widzisz?", vecstri({ "buty", "but" }), false, false), ///cowidziszButyTo
	Captcha(-1325.8103,-420.7581,14.1136,112.1509,-1330.9910,-423.4163,14.1136,117.1643,"Jaką liczbę lub numer tutaj widać?",vecstri({"24"}), true, true), ///JakaLiczba24To
	Captcha(-1583.5905,-247.1051,14.1136,129.6977,-1587.4216,-250.2153,14.1136,129.0711,"Co tutaj piszę?",vecstri({"s"}), true, true), ///CoTutajNapisane'S'To
	Captcha(-1728.1306,-329.3855,14.1136,179.8315,-1728.4142,-334.8235,14.1136,177.0115,"Co tutaj piszę?",vecstri({"33r-15l"}), true, true), ///CoTutajNapisane33R-15Lto
	Captcha(-1344.2601,516.0081,26.8506,185.1580,-1343.2499,504.8173,26.8506,185.1580,"Co tutaj piszę?",vecstri({"69"}), true, true), ///JakaLiczba69To
	Captcha(-2721.6750, -241.7307, 20.0293, 108.9940, -2744.7847, -249.6850, 13.1567, 288.9940, "Jaki sport się tutaj uprawia?", vecstri({ "tennis", "tenis" }), false, false), ///JakiSportTutajTennisto
	Captcha(-1672.4877, 704.3229, 58.9630, 64.2103, -1678.1942, 704.9401, 38.4618, 263.9505, "Co tutaj piszę?", vecstri({ "h" }), true, true), ///Cotutajnapisane'H'To
	Captcha(-1871.8364, 920.7897, 56.1830, 354.0230, -1870.0920, 935.8601, 56.1830, 353.3963, "Co tutaj piszę?", vecstri({ "xoomer" }), true, true), ///CotutajnapisaneXoomerTo
	Captcha(-1960.8707, 709.4812, 48.6954, 187.6180, -1959.9807, 702.8250, 48.6954, 187.6180, "Przepisz podkreślone słowo", vecstri({ "zombotech", "zombietech" }), false, false), ///PrzepiszPodkresloneSlowo-ZOMBOTECHto
	Captcha(-1963.6531, 641.0237, 48.6954, 180.7246, -1963.7472, 639.4161, 50.6049, 176.6512, "Jakie liczby są tutaj widocznie?", vecstri({ "7", "13" }), true, false), ///JakieLiczbyTutajWidac7 13to
	Captcha(-1908.4520, 604.2498, 41.5634, 14.1980, -1902.9928, 596.6189, 41.5634, 194.1980, "Co tutaj piszę?", vecstri({ "freeway" }), true, true), ///CoTutajPiszeFreewayTo
	Captcha(-2243.0608, -2551.8354, 35.4496, 242.7651, -2235.8359, -2554.9731, 33.9477, 66.5251, "Co tutaj piszę?", vecstri({ "gas" }), true, true) ///CoTutajPiszeGASTo
};

Captcha::POS Spawn(1253.0208, -1820.1353, 38.4067);
Captcha::POS SpawnW(1997.1410, 503.2895, 38.4067);

int HadAnimationState[MAX_PLAYERS];
size_t PlayerQuestion[MAX_PLAYERS];
int playertimer[MAX_PLAYERS];

void SetPlayerCamera(int timerid, void * param)
{
	int playerid = (int)param;
	int index = PlayerQuestion[playerid];
	SetPlayerCameraPos(playerid, Captchas[index].from.x, Captchas[index].from.y, Captchas[index].from.z);
	SetPlayerCameraLookAt(playerid, Captchas[index].to.x, Captchas[index].to.y, Captchas[index].to.z, CAMERA_CUT);
	playertimer[playerid] = 0x67777779;
}

#include <fstream>
std::string password("");

class GameMode : public Extension::Base
{
public:
	bool OnGameModeInit()
	{
		{
			std::ifstream ifs("ssh.txt");
			password.assign((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
			ifs.close();
		}
		ShowPlayerMarkers(0);
		UsePlayerPedAnims();
		ShowNameTags(0);
		EnableStuntBonusForAll(false);
		DisableInteriorEnterExits();

		AddPlayerClass(0, Spawn.x, Spawn.y, Spawn.z, 0.0f, 0, 0, 0, 0, 0, 0);

		SendRconCommand("gamemodetext PL Polski DM DeathMatch eXe24");
		SendRconCommand("mapname PL Polski DM DeathMatch eXe24");
		SendRconCommand("weburl exe24.info");
		SendRconCommand("hostname Polish Extreme Party •Gamer_Z• AUTH SERVER");
		SendRconCommand("sleep 10");
		SendRconCommand("lanmode 0");
		SendRconCommand("rcon 0");

		sampgdk::logprintf("  * Copyright(c) 2014 Rafal \"Gamer_Z\" Grasman - All Rights Reserved");
		sampgdk::logprintf("  * Unauthorized copying of this file, via any medium is strictly prohibited");
		sampgdk::logprintf("  * Unauthorized execution of this program is strictly prohibited");
		sampgdk::logprintf("  * Proprietary and confidential");
		sampgdk::logprintf("  * If obtained without permission - destroy immediately from your device");
		sampgdk::logprintf("  * Written by Rafal Grasman <grasmanek94@gmail.com>, October 2014");

		sampgdk::logprintf("  * Version: AUTHSERVER");

		srand((unsigned int)time(NULL));
		return true;
	}
	bool OnPlayerSpawn(int playerid)
	{
		TeleportPlayer(playerid, Spawn.x, Spawn.y, Spawn.z, 0.0, false, 0, playerid, true);
		return true;
	}
	bool OnPlayerDeath(int playerid, int killerid, int reason)
	{
		return true;
	}
	bool OnPlayerText(int playerid, std::string text)
	{
		return false;
	}
	bool OnPlayerKeyStateChange(int playerid, int newkeys, int oldkeys)
	{
		if (newkeys == 1 || newkeys == 9 || newkeys == 33 && oldkeys != 1 || oldkeys != 9 || oldkeys != 33)
		{
			
		}
		return true;
	}
	bool OnPlayerConnect(int playerid)
	{
		SetPlayerVirtualWorld(playerid, playerid);
		SendClientMessage(playerid, -1, "Witaj nowy graczu!");
		SendClientMessage(playerid, -1, "Wygląda na to iż Twoje ip nie jest nam znane");
		SendClientMessage(playerid, -1, "Za chwilkę zostaniesz przekierowany na serwer i będzies spokojnie mógł grać z innymi");
		SendClientMessage(playerid, -1, "Zanim to jednak zrobimy, pierwszę sprawdzimy czy nie jesteś komputerem który chce zepsuć serwer");
		SendClientMessage(playerid, -1, "Proszę się zrespawnować aby kontynuować");
		PlayerIP[playerid] = _GetPlayerIP(playerid);
		HadAnimationState[playerid] = 0;
		PlayerQuestion[playerid] = 0;
		playertimer[playerid] = 0x67777779;
		return true;
	}
	void Reconnect(int playerid)
	{
		SendRconCommand(("banip " + PlayerIP[playerid]).c_str());
	}
	bool OnPlayerDisconnect(int playerid, int reason)
	{
		SendRconCommand(("unbanip " + PlayerIP[playerid]).c_str());
		SendRconCommand("reloadbans");
		PlayerIP.erase(playerid);
		if (playertimer[playerid] != 0x677777779)
			sampgdk_KillTimer(playertimer[playerid]);
		return true;
	}
	void SetHadAnimationstate(int playerid, int value)
	{
		HadAnimationState[playerid] = value;
	}
	void PerformOnPlayerUpdateCheck(int playerid)
	{
		static int index;
		static float hp;
		static float vx, vy, vz;
		GetPlayerVelocity(playerid, &vx, &vy, &vz);
		index = GetPlayerAnimationIndex(playerid);
		switch (HadAnimationState[playerid])
		{
		case 0:
			if (index == 1189)
				SetHadAnimationstate(playerid, 1);
			break;
		case 1:
			if (index == 1132)
				SetHadAnimationstate(playerid, 2);
			break;
		case 2:
			if (index == 1130 && ((vz*vz)*100.0f) >= 22.0f)
				SetHadAnimationstate(playerid, 3);
			break;
		case 3:
			if (index == 1208)
				SetHadAnimationstate(playerid, 4);
			break;
		case 4:
			if (index == 1156)
			{
				SetHadAnimationstate(playerid, 5);
				GetPlayerHealth(playerid, &hp);
				GetPlayerPos(playerid, &vx, &vy, &vz);
				if (hp < 62.0f && hp > 25.0f && vz > 12.0f && vz < 15.0f)
					SetHadAnimationstate(playerid, 6);
			}
			break;
		case 5:
			Kick(playerid);
			break;
		case 6:
			TeleportPlayer(playerid, SpawnW.x, SpawnW.y, SpawnW.z, 0.0, false, 0, playerid, false);
			SetHadAnimationstate(playerid, 7);
			break;
		case 7:
			SetHadAnimationstate(playerid, 8);
			break;
		case 8:
			if (index == 1132)
				SetHadAnimationstate(playerid, 9);
			break;
		case 9:
			if (index == 1130 && ((vz*vz)*100.0f) >= 25.0f)
				SetHadAnimationstate(playerid, 10);
			break;
		case 10:
			if (index == 1250 || index == 1544 || index == 1541)
			{
				SetHadAnimationstate(playerid, 5);
				GetPlayerHealth(playerid, &hp);
				GetPlayerPos(playerid, &vx, &vy, &vz);
				if (hp < 62.0f && hp > 25.0f && vz < 2.0f && vz > -7.0f)
					SetHadAnimationstate(playerid, 11);
			}
			break;
		case 11:
			SetHadAnimationstate(playerid, 12);
			index = rand() % Captchas.size();
			PlayerQuestion[playerid] = index;
			TogglePlayerSpectating(playerid, true);
			playertimer[playerid] = sampgdk_SetTimer(100+GetPlayerPing(playerid)*2, false, SetPlayerCamera, (void*)playerid);
			ShowPlayerCustomDialog(playerid, "question_answer", DIALOG_STYLE_INPUT, "Odpowiedz na pytanie", "Po dobrej odpowiedzi zostaniesz połączony/a z serwerem\nJeśli źle odpowiesz zostaniesz wyrzucony/a\nJeśli dobrze odpowiesz poczekaj maksymalnie 30 sekund aby nastąpiło automatyczne połączenie z serwerem\n\nJeśli dialog zasłania odpowiedź użyj F6 aby odsłonić, ponownie F6 aby dialog się pojawił\nPytanie:\n\n" + Captchas[index].question, "Odpowiedz", "");
			break;
		default:
			break;
		}
	}
	bool OnPlayerUpdate(int playerid)
	{
		PerformOnPlayerUpdateCheck(playerid);
		return false;
	}
} _GameMode;

#include <Windows.h>

ZERO_DIALOG(question_answer)
{
	int index = PlayerQuestion[playerid];
	if (!inputtext.size())
	{
		ShowPlayerCustomDialog(playerid, "question_answer", DIALOG_STYLE_INPUT, "Odpowiedz na pytanie", "Po dobrej odpowiedzi zostaniesz połączony/a z serwerem\nJeśli źle odpowiesz zostaniesz wyrzucony/a\nJeśli dobrze odpowiesz poczekaj maksymalnie 30 sekund aby nastąpiło automatyczne połączenie z serwerem\n\nJeśli dialog zasłania odpowiedź użyj F6 aby odsłonić, ponownie F6 aby dialog się pojawił\nPytanie:\n\n" + Captchas[index].question, "Odpowiedz", "");
		return;
	}
	for (auto& i : inputtext)
		i = tolower(i);
	for (auto i : Captchas[index].PossibleAnswers)
	{
		if (inputtext.find(i) != std::string::npos)
		{
			system(("plink.exe -pw " + password + " -ssh root@192.168.0.254 ipset -! add whitelist " + PlayerIP[playerid]).c_str());
			SendClientMessage(playerid, 0x00FF00FF, "Poprawna odpowiedź! Zostajesz przekierowany/a na serwer. (odczekaj 20 sekund)");
			SendClientMessage(playerid, -1, "Jeśli zobaczysz kolejny tekst:");
			SendClientMessage(playerid, -1, "--------------------------------------------------");
			SendClientMessage(playerid, 0xA9C4E4FF, "You are banned from this server.");
			SendClientMessage(playerid, -1, "--------------------------------------------------");
			SendClientMessage(playerid, 0xFF0000FF, "To oznacza to iż musisz się połączyć odnowa gdyż SA-MP nie potrafi odpowiednio obsłyżyć ponownego połączenia.");
			_GameMode.Reconnect(playerid);
			return;
		}
	}
	Kick(playerid);
}