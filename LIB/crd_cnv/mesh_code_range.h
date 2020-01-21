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

#ifndef CRD_CNV_MESH_CODE_RANGE_H_
#define	CRD_CNV_MESH_CODE_RANGE_H_

#pragma warning( push )
#pragma warning( disable : 4284 )
#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/reverse_iterator.hpp>
#pragma warning( pop )
#include "crd_cnv.h"

//////////////////////////////////////////////////////////////////////
// mesh_code_range
//////////////////////////////////////////////////////////////////////

// ��`
//////////////////////////////////////////////////////////////////////

/**
 * @brief ����͈̔͂ɑ��݂��郁�b�V����񋓂���N���X�B
 *
 * @code
 * // �o�� : 533934 533935 533936 533944 533945 533946
 * std::copy(
 *	mesh_code_range(crd_cnv(), 533945, -1, -1, 1, 0).begin(),
 *	mesh_code_range::static_end(),
 *	std::ostream_iterator<int>(std::cout, "\n"));
 * @endcode
 */
class mesh_code_range
{
public:
// ��`
	/**
	 * @brief �����q�B
	 */
	class iterator : public boost::iterator_facade<iterator, const int, boost::bidirectional_traversal_tag, const int>
	{
	public:
	// �\�z/����
		iterator(const iterator& rIterator) :
		m_pGenerator(rIterator.m_pGenerator),
		m_nX(rIterator.m_nX),
		m_nY(rIterator.m_nY)
		{
		}

	private:
	// ��`
		friend class mesh_code_range;
		friend class boost::iterator_core_access;

	// �\�z/����
		iterator(const mesh_code_range* pGenerator, int nX, int nY) :
		m_pGenerator(pGenerator),
		m_nX(nX),
		m_nY(nY)
		{
		}

		const int dereference() const
		{
			return m_pGenerator->getMeshCode(m_nX, m_nY);
		}

		void increment()
		{
			m_nX++;
			if(m_nX == m_pGenerator->m_nXCount) {
				m_nX = 0;
				m_nY++;
			}
		}

		void decrement()
		{
			m_nX--;
			if(m_nX == -1) {
				m_nX = m_pGenerator->m_nXCount - 1;
				m_nY--;
			}
		}

		bool equal(iterator const& other) const
		{
			if(! m_pGenerator) {
				return other.m_pGenerator ? other.equal(*this) : true;
			}

			if(! other.m_pGenerator) {
				return 
					(other.m_nX == 0 && other.m_nY == 0) ?
					(m_nX == other.m_nX && m_nY == other.m_nY) :
					(m_nY == m_pGenerator->m_nYCount);
			}

			return m_pGenerator == other.m_pGenerator && m_nX == other.m_nX && m_nY == other.m_nY;
		}

	// �ϐ�
		const mesh_code_range* m_pGenerator;
		int m_nX;
		int m_nY;
	};

	typedef boost::reverse_iterator<iterator> reverse_iterator;

	typedef iterator const_iterator;
	typedef reverse_iterator const_reverse_iterator;

// �\�z/����
	mesh_code_range(const crd_cnv& rCrdCnv, int nOrgMeshCode, int nXCount, int nYCount);
	mesh_code_range(const crd_cnv& rCrdCnv, int nOrgMeshCode, int nYMin, int nYMax, int nXMin, int nXMax);

// �����q�̐���
	iterator begin() const { return iterator(this, 0, 0); }
	iterator end() const { return iterator(this, 0, m_nYCount); }

	reverse_iterator rbegin() const { return reverse_iterator(end()); }
	reverse_iterator rend() const { return reverse_iterator(begin()); }

	static iterator static_begin() { return iterator(0, 0, 0); }
	static iterator static_end() { return iterator(0, -1, -1); }

	static reverse_iterator static_rbegin() { return reverse_iterator(static_end()); }
	static reverse_iterator static_rend() { return reverse_iterator(static_begin()); }

private:
// ��`
	friend class iterator;

// �ݒ�
	void init(int nOrgMeshCode, int nXCount, int nYCount);

// �擾
	int getMeshCode(int nX, int nY) const;

// �ϐ�
	const crd_cnv& m_rCrdCnv; ///< ���W�ϊ��I�u�W�F�N�g�B
	int m_nOrgMeshCode; ///< �쐼�[���b�V���R�[�h�B
	int m_nXCount; ///< �������ɕ��ԃ��b�V�����B
	int m_nYCount; ///< �k�����ɕ��ԃ��b�V�����B
};

// ����
//////////////////////////////////////////////////////////////////////

/**
 * @brief �쐼�[�̃��b�V���R�[�h���w�肵�č\�z����B
 *
 * @param rCrdCnv [in,ref] ���W�ϊ��I�u�W�F�N�g�B
 * @param nOrgMeshCode [in] �쐼�[���b�V���R�[�h�B
 * @param nXCount [in] �������ɕ��ԃ��b�V�����B
 * @param nYCount [in] �k�����ɕ��ԃ��b�V�����B
 */
inline mesh_code_range::mesh_code_range(const crd_cnv& rCrdCnv, int nOrgMeshCode, int nXCount, int nYCount) :
m_rCrdCnv(rCrdCnv)
{
	if(nXCount == 0 || nYCount == 0) {
		nXCount = nYCount = 0;
	}
	init(nOrgMeshCode, nXCount, nYCount);
}

/**
 * @brief ����b�V���R�[�h�Ɣ͈͂��w�肵�č\�z����B
 *
 * @param rCrdCnv [in,ref] ���W�ϊ��I�u�W�F�N�g�B
 * @param nOrgMeshCode [in] �쐼�[���b�V���R�[�h�B
 * @param nXMin [in] ���������ŏ����΃��b�V���R�[�h�B
 * @param nYMin [in] ��k�����ŏ����΃��b�V���R�[�h�B
 * @param nXMax [in] ���������ő告�΃��b�V���R�[�h�B
 * @param nYMax [in] ��k�����ő告�΃��b�V���R�[�h�B
 */
inline mesh_code_range::mesh_code_range(const crd_cnv& rCrdCnv, int nOrgMeshCode, int nXMin, int nYMin, int nXMax, int nYMax) :
m_rCrdCnv(rCrdCnv)
{
	if(nXMin || nYMin) {
		nOrgMeshCode = const_cast<crd_cnv&>(m_rCrdCnv).GetMeshCode(nOrgMeshCode, nXMin, nYMin);
	}

	init(nOrgMeshCode, nXMax - nXMin + 1, nYMax - nYMin + 1);
}

/**
 * @brief �����o�ϐ��ɒl��ݒ肷��B
 *
 * @param nOrgMeshCode [in] �쐼�[���b�V���R�[�h�B
 * @param nXCount [in] �������ɕ��ԃ��b�V�����B
 * @param nYCount [in] �k�����ɕ��ԃ��b�V�����B
 */
inline void mesh_code_range::init(int nOrgMeshCode, int nXCount, int nYCount)
{
	m_nOrgMeshCode = nOrgMeshCode;
	m_nXCount = nXCount;
	m_nYCount = nYCount;
}

/**
 * @brief ���΃��b�V���R�[�h���΃��b�V���R�[�h�ɕϊ�����
 *
 * @param nX [in] �����������΃��b�V���R�[�h�B
 * @param nY [in] ��k�������΃��b�V���R�[�h�B
 */
inline int mesh_code_range::getMeshCode(int nX, int nY) const
{
	return const_cast<crd_cnv&>(m_rCrdCnv).GetMeshCode(m_nOrgMeshCode, nX, nY);
}

#endif // CRD_CNV_MESH_CODE_RANGE_H_
