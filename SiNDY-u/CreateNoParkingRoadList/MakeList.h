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

class CNoParkingLink
{
public:
	CNoParkingLink() : lNoParkingClass(0) {};
	~CNoParkingLink(){};
public:
	IFeaturePtr ipLink;
	long lNoParkingClass;
};

class CMakeList
{
public:
	CMakeList(void);
	~CMakeList(void);

	/**
	 * @brief ���֏d�_����H�����X�g�쐬�������s���B
	 * @note  ���֏d�_����H�����X�g�쐬�������s���B
	 *
	 * @param	ipLinkClass		[in]		���H�����N�N���X
	 * @param	vecFeatureClass	[in]		���֎���H���N���X���X�g
	 * @param	bShape			[in]		���֎���H���N���X���X�g��shape��PGDB��[true:shape,fales:pgdb]
	 * @param	ofp				[out]		�o�͐惊�X�g
	 * @param	lpcszShapeDir	[in]		�o��shape�f�B���N�g��
	 * @param	lpcszLinkDir	[in]		���ԋ֎~�����Ԃ̃��C���̏o��shape�f�B���N�g��
	 *
	 * @return	true		����I��
	 * @return	false		�ُ�I��
	 */
	bool create( IFeatureClass* ipLinkClass, const std::vector<CAdapt<IFeatureClassPtr>>& vecFeatureClass, bool bShape, std::ofstream& ofp, LPCTSTR lpcszShapeDir, LPCTSTR lpcszLinkDir );

private:

	/**
	 * @brief PGDB�̒��ԋ֎~����ʖ��ɑΏۃ����N���o�������s���B
	 * @note  PGDB�̒��ԋ֎~����ʖ��ɑΏۃ����N���o�������s���B
	 *
	 * @param	ipLinkClass			[in]		���H�����N�N���X
	 * @param	ipNoParkClass		[in]		���֎���H���N���X
	 * @param	lpcszWhereClause	[in]		�t�B�[�`�������p
	 * @param	ofp					[out]		�o�͐惊�X�g
	 * @param	bFlag				[in]		�ŏd�_�t���O(true:�ŏd�_�Afalse:�d�_)
	 *
	 * @return	true		����I��
	 * @return	false		�ُ�I��
	 */
	bool SearchClass( IFeatureClass* ipLinkClass, IFeatureClass* ipNoParkClass, LPCTSTR lpcszWhereClause, std::ofstream &ofp, bool bFlag );

	/**
	 * @brief �w�肵�������̃o�b�t�@���쐬����B
	 * @note  �w�肵���������̃o�b�t�@�������N�Ɏ��������`����쐬����B
	 *
	 * @param	ipGeo		[in]		���H�����N�`��
	 *
	 * @return �o�b�t�@���������`��
	 */
	IGeometryPtr GetBuffer( IGeometry* ipGeo );

	/**
	 * @brief �o�b�t�@�Ɋ܂܂�郊���N���擾����B
	 * @note  �o�b�t�@�Ɋ܂܂�Ă��郊���N���擾���A���肷��B
	 *
	 * @param	ipLinkClass	[in]		���H�����N�N���X
	 * @param	ipBuff		[in]		�o�b�t�@�`��
	 * @param	ofp			[out]		�o�̓��X�g
	 * @param	lClass		[in]		���ԋ֎~�����
	 *
	 * @return �o�b�t�@���������`��
	 */
	bool CompData( IFeatureClass* ipLinkClass, IGeometry* ipBuff, std::ofstream& ofp, long lClass );

	/**
	 * @brief �o�b�t�@�Ɋ܂܂�Ă��郊���N���𑪒肷��B
	 * @note  �w�肵�������N�̃o�b�t�@�Ɋ܂܂�Ă��钷���𑪒肷��B
	 *
	 * @param	ipLinkGeo	[in]		���H�����N
	 * @param	ipPolygon	[in]		���֘H���o�b�t�@
	 * @param	lOID		[in]		�����NID
	 *
	 * @return �����N��(m)
	 */
	double GetIntersecLinkLen( IGeometry* ipLinkGeo, IGeometry* ipPolygon, long lOID );

	/**
	 * @brief �����N���𑪒肷��B
	 * @note  �w�肵�������N�̒����𑪒肷��B
	 *
	 * @param  ipLinkGeom	 [in]				���H�����N
	 *
	 * @return �����N��(m)
	 */
	double GetLinkLen( IGeometry* ipLinkGeom );

	/**
	 * @brief �����N�����݂��郁�b�V�����擾����B
	 * @note  �w�肵�������N�����݂��郁�b�V���R�[�h���擾����B
	 *
	 * @param  ipLink		 [in]				���H�����N�`��
	 *
	 * @return 2�����b�V���R�[�h
	 */
	long GetExistMesh( IGeometry* ipLink );

	/**
	* @brief 10�i����16�i���\���ɕϊ�����B
	* @note  �w���10�i���l��16�i���\���ɕϊ�����B
	*
	* @param	rCstrID		[out]	16�i���\���p������
	* @param	rId			[in]	10�i���l
	*
	* @return	�Ȃ�
	*/
	void SetObjectID( CString& rCstrID, int rId) ;

	/**
	* @brief	���ԋ֎~�H���̓��H�����N��shape�ŏo��
	* @param	ipLinkClass		[in]		���H�����N�N���X
	* @retval	true	�o�͐���
	* @retval	false	�o�͎��s
	*/
	bool OutputShape( IFeatureClass* ipLinkClass );

	/**
	* @brief	���H�����N�`���shape�`��ɂ���
	* @param	cSHPHandle		[in]		Shape�n���h��
	* @param	ipGeom			[in]		���H�����N�`��
	* @return	shape��OID
	*/
	int CreateOneShape( CSHPHdl& cSHPHandle, IGeometry* ipGeom );

	/**
	* @brief	�����H�̌`���shape�ŏo��
	* @note		PGDB�ł͂Ȃ�shape�̏ꍇ�͓s���{���ʂɂȂ��Ă���̂ł������ɂ܂Ƃ߂�������
	* @param	vecBaseLink		[in]		�����H�Q
	*/
	void OutputBaseLink( const std::vector<CAdapt<IFeaturePtr>>& vecBaseLink );

	/**
	* @brief	19���W�n��10�̈�ɍ��W�ϊ������`��̃N���[���쐬
	* @note		��ԎQ�Ƃ��Z�b�g����Ă��Ȃ��`��̏ꍇ�͏����l��ݒ�iesriSRGeoCS_Tokyo:4301�j
	*/
	IGeometryPtr CMakeList::get_Tokyo10CloneGeom(IGeometry* ipGeom);

private:
	std::map<long,CNoParkingLink> m_mapNoParkingLink;	//!< �ŏd�_�H�������NID�Ƃ̒��ԋ֎~�����N�̃}�b�v
	std::map<long, double> m_LinkLength;	//!< �����N����

	std::vector<CAdapt<IFeaturePtr>> m_vecBaseLink;		//!< ���ԋ֎~�����Ԃ̃��C���Q

	long m_lCount;							//!< �t�B�[�`���N���X�Ń��[�v��������Ƃ��̃J�E���^
	long m_lFeatureClassSize;				//!< �t�B�[�`���N���X�̑���

	CString m_strShapeDir;					//!< �o��shape�f�B���N�g��
	CString m_strLinkDir;					//!< ���ԋ֎~�����Ԃ̃��C���̏o��shape�f�B���N�g��
};
