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

// SquareHash.h: SquareHash �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SQUAREHASH_H__12D63737_77DD_498A_B314_29654B13C9C3__INCLUDED_)
#define AFX_SQUAREHASH_H__12D63737_77DD_498A_B314_29654B13C9C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <set>
#include <map>

/**
 * @brief ���ʃn�b�V���N���X����`
 */
interface SquareHashI
{
	/**
	 * @brief �n�b�V���_�N���X
	 */
	class HashPoint
	{
	public:
		HashPoint(int nX, int nY) :
		m_nX(nX),
		m_nY(nY)
		{
		}

		int x() const { return m_nX; }
		int y() const { return m_nY; }

		int m_nX;
		int m_nY;
	};

	typedef HashPoint(*HashFunc)(double, double);
};

/**
 * @brief ���ʃn�b�V���N���X�e���v���[�g
 */
template <typename T>
class SquareHashT : public SquareHashI
{
public:
// ��`
	typedef T Member;
	typedef std::set<T> MemberSet;

	typedef std::map<int, MemberSet> YLine; //< Y��
	typedef std::map<int, YLine> XYSquare; //< XY����

	typedef typename XYSquare::const_iterator const_iterator;

// �\�z/����
	SquareHashT(HashFunc pfHash) :
	m_pfHash(pfHash)
	{
	}

// �ݒ�/�擾
	/// �o�^����Ă���v�f���N���A����
	void clear()
	{
		m_cMemberMap.clear();
	}

	/**
	 * @brief �w�肵���I�u�W�F�N�g�̃R�s�[���A�w�肵���P�_�ɑ�����v�f�Ƃ��ēo�^����
	 *
	 * @param rMember [in] �v�f�Ƃ��ăR�s�[�o�^����I�u�W�F�N�g
	 * @param dX [in] �v�f��X���W
	 * @param dY [in] �v�f��Y���W
	 * @retval true �o�^����
	 * @retval false �o�^���s
	 */
	bool insert(T& rMember, double dX, double dY)
	{
		return insert(rMember, hash(dX, dY));
	}

	/**
	 * @brief �w�肵���I�u�W�F�N�g�̃R�s�[���A�w�肵���n�b�V�����ʍ��W�ɏ�������v�f�Ƃ��ēo�^����
	 *
	 * @param rMember [in] �v�f�Ƃ��ăR�s�[�o�^����I�u�W�F�N�g
	 * @param cPoint [in] �n�b�V�����ʍ��W
	 * @retval true �o�^����
	 * @retval false �o�^���s
	 */
	bool insert(T const& rMember, const HashPoint& cPoint)
	{
		return m_cMemberMap[cPoint.x()][cPoint.y()].insert(rMember).second;
	}

	/**
	 * @brief �w�肵���I�u�W�F�N�g�̃R�s�[���A�w�肵����`�ɑ�����v�f�Ƃ��ēo�^����
	 *
	 * @param rMember [in] �v�f�Ƃ��ăR�s�[�o�^����I�u�W�F�N�g
	 * @param dMinX [in] �ŏ�X���W
	 * @param dMinY [in] �ŏ�Y���W
	 * @param dMaxX [in] �ő�X���W
	 * @param dMaxY [in] �ő�Y���W
	 */
	void insert(T const& rMember, double dMinX, double dMinY, double dMaxX, double dMaxY)
	{
		insert(rMember, hash(dMinX, dMinY), hash(dMaxX, dMaxY));
	}

	/**
	 * @brief �w�肵���I�u�W�F�N�g�̃R�s�[���A�w�肵���n�b�V�����ʋ�`�ɏ�������v�f�Ƃ��ēo�^����
	 *
	 * @param rMember [in] �v�f�Ƃ��ăR�s�[�o�^����I�u�W�F�N�g
	 * @param cMinPoint [in] �ŏ��n�b�V�����ʍ��W
	 * @param cMaxPoint [in] �ő�n�b�V�����ʍ��W
	 */
	void insert(T const& rMember, const HashPoint& cMinPoint, const HashPoint& cMaxPoint)
	{
		for(int x = cMinPoint.x(); x <= cMaxPoint.x(); x++) {
			for(int y = cMinPoint.y(); y <= cMaxPoint.y(); y++) {
				m_cMemberMap[x][y].insert(rMember);
			}
		}
	}

	/// �擪�̗v�f���w�������q���擾����
	const_iterator begin() const { return m_cMemberMap.begin(); }

	/// �Ō���̎��̗v�f���w�������q���擾����
	const_iterator end() const { return m_cMemberMap.end(); }

	/**
	 * @brief �w�肵�����W���܂ރn�b�V�����ʂɓo�^���ꂽ�v�f�ꗗ���擾����
	 *
	 * @param dX [in] X���W
	 * @param dY [in] Y���W
	 * @return �v�f�ꗗ�B���̃n�b�V�����ʂɈ�x���v�f���o�^���ꂽ���Ƃ��Ȃ��ꍇNULL�B
	 */
	const MemberSet* searchMemberSet(double dX, double dY) const
	{
		return searchMemberSet(hash(dX, dY));
	}

	/**
	 * @brief �w�肵���n�b�V�����ʂɓo�^���ꂽ�v�f�ꗗ���擾����
	 *
	 * @param cPoint [in] �n�b�V�����ʍ��W
	 * @return �v�f�ꗗ�B���̃n�b�V�����ʂɈ�x���v�f���o�^���ꂽ���Ƃ��Ȃ��ꍇNULL�B
	 */
	const MemberSet* searchMemberSet(const HashPoint& cPoint) const
	{
		// Y�������߂�
		XYSquare::const_iterator it1 = m_cMemberMap.find(cPoint.x());
		if(it1 == m_cMemberMap.end())
			return 0;

		// �n�b�V�����ʂ����߂�
		YLine::const_iterator it2 = it1->second.find(cPoint.y());
		if(it2 == it1->second.end())
			return 0;

		// �n�b�V�����ʂɓo�^���ꂽ�v�f�ꗗ��Ԃ�
		return &it2->second;
	}

	/**
	 * @brief �w�肵����`�Əd�Ȃ�n�b�V�����ʂɓo�^���ꂽ�v�f�ꗗ���擾����
	 *
	 * @param dMinX [in] �ŏ�X���W
	 * @param dMinY [in] �ŏ�Y���W
	 * @param dMaxX [in] �ő�X���W
	 * @param dMaxY [in] �ő�Y���W
	 * @param rMemberSet [out] �v�f�ꗗ�擾��
	 */
	void getMemberSet(double dMinX, double dMinY, double dMaxX, double dMaxY, MemberSet& rMemberSet) const
	{
		getMemberSet(hash(dMinX, dMinY), hash(dMaxX, dMaxY), rMemberSet);
	}

	/**
	 * @brief �w�肵���n�b�V�����ʋ�`�ɓo�^���ꂽ�v�f�ꗗ���擾����
	 *
	 * @param cMinPoint [in] �ŏ��n�b�V�����ʍ��W
	 * @param dMaxPoint [in] �ő�n�b�V�����ʍ��W
	 * @param rMemberSet [out] �v�f�ꗗ�擾��
	 */
	void getMemberSet(const HashPoint& cMinPoint, const HashPoint& cMaxPoint, MemberSet& rMemberSet) const
	{
		for(int x = cMinPoint.x(); x <= cMaxPoint.x(); x++) {
			XYSquare::const_iterator it1 = m_cMemberMap.find(x);
			if(it1 == m_cMemberMap.end())
				continue;

			for(int y = cMinPoint.y(); y <= cMaxPoint.y(); y++) {
				YLine::const_iterator it2 = it1->second.find(y);
				if(it2 == it1->second.end())
					continue;

				for(MemberSet::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3) {
					rMemberSet.insert(*it3);
				}
			}
		}
	}

	/**
	 * @brief �S�v�f�ɑ΂��Ċ֐�f()��K�p����
	 *
	 * @param f [in] �֐��N���X
	 */
	template <typename Function>
	void for_each(Function& f)
	{
		for(const_iterator it1 = begin(); it1 != end(); ++it1) {
			const YLine& rYLine = it1->second;
			for(YLine::const_iterator it2 = rYLine.begin(); it2 != rYLine.end(); ++it2) {
				const MemberSet& rMemberSet = it2->second;
				for(MemberSet::const_iterator it3 = rMemberSet.begin(); it3 != rMemberSet.end(); ++it3) {
					f(*it3);
				}
			}
		}
	}

	/**
	 * @brief �w�肵�����W��������n�b�V�����ʍ��W���擾����
	 *
	 * @note �f�t�H���g�̏����́A�{�����x���W�̏����_�ȉ���؂�̂Ă邾��
	 * @return �n�b�V�����ʍ��W
	 */
	virtual HashPoint hash(double dX, double dY) const
	{
		return m_pfHash ? m_pfHash(dX, dY) : HashPoint(static_cast<int>(dX), static_cast<int>(dY));
	}

private:
	XYSquare m_cMemberMap; ///< �n�b�V�����ʍ��W�ʗv�f�ꗗ
	HashFunc m_pfHash; ///< �n�b�V���֐�
};

#endif // !defined(AFX_SQUAREHASH_H__12D63737_77DD_498A_B314_29654B13C9C3__INCLUDED_)
