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

/**
 * @file file_sharing_reader.h
 * @brief ファイル共用読み込みライブラリ。
 *
 * 他のプロセスと読み込み位置を共用するファイル操作ライブラリ。
 *
 * @include file_sharing_reader_sample.cpp
 */

#ifndef FILE_SHARING_READER_H_
#define FILE_SHARING_READER_H_

#include <crtdbg.h>
#include <io.h>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <sys/types.h>
#include <sys/stat.h>
#include <windows.h>
#include <shlwapi.h>
#include <tchar.h>
#include <atlbase.h>
#include <atlconv.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#pragma warning( push )
#pragma warning( disable : 4284 ) // infix 形式の適用に関する警告を抑制。
#include <boost/iterator/iterator_facade.hpp>
#pragma warning( pop )
#include <boost/utility.hpp>

namespace rns {

#if _MSC_VER <= 1200 && defined(__SGI_STL_PORT)
using std::basic_istream;
using std::basic_ifstream;
using std::basic_filebuf;
using std::char_traits;
#endif // _MSC_VER <= 1200 && defined(__SGI_STL_PORT)

/**
 * @brief ファイル読み込みクラス基底クラス
 *
 * @ref basic_file_sharing_reader "ファイル共用読み込み"と@ref basic_simple_file_reader "通常の読み込み"の切り替えを支援するためのもの。
 */
template <typename CharType, typename Traits>
class basic_file_reader
{
public:
// 型定義

	/// 入力ストリームの型。
#if _MSC_VER <= 1200 && defined(__SGI_STL_PORT)
	typedef basic_istream<CharType, Traits> stream_type;
#else
	typedef std::basic_istream<CharType, Traits> stream_type;
#endif // _MSC_VER <= 1200 && defined(__SGI_STL_PORT)

	/// 入力ストリームのスマートポインタ型。
	typedef boost::shared_ptr<stream_type> stream_ref;

// デストラクタ
	virtual ~basic_file_reader() throw() {}

// 純粋仮想関数
	/**
	 * @brief ファイルを開いていたら閉じる。
	 *
	 * ! is_open() のときには何もしない。
	 *
	 * @pre lock() の戻り値が、コピーされているものも含めて全て破壊されている。
	 * @post ! is_open()
	 */
	virtual void close() = 0;

	/**
	 * @brief 今現在、ファイルを開いているか。
	 *
	 * @retval true 開いている。
	 * @retval false 開いていない。
	 */
	virtual bool is_open() const = 0;

	/**
	 * @brief ファイル読み込み処理の開始を宣言する。
	 *
	 * 読み込み処理が完了した時点で、戻り値は速やかに破棄するべきである。
	 *
	 * @return 読み込み処理用にロックされたファイルストリーム。
	 * @note このメソッドは、! is_open() の時にも正しく動作しなければならない。
	 */
	virtual stream_ref lock() = 0;
};

/**
 * @brief 単純ファイル読み込みクラス
 *
 * 何の工夫もなくファイルを読み込むクラス。<br>
 * インスタンスが破壊されるか、 close() されるまで、開いたファイルはロックされる。
 *
 * @test SimpleFileReaderTest
 */
template <typename CharType, typename Traits>
class basic_simple_file_reader : public basic_file_reader<CharType, Traits>, boost::noncopyable
{
public:
// コンストラクタとデストラクタ
	basic_simple_file_reader() :
	m_pStream(new base_stream_type)
	{
	}

	/**
	 * @brief ファイルを開いた状態でインスタンスを構築する（マルチバイト文字列のパス）。
	 *
	 * 渡された引数をそのまま open() に渡す。
	 * @sa open()
	 *
	 * @param lpszFilePath [in] ファイル名。
	 * @param nMode [in] ファイル操作モード。
	 */
	explicit basic_simple_file_reader(const char* lpszFilePath, int nMode = std::ios_base::in) :
	m_pStream(new base_stream_type)
	{
		open(lpszFilePath, nMode);
	}

	/**
	 * @brief ファイルを開いた状態でインスタンスを構築する（ワイド文字列のパス）。
	 *
	 * 渡された引数をそのまま open() に渡す。
	 * @sa open()
	 *
	 * @param lpszFilePath [in] ファイル名。
	 * @param nMode [in] ファイル操作モード。
	 */
	explicit basic_simple_file_reader(const wchar_t* lpszFilePath, int nMode = std::ios_base::in) :
	m_pStream(new base_stream_type)
	{
		open(lpszFilePath, nMode);
	}

	/**
	 * @brief ファイルを開いていたら閉じる。
	 *
	 * 中で close() を呼ぶ。<br>
	 * 発生した例外は揉み消される。
	 * @sa close()
	 */
	virtual ~basic_simple_file_reader() throw()
	{
		try {
			close();
		}
		catch(...) {}
	}

// 入出力
	/**
	 * @brief ファイルを開く（マルチバイト文字列のパス）。
	 *
	 * @param lpszFilePath [in] ファイル名。
	 * @param nMode [in] ファイル操作モード。
	 * @pre lock() の戻り値が、コピーされたものも含めて全て破壊されている。
	 */
	void open(const char* lpszFilePath, int nMode = std::ios_base::in)
	{
		// 共用ファイルを開く。
		m_pStream->open(
#if _MSC_VER == 1400
			CA2WEX<MAX_PATH>(lpszFilePath),
#else
			lpszFilePath, // VC8でマルチバイト文字列のパスを渡すのは危険（Wiki参照）
#endif // _MSC_VER == 1400
			nMode
		);
	}

	/**
	 * @brief ファイルを開く（ワイド文字列のパス）。
	 *
	 * @param lpszFilePath [in] ファイル名。
	 * @param nMode [in] ファイル操作モード。
	 * @pre lock() の戻り値が、コピーされたものも含めて全て破壊されている。
	 */
	void open(const wchar_t* lpszFilePath, int nMode = std::ios_base::in)
	{
		m_pStream->open(lpszFilePath, nMode);
	}

	virtual void close()
	{
		// 共用ファイルを閉じる。
		m_pStream->close();
	}

// 取得
	virtual bool is_open() const
	{
		return m_pStream->is_open();
	}

// 操作
	/**
	 * 特殊な操作は何もせず、ファイル入力ストリームをそのまま返す。
	 */
	virtual stream_ref lock()
	{
		return m_pStream;
	}

private:
// 型定義
#if _MSC_VER <= 1200 && defined(__SGI_STL_PORT)
	typedef basic_ifstream<CharType, Traits> base_stream_type;
#else
	typedef std::basic_ifstream<CharType, Traits> base_stream_type;
#endif // _MSC_VER <= 1200 && defined(__SGI_STL_PORT)

// 変数
	boost::shared_ptr<base_stream_type> m_pStream; ///< ファイル入力ストリーム。
};

/**
 * @brief ファイル共用読み込みクラス
 *
 * @ref file_sharing_info_fileを用いて、他のプロセスとファイルの読み込み位置を共用するクラス。<br>
 * インスタンスが破壊されるか、 close() されるまで、開いた共用ファイルはロックされる。<br>
 * lock() の戻り値が破壊されるまで、@ref file_sharing_info_fileはロックされる。
 *
 * @test FileSharingReaderTest
 * @test FileSharingReaderNetworkTest
 */
template <typename CharType, typename Traits>
class basic_file_sharing_reader : public basic_file_reader<CharType, Traits>, boost::noncopyable
{
public:
	/**
	 * @brief @ref file_sharing_info_fileに関する例外全般の基底となるクラス。
	 */
	class sharing_info_error : public std::runtime_error
	{
	public:
		sharing_info_error(const std::string& rMessage) :
		std::runtime_error(rMessage)
		{
		}
	};

	/**
	 * @brief @ref file_sharing_info_fileの作成に失敗した。
	 *
	 * パスが無効か、アクセス権がない。
	 */
	class cant_create_sharing_info_file : public sharing_info_error
	{
	public:
		cant_create_sharing_info_file(const char* lpszSharingInfoFilePath) :
		sharing_info_error(make_message(lpszSharingInfoFilePath))
		{
		}

	private:
		static std::string make_message(const char* lpszSharingInfoFilePath)
		{
			std::ostringstream oss;
			oss << "ファイル共用情報ファイル \"" << lpszSharingInfoFilePath << "\" の作成に失敗しました。";
			return oss.str();
		}
	};

	/**
	 * @brief @ref file_sharing_info_fileが存在しない。
	 */
	class not_existing_sharing_info_file : public sharing_info_error
	{
	public:
		not_existing_sharing_info_file(const char* lpszSharingInfoFilePath) :
		sharing_info_error(make_message(lpszSharingInfoFilePath))
		{
		}

	private:
		static std::string make_message(const char* lpszSharingInfoFilePath)
		{
			std::ostringstream oss;
			oss << "ファイル共用情報ファイル \"" << lpszSharingInfoFilePath << "\" が存在しません。";
			return oss.str();
		}
	};

	/**
	 * @brief @ref file_sharing_info_fileではないものを、@ref file_sharing_info_fileとして指定した。
	 */
	class illegal_sharing_info_file : public sharing_info_error
	{
	public:
		illegal_sharing_info_file(const char* lpszSharingInfoFilePath) :
		sharing_info_error(make_message(lpszSharingInfoFilePath))
		{
		}

	private:
		static std::string make_message(const char* lpszSharingInfoFilePath)
		{
			std::ostringstream oss;
			oss << "\"" << lpszSharingInfoFilePath << "\" は、正しいファイル共用情報ファイルではありません。";
			return oss.str();
		}
	};

	/**
	 * @brief 互換性がない@ref file_sharing_info_fileが指定された。
	 */
	class not_compatible_sharing_info_file : public sharing_info_error
	{
	public:
		not_compatible_sharing_info_file(const char* lpszSharingInfoFilePath) :
		sharing_info_error(make_message(lpszSharingInfoFilePath))
		{
		}

	private:
		static std::string make_message(const char* lpszSharingInfoFilePath)
		{
			std::ostringstream oss;
			oss << "ファイル共用情報ファイル \"" << lpszSharingInfoFilePath << "\" のバージョンには非対応です。";
			return oss.str();
		}
	};

	/**
	 * @brief 共用情報が一致しない。
	 */
	class not_match_sharing_info : public sharing_info_error
	{
	public:
		not_match_sharing_info(const char* lpszFilePath, const char* lpszSharingInfoFilePath) :
		sharing_info_error(make_message(lpszFilePath, lpszSharingInfoFilePath))
		{
		}

	private:
		static std::string make_message(const char* lpszFilePath, const char* lpszSharingInfoFilePath)
		{
			std::ostringstream oss;
			oss << "\"" << lpszSharingInfoFilePath << "\" は \"" << lpszFilePath << "\" の共用情報ファイルではありません。";
			return oss.str();
		}
	};

	/**
	 * @brief @ref file_sharing_info_fileが示すファイルが存在しない。
	 */
	class not_existing_sharing_file : public sharing_info_error
	{
	public:
		not_existing_sharing_file(const char* lpszSharingInfoFilePath) :
		sharing_info_error(make_message(lpszSharingInfoFilePath))
		{
		}

	private:
		static std::string make_message(const char* lpszSharingInfoFilePath)
		{
			std::ostringstream oss;
			oss << "ファイル共用情報ファイル \"" << lpszSharingInfoFilePath << "\" が示すファイルは共用できません。";
			return oss.str();
		}
	};

// コンストラクタとデストラクタ
	/**
	 * @post ! is_open() && use_count() == 0
	 */
	basic_file_sharing_reader()
	{
	}

	/**
	 * @brief 共用ファイルを開いた状態でインスタンスを構築する。
	 *
	 * 渡された引数をそのまま open() に渡す。
	 * @sa open()
	 *
	 * @param lpszFilePath [in,nullable] 共用ファイルパス。
	 * @param lpszSharingInfoFilePath [in,nullable,optional] @ref file_sharing_info_fileパス。
	 * @param nMode [in,optional] ファイル操作モード。
	 * @exception sharing_info_error @ref file_sharing_info_fileが無効。
	 */
	explicit basic_file_sharing_reader(
		const char* lpszFilePath,
		const char* lpszSharingInfoFilePath = 0,
		int nMode = std::ios_base::in)
	{
		open(lpszFilePath, lpszSharingInfoFilePath, nMode);
	}
	
	/**
	 * @brief 共用ファイルを開いていたら閉じる。
	 *
	 * 中で close() を呼ぶ。<br>
	 * 発生した例外は揉み消される。
	 * @sa close()
	 *
	 * @pre m_pLockedStream.expired()
	 */
	virtual ~basic_file_sharing_reader() throw()
	{
		try {
			close();
		}
		catch(...) {}
	}

// 入出力
	/**
	 * @brief 共用ファイルを開く。
	 *
	 * @param lpszFilePath [in,nullable] 共用ファイルパス。nullを指定した場合、@ref file_sharing_info_fileから取り出したものを使用する。
	 * @param lpszSharingInfoFilePath [in,nullable,optional] @ref file_sharing_info_fileパス。nullを指定した場合、共用ファイル名の後ろに ".shr" を付けたものを使用する。
	 * @param nMode [in,optional] ファイル操作モード。
	 * @pre m_pLockedStream.expired() && (lpszFilePath || lpszSharingInfoFilePath)
	 * @exception sharing_info_error @ref file_sharing_info_fileが無効。
	 */
	void open(const char* lpszFilePath, const char* lpszSharingInfoFilePath = 0, int nMode = std::ios_base::in)
	{
		_ASSERT(lpszFilePath || lpszSharingInfoFilePath);

		// まずは閉じる。
		close();

		// 共用情報ファイル名を設定。
		if(lpszSharingInfoFilePath) {
			m_strSharingInfoFilePath = lpszSharingInfoFilePath;
		}
		else {
			m_strSharingInfoFilePath = lpszFilePath;
			m_strSharingInfoFilePath += ".shr";
		}

		// ファイル共用数をインクリメントして保存する。
		sharing_info_file aSharingInfoFile(m_strSharingInfoFilePath, lpszFilePath);
		aSharingInfoFile.retain();
		aSharingInfoFile.write();

		// 共用ファイルを開く。
		m_SharingStream.open(
#if _MSC_VER == 1400
			CA2WEX<MAX_PATH>(aSharingInfoFile.path().c_str()), // VC8でマルチバイト文字列のパスを渡すのは危険（Wiki参照）
#else
			aSharingInfoFile.path().c_str(),
#endif // _MSC_VER == 1400
			nMode
		);
	}

	virtual void close()
	{
		_ASSERT(m_pLockedStream.expired());

		if(m_SharingStream.is_open()) {
			// 共用ファイルを閉じる。
			m_SharingStream.close();

			// ファイル共用数をデクリメントして保存する。
			sharing_info aSharingInfo(m_strSharingInfoFilePath);
			aSharingInfo.release();
			aSharingInfo.write();
		}
	}

// 取得
	/**
	 * @brief ファイルを共用しているのは自分一人か。
	 *
	 * @retval true use_count() == 1
	 * @retval false use_count() != 1
	 */
	bool unique() const
	{
		return use_count() == 1;
	}

	/**
	 * @brief 共用数を取得する。
	 */
	int use_count() const
	{
		if(m_strSharingInfoFilePath.empty())
			return 0;

		boost::shared_ptr<locked_ifstream> pLockedStream(m_pLockedStream.lock());
		return (pLockedStream ? pLockedStream->m_SharingInfo : sharing_info(m_strSharingInfoFilePath)).use_count();
	}

	virtual bool is_open() const
	{
		return m_SharingStream.is_open();
	}

// 操作
	/**
	 * @brief @ref file_sharing_info_fileをロックする。
	 *
	 * @return 自身とその浅いコピーが破壊されるまで、@ref file_sharing_info_fileをロックするファイル入力ストリーム。
	 * @post ! m_pLockedStream.expired()
	 */
	virtual stream_ref lock()
	{
		// 既にロック済みなら、新たにロックする必要はない。
		boost::shared_ptr<locked_ifstream> pLockedStream(m_pLockedStream.lock());

		if(! pLockedStream) {
			// 新規にロック。
			pLockedStream.reset(new locked_ifstream(m_SharingStream, m_strSharingInfoFilePath));
			m_pLockedStream = pLockedStream;
		}

		return pLockedStream;
	}

private:
// 型定義
	/// ファイル入力ストリームの型。
#if _MSC_VER <= 1200 && defined(__SGI_STL_PORT)
	typedef basic_ifstream<CharType, Traits> sharing_stream;
#else
	typedef std::basic_ifstream<CharType, Traits> sharing_stream;
#endif // _MSC_VER <= 1200 && defined(__SGI_STL_PORT)

	/**
	 * @ingroup file_sharing_info_file
	 * @brief ファイル共用情報クラス
	 *
	 * インスタンスが存在する間、@ref file_sharing_info_fileはロックされる。
	 */
	class sharing_info : boost::noncopyable
	{
	public:
	// コンストラクタ
		/**
		 * @brief @ref file_sharing_info_fileのヘッダ部分だけを開く。
		 *
		 * 開くことができるまで、10msの間隔を置いてアクセスし続ける。
		 *
		 * @param rSharingInfoFilePath [in] @ref file_sharing_info_fileパス。
		 * @param bCreatable [in,optional] @ref file_sharing_info_fileを新規に作成しても良いか。
		 * @pre ! rSharingInfoFilePath.empty()
		 * @post m_hFile != 0
		 * @exception cant_create_sharing_info_file @ref file_sharing_info_fileを作成できなかった。
		 * @exception not_existing_sharing_info_file @ref file_sharing_info_fileを開けなかった。
		 * @exception illegal_sharing_info_file @ref file_sharing_info_fileではないものが指定されている。
		 * @exception not_compatible_sharing_info_file @ref 互換性のない@ref file_sharing_info_fileが指定された。
		 */
		sharing_info(const std::string& rSharingInfoFilePath, bool bCreatable = false) :
		m_hFile(0)
		{
			_ASSERT(! rSharingInfoFilePath.empty());

			using namespace std;

			USES_CONVERSION;

			// ファイル共用情報ファイルのパスを_TCHAR配列化。
			const LPCTSTR lpszSharingInfoFilePathT = A2CT(rSharingInfoFilePath.c_str());

			// エラーが発生しない限り、とにかく開こうとする。
			while(1) {
				// ファイル共用情報ファイルへの排他アクセスを開始する。
				m_hFile =
					::CreateFile(
						lpszSharingInfoFilePathT,
						GENERIC_READ | GENERIC_WRITE, 
						0,
						NULL,
						bCreatable ? OPEN_ALWAYS : OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						NULL
					);

				if(m_hFile != INVALID_HANDLE_VALUE)
					break;

				// ファイルが存在しないと言うことは、排他制限に引っかかったわけではない。
				struct _stat buf;
				if(::_stat(rSharingInfoFilePath.c_str(), &buf) == -1 || buf.st_mode & _S_IFDIR) {
					if(bCreatable)
						throw cant_create_sharing_info_file(rSharingInfoFilePath.c_str());
					else
						throw not_existing_sharing_info_file(rSharingInfoFilePath.c_str());
				}

				// ちょっと待つ。
				::Sleep(10);
			}

			try {
				if(read()) {
					// 版番号を確認する。
					if(m_Data.m_nMajor > 1)
						throw not_compatible_sharing_info_file(rSharingInfoFilePath.c_str());
				}
				else {
					m_Data.m_nMajor = 1;
					m_Data.m_nMinor = 0;
					m_Data.m_nPos = 0;
					m_Data.m_nReferenceCount = 0;
					write();
				}
			}
			catch(const illegal_sharing_info_file_&) {
				::CloseHandle(m_hFile);
				m_hFile = 0;
				throw illegal_sharing_info_file(rSharingInfoFilePath.c_str());
			}
			catch(...) {
				::CloseHandle(m_hFile);
				m_hFile = 0;
				throw;
			}
		}

		/**
		 * @brief @ref file_sharing_info_fileのロックを解除する。
		 */
		virtual ~sharing_info() throw()
		{
			if(m_hFile) {
				::CloseHandle(m_hFile);
			}
		}

	// 取得／操作
		/**
		 * @brief 共用数をインクリメントする。
		 */
		void retain() throw()
		{
			++m_Data.m_nReferenceCount; 
		}

		/**
		 * @brief 共用数をデクリメントする。
		 */
		void release() throw()
		{
			--m_Data.m_nReferenceCount;
		}

		/**
		 * @brief 共用数を取得する。
		 */
		int use_count() const throw()
		{
			return m_Data.m_nReferenceCount;
		}

		/**
		 * @brief ファイルポジションを設定する。
		 *
		 * @param nPos [in] ファイルポジション。
		 */
		void pos(unsigned __int64 nPos) throw()
		{
			m_Data.m_nPos = nPos;
		}

		/**
		 * @brief ファイルポジションを取得する。
		 */
		unsigned __int64 pos() const throw()
		{
			return m_Data.m_nPos;
		}

	// 入出力
		/**
		 * @brief 共用情報をファイルに書き込む。
		 *
		 * @pre m_hFile != 0
		 */
		virtual void write() throw()
		{
			write_header();
		}

	protected:
	// 型定義
		class illegal_sharing_info_file_ : public std::exception {};

#pragma pack( push, 1 )
		/**
		 * @brief @ref file_sharing_info_file先頭16バイト
		 *
		 * 構造体の中身をそのままファイルに書き込むので、アライメントはきっちり定義しておく。
		 */
		struct data
		{
			unsigned char m_nMajor; ///< 共用ファイルメジャー版番号（上位互換性なし）
			unsigned char m_nMinor; ///< 共用ファイルマイナー版番号（上位互換性あり）
			unsigned __int64 m_nPos; ///< 共用ファイルポジション。
			int m_nReferenceCount; ///< 共用しているプロセスの数。
		};
#pragma pack( pop )

	// 変数
		HANDLE m_hFile; ///< @ref file_sharing_info_fileハンドル。

	private:
	// 入出力
		/**
		 * @brief @ref file_sharing_info_fileを読み込む。
		 *
		 * @retval true @ref file_sharing_info_fileに書き込まれている情報を読み込んだ。
		 * @retval false @ref file_sharing_info_fileには何も書き込まれていない。
		 * @pre m_hFile != 0
		 * @exception illegal_sharing_info_file_ @ref file_sharing_info_fileではない。
		 */
		bool read()
		{
			_ASSERT(m_hFile);

			// ファイルポインタを先頭へ移動。
			::SetFilePointer(m_hFile, 0, 0, FILE_BEGIN);

			// 先頭16バイトを読み込む。
			// 1バイトも読み込めなったら、これは新規に作成されたファイルである。
			DWORD dw;
			if(::ReadFile(m_hFile, &m_Data, sizeof(m_Data), &dw, 0) && dw == 0)
				return false;

			// 中途半端に読み込めたら、全く関係のないファイルである。
			if(dw < sizeof(m_Data))
				throw illegal_sharing_info_file_();

			return true;
		}

		/**
		 * @brief 先頭16バイトをファイルに書き出す。
		 *
		 * @pre m_hFile != 0
		 */
		void write_header() throw()
		{
			_ASSERT(m_hFile);

			::SetFilePointer(m_hFile, 0, 0, FILE_BEGIN);

			DWORD dw;
			::WriteFile(m_hFile, &m_Data, sizeof(m_Data), &dw, 0);
		}

	// 変数
		data m_Data; ///< ファイル共用情報。
	};

	/**
	 * @brief @ref file_sharing_info_fileをフルに開く。
	 *
	 * @param rSharingInfoFilePath [in] @ref file_sharing_info_fileパス。
	 * @param lpszFilePath [in,optional] 共用ファイルパス。これをnullにした場合、@ref file_sharing_info_fileに共用ファイルパスが記されていなければならない。 
	 * @pre ! rSharingInfoFilePath.empty()
	 * @post m_hFile != 0
	 * @exception not_match_sharing_info @ref file_sharing_info_fileと共用ファイルの関係に疑問がある。
	 * @exception not_existing_sharing_file @ref file_sharing_info_fileに記されているファイルが存在しない。
	 */
	class sharing_info_file : public sharing_info
	{
	public:
		sharing_info_file(const std::string& rSharingInfoFilePath, const char* lpszFilePath = 0) :
		sharing_info(rSharingInfoFilePath, lpszFilePath != 0)
		{
			// 絶対パスを用意する。
			char szSharingInfoFileAbsPath[_MAX_PATH] = { 0 };
			::_fullpath(szSharingInfoFileAbsPath, rSharingInfoFilePath.c_str(), _MAX_PATH);

			char szFileAbsPath[_MAX_PATH] = { 0 };
			if(lpszFilePath) {
				::_fullpath(szFileAbsPath, lpszFilePath, _MAX_PATH);
			}

			if(! read_path(szSharingInfoFileAbsPath)) {
				// 共用ファイルを読み込めなかった場合、明示的に指定されていなければならない。
				if(lpszFilePath == 0)
					throw illegal_sharing_info_file(rSharingInfoFilePath.c_str());

				m_strFilePath = szFileAbsPath;

				USES_CONVERSION;

				// ファイル共用情報ファイルからの相対パスを求める。
				TCHAR szRelPath[_MAX_PATH];
				const char* lpszRelPath = 
					::PathRelativePathTo(szRelPath, A2CT(szSharingInfoFileAbsPath), 0, A2CT(szFileAbsPath), 0)
					? T2CA(szRelPath)
					: szFileAbsPath;

				// パスを書き込む。
				DWORD dw;
				::WriteFile(m_hFile, lpszRelPath, ::strlen(lpszRelPath), &dw, 0);
			}
			else {
				if(lpszFilePath) {
					// 共用情報に記されているのとは別のファイルかもしれない。
					if(::_stricmp(m_strFilePath.c_str(), szFileAbsPath) != 0)
						throw not_match_sharing_info(lpszFilePath, rSharingInfoFilePath.c_str());
				}
				else {
					// 共用情報に記されているファイルが存在しない。
					if(::_access(m_strFilePath.c_str(), 0) == -1)
						throw not_existing_sharing_file(rSharingInfoFilePath.c_str());
				}
			}
		}

		/**
		 * @brief 共用ファイルパスを取得する。
		 */
		const std::string& path() const throw()
		{
			return m_strFilePath;
		}

	private:
	// 入出力
		/**
		 * @brief @ref file_sharing_info_fileを読み込む。
		 *
		 * @param lpszSharingInfoFileAbsPath [in] file_sharing_info_fileの絶対パス。
		 * @retval true 共用するファイルのパスを読み込んだ。
		 * @retval false 共用するファイルのパスは記されていない。
		 * @pre m_hFile != 0
		 * @post ::GetFilePointer(m_hFile, 0, 0, FILE_END) == 0
		 * @exception illegal_sharing_info_file @ref file_sharing_info_fileではない。
		 */
		bool read_path(const char* lpszSharingInfoFileAbsPath)
		{
			USES_CONVERSION;

			_ASSERT(! ::PathIsRelative(A2CT(lpszSharingInfoFileAbsPath)));
			_ASSERT(m_hFile);

			// 共用するファイルのパスを指す先頭のアドレスまで移動する。
			if(::SetFilePointer(m_hFile, sizeof(data), 0, FILE_BEGIN) != sizeof(data))
				throw illegal_sharing_info_file(lpszSharingInfoFileAbsPath);

			// ファイルサイズがヘッダサイズに等しかったら、共用するファイルのパスは記されていない。
			DWORD dw;
			const DWORD dwFileSize = ::GetFileSize(m_hFile, &dw);
			if(dwFileSize == sizeof(data))
				return false;

			// ファイルパスの長さを求める。
			const DWORD dwPathSize = dwFileSize - ::SetFilePointer(m_hFile, 0, 0, FILE_CURRENT);
			if(dwPathSize >= _MAX_PATH)
				throw illegal_sharing_info_file(lpszSharingInfoFileAbsPath);

			// ファイル共用情報ファイルからの相対パスを取り込む。
			char szFilePath[_MAX_PATH] = { 0 };
			if(! (::ReadFile(m_hFile, szFilePath, _MAX_PATH, &dw, 0) && dw > 0 && dw < _MAX_PATH))
				throw illegal_sharing_info_file(lpszSharingInfoFileAbsPath);

			// ファイル共用情報ファイルのディレクトリパスを求める。
			TCHAR szDirPathT[_MAX_PATH + 1] = { 0 };
			::_tcsncpy(szDirPathT, A2CT(lpszSharingInfoFileAbsPath), _MAX_PATH);
			::PathRemoveFileSpec(szDirPathT);

			// ファイル共用情報ファイルからの相対パスを、絶対パスに書き直す。
			TCHAR szPathT[_MAX_PATH + 1] = { 0 };
			if(! ::PathCombine(szPathT, szDirPathT, A2CT(szFilePath))) {
				::_tcscpy(szPathT, A2CT(szFilePath));
			}

			m_strFilePath = T2CA(szPathT);

			return true;
		}

	// 変数
		std::string m_strFilePath; ///< 共用ファイルパス。
	};

	/**
	 * @brief 共用情報をロックしてファイルを読み込むストリームクラス
	 */
#if _MSC_VER <= 1200 && defined(__SGI_STL_PORT)
	class locked_ifstream : public basic_istream<CharType, Traits>
	{
		typedef basic_istream<CharType, Traits> super;
#else
	class locked_ifstream : public std::basic_istream<CharType, Traits>, boost::noncopyable
	{
		typedef std::basic_istream<CharType, Traits> super;
#endif // _MSC_VER <= 1200 && defined(__SGI_STL_PORT)
	public:
	// コンストラクタとデストラクタ
		locked_ifstream(stream_type& rStream, const std::string& rSharingInfoFilePath) :
		super(rStream.rdbuf()),
		m_rStream(rStream),
		m_SharingInfo(rSharingInfoFilePath)
		{
#pragma warning( push )
#pragma warning( disable :  4244 )
			m_rStream.seekg(static_cast<sharing_stream::pos_type>(m_SharingInfo.pos()));
#pragma warning( pop )
		}

		virtual ~locked_ifstream() throw()
		{
			m_SharingInfo.pos(m_rStream.tellg());
			m_SharingInfo.write();
		}

	// 変数
		stream_type& m_rStream; ///< 共用ファイル入力ストリーム。
		sharing_info m_SharingInfo; ///< ファイル共用情報。
	};

// 変数
	sharing_stream m_SharingStream; ///< 共用ファイル入力ストリーム。
	std::string m_strSharingInfoFilePath; ///< @ref file_sharing_info_file名。

	boost::weak_ptr<locked_ifstream> m_pLockedStream; ///< 共用ファイルロック。
};

/**
 * @brief basic_file_reader 用の istream_iterator クラス
 *
 * インクリメント時に basic_file_reader を basic_file_reader::lock() して要素を取り出し、即座に basic_file_reader::lock() を解除する。
 */
#if _MSC_VER <= 1200 && defined(__SGI_STL_PORT)
template <typename ValueType, typename CharType = char, typename Traits = char_traits<CharType> >
#else
template <typename ValueType, typename CharType = char, typename Traits = std::char_traits<CharType> >
#endif // _MSC_VER <= 1200 && defined(__SGI_STL_PORT)
class file_read_iterator :
	public boost::iterator_facade<
		file_read_iterator<ValueType, CharType, Traits>,
		const ValueType,
		boost::single_pass_traversal_tag
	>
{
public:
// 型定義
	typedef basic_file_reader<CharType, Traits> file_reader_type; ///< ファイル読み込みクラス型。

// コンストラクタ
	/**
	 * @brief 終端反復子として構築する。
	 */
	file_read_iterator() throw() :
	m_pFileReader(0)
	{
	}

	/**
	 * @brief 非終端反復子として構築する。
	 *
	 * @param rFileReader [in,ref] ファイル読み込みクラス。
	 */
	file_read_iterator(file_reader_type& rFileReader) :
	m_pFileReader(&rFileReader)
	{
		// 最初の要素を取り出す。
		increment();
	}

private:
// 反復子定義
	friend class boost::iterator_core_access;

	/**
	 * @brief 最後に取り出した要素を返す。
	 *
	 * @return 最後に取り出した要素。
	 * @pre *this != file_read_iterator()
	 */
	reference dereference() const throw()
	{
		return m_Value;
	}

	/**
	 * @brief 反復子が同じ状態にあるか調べる。
	 *
	 * @retval true 同じ状態にある。
	 * @retval false 同じ状態にはない。
	 */
	bool equal(const file_read_iterator& rhs) const throw()
	{
		return m_pFileReader == rhs.m_pFileReader;
	}

	/**
	 * @brief ファイルから次の要素を取り出す。
	 *
	 * 要素の取り出しに失敗すると、インスタンスは終端反復子になる。
	 *
	 * @pre *this != file_read_iterator()
	 */
	void increment()
	{
		_ASSERT(m_pFileReader);

		file_reader_type::stream_ref pStream(m_pFileReader->lock());
		if(! (*pStream >> m_Value)) {
			m_pFileReader = 0;
		}
	}

private:
// 変数
	file_reader_type* m_pFileReader; ///< 共用ファイル読み込みインスタンス。nullならば終端反復子である。
	ValueType m_Value; ///< 最後に取り出した要素。
};

#if _MSC_VER <= 1200 && defined(__SGI_STL_PORT)
typedef basic_file_reader<char, char_traits<char> > file_reader;
typedef basic_simple_file_reader<char, char_traits<char> > simple_file_reader;
typedef basic_file_sharing_reader<char, char_traits<char> > file_sharing_reader;
#else
typedef basic_file_reader<char, std::char_traits<char> > file_reader;
typedef basic_simple_file_reader<char, std::char_traits<char> > simple_file_reader;
typedef basic_file_sharing_reader<char, std::char_traits<char> > file_sharing_reader;
#endif // _MSC_VER <= 1200 && defined(__SGI_STL_PORT)

} // namespace rns

#pragma comment( lib, "shlwapi.lib" )

#endif // FILE_SHARING_READER_H_
