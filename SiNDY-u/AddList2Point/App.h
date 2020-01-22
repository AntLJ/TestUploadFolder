#pragma once

#define IDLIST_MODE		1
#define MXYLIST_MODE	2

#define PROGNAME	_T("AddList2Point.exe")
#define OPERATOR	_T("sindy")

class CApp
{
public:
	CApp(void);
	~CApp(void);

	/**
	 * @brief	�g�p�@
	 */
	static void printUsage();

	/**
	 * @brief	������
	 * @param	argc	[in]	�����̐�
	 * @param	argv	[in]	����
	 * @return	bool
	 */
	bool init( int argc, char* argv[] );

	/**
	 * @brief	���s
	 * @return	bool
	 */
	bool execute();

private:

	/**
	 * @brief	���ϐ�����c�a�֌W�̏��擾
	 * @return	bool
	 */
	bool setDBInfoFromEnv();

	/**
	 * @brief	�ҏW�J�n
	 * @note	�ҏW�p�q�o�[�W�������쐬����
	 * @return	bool
	 */
	bool editStart();

	/**
	 * @brief	�ҏW�I��
	 * @return	bool
	 */
	bool editStop();

	/**
	 * @brief	�h�c���X�g����ǉ�
	 */
	void addFromID();

	/**
	 * @brief	MXY���X�g����ǉ�
	 */
	void addFromMXY();

	/**
	* @brief	�����l�Z�b�g
	* @warning  �����R�s�[���ƃZ�b�g��t�B�[�`���͓����t�B�[�`���N���X�ł��邱��
	* @param	ipSrcFeature	[in]		�����l�R�s�[���̃t�B�[�`��
	* @param	ipFeatureBuffer	[in,out]	�Z�b�g�Ώۂ̃t�B�[�`���o�b�t�@
	* @param	strError		[out]		�G���[���b�Z�[�W(�G���[�������Ƃ�)
	* @return	retval true  �Z�b�g����
	* @return	retval false �Z�b�g���s
	*/
	bool setAttribute(IFeaturePtr ipSrcFeature, IFeatureBufferPtr ipFeatureBuffer, std::string& strError);

	/**
	 * @brief	�����l�Z�b�g
	 * @param	mapVal			[in]		�����l�i�[�R���e�i
	 * @param	ipFeatureBuffer	[in,out]	�Z�b�g�Ώۂ̃t�B�[�`���o�b�t�@
	 * @param	strError		[out]		�G���[���b�Z�[�W(�G���[�������Ƃ�)
	 * @return	bool
	 */
	bool setAttribute( const std::map<CString, CString>& mapVal, IFeatureBuffer* ipFeatureBuffer, std::string& strError );

	/**
	 * @brief	���ʑ����Z�b�g
	 * @param	ipFeatureBuffer	[in,out]	�Z�b�g�Ώۂ̃t�B�[�`���o�b�t�@
 	 * @param	strError		[out]		�G���[���b�Z�[�W(�G���[�������Ƃ�)
	 * @return	bool
	 */
	bool setCommonAttribute( IFeatureBuffer* ipFeatureBuffer, std::string& strError );

	/**
	 * @brief	���O�o��
	 * @note	Bug8605
	 * @param	lOID		[in]	OID
	 * @param	ipFeature	[in]	�t�B�[�`��
	 */
	void PrintLog( long lOID, IFeaturePtr ipFeature );

///////////////////////////////
//	�����o�ϐ�
///////////////////////////////

	int		m_nListMode;					/// IDLIST_MODE or MXYLIST_MODE
	TCHAR	m_szListFile[_MAX_PATH];		/// ���X�g�t�@�C��

	IWorkspacePtr	m_ipFromWorkspace;		//!< �ǉ����c�a���[�N�X�y�[�X(ID���X�g�̂Ƃ��̂ݎg�p)
	IWorkspacePtr	m_ipToWorkspace;		//!< �ǉ���c�a���[�N�X�y�[�X

	IWorkspaceEditPtr	m_ipWorkspaceEdit;	/// �ҏW�p���[�N�X�y�[�X

	LPTSTR		m_lpszTargetFCName;			/// �ǉ����t�B�[�`���N���X��

	IFeatureClassPtr	m_ipTargetFromFC;	/// �ǉ����t�B�[�`���N���X
	IFeatureClassPtr	m_ipTargetToFC;		/// �ǉ���t�B�[�`���N���X

	std::vector<long>	m_vecTargetFCFID;	//!< �ǉ���t�B�[�`���N���X�t�B�[���h�C���f�b�N�X
};
