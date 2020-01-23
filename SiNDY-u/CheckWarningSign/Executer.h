#pragma once

/*
 *	@brief	���s���O�E�G���[���O�쐬����
 *	@param	TransParam [in] �e��p�����[�^
 *	@return true ����
 *	@return false ���s
 */
bool initLog(const TransParam &transParam);

/*
 *	@brief	�c�[�����o�͏���
 *	@param	TransParam [in] �e��p�����[�^
 *	@return true ����
 *	@return false ���s
 */
void procInfoWrite(TransParam &transParam);

/*
 *	@brief	�c�[�����o�͏���
 *	@param	TransParam [in] �e��p�����[�^
 *	@param	ofsOutputLog [out] ���s���O�p�t�@�C���X�g���[��
 *	@return true ����
 *	@return false ���s
 */
bool openOutputLog(const TransParam& transParam, std::ofstream	&ofsOutputLog);

/*
 *	@brief	�R�[�h�l�h���C�����X�g�쐬����
 *	@param	ipRoadField [in] ���H�t�B�[���h
 *	@param	mapDomain [out] �R�[�h�l�h���C�����X�g
 *	@return true ����
 *	@return false ���s
 */
bool domainlist(const IFieldPtr& ipRoadField, std::map<long, CString>& mapDomain);

/*
 *	@brief	���H�����N�f�[�^�̎擾
 *	@param	transParam [in] �e��p�����[�^
 *	@param	ipRoadWorkspace [in] ���H���[�N�X�y�[�X
 *	@param	ipRoadFC [out] ���H�t�B�[�`���N���X
 *	@param	ipRoadSPRef [out] ��ԎQ�Ə��
 *	@param	mapDomain [out] �R�[�h�l�h���C�����X�g
 *	@param	roadClassIndex [out] ���H�\�����̃t�B�[���h�ԍ�
 *	@return true ����
 *	@return false ���s
 */
bool getRoadLink(const TransParam& transParam, const IWorkspacePtr& ipRoadWorkspace, IFeatureClassPtr& ipRoadFC, ISpatialReferencePtr& ipRoadSPRef, std::map<long, CString>& mapDomain, long& roadClassIndex);

/*
 *	@brief	�x���W���f�[�^�̎擾
 *	@param	transParam [in] �e��p�����[�^
 *	@param	ipWSWorkspace [in] �x���W�����[�N�X�y�[�X
 *	@param	ipWSFC [out] �x���W���t�B�[�`���N���X
 *	@param	mapGeo [out] �x���W���f�[�^���
 *	@return true ����
 *	@return false ���s
 */
bool getWarningSign(const TransParam& transParam, const IWorkspacePtr& ipWSWorkspace, IFeatureClassPtr& ipWSFC, std::map<long, CAdapt<IPointPtr>>& mapGeo);

/*
 *	@brief	�S�̏���
 *	@param	transParam [in] �e��p�����[�^
 *	@return true ����
 *	@return false ���s
 */
bool exec(const TransParam &transParam);