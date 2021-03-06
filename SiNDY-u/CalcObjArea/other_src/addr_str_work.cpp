/*
 * Copyright (C) INCREMENT P CORP. All Rights Reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY INCREMENT P CORP., WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDER BE LIABLE FOR ANY
 * CLAIM, DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
*/

#include "Stdafx.h"
#include <stdio.h>
#include <string.h>
#include "addr_str_work.h"

/**
	sæ¬ºÌ»è
*/
int	addr_str_work::
Get_Sikutyouson ( char*	c_cpAddrCode )	///< ZR[h
{
	if( c_cpAddrCode == NULL ) {
		return	( 0 );
	}
	if( strlen(c_cpAddrCode) < 5 ) {
		return	( 0 );
	}
	// sæ¬ºÌæÊðR[hÅ»Ê
	if( strncmp ( c_cpAddrCode + 2, "2", 1 ) == 0 ) {	// s
		return	( SI );
	}else
	if( strncmp ( c_cpAddrCode + 2, "1", 1 ) == 0 ) {	// æ
		return	( KU );
	}
	return	( TYOU|SON );
}

/**
	sæ¬ºÌ»è
*/
int	addr_str_work::
Get_Sikutyouson_byName ( char*	c_cpAddrName )	///< ZR[h
{
	if( c_cpAddrName == NULL ) {
		return	( 0 );
	}
	int	a_iLeng	= 0;
	a_iLeng		= strlen ( c_cpAddrName );
	if( a_iLeng < 2 ) {
		return	( 0 );
	}
	// ¶ñ©ç
	if( strncmp( (c_cpAddrName+a_iLeng-2), "s", 2) == 0 ) {
		return	( SI );
	}
	if( strncmp( (c_cpAddrName+a_iLeng-2), "s", 2) == 0 ) {
		return	( KU );
	}
	if( strncmp( (c_cpAddrName+a_iLeng-2), "¬", 2) == 0 ) {
		return	( TYOU );
	}
	if( strncmp( (c_cpAddrName+a_iLeng-2), "º", 2) == 0 ) {
		return	( SON );
	}
	return	( 0 );
}

/**
	sæ¬º¼Ìl¾
	131xxAxx100ÈOÍÅÌXy[XÅØé
*/
void	addr_str_work::
Get_Sikutyouson_Name ( char*	c_cpAddrCode,	///< ZR[h
					   char*	c_cpInputName,	///< üÍ
					   char**	c_cpOutputName )///< oÍ
{
	if( c_cpInputName == NULL ) return;
	if( strlen(c_cpAddrCode) < 5 ) return;

	if(	strncmp ( c_cpAddrCode, "131", 3 ) == 0 ||
		strncmp ( c_cpAddrCode+2, "100", 3 ) == 0 ) {
		*c_cpOutputName	= c_cpInputName;// »ÌÜÜÔ·
		return;
	}
	
	char*	a_cpRetPt	= NULL;
	a_cpRetPt	= strstr ( c_cpInputName, "@" );	// SpXy[X
	if( a_cpRetPt == NULL ) {
		*c_cpOutputName	= c_cpInputName;
	}else {
		*c_cpOutputName	= a_cpRetPt+2;
	}
}

/**
	xxð¶Ìt^
*/
void	addr_str_work::
Put_Yakusyo	(	string&	c_sNoteStr )
{
	int	a_iLeng	= 0;
	a_iLeng	= c_sNoteStr.size();
	if( a_iLeng < 4 ) return;

	if( strncmp ( (&c_sNoteStr[a_iLeng-2]), "s", 2 ) == 0 ) {
		c_sNoteStr	+= "ð";
		return;
	}else
	if( strncmp ( (&c_sNoteStr[a_iLeng-2]), "æ", 2 ) == 0 ) {
		c_sNoteStr	+= "ð";
		return;
	}else {
		c_sNoteStr	+= "ðê";
		return;
	}
}
