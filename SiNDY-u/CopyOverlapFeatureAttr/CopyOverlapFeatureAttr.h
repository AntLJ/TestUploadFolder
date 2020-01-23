#pragma once

#include "Arguments.h"
#include "LogWriter.h"
#include "DatabaseUtility.h"

#include <ArcHelperEx/AheMeshUtil.h>

/**
 * @brief   ���C���N���X
 */
class CCopyOverlapFeatureAttr : public LogWriter
{
public:
	/**
	 * @brief �ݒ��� 
	 */
	struct SettingInfo
	{
		CString   strSource;          //!< ��r���̐ڑ���
		CString   strSourceTable;     //!< ��r���̃e�[�u����
		CString   strSourceField;     //!< ��r���̃t�B�[���h��
		CString   strSourceWhere;     //!< ��r����WHERE����

		CString   strTarget;          //!< ��r��̐ڑ���
		CString   strTargetTable;     //!< ��r��̃e�[�u����
		CString   strTargetField;     //!< ��r��̃t�B�[���h��
		CString   strTargetWhere;     //!< ��r���WHERE����

		CString   strMesh;            //!< �����P��(���b�V��)�̐ڑ���
		CString   strMeshTable;       //!< �����P��(���b�V��)�̃e�[�u����
		CString   strMeshListPath;    //!< �����͈́i���b�V�����X�g�j�̃t�@�C���p�X

		CString   strExeMode;         //!< ���s���[�h
		CString   strAttrListPath;    //!< �����ϊ��\�̃t�@�C���p�X
		bool      bDatumFlag;         //!< ���n�n�����ϊ��t���O
		bool      bFieldFlag;         //!< �t�B�[���h�^�����ϊ��t���O

		double    dOverlay;           //!< �d������臒l
		double    dMagnification;     //!< �g�嗦��臒l
		double    dReduction;         //!< �k������臒l
		double    dBuffer;            //!< �o�b�t�@�[�̕��i���[�g���P�ʁj

		SettingInfo(): bDatumFlag(false), bFieldFlag(false), dOverlay(-1.0), dMagnification(-1.0), dReduction(-1.0), dBuffer(-1.0) {}
		virtual ~SettingInfo(){}
	};

public:

	/**
	 * @brief   �R���X�g���N�^
	 */
	CCopyOverlapFeatureAttr(): m_nMeshCodeIndex(-1), m_nSourceFieldIndex(-1), m_nTargetFieldIndex(-1),
		m_nUpdateTypeIndex(-1), m_nModifyProgNameIndex(-1), m_nProgModifyDateIndex(-1) {}

	/**
	 * @brief   �f�X�g���N�^
	 */
	virtual ~CCopyOverlapFeatureAttr(){};

	/**
	 * @brief   ���s
	 * @param   args [in]  ����
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool execute(const Arguments& args);

private:
	static const int max_retry_num;     //!< �ڑ����g���C��
	static const double max_retry_time; //!< �ڑ����g���C����

	/**
	 * @brief   �q�o�[�W��������
	 * @param   ipWorkspace [in] ���[�N�X�y�[�X
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool createChildVer( const IWorkspacePtr& ipWorkspace );

	/**
	 * @brief	���[�N�X�y�[�X�I�[�v��
	 * @param	name         [in]    �ڑ���
	 * @param	ipWorkspace  [out]   ���[�N�X�y�[�X
	 * @retval	true    ����
	 * @retval	false   ���s
	 */
	bool openWorkspace( const CString& name, IWorkspacePtr& ipWorkspace );

	/**
	 * @brief   DB�̃I�[�v������
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool openDB();

	/**
	 * @brief   �t�B�[�`���N���X���擾
	 * @param   name           [in]   �t�B�[�`���N���X��
	 * @param   ipWorkspace    [in]   ���[�N�X�y�[�X
	 * @param   ipFeatureClass [out]  �t�B�[�`���N���X
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool openFeatureClass(const CString& name, const IWorkspacePtr& ipWorkspace, IFeatureClassPtr& ipFeatureClass);

	/**
	 * @brief ���̓t�B�[�`���N���X���擾����
	 * @retval true  �擾����
	 * @retval false �擾���s
	 */
	bool openFC();

	/**
	 * @brief �e�t�B�[���h��Index���擾
	 * @retval true  �擾����
	 * @retval false �擾���s
	 */
	bool getFieldsIndex();


	/**
	 * @brief ���n�n�̃`�F�b�N
	 * @retval true  �ُ�Ȃ�
	 * @retval false �ُ킠��
	 */
	bool checkDatum();

	/**
	 * @brief �t�B�[���h�^�̃`�F�b�N
	 * @retval true  �ُ�Ȃ�
	 * @retval false �ُ킠��
	 */
	bool checkFieldType();

	/**
	 * @brief �Ώۃt�B�[���h���X�V
	 * @param   maxOverlayOID [in]  �o�͗p�̔�r��OID
	 * @param   maxAttrValue  [in]  �X�V�Ώۂ̑����l
	 * @param   ipFeature     [in]  �X�V�Ώۃt�B�[�`���[
	 * @param   errItem       [in]  �G���[���
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool updateField(long maxOverlayOID, const CString& maxAttrValue, IFeaturePtr& ipFeature, ErrItem& errItem);

	/**
	 * @brief   ������
	 * @param   args [in]  ����
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool init(const Arguments& args);

	/**
	 * @brief   �ݒ�t�@�C���̓ǂݍ���
	 * @param   strFilePath [in]  �ݒ�t�@�C���̃p�X
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool readSettingFile( const CString& strFilePath );

	/**
	 * @brief   ���b�V�����X�g�̓ǂݍ���
	 * @param   strFilePath [in]  ���b�V�����X�g�̃p�X
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool readMeshList( const CString& strFilePath );

	/**
	 * @brief   �����ϊ��\�̓ǂݍ���
	 * @param   strFilePath [in]  �����ϊ��\�̃p�X
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool readAttributeList( const CString& strFilePath );

	/**
	 * @brief   �K�{���ځi������j�̃`�F�b�N
	 * @param   strKey   [in]  ���ږ�
	 * @param   strValue [in]  ���ڒl
	 * @retval  true  �ُ�Ȃ�
	 * @retval  false �ُ킠��
	 */
	bool checkStringSetting( const std::wstring& strKey, const CString& strValue );

	/**
	 * @brief   �K�{���ځi�����^�j�̃`�F�b�N
	 * @param   strKey   [in]  ���ږ�
	 * @param   value    [in]  ���ڒl
	 * @retval  true  �ُ�Ȃ�
	 * @retval  false �ُ킠��
	 */
	bool checkNumericSetting( const std::wstring& strKey, double value );

	/**
	 * @brief   �t�@�C���̃`�F�b�N
	 * @param   strKey      [in]  ���ږ�
	 * @param   strFilePath [in]  ���ڒl
	 * @retval  true  �ُ�Ȃ�
	 * @retval  false �ُ킠��
	 */
	bool checkFileSetting( const std::wifstream& ifs, const CString& strFilePath );

	/**
	 * @brief   �ݒ���e�̃`�F�b�N
	 * @retval  true  �ُ�Ȃ�
	 * @retval  false �ُ킠��
	 */
	bool checkSettings();

	/**
	 * @brief   �ݒ���̎擾
	 * @param   info [in]  �ݒ���
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool getSettingInfo(const std::vector<std::wstring>& info);

	/**
	 * @brief   ���΃p�X���΃p�X�ɕϊ�
	 * @param   strPath [in]  ���΃p�X
	 * @retval  ��΃p�X
	 */
	CString convertRelativePath(const CString& strPath);

	/**
	 * @brief  ���s���O�t�@�C���ɃI�v�V�����������o�͂���
	 */
	void writeSettings();

	/**
	 * @brief   ���b�V�����X�g���A���b�V���ƃo�b�t�@�[�̃|���S�����擾
	 * @param   mapMeshGeo [out]  �����͈͂̃��b�V��
	 * @retval  true �擾����
	 * @retval  false �擾���s
	 */
	bool getMeshPolygons(std::map<unsigned long, std::pair<IGeometryPtr, IGeometryPtr>>& mapMeshGeo);

	/**
	 * @brief   ��r���̃W�I���g�����擾
	 * @param   ipMeshGeo [in]   �����͈͂̃W�I���g���i�o�b�t�@�[�g�p�j
	 * @param   mapSource [out]  ��r���̃W�I���g��
	 * @retval  true �擾����
	 * @retval  false �擾���s
	 */
	bool getSourceObjectInMesh(const IGeometryPtr& ipMeshGeo, std::map<long, GeoClass>& mapSource);

	/**
	 * @brief   ��r��̃t�B�[���h���X�V
	 * @param   meshGeoRec   [in]  �����͈͂̃W�I���g��
	 * @param   mapSource    [in]  ��r���̃W�I���g��
	 * @param   sourceRTree  [in]  ��r����RTree
	 * @retval  true �X�V����
	 * @retval  false �X�V���s
	 */
	bool updateTargetField(const std::pair<unsigned long, std::pair<IGeometryPtr, IGeometryPtr>>& meshGeoRec, const std::map<long, GeoClass>& mapSource, const BGRTree& sourceRTree);

	/**
	 * @brief   R-Tree�𐶐�����
	 * @param   geoMap [in]  �Ώۃt�B�[�`���N���X
	 * @param   rTree  [out] ���ʂ�R-Tree(�`����[r]�͎Q�Ƃ̈Ӗ��ł͂Ȃ��ł�)
	 */
	void createRTree( const std::map<long, GeoClass>& geoMap, BGRTree& rTree );

	/**
	 * @brief   R-Tree����A�^�������͈̔͂Ɉ���������ID�Q���擾
	 * @param   box   [in]  �擾�������͈�(�G���x���[�v)
	 * @param   rTree [in]  �擾���Ă���Ώۂ�R-Tree
	 * @retval  set<long>   �Ώ۔͈͂�ID�Q
	 */
	std::set<long> getIntersectsIDs( const BGBox& box, const BGRTree& rTree );

	/**
	 * @brief   �d�������擾
	 * @param   ipSourceGeo [in]  ��r���̃W�I���g��
	 * @param   ipTargetGeo [in]  ��r��̃W�I���g��
	 * @retval  double  �d����
	 */
	double getOverlay( const IGeometryPtr& ipSourceGeo, const IGeometryPtr& ipTargetGeo );

	/**
	 * @brief   ��r���Ƃ̏d�������擾
	 * @param   ipTargetGeo [in]  ��r��̃W�I���g��
	 * @param   sourceIDs   [in]  �d�Ȃ�����r����ID
	 * @param   mapSource   [in]  ��r���̃W�I���g��
	 * @param   errItem     [in]  �G���[���
	 * @retval  std::map<long, GeoClass> �d�����Ɣ�r�����i�[����R���e�i
	 */
	std::multimap<double, GeoClass> getAllOverlay( const IGeometryPtr& ipTargetGeo, const std::set<long>& sourceIDs, const std::map<long, GeoClass>& mapSource, ErrItem& errItem );

	/**
	 * @brief   ��r���Ƃ̊g�k�������āA�����𖞂�����r�����擾
	 * @param   ipTargetGeo [in]  ��r��̃W�I���g��
	 * @param   sourceIDs   [in]  �d�Ȃ�����r����ID
	 * @param   mapSource   [in]  ��r���̃W�I���g��
	 * @param   errItem     [in]  �G���[���
	 * @retval  std::map<long, GeoClass>  �g�k���𖞂�����r��ID���i�[����R���e�i
	 */
	std::set<long> getAllOkScale( const IGeometryPtr& ipTargetGeo, const std::set<long>& sourceIDs, const std::map<long, GeoClass>& mapSource, ErrItem& errItem );

	/**
	 * @brief   �t�B�[���h�^����v���Ă��邩���`�F�b�N
	 * @retval  true  �t�B�[���h�^����v���Ă���
	 * @retval  false �t�B�[���h�^����v���Ă��Ȃ�
	 */
	bool isSameField();

	/**
	 * @brief   ���n�n����v���Ă��邩���`�F�b�N
	 * @retval  true  ���n�n����v���Ă���
	 * @retval  false ���n�n����v���Ă��Ȃ�
	 */
	bool isSameSpRef();

	/**
	 * @brief   �t�B�[���h�̃^�C�v�������Ώۂł��邩���`�F�b�N
	 * @param   fieldType [in]  �`�F�b�N�Ώۃt�B�[���h�̃^�C�v
	 * @retval  true  �t�B�[���h�̃^�C�v�������Ώۂł���
	 * @retval  false �t�B�[���h�̃^�C�v�������Ώۂł͂Ȃ�
	 */
	bool isProcessType( const esriFieldType& fieldType );

	/**
	 * @brief   �t�B�[���h�̃^�C�v�������Ώۂł��邩���`�F�b�N
	 * @retval  true  �t�B�[���h�̃^�C�v�������Ώۂł���
	 * @retval  false �t�B�[���h�̃^�C�v�������Ώۂł͂Ȃ�
	 */
	bool isProcessType();

	/**
	 * @brief   �t�B�[���h�l���w��l�ɕϊ�
	 * @param   strValue [in]  �w��l
	 * @retval  CComVariant�@�t�B�[���h�l
	 */
	CComVariant convertFieldValue( const CString& strValue );

	/**
	 * @brief   �G���[�o�͗p�̍��W�l���擾
	 * @param   shape   [in]   �t�B�[�`���`��
	 * @param   errItem [out]  �G���[���
	 * @retval  true �擾����
	 * @retval  false �擾���s
	 */
	bool getShapeCoordinate( const IGeometryPtr& shape, ErrItem& errItem );

	/**
	 * @brief   �G���[�o�͗p�̋��ʏ����擾
	 * @param   meshCode   [in]   ���b�V���R�[�h
	 * @param   target     [in]   ��r��̏��
	 * @retval  ErrItem �擾����
	 */
	ErrItem getCommonErrorInfo( unsigned long meshCode, const GeoClass& target );

	/**
	 * @brief   �G���[�o�͗p�̏ڍ׏���ݒ�
	 * @param   errCode   [in]    �G���[�R�[�h
	 * @param   errItem   [out]   �G���[���
	 */
	void setDetailErrorInfo( err_code::ECode errCode, ErrItem& errItem );

	/**
	 * @brief   �G���[�o�͗p�̏ڍ׏������Z�b�g
	 * @param   errItem   [out]   �G���[���
	 */
	void resetDetailErrorInfo( ErrItem& errItem );

	/**
	 * @brief   �Ώۃt�B�[���h���X�V
	 * @param   strValue  [in]   �t�B�[���h�l
	 * @param   ipFeature [in]   �X�V�Ώۃt�B�[�`���[
	 * @param   errItem   [in]   �G���[���
	 * @retval  true �X�V����
	 * @retval  false �X�V���s
	 */
	bool putValues(const CString& strValue, IFeaturePtr& ipFeature, ErrItem& errItem );


private:

	SettingInfo                 m_setting;                   //!< �ݒ���

	std::set<unsigned long>     m_setMeshList;               //!< ���b�V�����X�g
	std::map<CString, CString>  m_mapAttrList;               //!< �����ϊ��\(key:��r���̑����l�Avalue:��r��̑����l)

	IWorkspacePtr               m_ipSourceWorkspace;         //!< ��r�����[�N�X�y�[�X
	IWorkspacePtr               m_ipTargetWorkspace;         //!< ��r�惏�[�N�X�y�[�X
	IWorkspacePtr               m_ipMeshWorkspace;           //!< ���b�V�����[�N�X�y�[�X
	IWorkspaceEditPtr           m_ipWorkspaceEdit;           //!< �ҏW�p���[�N�X�y�[�X�i��r��j

	IFeatureClassPtr            m_ipSourceFC;                //!< ��r���t�B�[�`���N���X
	IFeatureClassPtr            m_ipTargetFC;                //!< ��r��t�B�[�`���N���X
	IFeatureClassPtr            m_ipMeshFC;                  //!< ���b�V���t�B�[�`���N���X

	ISpatialReferencePtr        m_ipSourceSpRef;             //!< ��r���̋�ԎQ��
	ISpatialReferencePtr        m_ipTargetSpRef;             //!< ��r��̋�ԎQ��
	
	IFieldPtr                   m_ipSourceField;             //!< ��r���̃t�B�[���h
	IFieldPtr                   m_ipTargetField;             //!< ��r��̃t�B�[���h

	long                        m_nMeshCodeIndex;            //!< �t�B�[���hMeshCode��Index
	long                        m_nSourceFieldIndex;         //!< ��r���t�B�[���h��Index
	long                        m_nTargetFieldIndex;         //!< ��r��t�B�[���h��Index
	long                        m_nUpdateTypeIndex;          //!< �t�B�[���h�ŏI�X�V���e�̂�Index
	long                        m_nModifyProgNameIndex;      //!< �t�B�[���h�ŏI�X�V�v���O��������Index
	long                        m_nProgModifyDateIndex;      //!< �t�B�[���h�ŏI�v���O�����X�V������Index
	
	std::set<long>              m_setProcessedOID;           //!< �����ς݂̔�r��OID
};

