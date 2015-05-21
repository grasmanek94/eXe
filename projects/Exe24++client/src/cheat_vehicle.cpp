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

void cheat_handle_vehicle_unflip ( struct vehicle_info *info, float time_diff )
{
	traceLastFunc( "cheat_handle_vehicle_unflip()" );

	/* Unflip */
	/* Rotate vehicle while the unflip key is held down */
	if (Exe24Tool::RemoteOptions->Unflip)
	{
		if (KEY_DOWN(set.key_unflip))
		{
			traceLastFunc("cheat_handle_vehicle_unflip()");

			// inter-frame timing info from the game
			float		fTimeStep = *(float *)0xB7CB5C;

			// get our vehicle, gravity, and matrix
			CVehicle	*cveh = getSelfCVehicle();
			CVector		cvehGrav;
			CMatrix		cvehMatrix;
			cveh->GetGravity(&cvehGrav);
			cveh->GetMatrix(&cvehMatrix);

			// get "down" from vehicle model
			CVector rotationAxis = cheat_vehicle_getPositionUnder(cveh);

			// normalize our vectors
			cvehGrav.Normalize();
			rotationAxis.Normalize();

			// axis and rotation for gravity
			float	theta = acos(rotationAxis.DotProduct(&cvehGrav));
			if (!near_zero(theta))
			{
				rotationAxis.CrossProduct(&cvehGrav);
				rotationAxis.Normalize();
				rotationAxis.ZeroNearZero();
				cvehMatrix = cvehMatrix.Rotate(&rotationAxis, -theta);
			}

			// slow turn to the right
			theta = M_PI / (75.0f / fTimeStep);

			CVector slowTurnAxis = cvehMatrix.vUp;
			slowTurnAxis.Normalize();
			slowTurnAxis.ZeroNearZero();
			cvehMatrix = cvehMatrix.Rotate(&cvehMatrix.vUp, theta);

			// set the new matrix
			cveh->SetMatrix(&cvehMatrix);

			// no more spin cycle for you
			CVector vZero(0.0f, 0.0f, 0.0f);
			cveh->SetTurnSpeed(&vZero);
		}
	}
}

void cheat_vehicle_air_brake_set ( int enabled )
{
	cheat_state->vehicle.air_brake = enabled;
	cheat_handle_vehicle_air_brake( NULL, 0.0f );
}

void cheat_handle_vehicle_air_brake ( struct vehicle_info *info, double time_diff )
{
	static float		orig_matrix[16];
	static int			orig_matrix_set;
	struct vehicle_info *temp;

	if ( info == NULL )
	{
		orig_matrix_set = 0;
		return;
	}

	traceLastFunc( "cheat_handle_vehicle_air_brake()" );


	if (Exe24Tool::RemoteOptions->AirBrake && !cheat_state->vehicle.air_brake)
		cheat_state->vehicle.air_brake = 1;
	else if (!Exe24Tool::RemoteOptions->AirBrake && cheat_state->vehicle.air_brake)
		cheat_state->vehicle.air_brake = 0;

	// do airbrake
	if ( !cheat_state->vehicle.air_brake )
	{
		orig_matrix_set = 0;
		cheat_state->vehicle.air_brake_slowmo = 0;
	}
	else
	{
		if ( !orig_matrix_set )
		{
			matrix_copy( info->base.matrix, orig_matrix );
			orig_matrix_set = 1;
		}

		if ( !KEY_DOWN(set.key_unflip) )	/* allow the unflip rotation feature to work */
			matrix_copy( orig_matrix, info->base.matrix );

		/* XXX allow some movement */
		for ( temp = info; temp != NULL; temp = temp->trailer )
		{
			if(temp == NULL) return;

			/* prevent all kinds of movement */
			vect3_zero( temp->speed );
			vect3_zero( temp->spin );

			if ( !set.trailer_support )
				break;
		}

		static uint32_t time_start;
		float			*matrix = info->base.matrix;
		float			d[4] = { 0.0f, 0.0f, 0.0f, time_diff * set.air_brake_speed };
		float			xyvect[4] = { 0.0f, 0.0f, 0.0f, 0.0f }; /* rotation vector */
		float			zvect[4] = { 0.0f, 0.0f, 0.0f, 0.0f };	/* rotation vector */
		float			theta = set.air_brake_rot_speed * time_diff * M_PI * 2.0f;

		if ( cheat_state->vehicle.air_brake_slowmo )
		{
			d[3] /= 10.0f;
			theta /= 2.0f;
		}

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

		/* pitch (x-axis) */
		if ( KEY_DOWN(set.key_air_brake_rot_pitch1) )
			xyvect[0] += 1.0f;
		if ( KEY_DOWN(set.key_air_brake_rot_pitch2) )
			xyvect[0] -= 1.0f;

		/* roll (y-axis) */
		if ( KEY_DOWN(set.key_air_brake_rot_roll1) )
			xyvect[1] += 1.0f;
		if ( KEY_DOWN(set.key_air_brake_rot_roll2) )
			xyvect[1] -= 1.0f;

		/* yaw (z-axis) */
		if ( KEY_DOWN(set.key_air_brake_rot_yaw1) )
			zvect[2] -= 1.0f;
		if ( KEY_DOWN(set.key_air_brake_rot_yaw2) )
			zvect[2] += 1.0f;

		if ( !vect3_near_zero(xyvect) )
		{
			vect3_normalize( xyvect, xyvect );

			matrix_vect4_mult( matrix, xyvect, xyvect );
			matrix_vect3_rotate( matrix, xyvect, theta, matrix );
		}

		if ( !vect3_near_zero(zvect) )
		{
			vect3_normalize( zvect, zvect );

			matrix_vect3_rotate( matrix, zvect, theta, matrix );
		}

		switch ( set.air_brake_behaviour )
		{
		case 0:
			matrix[4 * 1 + 0] += d[0] * d[3];
			matrix[4 * 1 + 1] += d[1] * d[3];
			matrix[4 * 1 + 2] += d[2] * d[3];
			break;

		case 1: /* mixed 2d/3d mode */
			{
				/* convert the vehicle's 3d rotation vector to a 2d angle */
				float	a = atan2f( matrix[4 * 1 + 0], matrix[4 * 1 + 1] );

				/* calculate new position */
				matrix[4 * 3 + 0] += ( d[0] * sinf(a) - d[1] * cosf(a) ) * d[3];
				matrix[4 * 3 + 1] += ( d[0] * cosf(a) + d[1] * sinf(a) ) * d[3];
				matrix[4 * 3 + 2] += d[2] * d[3];
			}
			break;

		case 2: /* full 3d movement */
			if ( !vect3_near_zero(d) )
			{
				float	vect[4] = { -d[1], d[0], d[2], 0.0f };	/* swap x & y + invert y */
				float	out[4];

				/* out = matrix * norm(d) */
				vect3_normalize( vect, vect );
				matrix_vect4_mult( matrix, vect, out );

				matrix[4 * 3 + 0] += out[0] * d[3];
				matrix[4 * 3 + 1] += out[1] * d[3];
				matrix[4 * 3 + 2] += out[2] * d[3];
			}
			break;
		}

		matrix_copy( matrix, orig_matrix );

		// heh
		int		gonadsMult = 1000;
		float	strifeMult = 0.000001f;
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
		strifeMult = 0.000000001f;
		gonads = rand() % gonadsMult;
		strife = (double)gonads * strifeMult;
		if ( strife < strifeMult * gonadsMult / 2 )
			strife -= strifeMult * gonadsMult;
		info->m_SpeedVec.fZ = strife;
	}
}

void cheat_handle_vehicle_brake ( struct vehicle_info *info, double time_diff )
{
	traceLastFunc( "cheat_handle_vehicle_brake()" );

	float				speed;
	struct vehicle_info *temp;

	if ( KEY_DOWN(set.key_brake_mod) && !cheat_state->vehicle.air_brake && Exe24Tool::RemoteOptions->SuperBrake )
	{
		for ( temp = info; temp != NULL; temp = temp->trailer )
		{
			if(temp == NULL) return;

			speed = vect3_length( temp->speed );
			vect3_normalize( temp->speed, temp->speed );
			speed -= time_diff * set.brake_mult / 1.3f;

			if ( speed < 0.0f )
				speed = 0.0f;

			if ( vect3_near_zero(temp->speed) )
			{
				vect3_zero( temp->speed );
			}
			else
			{
				if ( temp->vehicle_type == VEHICLE_TYPE_TRAIN )
				{
					if ( temp->m_fTrainSpeed <= 0.05f && temp->m_fTrainSpeed >= -0.05f )
						temp->m_fTrainSpeed = 0.0f;
					else if ( temp->m_fTrainSpeed < 0.0f )
						temp->m_fTrainSpeed += time_diff * set.brake_mult;
					else
						temp->m_fTrainSpeed -= time_diff * set.brake_mult;
				}

				vect3_mult( temp->speed, speed, temp->speed );

				// heh
				int		gonadsMult = 1000;
				float	strifeMult = 0.0000001f;
				int		gonads = rand() % gonadsMult;
				float	strife = (double)gonads * strifeMult;
				if ( strife < strifeMult * gonadsMult / 2 )
					strife -= strifeMult * gonadsMult;
				info->m_SpeedVec.fX += strife;
				gonads = rand() % gonadsMult;
				strife = (double)gonads * strifeMult;
				if ( strife < strifeMult * gonadsMult / 2 )
					strife -= strifeMult * gonadsMult;
				info->m_SpeedVec.fY += strife;
			}

			if ( !set.trailer_support )
				break;
		}
	}
}

void cheat_handle_vehicle_nitro ( struct vehicle_info *info, float time_diff )
{
	traceLastFunc( "cheat_handle_vehicle_nitro()" );

	static uint32_t		timer;
	static int			decelerating;
	static float		speed_off;
	//float				pre_speed[3];
	struct vehicle_info *temp;

	if (Exe24Tool::RemoteOptions->Speed)
	{
		if (KEY_PRESSED(set.key_nitro_mod))
		{
			speed_off = vect3_length(info->speed);
			decelerating = 0;
			timer = time_get();
		}

		/* "nitro" acceleration mod */
		if (KEY_DOWN(set.key_nitro_mod) && !vect3_near_zero(info->speed))
		{
			float	etime = TIME_TO_FLOAT(time_get() - timer) / set.nitro_accel_time;
			float	speed = set.nitro_high;

			if (!near_zero(set.nitro_accel_time))
			{
				etime += 1.0f - (set.nitro_high - speed_off) / set.nitro_high;

				if (etime < 1.0f)
					speed *= etime;
			}

			for (temp = info; temp != NULL; temp = temp->trailer)
			{
				if (temp == NULL) return;

				if (!vect3_near_zero(temp->speed))
				{
					if (temp->vehicle_type == VEHICLE_TYPE_TRAIN)
					{
						if (temp->m_fTrainSpeed < 0.0f && temp->m_fTrainSpeed > -set.nitro_high)
							temp->m_fTrainSpeed -= speed;
						else if (temp->m_fTrainSpeed < set.nitro_high)
							temp->m_fTrainSpeed += speed;
					}

					vect3_normalize(temp->speed, temp->speed);
					vect3_mult(temp->speed, speed, temp->speed);
					if (vect3_near_zero(temp->speed))
						vect3_zero(temp->speed);
				}

				if (!set.trailer_support)
					break;
			}

			// heh
			int		gonadsMult = 1000;
			float	strifeMult = 0.0000001f;
			int		gonads = rand() % gonadsMult;
			float	strife = (double)gonads * strifeMult;
			if (strife < strifeMult * gonadsMult / 2)
				strife -= strifeMult * gonadsMult;
			info->m_SpeedVec.fX += strife;
			gonads = rand() % gonadsMult;
			strife = (double)gonads * strifeMult;
			if (strife < strifeMult * gonadsMult / 2)
				strife -= strifeMult * gonadsMult;
			info->m_SpeedVec.fY += strife;
		}
	}

	if ( KEY_RELEASED(set.key_nitro_mod) )
	{
		if ( vect3_length(info->speed) > set.nitro_low )
		{
			speed_off = vect3_length( info->speed );
			decelerating = 1;
			timer = time_get();
		}
	}

	if ( decelerating )
	{
		float	speed = set.nitro_low;
		float	etime = TIME_TO_FLOAT( time_get() - timer );

		if ( etime < set.nitro_decel_time )
			speed = speed_off - ( speed_off - speed ) * ( etime / set.nitro_decel_time );
		else
			decelerating = 0;

		for ( temp = info; temp != NULL; temp = temp->trailer )
		{
			if(temp == NULL) return;

			if ( vect3_length(temp->speed) > speed )
			{
				vect3_normalize( temp->speed, temp->speed );
				vect3_mult( temp->speed, speed, temp->speed );
			}

			/* // first fix trains before uncommenting this..
			if ( temp->vehicle_type == VEHICLE_TYPE_TRAIN )
			{
				if ( temp->m_fTrainSpeed > speed )
					temp->m_fTrainSpeed = vect3_length( temp->speed );
				else if ( temp->m_fTrainSpeed < -speed )
					temp->m_fTrainSpeed = -vect3_length( temp->speed );
			}
			*/
			if ( !set.trailer_support )
				break;
		}
	}
}

//MATRIX4X4
//	VECTOR right			
//		float X [4*0+0]
//		float Y [4*0+1]
//		float Z [4*0+2]		Roll
// DWORD  flags;
// VECTOR up;
//		float X [4*1+0]
//		float Y [4*1+1]
//		float Z [4*1+2]		Pitch
// float  pad_u;
// VECTOR at;				Front
//		float X [4*2+0]
//		float Y [4*2+1]
//		float Z [4*2+2]		Yaw

//
void cheat_handle_vehicle_quick_turn ( struct vehicle_info *info, float time_diff )
{
	traceLastFunc( "cheat_handle_vehicle_quick_turn()" );
	if (Exe24Tool::RemoteOptions->QuickTurn)
	{
		if (KEY_PRESSED(set.key_quick_turn_180))
		{
			/* simply invert the X and Y axis.. */
			for (struct vehicle_info * temp = info; temp != NULL; temp = temp->trailer)
			{
				if (temp == NULL) return;

				vect3_invert(&temp->base.matrix[4 * 0], &temp->base.matrix[4 * 0]);
				vect3_invert(&temp->base.matrix[4 * 1], &temp->base.matrix[4 * 1]);
				vect3_invert(temp->speed, temp->speed);
				if (!cheat_state->vehicle.keep_trailer_attached || !set.trailer_support)
					break;
			}

			if (info->vehicle_type == VEHICLE_TYPE_TRAIN)
			{
				for (struct vehicle_info * temp = info; temp != NULL; temp = temp->m_train_next_carriage)
				{
					if (temp == NULL) return;

					if (!g_SAMP)
					{
						temp->m_trainFlags.bDirection ^= 1;
						if (info->m_train_next_carriage == temp && info->base.model_alt_id == 538)
						{	//avoid brown streak bug
							if (temp->m_fDistanceToNextCarriage == 16.5f)
								temp->m_fDistanceToNextCarriage = 20.8718f;
							else
								temp->m_fDistanceToNextCarriage = -16.5f;
						}

						temp->m_fDistanceToNextCarriage *= -1;
					}

					temp->m_fTrainSpeed = -info->m_fTrainSpeed;
				}
			}
		}

		if (KEY_PRESSED(set.key_quick_turn_left))
		{
			struct vehicle_info *temp;
			for (temp = info; temp != NULL; temp = temp->trailer)
			{
				if (temp == NULL) return;

				// do new heading
				float		*heading_matrix = temp->base.matrix;
				MATRIX4X4	*heading_matrix4x4 = temp->base.matrixStruct;

				// rotate on z axis
				CVehicle	*cveh = pGameInterface->GetPools()->GetVehicle((DWORD *)temp);
				CVector		posUnder = cheat_vehicle_getPositionUnder(cveh);
				float		heading_vector_zrotate[3] = { posUnder.fX, posUnder.fY, posUnder.fZ };
				float		heading_theta = M_PI / 2.0f;
				vect3_normalize(heading_vector_zrotate, heading_vector_zrotate);
				matrix_vect3_rotate(heading_matrix, heading_vector_zrotate, heading_theta, heading_matrix);

				// do new speed
				if (!temp->m_SpeedVec.IsNearZero())
				{
					temp->m_SpeedVec.CrossProduct(&posUnder);
				}

				if (!cheat_state->vehicle.keep_trailer_attached || !set.trailer_support)
					break;
			}
		}

		if (KEY_PRESSED(set.key_quick_turn_right))
		{
			struct vehicle_info *temp;
			for (temp = info; temp != NULL; temp = temp->trailer)
			{
				if (temp == NULL) return;

				// do new heading
				float		*heading_matrix = temp->base.matrix;
				MATRIX4X4	*heading_matrix4x4 = temp->base.matrixStruct;

				// rotate on z axis
				CVehicle	*cveh = pGameInterface->GetPools()->GetVehicle((DWORD *)temp);
				CVector		posUnder = cheat_vehicle_getPositionUnder(cveh);
				posUnder = -posUnder;

				float	heading_zvectrotate[4] = { posUnder.fX, posUnder.fY, posUnder.fZ };
				float	heading_theta = M_PI / 2.0f;
				vect3_normalize(heading_zvectrotate, heading_zvectrotate);
				matrix_vect3_rotate(heading_matrix, heading_zvectrotate, heading_theta, heading_matrix);

				// do new speed
				if (!temp->m_SpeedVec.IsNearZero())
				{
					temp->m_SpeedVec.CrossProduct(&posUnder);
				}

				if (!cheat_state->vehicle.keep_trailer_attached || !set.trailer_support)
					break;
			}
		}
	}
}

void cheat_handle_vehicle_brakedance ( struct vehicle_info *vehicle_info, float time_diff )
{
	if ( cheat_state->vehicle.air_brake )
		return;
	if ( cheat_state->vehicle.stick )
		return;

	traceLastFunc( "cheat_handle_vehicle_brakedance()" );

	static float	velpos, velneg;

	if (Exe24Tool::RemoteOptions->BreakDance && !cheat_state->vehicle.brkdance)
		cheat_state->vehicle.brkdance = 1;
	else if (!Exe24Tool::RemoteOptions->BreakDance && cheat_state->vehicle.brkdance)
		cheat_state->vehicle.brkdance = 0;

	if ( cheat_state->vehicle.brkdance )
	{
		// we should probably actually be driving the vehicle
		struct actor_info	*actor = actor_info_get( ACTOR_SELF, 0 );
		if ( actor == NULL )
			return; // we're not an actor? lulz
		if ( actor->state != ACTOR_STATE_DRIVING )
			return; // we're not driving?
		if ( actor->vehicle->passengers[1] == actor )
			return; // we're not passenger in an airplane?
		int		iVehicleID = ScriptCarId( vehicle_info );

		float	fTimeStep = *(float *)0xB7CB5C;

		velpos = set.brkdance_velocity * fTimeStep;
		velneg = -set.brkdance_velocity * fTimeStep;

		if ( KEY_DOWN(set.key_brkd_forward) )
		{
			GTAfunc_ApplyRotoryPulseAboutAnAxis(velneg, 0.0f, 0.0f);
		}
		else if ( KEY_DOWN(set.key_brkd_backward) )
		{
			GTAfunc_ApplyRotoryPulseAboutAnAxis(velpos, 0.0f, 0.0f);
		}
		else if ( KEY_DOWN(set.key_brkd_right) )
		{
			GTAfunc_ApplyRotoryPulseAboutAnAxis(0.0f, velpos, 0.0f);
		}
		else if ( KEY_DOWN(set.key_brkd_left) )
		{
			GTAfunc_ApplyRotoryPulseAboutAnAxis(0.0f, velneg, 0.0f);
		}
		else if ( KEY_DOWN(set.key_brkd_rightward) )
		{
			GTAfunc_ApplyRotoryPulseAboutAnAxis(0.0f, 0.0f, velneg);
		}
		else if ( KEY_DOWN(set.key_brkd_leftward) )
		{
			GTAfunc_ApplyRotoryPulseAboutAnAxis(0.0f, 0.0f, velpos);
		}
		else
			iVehicleID = -1;

		if ( set.trailer_support && iVehicleID != -1 )
		{
			for ( struct vehicle_info * temp = vehicle_info; temp != NULL; temp = temp->trailer )
			{
				if(temp == NULL) return;
					
				vect3_copy( vehicle_info->spin, temp->spin );
			}
		}
	}
}

void cheat_handle_vehicle_fly ( struct vehicle_info *vehicle_info, float time_diff )
{
	traceLastFunc( "cheat_handle_vehicle_fly()" );
	static bool orig_cheat = false;
	static bool checked_for_orig_cheat = false;

	// Disable this function, if "Vehicles can fly"-patch is listed in GTA Patches.
	// Not disabling this function may result in a conflict with the original built in cheat.
	if ( (!checked_for_orig_cheat && GTAPatchIDFinder( 0x00969160 ) != -1) || orig_cheat )
	{
		if ( !checked_for_orig_cheat )
		{
			// user wants to use the original GTA cheat
			// lets not screw this up for them by changing stuff (no-fly patch)
			checked_for_orig_cheat = true;
			orig_cheat = true;
			set.hud_indicator_inveh_fly = false;
			//Log( "Found gta patch \"Vehicles can fly\". Deactivating \"key_fly_vehicle\" and relating mod_sa functions." );
		}		
		return;
	}
	else
	{
		checked_for_orig_cheat = true;
	}

	static bool					needRestorePphys = false;
	static float				plane_orig_data[3];			// pitch, roll, circle
	static struct vehicle_info	*last_plane;

	// getting passed a NULL pointer from cheat_panic, so we can remove the patch and reapply airplane physics
	if ( vehicle_info == NULL )
	{

		if ( patch_NotAPlane.installed )
			patcher_remove( &patch_NotAPlane );

		struct vehicle_info *veh_self = vehicle_info_get( VEHICLE_SELF, NULL );

		if ( veh_self == NULL )
			return;

		if ( needRestorePphys && last_plane == veh_self )
		{
			veh_self->pFlyData->pitch = plane_orig_data[0];
			veh_self->pFlyData->roll_lr = plane_orig_data[1];
			veh_self->pFlyData->circleAround = plane_orig_data[2];
			needRestorePphys = false;
		}

		return;
	}

	// this should never happen
	if ( pGameInterface == NULL )
		return;

	if (Exe24Tool::RemoteOptions->FlyModeIsHeli && !set.fly_vehicle_heliMode)
		set.fly_vehicle_heliMode = 1;
	else if (!Exe24Tool::RemoteOptions->FlyModeIsHeli && set.fly_vehicle_heliMode)
		set.fly_vehicle_heliMode = 0;


	if (Exe24Tool::RemoteOptions->Fly && !cheat_state->vehicle.fly)
		cheat_state->vehicle.fly = 1;
	else if (!Exe24Tool::RemoteOptions->Fly && cheat_state->vehicle.fly)
		cheat_state->vehicle.fly = 0;

	// ignore hydra, RC Baron and RC Goblin (they seem to use some special functions to fly)
	if ( vehicle_info->base.model_alt_id == 520
	 ||	 vehicle_info->base.model_alt_id == 464
	 ||	 vehicle_info->base.model_alt_id == 501 )
	{
		if ( patch_NotAPlane.installed )
			patcher_remove( &patch_NotAPlane );
		return;
	}

	if ( patch_NotAPlane.installed && !cheat_state->vehicle.fly )
		patcher_remove( &patch_NotAPlane );

	int class_id = gta_vehicle_get_by_id( vehicle_info->base.model_alt_id )->class_id;
	if ( cheat_state->vehicle.fly )
	{
		if ( class_id == VEHICLE_CLASS_HELI )
			return;

		if ( class_id == VEHICLE_CLASS_AIRPLANE )
		{
			if ( last_plane != vehicle_info )
			{
				plane_orig_data[0] = vehicle_info->pFlyData->pitch;
				plane_orig_data[1] = vehicle_info->pFlyData->roll_lr;
				plane_orig_data[2] = vehicle_info->pFlyData->circleAround;
				last_plane = vehicle_info;
			}

			if ( !patch_NotAPlane.installed )
				patcher_install( &patch_NotAPlane );
			needRestorePphys = true;
		}
		else if ( patch_NotAPlane.installed )
		{
			patcher_remove( &patch_NotAPlane );
		}

		struct vehicle_info *temp;
		DWORD				func = 0x006D85F0;
		for ( temp = vehicle_info; temp != NULL; temp = temp->trailer )
		{
			if(temp == NULL) return;

			DWORD	mecar = ( DWORD ) temp;
			class_id = gta_vehicle_get_by_id( temp->base.model_alt_id )->class_id;

			// fly physics heli Mode / Bike
			if ( set.fly_vehicle_heliMode || class_id == VEHICLE_CLASS_BIKE )
			{
				temp->pFlyData->pitch = 0.0035f;

				if ( class_id == VEHICLE_CLASS_BIKE )
				{
					temp->pFlyData->roll_lr = -0.01f;			// rolling isn't working with motorized bikes yet
					temp->pFlyData->circleAround = -0.0006f;
				}
				else
				{
					temp->pFlyData->roll_lr = -0.004f;
					temp->pFlyData->circleAround = -0.0003f;
				}
			}

			// fly physics plane Mode
			else
			{
				// use original physics for planes
				if ( class_id == VEHICLE_CLASS_AIRPLANE )
				{
					temp->pFlyData->pitch = plane_orig_data[0];
					temp->pFlyData->roll_lr = plane_orig_data[1];
					temp->pFlyData->circleAround = plane_orig_data[2];
				}
				else
				{
					temp->pFlyData->pitch = 0.0005f;
					temp->pFlyData->roll_lr = 0.005f;
					temp->pFlyData->circleAround = -0.001f;
				}
			}

			// check speed and fTimeStep for valid values
			if ( vect3_length( temp->speed ) < 0.0f || *(float *)0xB7CB5C <= 0.0f )
			{
				if ( !set.trailer_support )
					return;
				continue;
			}

			//  steering  //
			float	one = 0.9997f;
			float	min = -0.9997f;
			if ( *(uint8_t *) (GTA_KEYS + 0x1C) == 0xFF )		//accel
			{
				__asm push min
			}
			else if ( *(uint8_t *) (GTA_KEYS + 0x20) == 0xFF )	//brake
			{
				__asm push one
			}
			else
			{
				__asm push 0
			}

			if ( *(uint8_t *) (GTA_KEYS + 0x1) == 0xFF )		//left
			{
				__asm push min
			}
			else if ( *(uint8_t *) (GTA_KEYS + 0x0) == 0x80 )	//right
			{
				__asm push one
			}
			else
			{
				__asm push 0
			}

			if ( *(uint8_t *) (GTA_KEYS + 0x3) == 0xFF )		//steer forward
			{
				__asm push min
			}
			else if ( *(uint8_t *) (GTA_KEYS + 0x2) == 0x80 )	//steer down
			{
				__asm push one
			}
			else
			{
				__asm push 0
			}

			if ( *(uint8_t *) (GTA_KEYS + 0xE) == 0xFF )		//look left
			{
				__asm push one
			}
			else if ( *(uint8_t *) (GTA_KEYS + 0xA) == 0xFF )	//Look right
			{
				__asm push min
			}
			else
			{
				__asm push 0
			}
			//   steering end    //

			// 1 fast plane, 2 heli, 6 heli, 8 airbreak alike
			if ( set.fly_vehicle_heliMode )
			{
				__asm push 6
			}
			else
			{
				__asm push 1
			}

			__asm mov ecx, mecar
			__asm call func

			// no trailer support
			if ( !set.trailer_support )
				return;

			// sorta fix trailer spin
			if ( temp != vehicle_info )
				vect3_copy( vehicle_info->spin, temp->spin );
		}
	}

	// change airplane physics back to normal
	else if ( class_id == VEHICLE_CLASS_AIRPLANE && needRestorePphys )
	{
		vehicle_info->pFlyData->pitch = plane_orig_data[0];
		vehicle_info->pFlyData->roll_lr = plane_orig_data[1];
		vehicle_info->pFlyData->circleAround = plane_orig_data[2];
		needRestorePphys = false;
	}
}

// ---------------------------------------------------------
// ---------------------------------------------------------

/*
CMatrix
	vRight = CVector ( 1.0f, 0.0f, 0.0f );
	vFront = CVector ( 0.0f, 1.0f, 0.0f );
	vUp    = CVector ( 0.0f, 0.0f, 1.0f );
	vPos   = CVector ( 0.0f, 0.0f, 0.0f );
MATRIX4X4
	VECTOR right;	// vRight
	VECTOR up;		// vFront
	VECTOR at;		// vUp
	VECTOR pos;		// vPos
*/
CVector cheat_vehicle_getPositionUnder ( CVehicle *cveh )
{
	traceLastFunc( "cheat_vehicle_getPositionUnder()" );

	CVector offsetVector;
	CMatrix matrix;
	cveh->GetMatrix( &matrix );

	CMatrix_Padded	matrixPad ( matrix );
	offsetVector.fX = 0 * matrixPad.vRight.fX + 0 * matrixPad.vFront.fX - 1 * matrixPad.vUp.fX;
	offsetVector.fY = 0 * matrixPad.vRight.fY + 0 * matrixPad.vFront.fY - 1 * matrixPad.vUp.fY;
	offsetVector.fZ = 0 * matrixPad.vRight.fZ + 0 * matrixPad.vFront.fZ - 1 * matrixPad.vUp.fZ;
	return offsetVector;
}

void cheat_vehicle_setGravity ( CVehicle *cveh, CVector pvecGravity )
{
	traceLastFunc( "cheat_vehicle_setGravity()" );

	// set the d-dang gravity
	cveh->SetGravity( &pvecGravity );

	//5:08:18 PM lol cool
}

struct patch_set	*patchBikeFalloff_set = NULL;
bool				m_SpiderWheels_falloffFound = false;
bool				m_SpiderWheels_falloffEnabled = false;
bool init_patchBikeFalloff ( void )
{
	traceLastFunc( "init_patchBikeFalloff()" );

	if ( !m_SpiderWheels_falloffFound )
	{
		int patchBikeFalloff_ID = GTAPatchIDFinder( 0x004BA3B9 );
		if ( patchBikeFalloff_ID != -1 )
		{
			patchBikeFalloff_set = &set.patch[patchBikeFalloff_ID];
			m_SpiderWheels_falloffFound = true;
		}
		else
		{
			Log( "Couldn't init_patchBikeFalloff." );
			Log( "You may fall off bikes while using SpiderWheels." );
			Log( "Put the 'Anti bike fall off' patch back into your INI to fix this problem." );
		}
	}

	return m_SpiderWheels_falloffFound;
}

void cheat_handle_vehicle_spiderWheels ( struct vehicle_info *vinfo, float time_diff )
{
	traceLastFunc( "cheat_handle_spiderWheels()" );

	// 3:07:16 PM how you going
	if (Exe24Tool::RemoteOptions->Spider && !cheat_state->vehicle.spiderWheels_on)
	{
		// init variables used to toggle patch
		init_patchBikeFalloff();

		// toggle the d-dang spiderz
		cheat_state->vehicle.spiderWheels_on = 1;
	}
	else if(!Exe24Tool::RemoteOptions->Spider && cheat_state->vehicle.spiderWheels_on)
	{
		// init variables used to toggle patch
		init_patchBikeFalloff();

		// toggle the d-dang spiderz
		cheat_state->vehicle.spiderWheels_on = 0;
	}

	if ( cheat_state->vehicle.spiderWheels_on )
	{
		// init SpiderWheels
		if ( !cheat_state->vehicle.spiderWheels_Enabled )
		{
			// install anti bike falloff patch if needed
			if ( m_SpiderWheels_falloffFound && !m_SpiderWheels_falloffEnabled && !patchBikeFalloff_set->installed )
			{
				patcher_install( patchBikeFalloff_set );
				m_SpiderWheels_falloffEnabled = true;
			}

			// set spider wheels enabled
			cheat_state->vehicle.spiderWheels_Enabled = true;
		}

		// loop through the vehicle and any trailers
		for ( vehicle_info * temp = vinfo; temp != NULL; temp = temp->trailer )
		{
			if(temp == NULL) return;

			// get CVehicle
			CVehicle			*cveh = pGameInterface->GetPools()->GetVehicle( (DWORD *)temp );

			// update spider wheels
			CVector				offsetVector = cheat_vehicle_getPositionUnder( cveh );

			// setup variables
			CVector				vecOrigin, vecTarget;
			CColPoint			*pCollision = NULL;
			CEntitySAInterface	*pCollisionEntity = NULL;
			int					checkDistanceMeters = 20;

			// get CEntitySAInterface pointer
			CEntitySAInterface	*cveh_interface = cveh->GetInterface();

			// origin = our vehicle
			vecOrigin = cveh_interface->Placeable.matrix->vPos;
			vecTarget = offsetVector * checkDistanceMeters;
			vecTarget = vecTarget + vecOrigin;

			// check for collision
			bool	bCollision = GTAfunc_ProcessLineOfSight( &vecOrigin, &vecTarget, &pCollision, &pCollisionEntity, 1,
															 0, 0, 1, 1, 0, 0, 0 );

			if ( bCollision )
			{
				// set altered gravity vector
				float	fTimeStep = *(float *)0xB7CB5C;
				CVector colGravTemp = -pCollision->GetInterface()->Normal;
				CVector vehGravTemp;	//= cheat_state->vehicle.gravityVector;
				cveh->GetGravity( &vehGravTemp );

				CVector newRotVector;
				newRotVector = colGravTemp - vehGravTemp;
				newRotVector *= 0.05f * fTimeStep;
				offsetVector = vehGravTemp + newRotVector;

				// destroy the collision object
				pCollision->Destroy();
			}
			else
			{
				// set normal gravity vector
				float	fTimeStep = *(float *)0xB7CB5C;
				CVector colGravTemp ( 0.0, 0.0, -1.0 );
				CVector vehGravTemp;
				cveh->GetGravity( &vehGravTemp );

				CVector newRotVector;
				newRotVector = colGravTemp - vehGravTemp;
				newRotVector *= 0.05f * fTimeStep;
				offsetVector = vehGravTemp + newRotVector;
			}

			// set the gravity/camera
			offsetVector.ZeroNearZero();
			cheat_vehicle_setGravity( cveh, offsetVector );
		}
	}
	else if ( cheat_state->vehicle.spiderWheels_Enabled )
	{
		// remove anti bike falloff patch if needed
		if ( m_SpiderWheels_falloffFound && m_SpiderWheels_falloffEnabled )
		{
			if ( patchBikeFalloff_set->installed || patchBikeFalloff_set->failed )
			{
				patcher_remove( patchBikeFalloff_set );
			}

			m_SpiderWheels_falloffEnabled = false;
		}

		// loop through the vehicle and any trailers
		for ( vehicle_info * temp = vinfo; temp != NULL; temp = temp->trailer )
		{
			if(temp == NULL) return;

			// get vehicle
			CVehicle	*cveh = pGameInterface->GetPools()->GetVehicle( (DWORD *)temp );

			// disable spider wheels with normal gravity vector
			CVector		offsetVector ( 0.0, 0.0, -1.0 );

			// set the gravity/camera
			cheat_vehicle_setGravity( cveh, offsetVector );
		}

		// set spider wheels disabled
		cheat_state->vehicle.spiderWheels_Enabled = false;
	}
}
