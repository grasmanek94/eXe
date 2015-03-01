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
#pragma once
#include "GameMode.hxx"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

const float MATH_PI = 3.14159265359f;
const float RAD_TO_DEG = 180.0f / MATH_PI;
const float DEG_TO_RAD = 1.0f / RAD_TO_DEG;

enum MatrixParts
{
    mp_PITCH,
    mp_ROLL,
    mp_YAW,
    mp_POS
};

enum MatrixIndicator
{
    mi_X,
    mi_Y,
    mi_Z
};

void GetVehicleMatrix(int vehicleid, glm::mat4x3& Mat4x3);

//left +x
//forward +y
//up +z
glm::mat4x3 PositionFromVehicleOffset(int vehicle, glm::vec3 offsets, glm::vec3& return_pos);
glm::vec2 GetPitchYawBetweenCoords(const glm::vec3 &source, const glm::vec3 &target);
void GetPitchYawBetweenCoords(const glm::vec3 &source, const glm::vec3 &target, glm::vec2 &output);
glm::vec3 QuatToEuler(glm::quat quaternion);
glm::quat EulerToQuat(glm::vec3 euler);
glm::vec2 GetPitchYawBetweenPositions(glm::vec3 source, glm::vec3 target);
glm::vec3 GetPitchYawRollBetweenPositions(glm::vec3 pointa, glm::vec3 pointb, glm::vec3 pointc);
glm::quat GetQuatRotBetweenPositions3D(glm::vec3 pointa, glm::vec3 pointb, glm::vec3 pointc);
glm::quat GetQuatRotBetweenPositions2D(glm::vec3 pointa, glm::vec3 pointb);
//gets the correct quaternion between two positions FOR A VEHICLE
glm::quat GetQuatRotForVehBetweenCoords2D(glm::vec3 pointa, glm::vec3 pointb);
//now the freaking most advanced thing of all shit here in this plugin
//this is not perfect but the closest I can get to a quaternion representing all axes (Pitch,Roll,Yaw) based on 3 points
glm::quat GetQuatRotForVehBetweenCoords3D(glm::vec3 pointa, glm::vec3 pointb, glm::vec3 pointc);