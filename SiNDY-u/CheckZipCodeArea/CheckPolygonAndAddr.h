#pragma once

#include "CheckBase.h"
#include <TDC/sindylib_core/FieldMap.h>
#include "TDC/RepPointCalculator/RepPointCalculator.h"

class CheckPolygonAndAddr
	: public CheckBase
{
public:
	CheckPolygonAndAddr();
	virtual ~CheckPolygonAndAddr();

	/**
	 * @brief ������
	 * @retval true ����
	 * @retval false ���s
	 */
	bool Init();

	/**
	 * @brief �`�F�b�N���s
	 * @retval true �G���[�Ȃ�
	 * @retval false �G���[����
	 */
	bool Check();

private:
	/**
	 * @brief �s���E���X�֔ԍ��ɕ�܂���Ă��邩�`�F�b�N
	 * @retval true �G���[�Ȃ�
	 * @retval false �G���[����
	 */
	bool CheckContainCityAdmin();

	/**
	 * @brief �Z���f�[�^(���ACS)���X�֔ԍ�����܂���Ă��邩�`�F�b�N
	 * @retval true �G���[�Ȃ�
	 * @retval false �G���[����
	 */
	bool CheckContainAddrPoint();

	/**
	 * @brief �X�֔ԍ��|���S�����Z���f�[�^���܂��Ă��邩�`�F�b�N
	 * @retval true �G���[�Ȃ�
	 * @retval false �G���[����
	 */
	bool ZipToAddrCheck(
		const sindy::CTableContainer& zipCodeAreaTbl,
		const addr::AddrRecs& existRecs,
		const CString& zipCode,
		const CString& addrCode);

	/**
	 * @brief �Z���f�[�^���X�֔ԍ��|���S���ɕ�܂���Ă��邩�`�F�b�N
	 * @retval true �G���[�Ȃ�
	 * @retval false �G���[����
	 */
	bool AddrToZipCheck(
		const sindy::CTableContainer& zipCodeAreaTbl,
		const addr::AddrRecs& existRecs,
		const CString& zipCode,
		const CString& addrCode);

private:
	addr::AddrRecHelper m_addrRecHelper; //!< �Z�����R�[�h�����⏕�N���X
	sindy::CTableFinder m_addrFinder; //!< �Z���f�[�^(�s���E�A���ACS)�擾�p
	sindy::CTableFinder m_relAddrFinder; //!< �R�t���e�[�u���擾�p
	RepPointCalculator m_repPointCalculator; //!< ��\�_�Z�o�N���X

};

