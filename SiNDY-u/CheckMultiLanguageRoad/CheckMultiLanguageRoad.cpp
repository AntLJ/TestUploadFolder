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

// CheckMultiLanguageRoad.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//


#include "CheckMultiLanguageRoad.h"
#include <WinLib/VersionInfo.h>

using namespace std;

#define OPTION_MAX 4


/**
 * @brief	�g�p�@
 */
void	CCheckMultiLanguageRoad::printUsage()
{
	cerr
		<< "�y������U���ē������[�X�p�f�[�^�`�F�b�N�c�[��" << "�z\n"
		<< "(Usage)CheckMultiLanguageRoad.exe [option]\n"
		<< "�E�I�v�V����\n"
		<< "-l[InLayerName]				���̓��C����\n"
		<< "-m[meshlist]				���b�V�����X�g�w��\n"
		<< "-i[InShapeFilePath]			�`�F�b�N�Ώ�05Shape�̊�_�p�X\n"
		<< "-x[logfile]			���O�o�̓t�@�C���w��\n"
		<< endl;
}

/**
 * @brief	������
 * @param	argc	[in]	�����̐�
 * @param	argv	[in]	����
 * @return	bool
 */
bool	CCheckMultiLanguageRoad::init(int argc, char *argv[])
{
	if( !arg_check( argc, argv ) )
		return false;

	CVersion cVer;
	::GetLocalTime( &m_st );

	m_ErrorLog << "#" << CT2CA(cVer.GetOriginalFilename()) << " FILEVERSION " <<CT2CA(cVer.GetFileVersion()) << " PRODUCTVERSION " << CT2CA(cVer.GetProductVersion()) << endl;
	m_ErrorLog << "#�J�n���� : " << m_st.wYear << "/" << m_st.wMonth << "/" << m_st.wDay << " " << m_st.wHour << ":" << m_st.wMinute << ":" << m_st.wSecond << endl;

	return true;
}


/**
 * @brief	���s
 * @return	bool
 */
bool	CCheckMultiLanguageRoad::execute()
{
	// OLE2T���̈ꕔ�}�N�����g�p����ꍇ�ɕK�v
	USES_CONVERSION;

	vector<string>::iterator	itrMesh;
	int							iMesh = 0;
	
	string	strInFullPath;

	for( itrMesh = m_vsMesh.begin() ; itrMesh != m_vsMesh.end() ; itrMesh++ )
	{
		strInFullPath		= make_mesh_fname( itrMesh->c_str(), m_InFilePath, m_InLayer );
		if( strInFullPath.empty() )
		{
			m_bIsError = true;
			continue;
		}

		// dbf�t�@�C����ǂݍ��݃`�F�b�N
		if( !CheckDBF( itrMesh->c_str(), strInFullPath ) )
		{
			m_ErrorLog << "#ERROR " << itrMesh->c_str() <<  " �`�F�b�N���s�I�I(dbf)" << endl;
			m_bIsError = true;
			return false;
		}

		m_ErrorLog << "#" << itrMesh->c_str() << " �`�F�b�N����" << endl;
	}

	::GetLocalTime( &m_st );
	m_ErrorLog << "#�I������ : " << m_st.wYear << "/" << m_st.wMonth << "/" << m_st.wDay << " " << m_st.wHour << ":" << m_st.wMinute << ":" << m_st.wSecond << endl; 

	return true;
}

/**
 * @brief	�����`�F�b�N
 * @param	argc	[in]	�����̐�
 * @param	arrv	[in]	����
 * @return	bool
 */
bool	CCheckMultiLanguageRoad::arg_check( int argc, char* argv[] )
{
	int		OptionCount[OPTION_MAX];				// �I�v�V�����d���J�E���g�p
	int		i = 0;
	CString temp;

	for( i = 0 ; i < OPTION_MAX ; ++i )
	{
		OptionCount[i] = 0;
	}

	for( i = 0 ; i < argc ; ++i )
	{
		if( strcmp( argv[i], "-l" ) == 0 )				// ���C���[��
		{
			++OptionCount[0];
			m_InLayer = argv[++i];
		}
		else if( strcmp( argv[i], "-m" ) == 0 )			// ���b�V��
		{
			m_Meshlistname = argv[++i];
			temp = m_Meshlistname.c_str();
			if( !loadlist( temp, m_vsMesh ) )			// ���b�V�����X�g�ǂݍ���
			{
				std::cerr << "#ERROR ���b�V�����X�g�̓ǂݍ��݂Ɏ��s�I" << endl;
				return false;
			}
			++OptionCount[1];
		}
		else if( strcmp( argv[i], "-i" ) == 0 )			// ���̓t�@�C��
		{
			m_InFilePath = argv[++i];
			++OptionCount[2];
		}
		else if( strcmp( argv[i], "-x" ) == 0 )			// ���O�t�@�C��
		{
			++OptionCount[3];
			m_ErrorLog.open( argv[++i], std::ios::out );
			if( m_ErrorLog == false )
			{
				std::cerr << "#ERROR ���O�o�̓t�@�C���̃I�[�v���Ɏ��s�I" << endl;
				return false;
			}
		}
		else
		{
			if( i != 0 )
			{
				std::cerr << "�s���Ȉ��� (" << argv[i] << ") ���m�F���Ă��������B" << endl;
				return false;
			}
		}
	}

	// �I�v�V�����d���̃`�F�b�N
	for(i = 0; i < OPTION_MAX; ++i)
	{
		if(OptionCount[i] > 1) {
			cerr << "#ERROR �I�v�V�����̎w�肪�d�����Ă��܂�!!" << endl;
			return false;
		}
		if(OptionCount[i] == 0){
			cerr << "#ERROR �I�v�V�������s�����Ă��܂��A�m�F���Ă�������!!" << endl;
			return false;
		}
	}

	return true;
}

/**
 * @brief	���X�g�t�@�C���ǂݍ���
 * @param	lpszFile	[in]	�ǂݍ��݂������X�g��
 * @param	VectorList	[out]	�Ǎ���Vector
 * @return	bool
 */
bool	CCheckMultiLanguageRoad::loadlist(LPCTSTR lpszFile, std::vector<std::string> &VectorList)
{
	ifstream ifs( static_cast<CT2CW>(lpszFile) );
	if ( !ifs )
		return false;

	copy(
		istream_iterator<string>(ifs),
		istream_iterator<string>(),
		back_inserter( VectorList )
		);

	return ifs.eof();
}

/**
 * @brief	���b�V���P�ʃt�@�C�����̍쐬
 * @param	iMeshCode	[in]	���b�V���R�[�h
 * @param	cPathName	[in]	�t�@�C����_�p�X
 * @param	strLayer	[in]	���C����
 * @return	string		[out]	�t�@�C���̃t���p�X
*/
string	CCheckMultiLanguageRoad::make_mesh_fname ( string strMesh,	string strPath, string strLayer )
{
	const int		iMeshCode = atoi( strMesh.c_str() );
	stringstream	ssSubStr;
	stringstream	ssFullPath;
	const string	sDataSet = "Road";

	if(  100000 <= iMeshCode && iMeshCode < 1000000)
	{
		ssSubStr << iMeshCode/100 << "\\" << iMeshCode;
	}
	else
	{
		m_ErrorLog << "#ERROR " << strMesh << "�F�s���ȃ��b�V���R�[�h�ł�" << endl;
		return "";
	}

	ssFullPath << strPath.c_str() << "\\" << sDataSet.c_str() << "\\" << ssSubStr.str() << "\\" << strMesh.c_str() << "_" << strLayer.c_str();

	return ssFullPath.str();
}

/**
 * @brief	dbf�t�@�C����ǂݍ��݁A�`�F�b�N���s��
 * @param	strMesh			[in]	�ϊ����b�V��
 * @param	strInFullPath	[in]	���̓t�@�C���̃t���p�X
 * @return	bool
 */	
bool	CCheckMultiLanguageRoad::CheckDBF( string strMesh, string strInFullPath )
{
	string			strInFullPathPlus		= strInFullPath + ".dbf";
	CDBFHandle		cInDBFHdl;		/// ����DBF�t�@�C���n���h���N���X

	// �`�F�b�N�Ώ�dbf�I�[�v��
	if( !cInDBFHdl.open((char*)strInFullPath.c_str(), "rb") )
	{
		cerr << "#ERROR DBF�t�@�C��<" << strInFullPathPlus << ">�̃I�[�v���Ɏ��s�I" << endl;
		m_ErrorLog << "#ERROR DBF�t�@�C��<" << strInFullPathPlus << ">�̃I�[�v���Ɏ��s�I" << endl;
		return false;
	}

	// ���R�[�h���̎擾
	int		iRecCount		= cInDBFHdl.get_RecordCount();

	// �e���R�[�h�̃`�F�b�N
	int		iCount = 0;
	for( iCount = 0 ; iCount < iRecCount ; iCount++ )
	{
		// 1���R�[�h�̃��[�h
		ShpRecord		stShpRecord;
		if( !LoadRecord( cInDBFHdl, iCount, stShpRecord ) )
		{
			cerr << "#ERROR DBF�t�@�C��<" << strInFullPathPlus << ">��<" << iCount << "�Ԗ�>�̃��R�[�h�ǂݍ��݂Ɏ��s�I" << endl;
			m_ErrorLog << "#ERROR DBF�t�@�C��<" << strInFullPathPlus << ">��<" << iCount << "�Ԗ�>�̃��R�[�h�ǂݍ��݂Ɏ��s�I" << endl;
			return false;
		}

		// �����E�ǂ݁E�|�󂪑S�ċ�Ȃ�΁A�`�F�b�N���Ȃ�
		if( !stShpRecord.wsNameKanji.empty() || !stShpRecord.wsNameYomi.empty() || !stShpRecord.wsNameEng.empty() )
		{
			// �������̂ƕ������т̃`�F�b�N
			MojiStrChk( stShpRecord );

			// �������ƌ��ǂ݁A�|�󖼏̂̊֘A���`�F�b�N
			MojiRelationChk( stShpRecord );
		}
	}

	return true;
}

/**
 * @brief	dbf�t�@�C����1���R�[�h��ǂݍ���
 * @param	cDBFHdl			[in]	�Y��dbf��dbfHandle�N���X
 * @param	iRecNum			[in]	�ǂݍ��݂������R�[�h�ԍ�
 * @param	stShpRecord		[out]	�ǂݍ��ݐ�̍\����
 * @return	bool
 */	
bool	CCheckMultiLanguageRoad::LoadRecord( CDBFHandle cDBFHdl, int iRecNum, ShpRecord &stShpRecord )
{
	// �t�B�[���h���̎擾
	int		iFieldCount	= cDBFHdl.get_FieldCount();

	int		iFidx;
	for( iFidx = 0 ; iFidx < iFieldCount ; iFidx++ )
	{
		// �t�B�[���h���擾
		char			*FieldName;
		string			sFieldName;
		DBFFieldType	dbfType	= FTInvalid;
		dbfType			= cDBFHdl.get_FieldInfo( iFidx, &FieldName, 0, 0 );
		sFieldName		= FieldName;

		// �v�f�擾�p
		bool			bReadRet = true;
		const char*		cpData;
		wchar_t			wcWStr[256];
		wmemset( wcWStr, '\0', 256 );

		// ���{�ꖼ�̃t�B�[���h�ł����
		if( sFieldName == "NAME_KANJI" )
		{
			bReadRet				= cDBFHdl.read_Attribute( iRecNum, iFidx, &cpData );
			MultiByteToWideChar(CP_UTF8, 0, cpData, -1, wcWStr, 256);	// 4�Ԗڂ̈�����-1��NULL�I�[�Ƃ݂Ȃ����
			stShpRecord.wsNameKanji	= wcWStr;
		}

		// ���{��ǂ݃t�B�[���h�ł����
		else if( sFieldName == "NAME_YOMI" )
		{
			bReadRet				= cDBFHdl.read_Attribute( iRecNum, iFidx, &cpData );
			MultiByteToWideChar(CP_UTF8, 0, cpData, -1, wcWStr, 256);	// 4�Ԗڂ̈�����-1��NULL�I�[�Ƃ݂Ȃ����
			stShpRecord.wsNameYomi	= wcWStr;
		}

		// �p��t�B�[���h�ł����
		else if( sFieldName == "NAME_ENG" )
		{
			bReadRet				= cDBFHdl.read_Attribute( iRecNum, iFidx, &cpData );
			MultiByteToWideChar(CP_UTF8, 0, cpData, -1, wcWStr, 256);	// 4�Ԗڂ̈�����-1��NULL�I�[�Ƃ݂Ȃ����
			stShpRecord.wsNameEng	= wcWStr;
		}

		if( bReadRet == false )
		{
			cerr << "#ERROR " << iRecNum << "�Ԗڂ̃��R�[�h��" << iFidx << "�Ԗڂ̃t�B�[���h�擾���s!" << endl;
			m_ErrorLog << "#ERROR " << iRecNum << "�Ԗڂ̃��R�[�h��" << iFidx << "�Ԗڂ̃t�B�[���h�擾���s!" << endl;
			return false;
		}
	}

	return true;
}

/**
 * @brief	�����񃋁[���`�F�b�N
 * @param	stShpRecord		[in]	shape��1���R�[�h(�����A�ǂ݁A�|��̂�)	
 */	
void	CCheckMultiLanguageRoad::MojiStrChk( ShpRecord &stShpRecord )
{
	string		sErrorMessage;

	//--------------------------------------------------------------------------------------------------
	// �p�꒍�L�`�F�b�N
	//--------------------------------------------------------------------------------------------------
	// �A���t�@�x�b�g�Ɛ����A�n�C�t�������e����Ă��镶���ȊO�̕����������Ă��Ȃ���
	// �A���t�@�x�b�g�Ɛ����A�n�C�t�������e����Ă��镶�����S�Ĕ��p�œ����Ă��邩
	// �X�y�[�X���S�Ĕ��p�\���ɂȂ��Ă��邩�������Ń`�F�b�N��
	if( stShpRecord.wsNameEng != _T("") ) {	// �����񂪋�̏ꍇ�͂��Ȃ�
		if( !e_sChk.isalnumsp_str ( stShpRecord.wsNameEng ) )
		{
			m_bIsError = true;
			sErrorMessage = "�p�ꕶ���R�[�h���͈͊O";
			OutputError( stShpRecord, sErrorMessage );
		}
		// �X�y�[�X��̍ŏ���1�����ڂ͑啶���ɂȂ��Ă��邩
		// �X�y�[�X��̕������uof�v�uat�v�uon�v�ufor�v�uin�v�Ƃ������O�u���ȊO�啶���ɂȂ��Ă��邩
		if( !e_sChk.Chk_Space_After_Big ( stShpRecord.wsNameEng ) )
		{
			m_bIsError = true;
			sErrorMessage = "�啶���������̃~�X";
			OutputError( stShpRecord, sErrorMessage );
		}
		// �X�y�[�X��2�ȏ�d�����Ă��Ȃ���
		if( !e_sChk.Space_Dupli_Chk ( stShpRecord.wsNameEng ) )
		{
			m_bIsError = true;
			sErrorMessage = "�X�y�[�X��2�ȏ�d��";
			OutputError( stShpRecord, sErrorMessage );
		}
		// �����̑O��ɕs���ȃX�y�[�X���Ȃ���
		if( !e_sChk.Nakaguro_Chk ( stShpRecord.wsNameEng ) )
		{
			m_bIsError = true;
			sErrorMessage = "�u�E�v�̑O��ɕs���ȃX�y�[�X";
			OutputError( stShpRecord, sErrorMessage );
		}
		// ���ʂ̑O��ɕs���ȃX�y�[�X���Ȃ���
		if( !e_sChk.Kakko_Chk ( stShpRecord.wsNameEng ) )
		{
			m_bIsError = true;
			sErrorMessage = "�u(�v�Ɓu)�v�̑O��ɕs���ȃX�y�[�X";
			OutputError( stShpRecord, sErrorMessage );
		}
	}
}

/**
 * @brief	������֘A���`�F�b�N
 * @param	stShpRecord		[in]	shape��1���R�[�h(�����A�ǂ݁A�|��̂�)	
 */	
void	CCheckMultiLanguageRoad::MojiRelationChk( ShpRecord &stShpRecord )
{
	string		sErrorMessage;

	//----------------------------------------------------------------------------------------------------------------
	// �p�꒍�L�`�F�b�N
	//----------------------------------------------------------------------------------------------------------------
	// �������ƌ��ǂ݂������Ă��Ȃ��̂ɁA�|�󖼏̂������Ă��邩�H
	if( stShpRecord.wsNameKanji.empty() && stShpRecord.wsNameYomi.empty() && !stShpRecord.wsNameEng.empty() )
	{
		m_bIsError = true;
		sErrorMessage = "�����̂ƌ��ǂ݂���Ȃ̂ɉp�ꖼ�̂������Ă���";
		OutputError( stShpRecord, sErrorMessage );
	}

	// �������ƌ��ǂ݂������Ă�̂ɁA�|�󖼏̂������Ă��Ȃ��H
	if( !stShpRecord.wsNameKanji.empty() && !stShpRecord.wsNameYomi.empty() && stShpRecord.wsNameEng.empty() )
	{
		m_bIsError = true;
		sErrorMessage = "�����̂ƌ��ǂ݂������Ă���̂ɉp�ꖼ�̂���";
		OutputError( stShpRecord, sErrorMessage );
	}
}

/**
 * @brief	UTF16��SJIS�ɂ��A���O�o��
 * @param	stShpRecord		[in]	shape��1���R�[�h(�����A�ǂ݁A�|��̂�)	
 * @param	sErrorMessage	[in]	�G���[���b�Z�[�W
 */	
void	CCheckMultiLanguageRoad::OutputError( ShpRecord stShpRecord, string sErrorMessage )
{
	string		sSjisNameKanji;
	string		sSjisNameYomi;
	string		sSjisNameEng;

	char		cStr[512];
	memset( cStr, '\0', 512 );

	// �������̂�SJIS��
	WideCharToMultiByte(CP_ACP, 0, stShpRecord.wsNameKanji.c_str(), -1, cStr, 512, NULL, 0 );	// 4�Ԗڂ̈�����-1��NULL�I�[�Ƃ݂Ȃ����
	sSjisNameKanji	= cStr;

	memset( cStr, '\0', 512 );

	// ���~���̂�SJIS��
	WideCharToMultiByte(CP_ACP, 0, stShpRecord.wsNameYomi.c_str(), -1, cStr, 512, NULL, 0 );	// 4�Ԗڂ̈�����-1��NULL�I�[�Ƃ݂Ȃ����
	sSjisNameYomi	= cStr;

	memset( cStr, '\0', 512 );

	// �p�ꖼ�̂�SJIS��
	WideCharToMultiByte(CP_ACP, 0, stShpRecord.wsNameEng.c_str(), -1, cStr, 512, NULL, 0 );	// 4�Ԗڂ̈�����-1��NULL�I�[�Ƃ݂Ȃ����
	sSjisNameEng	= cStr;

	m_ErrorLog << "#ERROR " << sErrorMessage << ", " << sSjisNameKanji << ", " << sSjisNameYomi << ", " << sSjisNameEng << endl; 
}
