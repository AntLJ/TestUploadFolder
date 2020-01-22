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
#include "addr_db_handle.h"
#include "addr_str_work.h"

/**
	�Z��DB�ւ̐ڑ�
*/
bool	addr_db_handle::
Init_DB ( char*	c_cpConnectStr, 	///< DB�ڑ�������(<���[�U��>/<�p�X���[�h>@<�T�[�r�X��>)
		  FILE* c_fpErr_Fp	)
{
	if( c_cpConnectStr == NULL ) {
		return	( false );
	}

	// �G���[�o�͏�����
	if( c_fpErr_Fp != NULL ) {
		m_fpErr_Fp	= c_fpErr_Fp;
	}

	// �I���N��DB�ǂݍ��݃��[�h
	m_iMode	=	READ_ODB;

	/// �N���X���C�u����������
	OStartup();	

	char*	a_sTmpStr	= NULL;
	a_sTmpStr	= new char[strlen(c_cpConnectStr)+1];
	strcpy	( a_sTmpStr, c_cpConnectStr );

	string	a_sUser_Name;	///< ���[�U��
	string	a_sPasswd;		///< �ڑ��p�X���[�h
	string	a_sService_Name;///< �T�[�r�X��

	char*	a_cpCurPt	= NULL;
	a_cpCurPt	= strtok( a_sTmpStr, "/" );
	if( a_cpCurPt != NULL ) {	// ���[�U��
		a_sUser_Name	= a_cpCurPt;
	}
	a_cpCurPt	= strtok( NULL, "@" );
	if( a_cpCurPt != NULL ) {	// �ڑ��p�X���[�h
		a_sPasswd		= a_cpCurPt;
	}
	a_cpCurPt	= strtok( NULL, "" );
	if( a_cpCurPt != NULL ) {	// �T�[�r�X��
		a_sService_Name	= a_cpCurPt;	
	}

	/// �ڑ��̐ݒ�
	if( m_oAdm_DB.Open ( a_sService_Name.c_str(), a_sUser_Name.c_str(), a_sPasswd.c_str() ) != OSUCCESS ) {	// �ڑ����s
		return	( false );
	}

	// DB�ł̍�Ƃ����s�\
	m_bExec_OK	= true;

	return	( true );
}

/**
	�N���X�̏�����
*/
bool	addr_db_handle::
Init_TextDB	( char*		c_cpDBPath,		///< �e�L�X�gDB�p�X��
	 		  FILE*		c_fpErr_Fp	)
{
    bool	iRet	= false;

	m_iMode	=	READ_TDB;

	// �G���[�o�͏�����
	if( c_fpErr_Fp != NULL ) {
		m_fpErr_Fp	= c_fpErr_Fp;
	}

    if(_access ( c_cpDBPath, 0 ) == 0){	// �e�L�X�gDB�p�X�����݂����ꍇ
        m_sDBPath	= c_cpDBPath;
        iRet = true;
		// ��Ƃ����s�\
		m_bExec_OK	= true;

    }else{
        fprintf	( m_fpErr_Fp, "AddressData may not exist. Check DataPath:%s\n", c_cpDBPath );
    }
    return ( iRet );
}

/**
	�s���R�[�h�̐������`�F�b�N
	@return RET_OK		�������R�[�h
	@return RET_HAISHI	�p�~�ɂȂ����R�[�h
	@return RET_000		000����
*/
int addr_db_handle::
IsValidCode11	(	const char	*Code,
					char		*AreaName	)
{
    int					iRet;
    const char			*p = Code;
    char				City[12];
    m_string::iterator	it;
    
	// 5���R�[�h���o��
    strcpy(City, Code);
    City[5] = '\0';
	// ��5���œǂݍ��ݍς݃��X�g���T�[�`
    if(m_Ready.find(City) == m_Ready.end()) {	
        LoadAddress(Code);			// �e�L�X�g�t�@�C������f�[�^�����[�h����
		m_Ready.insert(City);		// �ǂݍ��ݍς݃��X�g�֒ǉ�
    }
    
	// 11���R�[�h�Ńf�[�^���T�[�`
    if((it = m_Map.find(Code)) == m_Map.end()){
        string Comp = Code;
        Comp += '1';
        if((it = m_Map.find(Comp)) == m_Map.end()){
            Comp = Code;
            Comp += '2';
            if((it = m_Map.find(Comp)) == m_Map.end()){
                AreaName[0] = '\0';
                iRet = 0;  //Not Found
            }else{
                iRet = RET_HAISHI;  //Not Use
            }
        }else{
            iRet = RET_000; //Attn
        }
    }else{	/// �T�[�`�����f�[�^����������
        iRet = RET_OK;
    }
    if(iRet){
        FormatStr(AreaName, ((*it).second).data());
    }
    return iRet;
}

/**
	�s���R�[�h�̐������`�F�b�N�i5���p�j
	@return RET_OK		�������R�[�h
	@return RET_HAISHI	�p�~�ɂȂ����R�[�h
	@return RET_000		000����
*/
int addr_db_handle::
IsValidCode5	( const char	*Code )
{
    int					iRet;
    const char			*p = Code;
    char				City[12];
    m_string2::iterator	it;
    
	// 5���R�[�h���o��
    strcpy(City, Code);
    City[5] = '\0';
	// ��5���œǂݍ��ݍς݃��X�g���T�[�`
    if(m_Ready.find(City) == m_Ready.end()){
        m_Ready.insert(City);		// �ǂݍ��ݍς݃��X�g�֒ǉ�
		if(!LoadAddress(Code)) {	// �e�L�X�g�t�@�C������f�[�^�����[�h����
            return	0;
		}
    }
    
	// 11���R�[�h�Ńf�[�^���T�[�`
    if((it = m_City.find(City)) != m_City.end()){
		iRet	= (*it).second;
	}else {
		iRet	= 0;
	}
    return iRet;
}

/**
	OracleDB���璼�Ŏ���Ă�����������擾
*/
bool addr_db_handle::
GetOraDBStr	(	const char	*c_cpCode,
				addr_dat&	c_aDat	)
{
    bool				iRet;
    const char			*p = c_cpCode;
    char				City[12];
    m_string::iterator	it;

	memset	( City, '\0', 12 );
    
	// 5���R�[�h���o��
    strcpy(City, c_cpCode);
    City[5] = '\0';
	// ��5���œǂݍ��ݍς݃��X�g���T�[�`
    if(m_Ready.find(City) == m_Ready.end()) {	
        LoadAddress(c_cpCode);			// �e�L�X�g�t�@�C������f�[�^�����[�h����
		m_Ready.insert(City);			// �ǂݍ��ݍς݃��X�g�֒ǉ�
    }
    
	// 11���R�[�h�Ńf�[�^���T�[�`
	is_addr_dat		a_iIndx;
	addr_dat		a_aSKey;
	a_aSKey.e_sCode	= c_cpCode;
//	a_aSKey.e_sName	= "";
	a_iIndx	= m_sDatas.find ( a_aSKey );
	if( a_iIndx != m_sDatas.end() ) {
		c_aDat	= *a_iIndx;
		iRet			= true;
	}else {
		iRet			= false;
	}
    return iRet;
}

/**
	�於����s��5�����̏����擾����
*/
bool addr_db_handle::
GetCitybyWard(	addr_dat&	c_aInputDat,	///< ���͏��[IN]
				addr_dat&	c_aRetDat	)	///< �ԋp���[IN]
{
	// ���ʋ�Ή��p "xx�s�@oo��"�̏ꍇ�́Axx�s��5�����擾����
	long	a_lPos	= 0;
	a_lPos	= c_aInputDat.e_sShi_Kanji.find ( "�s�@" );
	if( a_lPos != c_aInputDat.e_sShi_Kanji.npos ) {	// �S�p�X�y�[�X�Ō���
		long	a_lLen	= 0;
		a_lLen	= c_aInputDat.e_sShi_Kanji.length();
		// �Ō��1���������o����r
		if(a_lLen - 2 > 0) {
			if( c_aInputDat.e_sShi_Kanji.substr(a_lLen-2,2) == "��") {
				// "�s"�ŕ��������؂�
				string	a_sShi;
				a_sShi	= c_aInputDat.e_sShi_Kanji.substr ( 0, a_lPos+2 );
				
				string	a_sKey;
				a_sKey	= c_aInputDat.e_sKen_Kanji;
				a_sKey	+=a_sShi;
				im_addr_dat	a_iIndx;
				a_iIndx	= m_sRevData.find(a_sKey);
				if( a_iIndx != m_sRevData.end()){
					c_aRetDat	= (*a_iIndx).second;
					return	( true );
				}
			}
		}
	}
	c_aRetDat	= c_aInputDat;
	return	( false );
}

void addr_db_handle::
FormatStr (	char		*Addr,
			const char	*Src )
{
    const char *p = Src;
    int i;
    for(i = 0; *p != '\t';i++){
        Addr[i] = *p;
        p++;
    }
    Addr[i] = ' ';
    
    for(p++, i++; *p != '\t';i++){
        Addr[i] = *p;
        p++;
    }
    Addr[i] = ' ';

    for(p++, i++; *p != '\t';i++){
        Addr[i] = *p;
        p++;
    }
    Addr[i] = ' ';

    for(p++, i++; *p != '\n';i++){
        Addr[i] = *p;
        p++;
    }
    Addr[i] = '\0';
}

/**
	�Z���f�[�^�̓ǂݍ���
*/
bool	addr_db_handle::
LoadAddress	( const char *c_cpCode )		///< �s���R�[�h
{
	if( m_iMode	== READ_ODB ) {
		if( !LoadAddress_DB ( c_cpCode ) ) {
			return	( false );
		}
	//	if( !LoadOldAddress_DB( c_cpCode ) ) {
	//		return	( false );
	//	}
	}else
	if( m_iMode == READ_TDB ) {
		if( !LoadAddress_TextDB ( c_cpCode ) ) {
			return	( false );
		}
	}
	return	( true );
}

/**
	�e�L�X�gDB�̓ǂݍ���
*/
bool addr_db_handle::
LoadAddress_TextDB( const char *c_cpCode )	///< �s���R�[�h
{
    static	char Path[256];
    static	char Buf[128];
    FILE	*fp;
    bool	iRet = true;
    string	ElvCode;
	string	FiveCode;	// 5���R�[�h�p
    string	str;
 
    pair<int, string> data;
	int	Status	= 2;	// 5���R�[�h�p(Not Use)
    sprintf(Path, "%s/%.2s/%.5s.txt", &m_sDBPath[0], c_cpCode, c_cpCode );
    if((fp = fopen(Path, "rt")) != NULL){
        while(fgets(Buf, 128, fp) != NULL){
            Buf[11] = '\0';
            ElvCode = Buf;
            if(Buf[12] != '0'){
                ElvCode += Buf[12];
            }else {
				Status	= 1;	// 5���R�[�h�p
			}
            str = &(Buf[14]);
            m_Map.insert(m_string::value_type(ElvCode, str));
        }
        fclose(fp);
		char	Tmp[8];
		strncpy	(Tmp, c_cpCode, 5);
		Tmp[5] = '\0';
		FiveCode = Tmp;
		m_City.insert(m_string2::value_type(FiveCode, Status));
    }else{
        iRet = false;
    }
    return iRet;
}

/**
	��ɑΉ�����s��5���R�[�h�̏����擾����
*/
bool	addr_db_handle::
GetShiKuInfo(	ODatabase&	c_oAdm_DB,	///< �Z��DB[IN]
				string&		c_sKen,		///< �s���{����[IN]
				string&		c_sShi,		///< �s�撬����[IN]
				addr_dat&	c_aDat	)	///< �n���f�[�^[OUT]
{
	//--------------------------------------------------------------------
	///	SQL��
	char	addsql[128];
	memset( addsql , '\0' , 128 );
	
	string	a_sKuSql;

	a_sKuSql	+= "SELECT "
					"KEN_CODE,"
					"SHI_CODE,"
					"SHI_KANJI "
				"from "
					"AREACODE_MASTER "
				"where ";

	sprintf( addsql ,	"KEN_KANJI='%s' AND SHI_KANJI='%s' AND HAISHI_DATE='000000'", c_sKen.c_str(), c_sShi.c_str() );	// 2009.05.22 �C��

	a_sKuSql += addsql;

	/// SQL���Ńf�[�^���o��
	ODynaset odyn_for_Ku( m_oAdm_DB , (char*)a_sKuSql.c_str() );
	char	a_ken_code_tmp[3];
	char	a_shi_code_tmp[4];

	char	a_ken_str[9];
	char	a_shi_str[25];

	addr_str_work	a_aASWork;

	addr_dat		a_iTmpAddr;	// ���ʋ�̑Ή��p
	bool			a_iRet	= true;
	while (!odyn_for_Ku.IsEOF()){
		memset	( a_ken_code_tmp, '\0' , 3 );
		memset	( a_shi_code_tmp, '\0' , 4 );

		memset	( a_shi_str, '\0' , 25 );

		if( odyn_for_Ku.GetFieldValue	( "KEN_CODE", a_ken_code_tmp, 3 ) != OSUCCESS ) {
			fprintf	( m_fpErr_Fp, "�Z��DB�t�B�[���h�̃f�[�^�̎擾�Ɏ��s\n");
			a_iRet	= false;
			break;
		}
		if( odyn_for_Ku.GetFieldValue	( "SHI_CODE", a_shi_code_tmp, 4 ) != OSUCCESS ) {
			fprintf	( m_fpErr_Fp, "�Z��DB�t�B�[���h�̃f�[�^�̎擾�Ɏ��s\n");
			a_iRet	= false;
			break;
		}
		if( odyn_for_Ku.GetFieldValue	( "SHI_KANJI", a_shi_str, 25 ) != OSUCCESS ) {
			fprintf	( m_fpErr_Fp, "�Z��DB�t�B�[���h�̃f�[�^�̎擾�Ɏ��s\n");
			a_iRet	= false;
			break;
		}

		addr_dat			a_iAdd;
		a_iAdd.e_sCode		= a_ken_code_tmp;
		a_iAdd.e_sCode		+=a_shi_code_tmp;
		a_iAdd.e_sKen_Kanji	= c_sKen;
		a_iAdd.e_sShi_Kanji	= c_sShi;

		c_aDat	= a_iAdd;
		break;	// ��񂾂��ŏ[���Ȃ̂Ŕ�����
	//	odyn_for_Ku.MoveNext();
	}
	return	( a_iRet );
	//--------------------------------------------------------------------
}

bool addr_db_handle::
LoadAddress_DB( const char *c_cpCode )	///< �s���R�[�h
{

	// ���ǃf�[�^�̊m�F
	is_addr_dat		a_iIndx;
	addr_dat		a_aSKey;	// �T�[�`�L�[

	a_aSKey.e_sCode	= c_cpCode;
	a_iIndx	= m_sDatas.find ( a_aSKey );
	if( a_iIndx != m_sDatas.end() ) {	// ���ɓǂݍ��񂾃f�[�^
		return	( true );
	}

	// �����R�[�h�̌����`�F�b�N
	size_t	a_sCodeLen	= 0;
	a_sCodeLen	= strlen (c_cpCode);
	if( a_sCodeLen != 5 && a_sCodeLen != 11 ) {
		return	( false );
	}

	// �ԍ₳��̃\�[�X���Q�l��
	char	AdrFive[6];
	char	KenCode[3];
	char	ShiCode[4];
//	char	OazaCode[4];

	memcpy( AdrFive  , c_cpCode , 5 );		AdrFive[5] = '\0';
	memcpy( KenCode  , c_cpCode , 2 );		KenCode[2] = '\0';
	memcpy( ShiCode  , c_cpCode+2 , 3 );	ShiCode[3] = '\0';
//	memcpy( OazaCode , c_cpCode+5 , 3 );	OazaCode[3]= '\0';

	///	SQL��
	char	tmpsql[1148];
	memset( tmpsql , '\0' , 1148 );
	
	string	a_sSql;

	a_sSql	+= "SELECT "
					"KEN_CODE,"
					"SHI_CODE,"
					"OAZA_CODE,"
					"AZA_CODE,"
					"KEN_KANJI,"
					"SHI_KANJI,"
					"OAZA_KANJI,"
					"AZA_KANJI "
				"from "
					"AREACODE_MASTER "
				"where ";

	sprintf( tmpsql ,	"KEN_CODE='%s' and SHI_CODE='%s'", KenCode, ShiCode );

	a_sSql += tmpsql;

	/// SQL���Ńf�[�^���o��
	addr_dat		a_iTmpAddr;	// ���ʋ�̑Ή��p
	
	ODynaset odyn( m_oAdm_DB , (char*)a_sSql.c_str() );
	char	a_ken_code_tmp[3];
	char	a_shi_code_tmp[4];
	char	a_oaza_code_tmp[4];
	char	a_aza_code_tmp[4];

	char	a_ken_str[9];
	char	a_shi_str[25];
	char	a_oaza_str[37];
	char	a_aza_str[25];
	char	a_cpCode11[12];
//	char	a_cpChimei[62];
	string	addr_str;

	addr_str_work	a_aASWork;

	int	a_iCount	= 0;
	while (!odyn.IsEOF()){
		memset	( a_ken_code_tmp, '\0' , 3 );
		memset	( a_shi_code_tmp, '\0' , 4 );
		memset	( a_oaza_code_tmp, '\0' , 4 );
		memset	( a_aza_code_tmp, '\0' , 4 );

		memset	( a_ken_str, '\0' , 9 );
		memset	( a_shi_str, '\0' , 25 );
		memset	( a_oaza_str, '\0' , 37 );
		memset	( a_aza_str, '\0' , 25 );
		memset	( a_cpCode11, '\0' , 12 );
	//	memset	( a_cpChimei, '\0' , 62 );

		if( odyn.GetFieldValue	( "KEN_CODE", a_ken_code_tmp, 3 ) != OSUCCESS ) {
			fprintf	( m_fpErr_Fp, "�Z��DB�t�B�[���h�̃f�[�^�̎擾�Ɏ��s\n");
			break;
		}
		if( odyn.GetFieldValue	( "SHI_CODE", a_shi_code_tmp, 4 ) != OSUCCESS ) {
			fprintf	( m_fpErr_Fp, "�Z��DB�t�B�[���h�̃f�[�^�̎擾�Ɏ��s\n");
			break;
		}
		if( odyn.GetFieldValue	( "OAZA_CODE", a_oaza_code_tmp, 4 ) != OSUCCESS ) {
			fprintf	( m_fpErr_Fp, "�Z��DB�t�B�[���h�̃f�[�^�̎擾�Ɏ��s\n");
			break;
		}
		if( odyn.GetFieldValue	( "AZA_CODE", a_aza_code_tmp, 4 ) != OSUCCESS ) {
			fprintf	( m_fpErr_Fp, "�Z��DB�t�B�[���h�̃f�[�^�̎擾�Ɏ��s\n");
			break;
		}
		if( odyn.GetFieldValue	( "KEN_KANJI", a_ken_str, 9 ) != OSUCCESS ) {
			fprintf	( m_fpErr_Fp, "�Z��DB�t�B�[���h�̃f�[�^�̎擾�Ɏ��s\n");
			break;
		}
		if( odyn.GetFieldValue	( "SHI_KANJI", a_shi_str, 25 ) != OSUCCESS ) {
			fprintf	( m_fpErr_Fp, "�Z��DB�t�B�[���h�̃f�[�^�̎擾�Ɏ��s\n");
			break;
		}
		if( odyn.GetFieldValue	( "OAZA_KANJI", a_oaza_str, 37 ) != OSUCCESS ) {
			fprintf	( m_fpErr_Fp, "�Z��DB�t�B�[���h�̃f�[�^�̎擾�Ɏ��s\n");
			break;
		}
		if( odyn.GetFieldValue	( "AZA_KANJI", a_aza_str, 25 ) != OSUCCESS ) {
			fprintf	( m_fpErr_Fp, "�Z��DB�t�B�[���h�̃f�[�^�̎擾�Ɏ��s\n");
			break;
		}

	//	sprintf	( tmpstr4, "%s%s%s", tmpstr1, tmpstr2, tmpstr3 );
		sprintf	( a_cpCode11, "%s%s%s%s", a_ken_code_tmp, a_shi_code_tmp, a_oaza_code_tmp, a_aza_code_tmp );
	//	sprintf	( a_cpChimei, "%s%s", a_oaza_str, a_aza_str );
	
	//	addr_str	= tmpstr4;
	
	//	m_Map.insert(m_string::value_type(addr_str, addr_str));
		addr_dat			a_iAdd;
		a_iAdd.e_sCode		= a_cpCode11;
	//	a_iAdd.e_sName		= a_cpChimei;
		a_iAdd.e_sKen_Kanji	= a_ken_str;
		a_iAdd.e_sShi_Kanji	= a_shi_str;
		a_iAdd.e_sOAza_Kanji= a_oaza_str;
		a_iAdd.e_sAza_Kanji	= a_aza_str;

		// �����ŕ������Ó��Ȍ`���ɕϊ����Ċi�[
	//	char*	a_cpCutStr	= NULL;
	//	a_aASWork.Get_Sikutyouson_Name ( tmpstr4, tmpstr3, &a_cpCutStr );// �s�撬������������
	//	a_iAdd.e_sName	= a_cpCutStr;

	//	printf("newcode=%s\tname=%s\n", a_cpCode11, a_cpChimei );			
	
		m_sDatas.insert( a_iAdd );
		// 5���R�[�h�����p�̃��R�[�h�����
		if( a_iCount == 0 ) {
			a_iAdd.e_sCode	= AdrFive;
			a_iAdd.e_sOAza_Kanji= "";
			a_iAdd.e_sAza_Kanji	= "";
			m_sDatas.insert( a_iAdd );
			a_iTmpAddr		= a_iAdd;	// ���ʋ�Ή��̔���p
		}
		odyn.MoveNext();
		a_iCount++;
	}
	odyn.Close();	// �N���[�Y���Ă����Ȃ���GetShiKuInfo���ł��������Ȃ�B
	// �����ɗ��ăf�[�^���Ȃ��ꍇ�͎��s
	if( a_iCount == 0 ) {
		return	( false );
	}
	// ���ʋ�Ή��p "xx�s�@oo��"�̏ꍇ�́Axx�s��5�����擾����
	long	a_lPos	= 0;
	a_lPos	= a_iTmpAddr.e_sShi_Kanji.find ( "�s�@" );
	if( a_lPos != a_iTmpAddr.e_sShi_Kanji.npos ) {	// �S�p�X�y�[�X�Ō���
		long	a_lLen	= 0;
		a_lLen	= a_iTmpAddr.e_sShi_Kanji.length();
		// �Ō��1���������o����r
		if(a_lLen - 2 > 0) {
			if( a_iTmpAddr.e_sShi_Kanji.substr(a_lLen-2,2) == "��") {
				// "�s"�ŕ��������؂�
				string	a_sShi;
				a_sShi	= a_iTmpAddr.e_sShi_Kanji.substr ( 0, a_lPos+2 );
				
				addr_dat	a_iTmpDat;
				if( GetShiKuInfo( m_oAdm_DB, a_iTmpAddr.e_sKen_Kanji, a_sShi, a_iTmpDat ) ) {
					string		a_sKeyStr;
					a_sKeyStr	= a_iTmpDat.e_sKen_Kanji;	// �s���{����
					a_sKeyStr	+=a_iTmpDat.e_sShi_Kanji;	// �s�撬����
					m_sRevData.insert( m_addr_dat::value_type( a_sKeyStr, a_iTmpDat ) );
				}
			}
		}
	}
	return	( true );
}

bool addr_db_handle::
LoadOldAddress_DB( const char *c_cpCode )	///< ���s���R�[�h
{

	// ���ǃf�[�^�̊m�F
	is_addr_dat		a_iIndx;
	addr_dat		a_aSKey;	// �T�[�`�L�[

	a_aSKey.e_sCode	= c_cpCode;
	a_iIndx	= m_sOldDatas.find ( a_aSKey );
	if( a_iIndx != m_sOldDatas.end() ) {
	}

	// �ԍ₳��̃\�[�X���Q�l��
	char	AdrFive[6];
	char	KenCode[3];
	char	ShiCode[4];

	memcpy( AdrFive , c_cpCode , 5 );	AdrFive[5] = '\0';
	memcpy( KenCode , c_cpCode , 2 );	KenCode[2] = '\0';
	memcpy( ShiCode , c_cpCode+2 , 3 );	ShiCode[3] = '\0';

	///	SQL��
	char	sql[1148];
	memset( sql , '\0' , 1148 );

	// ���R�[�h������Ă���
	sprintf( sql ,	"SELECT "
						"DISTINCT "
						"KEN_CODE,"
						"SHI_CODE, "
						"SHI_KANJI "
					"from "
						"AREACODE_MASTER "
					"where "
						"(NEW_KEN_CODE='%s' and NEW_SHI_CODE='%s') and "
						"not(KEN_CODE='%s' and SHI_CODE='%s') ",
						KenCode, ShiCode, KenCode, ShiCode);
					//	"and OAZA_CODE=%d) and AZA_CODE=%d)", );

	/// SQL���Ńf�[�^���o��
	ODynaset odyn( m_oAdm_DB , sql );
	char	tmpstr1[10];
	char	tmpstr2[10];
	char	tmpstr3[25];
	char	tmpstr4[45];
	string	addr_str;

	addr_str_work	a_aASWork;

	int	a_iCount	= 0;
	while (!odyn.IsEOF()){
		memset				( tmpstr1, '\0' , 10 );
		memset				( tmpstr2, '\0' , 10 );
		memset				( tmpstr3, '\0' , 25 );
		memset				( tmpstr4, '\0' , 45 );
	
		if( odyn.GetFieldValue	( "KEN_CODE", tmpstr1, 3 ) != OSUCCESS ) {
			fprintf	( m_fpErr_Fp, "�Z��DB�t�B�[���h�̃f�[�^�̎擾�Ɏ��s\n");
			break;
		}
		if( odyn.GetFieldValue	( "SHI_CODE", tmpstr2, 4 ) != OSUCCESS ) {
			fprintf	( m_fpErr_Fp, "�Z��DB�t�B�[���h�̃f�[�^�̎擾�Ɏ��s\n");
			break;
		}
		if( odyn.GetFieldValue	( "SHI_KANJI", tmpstr3, 25 ) != OSUCCESS ) {
			fprintf	( m_fpErr_Fp, "�Z��DB�t�B�[���h�̃f�[�^�̎擾�Ɏ��s\n");
			break;
		}
		sprintf	( tmpstr4, "%s%s", tmpstr1, tmpstr2 );
	
		addr_dat		a_iAdd;
	//	a_iAdd.e_sCode	= tmpstr4;
		a_iAdd.e_sCode	= c_cpCode;	// ���R�[�h������

		// �����ŕ������Ó��Ȍ`���ɕϊ����Ċi�[
		char*	a_cpCutStr	= NULL;
		a_aASWork.Get_Sikutyouson_Name ( tmpstr4, tmpstr3, &a_cpCutStr );// �s�撬������������
//		a_iAdd.e_sName	= a_cpCutStr;
printf("oldcode=%s\tname=%s\n", c_cpCode, a_cpCutStr );		
		m_sOldDatas.insert( a_iAdd );		
		odyn.MoveNext();
		a_iCount++;
	}
	return	( true );
}


/**
	�f�[�^����
*/
addr_dat	addr_db_handle::
Find_Addr_Data ( char*	c_cpKey	)	///< �T�[�`�L�[�i5���R�[�h�j
{
	addr_dat	a_aRet;		// �ԋp�l
	addr_dat	a_aSKey;	// �T�[�`�L�[

	a_aSKey.e_sCode	= c_cpKey;

	is_addr_dat	a_iIndx;
	a_iIndx	= m_sDatas.find ( a_aSKey );
	if( a_iIndx != m_sDatas.end() ) {
		a_aRet	= *a_iIndx;
	}
	return	( a_aRet );
}

/**
	�������X�g�̍ŏ���Ԃ�
*/
bool	addr_db_handle::
Find_Old_Name_First	( char*		c_cpKey,	///<�T�[�`�L�[�i5���R�[�h�j
					  addr_dat& c_aDat )	///< �����f�[�^[OUT]
{
	addr_dat	a_aSKey;	// �T�[�`�L�[

	a_aSKey.e_sCode	= c_cpKey;

	m_iOldIndx	= m_sOldDatas.find ( a_aSKey );
	if( m_iOldIndx != m_sOldDatas.end() ) {
		c_aDat	= *m_iOldIndx;
	}else {
		return	( false );
	}
	return	( true );
}

/**
	�������X�g�̎���Ԃ�
*/
bool	addr_db_handle::
Find_Old_Name_Next	( char*		c_cpKey,	///<�T�[�`�L�[�i5���R�[�h�j
					  addr_dat&	c_aDat )	///<�����f�[�^[OUT]
{
	m_iOldIndx++;
	if( m_iOldIndx != m_sOldDatas.end() ) {
		c_aDat	= *m_iOldIndx;
	}else {
		return	( false );
	}
	return	( true );
}

/**
	���̂ɂ��Ẵ`�F�b�N���s��
*/
/*
bool repre_pt_chk::
NameChk(	const char*	c_cpName, 		///< ��r�����̕�����[IN]
			char*		c_cpCmpStr )	///< ��r�Ɏg�p����������[out]
{
    const char *p1, *p2;
    char *p;
    char Buf[64];
    char Buf2[64];
	char a_cpConvName[64];
	memset	( a_cpConvName, '\0', 64 );
    
    p1 = strchr(c_cpName, ' ');
    p1++;
    p1 = strchr(p1, ' ');
    p1++;
    if(strcmp(&(e_cpCode[8]), "000") == 0){
        p2 = strchr(p1, ' ');
        strncpy(Buf, p1, p2-p1);
        Buf[p2-p1] = '\0';
        CutSpace(Buf);
        if(strstr(Buf, "�厚") == Buf){
            strcpy(a_cpConvName, Buf+4);	// �厚�������R�s�[
        }else if(strstr(Buf, "��") == Buf){
            strcpy(a_cpConvName, Buf+2);	// ���������R�s�[
        }else{
            strcpy(a_cpConvName, Buf);		// �Ȃ���΂��̂܂܃R�s�[
        }
    }else if(e_cpCode[8] == '0'){
        p2 = strchr(p1, ' ');
        p2++;
        strncpy(Buf, p1, p2-p1-1);
        if((p = strstr(p2, "��")) != NULL){		// ���ڂ̑O�܂ł����o��
            *p = '\0';
        }
        strcpy(&(Buf[p2-p1-1]), p2);
        CutSpace(Buf);
        strcpy(a_cpConvName, Buf);
    }else{
        p2 = strchr(p1, ' ');
        if(*(p2+1) == '\0'){
            strncpy(Buf, p1, p2-p1);
            Buf[p2-p1] = '\0';
        }else{
            strcpy(Buf, p2+1);
        }
        CutSpace(Buf);
        if(strstr(Buf, "��") == Buf){
            strcpy(a_cpConvName, Buf+2);
        }else{
            strcpy(a_cpConvName, Buf);
        }
    }
	strcpy(c_cpCmpStr, a_cpConvName);	// 2003.10.30 �ǉ�
    if(strcmp(a_cpConvName, e_cpName) != 0){
		ChgKetsu(e_cpName, Buf2);	//�@�P�A�c�̏���
		if(strcmp(a_cpConvName, Buf2) == 0){
			return true;
		}
		//fprintf(stderr, "���̂��Ⴄ,%s,%s,%s\n", Err->AddrCode, Err->Name, Err->Name2);
	}else {
		return	true;
	}
	// ���߂������ꍇ�͕������Ԃ�
    return false;
}*/

/**
	'�P'�A'��'�A'�c','�b'�̈Ⴂ�𓝈ꂷ��
*//*
void repre_pt_chk::
ChgKetsu(const char*	c_cpSrc,	///< �ϊ��O�̕�����
		char*			c_cpTgt)	///< �ϊ���̕�����
{
    int i;
    int len = strlen(c_cpSrc);

    for(i = 0; i < len; i += 2){
        if(memcmp(c_cpSrc+i, "��", 2) == 0){
            memcpy(c_cpTgt+i, "�P", 2);
        }else if(memcmp(c_cpSrc+i, "�b", 2) == 0){
            memcpy(c_cpTgt+i, "�c", 2);
        }else{
            memcpy(c_cpTgt+i, c_cpSrc+i, 2);
        }
    }
    *(c_cpTgt+i) = '\0';
}
*/
/**
	�X�y�[�X�̃J�b�g
*//*
void repre_pt_chk::
CutSpace(char *Buf)		///< ������
{
    char *p;

    if((p = strstr(Buf, "�@�厚")) != NULL){
        strcpy(p, p+6);
    }else if((p = strstr(Buf, "�@��")) != NULL){
        strcpy(p, p+4);
    }
    while((p = strstr(Buf, "�@")) != NULL){
        strcpy(p, p+2);
    }
}
*/
