/*****************************************************************************
*
*  PROJECT:		Multi Theft Auto v1.0
*  LICENSE:		See LICENSE in the top level directory
*  FILE:		sdk/game/CExplosionManager.h
*  PURPOSE:		Explosion manager interface
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#ifndef __CGAME_EXPLOSIONMANAGER
#define __CGAME_EXPLOSIONMANAGER

#include <CVector.h>
#include "CExplosion.h"
#include "Common.h"
#include "CEntity.h"

class CExplosionManager
{
public:
	virtual CExplosion *        AddExplosion                ( CEntity * pExplodingEntity, CEntity * pOwner, eExplosionType explosionType, CVector & vecPosition, unsigned int uiActivationDelay = 0, bool bMakeSound = true, float fCamShake = -1.0f, bool bNoDamage = false ) = 0;
	virtual VOID			    RemoveAllExplosionsInArea   ( CVector * vecPosition, FLOAT fRadius ) = 0;
	virtual VOID			    RemoveAllExplosions         ( void ) = 0;
	virtual CExplosion *        GetExplosion                ( DWORD ID ) = 0;
	virtual CExplosion *        FindFreeExplosion           ( void ) = 0;
	virtual int					GetExplosionActiveCount		( void ) = 0;
};

#endif