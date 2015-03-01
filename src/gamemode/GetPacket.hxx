/*
	Copyright (c) 2014 - 2015 Rafa³ "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafa³ Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Prototype declaration for the FNV hashing function to use elsewhere in the GameMode
			Implementation is located in the source file corresponding to this header

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
* fnv_32a_str - perform a 32 bit Fowler/Noll/Vo FNV-1a hash on a string
*
* input:
*	str	- string to hash
*	hval	- previous hash value or 0 if first call
*
* returns:
*	32 bit hash as a static hash type
*
* NOTE: To use the recommended 32 bit FNV-1a hash, use FNV1_32A_INIT as the
*  	 hval arg on the first call to either fnv_32a_buf() or fnv_32a_str().
*/
unsigned long fnv_32a_str(char *str, unsigned long hval);