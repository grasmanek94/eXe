/*****************************************************************************
*
*  PROJECT:		Multi Theft Auto v1.0
*  LICENSE:		See LICENSE in the top level directory
*  FILE:		game_sa/CAnimBlockSA.cpp
*  PURPOSE:		Animation block
*  DEVELOPERS:	Jax <>
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#include <main.h>

int CAnimBlockSAInterface::GetIndex ( void )
{
    return ( ( ( DWORD ) this - ARRAY_CAnimManager_AnimBlocks ) / 32 );
}