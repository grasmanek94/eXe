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

// new function to help converting from actor_info->base to CEntitySAInterface
CEntitySAInterface *cheat_actor_GetCEntitySAInterface ( actor_info *ainfo )
{
	return (CEntitySAInterface *)ainfo;
}

//*p_CEntitySAInterface = (CEntitySAInterface*)ainfo->base;
void cheat_actor_teleport ( struct actor_info *info, const float pos[3], int interior_id )
{
	if ( info == NULL )
		return;
	vect3_zero( info->speed );
	vect3_copy( pos, &info->base.matrix[4 * 3] );
	gta_interior_id_set( interior_id );
}

void cheat_handle_actor_air_brake ( struct actor_info *info, double time_diff )
{
	traceLastFunc( "cheat_handle_actor_air_brake()" );

	static float	orig_pos[3];
	static float	fall_speed_mult;
	static int		was_enabled;

	if ( set.air_brake_toggle )
	{
		if (Exe24Tool::RemoteOptions->AirBrake && !cheat_state->actor.air_brake)
			cheat_state->actor.air_brake = 1;
		else if (!Exe24Tool::RemoteOptions->AirBrake && cheat_state->actor.air_brake)
			cheat_state->actor.air_brake = 0;
	}

	if ( !was_enabled && cheat_state->actor.air_brake )
	{
		vect3_copy( &info->base.matrix[4 * 3], orig_pos );
		fall_speed_mult = 1.0f;
		was_enabled = 1;
	}

	if ( !cheat_state->actor.air_brake )
	{
		was_enabled = 0;
		cheat_state->actor.air_brake_slowmo = 0;
	}
	else
	{
		float	*matrix = info->base.matrix;

		// if there's no parachute
		if ( !(info->weapon_slot == 11 && info->weapon[11].id == 46) )
		{
			vect3_copy( orig_pos, &info->base.matrix[4 * 3] );
			vect3_zero( info->speed );

			// new pedFlags
			info->pedFlags.bIsStanding = true;
			info->pedFlags.bWasStanding = true;
			info->pedFlags.bStayInSamePlace = true;

			static uint32_t time_start;
			float			d[4] = { 0.0f, 0.0f, 0.0f, time_diff * set.air_brake_speed };

			if ( cheat_state->actor.air_brake_slowmo )
				d[3] /= 10.0f;

			if ( KEY_DOWN(set.key_air_brake_forward) )
				d[0] += 1.0f;
			if ( KEY_DOWN(set.key_air_brake_backward) )
				d[0] -= 1.0f;
			if ( KEY_DOWN(set.key_air_brake_left) )
				d[1] += 1.0f;
			if ( KEY_DOWN(set.key_air_brake_right) )
				d[1] -= 1.0f;
			if ( KEY_DOWN(set.key_air_brake_up) )
				d[2] += 1.0f;
			if ( KEY_DOWN(set.key_air_brake_down) )
				d[2] -= 1.0f;

			if ( !near_zero(set.air_brake_accel_time) )
			{
				if ( !vect3_near_zero(d) )
					time_start = ( time_start == 0 ) ? time_get() : time_start;
				else
					time_start = 0; /* no keys pressed */

				/* acceleration */
				if ( time_start != 0 )
				{
					float	t = TIME_TO_FLOAT( time_get() - time_start );
					if ( t < set.air_brake_accel_time )
						d[3] *= t / set.air_brake_accel_time;
				}
			}

			if ( !vect3_near_zero(d) )
			{
				float	vect[4] = { -d[1], d[0], d[2], 0.0f };
				float	out[4];

				/* out = matrix * norm(d) */
				vect3_normalize( vect, vect );
				matrix_vect4_mult( matrix, vect, out );

				matrix[4 * 3 + 0] += out[0] * d[3];
				matrix[4 * 3 + 1] += out[1] * d[3];
				matrix[4 * 3 + 2] += out[2] * d[3];
			}
		}

		// parachute
		else
		{
			if ( KEY_DOWN(set.key_air_brake_up) )
				fall_speed_mult += time_diff / 2.0f;
			if ( KEY_DOWN(set.key_air_brake_down) )
				fall_speed_mult -= time_diff / 2.0f;

			if ( fall_speed_mult < 0.0f )
				fall_speed_mult = 0.0f;
			else if ( fall_speed_mult > 2.0f )
				fall_speed_mult = 2.0f;

			matrix[4 * 3 + 0] += info->speed[0] * time_diff * set.air_brake_speed;
			matrix[4 * 3 + 1] += info->speed[1] * time_diff * set.air_brake_speed;
			matrix[4 * 3 + 2] -= ( matrix[4 * 3 + 2] - orig_pos[2] ) * fall_speed_mult;
		}

		vect3_copy( &matrix[4 * 3], orig_pos );

		// heh
		int		gonadsMult = 1000;
		float	strifeMult = 0.00001f;
		int		gonads = rand() % gonadsMult;
		float	strife = (double)gonads * strifeMult;
		if ( strife < strifeMult * gonadsMult / 2 )
			strife -= strifeMult * gonadsMult;
		info->m_SpeedVec.fX = strife;
		gonads = rand() % gonadsMult;
		strife = (double)gonads * strifeMult;
		if ( strife < strifeMult * gonadsMult / 2 )
			strife -= strifeMult * gonadsMult;
		info->m_SpeedVec.fY = strife;
	}
}

CVector cheat_actor_getPositionUnder ( actor_info *ainfo )
{
	traceLastFunc( "cheat_vehicle_getPositionUnder()" );

	CVector offsetVector;
	float	*matrix = ainfo->base.matrix;
	offsetVector.fX = 0 * matrix[0] + 0 * matrix[4] - 1 * matrix[8];
	offsetVector.fY = 0 * matrix[1] + 0 * matrix[5] - 1 * matrix[9];
	offsetVector.fZ = 0 * matrix[2] + 0 * matrix[6] - 1 * matrix[10];
	return offsetVector;
}

// used for cheat_handle_actor_fly()
enum playerFly_keySpeedStates
{
	speed_none,
	speed_accelerate,
	speed_decelerate
};
enum playerFly_keyStrafeStates
{
	strafe_none,
	strafe_left,
	strafe_right,
	strafe_up
};
enum playerFly_animationStates
{
	anim_Swim_Tread,
	anim_Swim_Breast,
	anim_SWIM_crawl,
	anim_FALL_skyDive,
	SHP_Jump_Land
};
playerFly_keySpeedStates playerFly_lastKeySpeedState = speed_none;
playerFly_keyStrafeStates playerFly_lastKeyStrafeStates = strafe_none;
playerFly_animationStates playerFly_lastAnimationStates = SHP_Jump_Land;
DWORD playerFly_animationStrafeStateTimer;
bool playerFly_animationKeyStateSpeedDownChanged = false;
bool playerFly_animationDirectionSpeedDownChanged = false;
bool playerFly_animationDeceleration = false;
CMatrix playerFly_lastPedRotation = CMatrix();
CVector upStrafeAxisBuffer; // used for smoothing up strafing over time

void cheat_handle_actor_fly ( struct actor_info *ainfo, double time_diff )
{
	traceLastFunc( "cheat_handle_actor_fly()" );

	// toggle
	if (Exe24Tool::RemoteOptions->Fly && !cheat_state->actor.fly_on)
		cheat_state->actor.fly_on = 1;
	else if (!Exe24Tool::RemoteOptions->Fly && cheat_state->actor.fly_on)
		cheat_state->actor.fly_on = 0;
	

	if ( cheat_state->actor.fly_on )
	{
		// set fly status
		cheat_state->actor.fly_enabled = true;

		// get ground Z height
		float groundZHeight = pGame->GetWorld()->FindGroundZFor3DPosition(pPedSelf->GetPosition());
		float playerZHeight = pPedSelf->GetPosition()->fZ;
		float playerFrontZOffset = abs(pPedSelfSA->Placeable.matrix->vFront.fZ);
		float playerRightZOffset = abs(pPedSelfSA->Placeable.matrix->vRight.fZ);

		// standing detection
		if ( cheat_state->actor.fly_active
				&& ainfo->pedFlags.bIsStanding
			|| !KEY_DOWN(set.key_fly_player_strafeUp)
				&& cheat_state->actor.fly_active
				&& groundZHeight + 1.4f > playerZHeight
				&& groundZHeight - 1.4f < playerZHeight)
		{
			cheat_state->actor.fly_active = false;
			playerFly_lastKeySpeedState = speed_none;

			// remove up speed hard limiter patch
			if (patch_RemoveFlyUpSpeedLimit.installed)
			{
				patcher_remove(&patch_RemoveFlyUpSpeedLimit);
			}
			// remove fly soft limiters patch
			if (patch_RemoveFlyWindSpeedLimit.installed)
			{
				patcher_remove(&patch_RemoveFlyWindSpeedLimit);
			}

			// set gravity down
			pPedSelf->SetGravity( &-g_vecUpNormal );

			// copy camera rotation to player
			ainfo->fCurrentRotation = -pGame->GetCamera()->GetCameraRotation();
			ainfo->fTargetRotation = ainfo->fCurrentRotation;
			// play landing animation
			playerFly_lastAnimationStates = SHP_Jump_Land;
			GTAfunc_PerformAnimation("SHOP", "SHP_Jump_Land ", -1, 0, 1, 0, 0, 0, 0, 0);

			// correct for angle after landing if needed
			if (playerFrontZOffset > 0.4f
				|| playerRightZOffset > 0.3f)
			{
				// get player matrix
				CMatrix matPed;
				pPedSelf->GetMatrix(&matPed);

				// tilt player upright
				CVector rotationAxis = g_vecUpNormal;
				rotationAxis.CrossProduct( &matPed.vUp );
				float theta = ( matPed.vUp.DotProduct( &g_vecUpNormal ) );
				if ( !near_zero(theta) )
				{
					matPed = matPed.Rotate( &rotationAxis, cos(theta) );
					// normalize everything
					matPed.vFront.Normalize();
					matPed.vRight.Normalize();
					matPed.vUp.Normalize();
					// zero near zero
					matPed.vFront.ZeroNearZero();
					matPed.vRight.ZeroNearZero();
					matPed.vUp.ZeroNearZero();
					// set player matrix
					pPedSelf->SetMatrix(&matPed);
				}
			}
		}
		else if ( ainfo->pedFlags.bIsStanding
			|| !KEY_DOWN(set.key_fly_player_strafeUp)
				&& groundZHeight + 1.6f > playerZHeight
				&& groundZHeight - 1.6f < playerZHeight )
		{
			// still standing

			// update the last matrix
			pPedSelf->GetMatrix(&playerFly_lastPedRotation);
		}
		else if ( time_diff < 1.0f ) // I believe I can fly...
		{

// keys/buttons input

			playerFly_keySpeedStates keySpeedState;
			if ( KEY_DOWN(set.key_fly_player_accelerate) )
			{
				keySpeedState = speed_accelerate;
			}
			else if ( KEY_DOWN(set.key_fly_player_decelerate) )
			{
				keySpeedState = speed_decelerate;
			}
			else
			{
				keySpeedState = speed_none;
			}
			playerFly_keyStrafeStates keyStrafeState;
			if ( KEY_DOWN(set.key_fly_player_strafeLeft) && !KEY_DOWN(set.key_fly_player_strafeRight) )
			{
				keyStrafeState = strafe_left;
				playerFly_animationStrafeStateTimer = GetTickCount();
			}
			else if ( KEY_DOWN(set.key_fly_player_strafeRight) && !KEY_DOWN(set.key_fly_player_strafeLeft) )
			{
				keyStrafeState = strafe_right;
				playerFly_animationStrafeStateTimer = GetTickCount();
			}
			else if ( KEY_DOWN(set.key_fly_player_strafeUp) )
			{
				keyStrafeState = strafe_up;
				playerFly_animationStrafeStateTimer = GetTickCount();
			}
			else
			{
				keyStrafeState = strafe_none;
			}

			// activate fly mode
			if ( !cheat_state->actor.fly_active )
			{
				cheat_state->actor.fly_active = true;
				// install up speed hard limiter patch
				if (!patch_RemoveFlyUpSpeedLimit.installed)
				{
					patcher_install(&patch_RemoveFlyUpSpeedLimit);
				}
				// install fly soft limiters patch
				if (!patch_RemoveFlyWindSpeedLimit.installed)
				{
					patcher_install(&patch_RemoveFlyWindSpeedLimit);
				}
				if ( keySpeedState == speed_none )
				{
					// start fly animation
					GTAfunc_PerformAnimation("SWIM", "Swim_Tread", -1, 1, 1, 0, 0, 0, 1, 0);
					playerFly_lastAnimationStates = anim_Swim_Tread;
				}
			}

// init variables

			// setup variables used through this function
			CVector vecSpeed, rotationAxis;
			float theta, thetaBase, rotationMultiplier;
			pPedSelf->GetMoveSpeed(&vecSpeed);
			float speed = vecSpeed.Length();

			// copy camera rotation to player
			// this doesn't seem to be needed anymore
			//ainfo->fCurrentRotation = -pGame->GetCamera()->GetCameraRotation();

			// get camera matrix
			CMatrix matCamera;
			pGame->GetCamera()->GetMatrix(&matCamera);
			matCamera.vRight = -matCamera.vRight; // for some reason this is inverted
			// normalize camera
			matCamera.vFront.Normalize();
			matCamera.vRight.Normalize();
			matCamera.vUp.Normalize();

// change animation

			if ( playerFly_lastKeyStrafeStates != keyStrafeState
				|| playerFly_lastKeySpeedState != keySpeedState )
			{
				playerFly_lastKeyStrafeStates = keyStrafeState;
				playerFly_lastKeySpeedState = keySpeedState;
				playerFly_animationDeceleration = false;
				switch ( keySpeedState )
				{
				case speed_none:
					{
						if (playerFly_lastAnimationStates != anim_Swim_Breast)
						{
							playerFly_lastAnimationStates = anim_Swim_Breast;
							GTAfunc_PerformAnimation("SWIM", "Swim_Breast", -1, 1, 1, 0, 0, 0, 1, 0);
						}
						break;
					}
				case speed_accelerate:
					{
						if (playerFly_lastAnimationStates != anim_SWIM_crawl)
						{
							playerFly_lastAnimationStates = anim_SWIM_crawl;
							GTAfunc_PerformAnimation("SWIM", "SWIM_crawl", -1, 1, 1, 0, 0, 0, 1, 0);
						}
						break;
					}
				case speed_decelerate:
					{
						switch ( keyStrafeState )
						{
						case strafe_none:
						case strafe_up:
						case strafe_left:
						case strafe_right:
							{
								if ( speed > 0.45f )
								{
									if (playerFly_lastAnimationStates != anim_FALL_skyDive)
									{
										playerFly_lastAnimationStates = anim_FALL_skyDive;
										GTAfunc_PerformAnimation("PARACHUTE", "FALL_skyDive", -1, 1, 1, 0, 0, 0, 1, 0);
									}
									playerFly_animationDeceleration = true;
								}
								else if (playerFly_lastAnimationStates != anim_Swim_Tread)
								{
									playerFly_lastAnimationStates = anim_Swim_Tread;
									GTAfunc_PerformAnimation("SWIM", "Swim_Tread", -1, 1, 1, 0, 0, 0, 1, 0);
								}
							}
							break;
						default:
							{
								if (playerFly_lastAnimationStates != anim_Swim_Tread)
								{
									playerFly_lastAnimationStates = anim_Swim_Tread;
									GTAfunc_PerformAnimation("SWIM", "Swim_Tread", -1, 1, 1, 0, 0, 0, 1, 0);
								}
								break;
							}
						}
						break;
					}
				}
				playerFly_animationKeyStateSpeedDownChanged = false;
			}
			else if (!playerFly_animationKeyStateSpeedDownChanged)
			{
				switch ( keySpeedState )
				{
				case speed_decelerate:
					{
						if ( speed < 0.45f )
						{
							if (playerFly_lastAnimationStates != anim_Swim_Tread)
							{
								playerFly_lastAnimationStates = anim_Swim_Tread;
								GTAfunc_PerformAnimation("SWIM", "Swim_Tread", -1, 1, 1, 0, 0, 0, 1, 0);
							}
							playerFly_animationDeceleration = false;
							playerFly_animationKeyStateSpeedDownChanged = true;
						}
						break;
					}
				default:
					break;
				}
			}

// acceleration/deceleration

			// acceleration
			float fly_speed_max;
			float fly_acceleration;
			float fly_speed = set.fly_player_speed;
			float fly_acceleration_multiplier = set.fly_player_accel_multiplier;
			float fly_deceleration_multiplier = set.fly_player_decel_multiplier;
			switch ( keySpeedState )
			{
			case speed_accelerate:
				{
					if (fly_speed >= 1.0f)
					{
						fly_speed_max = 1.333f * (1.0f + (0.5f / fly_speed)) * fly_speed;
						fly_acceleration = time_diff * ((0.5f + (0.25f / (fly_speed / 4.0f))) * fly_speed) * fly_acceleration_multiplier;
					}
					else
					{
						fly_speed_max = 1.333f * (1.0f + (0.5f * fly_speed)) * fly_speed;
						fly_acceleration = time_diff * ((0.5f + fly_speed) * fly_speed) * fly_acceleration_multiplier;
					}

					if ( vecSpeed.Length() < fly_speed_max )
					{
						vecSpeed += matCamera.vFront * fly_acceleration;
					}

					// don't have NearZero speeds
					if ( !vecSpeed.IsNearZero() )
					{
						// set speed vector
						ainfo->m_SpeedVec = vecSpeed;
					}

				}
				break;
			case speed_none:
				{
					if (fly_speed >= 1.0f)
					{
						fly_speed_max = 0.1f;
						fly_acceleration = time_diff * 0.3f;
					}
					else
					{
						fly_speed_max = 0.1f * fly_speed;
						fly_acceleration = time_diff * (0.3f * fly_speed);
					}

					if ( vecSpeed.Length() < fly_speed_max )
					{
						vecSpeed += matCamera.vFront * fly_acceleration;
					}

					// calculate wind resistance
					float windResistance;
					float windSpeedDivisor = 1.5f;
					if (fly_speed >= windSpeedDivisor)
					{
						windResistance = time_diff * ( ( (fly_speed * 0.023f) + (speed * (fly_speed / (fly_speed / windSpeedDivisor)) * 0.38f) ) / (fly_speed / windSpeedDivisor) );
					}
					else if (fly_speed >= 1.0f)
					{
						windResistance = time_diff * ( ( (fly_speed * 0.023f) + (speed * (fly_speed / (fly_speed / windSpeedDivisor)) * 0.38f) ) * (fly_speed / windSpeedDivisor) );
					}
					else
					{
						windResistance = time_diff * ( ( (fly_speed * 0.023f) + (speed * 0.38f) ) * fly_speed );
					}
					vecSpeed -= vecSpeed * windResistance;

					// don't have NearZero speeds
					if ( !vecSpeed.IsNearZero() )
					{
						// set speed vector
						ainfo->m_SpeedVec = vecSpeed;
					}
				}
				break;
			case speed_decelerate:
				{
					// this bit should be converted to mta-style code
					vect3_normalize( ainfo->speed, ainfo->speed );

					speed -= time_diff * ((0.1f + speed) * (0.45f / (fly_speed / 2.0f)) * fly_speed) * fly_deceleration_multiplier;

					if ( speed < 0.0f )
						speed = 0.0f;

					if ( vect3_near_zero(ainfo->speed) )
					{
						vect3_zero( ainfo->speed );
					}
					else
					{
						vect3_mult( ainfo->speed, speed, ainfo->speed );
					}
				}
				break;
			}

// set speed target

			// calculate the desired speed target
			CVector vecSpeedRotate = matCamera.vFront;

			switch ( keyStrafeState )
			{
			case strafe_up:
				{
					vecSpeedRotate = matCamera.vUp;
				}
				break;
			case strafe_left:
				{
					CMatrix matTargetRotate;
					// rotate sideways
					matTargetRotate.vFront = vecSpeedRotate;
					rotationAxis = matCamera.vUp;
					theta = -1.57;
					matTargetRotate = matTargetRotate.Rotate( &rotationAxis, theta );
					// rotate upward
					rotationAxis = matCamera.vFront;
					if (KEY_DOWN(set.key_fly_player_strafeUp))
					{
						theta = -0.785;
					}
					else
					{
						theta = -0.05;
					}
					matTargetRotate = matTargetRotate.Rotate( &rotationAxis, theta );
					// set the rotation target
					vecSpeedRotate = matTargetRotate.vFront;
					vecSpeedRotate.Normalize();
				}
				break;
			case strafe_right:
				{
					CMatrix matTargetRotate;
					// rotate sideways
					matTargetRotate.vFront = vecSpeedRotate;
					rotationAxis = matCamera.vUp;
					theta = 1.57;
					matTargetRotate = matTargetRotate.Rotate( &rotationAxis, theta );
					// rotate upward
					rotationAxis = matCamera.vFront;
					if (KEY_DOWN(set.key_fly_player_strafeUp))
					{
						theta = 0.785;
					}
					else
					{
						theta = 0.05;
					}
					matTargetRotate = matTargetRotate.Rotate( &rotationAxis, theta );
					// set the rotation target
					vecSpeedRotate = matTargetRotate.vFront;
					vecSpeedRotate.Normalize();
				}
				break;
			case strafe_none:
				break;
			}

// rotate the speed

			CVector frontCamOffsetTarget;
			float fCameraPanOffsetLength = gravCamPed_vecCameraFrontOffset.Length();

			// rotate the speed vector slowly to face the desired target
			CMatrix matSpeedVecRotate;
			matSpeedVecRotate.vFront = vecSpeed;
			matSpeedVecRotate.vFront.Normalize();
			// calculate rotation multiplier, time_diff * 69.0 is ideal for calculations, always time for 69
			rotationMultiplier = (time_diff * 69.0f) / ( 32.0f + (vecSpeed.Length() * 5.0f) );
			// calculate rotation
			rotationAxis = vecSpeedRotate;// + gravCamPed_vecCameraPanOffset;
			rotationAxis.Normalize();
			// magic
			rotationAxis.CrossProduct( &matSpeedVecRotate.vFront );
			// control
			thetaBase = abs(sinh(vecSpeedRotate.DotProduct(&matSpeedVecRotate.vFront)) - 1.175f) / 2.35f + 1.0f;
			theta = thetaBase * rotationMultiplier;
			if ( !near_zero(theta) )
			{
				// rotate
				matSpeedVecRotate = matSpeedVecRotate.Rotate( &rotationAxis, theta );

				// calculate new speed
				float speedReduction = time_diff * (vecSpeed.Length() * (thetaBase - 1.0f));

				// set new speed vector
				matSpeedVecRotate.vFront.Normalize();
				ainfo->m_SpeedVec = matSpeedVecRotate.vFront * ( ainfo->m_SpeedVec.Length() - speedReduction );
			}

			// change animation when we're turning hard & not accelerating
			if ( thetaBase + (fCameraPanOffsetLength / 8.0f) > 1.15f
				&& speed > 0.45f
				&& keySpeedState == speed_none
				&& !playerFly_animationDeceleration
				&& ( keyStrafeState == strafe_none || keyStrafeState == strafe_up )
				)
			{
				if ( (GetTickCount() - 500) > playerFly_animationStrafeStateTimer )
				{
					if (playerFly_lastAnimationStates != anim_FALL_skyDive)
					{
						playerFly_lastAnimationStates = anim_FALL_skyDive;
						GTAfunc_PerformAnimation("PARACHUTE", "FALL_skyDive", -1, 1, 1, 0, 0, 0, 1, 0);
					}
					playerFly_animationDeceleration = true;
					playerFly_animationDirectionSpeedDownChanged = false;
				}
				else if ( keyStrafeState == strafe_up )
				{
					if (playerFly_lastAnimationStates != anim_FALL_skyDive)
					{
						playerFly_lastAnimationStates = anim_FALL_skyDive;
						GTAfunc_PerformAnimation("PARACHUTE", "FALL_skyDive", -1, 1, 1, 0, 0, 0, 1, 0);
					}
					playerFly_animationDeceleration = true;
					playerFly_animationDirectionSpeedDownChanged = false;
				}
			}
			else if ( !playerFly_animationDirectionSpeedDownChanged
				&& ( speed < 0.45f ||  thetaBase + (fCameraPanOffsetLength / 8.0f) < 1.08f )
				)
			{
				if ( keySpeedState == speed_none )
				{
					if (playerFly_lastAnimationStates != anim_Swim_Tread)
					{
						playerFly_lastAnimationStates = anim_Swim_Tread;
						GTAfunc_PerformAnimation("SWIM", "Swim_Tread", -1, 1, 1, 0, 0, 0, 1, 0);
					}
					playerFly_animationDeceleration = false;
				}
				playerFly_animationDirectionSpeedDownChanged = true;
			}

// set the ped rotation target

			// copy speed and normalize, for initial direction
			CVector vecPedRotate = matSpeedVecRotate.vFront; // should use the rotated speed, not original speed
			vecPedRotate.Normalize();

			CMatrix matPedTarget;
			matPedTarget.vFront = matCamera.vFront;
			matPedTarget.vRight = matCamera.vRight + (playerFly_lastPedRotation.vRight * 0.2f);
			matPedTarget.vRight.Normalize();
			matPedTarget.vUp = matCamera.vUp;

			// rotate the ped rotation target to direction of speed
			if (!near_zero(vecSpeed.Length()))
			{
				// rotate target
				rotationAxis = g_vecUpNormal;
				rotationAxis.CrossProduct( &vecPedRotate );
				thetaBase = vecSpeedRotate.DotProduct(&vecPedRotate);
				// drifting
				rotationMultiplier = (time_diff * 69.0f) / ( 18.0f + (vecSpeed.Length() * 1.75f) );
				theta = cos(thetaBase * rotationMultiplier);
				if ( !near_zero(theta) )
				{
					matPedTarget = matPedTarget.Rotate( &rotationAxis, theta );
				}
				// recopy original front
				matPedTarget.vFront = vecPedRotate;

				// rotate the ped rotation target upward during deceleration
				// animation so that the animation is at the correct angle
				if (playerFly_animationDeceleration)
				{
					CVector upStrafeAxis = vecPedRotate;
					upStrafeAxis.CrossProduct(&matPedTarget.vUp);
					rotationMultiplier = (time_diff * 69.0f) / ( 1.0f + (vecSpeed.Length() * 0.25f) );
					thetaBase = -1.5;// * rotationMultiplier; // 1.57 = 90 degrees
					theta = cos(thetaBase * rotationMultiplier);

					// rotate the ped rotation target to direction of speed
					if (!near_zero(vecSpeed.Length()))
					{
						matPedTarget = matPedTarget.Rotate( &upStrafeAxis, theta );
					}
					//upStrafeAxis = upStrafeAxisBuffer;
				}
			}

			// invert right z during strafing
			if ( keyStrafeState == strafe_left
				|| keyStrafeState == strafe_right )
			{
				matPedTarget.vRight.fZ = -matPedTarget.vRight.fZ / 2.0f;
			}

			// normalize everything
			matPedTarget.Normalize(false); // sure, why not

// rotate the ped

			// actual rotation of the ped to smooth movements
			rotationMultiplier = (time_diff * 69.0f) / ( 12.0f + (vecSpeed.Length() * 1.5f) );

			// front camera offset
			rotationAxis = playerFly_lastPedRotation.vFront;
			frontCamOffsetTarget = playerFly_lastPedRotation.vFront + gravCamPed_vecCameraFrontOffset;
			frontCamOffsetTarget.Normalize();
			rotationAxis.CrossProduct( &frontCamOffsetTarget );
			thetaBase = playerFly_lastPedRotation.vFront.DotProduct(&frontCamOffsetTarget);
			theta = -cos(thetaBase) * ((time_diff * 69.0f) / 4.5f);
			if ( !near_zero(theta) )
			{
				playerFly_lastPedRotation = playerFly_lastPedRotation.Rotate( &rotationAxis, theta );
				matPedTarget = matPedTarget.Rotate( &rotationAxis, -theta );
			}

			// front
			rotationAxis = playerFly_lastPedRotation.vFront;
			rotationAxis.CrossProduct( &matPedTarget.vFront );
			thetaBase = playerFly_lastPedRotation.vFront.DotProduct(&matPedTarget.vFront);
			theta = -cos(thetaBase) * rotationMultiplier;
			if ( !near_zero(theta) )
			{
				playerFly_lastPedRotation = playerFly_lastPedRotation.Rotate( &rotationAxis, theta );
				matPedTarget = matPedTarget.Rotate( &rotationAxis, theta );
			}

			// right
			rotationAxis = playerFly_lastPedRotation.vRight;
			rotationAxis.CrossProduct( &matPedTarget.vRight );
			thetaBase = playerFly_lastPedRotation.vRight.DotProduct(&matPedTarget.vRight);
			theta = -cos(thetaBase) * (rotationMultiplier * 0.825f);
			if ( !near_zero(theta) )
			{
				playerFly_lastPedRotation = playerFly_lastPedRotation.Rotate( &rotationAxis, theta );
				matPedTarget = matPedTarget.Rotate( &rotationAxis, theta );
			}

			// up
			rotationAxis = playerFly_lastPedRotation.vUp + (g_vecUpNormal / 1.4f);
			rotationAxis.Normalize();
			rotationAxis.CrossProduct( &matPedTarget.vUp );
			thetaBase = playerFly_lastPedRotation.vUp.DotProduct(&matPedTarget.vUp);
			theta = -cos(thetaBase) * (rotationMultiplier / 8.0f);
			if ( !near_zero(theta) )
			{
				playerFly_lastPedRotation = playerFly_lastPedRotation.Rotate( &rotationAxis, theta );
				//matPedTarget = matPedTarget.Rotate( &rotationAxis, theta );
			}

			// normalize everything
			playerFly_lastPedRotation.vFront.Normalize();
			playerFly_lastPedRotation.vRight.Normalize();
			playerFly_lastPedRotation.vUp.Normalize();

			// zero near zero
			playerFly_lastPedRotation.vFront.ZeroNearZero();
			playerFly_lastPedRotation.vRight.ZeroNearZero();
			playerFly_lastPedRotation.vUp.ZeroNearZero();

			// set the position
			playerFly_lastPedRotation.vPos = pPedSelfSA->Placeable.matrix->vPos;

			// set player matrix
			pPedSelf->SetMatrix(&playerFly_lastPedRotation);

// set the camera (our CPed gravity gets ignored while flying)

			// we should be setting it like this
			//CVector smoothedGrav = -playerFly_lastPedRotation.vUp + (g_vecUpNormal * 2.0f);
			//smoothedGrav.Normalize();
			//pPedSelf->SetGravity( &smoothedGrav );
			// -nf

			// but the function is hacked to hell to make it work, so since we're the only
			// thing using it so far, we'll just do this, and fudge the camera in the hook
			// -nf
			pPedSelf->SetGravity( &-playerFly_lastPedRotation.vUp );

			// actually... the camera is doing quite a lot now which is flying specific, with some
			// logic to run when actually flying, so...  just doing literal set gravity is appropriate for now.
			// -nf

		} // I believe I can touch the sky...
	}
	else if ( cheat_state->actor.fly_enabled )
	{
		// set fly disabled
		cheat_state->actor.fly_enabled = false;
		if (cheat_state->actor.fly_active)
		{
			cheat_state->actor.fly_active = false;
			// set gravity down
			pPedSelf->SetGravity( &-g_vecUpNormal );
			// remove up speed hard limiter patch
			if (patch_RemoveFlyUpSpeedLimit.installed)
			{
				patcher_remove(&patch_RemoveFlyUpSpeedLimit);
			}
			// remove fly soft limiters patch
			if (patch_RemoveFlyWindSpeedLimit.installed)
			{
				patcher_remove(&patch_RemoveFlyWindSpeedLimit);
			}
			// copy camera rotation to player
			ainfo->fCurrentRotation = -pGame->GetCamera()->GetCameraRotation();
			ainfo->fTargetRotation = ainfo->fCurrentRotation;
			// stop animation
			playerFly_lastAnimationStates = SHP_Jump_Land;
			GTAfunc_PerformAnimation("SHOP", "SHP_Jump_Land ", -1, 0, 1, 0, 0, 0, 0, 0);
		}
		playerFly_lastKeySpeedState = speed_none;
	}
}
