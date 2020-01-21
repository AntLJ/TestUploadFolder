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
 * @brief イベントの受信環境を管理するクラス。
 *
 * コンストラクタでイベントの受信を開始し、デストラクタで終了する。<br>
 * ただしコンストラクタはprivateなので、オブジェクトの生成には advise() を使う。<br>
 * イベントの受信を一時的に停止させたいときは suspender を使う。
 */
template <const IID* piid>
class event_adviser : boost::noncopyable
{
public:
// 定義
	typedef boost::shared_ptr<event_adviser> ref;

	/**
	 * @brief イベントの受信を一時停止させる。
	 *
	 * オブジェクトが生存するあいだ、イベントの受信は停止される。
	 */
	class suspender : boost::noncopyable
	{
	public:
	// コンストラクタとデストラクタ
		suspender(ref& pAdviser) :
		m_pAdviser(pAdviser)
		{
			if(m_pAdviser) {
				if(m_pAdviser->m_dwEventCookie != 0xFEFEFEFE) {
					m_pAdviser->unadvise();
				}
				else {
					// もともとイベントに接続されていなかったなら、接続し直す必要もない。
					m_pAdviser.reset();
				}
			}
		}

		~suspender()
		{
			if(! m_pAdviser)
				return;

			// デストラクタからは例外を搬出しないこと。
			try {
				m_pAdviser->advise();
			}
			catch(...) {}
		}

	private:
	// 変数
		ref m_pAdviser;
	};

// デストラクタ
	/**
	 * @brief unadvise() する。
	 */ 
	~event_adviser()
	{
		unadvise();
	}

// ファクトリ
	/**
	 * @brief イベント受信環境管理オブジェクトを生成する。
	 *
	 * @param ipUnkCP [in] イベントサーバ。
	 * @param ipUnk [in] クライアント。
	 * @return イベント受信環境管理オブジェクト。
	 */
	static ref advise(IUnknown* ipUnkCP, IUnknown* ipUnk)
	{
		return ref(new event_adviser(ipUnkCP, ipUnk));
	}

private:
	friend class suspender;

// コンストラクタ
	/**
	 * @brief advise() する。
	 *
	 * @param ipUnkCP [in] イベントサーバ。
	 * @param ipUnk [in] クライアント。
	 */
	event_adviser(IUnknown* ipUnkCP, IUnknown* ipUnk) :
	m_dwEventCookie(0xFEFEFEFE),
	m_ipUnkCP(ipUnkCP),
	m_ipUnk(ipUnk)
	{
		advise();
	}

// 処理
	/**
	 * @brief イベントサーバに接続する。
	 */
	void advise()
	{
		atl2::valid(AtlAdvise(m_ipUnkCP, m_ipUnk, *piid, &m_dwEventCookie));
	}

	/**
	 * @brief イベントサーバへの接続を解除する。
	 */ 
	void unadvise()
	{
		if(m_ipUnkCP != 0 && m_dwEventCookie != 0xFEFEFEFE) {
			AtlUnadvise(m_ipUnkCP, *piid, m_dwEventCookie);
		}
		m_dwEventCookie = 0xFEFEFEFE;
	}

// 変数
	DWORD m_dwEventCookie; ///< イベントクッキー。
	IUnknownPtr m_ipUnkCP; ///< イベントサーバ。
	IUnknownPtr m_ipUnk; ///< クライアント。 suspender 用。
};

} // namespace atl2

#endif // ATL2_EVENT_ADVISER_H_
