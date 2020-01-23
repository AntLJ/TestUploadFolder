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

#include "ConvSJIStoUTF8.h"
#include "stdafx.h"

#include <WinLib/VersionInfo.h>

#define OPTION_MAX 8

using namespace std;

/**
 * @brief	�g�p�@
 */
void	CConvSJIStoUTF8::printUsage()
{
	cerr
		<< "�y05Shape�����R�[�h�ϊ��c�[��" << "�z\n"
		<< "(Usage)ConvSJIStoUTF8.exe [option]\n"
		<< "�E�I�v�V����\n"
		<< "-l[Layername]			�����Ώۃ��C�����w��(�K�{)\n"
		<< "-m[meshlist]			���b�V�����X�g�w��(citycode, roadcode�ȊO�K�{)\n"
		<< "-d[Datasetname]			�f�[�^�Z�b�g��(Citymap,Road��)(�K�{)\n"
		<< "-i[filepass]			�ϊ��ΏۃV�F�C�v�t�@�C���p�X(��_)(�K�{)\n"
		<< "-o[filepass]			�o�͐�V�F�C�v�t�@�C���p�X(��_)(�K�{)\n"
		<< "-x[logfile]				���O�o�̓t�@�C���w��(�K�{)\n"
		<< "-a[AllLayerList]		���C���w��~�X���ʗp�S���C���L�q�t�@�C��(�K�{)\n"
		<< "-g[IgnoreFieldList]		�ϊ������t�B�[���h�w��t�@�C��(�K�{)\n"
		<< "-dbf					dbf�ϊ����[�h(�C��)\n"
		<< endl;
}

/**
 * @brief	������
 * @param	argc	[in]	�����̐�
 * @param	argv	[in]	����
 * @return	bool
 */
bool	CConvSJIStoUTF8::init(int argc, char *argv[])
{
	if( !arg_check( argc, argv ) )
		return false;

	return true;
}

/**
 * @brief	�����`�F�b�N
 * @param	argc	[in]	�����̐�
 * @param	argv	[in]	����
 * @return	bool
 */
bool	CConvSJIStoUTF8::arg_check(int argc, char *argv[])
{
	int		a_OptionCount[OPTION_MAX];		// �I�v�V�����d���J�E���g�p
	int		i = 0;
	CString csTemp;
	string	sTemp;

	for( i = 0 ; i < OPTION_MAX ; i++ )
	{
		a_OptionCount[i]	= 0;
	}

	for( i = 0 ; i < argc ; ++i )
	{
		if( strcmp( argv[i], "-l" ) == 0 )
		{
			if( argc < i + 2 )	return false;
			++a_OptionCount[0];
			m_sLayerName	= argv[++i];

			if( m_sLayerName == "citycode" )
				m_iExeMode	|= ON_CITYCODE_MODE;
			else if( m_sLayerName == "roadcode" ) {
				m_iExeMode	|= ON_ROADCODE_MODE;
				m_DbfMode = true;
			}
		}
		else if( strcmp( argv[i], "-m" ) == 0 )
		{
			if( argc < i + 2 )	return false;
			++a_OptionCount[1];
			csTemp = argv[++i];
			if( !loadlist( csTemp, m_vsMesh ) )
			{
				cerr << "#ERROR ���b�V�����X�g�̓ǂݍ��݂Ɏ��s�I" << endl;
				return false;
			}
		}
		else if( strcmp( argv[i], "-d" ) == 0 )
		{
			if( argc < i + 2 )	return false;
			++a_OptionCount[2];
			m_sDataSet		= argv[++i];
		}
		else if( strcmp( argv[i], "-i" ) == 0 )
		{
			if( argc < i + 2 )	return false;
			++a_OptionCount[3];
			m_sFilePass		= argv[++i];
		}
		else if( strcmp( argv[i], "-o" ) == 0 )
		{
			if( argc < i + 2 )	return false;
			++a_OptionCount[4];
			m_sDestFilePass	= argv[++i];
		}
		else if( strcmp( argv[i], "-x" ) == 0 )		
		{
			if( argc < i + 2 )	return false;
			++a_OptionCount[5];
			m_ErrorLog.open( argv[++i], ios::out );
			if( m_ErrorLog == false )
			{
				cerr << "#ERROR ���O�o�̓t�@�C���̃I�[�v���Ɏ��s�I" << endl;
				return false;
			}
		}
		else if( strcmp( argv[i], "-a" ) == 0 )
		{
			if( argc < i + 2 )	return false;
			++a_OptionCount[6];
			csTemp = argv[++i];
			if( !loadlist( csTemp, m_vsAllLayer ) )
			{
				cerr << "#ERROR �S���C���L�q�t�@�C���̓ǂݍ��݂Ɏ��s�I" << endl;
				return false;
			}
		}
		else if( strcmp( argv[i], "-g" ) == 0 )
		{
			if( argc < i + 2 )	return false;
			++a_OptionCount[7];
			csTemp = argv[++i];
			if( !loadlist( csTemp, m_vsIgnoreField ) )
			{
				cerr << "#ERROR �ϊ������t�B�[���h�w��t�@�C���̓ǂݍ��݂Ɏ��s�I" << endl;
				return false;
			}
		}
		else if( strcmp( argv[i], "-dbf" ) == 0 )
		{
			++a_OptionCount[8];
			m_DbfMode = true;
		}
		else
		{
			if( i != 0 )
			{
				cerr << "�s���Ȉ��� (" << argv[i] << ") ���m�F���Ă��������I" << endl;
				return false;
			}
		}
	}

	// �I�v�V�����d���`�F�b�N
	for( i = 0 ; i < OPTION_MAX ; i++ )
	{
		if( a_OptionCount[i] > 1 ){
			cerr << "#ERROR �I�v�V�����̎w�肪�d�����Ă��܂��I" << endl;
			return false;
		}
		// roadcode.dbf��citycode.dbf�̃��b�V�����X�g�w��͖����Ă��悢
		if( i != 1 || ( (m_iExeMode ^ ON_ROADCODE_MODE) && (m_iExeMode ^ ON_CITYCODE_MODE) )){
			if( a_OptionCount[i] == 0 ){
				cerr << "#ERROR �I�v�V�����̎w�肪�s�����Ă��܂��I" << endl;
				return false;
			}	
		}
	}

	// �w�背�C�����݊m�F�`�F�b�N
	vector<string>::iterator	pos;
	pos	= find( m_vsAllLayer.begin(), m_vsAllLayer.end(), m_sLayerName );
	if( pos == m_vsAllLayer.end() ){
		cerr << "#ERROR �w�背�C�������݂��܂���I" << endl;
		return false;
	}

	// DBF�݂̂̃��C���̏������[�h���`
	if( m_iExeMode ^ ON_ROADCODE_MODE && m_iExeMode ^ ON_CITYCODE_MODE )
	{
		m_iExeMode |= ON_NORMAL_MODE;
	}

	return true;
}


/**
 * @brief	���s
 * @return	bool
 */
bool	CConvSJIStoUTF8::execute()
{

	CVersion cVer;
	CString strMsg;

	::GetLocalTime( &m_st );

	m_ErrorLog << "#" << CT2CA(cVer.GetOriginalFilename()) << " FILEVERSION " <<CT2CA(cVer.GetFileVersion()) << " PRODUCTVERSION " << CT2CA(cVer.GetProductVersion()) << endl;
	m_ErrorLog << "#�J�n���� : " << m_st.wYear << "/" << m_st.wMonth << "/" << m_st.wDay << " " << m_st.wHour << ":" << m_st.wMinute << ":" << m_st.wSecond << endl;

	m_ErrorLog << "#" << m_sLayerName.c_str() << " ���C���F�����J�n" << endl;

	string	strFullPass;
	string	strDestFullPass;

	if( m_iExeMode & ON_CITYCODE_MODE || m_iExeMode & ON_ROADCODE_MODE )
	{
		strFullPass		= m_sFilePass + "\\" + m_sDataSet + "\\" + m_sLayerName;
		strDestFullPass	= m_sDestFilePass + "\\" + m_sDataSet + "\\" + m_sLayerName;

		if( !GetFieldInfo( strFullPass ) )
			return false;

		if( !ConvertDBF( "", strFullPass, strDestFullPass ) )
		{
			m_ErrorLog << "#ERROR �ϊ����s" << endl;
			return false;
		}
	}
	else
	{
		vector<string>::iterator	a_itrMesh;

		for( a_itrMesh = m_vsMesh.begin() ; a_itrMesh != m_vsMesh.end() ; a_itrMesh++ )
		{
			string			strMesh(a_itrMesh->c_str());
			strFullPass		= make_mesh_fname( strMesh, (char*)m_sFilePass.c_str() );
			strDestFullPass	= make_mesh_fname( strMesh, (char*)m_sDestFilePass.c_str() );

			if( a_itrMesh == m_vsMesh.begin() )
			{
				if( !GetFieldInfo( strFullPass ) )
					return false;
			}

			m_ErrorLog << "#" << a_itrMesh->c_str() << endl;
			if( !ConvertDBF( a_itrMesh->c_str(), strFullPass, strDestFullPass ) )
			{
				m_ErrorLog << "#ERROR �ϊ����s" << endl;
				return false;
			}
		}
	}
	::GetLocalTime( &m_st );

	m_ErrorLog << "#" << m_sLayerName.c_str() << " ���C���F����I��" << endl;
	m_ErrorLog << "#�I������ : " << m_st.wYear << "/" << m_st.wMonth << "/" << m_st.wDay << " " << m_st.wHour << ":" << m_st.wMinute << ":" << m_st.wSecond << endl;

	return true;
}

/**
 * @brief	���X�g�t�@�C���ǂݍ���
 * @param	lpszFile	[in]	�ǂݍ��݂������X�g��
 * @param	vsVectorList	[out]	�Ǎ���Vector(1�t�B�[���h����)
 * @return	bool
 */
bool	CConvSJIStoUTF8::loadlist(LPCTSTR lpszFile, vector<string>& vsVectorList )
{
	ifstream ifs( static_cast<CT2CW>(lpszFile) );
	if ( !ifs )
		return false;

	copy(
		istream_iterator<string>(ifs),
		istream_iterator<string>(),
		back_inserter( vsVectorList )
		);

	return ifs.eof();
}

/**
 * @brief	dbf�t�@�C����UTF8�ϊ�
 * @param	���b�V��	[in]	�ϊ����b�V��
 * @param	�p�X		[in]	���̓t�@�C���̃t���p�X
 * @param	�p�X		[in]	�o�̓t�@�C���̃t���p�X
 * @return	bool
 */
bool	CConvSJIStoUTF8::ConvertDBF( string Mesh, string sFullPass, string sDestFullPass )
{
	string			strFullPassPlus		= sFullPass + ".dbf";
	string			strDestFullPassPlus	= sDestFullPass + ".dbf"; 

	CDBFHandle		a_cDBFHdl;		/// ����DBF�t�@�C���n���h���N���X
	CDBFHandle		a_cDestDBFHdl;	/// �o��DBF�t�@�C���n���h���N���X

	// �ǂݍ���dbf�I�[�v��
	if( !a_cDBFHdl.open((char*)sFullPass.c_str(), "rb") )
	{
		cerr << "#ERROR DBF�t�@�C��<" << strFullPassPlus << ">�̃I�[�v���Ɏ��s�I" << endl;
		m_ErrorLog << "#ERROR DBF�t�@�C��<" << strFullPassPlus << ">�̃I�[�v���Ɏ��s�I" << endl;
		return false;
	}

	// �o�͐�f�B���N�g���쐬(imagehlp.h�̃C���N���[�h��imagehlp.lib�̃����N)
	MakeSureDirectoryPathExists( sDestFullPass.c_str() );

	// �o��dbf�쐬
	if( !a_cDestDBFHdl.create((char*)sDestFullPass.c_str()) ){
		cerr << "#ERROR DBF�t�@�C��<" << strDestFullPassPlus << ">�̍쐬�Ɏ��s�I" << endl;
		m_ErrorLog << "#ERROR DBF�t�@�C��<" << strDestFullPassPlus << ">�̍쐬�Ɏ��s�I" << endl;
		return false;
	}

	// �t�B�[���h�ǉ�
	vector<FieldInfo>::iterator	itrFieldInfo;
	for( itrFieldInfo = m_vfFieldInfo.begin() ; itrFieldInfo != m_vfFieldInfo.end() ; itrFieldInfo++ )
	{
		if( itrFieldInfo->dType == FTString && itrFieldInfo->iIgnoreFlag == 0 ){
			// �ϊ��Ώۃt�B�[���h�̏ꍇ�̓T�C�Y��1.5�{(254�𒴂���ꍇ��254�ɐݒ�)
			long fieldLength = (itrFieldInfo->iWidth)*3/2;
			if (fieldLength > 254)
				fieldLength = 254;
			if( a_cDestDBFHdl.add_Field( itrFieldInfo->sName.c_str(), itrFieldInfo->dType, fieldLength, itrFieldInfo->iDecimals ) < 0 )	return false;
		}else{
			if( a_cDestDBFHdl.add_Field( itrFieldInfo->sName.c_str(), itrFieldInfo->dType, itrFieldInfo->iWidth, itrFieldInfo->iDecimals ) < 0 )	return false;
		}
	}

	// ���R�[�h���̎擾
	int a_iRecCount		= a_cDBFHdl.get_RecordCount();

	// �e���R�[�h�̏��擾
	int		a_iCount = 0;
	for( a_iCount = 0 ; a_iCount < a_iRecCount ; a_iCount++ )
	{
		for( itrFieldInfo = m_vfFieldInfo.begin() ; itrFieldInfo != m_vfFieldInfo.end() ; itrFieldInfo++ )
		{
			const char*		a_cpData;
			int				a_ciData	= 0;
			double			a_cdData	= 0;

			bool			bReadRet	= false;
			int				iWriteRet	= 0;

			// �ǂݍ���
			switch( itrFieldInfo->dType )
			{
				case FTString:
					bReadRet	= a_cDBFHdl.read_Attribute( a_iCount, itrFieldInfo->iID, &a_cpData );
					if( !CheckShiftJIS(a_cpData) ){
						cerr << "#ERROR ������u" << a_cpData << "�v�̕����R�[�h�� Shift-JIS �ł͂���܂���!" << endl;
						m_ErrorLog << "#ERROR ������u" << a_cpData << "�v�̕����R�[�h�� Shift-JIS �ł͂���܂���!" << endl;
						bReadRet = false;
					}
					break;
				case FTInteger:
					bReadRet	= a_cDBFHdl.read_Attribute( a_iCount, itrFieldInfo->iID, &a_ciData );
					break;
				case FTDouble:
					bReadRet	= a_cDBFHdl.read_Attribute( a_iCount, itrFieldInfo->iID, &a_cdData );
					break;
				case FTLogical:
				case FTInvalid:
				default:
					bReadRet	= false;
			}
			if( bReadRet == false )
			{
				cerr << "#ERROR DBF�t�@�C��<" << strFullPassPlus << ">��" << a_iCount << "�Ԗڂ̃��R�[�h��" << itrFieldInfo->iID << "�Ԗڂ̃t�B�[���h�擾���s!" << endl;
				m_ErrorLog << "#ERROR DBF�t�@�C��<" << strFullPassPlus << ">��" << a_iCount << "�Ԗڂ̃��R�[�h��" << itrFieldInfo->iID << "�Ԗڂ̃t�B�[���h�擾���s!" << endl;
				return false;
			}

			char	temp[512];
			wchar_t	temp_w[256];
			long	len = 0, wlen = 0, wwlen = 0;

			// ��������
			switch( itrFieldInfo->dType )
			{
				case FTString:
					if( itrFieldInfo->iIgnoreFlag == 0 )
					{
						// S-JIS �� UTF16 �� UTF8�̏��œǂݎ�����t�B�[���h�l��ϊ�
						len = strlen( a_cpData );
						memset( temp, '\0', 512 );
						memset( temp_w, '\0', 512 );
						wlen = MultiByteToWideChar( CP_ACP, 0, a_cpData, len, (LPWSTR)temp_w, 0 );					// �R�s�[�̈�̌v�Z
						MultiByteToWideChar( CP_ACP, 0, a_cpData, len, (LPWSTR)temp_w, wlen );						// ���ۂ�S-JIS��UTF-16�̕ϊ��R�s�[
						wwlen = WideCharToMultiByte( CP_UTF8, 0, (LPCWSTR)temp_w, wlen, (LPSTR)temp, 0, NULL, 0 );	// �R�s�[�̈�̌v�Z
						WideCharToMultiByte( CP_UTF8, 0, (LPCWSTR)temp_w, wlen, (LPSTR)temp, wwlen, NULL, 0 );		// ���ۂ�UTF-16��UTF-8�̕ϊ��R�s�[
						iWriteRet	= a_cDestDBFHdl.write_Attribute( a_iCount, itrFieldInfo->iID, temp );
					}
					else
						iWriteRet	= a_cDestDBFHdl.write_Attribute( a_iCount, itrFieldInfo->iID, a_cpData );
					break;
				case FTInteger:
					iWriteRet	= a_cDestDBFHdl.write_Attribute( a_iCount, itrFieldInfo->iID, a_ciData );
					break;
				case FTDouble:
					iWriteRet	= a_cDestDBFHdl.write_Attribute( a_iCount, itrFieldInfo->iID, a_cdData );
					break;
				case FTLogical:
				case FTInvalid:
				default:
					iWriteRet	= 0;
			}
			if( iWriteRet == 0 )
			{
				cerr << "#ERROR DBF�t�@�C��<" << strDestFullPassPlus << ">��" << a_iCount << "�Ԗڂ̃��R�[�h��" << itrFieldInfo->iID << "�Ԗڂ̃t�B�[���h�ɏ������ݎ��s!" << endl;
				m_ErrorLog << "#ERROR DBF�t�@�C��<" << strDestFullPassPlus << ">��" << a_iCount << "�Ԗڂ̃��R�[�h��" << itrFieldInfo->iID << "�Ԗڂ̃t�B�[���h�ɏ������ݎ��s!" << endl;
				return false;
			}
		}
	}
	a_cDBFHdl.close();
	a_cDestDBFHdl.close();

	// EOF(0x1A)���������ނ��߂̓��ꏈ��(�H�����񃍃W�b�N���p)
	FILE*	a_fpTmpDBFFp	= NULL;
	a_fpTmpDBFFp	= fopen ( strDestFullPassPlus.c_str(), "ab+" );
	if( a_fpTmpDBFFp == NULL ) {
		cerr << "#ERROR DBF�t�@�C��<" << strDestFullPassPlus << ">��EOF�ǉ������Ɏ��s!" << endl;
		m_ErrorLog << "#ERROR DBF�t�@�C��<" << strDestFullPassPlus << ">��EOF�ǉ������Ɏ��s!" << endl;
		return false;
	}else {
		fprintf	( a_fpTmpDBFFp, "%c", 0x1A );
		fclose	( a_fpTmpDBFFp );
	}

	// shp�t�@�C���������C����shp���R�s�[
	if(! m_DbfMode )
	{
		strFullPassPlus		= sFullPass + ".shp";
		strDestFullPassPlus	= sDestFullPass + ".shp"; 

		CSHPHandle		a_cSHPHdl;		/// ����SHP�t�@�C���n���h���N���X
		CSHPHandle		a_cDestSHPHdl;	/// �o��SHP�t�@�C���n���h���N���X

		// �ǂݍ���shp�I�[�v��
		if( !a_cSHPHdl.open((char*)sFullPass.c_str(), "rb+") )
		{
			cerr << "#ERROR SHP�t�@�C��<" << strFullPassPlus << ">�̃I�[�v���Ɏ��s�I" << endl;
			m_ErrorLog << "#ERROR SHP�t�@�C��<" << strFullPassPlus << ">�̃I�[�v���Ɏ��s�I" << endl;
			return false;
		}

		int		iShpType	= 0;
		int		iShpNum		= 0;
		a_cSHPHdl.get_FileInfo( &iShpNum, &iShpType, 0, 0 );

		// �o��shp�쐬(�o�͐�p�X�̃t�H���_��dbf�t�@�C���쐬�̍ۂɍ���Ă���̂ŏȗ�)
		if( !a_cDestSHPHdl.create((char*)sDestFullPass.c_str(), iShpType ) ){
			cerr << "#ERROR SHP�t�@�C��<" << strDestFullPassPlus << ">�̍쐬�Ɏ��s�I" << endl;
			m_ErrorLog << "#ERROR SHP�t�@�C��<" << strDestFullPassPlus << ">�̍쐬�Ɏ��s�I" << endl;
			return false;
		}

		// �e���R�[�h�̏��擾
		for( a_iCount = 0 ; a_iCount < iShpNum ; a_iCount++ )
		{
			CSHPObject		a_cSHPObj;	/// Shp�I�u�W�F�N�g�n���h���N���X

			// Shp�̃I�u�W�F�N�g�ǂݍ���
			if( a_cSHPHdl.read_Object( a_iCount, &a_cSHPObj ) == -1 )
			{
				cerr << "#ERROR SHP�t�@�C��<" << strFullPassPlus << ">��" << a_iCount << "�Ԗڂ̃I�u�W�F�N�g�擾���s!" << endl;
				m_ErrorLog << "#ERROR SHP�t�@�C��<" << strFullPassPlus << ">��" << a_iCount << "�Ԗڂ̃I�u�W�F�N�g�擾���s!" << endl;
				return false;
			}

			// Shp�̃I�u�W�F�N�g��������
			if( a_cDestSHPHdl.write_Object( &a_cSHPObj ) == -1 )
			{
				cerr << "#ERROR SHP�t�@�C��<" << strDestFullPassPlus << ">��" << a_iCount << "�Ԗڂ̃I�u�W�F�N�g�������ݎ��s!" << endl;
				m_ErrorLog << "#ERROR SHP�t�@�C��<" << strDestFullPassPlus << ">��" << a_iCount << "�Ԗڂ̃I�u�W�F�N�g�������ݎ��s!" << endl;
				return false;
			}
		}
		a_cSHPHdl.close();
		a_cDestSHPHdl.close();
	}
	return true;
}


/**
 * @brief	���b�V���P�ʃt�@�C�����̍쐬
 * @param	MeshCode	[in]	���b�V���R�[�h
 * @param	cPathName	[in]	�t�@�C����_�p�X
 * @return	�t�@�C���̃t���p�X
*/
string	CConvSJIStoUTF8::make_mesh_fname ( string MeshCode,	char* cpPathName )
{
	char	a_cSubStr[256];
	memset	( a_cSubStr, '\0', 256 );

	string	a_sSubDir		= "";
	string	a_sFullPass		= "";
	char	a_cTemp[4096];	

	if( 2 == MeshCode.length() || 4 == MeshCode.length() )
		sprintf( a_cSubStr, "%s", MeshCode.c_str() );

	else if( 6 == MeshCode.length() )
		sprintf( a_cSubStr, "%s\\%s", MeshCode.substr(0, 4).c_str(), MeshCode.c_str() );

	else if( 8 == MeshCode.length() )
		sprintf( a_cSubStr, "%s\\%s\\%s", MeshCode.substr(0, 4).c_str(), MeshCode.substr(0, 6).c_str(), MeshCode.c_str() );

	else
		m_ErrorLog << "#ERROR" << MeshCode.c_str() << "�F�s���ȃ��b�V���R�[�h�ł�" << endl;

	a_sSubDir	= a_cSubStr;

	sprintf( a_cTemp, "%s\\%s\\%s\\%s_%s", cpPathName, m_sDataSet.c_str(), a_sSubDir.c_str(), MeshCode.c_str(), m_sLayerName.c_str() );

	a_sFullPass	= a_cTemp;
	
	return a_sFullPass;
}

/**
 * @brief	�ϊ��Ώۃ��C������A������t�B�[���h�̃C���f�b�N�X���擾
 * @param	fullpass	[in]	�t�@�C���̃t���p�X
 * @return	bool
 */
bool	CConvSJIStoUTF8::GetFieldInfo( string sFullPass )
{
	CDBFHandle		a_cDBFHdl;	/// DBF�t�@�C���n���h���N���X

	if( !a_cDBFHdl.open((char*)sFullPass.c_str(), "rb") )
	{
		cerr << "#ERROR DBF�t�@�C��<" << sFullPass << ">�̃I�[�v���Ɏ��s�I" << endl;
		m_ErrorLog << "#ERROR DBF�t�@�C��<" << sFullPass << ">�̃I�[�v���Ɏ��s�I" << endl;
		return false;
	}

	// �t�B�[���h���̎擾
	int		a_iFieldCount	= 0;
	a_iFieldCount		= a_cDBFHdl.get_FieldCount();

	// string�^���܂ނ����ʗp
	int		a_iJudge = 0;

	int		a_iFidx;
	for( a_iFidx = 0 ; a_iFidx < a_iFieldCount ; a_iFidx++ )
	{
		FieldInfo	a_FieldInfo;
		a_FieldInfo.iID	= a_iFidx;

		// �t�B�[���h���擾
		char	*a_FieldName;
		a_FieldInfo.dType	= a_cDBFHdl.get_FieldInfo( a_FieldInfo.iID, &a_FieldName, &a_FieldInfo.iWidth, &a_FieldInfo.iDecimals );
		a_FieldInfo.sName	= a_FieldName;

		if( a_FieldInfo.dType == FTString )
		{
			vector<string>::iterator itr = find( m_vsIgnoreField.begin(), m_vsIgnoreField.end(), a_FieldInfo.sName );
			if( itr == m_vsIgnoreField.end() )
				a_iJudge++;
			else
				a_FieldInfo.iIgnoreFlag = 1;
		}

		// vector�Ƀt�B�[���h�ԍ��A�t�B�[���h�^�C�v�A�t�B�[���h���A�t�B�[���h�T�C�Y�A���x�A�ϊ������t���O��ǉ�(�Y��Shape�̃t�B�[���h��������)
		m_vfFieldInfo.push_back( a_FieldInfo );
	}
	a_cDBFHdl.close();

	if( a_iJudge == 0 ){
		cerr << "#ERROR " << m_sLayerName.c_str() << " �ɕϊ��Ώۃt�B�[���h�����݂��Ȃ����ߕϊ����܂���I" << endl;
		m_ErrorLog << "#ERROR " << m_sLayerName.c_str() << " �ɕϊ��Ώۃt�B�[���h�����݂��Ȃ����ߕϊ����܂���I" << endl;
		return false;
	}

	return true;
}

/**
 * @brief	�����R�[�h��Shift-JIS�����肷��B
 * @param	cChars	[in]	������
 * @return	bool
 */
bool	CConvSJIStoUTF8::CheckShiftJIS( const char* cChars )
{
	const unsigned char* p = ( unsigned char* ) cChars;
	unsigned char c1, c2;
	int len = strlen(cChars);
	
	for( int i=0; i < len; ++i, ++p)
	{
		c1 = *p;
		if( c1 == 0x00 )
			return false;
		else if( c1 <= 0x7F )	// ASCII����
			continue;
		else if( c1 >= 0xa0 && c1 <= 0xdf)	//���p�J�i
			continue;
		else if( ((c1 >= 0x81 && c1 <= 0x9F )|| (c1 >= 0xE0 && c1 <= 0xEF )) && i < len -1 )	// �S�p �Ђ炪��/����/�L��
		{
			c2 = *( p+1 );
			if( (c2 >= 0x40 && c2 <= 0x7E ) || (c2 >= 0x80 && c2 <= 0xFC) )
			{
				i++;
				p++;
			} 
			else
				return false;
		}
		else
			return false;
	}
	return true;
}
