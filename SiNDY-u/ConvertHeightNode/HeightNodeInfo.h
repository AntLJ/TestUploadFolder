#pragma once


struct HeightNodeRec
{
	IGeometryPtr e_ipGeometry;
	long e_linkid1;
	long e_height1;
	long e_table1;
	long e_linkid2;
	long e_height2;
	long e_table2;

	HeightNodeRec()
		:e_linkid1(-1),e_height1(0),e_table1(-1),
		 e_linkid2(-1),e_height2(0),e_table2(-1)
	{}
};


/**
 *	@brief	HeightNodeInfo���N���X
 */

class HeightNodeInfo
{
public:
	HeightNodeInfo(const IFeatureClassPtr& ipFeatureClass, const ISpatialReferencePtr& ipSpRef);
	~HeightNodeInfo(void);

	/*
	 *	@brief	static�����o�ϐ��̏�����
	 *	@note	�C���f�b�N�X�擾
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool Init();

	/*
	 *	@brief	�ϊ��Ώ�HeightNode�Z�b�g
	 *	@param	ipMeshGeoemtry [in] ���b�V���`��
	 *	@retval true ����
	 *	@retval	false ���s
	 */
	bool SetHeightNodeInfo(const IGeometryPtr ipMeshGeometry);

	/*
	 *	@brief HeightNode�R���e�i�̐擪���w���C�e���[�^�擾(const_iterator)
	 *	@return �R���X�g�C�e���[�^
	 */
	std::map<long, HeightNodeRec>::const_iterator begin() const
	{
		return m_recordInfo.begin();
	}

	/*
	 *	@brief HeightNode�R���e�i�̏I�[���w���C�e���[�^�擾(const_iterator)
	 *	@return �R���X�g�C�e���[�^
	 */
	std::map<long, HeightNodeRec>::const_iterator end() const
	{
		return m_recordInfo.end();
	}

	/*
	 *	@brief	HeightNode���C���̋�ԎQ�Ǝ擾
	 *	@return ipSpatialReferencePtr
	 */
	ISpatialReferencePtr GetSpRef() const
	{
		return m_ipSpRef;
	}

private:
	/*
	 *	@brief	HeightNode���R�[�h���擾
	 *	@param	ipFeature [in] HeightNode�t�B�[�`��
	 *	@param	heightNodeRec [out] HeightNode���R�[�h���
	 *	@retval true ����
	 *	@retval false ���s
	 */
	bool GetRecInfo(const IFeaturePtr& ipFeature, HeightNodeRec& heightNodeRec);


private:
	IFeatureClassPtr m_ipFeatureClass;
	ISpatialReferencePtr m_ipSpRef;
	
	// �ȉ��e��t�B�[���h��index��Init���s���Ɏ擾���A�C���X�^���X�ԂŎg���܂킵�����̂�static�Ƃ��Ă���
	static long m_linkid1Index;
	static long m_height1Index;
	static long m_linkTable1Index;
	static long m_linkid2Index;
	static long m_height2Index;
	static long m_linkTable2Index;

	std::map<long, HeightNodeRec> m_recordInfo;	//!< HeightNode���(�L�[�Fobjectid)
};

