/*

	PROJECT:		mod_sa
	LICENSE:		See LICENSE in the top level directory
	COPYRIGHT:		Copyright we_sux, FYP

	mod_sa is available from http://code.google.com/p/m0d-s0beit-sa/

	mod_sa is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	mod_sa is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with mod_sa.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "main.h"
#include <psapi.h>

/* Note that this is 100% C code, not C++. :p */
#ifndef MIN
#define MIN( a, b ) ( (a) > (b) ? (b) : (a) )
#endif

// old camera thing, still used in RenderMapDot()
float						*cam_matrix = (float *)0xB6F99C;

uint64_t					__time_current;
double						g_timeDiff;

struct pool					*pool_actor;
struct pool					*pool_vehicle;

static struct cheat_state	__cheat_state;
struct cheat_state			*cheat_state = NULL;

static void cheat_main_actor ( double time_diff )
{
	traceLastFunc( "cheat_main_actor()" );

	struct actor_info	*info = actor_info_get( ACTOR_SELF, 0 );
	if ( info == NULL )
	{
		Log( "wtf, actor_info_get() returned NULL." );
		return;
	}

	vect3_copy( &info->base.matrix[4 * 3], cheat_state->actor.coords );

	cheat_handle_hp( NULL, info, time_diff );

	// cheat_handle_SpiderFeet(info, time_diff);
	cheat_handle_actor_fly(info, time_diff);
}

static void cheat_main_vehicle ( double time_diff )
{
	traceLastFunc( "cheat_main_vehicle()" );

	struct vehicle_info *info = vehicle_info_get( VEHICLE_SELF, 0 );
	if ( info == NULL )
		return;

	// copy vehicle coords to cheat_state storage
	vect3_copy( &info->base.matrix[4 * 3], cheat_state->vehicle.coords );

	// the following functions can be found in cheat_generic.cpp
	cheat_handle_hp( info, NULL, time_diff );

	// the following functions can be found in cheat_vehicle.cpp

	cheat_handle_vehicle_unflip( info, time_diff );
	cheat_handle_vehicle_nitro( info, time_diff );
	cheat_handle_vehicle_air_brake( info, time_diff );
	cheat_handle_vehicle_quick_turn( info, time_diff );
	cheat_handle_vehicle_brake( info, time_diff );

	cheat_handle_vehicle_brakedance( info, time_diff );

	cheat_handle_vehicle_fly( info, time_diff );
	cheat_handle_vehicle_spiderWheels( info, time_diff );
}

// the main daddyo
int			m_InitStages = 0;
void cheat_hook ( HWND wnd )
{
	traceLastFunc( "cheat_hook()" );

	// get the time
	static uint64_t time_last;
	__time_current = __time_get();
	g_timeDiff = TIME_TO_DOUBLE(time_get() - time_last);

	// for looping
	int i;

	// install keyhook
	keyhook_maybe_install( wnd );

	// setup menu
	menu_maybe_init();
	traceLastFunc( "cheat_hook()" );

	/* initialize state */
	if ( cheat_state == NULL )
	{
		// set default cheat_state variables
		cheat_state = &__cheat_state;
		cheat_state->_generic.spoof_weapon = -1;
		cheat_state->_generic.money = set.money_enabled;
		cheat_state->_generic.weapon = set.weapon_enabled;
		cheat_state->_generic.vehicles_unlock = false;
		cheat_state->_generic.vehicles_warp_invert = true;
		cheat_state->actor.invulnerable = true;
		cheat_state->vehicle.invulnerable = true;
		cheat_state->vehicle.hp_tire_support = true;
		cheat_state->vehicle.hp_minimum_on = 1;
		cheat_state->vehicle.hp_regen_on = 1;
		cheat_state->actor.hp_regen_on = 1;
		cheat_state->vehicle.brkdance = 0;
		cheat_state->game_speed = 1.0f;
		cheat_state->vehicle.is_engine_on = 1;
		cheat_state->_generic.interior_patch_enabled = 0;

		// refreshes any cheat_state stuff set from the INI
		ini_load_setSettings();

		/* install patches from the .ini file */
		for ( i = 0; i < Exe24Tool::max_patches; i++ )
		{
			if ( set.patch[i].name != NULL && set.patch[i].ini_auto )
				patcher_install( &set.patch[i] );
		}

		if ( g_dwSAMP_Addr != NULL )
		{
			for (i = 0; i < Exe24Tool::max_samp_patches; i++)
			{
				if ( set.sampPatch[i].name != NULL && set.sampPatch[i].ini_auto )
					patcher_install( &set.sampPatch[i] );
			}
		}
	}	/* end initialize state */

	// set cheat state to "off"
	cheat_state->state = CHEAT_STATE_NONE;

	/* setup & refresh actor pool */
	pool_actor = *(struct pool **)ACTOR_POOL_POINTER;
	if ( pool_actor == NULL || pool_actor->start == NULL || pool_actor->size <= 0 )
		return;

	/* setup & refresh vehicle pool */
	pool_vehicle = *(struct pool **)VEHICLE_POOL_POINTER;
	if ( pool_vehicle == NULL || pool_vehicle->start == NULL || pool_vehicle->size <= 0 )
		return;

	//////////////////////////////////////////
	// looks like we have pools so continue //
	struct actor_info	*actor_info;
	struct vehicle_info *vehicle_info;
	actor_info = actor_info_get( ACTOR_SELF, ACTOR_ALIVE );
	vehicle_info = vehicle_info_get( VEHICLE_SELF, 0 );

	/* no vehicle, and no actor. exit. */
	if ( vehicle_info == NULL && actor_info == NULL )
	{
		if ( cheat_state->actor.air_brake
		 ||	 cheat_state->actor.stick
		 ||	 cheat_state->vehicle.air_brake
		 ||	 cheat_state->vehicle.stick )
		{
			cheat_state->actor.air_brake = 0;
			cheat_vehicle_air_brake_set( 0 );
			cheat_state->actor.stick = 0;
			cheat_state->vehicle.stick = 0;
			cheat_state_text( "Air brake / stick disabled" );
		}
	}
	else
	{
		if ( vehicle_info == NULL )
		{
			if ( cheat_state->vehicle.air_brake || cheat_state->vehicle.stick )
			{
				cheat_vehicle_air_brake_set( 0 );
				cheat_state->vehicle.stick = 0;
				cheat_state_text( "Air brake / stick disabled" );
			}

			cheat_state->state = CHEAT_STATE_ACTOR;

			// reset infinite NOS toggle state
			if ( cheat_state->vehicle.infNOS_toggle_on )
			{
				cheat_state->vehicle.infNOS_toggle_on = false;
				patcher_remove( &patch_vehicle_inf_NOS );
			}
		}
		else
		{
			if ( cheat_state->actor.air_brake || cheat_state->actor.stick )
			{
				cheat_state->actor.air_brake = 0;
				cheat_state->actor.stick = 0;
				cheat_state_text( "Air brake / stick disabled" );
			}

			cheat_state->state = CHEAT_STATE_VEHICLE;
		}

		// post GTA initialization initizalization routines
		if ( m_InitStages == 0 )
		{
			// setup CGame & friends
			pGameInterface = new CGameSA();
			pPools = pGameInterface->GetPools();

			//pGameInterface->Initialize(); // all this does so far is disable modshops and pay&sprays
			// this seems to fix SAMP's load hang bug
			//pGameInterface->StartGame();

			// we have to add ourself to the pool first so that we are always the 1st ref
			// NEW --> use the global external: pPedSelf
			pPedSelf = pGameInterface->GetPools()->AddPed( (DWORD *)actor_info );
			pPedSelfSA = pPedSelf->GetPedInterface();

			// install all startup hooks
			cheat_hookers_installhooks();

			// increment stage
			m_InitStages++;
		}
		else if ( m_InitStages == 1 )
		{
			// load all the weapon models
			loadAllWeaponModels();

			// get funky
			//pGameInterface->GetAudio()->PlayBeatTrack(2);

			// increment stage
			m_InitStages++;
		}
	}

	if ( cheat_state->state != CHEAT_STATE_NONE )
	{
		// keep this updated, cos something is switching it now
		pPedSelf = pPools->GetPedFromRef( CPOOLS_PED_SELF_REF );
		pPedSelfSA = pPedSelf->GetPedInterface();
	}

	// more random than random
	if ( rand() % 5000 == 0 )
		srand( rand() + time_get() );

	// OMGWTFBBQ?!?
	/*if ( cheat_panic() )
	{
		cheat_state->state = CHEAT_STATE_NONE;
		goto out;
	}*/

	// menus are good
	menu_run();

	// install volatile patches from the .ini file
	for ( i = 0; i < INI_PATCHES_MAX; i++ )
	{
		if ( set.patch[i].name != NULL && set.patch[i].has_volatile && set.patch[i].installed )
			patcher_install( &set.patch[i] );
	}

	// generic stuff - these want to run even though we're dead
	cheat_handle_misc();

	gta_game_speed_set( cheat_state->game_speed );

	if ( cheat_state->state != CHEAT_STATE_NONE )
	{
		// generic stuff
		cheat_handle_weapon_disable();

		// do stuff :p
		if ( cheat_state->state == CHEAT_STATE_VEHICLE )
		{
			cheat_main_vehicle( g_timeDiff );
		}
		else if ( cheat_state->state == CHEAT_STATE_ACTOR )
		{
			cheat_main_actor( g_timeDiff );
		}

		for ( i = 0; i < INI_PATCHES_MAX; i++ )
		{
			if (set.patch[i].name != NULL)
			{
				if (Exe24Tool::RemoteOptions->EnablePatch[i] && !set.patch[i].installed)
				{
					patcher_install(&set.patch[i]);
				}
				else if (!Exe24Tool::RemoteOptions->EnablePatch[i] && set.patch[i].installed || set.patch[i].failed)
				{
					patcher_remove(&set.patch[i]);
				}
			}
		}

		for ( i = 0; i < INI_SAMPPATCHES_MAX; i++ )
		{
			if ( set.sampPatch[i].name != NULL)
			{
				if (Exe24Tool::RemoteOptions->EnableSampPatch[i] && !set.sampPatch[i].installed)
				{
					patcher_install(&set.sampPatch[i]);
				}
				else if ((!Exe24Tool::RemoteOptions->EnableSampPatch[i] && set.sampPatch[i].installed) || set.sampPatch[i].failed)
				{
					patcher_remove(&set.sampPatch[i]);
				}
			}
		}
	}	// cheat_state->state != CHEAT_STATE_NONE

	// hack some SA:MP, shall we?
	if ( g_SAMP && g_renderSAMP_initSAMPstructs )
		sampMainCheat();

//out: ;
	if ( gta_menu_active() )
		keyhook_clear_states();
	else
		keyhook_run();
	time_last = time_get();
}
