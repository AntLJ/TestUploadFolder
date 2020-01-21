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

#include "crd_cnv.h"
#include "crd_cnv_defines.h"

//------------------------------------------------------------
/*
	‚P‚XÀ•WŒn‚Ì•\‚ğì‚éƒƒ“ƒoŠÖ”
*/
//------------------------------------------------------------
void	crd_cnv::
Make19tbl	(	void	)
{
	c19_origin	origin19[19] = 
				{{{129,30,0.0},	{33,0,0.0}},	// Œn”Ô†‚P
				{{131,0,0.0},	{33,0,0.0}},	// Œn”Ô†‚Q
				{{132,10,0.0},	{36,0,0.0}},	// Œn”Ô†‚R
				{{133,30,0.0},	{33,0,0.0}},	// Œn”Ô†‚S
				{{134,20,0.0},	{36,0,0.0}},	// Œn”Ô†‚T
				{{136,0,0.0},	{36,0,0.0}},	// Œn”Ô†‚U
				{{137,10,0.0},	{36,0,0.0}},	// Œn”Ô†‚V
				{{138,30,0.0},	{36,0,0.0}},	// Œn”Ô†‚W
				{{139,50,0.0},	{36,0,0.0}},	// Œn”Ô†‚X
				{{140,50,0.0},	{40,0,0.0}},	// Œn”Ô†‚P‚O
				{{140,15,0.0},	{44,0,0.0}},	// Œn”Ô†‚P‚P
				{{142,15,0.0},	{44,0,0.0}},	// Œn”Ô†‚P‚Q
				{{144,15,0.0},	{44,0,0.0}},	// Œn”Ô†‚P‚R
				{{142,0,0.0},	{26,0,0.0}},	// Œn”Ô†‚P‚S
				{{127,30,0.0},	{26,0,0.0}},	// Œn”Ô†‚P‚T
				{{124,0,0.0},	{26,0,0.0}},	// Œn”Ô†‚P‚U
				{{131,0,0.0},	{26,0,0.0}},	// Œn”Ô†‚P‚V
				{{136,0,0.0},	{20,0,0.0}},	// Œn”Ô†‚P‚W
				{{154,0,0.0},	{26,0,0.0}}};	// Œn”Ô†‚P‚X

	// •½–Ê’¼ŠpÀ•W(19Œn)‚Ìƒ‰ƒWƒAƒ“‚Ì•\‚ğì¬‚·‚é
	int	i;
	for (i = 0; i < 19; i++)
	{
		Rad_Origin[i].lat	= (origin19[i].lat.deg + (origin19[i].lat.min / 60.0)) * M_PI / 180.0;
		Rad_Origin[i].lon	= (origin19[i].lon.deg + (origin19[i].lon.min / 60.0)) * M_PI / 180.0;
	//	printf("Rad_Origin[%d].lat	= %0.60f;\n", i, Rad_Origin[i].lat);
	//	printf("Rad_Origin[%d].lon	= %0.60f;\n", i, Rad_Origin[i].lon);
	}
}	

//------------------------------------------------------------
/*
	Ô“¹‚©‚çˆÜ“xPhi‚Ü‚Å‚ÌqŒßüŒÇ’·‚ğ“¾‚é
	@return qŒßüŒÇ’·(m)

	yQlz
	http://vldb.gsi.go.jp/sokuchi/surveycalc/algorithm/b2s/b2s.htm
*/
//------------------------------------------------------------
inline	double	crd_cnv::
M_Arc_Len_Core	(	double	Ra,		// ’n‹…‘È‰~‘Ì‚Ì’·”¼Œaiƒ[ƒgƒ‹j
					double	ea2,	// ‘æˆê—£S—¦‚Ì2æ
					double	Phi	)	// ˆÜ“x
{
	double	tmp1 = 0.0;
	double	A=0.0, B=0.0, C=0.0, D=0.0, E=0.0, F=0.0, G=0.0, H=0.0, I=0.0;
	double	B1=0.0, B2=0.0, B3=0.0, B4=0.0, B5=0.0, B6=0.0, B7=0.0, B8=0.0, B9=0.0;
	double	arc_len=0.0;

	A	= 1.0 + 3.0*ea2/4.0 + 45.0*pow(ea2,2.0)/64.0 + 175.0*pow(ea2,3.0)/256.0 + 11025.0*pow(ea2,4.0)/16384 + 43659.0*pow(ea2,5.0)/65536.0
			+ 693693.0*pow(ea2,6.0)/1048576.0 + 19324305.0*pow(ea2,7.0)/29360128.0 + 4927697775.0*pow(ea2,8.0)/7516192768.0;

	B	= 3.0*ea2/4.0 + 15.0*pow(ea2,2.0)/16.0 + 525.0*pow(ea2,3.0)/512.0 + 2205.0*pow(ea2,4.0)/2048.0 + 72765.0*pow(ea2,5.0)/65536.0
			+ 297297.0*pow(ea2,6.0)/262144.0 + 135270135.0*pow(ea2,7.0)/117440512.0 + 547521975.0*pow(ea2,8.0)/469762048.0;

	C	= 15.0*pow(ea2,2.0)/64.0 + 105.0*pow(ea2,3.0)/256.0 + 2205.0*pow(ea2,4.0)/4096.0 + 10395.0*pow(ea2,5.0)/16384.0
			+ 1486485.0*pow(ea2,6.0)/2097152.0 + 45090045.0*pow(ea2,7.0)/58720256.0 + 766530765.0*pow(ea2,8.0)/939524096.0;

	D	= 35.0*pow(ea2,3.0)/512.0 + 315.0*pow(ea2,4.0)/2048.0 + 31185.0*pow(ea2,5.0)/131072.0 + 165165.0*pow(ea2,6.0)/524288.0
			+ 45090045.0*pow(ea2,7.0)/117440512.0 + 209053845.0*pow(ea2,8.0)/469762048.0;

	E	= 315.0*pow(ea2,4.0)/16384.0 + 3465.0*pow(ea2,5.0)/65536.0 + 99099.0*pow(ea2,6.0)/1048576.0 + 4099095.0*pow(ea2,7.0)/29360128.0
			+ 348423075.0*pow(ea2,8.0)/1879048192.0;

	F	= 693.0*pow(ea2,5.0)/131072.0 + 9009.0*pow(ea2,6.0)/524288.0 + 4099095.0*pow(ea2,7.0)/117440512.0 + 26801775.0*pow(ea2,8.0)/469762048.0;

	G	= 3003.0*pow(ea2,6.0)/2097152.0 + 315315.0*pow(ea2,7.0)/58720256.0 + 11486475.0*pow(ea2,8.0)/939524096.0;

	H	= 45045.0*pow(ea2,7.0)/117440512.0 + 765765.0*pow(ea2,8.0)/469762048.0;

	I	= 765765.0*pow(ea2,8.0)/7516192768.0;

	tmp1	= Ra*(1.0 - ea2);

	B1	= tmp1 * A;
	B2	= tmp1 * (-B/2.0);
	B3	= tmp1 * (C/4.0);
	B4	= tmp1 * (-D/6.0);
	B5	= tmp1 * (E/8.0);
	B6	= tmp1 * (-F/10.0);
	B7	= tmp1 * (G/12.0);
	B8	= tmp1 * (-H/14.0);
	B9	= tmp1 * (I/16.0);

	arc_len	= B1*Phi + B2*sin(2.0*Phi) + B3*sin(4.0*Phi) + B4*sin(6.0*Phi) + B5*sin(8.0*Phi)
				+ B6*sin(10.0*Phi) + B7*sin(12.0*Phi) + B8*sin(14.0*Phi) + B9*sin(16.0*Phi);

	return(arc_len);
}

//------------------------------------------------------------
/*
	Ô“¹‚©‚çˆÜ“xPhi‚Ü‚Å‚ÌqŒßüŒÇ’·‚ğ“¾‚é
	@return qŒßüŒÇ’·(m)
*/
//------------------------------------------------------------
inline	double	crd_cnv::
M_Arc_Len	(	double	Ra,		// ’n‹…‘È‰~‘Ì‚Ì’·”¼Œaiƒ[ƒgƒ‹j
				double	ea2,	// ‘æˆê—£S—¦‚Ì2æ
				double	Phi	)	// ˆÜ“x
{
	double	arc_len;

	arc_len	= Ra * (1.0 - ea2)
			* (C1 * Phi
			- C2 / 2.0 * sin(2.0 * Phi)
			+ C3 / 4.0 * sin(4.0 * Phi)
			- C4 / 6.0 * sin(6 * Phi)
			+ C5 / 8 * sin(8.0 * Phi)
			- C6 / 10.0 * sin(10.0 * Phi));

	return(arc_len);
}

//---------------------------------------------------------------------
/**
* 19À•WŒn‚Ì–kŠC“¹‚Ì5Œ…ƒR[ƒh‚ÆŒn‚Ì‘Î‰‚ğ‰Šú‰»
*/
//---------------------------------------------------------------------
bool	crd_cnv::
Sys_Number_Init(	char*	c_fname	)	///< [in]  “Ç‚İ‚İƒtƒ@ƒCƒ‹–¼
{
	if( c_fname != NULL ) {
		// ƒtƒ@ƒCƒ‹‚ğ“Ç‚İ‚Şê‡‚ÍÅ‰‚ÉƒNƒŠƒA
		m_mCode_Sys19.clear();

		const int	LINEMAX	= 512;
		FILE	*rfp	= NULL;
		char	r_buf[LINEMAX];
		int		max_field	= 0;	// 2000.11.30

		// ƒtƒ@ƒCƒ‹ƒI[ƒvƒ“
		rfp		= fopen(c_fname, "r");
		if(rfp == NULL)
		{
			fprintf(stderr, "ƒtƒ@ƒCƒ‹ƒI[ƒvƒ“ƒGƒ‰[ <%s> II\n", c_fname);
			return	( false );
		}

		memset(r_buf, '\0', LINEMAX);	// ƒoƒbƒtƒ@‚Ì‰Šú‰»

		int	a_iCurLine	= 0;
		for(; fgets(r_buf, LINEMAX, rfp) != NULL; a_iCurLine++)
		{
			if( r_buf[0] == '#' || r_buf[0] == '\n' ) {
				continue;
			}

			int	a_ilen	= strlen(r_buf);
			if( r_buf[a_ilen-1] == '\n' ) {
				r_buf[a_ilen-1] = '\0';
			}

			char	*delim		= "\t, ";			// ƒ^ƒuAƒRƒ“ƒ}AƒXƒy[ƒX‹æØ‚è
			char	*field[2]	= { NULL, NULL };	// ƒtƒB[ƒ‹ƒh‚Ì•¶š—ñ‚Ìƒ|ƒCƒ“ƒ^

			field[0]	= strtok(r_buf, delim);		// ZŠƒR[ƒh
			field[1]	= strtok(NULL, delim);		// Œn”Ô†

			// ƒtƒB[ƒ‹ƒh‚Ì“Ç‚İæ‚èƒ`ƒFƒbƒN
			int	i;
			for(i = 0; i < 2; i++)
			{
				if(field[i] == NULL) {
					return	( false );
				}
			}

			std::string		a_sCode;	// ƒR[ƒh
			int				a_iSys_Num;	// Œn”Ô†

			a_sCode		= field[0];
			a_iSys_Num	= atoi ( field[1] );

			m_mCode_Sys19.insert(std::pair<std::string, int>( a_sCode, a_iSys_Num ));	// ƒf[ƒ^Ši”[
			memset(r_buf, '\0', LINEMAX);	// ƒoƒbƒtƒ@‚Ì‰Šú‰»
		}
		fclose	( rfp );
		return	( true );
	}else {
		// ƒfƒtƒHƒ‹ƒg’lİ’è
		// 11Œn 140“x15•ª0•b0 44“x0•ª0•b0 ¬’Ms@”ŸŠÙs@ˆÉ’Bs@–k“ls@’_Ux’¡ŠÇ“à‚Ì‚¤‚¿—LìŒS‹y‚Ñˆ¸“cŒS@wRx’¡ŠÇ“à@Œãux’¡ŠÇ“à@“n“‡x’¡ŠÇ“à  
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01202", 11 ) );	// –kŠC“¹	“n“‡x’¡	”ŸŠÙs	ƒnƒRƒ_ƒeƒV	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01203", 11 ) );	// –kŠC“¹	Œãux’¡	¬’Ms	ƒIƒ^ƒ‹ƒV	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01233", 11 ) );	// –kŠC“¹	’_Ux’¡	ˆÉ’Bs	ƒ_ƒeƒV	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01236", 11 ) );	// –kŠC“¹	“n“‡x’¡	–k“ls	ƒzƒNƒgƒV	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01331", 11 ) );	// –kŠC“¹	“n“‡x’¡	¼‘O’¬	ƒ}ƒcƒ}ƒGƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01332", 11 ) );	// –kŠC“¹	“n“‡x’¡	•Ÿ“‡’¬	ƒtƒNƒVƒ}ƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01333", 11 ) );	// –kŠC“¹	“n“‡x’¡	’m“à’¬	ƒVƒŠƒEƒ`ƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01334", 11 ) );	// –kŠC“¹	“n“‡x’¡	–ØŒÃ“à’¬	ƒLƒRƒiƒCƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01337", 11 ) );	// –kŠC“¹	“n“‡x’¡	µ”Ñ’¬	ƒiƒiƒGƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01343", 11 ) );	// –kŠC“¹	“n“‡x’¡	­•”’¬	ƒVƒJƒxƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01345", 11 ) );	// –kŠC“¹	“n“‡x’¡	X’¬	ƒ‚ƒŠƒ}ƒ`	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01346", 11 ) );	// –kŠC“¹	“n“‡x’¡	”ª‰_’¬	ƒ„ƒNƒ‚ƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01347", 11 ) );	// –kŠC“¹	“n“‡x’¡	’·–œ•”’¬	ƒIƒVƒƒƒ}ƒ“ƒxƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01361", 11 ) );	// –kŠC“¹	wRx’¡	]·’¬	ƒGƒTƒVƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01362", 11 ) );	// –kŠC“¹	wRx’¡	ãƒm‘’¬	ƒJƒ~ƒmƒNƒjƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01363", 11 ) );	// –kŠC“¹	wRx’¡	Œú‘ò•”’¬	ƒAƒbƒTƒuƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01364", 11 ) );	// –kŠC“¹	wRx’¡	‰³•”’¬	ƒIƒgƒxƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01367", 11 ) );	// –kŠC“¹	wRx’¡	‰œK’¬	ƒIƒNƒVƒŠƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01370", 11 ) );	// –kŠC“¹	wRx’¡	¡‹à’¬	ƒCƒ}ƒJƒlƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01371", 11 ) );	// –kŠC“¹	wRx’¡	‚¹‚½‚È’¬	ƒZƒ^ƒiƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01391", 11 ) );	// –kŠC“¹	Œãux’¡	“‡–q‘º	ƒVƒ}ƒ}ƒLƒ€ƒ‰	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01392", 11 ) );	// –kŠC“¹	Œãux’¡	õ“s’¬	ƒXƒbƒcƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01393", 11 ) );	// –kŠC“¹	Œãux’¡	•¼“à’¬	ƒNƒƒ}ƒcƒiƒCƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01394", 11 ) );	// –kŠC“¹	Œãux’¡	—–‰z’¬	ƒ‰ƒ“ƒRƒVƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01395", 11 ) );	// –kŠC“¹	Œãux’¡	ƒjƒZƒR’¬	ƒjƒZƒRƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01396", 11 ) );	// –kŠC“¹	Œãux’¡	^ë‘º	ƒ}ƒbƒJƒŠƒ€ƒ‰	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01397", 11 ) );	// –kŠC“¹	Œãux’¡	—¯õ“s‘º	ƒ‹ƒXƒcƒ€ƒ‰	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01398", 11 ) );	// –kŠC“¹	Œãux’¡	Šì–Î•Ê’¬	ƒLƒ‚ƒxƒcƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01399", 11 ) );	// –kŠC“¹	Œãux’¡	‹‹É’¬	ƒLƒ‡ƒEƒSƒNƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01400", 11 ) );	// –kŠC“¹	Œãux’¡	‹ä’mˆÀ’¬	ƒNƒbƒ`ƒƒƒ“ƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01401", 11 ) );	// –kŠC“¹	Œãux’¡	‹¤˜a’¬	ƒLƒ‡ƒEƒƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01402", 11 ) );	// –kŠC“¹	Œãux’¡	Šâ“à’¬	ƒCƒƒiƒCƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01403", 11 ) );	// –kŠC“¹	Œãux’¡	”‘‘º	ƒgƒ}ƒŠƒ€ƒ‰	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01404", 11 ) );	// –kŠC“¹	Œãux’¡	_Œb“à‘º	ƒJƒ‚ƒGƒiƒCƒ€ƒ‰	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01405", 11 ) );	// –kŠC“¹	Œãux’¡	Ï’O’¬	ƒVƒƒƒRƒ^ƒ“ƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01406", 11 ) );	// –kŠC“¹	Œãux’¡	ŒÃ•½’¬	ƒtƒ‹ƒrƒ‰ƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01407", 11 ) );	// –kŠC“¹	Œãux’¡	m–Ø’¬	ƒjƒLƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01408", 11 ) );	// –kŠC“¹	Œãux’¡	—]s’¬	ƒˆƒCƒ`ƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01409", 11 ) );	// –kŠC“¹	Œãux’¡	Ôˆäì‘º	ƒAƒJƒCƒKƒƒ€ƒ‰	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01571", 11 ) );	// –kŠC“¹	’_Ux’¡	–L‰Y’¬	ƒgƒˆƒEƒ‰ƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01575", 11 ) );	// –kŠC“¹	’_Ux’¡	‘s•Ë’¬	ƒ\ƒEƒxƒcƒ`ƒ‡ƒE	11
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01584", 11 ) );	// –kŠC“¹	’_Ux’¡	“´–êŒÎ’¬	ƒgƒEƒ„ƒRƒ`ƒ‡ƒE	11
		// 12Œn 142“x15•ª0•b0 44“x0•ª0•b0 D–ys@ˆ®ìs@’t“às@—¯–Gs@”ü‰Ss@—[’£s ŠâŒ©‘òs@“Ï¬–qs@º—–s@m•Ês@–¼Šñs@ˆ°•Ês@Ô•½s@OŠ}s@‘êìs@»ìs@]•Ês çÎs@‰Ìu“às@[ìs@–ä•Ês@•x—Ç–ìs@“o•Ês@Œb’ës@–kL“‡s@Îës@Îëx’¡ŠÇ“à –Ô‘–x’¡ŠÇ“à‚Ì‚¤‚¿–ä•ÊŒS@ãìx’¡ŠÇ“à@@’Jx’¡ŠÇ“à@“ú‚x’¡ŠÇ“à@’_Ux’¡ŠÇ“ài—LìŒS‹y‚Ñˆ¸“cŒS‚ğœ‚­B)@‹ó’mx’¡ŠÇ“à@—¯–Gx’¡ŠÇ“à  
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01100", 12 ) );	// –kŠC“¹	Îëx’¡	D–ys	ƒTƒbƒ|ƒƒV	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01101", 12 ) );	// –kŠC“¹	Îëx’¡	D–ys_’†‰›‹æ	ƒTƒbƒ|ƒƒVƒ`ƒ…ƒEƒIƒEƒN	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01102", 12 ) );	// –kŠC“¹	Îëx’¡	D–ys_–k‹æ	ƒTƒbƒ|ƒƒVƒLƒ^ƒN	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01103", 12 ) );	// –kŠC“¹	Îëx’¡	D–ys_“Œ‹æ	ƒTƒbƒ|ƒƒVƒqƒKƒVƒN	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01104", 12 ) );	// –kŠC“¹	Îëx’¡	D–ys_”’Î‹æ	ƒTƒbƒ|ƒƒVƒVƒƒCƒVƒN	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01105", 12 ) );	// –kŠC“¹	Îëx’¡	D–ys_–L•½‹æ	ƒTƒbƒ|ƒƒVƒgƒˆƒqƒ‰ƒN	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01106", 12 ) );	// –kŠC“¹	Îëx’¡	D–ys_“ì‹æ	ƒTƒbƒ|ƒƒVƒ~ƒiƒ~ƒN	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01107", 12 ) );	// –kŠC“¹	Îëx’¡	D–ys_¼‹æ	ƒTƒbƒ|ƒƒVƒjƒVƒN	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01108", 12 ) );	// –kŠC“¹	Îëx’¡	D–ys_Œú•Ê‹æ	ƒTƒbƒ|ƒƒVƒAƒcƒxƒcƒN	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01109", 12 ) );	// –kŠC“¹	Îëx’¡	D–ys_èˆî‹æ	ƒTƒbƒ|ƒƒVƒeƒCƒlƒN	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01110", 12 ) );	// –kŠC“¹	Îëx’¡	D–ys_´“c‹æ	ƒTƒbƒ|ƒƒVƒLƒˆƒ^ƒN	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01204", 12 ) );	// –kŠC“¹	ãìx’¡	ˆ®ìs	ƒAƒTƒqƒJƒƒV	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01205", 12 ) );	// –kŠC“¹	’_Ux’¡	º—–s	ƒ€ƒƒ‰ƒ“ƒV	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01209", 12 ) );	// –kŠC“¹	‹ó’mx’¡	—[’£s	ƒ†ƒEƒoƒŠƒV	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01210", 12 ) );	// –kŠC“¹	‹ó’mx’¡	ŠâŒ©‘òs	ƒCƒƒ~ƒUƒƒV	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01212", 12 ) );	// –kŠC“¹	—¯–Gx’¡	—¯–Gs	ƒ‹ƒ‚ƒCƒV	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01213", 12 ) );	// –kŠC“¹	’_Ux’¡	“Ï¬–qs	ƒgƒ}ƒRƒ}ƒCƒV	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01214", 12 ) );	// –kŠC“¹	@’Jx’¡	’t“às	ƒƒbƒJƒiƒCƒV	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01215", 12 ) );	// –kŠC“¹	‹ó’mx’¡	”ü‰Ss	ƒrƒoƒCƒV	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01216", 12 ) );	// –kŠC“¹	‹ó’mx’¡	ˆ°•Ês	ƒAƒVƒxƒcƒV	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01217", 12 ) );	// –kŠC“¹	Îëx’¡	]•Ês	ƒGƒxƒcƒV	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01218", 12 ) );	// –kŠC“¹	‹ó’mx’¡	Ô•½s	ƒAƒJƒrƒ‰ƒV	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01219", 12 ) );	// –kŠC“¹	–Ô‘–x’¡	–ä•Ês	ƒ‚ƒ“ƒxƒcƒV	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01220", 12 ) );	// –kŠC“¹	ãìx’¡	m•Ês	ƒVƒxƒcƒV	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01221", 12 ) );	// –kŠC“¹	ãìx’¡	–¼Šñs	ƒiƒˆƒƒV	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01222", 12 ) );	// –kŠC“¹	‹ó’mx’¡	OŠ}s	ƒ~ƒJƒTƒV	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01224", 12 ) );	// –kŠC“¹	Îëx’¡	çÎs	ƒ`ƒgƒZƒV	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01225", 12 ) );	// –kŠC“¹	‹ó’mx’¡	‘êìs	ƒ^ƒLƒJƒƒV	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01226", 12 ) );	// –kŠC“¹	‹ó’mx’¡	»ìs	ƒXƒiƒKƒƒV	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01227", 12 ) );	// –kŠC“¹	‹ó’mx’¡	‰Ìu“às	ƒEƒ^ƒVƒiƒCƒV	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01228", 12 ) );	// –kŠC“¹	‹ó’mx’¡	[ìs	ƒtƒJƒKƒƒV	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01229", 12 ) );	// –kŠC“¹	ãìx’¡	•x—Ç–ìs	ƒtƒ‰ƒmƒV	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01230", 12 ) );	// –kŠC“¹	’_Ux’¡	“o•Ês	ƒmƒ{ƒŠƒxƒcƒV	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01231", 12 ) );	// –kŠC“¹	Îëx’¡	Œb’ës	ƒGƒjƒƒV	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01234", 12 ) );	// –kŠC“¹	Îëx’¡	–kL“‡s	ƒLƒ^ƒqƒƒVƒ}ƒV	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01235", 12 ) );	// –kŠC“¹	Îëx’¡	Îës	ƒCƒVƒJƒŠƒV	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01303", 12 ) );	// –kŠC“¹	Îëx’¡	“–•Ê’¬	ƒgƒEƒxƒcƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01304", 12 ) );	// –kŠC“¹	Îëx’¡	VÂ’Ã‘º	ƒVƒ“ƒVƒmƒcƒ€ƒ‰	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01423", 12 ) );	// –kŠC“¹	‹ó’mx’¡	“ì–y’¬	ƒiƒ“ƒ|ƒƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01424", 12 ) );	// –kŠC“¹	‹ó’mx’¡	“Şˆä]’¬	ƒiƒCƒGƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01425", 12 ) );	// –kŠC“¹	‹ó’mx’¡	ã»ì’¬	ƒJƒ~ƒXƒiƒKƒƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01427", 12 ) );	// –kŠC“¹	‹ó’mx’¡	—Rm’¬	ƒ†ƒjƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01428", 12 ) );	// –kŠC“¹	‹ó’mx’¡	’·À’¬	ƒiƒKƒkƒ}ƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01429", 12 ) );	// –kŠC“¹	‹ó’mx’¡	ŒIR’¬	ƒNƒŠƒ„ƒ}ƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01430", 12 ) );	// –kŠC“¹	‹ó’mx’¡	ŒŒ`’¬	ƒcƒLƒKƒ^ƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01431", 12 ) );	// –kŠC“¹	‹ó’mx’¡	‰Y‰P’¬	ƒEƒ‰ƒEƒXƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01432", 12 ) );	// –kŠC“¹	‹ó’mx’¡	V\’Ãì’¬	ƒVƒ“ƒgƒcƒJƒƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01433", 12 ) );	// –kŠC“¹	‹ó’mx’¡	–…”w‹’¬	ƒ‚ƒZƒEƒVƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01434", 12 ) );	// –kŠC“¹	‹ó’mx’¡	’•ƒ•Ê’¬	ƒ`ƒbƒvƒxƒcƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01436", 12 ) );	// –kŠC“¹	‹ó’mx’¡	‰J—³’¬	ƒEƒŠƒ…ƒEƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01437", 12 ) );	// –kŠC“¹	‹ó’mx’¡	–k—³’¬	ƒzƒNƒŠƒ…ƒEƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01438", 12 ) );	// –kŠC“¹	‹ó’mx’¡	À“c’¬	ƒkƒ}ƒ^ƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01452", 12 ) );	// –kŠC“¹	ãìx’¡	‘é²’¬	ƒ^ƒJƒXƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01453", 12 ) );	// –kŠC“¹	ãìx’¡	“Œ_Šy’¬	ƒqƒKƒVƒJƒOƒ‰ƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01454", 12 ) );	// –kŠC“¹	ãìx’¡	“––ƒ’¬	ƒgƒEƒ}ƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01455", 12 ) );	// –kŠC“¹	ãìx’¡	”ä•z’¬	ƒsƒbƒvƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01456", 12 ) );	// –kŠC“¹	ãìx’¡	ˆ¤•Ê’¬	ƒAƒCƒxƒcƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01457", 12 ) );	// –kŠC“¹	ãìx’¡	ãì’¬	ƒJƒ~ƒJƒƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01458", 12 ) );	// –kŠC“¹	ãìx’¡	“Œì’¬	ƒqƒKƒVƒJƒƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01459", 12 ) );	// –kŠC“¹	ãìx’¡	”ü‰l’¬	ƒrƒGƒCƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01460", 12 ) );	// –kŠC“¹	ãìx’¡	ã•x—Ç–ì’¬	ƒJƒ~ƒtƒ‰ƒmƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01461", 12 ) );	// –kŠC“¹	ãìx’¡	’†•x—Ç–ì’¬	ƒiƒJƒtƒ‰ƒmƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01462", 12 ) );	// –kŠC“¹	ãìx’¡	“ì•x—Ç–ì’¬	ƒ~ƒiƒ~ƒtƒ‰ƒmƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01463", 12 ) );	// –kŠC“¹	ãìx’¡	èŠ¥‘º	ƒVƒ€ƒJƒbƒvƒ€ƒ‰	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01464", 12 ) );	// –kŠC“¹	ãìx’¡	˜aŠ¦’¬	ƒƒbƒTƒ€ƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01465", 12 ) );	// –kŠC“¹	ãìx’¡	Œ••£’¬	ƒPƒ“ƒuƒ`ƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01468", 12 ) );	// –kŠC“¹	ãìx’¡	‰ºì’¬	ƒVƒ‚ƒJƒƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01469", 12 ) );	// –kŠC“¹	ãìx’¡	”ü[’¬	ƒrƒtƒJƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01470", 12 ) );	// –kŠC“¹	ãìx’¡	‰¹ˆĞq•{‘º	ƒIƒgƒCƒlƒbƒvƒ€ƒ‰	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01471", 12 ) );	// –kŠC“¹	ãìx’¡	’†ì’¬	ƒiƒJƒKƒƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01472", 12 ) );	// –kŠC“¹	ãìx’¡	–y‰Á“à’¬	ƒzƒƒJƒiƒCƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01481", 12 ) );	// –kŠC“¹	—¯–Gx’¡	‘–Ñ’¬	ƒ}ƒVƒPƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01482", 12 ) );	// –kŠC“¹	—¯–Gx’¡	¬•½’¬	ƒIƒrƒ‰ƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01483", 12 ) );	// –kŠC“¹	—¯–Gx’¡	“Ï‘O’¬	ƒgƒ}ƒ}ƒGƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01484", 12 ) );	// –kŠC“¹	—¯–Gx’¡	‰H–y’¬	ƒnƒ{ƒƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01485", 12 ) );	// –kŠC“¹	—¯–Gx’¡	‰R•Ê‘º	ƒVƒ‡ƒTƒ“ƒxƒcƒ€ƒ‰	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01486", 12 ) );	// –kŠC“¹	—¯–Gx’¡	‰“•Ê’¬	ƒGƒ“ƒxƒcƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01487", 12 ) );	// –kŠC“¹	—¯–Gx’¡	“V‰–’¬	ƒeƒVƒIƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01511", 12 ) );	// –kŠC“¹	@’Jx’¡	‰•¥‘º	ƒTƒ‹ƒtƒcƒ€ƒ‰	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01512", 12 ) );	// –kŠC“¹	@’Jx’¡	•l“Ú•Ê’¬	ƒnƒ}ƒgƒ“ƒxƒcƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01513", 12 ) );	// –kŠC“¹	@’Jx’¡	’†“Ú•Ê’¬	ƒiƒJƒgƒ“ƒxƒcƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01514", 12 ) );	// –kŠC“¹	@’Jx’¡	}K’¬	ƒGƒTƒVƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01516", 12 ) );	// –kŠC“¹	@’Jx’¡	–L•x’¬	ƒgƒˆƒgƒ~ƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01517", 12 ) );	// –kŠC“¹	@’Jx’¡	—ç•¶’¬	ƒŒƒuƒ“ƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01518", 12 ) );	// –kŠC“¹	@’Jx’¡	—˜K’¬	ƒŠƒVƒŠƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01519", 12 ) );	// –kŠC“¹	@’Jx’¡	—˜K•xm’¬	ƒŠƒVƒŠƒtƒWƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01520", 12 ) );	// –kŠC“¹	@’Jx’¡	–y‰„’¬	ƒzƒƒmƒxƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01555", 12 ) );	// –kŠC“¹	–Ô‘–x’¡	‰“Œy’¬	ƒGƒ“ƒKƒ‹ƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01558", 12 ) );	// –kŠC“¹	–Ô‘–x’¡	ã—N•Ê’¬	ƒJƒ~ƒ†ƒEƒxƒcƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01559", 12 ) );	// –kŠC“¹	–Ô‘–x’¡	—N•Ê’¬	ƒ†ƒEƒxƒcƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01560", 12 ) );	// –kŠC“¹	–Ô‘–x’¡	‘êã’¬	ƒ^ƒLƒmƒEƒGƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01561", 12 ) );	// –kŠC“¹	–Ô‘–x’¡	‹»•”’¬	ƒIƒRƒbƒyƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01562", 12 ) );	// –kŠC“¹	–Ô‘–x’¡	¼‹»•”‘º	ƒjƒVƒIƒRƒbƒyƒ€ƒ‰	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01563", 12 ) );	// –kŠC“¹	–Ô‘–x’¡	—Y•’¬	ƒIƒEƒ€ƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01578", 12 ) );	// –kŠC“¹	’_Ux’¡	”’˜V’¬	ƒVƒ‰ƒIƒCƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01581", 12 ) );	// –kŠC“¹	’_Ux’¡	Œú^’¬	ƒAƒcƒ}ƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01585", 12 ) );	// –kŠC“¹	’_Ux’¡	ˆÀ•½’¬	ƒAƒrƒ‰ƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01586", 12 ) );	// –kŠC“¹	’_Ux’¡	‚Ş‚©‚í’¬	ƒ€ƒJƒƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01601", 12 ) );	// –kŠC“¹	“ú‚x’¡	“ú‚’¬	ƒqƒ_ƒJƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01602", 12 ) );	// –kŠC“¹	“ú‚x’¡	•½æ’¬	ƒrƒ‰ƒgƒŠƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01604", 12 ) );	// –kŠC“¹	“ú‚x’¡	VŠ¥’¬	ƒjƒCƒJƒbƒvƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01607", 12 ) );	// –kŠC“¹	“ú‚x’¡	‰Y‰Í’¬	ƒEƒ‰ƒJƒƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01608", 12 ) );	// –kŠC“¹	“ú‚x’¡	—l—’¬	ƒTƒ}ƒjƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01609", 12 ) );	// –kŠC“¹	“ú‚x’¡	‚¦‚è‚à’¬	ƒGƒŠƒ‚ƒ`ƒ‡ƒE	12
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01610", 12 ) );	// –kŠC“¹	“ú‚x’¡	V‚Ğ‚¾‚©’¬	ƒVƒ“ƒqƒ_ƒJƒ`ƒ‡ƒE	12
		// 13Œn 144“x15•ª0•b0 44“x0•ª0•b0 –kŒ©s@‘ÑLs@‹ú˜Hs@–Ô‘–s@ªºs@ªºx’¡ŠÇ“à@‹ú˜Hx’¡ŠÇ“à@–Ô‘–x’¡ŠÇ“ài–ä•ÊŒS‚ğœ‚­B)@\Ÿx’¡ŠÇ“à  
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01206", 13 ) );	// –kŠC“¹	‹ú˜Hx’¡	‹ú˜Hs	ƒNƒVƒƒV	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01207", 13 ) );	// –kŠC“¹	\Ÿx’¡	‘ÑLs	ƒIƒrƒqƒƒV	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01208", 13 ) );	// –kŠC“¹	–Ô‘–x’¡	–kŒ©s	ƒLƒ^ƒ~ƒV	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01211", 13 ) );	// –kŠC“¹	–Ô‘–x’¡	–Ô‘–s	ƒAƒoƒVƒŠƒV	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01223", 13 ) );	// –kŠC“¹	ªºx’¡	ªºs	ƒlƒ€ƒƒV	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01543", 13 ) );	// –kŠC“¹	–Ô‘–x’¡	”ü–y’¬	ƒrƒzƒƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01544", 13 ) );	// –kŠC“¹	–Ô‘–x’¡	’Ã•Ê’¬	ƒcƒxƒcƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01545", 13 ) );	// –kŠC“¹	–Ô‘–x’¡	Î—¢’¬	ƒVƒƒƒŠƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01546", 13 ) );	// –kŠC“¹	–Ô‘–x’¡	´—¢’¬	ƒLƒˆƒTƒgƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01547", 13 ) );	// –kŠC“¹	–Ô‘–x’¡	¬´…’¬	ƒRƒVƒ~ƒYƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01549", 13 ) );	// –kŠC“¹	–Ô‘–x’¡	ŒPq•{’¬	ƒNƒ“ƒlƒbƒvƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01550", 13 ) );	// –kŠC“¹	–Ô‘–x’¡	’uŒË’¬	ƒIƒPƒgƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01552", 13 ) );	// –kŠC“¹	–Ô‘–x’¡	²˜CŠÔ’¬	ƒTƒƒ}ƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01564", 13 ) );	// –kŠC“¹	–Ô‘–x’¡	‘å‹ó’¬	ƒIƒIƒ]ƒ‰ƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01631", 13 ) );	// –kŠC“¹	\Ÿx’¡	‰¹X’¬	ƒIƒgƒtƒPƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01632", 13 ) );	// –kŠC“¹	\Ÿx’¡	m–y’¬	ƒVƒzƒƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01633", 13 ) );	// –kŠC“¹	\Ÿx’¡	ãm–y’¬	ƒJƒ~ƒVƒzƒƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01634", 13 ) );	// –kŠC“¹	\Ÿx’¡	­’Ç’¬	ƒVƒJƒIƒCƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01635", 13 ) );	// –kŠC“¹	\Ÿx’¡	V“¾’¬	ƒVƒ“ƒgƒNƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01636", 13 ) );	// –kŠC“¹	\Ÿx’¡	´…’¬	ƒVƒ~ƒYƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01637", 13 ) );	// –kŠC“¹	\Ÿx’¡	‰èº’¬	ƒƒ€ƒƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01638", 13 ) );	// –kŠC“¹	\Ÿx’¡	’†D“à‘º	ƒiƒJƒTƒcƒiƒCƒ€ƒ‰	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01639", 13 ) );	// –kŠC“¹	\Ÿx’¡	X•Ê‘º	ƒTƒ‰ƒxƒcƒ€ƒ‰	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01641", 13 ) );	// –kŠC“¹	\Ÿx’¡	‘å÷’¬	ƒ^ƒCƒLƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01642", 13 ) );	// –kŠC“¹	\Ÿx’¡	L”ö’¬	ƒqƒƒIƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01643", 13 ) );	// –kŠC“¹	\Ÿx’¡	–‹•Ê’¬	ƒ}ƒNƒxƒcƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01644", 13 ) );	// –kŠC“¹	\Ÿx’¡	’r“c’¬	ƒCƒPƒ_ƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01645", 13 ) );	// –kŠC“¹	\Ÿx’¡	–L ’¬	ƒgƒˆƒRƒƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01646", 13 ) );	// –kŠC“¹	\Ÿx’¡	–{•Ê’¬	ƒzƒ“ƒxƒcƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01647", 13 ) );	// –kŠC“¹	\Ÿx’¡	‘«Šñ’¬	ƒAƒVƒ‡ƒƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01648", 13 ) );	// –kŠC“¹	\Ÿx’¡	—¤•Ê’¬	ƒŠƒNƒxƒcƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01649", 13 ) );	// –kŠC“¹	\Ÿx’¡	‰Y–y’¬	ƒEƒ‰ƒzƒƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01661", 13 ) );	// –kŠC“¹	‹ú˜Hx’¡	‹ú˜H’¬	ƒNƒVƒƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01662", 13 ) );	// –kŠC“¹	‹ú˜Hx’¡	ŒúŠİ’¬	ƒAƒbƒPƒVƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01663", 13 ) );	// –kŠC“¹	‹ú˜Hx’¡	•l’†’¬	ƒnƒ}ƒiƒJƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01664", 13 ) );	// –kŠC“¹	‹ú˜Hx’¡	•W’ƒ’¬	ƒVƒxƒ`ƒƒƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01665", 13 ) );	// –kŠC“¹	‹ú˜Hx’¡	’íq‹ü’¬	ƒeƒVƒJƒKƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01667", 13 ) );	// –kŠC“¹	‹ú˜Hx’¡	’ß‹‘º	ƒcƒ‹ƒCƒ€ƒ‰	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01668", 13 ) );	// –kŠC“¹	‹ú˜Hx’¡	”’f’¬	ƒVƒ‰ƒkƒJƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01691", 13 ) );	// –kŠC“¹	ªºx’¡	•ÊŠC’¬	ƒxƒbƒJƒCƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01692", 13 ) );	// –kŠC“¹	ªºx’¡	’†•W’Ã’¬	ƒiƒJƒVƒxƒcƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01693", 13 ) );	// –kŠC“¹	ªºx’¡	•W’Ã’¬	ƒVƒxƒcƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01694", 13 ) );	// –kŠC“¹	ªºx’¡	—…‰P’¬	ƒ‰ƒEƒXƒ`ƒ‡ƒE	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01695", 13 ) );	// –kŠC“¹	ªºx’¡	F’O‘º	ƒVƒRƒ^ƒ“ƒ€ƒ‰	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01696", 13 ) );	// –kŠC“¹	ªºx’¡	”‘‘º	ƒgƒ}ƒŠƒ€ƒ‰	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01697", 13 ) );	// –kŠC“¹	ªºx’¡	—¯–é•Ê‘º	ƒ‹ƒˆƒxƒcƒ€ƒ‰	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01698", 13 ) );	// –kŠC“¹	ªºx’¡	—¯•Ê‘º	ƒ‹ƒxƒcƒ€ƒ‰	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01699", 13 ) );	// –kŠC“¹	ªºx’¡	Ñ“ß‘º	ƒVƒƒƒiƒ€ƒ‰	13
		m_mCode_Sys19.insert ( std::pair<std::string, int>(  "01700", 13 ) );	// –kŠC“¹	ªºx’¡	åAæ‘º	ƒVƒxƒgƒƒ€ƒ‰	13
	}
	return	( false );
}

//------------------------------------------------------------
/*
	ŒoˆÜ“x‚©‚çˆê”Ô‹ß‚¢‚Æ‚±‚ë‚Ì19À•WŒn‚ÌŒn‚ğ“¾‚é
	i‚È‚ñ‚©–³‘Ê‚È‚±‚Æ‚ğ‚µ‚Ä‚¢‚é‚Á‚Û‚¢j
	@return		Œn”Ô†
*/
//------------------------------------------------------------
int	crd_cnv::
Sys_Number	(	double		lon,	// Œo“x
				double		lat)	// ˆÜ“x
{
	double	d_lat,	d_lon;
	double	dist			= 9999999.0;	// Œ»İ’²‚×‚Ä‚¢‚é‹——£
	double	dist_n1			= 9999999.0;	// Å‚à‹ß‚©‚Á‚½‹——£
	double	dist_n2			= 9999999.0;	// ‚Q”Ô–Ú‚É‹ß‚©‚Á‚½‹——£
	int		sys_n1			= 0;
	int		sys_n2			= 0;
	int		i;

//	Make19tbl();	// 19À•WŒn‚Ìƒe[ƒuƒ‹‚ğì¬
		
	// degree‚ğradian‚É•ÏŠ·
	lat	= lat * M_PI / 180.0;
	lon	= lon * M_PI / 180.0;

	for(i = 0; i < 19; i++)
	{
		d_lat	= Rad_Origin[i].lat - lat;
		d_lon	= Rad_Origin[i].lon - lon;
		dist	= sqrt(pow(d_lat, 2.0) + pow(d_lon, 2.0));
		if(dist < dist_n1)
		{
			sys_n1		= i + 1;
			dist_n1		= dist;
		}
		else
		if(dist < dist_n2)
		{
			sys_n2		= i + 1;
			dist_n2		= dist;
		}
	}
	return(sys_n1);
}

//------------------------------------------------------------
/*
	ŒoˆÜ“x‚Æs‹æ’¬‘ºƒR[ƒh(5Œ…)‚©‚ç19À•WŒn‚ÌŒn‚ğ“¾‚é
	@return     Œn”Ô†(s‹æ’¬‘ºƒR[ƒh‚âŒoˆÜ“x‚ÉŠÔˆá‚¢‚ª‚ ‚éê‡‚Í0‚ğ•Ô‹p)
*/
//------------------------------------------------------------
int	crd_cnv::
Sys_Number	(	double	lon,	// [in]  Œo“x
				double	lat,	// [in]  ˆÜ“x
				char*	code5 )	// [in]  s‹æ’¬‘ºƒR[ƒh(5Œ…)
{
	// ƒe[ƒuƒ‹‚ª‹ó‚¾‚Á‚½ê‡Aˆê“xƒfƒtƒHƒ‹ƒg‚Ì‰Šú‰»‚ğ‚·‚éB
	// iŠù‚ÉŠO•”‚Å‰Šú‰»À{‚µ‚Ä‚¢‚éê‡A‚±‚±‚Í’Ê‚ç‚È‚¢j
	if( m_mCode_Sys19.size() == 0 ) {
		Sys_Number_Init();
	}

	if( code5 == NULL ) {
		return ( 0 );
	}else
	if( strlen(code5) < 5 ) {
		return ( 0 ); 
	}
	// 1Œn 129“x30•ª0•b0 33“x0•ª0•b0 ’·èŒ§ ­™“‡Œ§‚Ì‚¤‚¿–k•û–kˆÜ32“x“ì•û–kˆÜ27“x¼•û“ŒŒo128“x18•ª“Œ•û“ŒŒo130“x‚ğ‹«ŠEü‚Æ‚·‚é‹æˆæ“ài‰‚”üŒQ“‡‚Í“ŒŒo130“x13•ª‚Ü‚Å‚ğŠÜ‚ŞB)‚É‚ ‚é‚·‚×‚Ä‚Ì“‡A¬“‡AŠÂÊ‹y‚ÑŠâÊ  
	if( strncmp( code5, "42", 2 ) == 0 ) {	// ’·èŒ§
		return	( 1 );
	}else
	if( strncmp( code5, "46", 2 ) == 0 ){	// ­™“‡Œ§
		// –k•û–kˆÜ32“x “ì•û–kˆÜ27“x ¼•û“ŒŒo128“x18•ª “Œ•û“ŒŒo130“x
		if(	27.0 <= lat && lat <= 32.0 && 
			128.30 <= lon && lon <= 130.0 ) {
			return	( 1 );
		}else {
			// 2Œn 131“x 0•ª0•b0 33“x0•ª0•b0 •Ÿ‰ªŒ§@²‰êŒ§@ŒF–{Œ§@‘å•ªŒ§@‹{èŒ§@­™“‡Œ§i‘æIŒn‚É‹K’è‚·‚é‹æˆæ‚ğœ‚­B)  
			return	( 2 );
		}
	}else
	// 2Œn 131“x 0•ª0•b0 33“x0•ª0•b0 •Ÿ‰ªŒ§@²‰êŒ§@ŒF–{Œ§@‘å•ªŒ§@‹{èŒ§@­™“‡Œ§i‘æIŒn‚É‹K’è‚·‚é‹æˆæ‚ğœ‚­B)  
	if( strncmp( code5, "40", 2 ) == 0 ||	// •Ÿ‰ªŒ§
		strncmp( code5, "41", 2 ) == 0 ||	// ²‰êŒ§
		strncmp( code5, "43", 2 ) == 0 ||	// ŒF–{Œ§
		strncmp( code5, "44", 2 ) == 0 ||	// ‘å•ªŒ§
		strncmp( code5, "45", 2 ) == 0 ) {	// ‹{èŒ§
		return	( 2 );
	}else
	// 3Œn 132“x10•ª0•b0 36“x0•ª0•b0 RŒûŒ§@“‡ªŒ§@L“‡Œ§  
	if( strncmp( code5, "32", 2 ) == 0 ||	// “‡ªŒ§
		strncmp( code5, "34", 2 ) == 0 ||	// L“‡Œ§
		strncmp( code5, "35", 2 ) == 0 ) {	// RŒûŒ§
		return	( 3 );
	}else
	// 4Œn 133“x30•ª0•b0 33“x0•ª0•b0 ìŒ§@ˆ¤•QŒ§@“¿“‡Œ§@‚’mŒ§  
	if( strncmp( code5, "36", 2 ) == 0 ||	// “¿“‡Œ§
		strncmp( code5, "37", 2 ) == 0 ||	// ìŒ§
		strncmp( code5, "38", 2 ) == 0 ||	// ˆ¤•QŒ§
		strncmp( code5, "39", 2 ) == 0 ) {	// ‚’mŒ§
		return	( 4 );
	}else
	// 5Œn 134“x20•ª0•b0 36“x0•ª0•b0 •ºŒÉŒ§@’¹æŒ§@‰ªRŒ§  
	if( strncmp( code5, "28", 2 ) == 0 ||	// •ºŒÉŒ§
		strncmp( code5, "31", 2 ) == 0 ||	// ’¹æŒ§
		strncmp( code5, "33", 2 ) == 0 ) {	// ‰ªRŒ§
		return	( 5 );
	}else
	// 6Œn 136“x 0•ª0•b0 36“x0•ª0•b0 ‹“s•{@‘åã•{@•ŸˆäŒ§@ ‰êŒ§@OdŒ§@“Ş—ÇŒ§ ˜a‰ÌRŒ§ 
	if( strncmp( code5, "18", 2 ) == 0 ||	// •ŸˆäŒ§
		strncmp( code5, "24", 2 ) == 0 ||	// OdŒ§
		strncmp( code5, "25", 2 ) == 0 ||	//  ‰êŒ§
		strncmp( code5, "26", 2 ) == 0 ||	// ‹“s•{
		strncmp( code5, "27", 2 ) == 0 ||	// ‘åã•{
		strncmp( code5, "29", 2 ) == 0 ||	// “Ş—ÇŒ§
		strncmp( code5, "30", 2 ) == 0 ) {	// ˜a‰ÌRŒ§
		return	( 6 );
	}else
	// 7Œn 137“x10•ª0•b0 36“x0•ª0•b0 ÎìŒ§@•xRŒ§@Šò•ŒŒ§@ˆ¤’mŒ§  
	if( strncmp( code5, "16", 2 ) == 0 ||	// •xRŒ§	7
		strncmp( code5, "17", 2 ) == 0 ||	// ÎìŒ§	7
		strncmp( code5, "21", 2 ) == 0 ||	// Šò•ŒŒ§	7
		strncmp( code5, "23", 2 ) == 0 ) {	// ˆ¤’mŒ§	7
		return	( 7 );
	}else
	// 8Œn 138“x30•ª0•b0 36“x0•ª0•b0 VŠƒŒ§@’·–ìŒ§@R—œŒ§@Ã‰ªŒ§  
	if( strncmp( code5, "15", 2 ) == 0 ||	// VŠƒŒ§	8
		strncmp( code5, "19", 2 ) == 0 ||	// R—œŒ§	8
		strncmp( code5, "20", 2 ) == 0 ||	// ’·–ìŒ§	8
		strncmp( code5, "22", 2 ) == 0 ) {	// Ã‰ªŒ§	8
		return	( 8 );
	}else
	// 9Œn 139“x50•ª0•b0 36“x0•ª0•b0 “Œ‹“si14ŒnA18Œn‹y‚Ñ19Œn‚É‹K’è‚·‚é‹æˆæ‚ğœ‚­B)@•Ÿ“‡Œ§@“È–ØŒ§@ˆïéŒ§@é‹ÊŒ§ ç—tŒ§ ŒQ”nŒ§@_“ŞìŒ§  
	if( strncmp( code5, "13", 2 ) == 0 ) {	// “Œ‹“s
		if(	lat <= 28.0 ) {		//  –kˆÜ28“x‚©‚ç“ì
			// 14Œn 142“x 0•ª0•b0 26“x0•ª0•b0 “Œ‹“s‚Ì‚¤‚¿–kˆÜ28“x‚©‚ç“ì‚Å‚ ‚èA‚©‚Â“ŒŒo140“x30•ª‚©‚ç“Œ‚Å‚ ‚è“ŒŒo143“x‚©‚ç¼‚Å‚ ‚é‹æˆæ  
			if( 140.50 <= lon && lon <= 143.0 ) {	// “ŒŒo140“x30•ª‚©‚ç“Œ‚Å‚ ‚è“ŒŒo143“x‚©‚ç¼
				return	( 14 );
			}else
			// 18Œn 136“x 0•ª0•b0 20“x0•ª0•b0 “Œ‹“s‚Ì‚¤‚¿–kˆÜ28“x‚©‚ç“ì‚Å‚ ‚èA‚©‚Â“ŒŒo140“x30•ª‚©‚ç¼‚Å‚ ‚é‹æˆæ
			if( lon < 140.50 ) {	// “ŒŒo140“x30•ª‚©‚ç¼
				return	( 18 );
			}else
			// 19Œn 154“x 0•ª0•b0 26“x0•ª0•b0 “Œ‹“s‚Ì‚¤‚¿–kˆÜ28“x‚©‚ç“ì‚Å‚ ‚èA‚©‚Â“ŒŒo143“x‚©‚ç“Œ‚Å‚ ‚é‹æˆæ
			if( lon > 143.0 ) {		// “ŒŒo143“x‚©‚ç“Œ
				return	( 19 );
			}
		}
		return	( 9 );
	}else
	// 9Œn 139“x50•ª0•b0 36“x0•ª0•b0 “Œ‹“si14ŒnA18Œn‹y‚Ñ19Œn‚É‹K’è‚·‚é‹æˆæ‚ğœ‚­B)@•Ÿ“‡Œ§@“È–ØŒ§@ˆïéŒ§@é‹ÊŒ§ ç—tŒ§ ŒQ”nŒ§@_“ŞìŒ§  
	if( strncmp( code5, "07", 2 ) == 0 ||	// •Ÿ“‡Œ§	9
		strncmp( code5, "08", 2 ) == 0 ||	// ˆïéŒ§	9
		strncmp( code5, "09", 2 ) == 0 ||	// “È–ØŒ§	9
		strncmp( code5, "10", 2 ) == 0 ||	// ŒQ”nŒ§	9
		strncmp( code5, "11", 2 ) == 0 ||	// é‹ÊŒ§	9
		strncmp( code5, "12", 2 ) == 0 ||	// ç—tŒ§	9
		strncmp( code5, "14", 2 ) == 0 ) {	// _“ŞìŒ§	9
		return	( 9 );
	}else
	// 10Œn 140“x50•ª0•b0 40“x0•ª0•b0 ÂXŒ§@H“cŒ§@RŒ`Œ§@ŠâèŒ§@‹{éŒ§  
	if( strncmp( code5, "02", 2 ) == 0 ||	// ÂXŒ§	10
		strncmp( code5, "03", 2 ) == 0 ||	// ŠâèŒ§	10
		strncmp( code5, "04", 2 ) == 0 ||	// ‹{éŒ§	10
		strncmp( code5, "05", 2 ) == 0 ||	// H“cŒ§	10
		strncmp( code5, "06", 2 ) == 0 ) {	// RŒ`Œ§	10
		return	( 10 );
	}else
	// 11Œn 140“x15•ª0•b0 44“x0•ª0•b0 ¬’Ms@”ŸŠÙs@ˆÉ’Bs@–k“ls@’_Ux’¡ŠÇ“à‚Ì‚¤‚¿—LìŒS‹y‚Ñˆ¸“cŒS@wRx’¡ŠÇ“à@Œãux’¡ŠÇ“à@“n“‡x’¡ŠÇ“à  
	// 12Œn 142“x15•ª0•b0 44“x0•ª0•b0 D–ys@ˆ®ìs@’t“às@—¯–Gs@”ü‰Ss@—[’£s ŠâŒ©‘òs@“Ï¬–qs@º—–s@m•Ês@–¼Šñs@ˆ°•Ês@Ô•½s@OŠ}s@‘êìs@»ìs@]•Ês çÎs@‰Ìu“às@[ìs@–ä•Ês@•x—Ç–ìs@“o•Ês@Œb’ës@–kL“‡s@Îës@Îëx’¡ŠÇ“à –Ô‘–x’¡ŠÇ“à‚Ì‚¤‚¿–ä•ÊŒS@ãìx’¡ŠÇ“à@@’Jx’¡ŠÇ“à@“ú‚x’¡ŠÇ“à@’_Ux’¡ŠÇ“ài—LìŒS‹y‚Ñˆ¸“cŒS‚ğœ‚­B)@‹ó’mx’¡ŠÇ“à@—¯–Gx’¡ŠÇ“à  
	// 13Œn 144“x15•ª0•b0 44“x0•ª0•b0 –kŒ©s@‘ÑLs@‹ú˜Hs@–Ô‘–s@ªºs@ªºx’¡ŠÇ“à@‹ú˜Hx’¡ŠÇ“à@–Ô‘–x’¡ŠÇ“ài–ä•ÊŒS‚ğœ‚­B)@\Ÿx’¡ŠÇ“à  
	if(	strncmp( code5, "01", 2 ) == 0 ) {
		std::map<std::string, int>::iterator	a_iIndx;
		a_iIndx	= m_mCode_Sys19.find ( code5 );
		if( a_iIndx != m_mCode_Sys19.end() ) {
			return	( (*a_iIndx).second );
		}else {
			return	( -1 );
		}
	}else
	if( strncmp( code5, "47", 2 ) == 0 ) {		// ‰«“êŒ§
		// 15Œn 127“x30•ª0•b0 26“x0•ª0•b0 ‰«“êŒ§‚Ì‚¤‚¿“ŒŒo126“x‚©‚ç“Œ‚Å‚ ‚èA‚©‚Â“ŒŒo130“x‚©‚ç¼‚Å‚ ‚é‹æˆæ
		if( 126.0 <= lon && lon <= 130.0 ) {	// “ŒŒo126“x‚©‚ç“Œ‚Å‚ ‚èA‚©‚Â“ŒŒo130“x‚©‚ç¼
			return	( 15 );
		}else
		// 16Œn 124“x 0•ª0•b0 26“x0•ª0•b0 ‰«“êŒ§‚Ì‚¤‚¿“ŒŒo126“x‚©‚ç¼‚Å‚ ‚é‹æˆæ
		if( lon < 126.0 ) {
			return	( 16 );
		}else
		// 17Œn 131“x 0•ª0•b0 26“x0•ª0•b0 ‰«“êŒ§‚Ì‚¤‚¿“ŒŒo130“x‚©‚ç“Œ‚Å‚ ‚é‹æˆæ
		if( lon > 130.0 ) {
			return	( 17 );
		}
	}
	// ƒGƒ‰[
	return	( -1 );
}

//------------------------------------------------------------
/*
	ƒƒbƒVƒ…ƒR[ƒh‚©‚çŠY“–‚·‚é‚P‚XÀ•WŒn‚Ì”‚Æ”Ô†‚ğ“¾‚é \n
	ŠY“–‚·‚éŒn‚ª‚È‚¢ê‡A”A”Ô†‚Æ‚à‚É0‚ğ•Ô‚·
	@return \‘¢‘Ì sys_num ‚Ö‚Ìƒ|ƒCƒ“ƒ^
*/
//------------------------------------------------------------
sys_num		*crd_cnv::
Sys_Number	(	int		meshcode	)		// ƒƒbƒVƒ…ƒR[ƒh
{
	static	sys_num		sys;

	switch(meshcode){
		case	392725:		//15	“ß”es
			sys.total	= 1;
			sys.sys[0]	= 15;
			return(&sys);
		case	473024:		//2		­™“‡s
		case	473034:		//2		­™“‡s
		case	473163:		//2		‹{ès
		case	473173:		//2		‹{ès
			sys.total	= 1;
			sys.sys[0]	= 2;
			return(&sys);
		case	492906:		//1		’·ès
		case	492907:		//1		’·ès
		case	492916:		//1		’·ès
		case	492917:		//1		’·ès
		case	492955:		//1		²¢•Ûs
		case	492965:		//1		²¢•Ûs
			sys.total	= 1;
			sys.sys[0]	= 1;
			return(&sys);
		case	493015:		//2		ŒF–{s
		case	493062:		//2		²‰ês
		case	493072:		//2		²‰ês
		case	493074:		//2		‹v—¯•Äs
		case	493164:		//2		”ª‘ãs	‘å•ªs
		case	493165:		//2		‘å•ªs
		case	493173:		//2		•Ê•{s
		case	493174:		//2		•Ê•{s	‘å•ªs
		case	503022:		//2		“ß‰Ïì’¬
		case	503023:		//2		t“ús	‘å–ìés	“ß‰Ïì’¬	u–Æ’¬	{Œb’¬
			sys.total	= 1;
			sys.sys[0]	= 2;
			return(&sys);
		case	503032:		//		ŠY“–‚È‚µ
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	503033:		//2		u–Æ’¬	{Œb’¬	””‰®’¬
		case	503065:		//2		…Šª’¬
			sys.total	= 1;
			sys.sys[0]	= 2;
			return(&sys);
		case	503066:		//		ŠY“–‚È‚µ
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	503067:		//3		‰ºŠÖs
		case	503077:		//3		‰ºŠÖs
		case	503171:		//3		‰F•”s
		case	503172:		//3		‰F•”s
			sys.total	= 1;
			sys.sys[0]	= 3;
			return(&sys);
		case	503255:		//4		¼Rs
		case	503256:		//4		¼Rs
		case	503265:		//4		¼Rs
		case	503266:		//4		¼Rs
		case	503324:		//4		‚’ms
		case	503372:		//4		V‹•ls
			sys.total	= 1;
			sys.sys[0]	= 4;
			return(&sys);
		case	513104:		//3		RŒûs	–h•{s
		case	513106:		//3		“¿Rs
		case	513113:		//3		‰F•”s	RŒûs	–h•{s
		case	513123:		//3		RŒûs
			sys.total	= 1;
			sys.sys[0]	= 3;
			return(&sys);
		case	513207:		//4		¡¡s
			sys.total	= 1;
			sys.sys[0]	= 4;
			return(&sys);
		case	513211:		//3		Šâ‘s
		case	513221:		//3		Šâ‘s	˜a–Ø’¬	‘å’|s
		case	513224:		//3		Œàs
		case	513234:		//3		Œàs	â’¬
		case	513242:		//3		“ù“úss
		case	513243:		//3		•{’†’¬	â’¬
		case	513244:		//3		•{’†’¬	ŠC“c’¬	â’¬
			sys.total	= 1;
			sys.sys[0]	= 3;
			return(&sys);
		case	513253:		//		ŠY“–‚È‚µ
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	513254:		//3		“ŒL“‡s	•{’†’¬
			sys.total	= 1;
			sys.sys[0]	= 3;
			return(&sys);
		case	513300:		//4		¡¡s
			sys.total	= 1;
			sys.sys[0]	= 4;
			return(&sys);
		case	513341:		//3		”ö“¹s	•ŸRs
		case	513352:		//3		•ŸRs
		case	513353:		//3		”ö“¹s	•ŸRs
			sys.total	= 1;
			sys.sys[0]	= 3;
			return(&sys);
		case	513365:		//5		‘q•~s
		case	513366:		//5		‰ªRs	‘q•~s
		case	513375:		//5		‘q•~s
		case	513376:		//5		‰ªRs	‘q•~s
		case	513377:		//5		‰ªRs
			sys.total	= 1;
			sys.sys[0]	= 5;
			return(&sys);
		case	513404:		//4		“¿“‡s
		case	513430:		//4		‚¼s
		case	513440:		//4		‚¼s
			sys.total	= 1;
			sys.sys[0]	= 4;
			return(&sys);
		case	513477:		//5		–¾Îs
			sys.total	= 1;
			sys.sys[0]	= 5;
			return(&sys);
		case	513521:		//6		˜a‰ÌRs
		case	513541:		//6		ò“ìs	ã“ìs	“cK’¬				
		case	513542:		//6		ò“ìs	ã“ìs	“cK’¬	ŒFæ’¬	ŠL’Ës	ò²–ìs	
		case	513551:		//6		ò“ìs	ã“ìs	“cK’¬	ŠL’Ës	ò²–ìs		
		case	513552:		//6		’‰‰ª’¬	“cK’¬	Šİ˜a“cs	ŠL’Ës	ò²–ìs		
		case	513553:		//6		’‰‰ª’¬	äs	Šİ˜a“cs	ŠL’Ës	˜aòs		
		case	513554:		//6		äs	•x“c—Ñs	‰Í“à’·–ìs	˜aòs	‘åã‹·Rs		
		case	513562:		//6		äs	Šİ˜a“cs	ò‘å’Ãs	ŠL’Ës	‚Îs	’‰‰ª’¬	
		case	513563:		//6		äs	ò‘å’Ãs	˜aòs	‚Îs	’‰‰ª’¬		
		case	513564:		//6		äs ”ª”ös Q‰®ìs ¼Œ´s ”Œ´s ‰H‰g–ìs “¡ˆä›s ‘åã‹·Rs ”üŒ´’¬
		case	513565:		//6		•x“c—Ñs ”Œ´s ‰H‰g–ìs “¡ˆä›s ‘å˜a‚“cs Ås V¯’¬ ác–ƒ’¬ ã–q’¬ ‰¤›’¬ L—Ë’¬ ‰Í‡’¬
			sys.total	= 1;
			sys.sys[0]	= 6;
			return(&sys);
		case	513570:		//5		–¾Îs						
			sys.total	= 1;
			sys.sys[0]	= 5;
			return(&sys);
		case	513571:		//6		’‰‰ª’¬						
			sys.total	= 1;
			sys.sys[0]	= 6;
			return(&sys);
		case	513573:		//5	6	“òès(5)	äs(6)					
			sys.total	= 2;
			sys.sys[0]	= 5;
			sys.sys[1]	= 6;
			return(&sys);
		case	513574:		//6		äs	”ª”ös	¼Œ´s	”Œ´s	“¡ˆä›s	“Œ‘åãs	
		case	513575:		//6		”ª”ös	”Œ´s	“Œ‘åãs	“Ş—Çs	‘å˜aŒSRs	¶‹îs	”Á”µ’¬	ˆÀ“g’¬	‰¤›’¬	‰Í‡’¬
			sys.total	= 1;
			sys.sys[0]	= 6;
			return(&sys);
		case	523307:		//5		‰ªRs						
		case	523406:		//5		•P˜Hs	–¾Îs	‰ÁŒÃìs	‚»s	”d–’¬
		case	523407:		//5		–¾Îs	‰ÁŒÃìs	ˆî”ü’¬	”d–’¬
		case	523415:		//5		•P˜Hs	‚»s		
		case	523416:		//5		‚»s	•P˜Hs	‰ÁŒÃìs	
		case	523417:		//5		‰ÁŒÃìs	ˆî”ü’¬		
		case	523425:		//5		•P˜Hs			
			sys.total	= 1;
			sys.sys[0]	= 5;
			return(&sys);
		case	523501:		//		ŠY“–‚È‚µ
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	523502:		//5		“òès	¼‹{s	ˆ°‰®s	
		case	523503:		//5		“òès	¼‹{s	–¾Îs	
			sys.total	= 1;
			sys.sys[0]	= 5;
			return(&sys);
		case	523504:		//		ŠY“–‚È‚µ
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	523505:		//5		•P˜Hs			
		case	523506:		//5		•P˜Hs			
		case	523512:		//5		¼‹{s	ˆ°‰®s	ˆÉ’Os	•ó’Ës
		case	523513:		//5		“òès	–¾Îs	¼‹{s	ˆÉ’Os	•ó’Ës	ì¼s
			sys.total	= 1;
			sys.sys[0]	= 5;
			return(&sys);
		case	523514:		//6		–L’†s	“cs	‚’Îs	çŒûs	–‡•ûs	ˆï–Øs	Q‰®ìs	–¥–Ês	Û’Ãs
		case	523515:		//6		‚’Îs	–‡•ûs	Q‰®ìs	Œğ–ìs	¶‹îs	Ås
		case	523523:		//6		’r“cs	ˆï–Øs	–¥–Ês	•ó’Ës	ì¼s
		case	523524:		//6		‚’Îs	–¥–Ês	‚’Îs
		case	523525:		//6		‚’Îs	–‡•ûs	“‡–{’¬	’·‰ª‹s	”ª”¦s	‘åRè’¬	‹vŒäR’¬
		case	523526:		//6		‰F¡s	é—zs	”ª”¦s	‹vŒäR’¬	‘å’Ãs
		case	523535:		//6		Œü“ús	’·‰ª‹s	‚’Îs	“‡–{’¬
		case	523536:		//6		‰F¡s	‘å’Ãs
		case	523537:		//6		‰F¡s	‘å’Ãs	ŒI“Œ’¬
			sys.total	= 1;
			sys.sys[0]	= 6;
			return(&sys);
		case	523545:		//		ŠY“–‚È‚µ
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	523546:		//6		‘å’Ãs	
		case	523547:		//6		‘å’Ãs	çRs	ŒI“Œ’¬	–ìF’¬
		case	523603:		//6		’Ãs	
		case	523604:		//6		’Ãs	
		case	523634:		//6		l“úss	—é­s
		case	523635:		//6		l“úss	ŒK–¼s	’·“‡’¬
		case	523640:		//6		çRs	ŒI“Œ’¬	–ìF’¬
		case	523645:		//6		l“úss	’·“‡’¬
			sys.total	= 1;
			sys.sys[0]	= 6;
			return(&sys);
		case	523656:		//7		µ•ó’¬	‘å¡’¬					
			sys.total	= 1;
			sys.sys[0]	= 7;
			return(&sys);
		case	523657:		//		ŠY“–‚È‚µ
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	523666:		//7		ˆê‹{s ˆî‘òs ¼”ø”f“‡’¬ tŸ’¬ ¼t’¬ t“ú’¬ ´F’¬ Vì’¬ µ•ó’¬ ”ü˜a’¬ r–Ú›’¬ ‘å¡’¬
		case	523667:		//7		t“úˆäs ¼”ø”f“‡’¬ –LR’¬ tŸ’¬ ¼t’¬		
			sys.total	= 1;
			sys.sys[0]	= 7;
			return(&sys);
		case	523671:		//6		•Fªs						
		case	523672:		//6		•Fªs						
			sys.total	= 1;
			sys.sys[0]	= 6;
			return(&sys);
		case	523676:		//7		‰ªès	”ö¼s	ˆî‘òs	Šâ‘qs	tŸ’¬	¼t’¬	–Ø‘]ì’¬	‰H“‡s
		case	523677:		//7		ˆê‹{s	t“úˆäs	¬–qs	Šâ‘qs	–LR’¬	tŸ’¬	¼t’¬
		case	523702:		//7		–L‹´s						
		case	523703:		//7		–L‹´s						
			sys.total	= 1;
			sys.sys[0]	= 7;
			return(&sys);
		case	523705:		//8		•l¼s						
		case	523706:		//8		•l¼s						
			sys.total	= 1;
			sys.sys[0]	= 8;
			return(&sys);
		case	523712:		//7		–L‹´s						
		case	523713:		//7		–L‹´s						
		case	523731:		//7		‰ªès	–L“cs	ˆÀés				
		case	523741:		//7		‰ªès	–L“cs					
		case	523750:		//7		–L“cs	–L–¾s	OD’¬
		case	523751:		//7		–L“cs	OD’¬
		case	523760:		//7		£ŒËs	t“úˆäs	–L“cs	”ö’£ˆ®s
			sys.total	= 1;
			sys.sys[0]	= 7;
			return(&sys);
		case	523832:		//8		Ã‰ªs
		case	523833:		//8		Ã‰ªs	´…s
		case	523837:		//8		À’Ãs	C‘P›’¬
		case	523843:		//8		Ã‰ªs	´…s
		case	523846:		//8		À’Ãs
		case	523847:		//8		À’Ãs	O“‡s	ˆÉ“¤’·‰ª’¬	C‘P›’¬	´…’¬
		case	523855:		//8		À’Ãs	•xms
		case	523856:		//8		À’Ãs	•xms
		case	523857:		//8		À’Ãs	O“‡s	´…’¬
		case	523930:		//8		ˆÉ“Œs	C‘P›’¬
		case	523940:		//8		”MŠCs	ˆÉ“Œs	C‘P›’¬
		case	523950:		//8		”MŠCs	O“‡s
			sys.total	= 1;
			sys.sys[0]	= 8;
			return(&sys);
		case	523954:		//9		O‰Ys
		case	523955:		//9		‰¡{‰ês	O‰Ys	•x’Ãs
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	523960:		//9	8	¬“cŒ´s(9)	” ª’¬(9)	“’‰ÍŒ´’¬(9)	O“‡s(8)
			sys.total	= 2;
			sys.sys[0]	= 8;
			sys.sys[0]	= 9;
			return(&sys);
		case	523961:		//9		¬“cŒ´s	“ñ‹{’¬	“’‰ÍŒ´’¬
		case	523964:		//9		‰¡{‰ês	O‰Ys	—tR’¬
		case	523965:		//9		‰¡{‰ês	O‰Ys	•x’Ãs
		case	523966:		//9		•x’Ãs	‰¡{‰ês
		case	523971:		//9		•½’Ës	¬“cŒ´s	“ì‘«•¿s	“ñ‹{’¬	’†ˆä’¬	ŠJ¬’¬
		case	523972:		//9		•½’Ës	Šƒ–ès	‘åˆé’¬	“ñ‹{’¬	’†ˆä’¬
		case	523973:		//9		•½’Ës	Š™‘qs	“¡‘òs	Šƒ–ès	€qs	—tR’¬
		case	523974:		//9		‰¡{‰ês	Š™‘qs	“¡‘òs	€qs	—tR’¬
		case	523975:		//9		‰¡{‰ês	€qs	—tR’¬	•x’Ãs
		case	523976:		//9		ŒN’Ãs	•x’Ãs	‰¡{‰ês
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	533310:		//3		¼]s
			sys.total	= 1;
			sys.sys[0]	= 3;
			return(&sys);
		case	533411:		//5		’¹æs
		case	533421:		//5		’¹æs
			sys.total	= 1;
			sys.sys[0]	= 5;
			return(&sys);
		case	533605:		//7		Šò•Œs	‰H“‡s	Š}¼’¬	–ö’Ã’¬	‘ƒ“ì’¬
		case	533606:		//7		Šò•Œs	‰H“‡s	Šò“ì’¬	Š}¼’¬	–ö’Ã’¬	ˆê‹{s	–Ø‘]ì’¬
		case	533616:		//7		Šò•Œs		
			sys.total	= 1;
			sys.sys[0]	= 7;
			return(&sys);
		case	533834:		//8		b•{s		
		case	533844:		//8		b•{s		
			sys.total	= 1;
			sys.sys[0]	= 8;
			return(&sys);
		case	533902:		//9		•½’Ës	Šƒ–ès	`–ìs	Œú–Øs	ˆÉ¨Œ´s	ŠC˜V–¼s	Š¦ì’¬	’†ˆä’¬
		case	533903:		//9		•½’Ës	Š™‘qs	“¡‘òs	Šƒ–ès	Œú–Øs	ŠC˜V–¼s	ˆ»£s	Š¦ì’¬
		case	533904:		//9		Š™‘qs	“¡‘òs	
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	533905:		//		ŠY“–‚È‚µ
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	533912:		//9		Œú–Øs	ˆÉ¨Œ´s	ŠC˜V–¼s	ˆ¤ì’¬
		case	533913:		//9		“¡‘òs	‘Š–ÍŒ´s	Œú–Øs	‘å˜as	ŠC˜V–¼s	ÀŠÔs	ˆ»£s
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	533914:		//		ŠY“–‚È‚µ
		case	533915:		//		ŠY“–‚È‚µ
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	533916:		//9		–ØX’Ãs	ŒN’Ãs	
		case	533922:		//9		‘Š–ÍŒ´s	Œú–Øs	ˆ¤ì’¬
		case	533923:		//9		‘Š–ÍŒ´s	Œú–Øs	‘å˜as	ÀŠÔs
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	533924:		//		ŠY“–‚È‚µ
		case	533925:		//		ŠY“–‚È‚µ
		case	533926:		//		ŠY“–‚È‚µ
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	533932:		//9		‘Š–ÍŒ´s
		case	533933:		//9		‘Š–ÍŒ´s
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	533934:		//		ŠY“–‚È‚µ
		case	533935:		//		ŠY“–‚È‚µ
		case	533936:		//		ŠY“–‚È‚µ
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	533937:		//9		sìs	‘D‹´s	Ku–ìs	‰YˆÀs
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	533942:		//		ŠY“–‚È‚µ
		case	533943:		//		ŠY“–‚È‚µ
		case	533944:		//		ŠY“–‚È‚µ
		case	533945:		//		ŠY“–‚È‚µ
		case	533946:		//		ŠY“–‚È‚µ
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	533947:		//9		sìs	‘D‹´s	¼ŒËs	Š™ƒ–’Js	‰YˆÀs
		case	533952:		//9		”Ñ”\s	“üŠÔs
		case	533953:		//9		Š‘òs	‹·Rs	“üŠÔs	O–F’¬
		case	533954:		//9		‰Y˜as	Š‘òs	ŒË“cs	’©‰às	u–Øs	˜aŒõs	VÀs	•xmŒ©s	O–F’¬
		case	533955:		//9		ìŒûs	‰Y˜as	˜ns	ŒË“cs	”µƒ–’Js	’©‰às	˜aŒõs
		case	533956:		//9		ìŒûs	‘‰Ás	”µƒ–’Js	”ª’ªs	O‹½s
		case	533957:		//9		O‹½s	sìs	‘D‹´s	¼ŒËs	”s	—¬Rs	Š™ƒ–’Js	À“ì’¬
		case	533962:		//9		”Ñ”\s	‹·Rs	“üŠÔs	âŒËs	“ú‚s
		case	533963:		//9		ì‰zs	Š‘òs	”Ñ”\s	‹·Rs	“üŠÔs	’ßƒP“‡s	“ú‚s	‘åˆä’¬	O–F’¬
		case	533964:		//9		ì‰zs	‰Y˜as	‘å‹{s	—^–ìs	u–Øs	•xmŒ©s	ã•Ÿ‰ªs	‘åˆä’¬	O–F’¬
		case	533965:		//9		ìŒûs	‰Y˜as	‘å‹{s	Šâ’Îs	—^–ìs	‰z’Js	”µƒ–’Js
		case	533966:		//9		ìŒûs	Šâ’Îs	‘‰Ás	‰z’Js	”ª’ªs	O‹½s	‹gìs	¼•š’¬	–ì“cs
		case	533967:		//9		O‹½s	‹gìs	¼ŒËs	–ì“cs	”s	—¬Rs	‰ä‘·qs	À“ì’¬	æès	ç’J’¬
		case	533973:		//9		ì‰zs	“Œ¼Rs	âŒËs	’ßƒP“‡s	“ú‚s
		case	533974:		//9		ì‰zs	‘å‹{s	ã”ös	‰±ìs	–k–{s	ˆÉ“Ş’¬	ì“‡’¬
		case	533975:		//9		‘å‹{s	Šâ’Îs	t“ú•”s	ã”ös	‰z’Js	˜@“cs	ˆÉ“Ş’¬	‹{‘ã’¬	”’‰ª’¬	™ŒË’¬
		case	533976:		//9		Šâ’Îs	t“ú•”s	‰z’Js	‹gìs	™ŒË’¬	¼•š’¬	¯˜a’¬	–ì“cs
		case	533977:		//9		–ì“cs	”s	…ŠC“¹s	Šâˆäs	’J˜aŒ´‘º	ç’J’¬
		case	534010:		//9		sŒ´s	‘³ƒ–‰Ys			
		case	534020:		//9		sŒ´s	‘³ƒ–‰Ys	
		case	534021:		//9		sŒ´s		
		case	534022:		//9		“Œ‹às	sŒ´s	”ªŠXs	‘å–Ô”’—¢’¬
		case	534030:		//9		‘D‹´s	Ku–ìs	
		case	534031:		//9		²‘qs	lŠX“¹s	
		case	534040:		//9		‘D‹´s	Ku–ìs	”ªç‘ãs	Š™ƒ–’Js
		case	534041:		//9		²‘qs	”ªç‘ãs	lŠX“¹s	ğXˆä’¬	ˆó×‘º
		case	534050:		//9		‘D‹´s	¼ŒËs	”s	”ªç‘ãs	Š™ƒ–’Js	ˆó¼s	À“ì’¬	”’ˆä’¬
		case	534060:		//9		”s	‰ä‘·qs	ˆó¼s	À“ì’¬	æès	ç’J’¬	“¡‘ã’¬
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	543601:		//6		•Ÿˆäs		
			sys.total	= 1;
			sys.sys[0]	= 6;
			return(&sys);
		case	543665:		//7		‹à‘òs		
			sys.total	= 1;
			sys.sys[0]	= 7;
			return(&sys);
		case	543727:		//8		¼–{s		
		case	543737:		//8		¼–{s		
		case	543800:		//8		‰ª’Js	z–Ks	‰ºz–K’¬
		case	543801:		//8		z–Ks	‰ºz–K’¬	
			sys.total	= 1;
			sys.sys[0]	= 8;
			return(&sys);
		case	543837:		//9		‚ès		
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	543845:		//8		Œyˆä‘ò’¬		
		case	543871:		//8		’·–ìs		
			sys.total	= 1;
			sys.sys[0]	= 8;
			return(&sys);
		case	543924:		//9		ŠÙ—Ñs	”Â‘q’¬	–¾˜a‘º	—WŠy’¬	s“cs	‰Á{s	‰H¶s
		case	543930:		//9		‘O‹´s	‚ès	“¡‰ªs	V’¬	‹Ê‘º’¬	ã—¢’¬
		case	543931:		//9		‘O‹´s	ˆÉ¨ès	V’¬	‹«’¬	‹Ê‘º’¬	–{¯s	ã—¢’¬
		case	543932:		//9		ÈÀ’¬	‘¾“cs	‹«’¬	”ö“‡’¬	V“c’¬	–÷’Ë–{’¬
		case	543933:		//9		‘«—˜s	‘¾“cs	ŠÙ—Ñs	‘åò’¬	—WŠy’¬
		case	543934:		//9		ŠÙ—Ñs	”Â‘q’¬	—WŠy’¬	‘«—˜s	²–ìs	“¡‰ª’¬	ŠâM’¬	“cÀ’¬
		case	543936:		//9		Œ‹és	¬Rs	–ì–Ø’¬
		case	543940:		//9		‘O‹´s	‚ès	
		case	543942:		//9		‹Ë¶s	‘¾“cs	Ô–x’¬	V“c’¬	–÷’Ë–{’¬	Š}Œœ’¬
		case	543943:		//9		‹Ë¶s	‘¾“cs	‘«—˜s	“cÀ’¬
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	543966:		//8	9	‰F“s‹{s(8)	­Às(9)	p¶’¬(9)
		case	543967:		//8	9	‰F“s‹{s(8)	^‰ªs(9)	
			sys.total	= 2;
			sys.sys[0]	= 8;
			sys.sys[1]	= 9;
			return(&sys);
		case	544001:		//9		“y‰Ys	‹‹vs	‚Â‚­‚Îs	ˆ¢Œ©’¬	Œsè’¬
		case	544011:		//9		“y‰Ys	Î‰ªs	‚Â‚­‚Îs	ç‘ã“c’¬	V¡‘º
		case	544043:		//9		…ŒËs	‚Ğ‚½‚¿‚È‚©s	ˆïé’¬	“àŒ´’¬
		case	544044:		//9		…ŒËs	‚Ğ‚½‚¿‚È‚©s
		case	544065:		//9		“ú—§s
		case	544075:		//9		“ú—§s
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	553701:		//7		•xRs
			sys.total	= 1;
			sys.sys[0]	= 7;
			return(&sys);
		case	554046:		//9		‚¢‚í‚«s
		case	554047:		//9		‚¢‚í‚«s
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	563816:		//8		’·‰ªs
		case	563817:		//8		’·‰ªs
			sys.total	= 1;
			sys.sys[0]	= 8;
			return(&sys);
		case	563917:		//9		‰ï’Ãá¼s
		case	563927:		//9		‰ï’Ãá¼s
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	563960:		//8		VŠƒs	‹T“c’¬	•é’¬
		case	563970:		//8		VŠƒs
			sys.total	= 1;
			sys.sys[0]	= 8;
			return(&sys);
		case	564002:		//9		ŒSRs
		case	564003:		//9		ŒSRs
		case	564043:		//9		•Ÿ“‡s
		case	564053:		//9		•Ÿ“‡s
			sys.total	= 1;
			sys.sys[0]	= 9;
			return(&sys);
		case	574022:		//10	RŒ`s	ãRs
			sys.total	= 1;
			sys.sys[0]	= 10;
			return(&sys);
		case	574026:		//		ŠY“–‚È‚µ
		case	574027:		//		ŠY“–‚È‚µ
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	574032:		//10	RŒ`s	“V“¶s
			sys.total	= 1;
			sys.sys[0]	= 10;
			return(&sys);
		case	574036:		//		ŠY“–‚È‚µ
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	574037:		//10	‰–Š–s	‘½‰êés	—˜•{’¬
		case	574042:		//10	RŒ`s	“V“¶s
		case	574043:		//10	RŒ`s	“V“¶s
		case	594040:		//10	H“cs
		case	594041:		//10	H“cs
		case	594140:		//10	·‰ªs
		case	594141:		//10	·‰ªs
		case	604073:		//10	O‘Os
		case	604163:		//10	”ªŒËs
		case	604164:		//10	”ªŒËs
		case	614015:		//10	ÂXs
		case	614016:		//10	ÂXs
			sys.total	= 1;
			sys.sys[0]	= 10;
			return(&sys);
		case	624045:		//11	”ŸŠÙs	ãˆé’¬
		case	624055:		//11	6	”ŸŠÙs	ãˆé’¬	‘å–ì’¬(6)
		case	624056:		//11	”ŸŠÙs
		case	644067:		//11	¬’Ms
			sys.total	= 1;
			sys.sys[0]	= 11;
			return(&sys);
		case	644132:		//		ŠY“–‚È‚µ
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	644133:		//12	–kL“‡s
			sys.total	= 1;
			sys.sys[0]	= 12;
			return(&sys);
		case	644142:		//		ŠY“–‚È‚µ
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
		case	644143:		//12	]•Ês	–kL“‡s
		case	644144:		//12	]•Ês	–kL“‡s
			sys.total	= 1;
			sys.sys[0]	= 12;
			return(&sys);
		case	644151:		//11	¬’Ms
		case	644152:		//11	12	¬’Ms(11)	Îës(12)
			sys.total	= 1;
			sys.sys[0]	= 11;
			sys.sys[1]	= 12;
			return(&sys);
		case	644153:		//12	]•Ês
		case	644154:		//12	]•Ês
			sys.total	= 1;
			sys.sys[0]	= 12;
			return(&sys);
		case	644160:		//11	¬’Ms
			sys.total	= 1;
			sys.sys[0]	= 11;
			return(&sys);
		case	654252:		//12	ˆ®ìs
		case	654253:		//12	ˆ®ìs
			sys.total	= 1;
			sys.sys[0]	= 12;
			return(&sys);
		default:
			sys.total	= 0;
			sys.sys[0]	= 0;
			return(&sys);
	}
}

// ‚±‚±‚©‚ç z19_add.cpp
//1999.12.27 ‚±‚±‚Ü‚Å
//------------------------------------------------------------
/*
	ƒƒbƒVƒ…ƒR[ƒh‚©‚çŠY“–‚·‚é‚P‚XÀ•WŒn‚Ì”‚Æ”Ô†‚ğ“¾‚é \n
	ŠY“–‚·‚éŒn‚ª‚È‚¢ê‡A”A”Ô†‚Æ‚à‚É0‚ğ•Ô‚·
*/
//------------------------------------------------------------
void	crd_cnv::
Sys_Number	(	int		meshcode,		// ƒƒbƒVƒ…ƒR[ƒh
				sys_num	*sys)			// Œn”Ô†
{
	*sys	= *Sys_Number(meshcode);	// 2009.05.25 C³
}

//------------------------------------------------------------
/*
	ŒoˆÜ“x‚©‚ç‚P‚XÀ•WŒn‚Ì‚w‚x‚Ö•ÏŠ·(‘È‰~‘Ìî•ñ‚ğw’è)
*/
//------------------------------------------------------------
void	crd_cnv::
LLtoXY19_Core	(	double		Ra,			// ’n‹…‘È‰~‘Ì‚Ì’·”¼Œaiƒ[ƒgƒ‹j
					double		ea2,		// ‘æˆê—£S—¦‚Ì2æ
					double		eb2,		// ‘æ“ñ—£S—¦‚Ì2æ
					int			axis,		// ŠY“–‚·‚éÀ•WŒn‚Ì”Ô†
					double		lon,		// Œo“x
					double		lat,		// ˆÜ“x
					int			mode,		// •ÏŠ·ƒ‚[ƒh(0=“ú–{‘ª’nŒn,1=¢ŠE‘ª’nŒn)
					double		*x,			// ‚P‚XÀ•WŒn‚Ì‚w
					double		*y)			// ‚P‚XÀ•WŒn‚Ì‚x
{

	double			d_lon,	Tau;
	double			Eta2,	Eta4;
	double			N,		Phi;
	double			DPhi,	t;
	double			tx,		ty;

//	Make19tbl();			// 19À•WŒn‚Ìƒe[ƒuƒ‹‚ğì¬

	d_lon	= lon * M_PI / 180.0 - Rad_Origin[axis - 1].lon;
	lat		= lat * M_PI / 180.0;

	Tau		= tan(lat);
	Eta2	= eb2 * pow(cos(lat), 2.0);
	Eta4	= pow(Eta2, 2.0);
	N		= Ra / sqrt(1.0 - ea2 * pow(sin(lat), 2.0));
	Phi		= Rad_Origin[axis - 1].lat;
	DPhi	= lat - Rad_Origin[axis - 1].lat;
	t		= tan(lat);

	double	arclen	= 0.0, arclen_org = 0.0;

	if( mode == 0 ) {
		arclen		= M_Arc_Len(Ra, ea2, lat);
		arclen_org	= M_Arc_Len(Ra, ea2, Rad_Origin[axis - 1].lat);
	}else {
		arclen		= M_Arc_Len_Core(Ra, ea2, lat);
		arclen_org	= M_Arc_Len_Core(Ra, ea2, Rad_Origin[axis - 1].lat);
	}

	tx		= arclen - arclen_org
  			+ N * pow(d_lon, 2.0) * sin(lat) * cos(lat) / 2.0
			* (1.0 + pow(d_lon, 2.0) * pow(cos(lat), 2.0)
			* ((5.0 - pow(t, 2.0) + 9.0 * Eta2 + 4.0 * Eta4) / 12.0
			+ pow(d_lon, 2.0) * pow(cos(lat), 2.0)
			* (61.0 - 58.0 * pow(t, 2.0) + pow(t, 4.0) + 270.0 * Eta2
			- 330.0 * pow(t, 2.0) * Eta2) / 30.0));

	ty		= N * d_lon * cos(lat)
			* (1.0 + pow(d_lon, 2.0) * pow(cos(lat), 2.0)
			* ((1.0 - pow(t, 2.0) + Eta2)
			+ pow(d_lon, 2.0) * pow(cos(lat), 2.0)
			* (5.0 - 18.0 * pow(t, 2.0) + pow(t, 4.0)
			+ 14.0 * Eta2 -58.0 * pow(t, 2.0) * Eta2) / 20.0) / 6.0);

	*x	= tx * M0;
	*y	= ty * M0;
}

//------------------------------------------------------------
/*
	ŒoˆÜ“x‚©‚ç‚P‚XÀ•WŒn‚Ì‚w‚x‚Ö•ÏŠ·
	@return	\‘¢‘Ì xy_coord ‚Ö‚Ìƒ|ƒCƒ“ƒ^
*/
//------------------------------------------------------------
xy_coord*	crd_cnv::
LLtoXY19	(	int		axis,		// ŠY“–‚·‚éÀ•WŒn‚Ì”Ô†
				double	lon,		// Œo“x
				double	lat)		// ˆÜ“x
{
	static xy_coord	coord;

	LLtoXY19 ( axis, lon, lat, &coord.x, &coord.y );

	return(&coord);
}

//------------------------------------------------------------
/*
	ŒoˆÜ“x‚©‚ç‚P‚XÀ•WŒn‚Ì‚w‚x‚Ö•ÏŠ·
	@return	\‘¢‘Ì coord19 ‚Ö‚Ìƒ|ƒCƒ“ƒ^
*/
//------------------------------------------------------------
coord19*	crd_cnv::
LLtoXY19	(	double	lon,		// Œo“x
				double	lat)		// ˆÜ“x
{
	static	coord19		sys_xy;	
	int					sys		= 0;

	sys			= Sys_Number(lon, lat);		
	sys_xy.sys	= sys;

	LLtoXY19 ( sys, lon, lat, &sys_xy.x, &sys_xy.y );

	return(&sys_xy);
}

//------------------------------------------------------------
/*
	ŒoˆÜ“x‚©‚ç‚P‚XÀ•WŒn‚Ì‚w‚x‚Ö•ÏŠ·
*/
//------------------------------------------------------------
void	crd_cnv::
LLtoXY19	(	int			axis,		// ŠY“–‚·‚éÀ•WŒn‚Ì”Ô†
				double		lon,		// Œo“x
				double		lat,		// ˆÜ“x
				xy_coord	*coord)		// 19À•WŒn‚Ì‚w‚x
{
	LLtoXY19 ( axis, lon, lat, &coord->x, &coord->y );
}

//------------------------------------------------------------
/*
	ŒoˆÜ“x‚©‚ç‚P‚XÀ•WŒn‚Ì‚w‚x‚Ö•ÏŠ·		1998.7.14
*/
//------------------------------------------------------------
void	crd_cnv::
LLtoXY19	(	double		lon,			// Œo“x
				double		lat,			// ˆÜ“x
				coord19		*sys_xy)		// 19À•WŒn‚ÌŒn‚Æ‚w‚x‚ğ•\‚·
{
	int			sys		= 0;

	sys			= Sys_Number(lon, lat);		
	sys_xy->sys = sys;
	LLtoXY19 ( sys, lon, lat, &sys_xy->x, &sys_xy->y );
}

//------------------------------------------------------------
/*
	ŒoˆÜ“x‚©‚ç‚P‚XÀ•WŒn‚Ì‚w‚x‚Ö•ÏŠ·		2000.1.12
*/
//------------------------------------------------------------
void	crd_cnv::
LLtoXY19	(	int			axis,		// ŠY“–‚·‚éÀ•WŒn‚Ì”Ô†
				double		lon,		// Œo“x
				double		lat,		// ˆÜ“x
				double		*x,			// ‚P‚XÀ•WŒn‚Ì‚w
				double		*y)			// ‚P‚XÀ•WŒn‚Ì‚x
{
	LLtoXY19_Core ( RA, EA2, EB2, axis, lon, lat, 0, x, y );
}

//------------------------------------------------------------
/*
	ŒoˆÜ“x‚©‚ç‚P‚XÀ•WŒn‚Ì‚w‚x‚Ö•ÏŠ·i¢ŠE‘ª’nŒn”Åj
	@return	\‘¢‘Ì xy_coord ‚Ö‚Ìƒ|ƒCƒ“ƒ^
*/
//------------------------------------------------------------
void	crd_cnv::
LLtoXY19_JGD2000	(	int		axis,		// ŠY“–‚·‚éÀ•WŒn‚Ì”Ô†
						double	lon,		// Œo“x
						double	lat,		// ˆÜ“x
						double	*x,			// ‚P‚XÀ•WŒn‚Ì‚w
						double	*y)			// ‚P‚XÀ•WŒn‚Ì‚x
{
	LLtoXY19_Core ( GRS80_RA, GRS80_EA2, GRS80_EB2, axis, lon, lat, 1, x, y );
}

//------------------------------------------------------------
/*
	‚P‚XÀ•WŒn‚Ì‚w‚x‚©‚çŒoˆÜ“x‚Ö•ÏŠ·(‘È‰~‘Ìî•ñ‚ğw’è)
*/
//------------------------------------------------------------
void 	crd_cnv::
XY19toLL_Core	(	double		Ra,		// ’n‹…‘È‰~‘Ì‚Ì’·”¼Œaiƒ[ƒgƒ‹j
					double		ea2,	// ‘æˆê—£S—¦‚Ì2æ
					double		eb2,	// ‘æ“ñ—£S—¦‚Ì2æ
					double		f,		// ‘È‰~‘Ì‚ÌG•½—¦
					int			axis,	// ŠY“–‚·‚éÀ•WŒn‚Ì”Ô†
					double		x,		// –k•ûŒü‚ÌÀ•W’l
					double		y,		// “Œ•ûŒü‚ÌÀ•W’l
					int			mode,	// •ÏŠ·ƒ‚[ƒh(0=“ú–{‘ª’nŒn,1=¢ŠE‘ª’nŒn)
					double		*lon,	// Œo“x
					double		*lat)	// ˆÜ“x
{
	double	Rad_Phi,	Rad_Ramda0,	Rad_Ramda;
	double	Rad_Phi1,	Rad_Phi0;
	double	Rad_Phi_n,	Rad_Phi_c,	Gunma2;
	double	t1,	a1,	p;

//	Make19tbl();	// 19À•WŒn‚Ìƒe[ƒuƒ‹‚ğì¬

	// Œ´“_ŒoˆÜ“x‚Ìæ‚èo‚µ
	Rad_Phi0		= Rad_Origin[axis - 1].lat;

	Rad_Ramda0		= Rad_Origin[axis - 1].lon;

	Rad_Phi_c		= Rad_Phi0;

	double	arc_len1 = 0.0, arc_len2 = 0.0;

	// ‚‘«ˆÜ“x‚ğ‹‚ß‚é
	while(1)
	{
		if( mode == 0 ) {
			arc_len1	= M_Arc_Len(Ra, ea2, Rad_Phi_c);
			arc_len2	= M_Arc_Len(Ra, ea2, Rad_Phi0);
		}else {
			arc_len1	= M_Arc_Len_Core(Ra, ea2, Rad_Phi_c);
			arc_len2	= M_Arc_Len_Core(Ra, ea2, Rad_Phi0);
		}
		Rad_Phi_n	= Rad_Phi_c
					- ((arc_len1 - arc_len2 - (x / M0)) / Ra * (1.0 - ea2))
					* pow((1.0 - ea2 * pow(sin(Rad_Phi_c), 2.0)), 1.5);

		if(fabs(Rad_Phi_n - Rad_Phi_c) < 1.0e-12)
		{
			break;
		}
		Rad_Phi_c	= Rad_Phi_n;
	}
	Rad_Phi1	= Rad_Phi_n;

	t1		= tan(Rad_Phi1);
	Gunma2	= eb2 * pow(cos(Rad_Phi1), 2.0);
	a1		= Ra / sqrt(1.0 - ea2 * pow(sin(Rad_Phi1), 2.0));
	p		= y / M0;

	double p2, p4, t2, t4, a2, a4;
	p2		= pow(p,	2.0);
	p4		= pow(p,	4.0);
	t2		= pow(t1,	2.0);
	t4		= pow(t1,	4.0);
	a2		= pow(a1,	2.0);
	a4		= pow(a1,	4.0);

	Rad_Phi		= Rad_Phi1
				- (p2 * (1.0 + Gunma2) * t1) / (2.0 * a2)
				* (1.0 - p2 / (12.0 * a2)
				* (5.0 + 3.0 * t2 + Gunma2 - 9.0 * t2 * Gunma2)
				+ p4 / (360.0 * a4) 
				* (61.0 + 90.0 * t2 + 45.0 * t4));

	Rad_Ramda	= Rad_Ramda0
				+ p / (a1 * cos(Rad_Phi1))
				* (1.0 - p2 / (6.0 * a2)
				* (1.0 + 2.0 * t2 + Gunma2)
				+ p4 / (120.0 * a4)
				* (5.0 + 28.0 * t2 + 24.0 * t4));

	*lon	= Rad_Ramda	/ (M_PI/180.0);
	*lat	= Rad_Phi	/ (M_PI/180.0);
}

//------------------------------------------------------------
/*
	‚P‚XÀ•WŒn‚Ì‚w‚x‚©‚çŒoˆÜ“x‚Ö•ÏŠ·
	@return	\‘¢‘Ì d_lonlat ‚Ö‚Ìƒ|ƒCƒ“ƒ^
*/
//------------------------------------------------------------
d_lonlat*	crd_cnv::
XY19toLL	(	int		axis,		// ŠY“–‚·‚éÀ•WŒn‚Ì”Ô†
				double	x,			// –k•ûŒü‚ÌÀ•W’l
				double	y)			// “Œ•ûŒü‚ÌÀ•W’l
{
	static	d_lonlat	lonlat;

	XY19toLL ( axis, x, y, &lonlat.lon, &lonlat.lat );

	return(&lonlat);
}

//------------------------------------------------------------
/*
	‚P‚XÀ•WŒn‚Ì‚w‚x‚©‚çŒoˆÜ“x‚Ö•ÏŠ·
*/
//------------------------------------------------------------
void 	crd_cnv::
XY19toLL	(	int			axis,		// ŠY“–‚·‚éÀ•WŒn‚Ì”Ô†
				double		x,			// –k•ûŒü‚ÌÀ•W’l
				double		y,			// “Œ•ûŒü‚ÌÀ•W’l
				d_lonlat	*lonlat)	// 10i‚Å•\‚µ‚½ŒoˆÜ“x(ex:130.23432342....)
{
	XY19toLL ( axis, x, y, &lonlat->lon, &lonlat->lat );
}

//------------------------------------------------------------
/*
	‚P‚XÀ•WŒn‚Ì‚w‚x‚©‚çŒoˆÜ“x‚Ö•ÏŠ·
*/
//------------------------------------------------------------
void 	crd_cnv::
XY19toLL	(	int			axis,	// ŠY“–‚·‚éÀ•WŒn‚Ì”Ô†
				double		x,		// –k•ûŒü‚ÌÀ•W’l
				double		y,		// “Œ•ûŒü‚ÌÀ•W’l
				double		*lon,	// Œo“x
				double		*lat)	// ˆÜ“x
{
	double	f = 1.0 / BESSEL_INV_F;	// ‘È‰~‘Ì‚ÌG•½—¦

	XY19toLL_Core (	RA, EA2, EB2, f, axis, x, y, 0, lon, lat );
}

//------------------------------------------------------------
/*
	‚P‚XÀ•WŒn‚Ì‚w‚x‚©‚çŒoˆÜ“x‚Ö•ÏŠ·i¢ŠE‘ª’nŒn”Åj
	@return	\‘¢‘Ì d_lonlat ‚Ö‚Ìƒ|ƒCƒ“ƒ^
*/
//------------------------------------------------------------
void	crd_cnv::
XY19toLL_JGD2000	(	int		axis,		// ŠY“–‚·‚éÀ•WŒn‚Ì”Ô†
						double	x,			// –k•ûŒü‚ÌÀ•W’l
						double	y,			// “Œ•ûŒü‚ÌÀ•W’l
						double	*lon,		// Œo“x
						double	*lat	)	// ˆÜ“x
{
	double	f = 1.0 / GRS80_INV_F;	// ‘È‰~‘Ì‚ÌG•½—¦

	XY19toLL_Core (	GRS80_RA, GRS80_EA2, GRS80_EB2, f, axis, x, y, 1, lon, lat );
}

//------------------------------------------------------------
/*
	‚P‚XÀ•WŒn‚Ì(X,Y)‚©‚çAƒƒbƒVƒ…ƒR[ƒh + (x,y) ‚ğ‹‚ß‚é
	@retval		0	¬Œ÷
	@retval		-1	¸”s
*/
//------------------------------------------------------------
int		crd_cnv::
XY19toMesh	(	int		sys,		// ‚P‚XÀ•WŒn‚ÌŒn”Ô†
				double	x_19,		// ‚P‚XÀ•WŒn‚Ì‚wiˆÜ“x•ûŒüj
				double	y_19,		// ‚P‚XÀ•WŒn‚Ì‚xiŒo“x•ûŒüj
				int		level,		// ƒƒbƒVƒ…‚ÌƒŒƒxƒ‹ (1,2,64)
				int		*meshcode,	// ƒƒbƒVƒ…ƒR[ƒh
				int		*x,			// ³‹K‰»‚w
				int		*y)			// ³‹K‰»‚x
{
	double	lat	= 0.0,	lon	= 0.0;

	XY19toLL(sys, x_19, y_19, &lon, &lat);
	if(LLtoMesh(lon, lat, level, meshcode, x, y) == 0)
	{
		return(0);
	}
	else
	{
		return(-1);
	}
}

//------------------------------------------------------------
/*
	‚P‚XÀ•WŒn‚Ì(X,Y)‚©‚çAƒƒbƒVƒ…ƒR[ƒh + (x,y) ‚ğ‹‚ß‚é
*/
//------------------------------------------------------------
void	crd_cnv::
MeshtoXY19	(	int		sys,		// ‚P‚XÀ•WŒn‚ÌŒn”Ô†
				int		meshcode,	// ƒƒbƒVƒ…ƒR[ƒh
				int		x,			// ³‹K‰»‚w
				int		y,			// ³‹K‰»‚x
				double	*x_19,		// ‚P‚XÀ•WŒn‚Ì‚wiˆÜ“x•ûŒüj
				double	*y_19)		// ‚P‚XÀ•WŒn‚Ì‚xiŒo“x•ûŒüj
{
	double	lon	= 0.0,	lat	= 0.0;

	MeshtoLL(meshcode, x, y, &lon, &lat);
	LLtoXY19(sys, lon, lat, x_19, y_19); 
}
