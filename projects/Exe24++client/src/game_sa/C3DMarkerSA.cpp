/*****************************************************************************
*
*  PROJECT:		Multi Theft Auto v1.0
*  LICENSE:		See LICENSE in the top level directory
*  FILE:		game_sa/C3DMarkerSA.cpp
*  PURPOSE:		3D Marker entity
*  DEVELOPERS:	Ed Lyons <eai@opencoding.net>
*               Christian Myhre Lundheim <>
*               Cecill Etheredge <ijsf@gmx.net>
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#include <main.h>

void C3DMarkerSA::GetMatrix ( CMatrix * pMatrix )
{
    CMatrix_Padded * mat = &GetInterface ()->m_mat;
    memcpy ( &pMatrix->vPos, &mat->vPos, sizeof ( CVector ) );
    memcpy ( &pMatrix->vFront, &mat->vFront, sizeof ( CVector ) );
    memcpy ( &pMatrix->vRight, &mat->vRight, sizeof ( CVector ) );
    memcpy ( &pMatrix->vUp, &mat->vUp, sizeof ( CVector ) );
}


void C3DMarkerSA::SetMatrix ( CMatrix * pMatrix )
{
    CMatrix_Padded * mat = &GetInterface ()->m_mat;
    memcpy ( &mat->vPos, &pMatrix->vPos, sizeof ( CVector ) );
    memcpy ( &mat->vFront, &pMatrix->vFront, sizeof ( CVector ) );
    memcpy ( &mat->vRight, &pMatrix->vRight, sizeof ( CVector ) );
    memcpy ( &mat->vUp, &pMatrix->vUp, sizeof ( CVector ) );
}

VOID C3DMarkerSA::SetPosition(CVector * vecPosition)
{
	DEBUG_TRACE("VOID C3DMarkerSA::SetPosition(CVector * vecPosition)");
    this->GetInterface()->m_mat.vPos = *vecPosition;
}

CVector * C3DMarkerSA::GetPosition()
{
	DEBUG_TRACE("CVector * C3DMarkerSA::GetPosition()");
	return &this->GetInterface()->m_mat.vPos;
}

DWORD C3DMarkerSA::GetType()
{
	DEBUG_TRACE("DWORD C3DMarkerSA::GetType()");
	return this->GetInterface()->m_nType;
}

VOID C3DMarkerSA::SetType(DWORD dwType)
{
	DEBUG_TRACE("VOID C3DMarkerSA::SetType(DWORD dwType)");
	this->GetInterface()->m_nType = (unsigned short)(dwType);
}

BOOL C3DMarkerSA::IsActive()
{
	DEBUG_TRACE("BOOL C3DMarkerSA::IsActive()");
	return (BOOL)this->GetInterface()->m_bIsUsed;
}

DWORD C3DMarkerSA::GetIdentifier()
{
	DEBUG_TRACE("DWORD C3DMarkerSA::GetIdentifier()");
	return this->GetInterface()->m_nIdentifier;
}

RGBA C3DMarkerSA::GetColor()
{
	DEBUG_TRACE("RGBA C3DMarkerSA::GetColor()");
    // From ABGR
    unsigned long ulABGR = this->GetInterface()->rwColour;
	return ( ulABGR >> 24 ) | ulABGR | ( ulABGR >> 8 ) | ( ulABGR >> 16 );
}

VOID C3DMarkerSA::SetColor(RGBA color)
{
    // To ABGR
	this->GetInterface()->rwColour = ( color.A << 24 ) | ( color.B << 16 ) | ( color.G << 8 ) | color.R;
}

VOID C3DMarkerSA::SetPulsePeriod(WORD wPulsePeriod)
{
	DEBUG_TRACE("VOID C3DMarkerSA::SetPulsePeriod(WORD wPulsePeriod)");
	this->GetInterface()->m_nPulsePeriod = wPulsePeriod;
}

VOID C3DMarkerSA::SetRotateRate(short RotateRate)
{
	DEBUG_TRACE("VOID C3DMarkerSA::SetRotateRate(short RotateRate)");
	this->GetInterface()->m_nRotateRate = RotateRate;
}

FLOAT C3DMarkerSA::GetSize()
{
	DEBUG_TRACE("FLOAT C3DMarkerSA::GetSize()");
	return this->GetInterface()->m_fSize;
}

VOID C3DMarkerSA::SetSize(FLOAT fSize)
{
	DEBUG_TRACE("VOID C3DMarkerSA::SetSize(FLOAT fSize)");
	this->GetInterface()->m_fSize = fSize;
}

FLOAT C3DMarkerSA::GetBrightness()
{
	DEBUG_TRACE("FLOAT C3DMarkerSA::GetBrightness()");
	return this->GetInterface()->m_fBrightness;
}

VOID C3DMarkerSA::SetBrightness(FLOAT fBrightness)
{
	DEBUG_TRACE("VOID C3DMarkerSA::SetBrightness(FLOAT fBrightness)");
	this->GetInterface()->m_fBrightness = fBrightness;
}

VOID C3DMarkerSA::SetCameraRange(FLOAT fCameraRange)
{
	DEBUG_TRACE("VOID C3DMarkerSA::SetCameraRange(FLOAT fCameraRange)");
	this->GetInterface()->m_fCameraRange = fCameraRange;
}

VOID C3DMarkerSA::SetPulseFraction(FLOAT fPulseFraction)
{
	DEBUG_TRACE("VOID C3DMarkerSA::SetPulseFraction(FLOAT fPulseFraction)");
	this->GetInterface()->m_fPulseFraction = fPulseFraction;
}

FLOAT C3DMarkerSA::GetPulseFraction()
{
	DEBUG_TRACE("FLOAT C3DMarkerSA::GetPulseFraction()");
	return this->GetInterface()->m_fPulseFraction;
}

VOID C3DMarkerSA::Disable ()
{
	DEBUG_TRACE("VOID C3DMarkerSA::Disable ()");
	this->GetInterface()->m_nIdentifier = 0;
}

VOID C3DMarkerSA::DeleteMarkerObject ()
{
	if ( this->GetInterface()->m_pRwObject )
	{
		DWORD dwFunc = FUNC_DeleteMarkerObject;
		DWORD dwThis = (DWORD)this->GetInterface();
		_asm
		{
			mov		ecx, dwThis
			call	dwFunc
		}

		//OutputDebugString ( "Object destroyed!" );
	}
	else
	{
		//OutputDebugString ( "No object to destroy!" );
	}
}

VOID C3DMarkerSA::Reset()
{
    this->internalInterface->m_lastPosition = this->internalInterface->m_mat.vPos;
}