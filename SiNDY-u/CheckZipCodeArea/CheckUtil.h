#pragma once

class CheckUtil
{
public:
	CheckUtil(addr::AddrRecHelper& addrRecHelper);
	virtual ~CheckUtil();

	/**
	 * @brief 11���{���̂̃��X�g�����
	 * @param relAddrTbl [in] �Z���R�t���e�[�u��
	 * @param relAddrMap [out] ���X�g
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool MakeAddrYomiList(
		const sindy::CTableContainer & relAddrTbl,
		std::map<CString, addr::AddrRecPairs>& relAddrMap);

	/**
	 * @brief �Z���f�[�^�̑��ݔ���
	 * @param adminAddrRecs [in] �s�s�n�}�s���E�f�[�^
	 * @param csAddrRecs [in] CS�|�C���g�f�[�^
	 * @param gouPointTbl [in] ���|�C���g�f�[�^
	 * @param addrRec [in] ����Ώۃf�[�^
	 * @retval true ���݂���
	 * @retval false ���݂��Ȃ�
	 */
	bool CheckAddressDataExistance(
		const addr::AddrRecPairs& adminAddrRecs,
		const addr::AddrRecs& csAddrRecs,
		sindy::CTableContainer& gouPointTbl,
		const addr::AddrRec& addrRec
		);


	/**
	 * @brief �Z���f�[�^�̑��ݔ���B���݂���ꍇ��AddrRec�Ƃ��Ď擾�B
	 * @param adminAddrRecs [in] �s�s�n�}�s���E�f�[�^
	 * @param csAddrRecs [in] CS�|�C���g�f�[�^
	 * @param gouPointTbl [in] ���|�C���g�f�[�^
	 * @param addrRec [in] ����Ώۃf�[�^
	 * @retval true ���݂���
	 * @retval false ���݂��Ȃ�
	 */
	bool CheckAddressDataExistance(
		const addr::AddrRecPairs& adminAddrRecs,
		const addr::AddrRecs& csAddrRecs,
		sindy::CTableContainer& gouPointTbl,
		const addr::AddrRec& addrRec,
		addr::AddrRecs& existAddrRecs
	);


private:
	/**
	 * @brief 11�����獆�|�C���g�擾
	 * @param relAddrTbl [in] ���|�C���g�f�[�^(���o��)
	 * @param relAddrTbl [in] �s���E�f�[�^
	 * @param relAddrMap [out] ���|�C���g�f�[�^(���o��)
	 */
	void getGpAddrRecs(
		sindy::CTableContainer& gouPointTbl,
		const addr::AddrRecPairs& caAddrRecPairs,
		addr::AddrRecs& gpAddrRecs
	);

	/**
	 * @brief 11���Z���R�[�h�Ƃ��Ă̔���
	 * @param caAddrRecPairs [in] �s���E�f�[�^
	 * @param csAddrRecs [in] CS�|�C���g�f�[�^
	 * @param rec [in] ����Ώۃf�[�^
	 * @retval true �s���E�ACS�|�C���g�ƈ�v����
	 * @retval false �s���E�ACS�|�C���g�ƈ�v���Ȃ�
	 */
	bool decideAddrDigitStatus(
		const addr::AddrRecPairs& caAddrRecPairs,
		const addr::AddrRecs& csAddrRecs,
		const addr::AddrRec& rec
	);

	/**
	 * @brief 11���Z���R�[�h�Ƃ��Ă̔���(����true�̃��R�[�h��Ԃ���)
	 * @param caAddrRecPairs [in] �s���E�f�[�^
	 * @param csAddrRecs [in] CS�|�C���g�f�[�^
	 * @param rec [in] ����Ώۃf�[�^
	 * @param existAddrRecs [out] ���肪true�ɂȂ����s���E�ACS���R�[�h
	 * @retval true �s���E�ACS�|�C���g�ƈ�v����
	 * @retval false �s���E�ACS�|�C���g�ƈ�v���Ȃ�
	 */
	bool decideAddrDigitStatus(
		const addr::AddrRecPairs& caAddrRecPairs,
		const addr::AddrRecs& csAddrRecs,
		const addr::AddrRec& rec,
		addr::AddrRecs& existAddrRecs
	);

	/**
	 * @brief 16���Z���R�[�h�Ƃ��Ă̔���
	 * @param caAddrRecPairs [in] �s���E�f�[�^
	 * @param gpAddrRecs [in] ���|�C���g�f�[�^
	 * @param csAddrRecs [in] CS�|�C���g�f�[�^
	 * @param rec [in] ����Ώۃf�[�^
	 * @retval true �s���E�ACS�A���|�C���g�ƈ�v����
	 * @retval false �s���E�ACS�A���|�C���g�ƈ�v���Ȃ�
	 */
	bool decideChibanDigitStatus(
		const addr::AddrRecPairs& caAddrRecPairs,
		const addr::AddrRecs& gpAddrRecs,
		const addr::AddrRecs& csAddrRecs,
		const addr::AddrRec& rec
	);

	/**
	 * @brief 16���Z���R�[�h�Ƃ��Ă̔���(����true�̃��R�[�h��Ԃ���)
	 * @param caAddrRecPairs [in] �s���E�f�[�^
	 * @param gpAddrRecs [in] ���|�C���g�f�[�^
	 * @param csAddrRecs [in] CS�|�C���g�f�[�^
	 * @param rec [in] ����Ώۃf�[�^
	 * @param existAddrRecs [out] ���肪true�ɂȂ����s���E�A���ACS���R�[�h
	 * @retval true �s���E�ACS�A���|�C���g�ƈ�v����
	 * @retval false �s���E�ACS�A���|�C���g�ƈ�v���Ȃ�
	 */
	bool decideChibanDigitStatus(
		const addr::AddrRecPairs& caAddrRecPairs,
		const addr::AddrRecs& gpAddrRecs,
		const addr::AddrRecs& csAddrRecs,
		const addr::AddrRec& rec,
		addr::AddrRecs& existAddrRecs
	);

	/**
	 * @brief 20���Z���R�[�h�Ƃ��Ă̔���
	 * @param gpAddrRecs [in] ���|�C���g�f�[�^
	 * @param csAddrRecs [in] CS�|�C���g�f�[�^
	 * @param rec [in] ����Ώۃf�[�^
	 * @retval true CS�A���|�C���g�ƈ�v����
	 * @retval false CS�A���|�C���g�ƈ�v���Ȃ�
	 */
	bool decideGouDigitStatus(
		const addr::AddrRecs& gpAddrRecs,
		const addr::AddrRecs& csAddrRecs,
		const addr::AddrRec& rec
	);

	/**
	 * @brief 20���Z���R�[�h�Ƃ��Ă̔���(����true�̃��R�[�h��Ԃ���)
	 * @param gpAddrRecs [in] ���|�C���g�f�[�^
	 * @param csAddrRecs [in] CS�|�C���g�f�[�^
	 * @param rec [in] ����Ώۃf�[�^
	 * @param existAddrRecs [out] ���肪true�ɂȂ������ACS���R�[�h
	 * @retval true CS�A���|�C���g�ƈ�v����
	 * @retval false CS�A���|�C���g�ƈ�v���Ȃ�
	 */
	bool decideGouDigitStatus(
		const addr::AddrRecs& gpAddrRecs,
		const addr::AddrRecs& csAddrRecs,
		const addr::AddrRec& rec,
		addr::AddrRecs& existAddrRecs
	);

	/**
	 * @brief CS�f�[�^���ǂ�������
	 * @param addrRecs [in] CS�|�C���g�f�[�^
	 * @param rec [in] ����Ώۃf�[�^
	 * @retval true CS�|�C���g�ł���
	 * @retval false CS�|�C���g�ł͂Ȃ�
	 */
	bool judgeSameAddrRecByCs(
		const addr::AddrRecs& addrRecs,
		const addr::AddrRec& rec
	);
	
	/**
	 * @brief CS�f�[�^���ǂ�������(����true�̃��R�[�h��Ԃ���)
	 * @param addrRecs [in] CS�|�C���g�f�[�^
	 * @param rec [in] ����Ώۃf�[�^
	 * @param existAddrRecs [out] ���肪true�ɂȂ���CS���R�[�h
	 * @retval true CS�|�C���g�ł���
	 * @retval false CS�|�C���g�ł͂Ȃ�
	 */
	bool judgeSameAddrRecByCs(
		const addr::AddrRecs& addrRecs,
		const addr::AddrRec& rec,
		addr::AddrRecs& existAddrRecs
	);

private:
	addr::AddrRecHelper m_addrRecHelper; //!< AddrRecHelper�N���X
};

