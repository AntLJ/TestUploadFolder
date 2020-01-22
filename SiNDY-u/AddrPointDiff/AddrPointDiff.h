#pragma once

#include "Arguments.h"
#include "stdafx.h"
#include "AddrPointRec.h"
#include "ErrInfoRec.h"
#include "AddressSort.h"
#include "Utility.h"

#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/AheGlobalsMisc.h>
#include <ArcHelperEx/AheGeometryOp.h>
#include <TDC/sindylib_base/TableFinder.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/RowContainer.h>

/**
 * @brief �G���[���O�w�b�_���X�g
 */
const std::vector<CString> ERRLOG_HEADER_LIST = boost::assign::list_of
		( _T("FLAG") )
		( _T("LAYER") )
		( _T("OBJECTID") )
		( _T("1:LAYER") )
		( _T("1:OBJECTID") )
		( _T("�G���[���x��") )
		( _T("�����p�^�[��") )
		( _T("Geospace����ID") )
		( _T("iPC�Z��:11���R�[�h") )
		( _T("iPC�Z��:�Z��������") )
		( _T("�Z������F:11���R�[�h") )
		( _T("�Z������F:�Z��������") )
		( _T("����") )
		;

/**
 * @brief   �Z���|�C���g���ݎ�ʃp�^�[��
 */
typedef std::pair<AddrExistAttr, CString> PointExistAttrPattern;

/**
 * @brief   ���C���N���X
 */
class CAddrPointDiff
{

public:

	/**
	 * @brief   �R���X�g���N�^
	 * @param   args [in]  ����
	 */
	CAddrPointDiff( const Arguments& args ) : m_args( args ){}

	/**
	 * @brief   �f�X�g���N�^
	 */
	~CAddrPointDiff(){};

	/**
	 * @brief   ���s
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool execute();

private:

	/**
	 * @brief   ������
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool init();

	/**
	 * @brief �e�[�u���I�[�v��
	 * @param[in]  conn      �ڑ�������
	 * @param[in]  tableName �Ώۃe�[�u����
	 * @param[out] table     �ڑ��e�[�u���iITablePtr�j
	 * @retval true ����
	 * @retval false �G���[
	 */
	bool openTable(const CString& conn, const CString& tableName, ITablePtr& table);

	/**
	 * @brief �e�[�u���t�@�C���_������
	 * @param[in]  finder  �e�[�u���t�@�C���_
	 * @param[in]  conn    �ڑ���
	 * @param[out] table   �e�[�u����
	 * @retval true ����
	 * @retval false �G���[
	 */
	bool initCollection(sindy::CTableFinder& finder, const CString& conn, const CString& table);

	/**
	 * @brief iPC������ɑ��݂��邩�`�F�b�N����i���ԃt�@�C���Z���|�C���g�p�j
	 * @param[in]      buildingT      �e�[�u���R���e�i�iBUILDING�j
	 * @param[in/out]  addrPointRecs  �Z���|�C���g�Z�b�g
	 */
	void checkInBuilding(
				sindy::CTableContainer& buildingT,
				AddrPointRecs& addrPointRecs
			);

	/**
	 * @brief iPC������ɑ��݂��邩�`�F�b�N����i���|�C���g/CS�|�C���g�p�j
	 * @param[in]      buildingT      �e�[�u���R���e�i�iBUILDING�j
	 * @param[in/out]  addrPointRecs  �Z���|�C���g�Z�b�g
	 * @param[in]      targetGeo      BUILDING�擾���ɐڐG����Ɏg�p����`��
	 */
	void checkInBuilding(
				sindy::CTableContainer& buildingT,
				AddrPointRecs& addrPointRecs,
				const IGeometryPtr& targetGeo
			);

	/**
	 * @brief �Z���|�C���g�R���e�i�𐶐�����
	 * @param[in]      type          �Z���|�C���g���
	 * @param[in]      addrcode11    �Z��11���R�[�h
	 * @param[in]      targetRow     Row�R���e�i�i�f�[�^�擾���j
	 * @param[in]      gaikuFugo     �X�敄��
	 * @param[in]      extGaikuFugo  �g���X�敄��
	 * @param[in]      addrClass     �Z���������
	 * @param[in/out]  addrPointRecs �Z���|�C���g�Z�b�g
	 */
	void makeAddrPoint(
				AddrPointType type,
				const CString& addrcode11,
				const sindy::CRowContainer& targetRow,
				const CString& gaikuFugo,
				const CString& extGaikuFugo,
				int addrClass,
				AddrPointRecs& addrPointRecs
			);

	/**
	 * @brief �Z���|�C���g�R���e�i�𐶐�����
	 * @param[in]      type          �Z���|�C���g���
	 * @param[in]      addrcode11    �Z��11���R�[�h
	 * @param[in]      targetRow     Row�R���e�i�i�f�[�^�擾���j
	 * @param[in/out]  addrPointRecs �Z���|�C���g�Z�b�g
	 */
	void makeAddrPoint(
				AddrPointType type,
				const CString& addrcode11,
				const sindy::CRowContainer& targetRow,
				AddrPointRecs& addrPointRecs
			);

	/**
	 * @brief �Z���|�C���g�̓s�s�n�}�G���A���O������s��
	 * @param[in]   cityAdminT             �e�[�u���R���e�i�iCITY_ADMIN�j
	 * @param[in]   addrPointRecs          �Z���|�C���g�Z�b�g
	 * @param[in]   targetGeo              CITY_ADMIN�擾���ɐڐG����Ɏg�p����`��
	 * @param[out]  urbanAreaAddrPoints    �Z���|�C���g�Z�b�g�Ǘ��R���e�i�i�s�s�n�}�G���A���A11���R�[�h�P�ʁj
	 * @param[out]  nonUrbanAreaAddrPoints �Z���|�C���g�Z�b�g�Ǘ��R���e�i�i�s�s�n�}�G���A�O�A11���R�[�h�P�ʁj
	 * @retval true ����
	 * @retval false �G���[
	 */
	bool assortAddrPointsByArea(
				sindy::CTableContainer& cityAdminT,
				const AddrPointRecs& addrPointRecs,
				const IGeometryPtr& targetGeo,
				std::map<CString, AddrPointRecSet>& urbanAreaAddrPoints,
				std::map<CString, AddrPointRecSet>& nonUrbanAreaAddrPoints
			);

	/**
	 * @brief �擾�����Z���|�C���g�Z�b�g���Aaddrcodd2���l�����ďZ���|�C���g�Z�b�g�Ǘ��R���e�i�Ɋi�[����
	 * @param[in]      addrPointRecs  �Z���|�C���g�Z�b�g
	 * @param[in]      addrcode2      �Z��11���R�[�h�iaddrcode2�l���j
	 * @param[in/out]  addrPointSets  �Z���|�C���g�Z�b�g�Ǘ��R���e�i�i11���R�[�h�P�ʁj
	 */
	void setAddrPointRecs(
				const AddrPointRecs& addrPointRecs,
				const CString& addrcode2,
				std::map<CString, AddrPointRecSet>& addrPointSets
			);
			
	/**
	 * @brief ���|�C���g����Z���|�C���g�𐶐����A�Z���|�C���g�Z�b�g�Ǘ��R���e�i�Ɋi�[����
	 * @param[in]      citycode               �s�撬���R�[�h
	 * @param[in]      cityAdminT             �e�[�u���R���e�i�iCITY_ADMIN�j
	 * @param[in]      buildingT              �e�[�u���R���e�i�iBUILDING�j
	 * @param[in/out]  addrcode11List         �Z��11�R�[�h�Z�b�g
	 * @param[in/out]  urbanAreaAddrPoints    �Z���|�C���g�Z�b�g�Ǘ��R���e�i�i�s�s�n�}�G���A���A11���R�[�h�P�ʁj
	 * @param[in/out]  nonUrbanAreaAddrPoints �Z���|�C���g�Z�b�g�Ǘ��R���e�i�i�s�s�n�}�G���A�O�A11���R�[�h�P�ʁj
	 */
	void getAddrPointsFromGouPoint(
				const CString& citycode,
				sindy::CTableContainer& cityAdminT,
				sindy::CTableContainer& buildingT,
				std::set<CString>& addrcode11List,
				std::map<CString, AddrPointRecSet>& urbanAreaAddrPoints,
				std::map<CString, AddrPointRecSet>& nonUrbanAreaAddrPoints
			);
			
	/**
	 * @brief CS�|�C���g����Z���|�C���g�𐶐����A�Z���|�C���g�Z�b�g�Ǘ��R���e�i�Ɋi�[����
	 * @param[in]      citycode               �s�撬���R�[�h
	 * @param[in]      cityAdminT             �e�[�u���R���e�i�iCITY_ADMIN�j
	 * @param[in]      buildingT              �e�[�u���R���e�i�iBUILDING�j
	 * @param[in/out]  addrcode11List         �Z��11�R�[�h�Z�b�g
	 * @param[in/out]  urbanAreaAddrPoints    �Z���|�C���g�Z�b�g�Ǘ��R���e�i�i�s�s�n�}�G���A���A11���R�[�h�P�ʁj
	 * @param[in/out]  nonUrbanAreaAddrPoints �Z���|�C���g�Z�b�g�Ǘ��R���e�i�i�s�s�n�}�G���A�O�A11���R�[�h�P�ʁj
	 * @retval true ����
	 * @retval false �G���[
	 */
	bool getAddrPointsFromCsPoint(
				const CString& citycode,
				sindy::CTableContainer& cityAdminT,
				sindy::CTableContainer& buildingT,
				std::set<CString>& addrcode11List,
				std::map<CString, AddrPointRecSet>& urbanAreaAddrPoints,
				std::map<CString, AddrPointRecSet>& nonUrbanAreaAddrPoints
			);
			
	/**
	 * @brief ���ԃt�@�C���Z���|�C���g����Z���|�C���g�𐶐����A�Z���|�C���g�Z�b�g�Ǘ��R���e�i�Ɋi�[����
	 * @param[in]      citycode               �s�撬���R�[�h
	 * @param[in]      cityAdminT             �e�[�u���R���e�i�iCITY_ADMIN�j
	 * @param[in]      buildingT              �e�[�u���R���e�i�iBUILDING�j
	 * @param[in/out]  addrcode11List         �Z��11�R�[�h�Z�b�g
	 * @param[in/out]  urbanAreaAddrPoints    �Z���|�C���g�Z�b�g�Ǘ��R���e�i�i�s�s�n�}�G���A���A11���R�[�h�P�ʁj
	 * @param[in/out]  nonUrbanAreaAddrPoints �Z���|�C���g�Z�b�g�Ǘ��R���e�i�i�s�s�n�}�G���A�O�A11���R�[�h�P�ʁj
	 * @retval true ����
	 * @retval false �G���[
	 */
	bool getAddrPointsFromMidAddrPoint(
				const CString& citycode,
				sindy::CTableContainer& cityAdminT,
				sindy::CTableContainer& buildingT,
				std::set<CString>& addrcode11List,
				std::map<CString, AddrPointRecSet>& urbanAreaAddrPoints,
				std::map<CString, AddrPointRecSet>& nonUrbanAreaAddrPoints
			);

	/**
	 * @brief �s�s�n�}�G���A����iPC�Z��/���ԃt�@�C���Z���|�C���g���r���A�������擾����
	 * @param[in]      addrcode11             �Z��11���R�[�h
	 * @param[in/out]  urbanAreaAddrPoints    �Z���|�C���g�Z�b�g�Ǘ��R���e�i�i�s�s�n�}�G���A���A11���R�[�h�P�ʁj
	 * @param[in/out]  nonUrbanAreaAddrPoints �Z���|�C���g�Z�b�g�Ǘ��R���e�i�i�s�s�n�}�G���A�O�A11���R�[�h�P�ʁj
	 */
	void diffUrbanAreaPoints(
			const CString& addrcode11,
			std::map<CString, AddrPointRecSet>& urbanAreaAddrPoints,
			std::map<CString, AddrPointRecSet>& urbanAreaMidAddrPoints
			);

	/**
	 * @brief �s�s�n�}�G���A�O��iPC�Z��/���ԃt�@�C���Z���|�C���g���r���A�������擾����
	 * @param[in]      addrcode11             �Z��11���R�[�h
	 * @param[in/out]  urbanAreaAddrPoints    �Z���|�C���g�Z�b�g�Ǘ��R���e�i�i�s�s�n�}�G���A���A11���R�[�h�P�ʁj
	 * @param[in/out]  nonUrbanAreaAddrPoints �Z���|�C���g�Z�b�g�Ǘ��R���e�i�i�s�s�n�}�G���A�O�A11���R�[�h�P�ʁj
	 */
	void diffNonUrbanAreaPoints(
			const CString& addrcode11,
			std::map<CString, AddrPointRecSet>& nonUrbanAreaAddrPoints,
			std::map<CString, AddrPointRecSet>& urbanAreaMidAddrPoints
			);

	/**
	 * @brief �d���������ԃt�@�C���Z���|�C���g�Q����ɁA�����p�^�[�������肷��
	 * @param[in]   midPointRecs       �d�������s�������ԃt�@�C���Z���|�C���g
	 * @param[out]  outMidPointRecs    ���肵�������p�^�[���ɕR�t���Z���|�C���g
	 * @return �Z���|�C���g���ݎ�ʃp�^�[��
	 */
	PointExistAttrPattern decideDiffPattern(
				std::map<AddrExistAttr, std::vector<AddrPointWithSortKey>>& midPointRecs,
				std::vector<AddrPointWithSortKey>& outMidPointRecs
			);

	/**
	 * @brief �G���[���O��iPC�Z��/���ԃt�@�C���Z���̔�r���ʂ���������
	 */
	void CAddrPointDiff::writeErrLog();

private:

	Arguments m_args;                                //!< ����
	sindy::CTableFinder m_addrFinder;                //!< �e�[�u���t�@�C���_�iSiNDY�i�Z���j�j
	sindy::CTableFinder m_bgFinder;                  //!< �e�[�u���t�@�C���_�iSiNDY�i�w�i�j�j
	sindy::CTableFinder m_midAddrPointFinder;        //!< �e�[�u���t�@�C���_�i���ԃt�@�C���Z���j

	std::vector<ErrInfoRec> errInfoRecs;             //!< �G���[��񃊃X�g
};


