#pragma once
#include "HeightNodeInfo.h"
#include "RoadWalkLinkInfo.h"

struct OutSHPRec
{
	IGeometryPtr e_ipGeoemtry;
	long e_linkid1;
	long e_divid1;
	long e_height1;
	long e_linkid2;
	long e_divid2;
	long e_height2;

	OutSHPRec(IGeometryPtr ipGeometry,
			  long linkid1, long divid1, long height1,
			  long linkid2, long divid2, long height2)
		:e_ipGeoemtry(ipGeometry),
		 e_linkid1(linkid1), e_divid1(divid1), e_height1(height1),
		 e_linkid2(linkid2), e_divid2(divid2), e_height2(height2)
	{}
};

class CnvData
{
public:
	CnvData(void);
	~CnvData(void);

	/*
	 *	@brief	static�����o�ϐ��̏�����
	 *	@note	�������莞�̃o�b�t�@�̐ݒ�
	 *	@param	buffer [in] �o�b�t�@
	 */
	void Init(const double& buffer) {
		m_buffer = buffer;
	}

	/*
	 *	@brief	���΍���Shape�ϊ�
	 *	@param	secondMesh [in] �ϊ��Ώۃ��b�V��
	 *	@param  heightNodeInfo [in] HeightNode���
	 *	@param	roadWalkLinkInfo [in] ���H�E���s�҃����N���
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool ConverteHeightNode(const long secondMesh, const HeightNodeInfo& heightNodeInfo, const RoadWalkLinkInfo& roadWalkLinkInfo);

	/*
	 *	@brief	�ϊ��f�[�^�R���e�i�̐擪���w���C�e���[�^�擾(const_iterator)
	 *	@return �R���X�g�C�e���[�^
	 */
	std::map<long, OutSHPRec>::const_iterator begin() const
	{
		return m_recordInfo.begin();
	}
	/*
	 *	@brief  �C�ӂ�objectid�����ϊ��f�[�^�R���e�i�̃C�e���[�^���擾(const_iterator)
	 *	@param	objectid [in] ��������objectid
	 *	@return �R���X�g�C�e���[�^
	 */
	std::map<long, OutSHPRec>::const_iterator find(const long objectid) const
	{
		return m_recordInfo.find(objectid);
	}

	/*
	 *	@brief	�ϊ��f�[�^�R���e�i�̏I�[���w���C�e���[�^�擾(const_iterator)
	 *	@return �R���X�g�C�e���[�^
	 */
	std::map<long, OutSHPRec>::const_iterator end() const
	{
		return m_recordInfo.end();
	}

#ifdef _DEBUG_DIV
	long size() const
	{
		return m_recordInfo.size();
	}
#endif // _DEBUG_DIV

private:
	/*
	 *	@brief	���������N�擾
	 *	@param	secondMesh [in] ���b�V��
	 *	@param	heightNodeID [in] ���΍����m�[�hID
	 *	@param	linkID [in] �����Ώۃ����NID(���΍����m�[�h���ێ����Ă���ID)
	 *	@param	tableCode [in] linkID�̃e�[�u�����
	 *	@param	ipHeightNodeGeo [in] ���΍����`��
	 *	@param	ipSpRef [in] ���΍����̋�ԎQ��
	 *	@param	roadWalkLinkInfo [in] ���H�E���s�҃����N���
	 *	@param	roadWalkID [out] ���������N��OBJECTID
	 *	@param	roadWalkLinkRec [out] ���������N�̑������
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SelectTgtLink(const long secondMesh,
					   const long heightNodeID, 
					   const long linkID,
					   const long tableCode,
					   const IGeometryPtr& ipHeightNodeGeo, 
					   const ISpatialReferencePtr& ipSpRef,
					   const RoadWalkLinkInfo& roadWalkLinkInfo,
					   long& roadWalkID,
					   RoadWalkLinkRec& roadWalkLinkRec);
   

private:
	static double				m_buffer;		//!<	�������莞�̃o�b�t�@
	std::map<long, OutSHPRec>	m_recordInfo;	//!<	�ϊ��㑊�΍������R�[�h���
};

