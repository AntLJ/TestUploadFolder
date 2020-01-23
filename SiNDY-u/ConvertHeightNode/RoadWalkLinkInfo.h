#pragma once

struct RoadWalkLinkRec
{
	long			e_divid;
	IGeometryPtr	e_ipGeoemtry;

	RoadWalkLinkRec()
		:e_divid(-1)
	{}

	bool operator < (const RoadWalkLinkRec roadWalkLinkRec) const {
		return e_divid < roadWalkLinkRec.e_divid;
	}
};

class RoadWalkLinkInfo
{
public:
	RoadWalkLinkInfo(void);
	~RoadWalkLinkInfo(void);


	/*
	 *	@brief	static�����o�ϐ��̏�����
	 *	@note	���[�g�p�X�̐ݒ�̂�
	 *	@param	shpRootPath [in] ���H�E���s��Shape�i�[�t�H���_���[�g�p�X
	 */
	void Init(LPCTSTR shpRootPath){
		m_shpRootPath = shpRootPath;
	};

	/*
	 *	@brief	�ϊ��Ώۓ��H���s�҃����N�Z�b�g
	 *	@param	mesh [in] �ϊ��Ώۃ��b�V��
	 *	@retval	true ����
	 *	@retval false ���s
	 */
	bool SetRoadWalkLinkInfo(const long secondMesh);

	/*
	 *	@brief	�C�ӂ�objectid�������H�E���s��Shape���̃C�e���[�^���擾(const_iterator)
	 *	@param	objectid [in] ��������objectid
	 *	@return	�R���X�g�C�e���[�^
	 */
	std::map<long, std::set<RoadWalkLinkRec>>::const_iterator find(const long objectid) const
	{
		return m_recordInfo.find(objectid);
	}

	/*
	 *	@brief	���H�E���s��Shape���R���e�i�̏I�[���w���C�e���[�^�擾(const_iterator)
	 *	@return �R���X�g�C�e���[�^
	 */
	std::map<long, std::set<RoadWalkLinkRec>>::const_iterator end() const
	{
		return m_recordInfo.end();
	}


private:
	/*
	 *	@brief	�C���f�b�N�X�擾
	 *	@param	ipFeatureClass [in] ���H�E���s�҃����N�t�B�[�`���N���X
	 *	@param	shpName [in] shp�t�@�C����
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool SetIndex(const IFeatureClassPtr& ipFeatureClass, LPCTSTR shpName);

	/*
	 *	@brief	���H�E���s�҃����N���R�[�h���擾
	 *	@param	ipFeature [in] ���H�E���s�҃����N�t�B�[�`��
	 *	@param	shpName [in] shp�t�@�C����
	 *	@param	objectid [out] objectid
	 *	@param	roadWalkLinkRec [out] ���H�E���s�҃��R�[�h���
	 *	@retval	true ����
	 *	@retval false ���s
	 */
	bool GetRecInfo(const IFeaturePtr& ipFeature, LPCTSTR shpName, long& objectid, RoadWalkLinkRec& roadWalkLinkRec);

private:
	static	CString m_shpRootPath;								//!<	���H�E���s��Shape�i�[�t�H���_���[�g�p�X
	std::map<long, std::set<RoadWalkLinkRec>>	m_recordInfo;	//!<	���H�E���s��Shape���(�L�[objectid)

	static	long m_objectidIndex;
	static	long m_dividIndex;
};

