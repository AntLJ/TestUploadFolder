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

/**
* @file InifileHelper.h
*
* @brief IniFile�֘A�𑀍삷��N���X�Q�̒�`�t�@�C��
*
* @author �Ð� �M�G
*
* @date   2001/12/10
*/
#if !defined(AFX_INIFILEHELPER_H__7746FE5E_5FFC_44A0_9BEC_76C538AAF0D0__INCLUDED_)
#define AFX_INIFILEHELPER_H__7746FE5E_5FFC_44A0_9BEC_76C538AAF0D0__INCLUDED_	//!< �d���C���N���[�h�h�~

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
* @class CInifileHelperRoot
*
* @brief ini�t�@�C���ւ̊�{�A�N�Z�X��񋟂���N���X
*/
class CInifileHelperRoot
{
public:
	CInifileHelperRoot( LPCTSTR lpcszPath = NULL);
	~CInifileHelperRoot();

	/**
	* @brief �ǂݍ��ݑΏۂ�INI�t�@�C�����ݒ�ς݂��e�X�g����
	*
	* @retval TRUE ini�t�@�C���͖��ݒ�
	* @retval FALSE ini�t�@�C���͐ݒ�ς�
	*/
	virtual BOOL IsEmpty();

	/**
	* @brief �ŗL�ݒ�t�@�C������̐��l�擾�֐�
	*
	* @param lpAppName [in]
	* @param lpKeyName [in]
	* @param nDefault [in]
	*
	* @return �擾�f�[�^
	*/
	int GetPrivateProfileInt( LPCTSTR lpAppName, LPCTSTR lpKeyName, int nDefaule);

	/**
	* @brief �ŗL�ݒ�t�@�C������̕�����擾�֐�
	*
	* @param lpAppName [in]
	* @param lpKeyName [in]
	* @param lpDefault [in]
	* @param cString [out]
	* @param nSize [in]
	*
	* @return �擾�f�[�^��
	*/
	DWORD GetPrivateProfileString( LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, CString& cString, int nSize = DEFAULT_ALLOC);

	/**
	* @brief �ŗL�ݒ�t�@�C���ւ̐��l�ݒ�֐�
	*
	* @param lpAppName [in]
	* @param lpKeyName [in]
	* @param nDefault [in]
	*
	* @retval �O(Zero) ����
	* @retval �O�ȊO ���s
	*/
	DWORD WritePrivateProfileInt( LPCTSTR lpAppName, LPCTSTR lpKeyName, int nData);

	/**
	* @brief �ŗL�ݒ�t�@�C���ւ̕�����ݒ�֐�
	*
	* @param lpAppName [in]
	* @param lpKeyName [in]
	* @param lpcszString [in]
	*
	* @retval �O(Zero) ����
	* @retval �O�ȊO ���s
	*/
	DWORD WritePrivateProfileString( LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpcszString);

	/**
	* @brief �Ǘ��Ώۂ�INI�t�@�C���擾�֐�
	*
	* @param cStrPath [out] �Ǘ��Ώ�INI�t�@�C���ւ̃p�X
	*/
	void GetPrivateProfilePath( CString& cStrPath);

	/**
	* @brief �Ǘ��Ώۂ�INI�t�@�C���ݒ�֐�
	*
	* @param lpszPath [in] �Ǘ��Ώۂɂ���INI�t�@�C���ւ̃p�X
	*/
	void SetPrivateProfilePath( LPCTSTR lpszPath);

private:
	enum MAX_LENGTH
	{
		DEFAULT_ALLOC = 512	//!< �f�t�H���g�̕�����擾�G���A�T�C�Y
	};
	CString m_cStrInifile;	//!< �Ǘ��Ώ�ini�t�@�C���̃p�X
};

/**
* @class CRasterInifileHelper
*
* @brief ���X�^�t�@�C���ǂݍ��݃R�}���h�ݒ�INI�t�@�C�p�N���X�B
*
* @note INI�t�@�C���̗�
* @note [rasterfile]
* @note basedir=C:\TChiba\ArcWORK\���X�^�f�[�^\
* @note purpose0=�n�`�}���X�^
* @note 
* @note [�n�`�}���X�^]
* @note ; ���̓R�[�h�̃^�C�g��
* @note codealias=���b�V���R�[�h
* @note ; ���̓R�[�h�̍ő咷�ŏ���
* @note ; 0�ȉ��ݒ�ł͖��`�F�b�N
* @note codemin=8
* @note codemax=8
* @note ; �R�[�h����̃p�X�ϊ�
* @note ; %.[������]s
* @note ; %s�ł��ׂ�
* @note code2path=%.4s\%.6s\C%s.tif
* @note subdir=�n�`�}\
* @note ; C:\TChiba\ArcWORK\���X�^�f�[�^\�n�`�}\xxxx\xxxxxx\Cxxxxxxxx.tif
**/
class CRasterInifileHelper : public CInifileHelperRoot
{
public:
	CRasterInifileHelper( LPCTSTR lpszPath = NULL);
	~CRasterInifileHelper();

	/**
	* @brief �x�[�X�f�B���N�g���p�X�擾�֐�
	*
	* @param cStrBaseDir [out] �x�[�X�f�B���N�g��
	*/
	void GetBaseDir( CString& cStrBaseDir);

	/**
	* @brief �x�[�X�f�B���N�g���p�X�ݒ�֐�
	*
	* @note ���X�^�t�@�C���ǂݍ��݃R�}���h�pINI�t�@�C���ցA�x�[�X�f�B���N�g����ݒ�
	*
	* @param lpcszBaseDir [in] �x�[�X�f�B���N�g��
	*/
	void WriteBaseDir( LPCTSTR lpcszBaseDir);

	/**
	* @brief ���X�^�̎�ގ擾�֐�
	*
	* @note ���X�^�t�@�C���ǂݍ��݃R�}���h�pINI�t�@�C������A���X�^��ʂ��擾
	*
	* @param nIndex [in] Purpose[n]�̃C���f�b�N�X�l
	* @param cStrPurpose [out] ���X�^��ʖ���
	*/
	void GetPurpose( int nIndex, CString& cStrPurpose);

	/**
	* @brief ���X�^�̎�ސݒ�֐�
	*
	* @note ���X�^�t�@�C���ǂݍ��݃R�}���h�pINI�t�@�C���ցA���X�^��ʂ�ݒ�
	*
	* @param nIndex [in] Purpose[n]�̃C���f�b�N�X�l
	* @param lpcszPurpose [in] ���X�^��ʖ���
	*/
	void WritePurpose( int nIndex, LPCTSTR lpcszPurpose);

	/**
	* @brief ���X�^�̍ő�\�������擾�֐�
	*
	* @note ���X�^�t�@�C���ǂݍ��݃R�}���h�pINI�t�@�C������A���X�^�ő�\���������擾
	*
	* @param nMax [out] ���X�^�ő�\������
	*/
	void GetRasterMax( int& nMax);

	/**
	* @brief ���̓R�[�h���̎擾�֐�
	*
	* @note ���̓R�[�h�^�C�g�����擾
	*
	* @param lpcszPurpose [in] ���X�^����
	* @param cStrAlias [out] ���̓R�[�h�^�C�g��
	*/
	void GetAlias( LPCTSTR lpcszPurpose, CString& cStrAlias);

	/**
	* @brief ���̓R�[�h���̐ݒ�֐�
	*
	* @note ���̓R�[�h�^�C�g����INI�t�@�C���֕ۑ�
	*
	* @param lpcszPurpose [in] ���X�^����
	* @param lpcszAlias [in] ���̓R�[�h�^�C�g��
	*/
	void WriteAlias( LPCTSTR lpcszPurpose, LPCTSTR lpcszAlias);

	/**
	* @brief ���̓R�[�h�ő咷�ŏ����擾�֐�
	*
	* @note �R�[�h���͂��T�|�[�g���邽�߁A�w��͈̔͂ɊY������ꍇ�݂̂n�j�{�^����Enable�ɂ��邽�߂ɗ��p����B
	*
	* @param lpcszPurpose [in] ���X�^����
	* @param nMax [out] �ő啶����
	* @param nMin [out] �ŏ�������
	*/
	void GetMaxMin( LPCTSTR lpcszPurpose, int& nMax, int& nMin);

	/**
	* @brief ���̓R�[�h�ő咷�ŏ����ݒ�֐�
	*
	* @param lpcszPurpose [in] ���X�^����
	* @param nMax [in] �ő啶����
	* @param nMix [in] �ŏ�������
	*/
	void WriteMaxMin( LPCTSTR lpcszPurpose, int nMax, int nMin);

	/**
	* @brief �R�[�h�p�X�ϊ������擾�֐�
	*
	* @note ���͂��ꂽ��������p�X�֕ϊ����邽�߂́A�ϊ�����擾����֐��B
	* @note spritf��%s�����p
	*
	* @param lpcszPurpose [in] ���X�^����
	* @param cStrSeed [out] �ϊ��핶����
	*/
	void GetSeed( LPCTSTR lpcszPurpose, CString& cStrSeed);

	/**
	* @brief ���X�^�t�@�C���g���q�擾�֐�
	*
	* @note ���X�^�t�@�C���g���q���擾����֐��B
	*
	* @param lpcszPurpose [in] ���X�^����
	* @param cStrExt [out] ���X�^�t�@�C���g���q
	*/
	void GetFileExtension( LPCTSTR lpcszPurpose, CString& cStrExt);

	/**
	* @param �R�[�h�p�X�ϊ������ݒ�֐�
	*
	* @param lpcszPurpose [in] ���X�^����
	* @param lpcszSeed [in] �ϊ��핶����
	*/
	void WriteSeed( LPCTSTR lpcszPurpose, LPCTSTR lpcszSeed);

	/**
	* @brief �T�u�f�B���N�g���擾�֐�
	*
	* @note �w��R�[�h����w��t�@�C���ւ̃p�X�ɕϊ�����Ăɗ��p����A�T�u�f�B���N�g���l
	* @note [BaseDir]\[Subdir]\[Seed]�ւ̕ϊ����ɗ��p
	*
	* @param lpcszPurpose [in] ���X�^����
	* @param cStrSubdir [out] �ϊ����ɗ��p�����T�u�f�B���N�g��������
	*/
	void GetSubdir( LPCTSTR lpcszPurpose, CString& cStrSubdir);

	/**
	* @brief �T�u�f�B���N�g���ݒ�֐�
	*
	* @param lpcszPurpose [in] ���X�^����
	* @param lpcszSubdir [in] �T�u�f�B���N�g��������
	*/
	void WriteSubdir( LPCTSTR lpcszPurpose, LPCTSTR lpcszSubdir);

   // void CRasterInifileHelper::GetPyramid( LPCTSTR lpcszPurpose, int *nPyramid);
	void GetPyramid(LPCTSTR lpcszPurpose, int& nPyramid);
	void WritePyramid(LPCTSTR lpcszPurpose, int nPyramid);
	void GetTransmissivity(LPCTSTR lpcszPurpose, int& nTransmissivity);
	void GetGroupLayer(LPCTSTR lpcszPurpose, int& nGroupLayer);
	


private:
	/**
	* @brief INI�t�@�C������̃f�[�^�擾���ɗ��p����o�b�t�@�T�C�Y
	*
	* @note �ݒ�͏\���Ȓl��K�X�ݒ�̂���
	*/
	enum MAX_LENGTH
	{
		_MAX_BASEDIR = 512,		//!< GetBaseDir�ŗ��p�����o�b�t�@�T�C�Y
		_MAX_PURPOSE = 512,		//!< GetPurpose�ŗ��p�����o�b�t�@�T�C�Y
		_MAX_CODEALIAS = 512,	//!< GetAlias�ŗ��p�����o�b�t�@�T�C�Y
		_MAX_CODE2PATH = 512,	//!< GetSeed�ŗ��p�����o�b�t�@�T�C�Y
		_MAX_SUBDIR = 512		//!< GetSubdir�ŗ��p�����o�b�t�@�T�C�Y
	};
};

/**
* @class CInifileHelper
*
* @brief SiNDY-e Tool�̐ݒ�t�@�C���ɃA�N�Z�X����A�w���p�N���X
*
* @note ���̃N���X�́A���W�X�g����ini�t�@�C���ւ̃A�N�Z�X���I�ɍs���N���X�B
*
* @note INI�t�@�C���̗�
* @note [currentdata]
* @note work=foobar
* @note user=Nanno,Darebeh
* @note [rasterfile]
* @note filepath=C:\TChiba\ArcWORK\Raster.ini
* @note [worklfile]
* @note filepath=C:\TChiba\ArcWORK\Work.ini
* @note [�f�t�H���g]
* @note layerfile0=C:\TChiba\ArcWORK\defaultsymbol\DBTEST.Intersection.lyr
* @note layerfile1=C:\TChiba\ArcWORK\defaultsymbol\DBTEST.Node.lyr
* @note layerfile2=C:\TChiba\ArcWORK\defaultsymbol\DBTEST.Road_Link.lyr
* @note layerfile3=C:\TChiba\ArcWORK\defaultsymbol\DBTEST.Parking_link.lyr
* @note layerfile4=C:\TChiba\ArcWORK\defaultsymbol\DBTEST.Grad.lyr
* @note layerfile5=C:\TChiba\ArcWORK\defaultsymbol\DBTEST.Segment_Attr.lyr
* @note [Tools]
* @note ;�ҏW�Ώی����͈�
* @note Tolerance=8
* @note [SketchTool]
* @note SplitRoadByRoad=1
* @note SplitRoadByNode=1
* @note [EditTool]
* @note SplitRoadByRoad=0
* @note SplitRoadByNode=0
* @note ;�^�[�Q�b�g���C���́A|(OR)�ŋ�؂�܂��B�擪�Ɩ����ɂ��K�v�ŁA�P�����w�肷��ꍇ�ɂ����l�ł��B
* @note ;TargetLayers=|Node|Road_Link|
* @note TargetLayers=|Node|Road_Link|Parking_link|Segment_Attr|Grad|Vics|Turn_Regulation|Guide|Direction_Guide|Lane|Base_Station|Base_Site|Base_Railway|Base_Line|
* @note ;�ړ���̃`�F�b�N�Ώۃ��C��
* @note CheckLayers=|Node|Road_Link|Parking_link|Segment_Attr|Grad|Vics|Turn_Regulation|Guide|Direction_Guide|Lane|Base_Station|Base_Site|Base_Railway|Base_Line|
* @note [DeleteTool]
*/
class CInifileHelper  
{
public:
	CInifileHelper();
	virtual ~CInifileHelper();
	
	/**
	* @brief SiNDY-e�̃C���X�g�[�����[�g�p�X���擾
	*
	* @param strInifilePath [out] INI�t�@�C���̃p�X
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL GetInstallRoot( CString& strInstallRoot );

	/**
	* @brief INI�t�@�C���̃t�@�C�������t���p�X�Ŏ擾
	*
	* @param strInifilePath [out] INI�t�@�C���̃p�X
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL GetInifilePath( CString& strInifilePath );

	/**
	* @brief �f�[�^�x�[�X�T�[�o����INI�t�@�C������擾
	*
	* @param strDBServerName [out] �f�[�^�x�[�X�T�[�o��
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL GetSDEServerName( CString& strDBServerName );

	/**
	* @brief ���X�^�t�@�C����`�t�@�C���ւ̃A�N�Z�X�w���p�N���X��������
	*
	* @param cStrRasterfilePath [out] ���������郉�X�^�t�@�C����`�t�@�C���A�N�Z�X�w���p
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL GetRasterfile( CRasterInifileHelper& cRasterfileHelper);

	/**
	* @brief ���X�^�t�@�C����`�t�@�C���̃p�X���擾
	*
	* @param cRasterfileHelper [out] ���X�^�t�@�C����`�t�@�C���̃p�X
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL GetRasterfilePath( CString& cRasterfileHelper);

	/**
	* @brief �V���{���ύX�p��`�t�@�C���̃p�X���擾
	*
	* @param strSymbolfilePath [out] �V���{���ύX�p�t�@�C���̃p�X
	*
	* @retval TRUE ����I��
	* @retval FALSE �ُ�I��
	*/
	BOOL GetSymbolfilePath( CString& strSymbolfilePath);

	BOOL GetAddLayerfilePath( CString& strAddLayerPath);

	/**
	* @brief ���[�U�̃��[�j���O���x���擾
	*
	* @return ���[�j���O���x��
	*/
	int GetWarningLevel( void);

	/**
	* @brief �I���G���A���a�l�擾
	*
	* @note �w�肳��Ă���l��Pixel�P�ʁB
	*
	* @return �I���G���A���a
	*/
	UINT GetSelectTolerance( void);

	/**
	* @brief �X�΃��C���t�@�C��
	*
	* @param cGrandLayerFile [out] �X�΃��C���[�p�̃��C���[�t�@�C��
	*
	* return �X�΃��C���[�p�̃��C���[�t�@�C��������
	*/
	int GetGradLayerFile( CString& cGradLayerFile);
	/**
	* @brief �X�΃��C��
	*
	* @param cGradLayer [out] �X�΃��C���[�����C���[���X�g�ɒǉ�����ۂ̃��C���[��
	*
	* @return �X�΃��C���[�����C���[���X�g�ɒǉ�����ۂ̃��C���[��������
	*/
	int GetGradLayer( CString& cGradLayer);


	/*
	/// �V���{���ύX�c�[���̃N���XID���擾
	BOOL GetChangeSymbolToolCLSID( LPCTSTR lpszSection, CString& strCLSID );
	/// �ړI�ʃV���{���ύX�p�̃��C���t�@�C������Ԃ��B������� FALSE�B
	BOOL GetChangeSymbolLayerFileName( LPCTSTR lpszSection, LONG index, CString& strLayerFilePath );
	*/
private:
	/**
	* @brief �C�j�V�����C�Y
	*
	* @note ���W�X�g�����Q�Ƃ��A�ݒ�t�@�C���̃p�X�擾���������Ȃ��B
	*/
	void Initialize();

	/// ���W�X�g���֌W

	/**
	* @brief ���W�X�g���̐ݒ�
	*/
	void  SetRegistryKey( LPCTSTR lpszRegistryKey );

	/**
	* @brief ���W�X�g����int�l�̏�������
	*/
	BOOL  WriteRegInt( LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue );

	/**
	* @brief ���W�X�g������int�l�̓ǂݍ���
	*/
	UINT  GetRegInt( LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault );

	/**
	* @brief ���W�X�g���֕�����̏�������
	*/
	BOOL  WriteRegString( LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue );

	/**
	* @brief ���W�X�g���֕�����̓ǂݍ���
	*/
	void  GetRegString( LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault, 
						LPTSTR lpReturnedString, DWORD nSize );
private:
	HKEY				m_hKey;				//!< SiNDY�̐ݒ��ۑ����Ă��郌�W�X�g���ւ̃n���h��
	CString				m_strInifilePath;	//!< �ݒ�t�@�C���ւ̂ς�
};

#include "InifileHelper.inl"

#endif // !defined(AFX_INIFILEHELPER_H__7746FE5E_5FFC_44A0_9BEC_76C538AAF0D0__INCLUDED_)
