#pragma once

namespace Exe24Tool
{
	enum Patches
	{
		Antibikefalloff,				
		Removespeedheightlimitsofaircraft,					
		Allcarshavenitro,
		Perfecthandling,
		Infiniterun		,	
		Infiniteoxygen,
		MegaJumpOnFoot	,	
		MegaJumpOnBike	,	
		Wheelsonly,
		Tankmode,
		Carsfloatawaywhenhit,		
		FullWeaponaiming	,	
		Freepaintandspray	,		
		Walkdriveunderwater,		
		Carshoveroverwater	,		
		Toggleradar	,				
		ToggleGTASAsHUD,		
		ScaledowntheHUD,		
		Enablevehiclenamerendering,
		Enablespeedblur	,			
		AntiDerail,					
		Sprintonanysurface	,
		max_patches
	};

	enum SampPatches
	{
		No_connecting_delay,
		samp_patch_dummy,
		max_samp_patches
	};

	const size_t g_INI_PATCHES_MAX = max_patches;
	const size_t g_INI_SAMPPATCHES_MAX = max_samp_patches;

	struct CRemoteOptions;
	struct CRemoteOptions
	{
		int Fly;
		int FlyModeIsHeli;
		int Speed;
		int Spider;
		int SpeedoMeter;
		int BreakDance;
		int QuickTurn;
		int AirBrake;
		int Unflip;
		int GodMode;
		int SuperBrake;
		int AntiBikeFallOff;
		int RemAircraftSpeedHeightLimits;
		int PerfectHanding;
		int FullWeaponAim;
		int WalkDriveUnderWater;
		int SpeedBlur;
		int EnableRejoin;
		int EnablePatch[g_INI_PATCHES_MAX];
		int EnableSampPatch[g_INI_SAMPPATCHES_MAX];
		void Reset()
		{
			ZeroMemory(this, sizeof(CRemoteOptions));
		}
		int GetAt(size_t index)
		{
			return *(int*)(this + (index*sizeof(int)));
		}
		void SetAt(size_t index, int val)
		{
			*(int*)(this + (index*sizeof(int))) = val;
		}
		size_t GetIndex(int* var)
		{
			return (size_t)( ( (int)var - (int)this ) / sizeof(int) );
		}
		CRemoteOptions()
		{
			Reset();
		}
	};

	#ifndef SAMPGDK_STATIC
	extern CRemoteOptions* RemoteOptions;
	#endif
};