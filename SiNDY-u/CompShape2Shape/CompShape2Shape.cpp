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

// CompShape2Shape.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//
#include "CompShape2Shape.h"
#include "stdafx.h"
#include "ShapeChk.h"

#define OPTION_MAX 13

using namespace std;

/**
 * @brief	�g�p�@
*/
void	CCompShape2Shape::printUsage()
{
	cerr
		<< "�yshape�t�@�C�����m�̑����E�`���r�c�[��(" << m_sVersion.c_str() << ")�z\n"
		<< "(Usage)CompShape2Shape.exe [option]\n"
		<< "�E�I�v�V����\n"
		<< "-m[meshlist]			���b�V�����X�g�w��\n"
		<< "-i1[Orifilepass]		��r���V�F�C�v�t�@�C���p�X(�K�{)\n"
		<< "-i2[Destfilepass]		��r��V�F�C�v�t�@�C���p�X(�K�{)\n"
		<< "-l[Layername]			���C�����w��(�K�{)\n"
		<< "-d[Datasetname]			�f�[�^�Z�b�g��(�K�{)(��FCitymap��)\n"
		<< "-o[output]			���O�o�̓t�@�C���w��(�K�{)\n"
		<< "-ATTR				������r���[�h(-ATTR��-SHP�ǂ��炩�͕K�{�A�����w�肵�Ă��ǂ�)\n"
		<< "-SHP				�`���r���[�h\n"
		<< "-dist_b[DistanceBorder]		���e�덷(�����`�F�b�N)(�덷�̓p�[�Z���e�[�W�Ŏw�肷�邱��)�i��F1.0)\n"
		<< "-FID				OBJECTID�ł͂Ȃ��AFID�ŕR�t����ꍇ�͎w��\n"
		<< "-BIN				�o�C�i����r���[�h(���x�d��)\n"
		<< endl;
}

/**
 * @brief	�o�[�W�����o��
 */
void	CCompShape2Shape::print_ver()
{
	m_ofErrorLog 
		<< "#�yshape�t�@�C�����m�̑����E�`���r�c�[��(" << m_sVersion.c_str() << ")�z\n"
		<< "#CompShape2Shape.exe\n"
		<< "#FileVersion\t" << m_sVersion << "\n"
		<< endl;
}

/**
 * @brief	Shape�t�@�C����_�p�X�o��
 */
void	CCompShape2Shape::print_pass()
{
	m_ofErrorLog 
		<< "#��r����_�p�X:<" << m_sOriFPass << ">\n"
		<< "#��r���_�p�X:<" << m_sDestFPass << ">\n";
}

/**
 * @brief	�J�n�E�I�����ԏo��
 */
void	CCompShape2Shape::print_begin_time()
{
	m_ofErrorLog
		<< "#�J�n����: " << m_st.wYear << "/" << m_st.wMonth << "/" << m_st.wDay << " " << m_st.wHour << ":" << m_st.wMinute << ":" << m_st.wSecond << endl;
}

/**
 * @brief	�J�n�E�I�����ԏo��
 */
void	CCompShape2Shape::print_end_time()
{
	m_ofErrorLog
		<< "#�I������: " << m_st.wYear << "/" << m_st.wMonth << "/" << m_st.wDay << " " << m_st.wHour << ":" << m_st.wMinute << ":" << m_st.wSecond << endl;
}

/**
 * @brief	���O�w�b�_(�t�B�[���h��)�o��
 */
void	CCompShape2Shape::print_log_head()
{
	m_ofErrorLog
		<< "\n#SINDYSTDLOG\n"
		<< "�Ή���\t���C����\t�I�u�W�F�N�gID(FID)\tX(�o�x)\tY(�ܓx)\t�G���[���x��\t�G���[�R�[�h\t�G���[���b�Z�[�W\t���b�V���R�[�h\t����(��r��)[m]\t����(��r��)[m]\t��������[m]\t�������ኄ��[%]\n";
}

/**
 * @brief	�����`�F�b�N
 * @param	argc	[in]	�����̐�
 * @param	argv	[in]	����
 * @return	bool
 */
bool	CCompShape2Shape::arg_check(int argc, char *argv[])
{
	int		a_OptionCount[OPTION_MAX];		// �I�v�V�����d���J�E���g�p
	int		i = 0;
	CString csTemp;
	string	sTemp;

	// �c�[���o�[�W�����\��
	CVersion	a_cvTool_Var;
	CString		a_csVar;
	a_csVar		= a_cvTool_Var.GetFileVersion();
	m_sVersion	= (char*)(_bstr_t(a_csVar));

	for( i = 0 ; i < OPTION_MAX ; i++ )
	{
		a_OptionCount[i]	= 0;
	}

	for( i = 0 ; i < argc ; ++i )
	{
		if( strcmp( argv[i], "-m" ) == 0 )
		{
			++a_OptionCount[0];
			csTemp = argv[++i];
			if( !loadlist( csTemp, m_vsMesh ) )
			{
				cerr << "#ERROR ���b�V�����X�g�̓ǂݍ��݂Ɏ��s�I" << endl;
				return false;
			}
		}
		else if( strcmp( argv[i], "-i1" ) == 0 )
		{
			if( argc < i + 2 )	return false;
			++a_OptionCount[1];
			m_sOriFPass		= argv[++i];
		}
		else if( strcmp( argv[i], "-i2" ) == 0 )
		{
			if( argc < i + 2 )	return false;
			++a_OptionCount[2];
			m_sDestFPass	= argv[++i];
		}
		else if( strcmp( argv[i], "-l" ) == 0 )
		{
			if( argc < i + 2 )	return false;
			++a_OptionCount[3];
			m_sLayerName	= argv[++i];
		}
		else if( strcmp( argv[i], "-o" ) == 0 )		
		{
			++a_OptionCount[4];
			m_ofErrorLog.open( argv[++i], ios::out );
			if( m_ofErrorLog == false )
			{
				cerr << "#ERROR ���O�o�̓t�@�C���̃I�[�v���Ɏ��s�I" << endl;
				return false;
			}
		}
		else if( strcmp( argv[i], "-FID" ) == 0 )
		{
			++a_OptionCount[5];
			m_iExeMode		|= ON_FID_MODE;
		}
		else if( strcmp( argv[i], "-d" ) == 0 )
		{
			++a_OptionCount[6];
			m_sDataSet		= argv[++i];
			if( strcmp( m_sDataSet.c_str(), "ROAD" ) == 0 )
				m_iExeMode		|= ON_ROAD_MODE;
			else
				m_iExeMode		|= ON_MAP_MODE;
		}
		else if( strcmp( argv[i], "-fss" ) == 0 )
		{
			++a_OptionCount[7];
			m_iExeMode		|= ON_FSPECIFY_MODE;
			sTemp			= argv[++i];
			m_vsFSpecify.push_back( sTemp );
		}
		else if( strcmp( argv[i], "-fsl" ) == 0 )
		{
			++a_OptionCount[8];
			m_iExeMode		|= ON_FSPECIFY_MODE;
			csTemp = argv[++i];
			if( !loadlist( csTemp, m_vsFSpecify ) )
			{
				cerr << "#ERROR ��r�Ώۃt�B�[���h���X�g�̓ǂݍ��݂Ɏ��s�I" << endl;
				return false;
			}
		}
		else if( strcmp( argv[i], "-BIN" ) == 0 )
		{
			++a_OptionCount[9];
			m_iExeMode		|= ON_EXE_BINARY_MODE;
		}
		else if( strcmp( argv[i], "-ATTR" ) == 0 )
		{
			++a_OptionCount[10];
			m_bAttr = true;
		}
		else if( strcmp( argv[i], "-SHP" ) == 0 )
		{
			++a_OptionCount[11];
			m_bShp = true;
		}
		else if( strcmp( argv[i], "-dist_b" ) == 0 )
		{
			++a_OptionCount[12];
			m_dDistBorder = atof(argv[++i]);
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

	// �I�v�V����FID���w�肳��Ă��Ȃ���΁AOBJECTID���[�h
	if( a_OptionCount[5] == 0 )
		m_iExeMode		|= ON_OBJECTID_MODE;

	// �I�v�V����-fss��-fsl�Ŕ�r�Ώۃt�B�[���h���w�肳��Ă��Ȃ���΁A�S��r���[�h
	if( a_OptionCount[7] == 0 && a_OptionCount[8] == 0 )
		m_iExeMode		|= ON_FALL_MODE;
	
	// �I�v�V����-BIN���w�肳��Ă��Ȃ���΁A�ʏ���s���[�h
	if( a_OptionCount[9] == 0 )
		m_iExeMode		|= ON_EXE_NORMAL_MODE;

	//�I�v�V����-ATTR�A-SHP�̂ǂ��炩���w�肳��Ă��Ȃ���΁A�G���[
	if( a_OptionCount[10] == 0 && a_OptionCount[11] == 0)
	{
		cerr << "#ERROR -ATTR �� -SHP �I�v�V�������w�肵�Ă�������!" << endl;
		return false;
	}

	// �I�v�V�����d���`�F�b�N
	for( i = 0 ; i < OPTION_MAX ; i++ )
	{
		if( a_OptionCount[i] > 1 ){
			cerr << "#ERROR �I�v�V�����̎w�肪�d�����Ă��܂��I" << endl;
			return false;
		}
		if( (i != 5 && i != 7 && i != 8 && i != 9 && i !=10 && i !=11 && i !=12 ) && a_OptionCount[i] == 0){
			cerr << "#ERROR �I�v�V�������s�����Ă��܂��A�m�F���Ă��������I" << endl;
			return false;
		}
	}
	if( (a_OptionCount[11] == 1) && (a_OptionCount[12] == 0 )){
		cerr << "#ERROR �I�v�V�������s�����Ă��܂��A�m�F���Ă��������I" << endl;
		return false;
	}
	return true;
}


bool	CCompShape2Shape::loadlist(LPCTSTR lpszFile, vector<string>& vsVectorList )
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
 * @brief	���b�V���P�ʃt�@�C�����̍쐬
 * @param	iMeshCode	[in]	���b�V���R�[�h
 * @param	cPathName	[in]	�t�@�C����_�p�X
 * @return	string		[out]	�t�@�C���̃t���p�X
*/
string	CCompShape2Shape::make_mesh_fname ( int iMeshCode,	char* cpPathName )
{
	char	a_cSubStr[256];
	memset	( a_cSubStr, '\0', 256 );

	string	a_sSubDir		= "";
	string	a_sFullPass		= "";
	char	a_cTemp[4096];	

	int			a_iDegit = 0;
	a_iDegit	= m_cCrd.GetMeshDigit( iMeshCode );

	switch( a_iDegit )
	{
	case 2:
		sprintf( a_cSubStr, "%02d", iMeshCode );
		break;
	case 4:
		sprintf( a_cSubStr, "%d", iMeshCode );
		break;
	case 6:
		sprintf( a_cSubStr, "%d\\%d", iMeshCode/100, iMeshCode );
		break;
	case 8:
		sprintf( a_cSubStr, "%d\\%d\\%d", iMeshCode/10000, iMeshCode/100, iMeshCode );
		break;
	default:
		break;
	}
	a_sSubDir	= a_cSubStr;

	sprintf( a_cTemp, "%s\\%s\\%s\\%02d_%s", cpPathName, (char*)m_sDataSet.c_str(), (char*)a_sSubDir.c_str(), iMeshCode, (char*)m_sLayerName.c_str() );

	a_sFullPass	= a_cTemp;
	
	return a_sFullPass;
}

/**
 * @brief	Shape�t�@�C����ǂݍ��݁Amultimap<ID,shp1>�ɗ��Ƃ�����
 * @param	sFullPass	[in]	�ǂݍ���Shape�t�@�C���̃t���p�X
 * @param	mmShp1		[out]	Shape�t�@�C�����g�i�[�p�N���X�W���R���e�i
 * @return	bool
 */
bool	CCompShape2Shape::load_shape_info( string sFullPass, int iMesh, mmShp1& mmShp1 )
{
	CSHPHandle		a_cSHPHdl;	/// �V�F�C�v�n���h���N���X(Shapelib)
	CDBFHandle		a_cDBFHdl;	/// DBF�t�@�C���n���h���N���X


	if( !a_cDBFHdl.open((char*)sFullPass.c_str(), "rb") )
	{
		cerr << "#ERROR DBF�t�@�C��<" << sFullPass << ">�̃I�[�v���Ɏ��s�I" << endl;
		m_ofErrorLog << "#ERROR DBF�t�@�C��<" << sFullPass << ">�̃I�[�v���Ɏ��s�I" << endl;
		return false;
	}
	
	// ���R�[�h���̎擾
	int a_iRecCount		= 0;
	a_iRecCount			= a_cDBFHdl.get_RecordCount();
	
	// �t�B�[���h���̎擾
	int	a_iFieldCount	= 0;
	a_iFieldCount		= a_cDBFHdl.get_FieldCount();

	// �t�B�[���h���̎擾�p
	vector<int>			a_viField_Indexes;		// �t�B�[���h�C���f�b�N�X
	vector<string>		a_vsField_Names;		// �t�B�[���h��
	vector<int>			a_viField_Types;		// �t�B�[���h�̃f�[�^�^�C�v

	int		a_iFidx;
	for( a_iFidx = 0 ; a_iFidx < a_iFieldCount ; a_iFidx++ )
	{
		char*	a_cpFieldName	= NULL;
		int		a_iWidth		= 0;
		int		a_iDecimals		= 0;
		int		a_iType			= 0;

		// �t�B�[���h���擾
		a_iType			= a_cDBFHdl.get_FieldInfo( a_iFidx, &a_cpFieldName, &a_iWidth, &a_iDecimals );

		a_viField_Indexes.push_back	( a_iFidx );
		a_vsField_Names.push_back	( a_cpFieldName );
		a_viField_Types.push_back	( a_iType );
	}

	// �e���R�[�h�̏��擾
	int		a_iCount = 0;
	for( a_iCount = 0 ; a_iCount < a_iRecCount ; a_iCount++ )
	{
		shp1	a_shpCurData;
		
		// �t�B�[���h���ۑ�
		a_shpCurData.iField_num		= a_iFieldCount;

		// �t�B�[���h�̃��[�v
		for( a_iFidx = 0 ; a_iFidx < a_iFieldCount ; a_iFidx++ )
		{
			int			a_iData;
			double		a_dData;
			const char*	a_cpData;
			wchar_t		a_wcWstr[512];
			string		temp;

			memset( a_wcWstr, '\0', 256 );

			bool		a_bFailed_Flag	= false;

			// �t�B�[���h�������ԂɊi�[���Ă���
			a_shpCurData.vsField_name.push_back( a_vsField_Names[a_iFidx] );
			//a_shpCurData.msField_name.insert( map<int, string>::value_type( a_iFidx, a_vsField_Names[a_iFidx] ) );

			// �t�B�[���h�^�C�v�ɂ�菈������
			switch( a_viField_Types[a_iFidx] )
			{
			case		FTString:
				a_cDBFHdl.read_Attribute( a_iCount, a_iFidx, &a_cpData );
				MultiByteToWideChar( CP_UTF8, 0, a_cpData, -1, a_wcWstr, 512 );
				break;

			case		FTInteger:
				a_cDBFHdl.read_Attribute( a_iCount, a_iFidx, &a_iData );
				temp	= boost::lexical_cast<string>(a_iData);
				MultiByteToWideChar( CP_UTF8, 0, temp.c_str(), -1, a_wcWstr, 512 );
				break;

			case		FTDouble:
				a_cDBFHdl.read_Attribute( a_iCount, a_iFidx, &a_dData );
				temp	= boost::lexical_cast<string>(a_dData);
				MultiByteToWideChar( CP_UTF8, 0, temp.c_str(), -1, a_wcWstr, 512 );
				break;
			
			default:
				// �����Ă��Ȃ��^�C�v
				a_bFailed_Flag	= true;
				break;
			}
			
			if( a_bFailed_Flag == true )
				return false;
			if( ( m_iExeMode & ON_FID_MODE ) && ( a_vsField_Names[a_iFidx] == "OBJECTID" || a_vsField_Names[a_iFidx] == "IPC_ID" ) )
				continue;
			else if( ( m_iExeMode & ON_OBJECTID_MODE ) && ( a_vsField_Names[a_iFidx] == "OBJECTID" || a_vsField_Names[a_iFidx] == "IPC_ID" ) )
				a_shpCurData.shp_ID		= a_iData;
			else
				a_shpCurData.Fields.insert( map<string, wstring>::value_type( a_vsField_Names[a_iFidx], a_wcWstr ) );
		}
		// FID���[�h�Ȃ�΁A�����ŃC���f�b�N�X�ԍ���shp_ID�Ɋi�[
		if( m_iExeMode & ON_FID_MODE )
			a_shpCurData.shp_ID		= a_iCount + 1;

		// shape�̕�ԓ_���̒ǉ�
		if( !a_cSHPHdl.open((char*)sFullPass.c_str(), "rb") )
		{
			// ���H�n��shp�t�@�C�����������̂�����̂ŁA�n�}�n�̂݃G���[
			if( m_iExeMode	|= ON_MAP_MODE )
			{				
				cerr << "#ERROR SHP�t�@�C��<" << sFullPass << ">�̃I�[�v���Ɏ��s�I" << endl;
				m_ofErrorLog << "#ERROR SHP�t�@�C��<" << sFullPass << ">�̃I�[�v���Ɏ��s�I" << endl;
				return false;
			}
		}
		else
		{
			CSHPObject		a_cTmpOBJ;
			CSHPPoint		a_cTmpPoint;

			if( a_cSHPHdl.read_Object( a_iCount, &a_cTmpOBJ ) == -1 )
			{
				cerr << "#ERROR shapeOBJECT�쐬���s" << endl;
				m_ofErrorLog << "#ERROR shapeOBJECT�쐬���s" << endl;
				return false;
			}
			else
			{
				// ��ԓ_���擾
				//a_shpCurData.point_num = a_cTmpOBJ.get_PointCount();
				
				// (x,y)�擾
				a_cTmpOBJ.get_Point( 0, &a_cTmpPoint );
				a_shpCurData.shp_dX = a_cTmpPoint.get_X();
				a_shpCurData.shp_dY = a_cTmpPoint.get_Y();
			}
		}
		

		// ID�擾�m�F
		if( a_shpCurData.shp_ID == 0 )
		{
			cerr << "#ERROR ID���擾�ł��܂���ł����I" << endl;
			m_ofErrorLog << "#ERROR ID���擾�ł��܂���ł����I" << endl;
			return false;
		}
		a_shpCurData.iMeshCode	= iMesh;
		mmShp1.insert( mmShp1::value_type( a_shpCurData.shp_ID, a_shpCurData ) );
	}
	a_cSHPHdl.close();
	a_cDBFHdl.close();

	return true;
}

/**
 * @brief	���Shape�t�@�C���̑����ƕ�ԓ_�����r���A�قȂ�΃G���[�o��
 * @param	mmShpOri	[in]	��r��Shape�t�@�C�����g�i�[�p�N���X�W���R���e�i<ID, shp1�{��>
 * @param	mmShpDest	[in]	��r��Shape�t�@�C�����g�i�[�p�N���X�W���R���e�i<ID, shp1�{��>
 */
bool	CCompShape2Shape::comp_shape_info( mmShp1 mmShpOri, mmShp1 mmShpDest )
{
	bool bRet = true;
	itrmmShp1						a_itrmmShpOri, a_itrmmShpDest;				// shape�R���e�i�C�e���[�^
	map<string, wstring>::iterator	a_itrmFieldsOri, a_itrmFieldsDest;			// �R���e�i���̃t�B�[���h�̃C�e���[�^
	vector<string>::iterator		a_itrvsFieldNameOri, a_itrvsFieldNameDest;	// �t�B�[���h���̃C�e���[�^
	int								i = 0;										// ���[�v�p

	// ��r���S��
	for( a_itrmmShpOri = mmShpOri.begin() ; a_itrmmShpOri != mmShpOri.end() ; a_itrmmShpOri++ )
	{
		a_itrmmShpDest	= mmShpDest.find( a_itrmmShpOri->first );
		if( a_itrmmShpDest != mmShpDest.end() )
		{
			if( a_itrmmShpDest->second.check_flag == 0 )
			{
				// ��ԓ_����r
				//if( a_itrmmShpOri->second.point_num != a_itrmmShpDest->second.point_num )
				//{
				//	m_ofErrorLog
				//		<< "0\t" << m_sLayerName << "\t" << a_itrmmShpOri->second.shp_ID << "\t"
				//		<< a_itrmmShpOri->second.shp_dX << "\t" << a_itrmmShpOri->second.shp_dY << "\tERROR\t"
				//		<< "POINTERR\t��ԓ_�̌���<" << a_itrmmShpOri->second.point_num << ">��<"
				//		<< a_itrmmShpDest->second.point_num << ">�ňقȂ�\t" << a_itrmmShpOri->second.iMeshCode << endl;
				//
				//}
				
				// �t�B�[���h���̔�r
				if( a_itrmmShpOri->second.iField_num != a_itrmmShpDest->second.iField_num )
				{
					m_ofErrorLog
						<< "0\t" << m_sLayerName << "\t" << a_itrmmShpOri->second.shp_ID << "\t"
						<< a_itrmmShpOri->second.shp_dX << "\t" << a_itrmmShpOri->second.shp_dY << "\tERROR\t"
						<< "FIELDNUMERR(����)\t�t�B�[���h�̌����قȂ��Ă��܂�\t" << a_itrmmShpOri->second.iMeshCode << endl;
					bRet=false;
				}

				// �S�Ẵt�B�[���h�����Ԓʂ�i�[����Ă��邩��r
				for( i = 0 ; i < a_itrmmShpOri->second.iField_num ; i++ )
				{
					if( strcmp( a_itrmmShpOri->second.vsField_name[i].c_str(), a_itrmmShpDest->second.vsField_name[i].c_str() ) != 0 )
					{
						m_ofErrorLog 
							<< "0\t" << m_sLayerName << "\t" << a_itrmmShpOri->second.shp_ID << "\t"
							<< a_itrmmShpOri->second.shp_dX << "\t" << a_itrmmShpOri->second.shp_dY << "\tERROR\t"
							<< "FIELDSEQERR(����)\t�t�B�[���h�̏��Ԃ��قȂ��Ă��܂�\t" << a_itrmmShpOri->second.iMeshCode << endl;
						bRet=false;
					}
				}
				a_itrmFieldsDest	= a_itrmmShpDest->second.Fields.begin();
				// �S�Ẵt�B�[���h�̑���������������r(���Ńt�B�[���h�̏��Ԃ��`�F�b�N���Ă���̂ŁA�t�B�[���h���͓��������ƑO��)
				for( a_itrmFieldsOri = a_itrmmShpOri->second.Fields.begin() ; a_itrmFieldsOri != a_itrmmShpOri->second.Fields.end() ; a_itrmFieldsOri++ )
				{				
					if( wcscmp( a_itrmFieldsOri->second.c_str(), a_itrmFieldsDest->second.c_str() ) != 0 )
					{
						m_ofErrorLog 
							<< "0\t" << m_sLayerName << "\t" << a_itrmmShpOri->second.shp_ID << "\t"
							<< a_itrmmShpOri->second.shp_dX << "\t" << a_itrmmShpOri->second.shp_dY << "\tERROR\t"
							<< "ATTRERR(����)\t�t�B�[���h<" << a_itrmFieldsOri->first.c_str() << ">�̒l���قȂ�\t"
							<< a_itrmmShpOri->second.iMeshCode << endl;
						bRet=false;
					}
					a_itrmFieldsDest++;
				}
				a_itrmmShpOri->second.check_flag	= 1;
				a_itrmmShpDest->second.check_flag	= 1;
			}
		}
	}
	
	// ���`�F�b�N�̂��̂��c���Ă��Ȃ����H
	for( a_itrmmShpOri = mmShpOri.begin() ; a_itrmmShpOri != mmShpOri.end() ; a_itrmmShpOri++ )
	{
		if( a_itrmmShpOri->second.check_flag == 0 )
		{
			m_ofErrorLog
				<< "0\t" << m_sLayerName << "\t" << a_itrmmShpOri->second.shp_ID << "\t"
				<< a_itrmmShpOri->second.shp_dX << "\t" << a_itrmmShpOri->second.shp_dY << "\tERROR\t"
				<< "DATAFINDERR(����)\t�Ή�����ID�̂��̂���r��ɂȂ����`�F�b�N�ł�\t"
				<< a_itrmmShpOri->second.iMeshCode << endl;
			bRet=false;
		}
	}
	for( a_itrmmShpDest = mmShpDest.begin() ; a_itrmmShpDest != mmShpDest.end() ; a_itrmmShpDest++ )
	{
		if( a_itrmmShpDest->second.check_flag == 0 )
		{
			m_ofErrorLog
				<< "0\t" << m_sLayerName << "\t" << a_itrmmShpDest->second.shp_ID << "\t"
				<< a_itrmmShpDest->second.shp_dX << "\t" << a_itrmmShpDest->second.shp_dY << "\tERROR\t"
				<< "DATAFINDERR(����)\t�Ή�����ID�̂��̂���r���ɂȂ����`�F�b�N�ł�\t"
				<< a_itrmmShpDest->second.iMeshCode << endl;
			bRet=false;
		}
	}
	return bRet;
}

/**
 * @brief	Shape�t�@�C�����o�C�i���œǂݍ��݁Ashp2�ɗ��Ƃ�����
 * @param	sFullPass	[in]	�ǂݍ���Shape�t�@�C���̃t���p�X
 * @param	iMesh		[in]	���b�V���R�[�h
 * @param	cShp2		[out]	Shape�t�@�C�����g�i�[�p�N���X�i�o�C�i���j
 * @return	bool
 */
bool	CCompShape2Shape::load_shape_info_bin(std::string sFullPass, int iMesh, shp2 &cShp2)
{
	ifstream			a_ifShapeBin;				// Shape�t�@�C���ǂݍ��ݗp�i�o�C�i���j
	int					i = 0, j = 0, k = 0;		// ���[�v�p

	cShp2.iMeshCode		= iMesh;

	// ���ڃt�@�C�����J���̂ŁA.dbf�ǉ�
	sFullPass			+= ".dbf";

	// �o�C�i�����[�h��Shape�t�@�C���I�[�v��
	a_ifShapeBin.open( sFullPass.c_str(), ios::in|ios::binary );
	
	if( !a_ifShapeBin )
	{
		cerr << "#ERROR �t�@�C��<" << sFullPass << ">�̃I�[�v���Ɏ��s�I" << endl;
		m_ofErrorLog << "#ERROR �t�@�C��<" << sFullPass << ">�̃I�[�v���Ɏ��s�I" << endl;
		return false;
	}

	// �K�v�Ȃ����ǂݔ�΂�
	a_ifShapeBin.seekg( 4 );
	
	char				a_cRecNum[4];
	char				a_cHeadSize[2];
	char				a_cRecSize[2];
	char				a_cFieldName[11];
	char				a_cUnicodeComp[2];
	
	// �w�b�_�I�_��r�p�����i0DH�j
	a_cUnicodeComp[0]	= 0x0d;

	// �����t�B�[���h�󔒔�r�p
	a_cUnicodeComp[1]	= 0x20;

	memset( a_cRecNum, '\0', sizeof( a_cRecNum ) );
	memset( a_cHeadSize, '\0', sizeof( a_cHeadSize ) );
	memset( a_cRecSize, '\0', sizeof( a_cRecSize ) );
	memset( a_cFieldName, '\0', sizeof( a_cFieldName ) );

	// ���R�[�h���擾
	a_ifShapeBin.read( a_cRecNum, sizeof( a_cRecNum ) );
	memcpy( &cShp2.iRec_num, a_cRecNum, sizeof( a_cRecNum ) );

	// �w�b�_�T�C�Y�擾
	a_ifShapeBin.read( a_cHeadSize, sizeof( a_cHeadSize ) );
	memcpy( &cShp2.iHeadSize, a_cHeadSize, sizeof( a_cHeadSize ) );

	// 1���R�[�h�̃T�C�Y�擾
	a_ifShapeBin.read( a_cRecSize, sizeof( a_cRecSize ) );
	memcpy( &cShp2.iRecSize, a_cRecSize, sizeof( a_cRecSize ) );

	string	a_sTemp;			// �t�B�[���h���A1���R�[�h�擾�p
	char	a_cID[128];			// �擾�������R�[�h����ID�����̔������悤
	int		a_iID	= 0;		// �L�[�ɂ���ID�i�[�p
	char	a_cIDFieldSize[1];	// ID�t�B�[���h�T�C�Y�i�[�p
	int		a_iIDFieldSize = 0;	// ID�t�B�[���h�T�C�Y�i�[�p
	int		a_iFlag	= 0;		// �`�F�b�N�t���O=0���i�[�p


	memset( a_cIDFieldSize, '\0', sizeof( a_cIDFieldSize ) );

	// �K�v�Ȃ����ǂݔ�΂�
	a_ifShapeBin.seekg( 20, ios::cur );
	
	// �w�b�_�I�_�����������܂�
	while(1)
	{
		// 1�t�B�[���h���擾
		a_ifShapeBin.read( a_cFieldName, sizeof( a_cFieldName ) );

		// �擪���w�b�_�I�_�����Ȃ�΃w�b�_���I��
		if( a_cFieldName[0] ==  a_cUnicodeComp[0] )
			break;
		if( m_iExeMode & ON_OBJECTID_MODE && ( strcmp( a_cFieldName, "OBJECTID" ) == 0 || strcmp( a_cFieldName, "IPC_ID" ) == 0 ) )
		{
			// �K�v�Ȃ����ǂݔ�΂�
			a_ifShapeBin.seekg( 5, ios::cur );
			
			// �t�B�[���h�T�C�Y�ǂ݂���	
			a_ifShapeBin.read( a_cIDFieldSize, 1 );
			memcpy( &a_iIDFieldSize, a_cIDFieldSize, sizeof( a_cIDFieldSize ) );
			
			// �K�v�Ȃ����ǂݔ�΂�
			a_ifShapeBin.seekg( 15, ios::cur );
			
			// ���̕��Ńt�B�[���h�T�C�Y���i�[����ĂȂ���΁AFID�̑���Ƀ��R�[�h�ԍ��̃V�[�P���X���i�[����B
		}
		else
		{
			// �K�v�Ȃ����ǂݔ�΂�
			a_ifShapeBin.seekg( 21, ios::cur );
		}

		a_sTemp		= a_cFieldName;
		cShp2.vsField_name.push_back( a_sTemp );
	}

	// �f�[�^���ŏ��փV�[�N���Ă���J�n
	a_ifShapeBin.seekg( cShp2.iHeadSize, ios::beg );
	
	// �w�b�_�ƃ��R�[�h�̐؂�ڂ̗]����ǂݔ�΂�
	a_ifShapeBin.seekg( 1, ios::cur );

	// 1���R�[�h���̃o�C�i��������X�ɃN���X�̒���multiset�ɃI�u�W�F�N�gID���L�[�Ɋi�[����
	for( i = 0 ; i < cShp2.iRec_num ; i++ )
	{
		// ������
		a_iID		= 0;

		char *a_cData = new char[cShp2.iRecSize];
		memset( a_cData, '\0', cShp2.iRecSize );

		// 1���R�[�h���̃o�C�i������ۂ��Ǝ擾
		a_ifShapeBin.read( a_cData, cShp2.iRecSize );

		// OBJECTID�EIPC_ID�̃t�B�[���h�T�C�Y���擾����Ă���΁A��������ID���擾
		if( a_iIDFieldSize != 0 )
		{
			memset( a_cID, '\0', sizeof( a_cID ) );
			memcpy( &a_cID, a_cData, a_iIDFieldSize );
			
			// �L�[�p�̃I�u�W�F�N�gID��INT�ɕϊ�����B
			a_iID	= atoi( a_cID );
		}
		
		// ���̎��_��OBJECTID�EIPC_ID���i�[����Ă��Ȃ���΁AFID���[�h��OBJECTID�̂Ȃ����C���Ȃ̂ŁA���R�[�h�ԍ����i�[����B
		if( a_iID	== 0 )
		{
			a_iID	= i + 1;
		}
		
		// string �̃R���X�g���N�^�ŃT�C�Y���w�肷�邱�ƂŁA�o�C�i����S�̂���������i�[�����悤�ɂ���
		cShp2.msBinData.insert( multimap<int, pair<int, string> >::value_type( a_iID,  pair<int, string>( a_iFlag, string( a_cData, cShp2.iRecSize ) ) ) );

		delete [] a_cData;
	}
	return true;
}


/**
 * @brief	���Shape�t�@�C���̃��R�[�h���o�C�i���Ŕ�r���A�قȂ�΃G���[�o��
 * @param	cShp2Ori	[in]	��r��Shape�t�@�C�����g�i�[�p�N���X�i�o�C�i���j
 * @param	cShp2Dest	[in]	��r��Shape�t�@�C�����g�i�[�p�N���X�i�o�C�i���j
 */
bool	CCompShape2Shape::comp_shape_info_bin(shp2 cShp2Ori, shp2 cShp2Dest)
{
	bool bRet = true;
	// �t�B�[���h���C�e���[�^
	vector<string>::iterator		a_itrvsFiNameOri, a_itrvsFiNameDest;
	
	// ���R�[�h���Ƃ̃o�C�i���f�[�^�C�e���[�^
	multimap<int, pair<int, string>>::iterator	a_itrmsBiDataOri, a_itrmsBiDataDest;
	//vector<string>::iterator		a_itrvsBiDataOri, a_itrvsBiDataDest;
	
	// �t�B�[���h���̌���r
	if( cShp2Ori.vsField_name.size() != cShp2Dest.vsField_name.size() )
	{
		m_ofErrorLog
			<< "0\t" << m_sLayerName << "\t\t\t\tERROR\tFIELDNUMEERR\t"
			<< "�t�B�[���h�̌����قȂ��Ă��܂�\t" << cShp2Ori.iMeshCode << endl;
		bRet=false;
	}

	// �e�t�B�[���h���̔�r
	a_itrvsFiNameDest		= cShp2Dest.vsField_name.begin();
	for( a_itrvsFiNameOri = cShp2Ori.vsField_name.begin() ; a_itrvsFiNameOri != cShp2Ori.vsField_name.end() ; a_itrvsFiNameOri++ )
	{
		if( strcmp( a_itrvsFiNameOri->c_str(), a_itrvsFiNameDest->c_str() ) != 0 )
		{
			m_ofErrorLog
				<< "0\t" << m_sLayerName << "\t\t\t\tERROR\tFIELDNAMEERR\t�t�B�[���h����<"
				<< a_itrvsFiNameOri->c_str() << ">��<" << a_itrvsFiNameDest->c_str()
				<< ">�ňقȂ��Ă��܂�\t" << cShp2Ori.iMeshCode << endl;
			bRet=false;
		}
		a_itrvsFiNameDest++;
	}

	// �o�C�i�����R�[�h�̌���r
	if( cShp2Ori.msBinData.size() != cShp2Dest.msBinData.size() )
	{
		m_ofErrorLog
			<< "0\t" << m_sLayerName << "\t\t\t\tERROR\tDATANUMEERR\t"
			<< "�f�[�^�̌����قȂ��Ă��܂�\t" << cShp2Ori.iMeshCode << endl;
		bRet=false;
	}
	for( a_itrmsBiDataOri = cShp2Ori.msBinData.begin() ; a_itrmsBiDataOri != cShp2Ori.msBinData.end() ; a_itrmsBiDataOri++)
	{
		// ���ꂼ���Shape�̃o�C�i���f�[�^�񂩂�ID���L�[�Ɍ������A�R�Â�����A�o�C�i�����r
		a_itrmsBiDataDest		= cShp2Dest.msBinData.find( a_itrmsBiDataOri->first );
		
		// �Ή�����ID�̂��̂�������Ȃ�
		if( a_itrmsBiDataDest	== cShp2Dest.msBinData.end() )
		{
			m_ofErrorLog
				<< "0\t" << m_sLayerName << "\t" << a_itrmsBiDataOri->first 
				<< "\t\t\tERROR\tBiDATAFINDERR\t�Ή�����ID�̂��̂���r��ɂ���܂���\t" 
				<< cShp2Ori.iMeshCode << endl;
			bRet=false;
		}

		// ��������
		else
		{
			// ����ID�����f�[�^�̃o�C�i������r����
			if( strcmp( a_itrmsBiDataOri->second.second.c_str(), a_itrmsBiDataDest->second.second.c_str() ) != 0 )
			{
				m_ofErrorLog
					<< "0\t" << m_sLayerName << "\t" << a_itrmsBiDataOri->first
					<< "\t\t\tERROR\tBiDATAERR\t�o�C�i���񂪔�r���Ɣ�r��ňقȂ��Ă��܂�\t"
					<< cShp2Ori.iMeshCode << endl;
				bRet=false;
			}
			
			// ���������ꍇ�͔�r���E��r��̃f�[�^�̃`�F�b�N�t���O�𗧂Ă�
			a_itrmsBiDataOri->second.first++;
			a_itrmsBiDataDest->second.first++;
		}
	}

	// ���`�F�b�N�̂��̂��c���Ă��Ȃ����H
	for( a_itrmsBiDataOri = cShp2Ori.msBinData.begin() ; a_itrmsBiDataOri != cShp2Ori.msBinData.end() ; a_itrmsBiDataOri++ )
	{
		if( a_itrmsBiDataOri->second.first == 0 )
		{
			m_ofErrorLog
				<< "0\t" << m_sLayerName << "\t" << a_itrmsBiDataOri->first
				<< "\t\t\tERROR\tBiDATAFINDERR\t�Ή�����ID�̂��̂���r��ɂȂ����`�F�b�N�ł�\t"
				<< cShp2Ori.iMeshCode << endl;
			bRet=false;
		}
	}
	for( a_itrmsBiDataDest = cShp2Dest.msBinData.begin() ; a_itrmsBiDataDest != cShp2Dest.msBinData.end() ; a_itrmsBiDataDest++ )
	{
		if( a_itrmsBiDataDest->second.first == 0 )
		{
			m_ofErrorLog
				<< "0\t" << m_sLayerName << "\t" << a_itrmsBiDataDest->first
				<< "\t\t\tERROR\tBiDATAFINDERR\t�Ή�����ID�̂��̂���r���ɂȂ����`�F�b�N�ł�\t"
				<< cShp2Dest.iMeshCode << endl;
			bRet=false;
		}
	}
	
	return bRet;
}


/**
 * @brief	���s
 * @return	bool
 */
int	CCompShape2Shape::execute()
{
	int iErrFlag = 0;
	int iRET = 0;
	
	// ���O�̏o�͂̍ہA�������ۂߍ��܂�Ȃ��悤�ɐݒ�
	m_ofErrorLog << setprecision(10);
	// �����������O�ɏo��
	::GetLocalTime( &m_st );
	print_ver();
	print_pass();
	print_begin_time();
	print_log_head();
	m_ofErrorLog.flush();

	vector<string>::iterator	a_itrMesh;
	for( a_itrMesh = m_vsMesh.begin() ; a_itrMesh != m_vsMesh.end() ; a_itrMesh++ )
	{
		cerr << "<" << a_itrMesh->c_str() << ">������....." << endl;
		
		int		a_iMesh	= atoi( (*a_itrMesh).c_str() );
		string	a_sFullPassOri, a_sFullPassDest;
		
		// �p�X�쐬(��r��)
		a_sFullPassOri		= make_mesh_fname( a_iMesh, (char*)m_sOriFPass.c_str() );
		
		// �p�X�쐬(��r��)
		a_sFullPassDest		= make_mesh_fname( a_iMesh, (char*)m_sDestFPass.c_str() );
	
		//�I�v�V������-ATTR���w�肳��Ă����ꍇ�A������r�`�F�b�N���s��
		if( m_bAttr ){
			// �m�[�}�����[�h
			if( m_iExeMode & ON_EXE_NORMAL_MODE )
			{
				mmShp1				a_mmShp1_Ori, a_mmShp1_Dest;

				// shape�ǂݍ���(��r��)
				if( !load_shape_info( a_sFullPassOri, a_iMesh, a_mmShp1_Ori ) )
				{
					cerr << "#ERROR ��r��Shape�t�@�C���̓ǂݍ��݂Ɏ��s�I" << endl;
					iErrFlag = 1;
					continue;
				}

				// shape�ǂݍ���(��r��)
				if( !load_shape_info( a_sFullPassDest, a_iMesh, a_mmShp1_Dest ) )
				{
					cerr << "#ERROR ��r��Shape�t�@�C���̓ǂݍ��݂Ɏ��s�I" << endl;
					iErrFlag = 1;
					continue;
				}

				// 2��shape�̑�����r
				if (!comp_shape_info( a_mmShp1_Ori, a_mmShp1_Dest ))
				{
					iRET = 3;
					cerr << "������r�ŃG���[�����݂��܂���" << endl;
				}

				// ���shape�N���X�̉��
				a_mmShp1_Ori.clear();
				a_mmShp1_Dest.clear();
			}
			
			// �o�C�i�����[�h
			else if( m_iExeMode & ON_EXE_BINARY_MODE )
			{
				shp2				a_cShp2_Ori, a_cShp2_Dest;

				// shape�ǂݍ���(��r��)
				if( !load_shape_info_bin( a_sFullPassOri, a_iMesh, a_cShp2_Ori ) )
				{
					cerr << " #ERROR ��r��Shape�t�@�C���̓ǂݍ��݂Ɏ��s�I" << endl;
					iErrFlag = 1;
					continue;
				}

				// shape�ǂݍ���(��r��)
				if( !load_shape_info_bin( a_sFullPassDest, a_iMesh, a_cShp2_Dest ) )
				{
					cerr << " #ERROR ��r��Shape�t�@�C���̓ǂݍ��݂Ɏ��s�I" << endl;
					iErrFlag = 1;
					continue;
				}

				// 2��shape�̑�����r
				if (!comp_shape_info_bin( a_cShp2_Ori, a_cShp2_Dest ))
				{
					iRET= 3;
					cerr << "������r�ŃG���[�����݂��܂���" << endl;
				}

			}else{
				return false;
			}
			if (iRET == 0){
				cerr << "������r�����I" << endl;
			}
		}
		//�I�v�V������-SHP���[�h���w�肳��Ă�����A�`��`�F�b�N�J�n
		if( m_bShp )
		{
			//�`��`�F�b�N
			CShapeChk aShapeChk(&m_ofErrorLog,a_iMesh,m_iExeMode,m_sLayerName,a_sFullPassOri,a_sFullPassDest,m_dDistBorder);
			if( !aShapeChk.app()){
				cerr << "�`���r�ŃG���[�����݂��܂���" << endl;
				iRET = 3;
			}else{
				cerr << "�`���r����!" << endl;
			}
		}

	}
	::GetLocalTime( &m_st );
	print_end_time();
	m_ofErrorLog.flush();

	if (iErrFlag == 1)
	{
		return 1;
	} else {
		return iRET;
	}
}
