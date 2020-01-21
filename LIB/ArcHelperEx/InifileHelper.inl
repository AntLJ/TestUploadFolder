#include "AheGlobals.h"
/**
* @file InifileHelper.inl
*
* @brief IniFile�֘A�𑀍삷��N���X�Q�̃C�����C���֐��t�@�C��
*/
inline CInifileHelperRoot::CInifileHelperRoot( LPCTSTR lpcszPath)
{
	m_cStrInifile = lpcszPath;
}

inline CInifileHelperRoot::~CInifileHelperRoot()
{
}

/// m_cStrInifile����`����Ă��邩�H�̃`�F�b�N
inline BOOL CInifileHelperRoot::IsEmpty()
{
	return m_cStrInifile.IsEmpty();
}

/// �ŗL�ݒ�t�@�C������̐��l�擾�֐�
inline int CInifileHelperRoot::GetPrivateProfileInt( LPCTSTR lpAppName, LPCTSTR lpKeyName, int nDefaule)
{
	ATLASSERT( !m_cStrInifile.IsEmpty());
	return ::GetPrivateProfileInt( lpAppName, lpKeyName, nDefaule, m_cStrInifile);
}

/// �ŗL�ݒ�t�@�C������̕�����擾�֐�
inline DWORD CInifileHelperRoot::GetPrivateProfileString( LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, CString& cString, int nSize)
{
	ATLASSERT( !m_cStrInifile.IsEmpty());
	DWORD dwResult = ::GetPrivateProfileString( lpAppName, lpKeyName, lpDefault, cString.GetBuffer( nSize), nSize, m_cStrInifile);
	cString.ReleaseBuffer();
	return dwResult;
}

/// �ŗL�ݒ�t�@�C������̐��l�ݒ�֐�
inline DWORD CInifileHelperRoot::WritePrivateProfileInt( LPCTSTR lpAppName, LPCTSTR lpKeyName, int nData)
{
	CString cStr;
	cStr.Format( TEXT( "%ld"), nData);
	return ::WritePrivateProfileString( lpAppName, lpKeyName, cStr, m_cStrInifile);
}

/// �ŗL�ݒ�t�@�C������̕�����ݒ�֐�
inline DWORD CInifileHelperRoot::WritePrivateProfileString( LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpcszString)
{
	return ::WritePrivateProfileString( lpAppName, lpKeyName, lpcszString, m_cStrInifile);
}

/// �Ǘ��Ώۂ�INI�t�@�C���擾�֐�
inline void CInifileHelperRoot::GetPrivateProfilePath( CString& cStrPath)
{
	cStrPath = m_cStrInifile;
}

/// �Ǘ��Ώۂ�INI�t�@�C���ݒ�֐�
inline void CInifileHelperRoot::SetPrivateProfilePath( LPCTSTR lpszPath)
{
	m_cStrInifile = lpszPath;
}


/// ���X�^��`�t�@�C���A�N�Z�X�N���X
inline CRasterInifileHelper::CRasterInifileHelper( LPCTSTR lpcszPath) : CInifileHelperRoot( lpcszPath)
{
}

inline CRasterInifileHelper::~CRasterInifileHelper()
{
}

/// �x�[�X�f�B���N�g���p�X�擾�֐�
inline void CRasterInifileHelper::GetBaseDir( CString& cStrBaseDir)
{
	CInifileHelperRoot::GetPrivateProfileString( TEXT( "rasterfile"), TEXT( "basedir"), TEXT( ""), cStrBaseDir, _MAX_BASEDIR);
	cStrBaseDir.ReleaseBuffer();
}

/// �x�[�X�f�B���N�g���p�X�ݒ�֐�
inline void CRasterInifileHelper::WriteBaseDir( LPCTSTR lpcszBaseDir)
{
	CInifileHelperRoot::WritePrivateProfileString( TEXT( "rasterfile"), TEXT( "basedir"), lpcszBaseDir);
}

/// ���X�^�̎�ގ擾�֐�
inline void CRasterInifileHelper::GetPurpose( int nIndex, CString& cStrPurpose)
{
	CString cStrEntry;
	cStrEntry.Format( TEXT( "purpose%d"), nIndex);
	CInifileHelperRoot::GetPrivateProfileString( TEXT( "rasterfile"), cStrEntry, TEXT( ""), cStrPurpose, _MAX_PURPOSE);
	cStrPurpose.ReleaseBuffer();
}

/// ���X�^�̍ő�\�������擾�֐�
inline void CRasterInifileHelper::GetRasterMax( int& nMax)
{
	nMax = CInifileHelperRoot::GetPrivateProfileInt( TEXT( "rasterfile"), TEXT( "rastermax"), -1);
}

/// ���X�^�̎�ސݒ�֐�
inline void CRasterInifileHelper::WritePurpose( int nIndex, LPCTSTR lpcszPurpose)
{
	CString cStrEntry;
	cStrEntry.Format( TEXT( "purpose%d"), nIndex);
	CInifileHelperRoot::WritePrivateProfileString( TEXT( "rasterfile"), cStrEntry, lpcszPurpose);
}

/// ���̓R�[�h���̎擾�֐�
inline void CRasterInifileHelper::GetAlias( LPCTSTR lpcszPurpose, CString& cStrAlias)
{
	CInifileHelperRoot::GetPrivateProfileString( lpcszPurpose, TEXT( "codealias"), TEXT( ""), cStrAlias, _MAX_CODEALIAS);
	cStrAlias.ReleaseBuffer();
}

/// ���̓R�[�h���̐ݒ�֐�
inline void CRasterInifileHelper::WriteAlias( LPCTSTR lpcszPurpose, LPCTSTR lpcszAlias)
{
	CInifileHelperRoot::WritePrivateProfileString( lpcszPurpose, TEXT( "codealias"), lpcszAlias);
}

/// ���̓R�[�h�ő咷�ŏ����擾�֐�
inline void CRasterInifileHelper::GetMaxMin( LPCTSTR lpcszPurpose, int& nMax, int& nMin)
{
	nMax = CInifileHelperRoot::GetPrivateProfileInt( lpcszPurpose, TEXT( "codemax"), -1);
	nMin = CInifileHelperRoot::GetPrivateProfileInt( lpcszPurpose, TEXT( "codemin"), -1);
}

/// ���̓R�[�h�ő咷�ŏ����ݒ�֐�
inline void CRasterInifileHelper::WriteMaxMin( LPCTSTR lpcszPurpose, int nMax, int nMin)
{
	CInifileHelperRoot::WritePrivateProfileInt( lpcszPurpose, TEXT( "codemax"), nMax);
	CInifileHelperRoot::WritePrivateProfileInt( lpcszPurpose, TEXT( "codemax"), nMin);
}

/// �R�[�h�p�X�ϊ������擾�֐�
inline void CRasterInifileHelper::GetSeed( LPCTSTR lpcszPurpose, CString& cStrSeed)
{
	CInifileHelperRoot::GetPrivateProfileString( lpcszPurpose, TEXT( "code2path"), TEXT( ""), cStrSeed, _MAX_CODE2PATH);
	cStrSeed.ReleaseBuffer();
}
/// ���X�^�t�@�C���g���q�擾�֐�
inline void CRasterInifileHelper::GetFileExtension( LPCTSTR lpcszPurpose, CString& cStrExt)
{
	CInifileHelperRoot::GetPrivateProfileString( lpcszPurpose, TEXT( "extension"), TEXT( ""), cStrExt, _MAX_CODE2PATH);
	cStrExt.ReleaseBuffer();
}

/// �R�[�h�p�X�ϊ������ݒ�֐�
inline void CRasterInifileHelper::WriteSeed( LPCTSTR lpcszPurpose, LPCTSTR lpcszSeed)
{
	CInifileHelperRoot::WritePrivateProfileString( lpcszPurpose, TEXT( "code2path"), lpcszSeed);
}

/// �T�u�f�B���N�g���擾�֐�
inline void CRasterInifileHelper::GetSubdir( LPCTSTR lpcszPurpose, CString& cStrSubdir)
{
	CInifileHelperRoot::GetPrivateProfileString( lpcszPurpose, TEXT( "subdir"), TEXT( ""), cStrSubdir, _MAX_SUBDIR);
	cStrSubdir.ReleaseBuffer();
}

/// �T�u�f�B���N�g���ݒ�֐�
inline void CRasterInifileHelper::WriteSubdir( LPCTSTR lpcszPurpose, LPCTSTR lpcszSubdir)
{
	CInifileHelperRoot::WritePrivateProfileString( lpcszPurpose, TEXT( "subdir"), lpcszSubdir);
}

/// �s���~�b�h�\���\�z����擾�֐�
inline void CRasterInifileHelper::GetPyramid(LPCTSTR lpcszPurpose, int& nPyramid)
{
	nPyramid = CInifileHelperRoot::GetPrivateProfileInt( lpcszPurpose, TEXT( "Pyramid"), -1);
}

/// �s���~�b�h�\���\�z����ݒ�֐�
inline void CRasterInifileHelper::WritePyramid(LPCTSTR lpcszPurpose, int nPyramid)
{
    CInifileHelperRoot::WritePrivateProfileInt( lpcszPurpose, TEXT( "Pyramid"), nPyramid);
}

/// ���ߗ�����擾�֐�
inline void CRasterInifileHelper::GetTransmissivity(LPCTSTR lpcszPurpose, int& nTransmissivity)
{
	nTransmissivity = CInifileHelperRoot::GetPrivateProfileInt( lpcszPurpose, TEXT( "Transmissivity"), -1);
}

/// �O���[�v���C���쐬����֐�
inline void CRasterInifileHelper::GetGroupLayer(LPCTSTR lpcszPurpose, int& nGroupLayer)
{
	nGroupLayer = CInifileHelperRoot::GetPrivateProfileInt( lpcszPurpose, TEXT( "group"), -1);
}

/// �I���G���A���a�l�擾
inline UINT CInifileHelper::GetSelectTolerance( void)
{
	return AheGetSelectTolerance( m_strInifilePath);
}

// �X�΃��C���t�@�C��
inline int CInifileHelper::GetGradLayerFile( CString& cGradLayerFile)
{
	::GetPrivateProfileString( TEXT("GradEdit"), TEXT("LayerFileName"), TEXT( ""), cGradLayerFile.GetBuffer( MAX_PATH), MAX_PATH, (LPCTSTR)m_strInifilePath);
	cGradLayerFile.ReleaseBuffer();
	return cGradLayerFile.GetLength();
}
// �X�΃��C��
inline int CInifileHelper::GetGradLayer( CString& cGradLayer)
{
	::GetPrivateProfileString( TEXT("GradEdit"), TEXT("LayerName"), TEXT( "�X�΃��f�B�t�@�C"), cGradLayer.GetBuffer( MAX_PATH), MAX_PATH, (LPCTSTR)m_strInifilePath);
	cGradLayer.ReleaseBuffer();
	return cGradLayer.GetLength();
}
