#pragma once

/*
 *	@brief	�x���W���̓��H�����N�̒��������̎Z�o����
 *	@param	warningSignPoint [in] �x���W���|�C���g
 *	@param	nearestPoint [in] ���H�����N�ŋߖT�|�C���g
 *	@param	distance [out]���������i�[�p�ϐ�
 *	@retval	true ����
 *	@retval	false ���s
 */
bool directDistance(const IPointPtr& warningSignPoint, const IPointPtr& nearestPoint, double& distance);

/*
 *	@brief	�R�[�h�l�h���C����������
 *	@param	mapDomain [in] �R�[�h�l�h���C�����X�g
 *	@param	ClassCode [in] �R�[�h�l
 *	@param	domainName [out] �h���C����
 *	@retval	true ����
 *	@retval	false ���s
 */
bool getDomainName(const std::map<long, CString>& mapDomain, const long& ClassCode, CString& domainName);

/*
 *	@brief	�ŋߖT���H�����N�o�͏���
 *	@param	transParam [in] �e��p�����[�^
 *	@param	ipRoadFC [in] ���H�t�B�[�`���N���X
 *	@param	ipRoadSPRef [in] ��ԎQ�Ə��
 *	@param	roadClassIndex [in] ���H�\�����̃t�B�[���h�ԍ�
 *	@param	mapGeo [in] �x���W���f�[�^���
 *	@param	mapDomain [in] �R�[�h�l�h���C�����X�g
 *	@param	ofsOutputLog [out] ���s���O�p�t�@�C���X�g���[��
 *	@retval	true ����
 *	@retval	false ���s
 */
bool checkWarningSign(const TransParam& transParam, const IFeatureClassPtr& ipRoadFC, const ISpatialReferencePtr& ipRoadSPRef, const long& roadClassIndex, const std::map<long, CAdapt<IPointPtr>>& mapGeo, const std::map<long, CString>& mapDomain, std::ofstream	&ofsOutputLog);