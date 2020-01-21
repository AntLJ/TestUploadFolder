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

#ifndef ATL2_EVENT_ADVISER_H_
#define ATL2_EVENT_ADVISER_H_

#include <atl2/exception.h>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

namespace atl2 {

/**
 * @brief �C�x���g�̎�M�����Ǘ�����N���X�B
 *
 * �R���X�g���N�^�ŃC�x���g�̎�M���J�n���A�f�X�g���N�^�ŏI������B<br>
 * �������R���X�g���N�^��private�Ȃ̂ŁA�I�u�W�F�N�g�̐����ɂ� advise() ���g���B<br>
 * �C�x���g�̎�M���ꎞ�I�ɒ�~���������Ƃ��� suspender ���g���B
 */
template <const IID* piid>
class event_adviser : boost::noncopyable
{
public:
// ��`
	typedef boost::shared_ptr<event_adviser> ref;

	/**
	 * @brief �C�x���g�̎�M���ꎞ��~������B
	 *
	 * �I�u�W�F�N�g���������邠�����A�C�x���g�̎�M�͒�~�����B
	 */
	class suspender : boost::noncopyable
	{
	public:
	// �R���X�g���N�^�ƃf�X�g���N�^
		suspender(ref& pAdviser) :
		m_pAdviser(pAdviser)
		{
			if(m_pAdviser) {
				if(m_pAdviser->m_dwEventCookie != 0xFEFEFEFE) {
					m_pAdviser->unadvise();
				}
				else {
					// ���Ƃ��ƃC�x���g�ɐڑ�����Ă��Ȃ������Ȃ�A�ڑ��������K�v���Ȃ��B
					m_pAdviser.reset();
				}
			}
		}

		~suspender()
		{
			if(! m_pAdviser)
				return;

			// �f�X�g���N�^����͗�O����o���Ȃ����ƁB
			try {
				m_pAdviser->advise();
			}
			catch(...) {}
		}

	private:
	// �ϐ�
		ref m_pAdviser;
	};

// �f�X�g���N�^
	/**
	 * @brief unadvise() ����B
	 */ 
	~event_adviser()
	{
		unadvise();
	}

// �t�@�N�g��
	/**
	 * @brief �C�x���g��M���Ǘ��I�u�W�F�N�g�𐶐�����B
	 *
	 * @param ipUnkCP [in] �C�x���g�T�[�o�B
	 * @param ipUnk [in] �N���C�A���g�B
	 * @return �C�x���g��M���Ǘ��I�u�W�F�N�g�B
	 */
	static ref advise(IUnknown* ipUnkCP, IUnknown* ipUnk)
	{
		return ref(new event_adviser(ipUnkCP, ipUnk));
	}

private:
	friend class suspender;

// �R���X�g���N�^
	/**
	 * @brief advise() ����B
	 *
	 * @param ipUnkCP [in] �C�x���g�T�[�o�B
	 * @param ipUnk [in] �N���C�A���g�B
	 */
	event_adviser(IUnknown* ipUnkCP, IUnknown* ipUnk) :
	m_dwEventCookie(0xFEFEFEFE),
	m_ipUnkCP(ipUnkCP),
	m_ipUnk(ipUnk)
	{
		advise();
	}

// ����
	/**
	 * @brief �C�x���g�T�[�o�ɐڑ�����B
	 */
	void advise()
	{
		atl2::valid(AtlAdvise(m_ipUnkCP, m_ipUnk, *piid, &m_dwEventCookie));
	}

	/**
	 * @brief �C�x���g�T�[�o�ւ̐ڑ�����������B
	 */ 
	void unadvise()
	{
		if(m_ipUnkCP != 0 && m_dwEventCookie != 0xFEFEFEFE) {
			AtlUnadvise(m_ipUnkCP, *piid, m_dwEventCookie);
		}
		m_dwEventCookie = 0xFEFEFEFE;
	}

// �ϐ�
	DWORD m_dwEventCookie; ///< �C�x���g�N�b�L�[�B
	IUnknownPtr m_ipUnkCP; ///< �C�x���g�T�[�o�B
	IUnknownPtr m_ipUnk; ///< �N���C�A���g�B suspender �p�B
};

} // namespace atl2

#endif // ATL2_EVENT_ADVISER_H_
