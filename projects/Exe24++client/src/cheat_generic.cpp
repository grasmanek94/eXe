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

int iClickWarpEnabled = 0;

int cheat_panic ( void )
{
	traceLastFunc( "cheat_panic()" );

	//static int	pstate_map = 0, // m0d's map
	//	pstate_d3dtext_hud = 0, // hud bar
	//	pstate_actor_hp_nSP = 0, // "Extra actor invincibility" patch (invulnerable single player enemies)
	//	pstate_actor_hp_SP = 0, // "Extra actor invincibility2" patch (no problems in SP)
	//	pstate_vehicle_hp = 0, // vehicle hp patch
	//	pstate_generic_menu = 0, // m0d's menu
	//	pstate_infnos = 0, // infinite NOS
	//	pstate_ini[INI_PATCHES_MAX],
	//	pstate_dummy_aligner; // this should all be a structure, static = DOING IT WRONG
	//int			i;

	// sa-mp related
	//static int	pstate_chat = 0, // chat
	//	pstate_deathlist = 0, // deathlist
	//	pstate_extragm = 0; // extra godmode

	/*if ( KEY_PRESSED(set.key_panic) )
	{
		// toggle panic
		cheat_state->_generic.cheat_panic_enabled ^= 1;

		if ( cheat_state->_generic.cheat_panic_enabled )
		{
			struct actor_info	*actor_info = actor_info_get( ACTOR_SELF, ACTOR_ALIVE );

			if ( actor_info )
			{
				actor_info->flags &= ~ACTOR_FLAGS_INVULNERABLE;
				actor_info->weapon_slot = 0;
			}

			// remove infinite NOS
			pstate_infnos = cheat_state->vehicle.infNOS_toggle_on;
			cheat_state->vehicle.infNOS_toggle_on = false;
			patcher_remove( &patch_vehicle_inf_NOS );

			if ( pPedSelf->GetVehicle() )
			{
				CVehicle	*pVehicleSelf = pPedSelf->GetVehicle();
				CVehicle	*pVehicleTemp = NULL;

				for ( pVehicleTemp = pVehicleSelf; pVehicleTemp != NULL; pVehicleTemp = pVehicleTemp->GetTowedVehicle() )
				{
					pVehicleTemp->SetGravity( &CVector(0.0, 0.0, -1.0) );
					pVehicleTemp->SetCanBeDamaged( true );

					if ( !set.trailer_support )
						break;
				}

				// remove inf NOS
				if ( pstate_infnos )
				{
					pVehicleSelf->RemoveVehicleUpgrade( 1010 );
					//pVehicleSelf->AddVehicleUpgrade( 1010 );
				}

				// reset overrideLights, pstate not needed, will be reactivated on demand
				if ( set.enable_car_lights_at_day_time )
					pVehicleSelf->SetOverrideLights( 0 );
			}

			// hud bar, this should probably become a cheat_state
			pstate_d3dtext_hud = set.d3dtext_hud;
			set.d3dtext_hud = 0;

			// m0d's map
			pstate_map = cheat_state->_generic.map;
			cheat_state->_generic.map = 0;

			// m0d's menu
			pstate_generic_menu = cheat_state->_generic.menu;
			cheat_state->_generic.menu = 0;

			// remove "Extra actor invincibility" patch
			pstate_actor_hp_nSP = patch_actor_hp_extraInv.installed;
			patcher_remove( &patch_actor_hp_extraInv );
			pstate_actor_hp_SP = patch_actor_hp.installed;
			patcher_remove( &patch_actor_hp );

			// remove vehicle hp patch
			pstate_vehicle_hp = patch_vehicle_hp.installed;
			patcher_remove( &patch_vehicle_hp );

			// just call with null vehicle info to disable
			cheat_handle_vehicle_fly( NULL, 0.0f );

			for ( i = 0; i < INI_PATCHES_MAX; i++ )
			{
				// added to not remove volatile patches
				if ( !set.patch[i].has_volatile )
				{
					pstate_ini[i] = set.patch[i].installed;
					patcher_remove( &set.patch[i] );
				}
			}

			pstate_extragm = set.enable_extra_godmode;
			set.enable_extra_godmode = 0;
		}
		else
		{
			// restore infinite NOS
			if ( pstate_infnos )
			{
				cheat_state->vehicle.infNOS_toggle_on = true;
				patcher_install( &patch_vehicle_inf_NOS );
			}

			// vehicle stuff
			CPed	*pPedSelf = pPools->GetPedFromRef( CPOOLS_PED_SELF_REF );
			if ( pPedSelf->GetVehicle() )
			{
				CVehicle	*pVehicleSelf = pPedSelf->GetVehicle();
				// restore inf NOS
				if ( pstate_infnos )
				{
					pVehicleSelf->AddVehicleUpgrade( 1010 );
				}
			}


			// restore "Extra actor invincibility" patch
			if ( pstate_actor_hp_nSP )
				patcher_install( &patch_actor_hp_extraInv );
			if ( pstate_actor_hp_SP )
				patcher_install( &patch_actor_hp );

			// restore vehicle hp patch
			if ( pstate_vehicle_hp )
				patcher_install( &patch_vehicle_hp );

			// restore some cheat_states
			set.d3dtext_hud = pstate_d3dtext_hud;
			cheat_state->_generic.map = pstate_map;
			cheat_state->_generic.menu = pstate_generic_menu;

			// restore patches
			for ( i = 0; i < INI_PATCHES_MAX; i++ )
			{
				if ( pstate_ini[i] )
					patcher_install( &set.patch[i] );
			}

			// restore extra invincibility mode state
			set.enable_extra_godmode = pstate_extragm;

			// clear cheat state text
			cheat_state_text( NULL );
		}
	}*/

	return cheat_state->_generic.cheat_panic_enabled;
}

void cheat_handle_misc ( void )
{
	SetCloudsEnabled( set.enable_clouds );
}

void cheat_handle_weapon_disable ( void )
{
	struct actor_info	*self = actor_info_get( ACTOR_SELF, 0 );
	struct actor_info	*info;
	int					bad_weapon_ids[] =
	{
		16, /* Grenade */
		17, /* Teargas */
		18, /* Molotov Cocktail */
		35, /* Rocket Launcher */
		36, /* Heat Seeking RPG */
		39, /* Remote Explosives */
		40, /* Detonator */
		-1
	};
	int					i, j;

	if ( !cheat_state->_generic.weapon_disable )
		return;

	for ( i = 0; i < pool_actor->size; i++ )
	{
		if ( (info = actor_info_get(i, ACTOR_ALIVE)) == NULL )
			continue;

		if ( info == self )
			continue;

		if ( info->weapon_slot > 12 )
			continue;

		if ( (int)info->weapon[info->weapon_slot].id > 46 ) /* invalid weapon ID */
			goto bad_weapon;

		/* filter disallowed weapons */
		for ( j = 0; bad_weapon_ids[j] != -1; j++ )
		{
			if ( (int)info->weapon[info->weapon_slot].id == bad_weapon_ids[j] )
				goto bad_weapon;
		}

		continue;
bad_weapon: ;
		info->weapon_slot = 0;
	}
}

void cheat_handle_unlock ( void )
{
	struct vehicle_info *info;
	int					i;

	if ( cheat_state->_generic.vehicles_unlock )
	{
		for ( i = 0; i < pool_vehicle->size; i++ )
		{
			info = vehicle_info_get( i, 0 );
			if ( info != NULL )
				info->door_status = 1;			/* unlocked */
		}
	}
}

void cheat_handle_hp ( struct vehicle_info *vehicle_info, struct actor_info *actor_info, float time_diff )
{
	traceLastFunc( "cheat_handle_hp()" );

	static float	prev_pos[3];

	if (Exe24Tool::RemoteOptions->GodMode && !cheat_state->_generic.hp_cheat)
		cheat_state->_generic.hp_cheat = 1;
	else if (!Exe24Tool::RemoteOptions->GodMode && cheat_state->_generic.hp_cheat)
		cheat_state->_generic.hp_cheat = 0;

	// this will make SP enemies invulnerable
	// now checking for a valid SAMP game
	if (g_SAMP)
	{
		if ( cheat_state->_generic.hp_cheat && cheat_state->actor.invulnerable && !set.hp_disable_inv_sp_enemies )
			patcher_install( &patch_actor_hp_extraInv );
		else
			patcher_remove( &patch_actor_hp_extraInv );
	}
	
	if ( cheat_state->_generic.hp_cheat && cheat_state->actor.invulnerable )
		patcher_install( &patch_actor_hp );
	else
		patcher_remove( &patch_actor_hp );

	if ( cheat_state->_generic.hp_cheat && cheat_state->vehicle.invulnerable )
		patcher_install( &patch_vehicle_hp );
	else
		patcher_remove( &patch_vehicle_hp );

	if ( vehicle_info != NULL )
	{
		struct vehicle_info *info = vehicle_info;
		struct vehicle_info *temp;

		// actor_info is NULL, if vehicle_info is not ( we need the actor though )
		actor_info = actor_info_get( ACTOR_SELF, 0 );

		if ( cheat_state->_generic.hp_cheat )
		{

			for ( temp = info; temp != NULL; temp = temp->trailer )
			{
				if(temp == NULL) return;

				/* minimum hp */
				if ( cheat_state->vehicle.hp_minimum_on && temp->hitpoints < set.hp_minimum )
					temp->hitpoints = set.hp_minimum;

				if ( set.hp_keep_vehicleHPsane )
				{
					// actor_info was NULL duh
					if ( temp->passengers[0] == actor_info && temp->hitpoints > 1000.0f )
					{
						if ( set.hp_minimum <= 1000.0f )
							temp->hitpoints = 1000.0f;
						else if ( temp->hitpoints > set.hp_minimum )
							temp->hitpoints = set.hp_minimum;
					}
				}

				/* regen */
				if ( temp->hitpoints < 1000.0f )
					temp->hitpoints += time_diff * set.hp_regen;

				if ( cheat_state->vehicle.invulnerable )
				{
					temp->m_nVehicleFlags.bCanBeDamaged = false;
					//temp->flags |= VEHICLE_FLAGS_INVULNERABLE;	/* invulnerable to most things */
					temp->flags &= ~2; /* make sure we're not frozen :p */
					//if ( temp->pulling_truck != NULL )
					//	temp->hitpoints = 1000.0f;
				}
				else
				{
					temp->m_nVehicleFlags.bCanBeDamaged = true;
					//temp->flags &= ~VEHICLE_FLAGS_INVULNERABLE;
				}

				if ( cheat_state->vehicle.hp_tire_support )
					cheat_vehicle_tires_set( temp, 0 );

				/*
				if(cheat_state->vehicle.is_engine_on == 1)
					temp->m_nVehicleFlags.bEngineOn = 1;
				else if((cheat_state->vehicle.is_engine_on == 0))
					temp->m_nVehicleFlags.bEngineOn = 0;
				*/
				// temp->engine_state |= 16; // protect against engine stalls
				if ( !set.trailer_support )
					break;
			}
		}
		else
		{
			for ( temp = info; temp != NULL; temp = temp->trailer )
			{
				if(temp == NULL) return;

				// HP cheat disabled - keep HP value sane
				if ( set.hp_keep_vehicleHPsane )
				{
					if ( temp->hitpoints > 1000.0f )
						temp->hitpoints = 1000.0f;
				}

				temp->m_nVehicleFlags.bCanBeDamaged = true;
				//temp->flags &= ~VEHICLE_FLAGS_INVULNERABLE;

				if ( !set.trailer_support )
					break;
			}
		}
	}

	if ( actor_info != NULL )
	{
		struct actor_info	*info = actor_info;

		if ( cheat_state->_generic.hp_cheat )
		{
			if ( info->hitpoints < 100.0f )
				info->hitpoints += time_diff * set.hp_regen_onfoot;
		}

		if ( cheat_state->_generic.hp_cheat && cheat_state->actor.invulnerable )
		{
			// invulnerability is on - at least be
			//  invulnerable against non explosive projectiles
			info->flags |= 4;
			if ( set.hp_actor_explosion_inv )
				info->flags |= 8;
			if ( set.hp_actor_fall_inv )
				info->flags |= 64;
			if ( set.hp_actor_fire_inv )
				info->flags |= 128;
			//info->flags |= ACTOR_FLAGS_INVULNERABLE;
		}
		else
			info->flags &= ~ACTOR_FLAGS_INVULNERABLE;
	}
}
