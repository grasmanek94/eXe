/*****************************************************************************
*
*  PROJECT:		Multi Theft Auto v1.0
*  LICENSE:		See LICENSE in the top level directory
*  FILE:		game_sa/CGameSA.h
*  PURPOSE:		Header file for base game logic handling class
*  DEVELOPERS:	Ed Lyons <eai@opencoding.net>
*               Christian Myhre Lundheim <>
*               Jax <>
*               Cecill Etheredge <ijsf@gmx.net>
*               Stanislav Bobrov <lil_toady@hotmail.com>
*               Alberto Alonso <rydencillo@gmail.com>
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#ifndef __CGAMESA
#define __CGAMESA

#include "CModelInfoSA.h"

#define     MAX_MEMORY_OFFSET_1_0           0xCAF008

#define		CLASS_CPlayerInfo				0xB7CD98	// ##SA##
#define		CLASS_CCamera					0xB6F028	// ##SA##
#define		CLASS_CPad						0xB73458	// ##SA##
#define     CLASS_CGarages                  0x96C048    // ##SA##
#define     CLASS_CFx                       0xa9ae00    // ##SA##

#define     CLASS_RwCamera                  0xB6F97C

#define		ARRAY_WeaponInfo				0xC8AAB8    // ##SA##
#define		CLASSSIZE_WeaponInfo			112         // ##SA##
#define     NUM_WeaponInfosStdSkill         WEAPONTYPE_LAST_WEAPONTYPE
#define     NUM_WeaponInfosOtherSkill       11
#define     NUM_WeaponInfosTotal            (NUM_WeaponInfosStdSkill + (3*NUM_WeaponInfosOtherSkill)) // std, (poor, pro, special)

#define     MODELINFO_LAST_PLAYER_ID        288         // ??
#define     MODELINFO_MAX                   65535

#define		FUNC_GetLevelFromPosition		0x4DD300

#define		FUNC_CDebug_DebugDisplayTextBuffer		0x532260
#define     FUNC_JPegCompressScreenToFile   0x5D0820

#define     VAR_FlyingCarsEnabled           0x969160 // ##SA##
#define     VAR_ExtraBunnyhopEnabled        0x969161 // ##SA##
#define     VAR_HoveringCarsEnabled         0x969152 // ##SA##
#define     VAR_ExtraJumpEnabled            0x96916C // ##SA##

#define CHEAT_HOVERINGCARS   "hovercars"
#define CHEAT_FLYINGCARS     "aircars"
#define CHEAT_EXTRABUNNYHOP  "extrabunny"
#define CHEAT_EXTRAJUMP      "extrajump"

class CGameSA : public CGame
{
    friend class COffsets;

private:
	CWeaponInfo			* WeaponInfos[NUM_WeaponInfosTotal];
	CModelInfoSA		ModelInfo[MODELINFO_MAX];
public:
	CGameSA(); // constructor
    ~CGameSA ();

    inline CPools				    * GetPools()				{ DEBUG_TRACE("CPools     * GetPools()"); return m_pPools; };
	inline CPlayerInfo			    * GetPlayerInfo()			{ DEBUG_TRACE("CPlayerInfo    * GetPlayerInfo()");return m_pPlayerInfo; };
	inline CProjectileInfo		    * GetProjectileInfo()		{ DEBUG_TRACE("CProjectileInfo   * GetProjectileInfo()");return m_pProjectileInfo; };
	inline CRadar				    * GetRadar()				{ DEBUG_TRACE("CRadar     * GetRadar()"); return m_pRadar; };
	inline CRestart				    * GetRestart()				{ DEBUG_TRACE("CRestart    * GetRestart()"); return m_pRestart; };
	inline CClock				    * GetClock()				{ DEBUG_TRACE("CClock     * GetClock()"); return m_pClock; };
	inline CCoronas				    * GetCoronas()				{ DEBUG_TRACE("CCoronas    * GetCoronas()"); return m_pCoronas; };
	inline CCheckpoints			    * GetCheckpoints()			{ DEBUG_TRACE("CCheckpoints   * GetCheckpoints()"); return m_pCheckpoints; };
	inline CEventList			    * GetEventList()			{ DEBUG_TRACE("CEventList    * GetEventList()"); return m_pEventList; };
	inline CFireManager			    * GetFireManager()			{ DEBUG_TRACE("CFireManager   * GetFireManager()"); return m_pFireManager; };
	inline CExplosionManager	    * GetExplosionManager()		{ DEBUG_TRACE("CExplosionManager  * GetExplosionManager()");return m_pExplosionManager; };
	inline CGarages				    * GetGarages()				{ DEBUG_TRACE("CGarages    * GetGarages()"); return m_pGarages; };
	inline CHud					    * GetHud()					{ DEBUG_TRACE("CHud     * GetHud()"); return m_pHud; };
	inline CWeather				    * GetWeather()				{ DEBUG_TRACE("CWeather    * GetWeather()");return m_pWeather; };
	inline CWorld				    * GetWorld()				{ DEBUG_TRACE("CWorld     * GetWorld()"); return m_pWorld; };
	inline CCamera				    * GetCamera()				{ DEBUG_TRACE("CCamera     * GetCamera()"); return m_pCamera; };
	inline CPickups				    * GetPickups()				{ DEBUG_TRACE("CPickups    * GetPickups()"); return m_pPickups; };
	inline C3DMarkers			    * Get3DMarkers()			{ DEBUG_TRACE("C3DMarkers    * Get3DMarkers()");return m_p3DMarkers; };
	inline CPad					    * GetPad()					{ DEBUG_TRACE("CPad     * GetPad()");return m_pPad; };
	inline CTheCarGenerators	    * GetTheCarGenerators()		{ DEBUG_TRACE("CTheCarGenerators  * GetTheCarGenerators()");return m_pTheCarGenerators; };
	inline CAERadioTrackManager	    * GetAERadioTrackManager()	{ DEBUG_TRACE("CAERadioTrackManager * GetAERadioTrackManager()");return m_pCAERadioTrackManager; };
	inline CAudio				    * GetAudio()				{ DEBUG_TRACE("CAudio     * GetAudio()");return m_pAudio; };
	inline CMenuManager			    * GetMenuManager()			{ DEBUG_TRACE("CMenuManager			* GetMenuManager()");return m_pMenuManager; };
	inline CText				    * GetText()					{ DEBUG_TRACE("CText					* GetText()");return m_pText; };
	inline CStats				    * GetStats()				{ DEBUG_TRACE("CStats					* GetStats()");return m_pStats; };
	inline CFont				    * GetFont()					{ DEBUG_TRACE("CFont					* GetFont()");return m_pFont; };
	inline CPathFind			    * GetPathFind()				{ DEBUG_TRACE("CPathFind				* GetPathFind()");return m_pPathFind; };
	inline CPopulation			    * GetPopulation()			{ DEBUG_TRACE("CPopulation				* GetPopulation()");return m_pPopulation; };
    inline CTaskManagementSystem    * GetTaskManagementSystem() { DEBUG_TRACE("CTaskManagementSystemSA * GetTaskManagementSystem()");return m_pTaskManagementSystem; };
    inline CTasks                   * GetTasks() { DEBUG_TRACE("CTasks * GetTasks()");return m_pTasks; };
    inline CGameSettings            * GetSettings()             { DEBUG_TRACE("CGameSettings * GetSettings()");return m_pSettings; };
    inline CCarEnterExit            * GetCarEnterExit()         { DEBUG_TRACE("CCarEnterExit           * GetCarEnterExit()");return m_pCarEnterExit; };
    inline CControllerConfigManager * GetControllerConfigManager()  { DEBUG_TRACE("CControllerConfigManager* GetControllerConfigManager()");return m_pControllerConfigManager; };
	inline CRenderWare              * GetRenderWare()           { DEBUG_TRACE("CRenderWare * GetRenderWare()");return m_pRenderWare; };
    inline IHandlingManager         * GetHandlingManager ()      { return m_pHandlingManager; };
    inline CAnimManager             * GetAnimManager ()          { return m_pAnimManager; }
    inline CStreaming               * GetStreaming ()            { return m_pStreaming; }
    inline CVisibilityPlugins       * GetVisibilityPlugins ()    { return m_pVisibilityPlugins; }
    inline CKeyGen                  * GetKeyGen ()               { return m_pKeyGen; }
    inline CRopes                   * GetRopes ()                { return m_pRopes; }
    inline CFx                      * GetFx ()                   { return m_pFx; }
    inline CWaterManager            * GetWaterManager ()         { return m_pWaterManager; }

	CWeaponInfo				* GetWeaponInfo(eWeaponType weapon,eWeaponSkill skill=WEAPONSKILL_STD);
	CModelInfo				* GetModelInfo( DWORD dwModelID );

	inline DWORD            GetSystemTime (  )		{ DEBUG_TRACE("DWORD     GetSystemTime (  )");return *VAR_SystemTime; };
	inline BOOL             IsAtMenu (  )			{ DEBUG_TRACE("BOOL     IsAtMenu (  )");if(*VAR_IsAtMenu) return TRUE; else return FALSE; };
	inline BOOL             IsGameLoaded (  )		{ DEBUG_TRACE("BOOL     IsGameLoaded (  )");if(*VAR_IsGameLoaded) return TRUE; else return FALSE; };
	VOID					StartGame ( );
	VOID					SetSystemState ( eSystemState State );
	eSystemState			GetSystemState ( );
	inline BOOL             IsNastyGame (  )					{ DEBUG_TRACE("BOOL     IsNastyGame (  )"); return *VAR_IsNastyGame; };
    inline VOID             SetNastyGame ( BOOL IsNasty )		{ DEBUG_TRACE("VOID     SetNastyGame ( BOOL IsNasty )"); *VAR_IsNastyGame = IsNasty?true:false; };
	VOID					Pause ( bool bPaused );
	bool					IsPaused ( );
    bool                    IsInForeground ( );
	VOID					DisableRenderer( bool bDisabled );
    VOID                    TakeScreenshot ( char * szFileName );
    DWORD                   * GetMemoryValue ( DWORD dwOffset );

	VOID					SetRenderHook ( InRenderer* pInRenderer );

    void                    Initialize  ( void );
    void                    Reset                       ( void );
    void                    Terminate ( void );

    eGameVersion            GetGameVersion ( void );
    eGameVersion            FindGameVersion ( void );

    float                   GetFPS ( void );
    float                   GetTimeStep ( void );
    float                   GetOldTimeStep ( void );
    float                   GetTimeScale ( void );
    void                    SetTimeScale ( float fTimeScale );

    unsigned long           GetFramelimiter         ( void );
    void                    SetFramelimiter         ( unsigned long ulFramelimiter );

	BOOL					InitLocalPlayer(  );

    float                   GetGravity              ( void );
    void                    SetGravity              ( float fGravity );

    float                   GetGameSpeed            ( void );
    void                    SetGameSpeed            ( float fSpeed );

    unsigned char           GetBlurLevel            ( void );
    void                    SetBlurLevel            ( unsigned char ucLevel );

	unsigned long			GetMinuteDuration		( void );
	void					SetMinuteDuration		( unsigned long ulTime );

    bool                    IsCheatEnabled          ( const char* szCheatName );
    bool                    SetCheatEnabled         ( const char* szCheatName, bool bEnable );
    void                    ResetCheats             ();

    bool                    VerifySADataFileNames   ();

private:
	CPools					* m_pPools;
	CPlayerInfo				* m_pPlayerInfo;
	CProjectileInfo			* m_pProjectileInfo;
	CRadar					* m_pRadar;
	CRestart				* m_pRestart;
	CClock					* m_pClock;
	CCoronas				* m_pCoronas;
	CCheckpoints			* m_pCheckpoints;
	CEventList				* m_pEventList;
	CFireManager			* m_pFireManager;
	CGarages				* m_pGarages;
	CHud					* m_pHud;
	CWanted					* m_pWanted;
	CWeather				* m_pWeather;
	CWorld					* m_pWorld;
	CCamera					* m_pCamera;
	CModelInfo				* m_pModelInfo;	
	CPickups				* m_pPickups;
	CWeaponInfo				* m_pWeaponInfo;
	CExplosionManager		* m_pExplosionManager;
	C3DMarkers				* m_p3DMarkers;
	CRenderWare				* m_pRenderWare;
    IHandlingManager        * m_pHandlingManager;
    CAnimManager            * m_pAnimManager;
    CStreaming              * m_pStreaming;
    CVisibilityPlugins      * m_pVisibilityPlugins;
    CKeyGen                 * m_pKeyGen;
    CRopes                  * m_pRopes;
    CFx                     * m_pFx;
    CWaterManager           * m_pWaterManager;

	CPad					    * m_pPad;
	CTheCarGenerators		    * m_pTheCarGenerators;
	CAERadioTrackManager	    * m_pCAERadioTrackManager;
	CAudio					    * m_pAudio;
	CMenuManager			    * m_pMenuManager;
	CText					    * m_pText;
	CStats					    * m_pStats;
	CFont					    * m_pFont;
	CPathFind				    * m_pPathFind;
	CPopulation				    * m_pPopulation;
    CTaskManagementSystem       * m_pTaskManagementSystem; // not used outside the game_sa
    CTasks                      * m_pTasks;
    CGameSettings               * m_pSettings;
    CCarEnterExit               * m_pCarEnterExit;
    CControllerConfigManager    * m_pControllerConfigManager;

    eGameVersion            m_eGameVersion;

    static unsigned long*   VAR_SystemTime;
    static unsigned long*   VAR_IsAtMenu;
    static unsigned long*   VAR_IsGameLoaded;
    static bool*            VAR_GamePaused;
    static bool*            VAR_IsForegroundWindow;;
    static unsigned long*   VAR_SystemState;
    static void*            VAR_StartGame;
    static bool*            VAR_IsNastyGame;
    static float*           VAR_TimeScale;
    static float*           VAR_FPS;
    static float*           VAR_OldTimeStep;
    static float*           VAR_TimeStep;
    static unsigned long*   VAR_Framelimiter;

    std::map < std::string, BYTE* > m_Cheats;
};

#endif

