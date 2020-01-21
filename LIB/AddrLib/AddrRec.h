#pragma once

#include "stdafx.h"

#include <TDC/useful_headers/str_util.h>

namespace addr
{
/**
 * @brief �Z���R�[�h����
 */
enum AddrDigit
{
	addr   = 11, // 11��
	chiban = 16, // 16��
	gou    = 20, // 20��
	Other  = 99  // ����ȊO
};

/**
 * @brief �����t���Z���R�[�h���ǂ����𔻒�
 * @param[in] code ����Ώۂ̏Z�����R�[�h
 * @retval true �����t���Z���R�[�h�ł���
 * @retval false �����t���Z���R�[�h�ł͂Ȃ�
 */
bool judgeAddrChar(const CString& code);

/**
 * @brief �Z�����R�[�h�R���e�i
 */
class AddrRec
{
public:

	/**
	 * @brief �R���X�g���N�^
	 * @param[in] jusyocode  �Z���R�[�h
	 * @param[in] geo        �`��
	 */
	AddrRec(const CString& jusyocode, const IGeometryPtr& geo);

	/**
	 * @brief �A�N�Z�T�isetter�F�n�Ԗ��́{�����́j
	 * @param[in] chibanName �n�Ԗ���
	 * @param[in] gouName    ������
	 */
	void setAddrNames(const CString& chibanName, const CString& gouName)
	{
		setChibanName(chibanName);
		setGouName(gouName);
	}

	/**
	 * @brief �A�N�Z�T�isetter�F�n�Ԗ��́j
	 * @param[in] chibanName �n�Ԗ���
	 */
	void setChibanName(const CString& chibanName)
	{
		m_chibanName = chibanName;
	}

	/**
	 * @brief �A�N�Z�T�isetter�F�����́j
	 * @param[in] chibanName ������
	 */
	void setGouName(const CString& gouName)
	{
		m_gouName = gouName;
	}

	/**
	 * @brief �A�N�Z�T�isetter�F��v��r���A�ΏۂƂ��邩�ǂ����j
	 * @param[in] res ��v��r���̔�r�Ώۉ�
	 */
	void setIsMatchTarget(bool res)
	{
		m_isMatchTarget = res;
	}

	/**
	 * @brief �A�N�Z�T�igetter�F�Z���R�[�h�j
	 * @retval �Z���R�[�h
	 */
	const CString getJusyocode() const { return m_citycode + m_addrcode + m_chibancode + m_goucode; }

	/**
	 * @brief �A�N�Z�T�igetter�F�Z���R�[�h�i11���j�j
	 * @retval �Z���R�[�h�i11���j
	 */
	const CString getAdrcode11() const { return m_citycode + m_addrcode; }

	/**
	 * @brief �A�N�Z�T�igetter�F�s�撬���R�[�h�j
	 * @retval �s�撬���R�[�h
	 */
	const CString& getCitycode() const { return m_citycode; }

	/**
	 * @brief �A�N�Z�T�igetter�F�厚�E���R�[�h�j
	 * @retval �厚�E���R�[�h
	 */
	const CString& getAddrcode() const { return m_addrcode; }

	/**
	 * @brief �A�N�Z�T�igetter�F�n�ԃR�[�h�j
	 * @retval �n�ԃR�[�h
	 */
	const CString& getChibancode() const { return m_chibancode; }

	/**
	 * @brief �A�N�Z�T�igetter�F���R�[�h�j
	 * @retval ���R�[�h
	 */
	const CString& getGoucode() const { return m_goucode; }

	/**
	 * @brief �A�N�Z�T�igetter�F�n�Ԗ��́j
	 * @retval �n�Ԗ���
	 */
	const CString& getChibanName() const { return m_chibanName; }

	/**
	 * @brief �A�N�Z�T�igetter�F�����́j
	 * @retval ������
	 */
	const CString& getGouName() const { return m_gouName; }

	/**
	 * @brief �A�N�Z�T�igetter�F�`��j
	 * @retval �`��
	 */
	const IGeometryPtr& getGeometry() const { return m_geo; }

	/**
	 * @brief �����t���n�Ԃ��ǂ���
	 * @retval true �����t���n�Ԃł���
	 * @retval false �����t���n�Ԃł͂Ȃ�
	 */
	bool isChibanChar() const { return m_isChibanChar; }

	/**
	 * @brief �����t�������ǂ���
	 * @retval true �����t�����ł���
	 * @retval false �����t�����ł͂Ȃ�
	 */
	bool isGouChar() const { return m_isGouChar; }

	/**
	 * @brief ��v��r���A�ΏۂƂ��邩�ǂ���
	 * @retval true �ΏۂƂ���
	 * @retval false �ΏۂƂ��Ȃ�
	 */
	bool isMatchTarget() const { return m_isMatchTarget; }

	/**
	 * @brief �Z���R�[�h�̊K�w���x�����擾
	 * @retval �Z���K�w���x��
	 */
	 AddrDigit getAddrDigit() const { return m_addrDigit; }

	bool operator ==(const AddrRec& rec) const
	{
		// �Z���R�[�h�i11���j�ƒn�Ԗ��́A�����̂���v����ꍇ�A�����Ƃ݂Ȃ�
		return	getAdrcode11() ==rec.getAdrcode11() &&
				getChibanName() == rec.getChibanName() &&
				getGouName() == rec.getGouName();
	}
	bool operator <(const AddrRec& rec) const
	{
		// �Z���R�[�h�i11���j�ƒn�Ԗ��́A�����̂���v����ꍇ�A�����Ƃ݂Ȃ�
		return	getJusyocode() < rec.getJusyocode();
	}

private:
	CString m_citycode;    //!< �s�撬���R�[�h
	CString m_addrcode;    //!< �厚�E���R�[�h
	CString m_chibancode;  //!< �n�ԃR�[�h
	CString m_goucode;     //!< ���R�[�h
	CString m_chibanName;  //!< �n�Ԗ���
	CString m_gouName;     //!< ������
	IGeometryPtr m_geo;    //!< �`��
	AddrDigit m_addrDigit; //!< �Z���K�w���x��
	bool m_isChibanChar;   //!< �����t���n�Ԃ��ǂ���
	bool m_isGouChar;      //!< �����t�������ǂ���
	bool m_isMatchTarget;  //!< ��v��r���A�ΏۂƂ��邩�ǂ���
};

/**
 * @brief �Z�����R�[�h�Z�b�g
 */
typedef std::vector<AddrRec> AddrRecs;

} 	// namespace addr