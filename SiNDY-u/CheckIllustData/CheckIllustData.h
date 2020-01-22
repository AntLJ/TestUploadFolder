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

#pragma once

#include "Arguments.h"
#include "util.h"
#include <TDC/illustlib/HighwayInfo.h>
#include <TDC/sindylib_base/TableFinder.h>
#include <TDC/illustlib/IllustLinkTable.h>

#include <TDC/illustlib/IllustLinkRelCheck.h>

/**
 * @brief   ���C���N���X
 */
class CCheckIllustData
{
public:
	typedef std::map<CString, sindy::CSPIllustLinkRow> CHECKSAMENODEMAP;
	typedef std::multimap<CString, sindy::CSPIllustLinkRow> CHECKDIRECTIONCODEMAP;
	typedef CHECKDIRECTIONCODEMAP::const_iterator CHECKDIRECTIONCODEITER;

	enum EMode
	{
		kSchemaC = 1,        // �X�L�[�}�`�F�b�N�A�I�u�W�F�N�g�`�F�b�N
		kNodeC = 2,          // �m�[�h�`�F�b�N
		kIllustC = 4,        // �C���X�g�`�F�b�N
		kNodeAndIllustC = 6, // �m�[�h+�C���X�g�`�F�b�N
		kUnknown = -1,
	};

	/**
	 * @brief   �R���X�g���N�^
	 * @param   args [in]  ����
	 */
	CCheckIllustData( const Arguments& args ){
		m_args = args;

		// �u�V�X�e�������{����v�t���O������������
		m_isJapaneseOs =  ( GetSystemDefaultLCID() == util::JAPANESE_LANGUAGE );

		// ASEAN�����t���O������������
		m_isAseanSchema = AheIsWorldMeshMode();
	};

	/**
	 * @brief   �f�X�g���N�^
	 */
	~CCheckIllustData(){};

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
	 * @brief   �e�t�@�C�����J��
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool OpenFile();

	/**
	 * @brief   ���[�h��ݒ肷��
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool SetMode();

	/**
	 * @brief   �C���X�g�����N�Ɋ֘A����t�B�[�`�����L���b�V��
	 * @param  spIllustTable [in] �C���X�g�����N�e�[�u��
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool SetRelMap(sindy::CSPIllustLinkTable spIllustTable);

	/**
	 * @brief   ���C���`�F�b�N
	 * @param  cIllust [in] �C���X�g�e�[�u��
	 * @retval true  �`�F�b�N����
	 * @retval false �`�F�b�N���s
	 */
	bool check(sindy::CSPIllustLinkTable spIllustTable);

	/**
	 * @brief   �����ʁA�d����ŏd�����Ă��郌�R�[�h���Ȃ����`�F�b�N
	 * @param spRow [in] �C���X�g�����N
	 * @param vecNodeQueue [in] �m�[�h��
	 * @param checkSameNodeMap [in,out] �L�[�F��ʁA�d����ASINGLE_BRANCH_C�A�m�[�h��𕶎���ɂ������́@�l�F�C���X�g�����N
	 * @param checkSameIllustNameSet [in,out] ��ʁA�d����A�C���X�g���𕶎���ɂ����v�f�Q
	 * @param errList [out] �G���[���X�g
	 */
	void CheckDuplicate( sindy::CSPIllustLinkRow& spRow, const sindy::CNodeQueueCheck::NODEQUEUE& vecNodeQueue,
		CHECKSAMENODEMAP& checkSameNodeMap, std::set<CString>& checkSameIllustNameSet, sindy::ERRLIST& errList );

	/**
	 * @brief   ���E���h�A�o�E�g�̃C���X�g�����N�̃`�F�b�N
	 * @param vecNodeQueue [in] �m�[�h��
	 * @param errList [out] �G���[���X�g
	 */
	void checkRoundAbout( const sindy::CNodeQueueCheck::NODEQUEUE& vecNodeQueue, sindy::ERRLIST& errList);
	/**
	 * @brief   �C���X�g�����擾����
	 * @param spRow [in] �C���X�g�����N
	 * @param[in] exceptFields ���O����t�B�[���h�Q
	 * @return �C���X�g��
	 */
	CString GetIllustName(sindy::CSPIllustLinkRow& spRow, const std::set<CString>& exceptFields = std::set<CString>());

	/**
	 * @brief   �N�_������e����Ă���m�[�h��`�F�b�N
	 * @param checkSameNodeMap [in] �L�[�F��ʁA�d����A�m�[�h��𕶎���ɂ������́@�l�F�C���X�g�����N
	 */
	void CheckComprehensionNode( const CHECKSAMENODEMAP& checkSameNodeMap );

	/**
	 * @brief   �m�[�h��ƕ����R�[�h�̐������`�F�b�N
	 * @param checkDirectionMap [in] [multimap]�L�[�F�C���X�g�t�@�C�����A��ʁ@�l�F�C���X�g�����N
	 * @retval true  �`�F�b�N����
	 * @retval false �`�F�b�N���s
	 */
	bool CheckDirectionCode( const CHECKDIRECTIONCODEMAP& checkDirectionMap );

	/**
	 * @brief   �m�[�h��ƕ����R�[�h�̐������`�F�b�N
	 * @param sameValsPair [in] equal_range�ł܂Ƃ߂��v�f�Q
	 */
	void CheckDirectionCode( const std::pair<CHECKDIRECTIONCODEITER, CHECKDIRECTIONCODEITER>& sameValsPair );

	/**
	 * @brief   �m�[�h��ƕ��ʈē��̐������`�F�b�N
	 * @param sameValsPair [in] equal_range�ł܂Ƃ߂��v�f�Q
	 * @param setNoDirguideLink [in] ���ʈē������Ƃ��Ĉ������H�����N�Q
	 */
	void CheckRelDirguideExist( const std::pair<CHECKDIRECTIONCODEITER, CHECKDIRECTIONCODEITER>& sameValsPair, const std::set<long>& setNoDirguideLink );

	/**
	 * @brief   ���ʈē������Ƃ��Ĉ������H�����N�Q���擾����
	 * @param setNoDirguideLink [out] ���ʈē������Ƃ��Ĉ������H�����N�Q
	 * @retval true  �擾����
	 * @retval false �擾���s
	 */
	bool GetRoadLinkTreatedAsNoDirguide(std::set<long>& setNoDirguideLink);

	/**
	 * @brief   ���ʈē��̗L������R�[�h�l�擾
	 * @param lBeforeLinkID [in] ����O�����N
	 * @param lLeftLinkID   [in] ���򍶑��̃����N
	 * @param lRightLinkID  [in] ����E���̃����N
	 * @param setNoDirguideLink [in] ���ʈē������Ƃ��Ĉ������H�����N�Q
	 * @retval ���ʈē��ŔL���R�[�h
	 */
	long GetDirGuideExist( long lBeforeLinkID, long lLeftLinkID, long lRightLinkID, const std::set<long>& setNoDirguideLink );

	/**
	 * @brief   ���H�m�[�h1��2��3�̊p�x�����߂�B�i��123�̊p�x�j
	 * @note    ���m�ɂ͍\���_�����H�m�[�h2���\���_�̊p�x�����߂�
	 * @param node1 [in] �������m�[�h
	 * @param node2 [in] �p�x�����߂�m�[�h
	 * @param node3 [in] ���o���m�[�h
	 * @param spRow [in] �C���X�g�����N
	 * @param angle [out] �p�x(0�`360�x)
	 * @retval true  �p�x�擾����
	 * @retval false �Ή�����m�[�h�A�����N���Ȃ��A�`�󂪂Ȃ��Ċp�x���擾�ł��Ȃ�����
	 */
	bool GetAngleOf3Nodes( long node1, long node2, long node3, const sindy::CSPIllustLinkRow& spRow, double& angle );

	/**
	 * @brief   ���H�����N�̑Ώۃm�[�h���\���_�̊p�x���擾
	 * @param ipRoadLink [in] ���H�����N
	 * @param tgtNode [in] �p�x���擾���������̓��H�m�[�h
	 * @param angle [out] �擾�����p�x(���W�A��)
	 * @retval true  �擾����
	 * @retval false �擾���s
	 */
	bool GetOutflowLinkAngle(IFeaturePtr ipRoadLink, long tgtNode, double& angle);

	/**
	 * @brief   �|�C���g1�ƃ|�C���g2�̊p�x�����߂�
	 * @param ipPoint1 [in] �|�C���g1
	 * @param ipPoint2 [in] �|�C���g2
	 * @return �|�C���g1���|�C���g2�̊p�x(���W�A��)
	 */
	double GetAngle(IPointPtr ipPoint1, IPointPtr ipPoint2);

	/**
	 * @brief   �|�C���g1�ƃ|�C���g2�̊p�x�����߂�
	 * @param lon1 [in] �|�C���g1�̌o�x
	 * @param lat1 [in] �|�C���g1�̈ܓx
	 * @param lon2 [in] �|�C���g2�̌o�x
	 * @param lat2 [in] �|�C���g2�̈ܓx
	 * @return �|�C���g1���|�C���g2�̊p�x(���W�A��)
	 */
	double GetAngle(double lon1, double lat1, double lon2, double lat2);

	/**
	 * @brief   HWY�n�̑����l�ƃC���X�g�����N�̑����l�̐������`�F�b�N
	 * @param spRow [in] �C���X�g�����N
	 * @param vecNodeQueue [in] �m�[�h��
	 */
	void CheckRelHighwayInfo( sindy::CSPIllustLinkRow& spRow, const sindy::CNodeQueueCheck::NODEQUEUE& vecNodeQueue );

	/**
	 * @brief   ���H�m�[�h�̈ʒu�ɂ���HIGHWAY_NODE����HWY�n�̏����֘A�t����
	 * @param ipNode [in] ���H�m�[�h
	 * @retval true  �擾����
	 * @retval false �擾���s(HIGHWAY_NODE���Ȃ��ꍇ������)
	 */
	bool AssociateHighwayInfo( IFeaturePtr ipNode );

	/**
	 * @brief   �`�F�b�N�Ώۂ�HWY�f�[�^�ł��邩
	 * @param lIllustC [in] �C���X�g���
	 * @param cHighwayInfo [in] HWY�f�[�^
	 * @retval true  �Ώۂł���
	 * @retval false �ΏۂłȂ�
	 */
	bool IsTargetHWYInfo(long lIllustC, CHighwayInfo cHighwayInfo);

	/**
	 * @brief   HWY�n�̑����l�ƃC���X�g�����N�̑����l�̐������`�F�b�N
	 * @param spRow [in] �C���X�g�����N
	 * @param cHighwayInfo [in] HWY�n�̏��
	 * @retval true  ����������ꂽ�f�[�^
	 * @retval false �s�����ȃf�[�^
	 */
	bool CheckRelHighwayInfo( sindy::CSPIllustLinkRow& spRow, CHighwayInfo& cHighwayInfo );

	/**
	 * @brief   2�_�̓��H�m�[�h���瓹�H�����N������
	 * @note CNodeQueueCheck::FindLink����(�C���X�^���X���̖ʓ|�Ȃ̂�)
	 * @param node1 [in] �m�[�h1
	 * @param node2 [in] �m�[�h2
	 * @return �������ꂽ���H�����N
	 */
	sindy::ROADLINKMAP::const_iterator FindLink(long node1, long node2) const;

	sindy::ROADLINKMAP::const_iterator EndLink() const { return m_mapRoadLink.end(); }

	sindy::FEATUREMAP::const_iterator FindNode(long node) const;

	sindy::FEATUREMAP::const_iterator EndNode() const{ return m_mapRoadNode.end(); }
	
	/**
	 * @brief   ���s���O���擾����
	 * @return  ���s���O
	 */
	std::wofstream& RunLog() const { return *m_ofsRunLog; }

	/**
	 * @brief   �G���[���O���擾����
	 * @return  �G���[���O
	 */
	std::wofstream& ErrLog() const { return *m_ofsErrLog; }

	/**
	 * @brief   �G���[���O�ɃR�����g����������
	 * @param message [in] ���b�Z�[�W
	 */
	void WriteComment(const CString& message);

	/**
	 * @brief   �S�Ẵ��O�ɃR�����g����������
	 * @param message [in] ���b�Z�[�W
	 */
	void AllWriteComment(const CString& message);

	/**
	 * @brief   �G���[���O�ɃG���[���b�Z�[�W����������
	 * @param spRow [in] �C���X�g�����N
	 * @param errInfo [in] �G���[���
	 */
	void WriteError( const sindy::CSPIllustLinkRow& spRow, const IllustErrorInfo& errInfo);

	/**
	 * @brief   �G���[���O�ɃG���[���b�Z�[�W����������
	 * @param field [in] �t�B�[���h��
	 * @param oid [in] OID
	 * @param errInfo [in] �G���[���
	 * @param lIllustC [in] �C���X�g���
	 * @param lCustomer [in] �d����
	 * @param illustName [in] �C���X�g��
	 */
	void WriteError( const CString& field, long oid, const IllustErrorInfo& errInfo, long lIllustC, long lCustomer, const CString& illustName,
					const CString& strDate, const CString& strEditor);

	/**
	 * @brief   �ŐV�̍X�V�����A�ҏW�҂��擾�iProgModify��Modify��)
	 * @param spRow [in] �C���X�g�����N
	 * @param strDate [out] �X�V����
	 * @param strEditor [out] �X�V��
	 */
	void GetNewestEditInfo(const sindy::CSPIllustLinkRow& spRow, CString& strDate, CString& strEditor);

	/**
	 * @brief   �t�B�[���h�����������擾
	 * @param spRow [in] �C���X�g�����N
	 * @param strField [in] �t�B�[���h��
	 */
	CString GetTime(const sindy::CSPIllustLinkRow& spRow, const CString& strField);

	/**
	 * @brief   �I�����O����������
	 * @param   result [in] ���s����
	 */
	void WriteEndLog(bool result);

	/**
	 * @brief   From Node�@�� To Node �ɂ���ă����N���擾
	 * @param   [in]�@firstNodeId From Node
	 * @param   [in]�@secondNodeId To Node
	 * @return  �����N�t�B�[�`��
	 */
	IFeaturePtr getRoadLinkFeature(long firstNodeId, long secondNodeId);

	/**
	 * @brief   �����N�t�B�[�`���N���[�X �ɂ���ă����N�̎�ރt���O���擾
	 * @param   [in]�@ipLink �����N�t�B�[�`���N���[�X
	 * @param   [out]�@isExistRoundAboutLink ���E���h�A�o�E�g�����N�t���O
	 * @param   [out]�@isExistIntersectionLink �����_�����N�����N�t���O
	 * @param   [out]�@isExistNormalLink ���ʃ����N�t���O
	 */
	void getRoadLinkClassFlag(const IFeaturePtr& ipLink, bool& isExistRoundAboutLink, bool& isExistIntersectionLink, bool& isExistNormalLink );

private:

	Arguments m_args;                  //!< ����
	IWorkspacePtr m_ipRoadWorkspace;   //!< ���H���[�N�X�y�[�X
	IWorkspacePtr m_ipIllustWorkspace; //!< �C���X�g�����N���[�N�X�y�[�X

	IFeatureClassPtr m_ipRoadLinkFC;    //!< ���H�����N�t�B�[�`���N���X
	IFeatureClassPtr m_ipRoadNodeFC;    //!< ���H�m�[�h�t�B�[�`���N���X
	IFeatureClassPtr m_ipHighwayNodeFC; //!< HIGHWAY_NODE�t�B�[�`���N���X
	IFeatureClassPtr m_ipFacilInfoFC;   //!< FACIL_INFO_POINT�t�B�[�`���N���X
	ITablePtr m_ipRoadCodeT;            //!< ROAD_CODE_LIST�e�[�u��
	ITablePtr m_ipLQDirguideT;          //!< LQ_DIRGUIDE�e�[�u��
	ITablePtr m_ipLQTurnRegT;           //!< LQ_TURNREG�e�[�u��
	ITablePtr m_ipINFTurnRegT;          //!< INF_TURNREG�e�[�u��
	ITablePtr m_ipSubRoadLinkT;         //!< SUBROAD_LINK�e�[�u��
	ITablePtr m_ipSubRoadPartT;         //!< SUBROAD_LINK�e�[�u��
	EMode m_Mode;                       //!< ���s���[�h
	std::wofstream* m_ofsRunLog;         //!< ���s���O
	std::wofstream* m_ofsErrLog;         //!< �G���[���O

	CIllustDirInfo  m_DirInfo;          //!< �C���X�g�t�@�C���i�[����
	CIllustRuleInfo m_RuleInfo;         //!< �C���X�g�t�@�C�����������[��

	sindy::CTableFinder m_finder;         //!< �e�[�u���t�@�C���_
	sindy::ROADLINKMAP m_mapRoadLink;     //!< �C���X�g�����N�Ɋ֘A���铹�H�����N�S��(FROM,TO�m�[�hID���L�[)
	sindy::FEATUREMAP m_mapRoadNode;      //!< �C���X�g�����N�Ɋ֘A���铹�H�m�[�h�S��
	sindy::TURNREGMAP m_mapTurnReg;       //!< �C���X�g�����N�Ɋ֘A����E���܋֎~���S��
	sindy::TURNREGINFMAP m_mapTurnRegInf; //!< �C���X�g�����N�Ɋ֘A���铹�H�����N����INF_ID���t�������邽�߂̏��
	std::set<long> m_unopenedRoadId;      //!< ���J�ʘH���Ɋ֘A���铹�H�����NID
	std::map<long, std::vector<long>> m_mapRelNodeHwyNode; //!< �L�[�F�m�[�hID�A�l�FHIGHWAY_NODE��OID�Q(��Ԍ����̉񐔂����炷���߁B)
	std::map<long, CHighwayInfo> m_mapHighwayInfo; //!< �L�[�FHIGHWAY_NODE�A�l�FHWY�n���

	bool m_isJapaneseOs;                   //!< �V�X�e�������{����t���O(�V�X�e������ɂ���Ĕ���A�l��0x0411�̏ꍇ(���{���):true, �l��0x0411�ł͂Ȃ��ꍇ(�p���):false)
	bool m_isAseanSchema;                  //!< ASEAN�����t���O(WorldMeshMode�ɂ���Ĕ���A�l��1�̏ꍇ(ASEAN����):true, �l��0�̏ꍇ(��������):false)
};

