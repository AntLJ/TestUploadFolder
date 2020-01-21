/**
 * @file ErrObj.h
 * 
 * @biref �_���`�F�b�N���̃G���[�����I�u�W�F�N�g�ʂɊǗ�����@�\��񋟂���
 * �N���X�Q�̒�`�t�@�C���ł��B
 *
 * ���̃N���X�Q�́ASiNDYLib�ɂ���ErrorObject.h���Q�l�ɍ쐬���܂����B
 *
 * @author �R���e���c�{���Z�p�J���V�X�e���J���O���[�v �Ð�M�G
 * @date 2008/06/25
 * @copy Copyright(c) 2008 INCREMENT P CORP., All Rights Reserved.
 */
#pragma once

enum sindyeLogFormat {
	sindyeLogFormat_Common,		//!< ���O�n���h���W���t�H�[�}�b�g
	sindyeLogFormat_Freestyle,	//!< ���R�`�����O
};

enum sindyeLogLevel {
	sindyeLogLevel_INFO,		//!< ���
	sindyeLogLevel_WARN,		//!< �x��
	sindyeLogLevel_ERR,			//!< �G���[
};

/**
 * @class CErrInfo
 * 
 * @brief �G���[�����Ǘ�����N���X

 * �X�̃G���[�����Ǘ����܂��B
 * �t�B�[�`���Ƃ̊֘A�t���͏�ʃN���X�ōs���܂��B
 */
class CErrInfo
{
public:
	CErrInfo( long lField, const WKSPoint& point, long scale, sindyeLogLevel emLevel, LPCTSTR lpcszMessage );

	void init( long lField, const WKSPoint& point, long scale, sindyeLogLevel emLevel, LPCTSTR lpcszFormat, ... );
	void initV( long lField, const WKSPoint& point, long scale, sindyeLogLevel emLevel, LPCTSTR lpcszFormat, va_list args );

	CString ToString() const;
	CString ToLog( sindyeLogFormat emFormat ) const;
public:
	WKSPoint				m_cPoint;		//!< �G���[�|�C���g
	long					m_lField;		//!< �t�B�[���h�C���f�b�N�X�ԍ�
	CString					m_strMsg;		//!< �G���[���b�Z�[�W
	long					m_lScale;		//!< �G���[�Q�ƃX�P�[��
	sindyeLogLevel			m_emLevel;		//!< �G���[���x��
};

/**
 * @clas CErrInfoContainer
 * 
 * @brief �����̃G���[�����Ǘ�����N���X
 *
 * CErrInfo�𕡐��Ǘ����܂��B
 * �t�B�[�`���Ƃ̊֘A�t���͏�ʃN���X�ōs���܂��B
 */
class CErrInfoContainer : public std::list<CErrInfo>
{
public:
	void add( long lField, const WKSPoint& point, long scale, sindyeLogLevel emLevel, LPCTSTR lpcszFormat, ... );
	void addV( long lField, const WKSPoint& point, long scale, sindyeLogLevel emLevel, LPCTSTR lpcszFormat, va_list args );

	CString ToString() const;
};

/** 
 * @class CErrObj
 * 
 * @biref �t�B�[�`���P�ʂ̃G���[�����Ǘ�����N���X
 *
 * CErrInfoContainer�ƃt�B�[�`���i���R�[�h�j�̊֘A��
 * �Ǘ����܂��B
 */
class CErrObj : public CErrInfoContainer
{
public:
	CErrObj(){}
	CErrObj( LPCTSTR lpcszTableName, long lOID, IGeometry* ipGeom )
	{
		init( lpcszTableName, lOID, ipGeom );
	}
	~CErrObj(){}

	void init( LPCTSTR lpcszTableName, long lOID, IGeometry* ipGeom );

	CString ToString() const;
	void ToLog( sindyeLogFormat emFormat, std::list<CString>& listLog ) const;
public:
	CString m_strTableName; //!< �e�[�u����
	long m_lOID; //!< OBJECTID
	IGeometryPtr m_ipGeom; //!< �`��
};

/**
 * @class CErrObjContainer
 *
 * @brief �����̃t�B�[�`���̃G���[�����Ǘ�����N���X
 * 
 * CErrObj�𕡐��Ǘ����܂��B
 * �܂��A�G���[�����_�C�A���O���Ŏg�p���镶����ɕϊ�������A
 * LogHandleTool�ň����郍�O�`���œf���o�����肷�邱�Ƃ��ł��܂��B
 */
class CErrObjContainer : public std::list<CErrObj>
{
public:
	void add( LPCTSTR lpcszTableName, long lOID, IGeometry* ipGeom, long lField, const WKSPoint& point, long scale, sindyeLogLevel emLevel, LPCTSTR lpcszFormat, ... );
	iterator find( LPCTSTR lpcszTableName, long lOID );
	const_iterator find( LPCTSTR lpcszTableName, long lOID ) const;

	CString ToString() const;
	void ToLog( sindyeLogFormat emFormat, FILE* out ) const;
	void ToLog( sindyeLogFormat emFormat, std::list<CString>& listLog ) const;

	/// �ō��l�̃��O���x����Ԃ�
	sindyeLogLevel GetMaxLogLevel() const;
};