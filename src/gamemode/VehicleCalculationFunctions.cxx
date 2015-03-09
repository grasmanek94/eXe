/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file

	________________________________________________________________	
	Notes

	________________________________________________________________
	Dependencies

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoñ
*/
#include "GameMode.hxx"
/*
    Get Corresponding GTA {{Internal}} Vehicle Matrix
*/
//aka quaternion to matrix conversion + including position
void GetVehicleMatrix(int vehicleid, glm::mat4x3& Mat4x3)
{
//initial processing step - gathering information
	float rx, ry, rz, rw, x, y, z;

    GetVehicleRotationQuat(vehicleid,&rw,&rx,&ry,&rz);
    GetVehiclePos(vehicleid,&x, &y, &z);

//initialized math for quaternion to matrix conversion {for sake of simplicity and efficiency}

	float
        x2 = rx * rx,
        y2 = ry * ry,
        z2 = rz * rz,
        xy = rx * ry,
        xz = rx * rz,
        yz = ry * rz,
        wx = rw * rx,
        wy = rw * ry,
        wz = rw * rz;

//the maths required to convert a quat to a matrix
    Mat4x3[mp_PITCH][mi_X] = 1.0 - 2.0 * (y2 + z2);
    Mat4x3[mp_PITCH][mi_Y] = 2.0 * (xy - wz);
    Mat4x3[mp_PITCH][mi_Z] = 2.0 * (xz + wy);
    Mat4x3[mp_ROLL][mi_X] = 2.0 * (xy + wz);
    Mat4x3[mp_ROLL][mi_Y] = 1.0 - 2.0 * (x2 + z2);
    Mat4x3[mp_ROLL][mi_Z] = 2.0 * (yz - wx);
    Mat4x3[mp_YAW][mi_X] = 2.0 * (xz - wy);
    Mat4x3[mp_YAW][mi_Y] = 2.0 * (yz + wx);
    Mat4x3[mp_YAW][mi_Z] = 1.0 - 2.0 * (x2 + y2);
//the gta vehicle matrix has the position in it, I want it to keep just like GTA does so I put the position in
    Mat4x3[mp_POS][mi_X] = x;
    Mat4x3[mp_POS][mi_Y] = y;
    Mat4x3[mp_POS][mi_Z] = z;
}

/*
    Get position offset at position corresponding to the correct vehicle rotation
*/

glm::mat4x3 PositionFromVehicleOffset(int vehicle, glm::vec3 offsets, glm::vec3& return_pos)
{
//initial processing step - gather information
	glm::mat4x3 Mat4x3;
    GetVehicleMatrix(vehicle,Mat4x3);

//offset calculation math
    return_pos.x = offsets.x * Mat4x3[mp_PITCH][mi_X] + offsets.y * Mat4x3[mp_ROLL][mi_X] + offsets.z * Mat4x3[mp_YAW][mi_X] + Mat4x3[mp_POS][mi_X];
    return_pos.y = offsets.x * Mat4x3[mp_PITCH][mi_Y] + offsets.y * Mat4x3[mp_ROLL][mi_Y] + offsets.z * Mat4x3[mp_YAW][mi_Y] + Mat4x3[mp_POS][mi_Y];
    return_pos.z = offsets.x * Mat4x3[mp_PITCH][mi_Z] + offsets.y * Mat4x3[mp_ROLL][mi_Z] + offsets.z * Mat4x3[mp_YAW][mi_Z] + Mat4x3[mp_POS][mi_Z];
	return Mat4x3;
}

glm::vec2 GetPitchYawBetweenCoords(const glm::vec3 &source, const glm::vec3 &target)
{
	glm::vec2 output;
	glm::vec3 result = target - source;
	output.x = atan2(result.y, result.x) * RAD_TO_DEG;//yaw?
	output.y = atan2(result.z, sqrt(result.x * result.x + result.y * result.y)) * RAD_TO_DEG;//pitch?
	return output;
}

void GetPitchYawBetweenCoords(const glm::vec3 &source, const glm::vec3 &target, glm::vec2 &output)
{
	glm::vec3 result = target - source;
	output.x = atan2(result.y, result.x) * RAD_TO_DEG;//yaw
	output.y = atan2(result.z, sqrt(result.x * result.x + result.y * result.y)) * RAD_TO_DEG;//pitch
}

glm::vec3 QuatToEuler(glm::quat quaternion)
{
	return glm::vec3(glm::eulerAngles(quaternion)*RAD_TO_DEG);
}

glm::quat EulerToQuat(glm::vec3 euler)
{
	return glm::quat(euler*DEG_TO_RAD);
}

glm::vec2 GetPitchYawBetweenPositions(glm::vec3 source, glm::vec3 target)
{
	glm::vec2 rot;
	GetPitchYawBetweenCoords(source, target, rot);
	return rot;
}

glm::vec3 GetPitchYawRollBetweenPositions(glm::vec3 pointa, glm::vec3 pointb, glm::vec3 pointc)
{
	glm::vec3 points[3] = { pointa, pointb, pointc };
	glm::vec2 YawPitchRotations[2];
	GetPitchYawBetweenCoords(points[0] + ((points[1] - points[0]) / 2.0f), points[2], YawPitchRotations[0]);
	GetPitchYawBetweenCoords(points[0], points[1], YawPitchRotations[1]);
	return glm::vec3(YawPitchRotations[1].y, YawPitchRotations[0].x, YawPitchRotations[0].y);
}

glm::quat GetQuatRotBetweenPositions3D(glm::vec3 pointa, glm::vec3 pointb, glm::vec3 pointc)
{
	glm::vec3 points[3] = { pointa, pointb, pointc };
	glm::vec2 YawPitchRotations[2];
	GetPitchYawBetweenCoords(points[0] + ((points[1] - points[0]) / 2.0f), points[2], YawPitchRotations[0]);
	GetPitchYawBetweenCoords(points[0], points[1], YawPitchRotations[1]);
	glm::vec3 eulerangles(YawPitchRotations[0].y, YawPitchRotations[0].x, YawPitchRotations[1].x);
	return  glm::quat(eulerangles*DEG_TO_RAD);
}

glm::quat GetQuatRotBetweenPositions2D(glm::vec3 pointa, glm::vec3 pointb)
{
	glm::vec2 rot;
	GetPitchYawBetweenCoords(pointa, pointb);
	glm::vec3 eulerangles(rot.y, rot.x, 0.0f);
	return glm::quat(eulerangles*DEG_TO_RAD);
}

//gets the correct quaternion between two positions FOR A VEHICLE
glm::quat GetQuatRotForVehBetweenCoords2D(glm::vec3 pointa, glm::vec3 pointb)
{
	glm::vec2 rot;
	GetPitchYawBetweenCoords(pointa, pointb, rot);
	rot.x = std::fmod(450.0f - rot.x, 360.0f);
	if (rot.x < 0.0f)
	{
		rot.x += 360.0f;
	}
	return glm::quat(glm::vec3(-rot.y*cos(rot.x*DEG_TO_RAD), -rot.y*sin(-rot.x*DEG_TO_RAD), rot.x)*DEG_TO_RAD);
}

//now the freaking most advanced thing of all shit here in this plugin
//this is not perfect but the closest I can get to a quaternion representing all axes (Pitch,Roll,Yaw) based on 3 points
glm::quat GetQuatRotForVehBetweenCoords3D(glm::vec3 pointa, glm::vec3 pointb, glm::vec3 pointc)
{
	glm::vec3 points[3] = { pointa, pointb, pointc };
	glm::vec2 YawPitchRotations[2];
	GetPitchYawBetweenCoords(points[0] + ((points[1] - points[0]) / 2.0f), points[2], YawPitchRotations[0]);
	GetPitchYawBetweenCoords(points[0], points[1], YawPitchRotations[1]);

	YawPitchRotations[0].x = std::fmod(450.0f - YawPitchRotations[0].x, 360.0f);
	if (YawPitchRotations[0].x < 0.0f)
	{
		YawPitchRotations[0].x += 360.0f;
	}

	float PitchDc = -YawPitchRotations[0].y*cos(YawPitchRotations[0].x*DEG_TO_RAD);
	float PitchDs = -YawPitchRotations[0].y*sin(-YawPitchRotations[0].x*DEG_TO_RAD);
	float RollDc = YawPitchRotations[1].y*cos(YawPitchRotations[0].x*DEG_TO_RAD);
	float RollDs = YawPitchRotations[1].y*sin(YawPitchRotations[0].x*DEG_TO_RAD);

	return glm::quat(glm::vec3(RollDs + PitchDc,RollDc + PitchDs,YawPitchRotations[0].x)*DEG_TO_RAD);
}
