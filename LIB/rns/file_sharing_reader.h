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
 * @brief �t�@�C�����p�ǂݍ��݃��C�u�����B
 *
 * ���̃v���Z�X�Ɠǂݍ��݈ʒu�����p����t�@�C�����색�C�u�����B
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
#pragma warning( disable : 4284 ) // infix �`���̓K�p�Ɋւ���x����}���B
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
 * @brief �t�@�C���ǂݍ��݃N���X���N���X
 *
 * @ref basic_file_sharing_reader "�t�@�C�����p�ǂݍ���"��@ref basic_simple_file_reader "�ʏ�̓ǂݍ���"�̐؂�ւ����x�����邽�߂̂��́B
 */
template <typename CharType, typename Traits>
class basic_file_reader
{
public:
// �^��`

	/// ���̓X�g���[���̌^�B
#if _MSC_VER <= 1200 && defined(__SGI_STL_PORT)
	typedef basic_istream<CharType, Traits> stream_type;
#else
	typedef std::basic_istream<CharType, Traits> stream_type;
#endif // _MSC_VER <= 1200 && defined(__SGI_STL_PORT)

	/// ���̓X�g���[���̃X�}�[�g�|�C���^�^�B
	typedef boost::shared_ptr<stream_type> stream_ref;

// �f�X�g���N�^
	virtual ~basic_file_reader() throw() {}

// �������z�֐�
	/**
	 * @brief �t�@�C�����J���Ă��������B
	 *
	 * ! is_open() �̂Ƃ��ɂ͉������Ȃ��B
	 *
	 * @pre lock() �̖߂�l���A�R�s�[����Ă�����̂��܂߂đS�Ĕj�󂳂�Ă���B
	 * @post ! is_open()
	 */
	virtual void close() = 0;

	/**
	 * @brief �����݁A�t�@�C�����J���Ă��邩�B
	 *
	 * @retval true �J���Ă���B
	 * @retval false �J���Ă��Ȃ��B
	 */
	virtual bool is_open() const = 0;

	/**
	 * @brief �t�@�C���ǂݍ��ݏ����̊J�n��錾����B
	 *
	 * �ǂݍ��ݏ����������������_�ŁA�߂�l�͑��₩�ɔj������ׂ��ł���B
	 *
	 * @return �ǂݍ��ݏ����p�Ƀ��b�N���ꂽ�t�@�C���X�g���[���B
	 * @note ���̃��\�b�h�́A! is_open() �̎��ɂ����������삵�Ȃ���΂Ȃ�Ȃ��B
	 */
	virtual stream_ref lock() = 0;
};

/**
 * @brief �P���t�@�C���ǂݍ��݃N���X
 *
 * ���̍H�v���Ȃ��t�@�C����ǂݍ��ރN���X�B<br>
 * �C���X�^���X���j�󂳂�邩�A close() �����܂ŁA�J�����t�@�C���̓��b�N�����B
 *
 * @test SimpleFileReaderTest
 */
template <typename CharType, typename Traits>
class basic_simple_file_reader : public basic_file_reader<CharType, Traits>, boost::noncopyable
{
public:
// �R���X�g���N�^�ƃf�X�g���N�^
	basic_simple_file_reader() :
	m_pStream(new base_stream_type)
	{
	}

	/**
	 * @brief �t�@�C�����J������ԂŃC���X�^���X���\�z����i�}���`�o�C�g������̃p�X�j�B
	 *
	 * �n���ꂽ���������̂܂� open() �ɓn���B
	 * @sa open()
	 *
	 * @param lpszFilePath [in] �t�@�C�����B
	 * @param nMode [in] �t�@�C�����샂�[�h�B
	 */
	explicit basic_simple_file_reader(const char* lpszFilePath, int nMode = std::ios_base::in) :
	m_pStream(new base_stream_type)
	{
		open(lpszFilePath, nMode);
	}

	/**
	 * @brief �t�@�C�����J������ԂŃC���X�^���X���\�z����i���C�h������̃p�X�j�B
	 *
	 * �n���ꂽ���������̂܂� open() �ɓn���B
	 * @sa open()
	 *
	 * @param lpszFilePath [in] �t�@�C�����B
	 * @param nMode [in] �t�@�C�����샂�[�h�B
	 */
	explicit basic_simple_file_reader(const wchar_t* lpszFilePath, int nMode = std::ios_base::in) :
	m_pStream(new base_stream_type)
	{
		open(lpszFilePath, nMode);
	}

	/**
	 * @brief �t�@�C�����J���Ă��������B
	 *
	 * ���� close() ���ĂԁB<br>
	 * ����������O�͝��ݏ������B
	 * @sa close()
	 */
	virtual ~basic_simple_file_reader() throw()
	{
		try {
			close();
		}
		catch(...) {}
	}

// ���o��
	/**
	 * @brief �t�@�C�����J���i�}���`�o�C�g������̃p�X�j�B
	 *
	 * @param lpszFilePath [in] �t�@�C�����B
	 * @param nMode [in] �t�@�C�����샂�[�h�B
	 * @pre lock() �̖߂�l���A�R�s�[���ꂽ���̂��܂߂đS�Ĕj�󂳂�Ă���B
	 */
	void open(const char* lpszFilePath, int nMode = std::ios_base::in)
	{
		// ���p�t�@�C�����J���B
		m_pStream->open(
#if _MSC_VER == 1400
			CA2WEX<MAX_PATH>(lpszFilePath),
#else
			lpszFilePath, // VC8�Ń}���`�o�C�g������̃p�X��n���̂͊댯�iWiki�Q�Ɓj
#endif // _MSC_VER == 1400
			nMode
		);
	}

	/**
	 * @brief �t�@�C�����J���i���C�h������̃p�X�j�B
	 *
	 * @param lpszFilePath [in] �t�@�C�����B
	 * @param nMode [in] �t�@�C�����샂�[�h�B
	 * @pre lock() �̖߂�l���A�R�s�[���ꂽ���̂��܂߂đS�Ĕj�󂳂�Ă���B
	 */
	void open(const wchar_t* lpszFilePath, int nMode = std::ios_base::in)
	{
		m_pStream->open(lpszFilePath, nMode);
	}

	virtual void close()
	{
		// ���p�t�@�C�������B
		m_pStream->close();
	}

// �擾
	virtual bool is_open() const
	{
		return m_pStream->is_open();
	}

// ����
	/**
	 * ����ȑ���͉��������A�t�@�C�����̓X�g���[�������̂܂ܕԂ��B
	 */
	virtual stream_ref lock()
	{
		return m_pStream;
	}

private:
// �^��`
#if _MSC_VER <= 1200 && defined(__SGI_STL_PORT)
	typedef basic_ifstream<CharType, Traits> base_stream_type;
#else
	typedef std::basic_ifstream<CharType, Traits> base_stream_type;
#endif // _MSC_VER <= 1200 && defined(__SGI_STL_PORT)

// �ϐ�
	boost::shared_ptr<base_stream_type> m_pStream; ///< �t�@�C�����̓X�g���[���B
};

/**
 * @brief �t�@�C�����p�ǂݍ��݃N���X
 *
 * @ref file_sharing_info_file��p���āA���̃v���Z�X�ƃt�@�C���̓ǂݍ��݈ʒu�����p����N���X�B<br>
 * �C���X�^���X���j�󂳂�邩�A close() �����܂ŁA�J�������p�t�@�C���̓��b�N�����B<br>
 * lock() �̖߂�l���j�󂳂��܂ŁA@ref file_sharing_info_file�̓��b�N�����B
 *
 * @test FileSharingReaderTest
 * @test FileSharingReaderNetworkTest
 */
template <typename CharType, typename Traits>
class basic_file_sharing_reader : public basic_file_reader<CharType, Traits>, boost::noncopyable
{
public:
	/**
	 * @brief @ref file_sharing_info_file�Ɋւ����O�S�ʂ̊��ƂȂ�N���X�B
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
	 * @brief @ref file_sharing_info_file�̍쐬�Ɏ��s�����B
	 *
	 * �p�X���������A�A�N�Z�X�����Ȃ��B
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
			oss << "�t�@�C�����p���t�@�C�� \"" << lpszSharingInfoFilePath << "\" �̍쐬�Ɏ��s���܂����B";
			return oss.str();
		}
	};

	/**
	 * @brief @ref file_sharing_info_file�����݂��Ȃ��B
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
			oss << "�t�@�C�����p���t�@�C�� \"" << lpszSharingInfoFilePath << "\" �����݂��܂���B";
			return oss.str();
		}
	};

	/**
	 * @brief @ref file_sharing_info_file�ł͂Ȃ����̂��A@ref file_sharing_info_file�Ƃ��Ďw�肵���B
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
			oss << "\"" << lpszSharingInfoFilePath << "\" �́A�������t�@�C�����p���t�@�C���ł͂���܂���B";
			return oss.str();
		}
	};

	/**
	 * @brief �݊������Ȃ�@ref file_sharing_info_file���w�肳�ꂽ�B
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
			oss << "�t�@�C�����p���t�@�C�� \"" << lpszSharingInfoFilePath << "\" �̃o�[�W�����ɂ͔�Ή��ł��B";
			return oss.str();
		}
	};

	/**
	 * @brief ���p��񂪈�v���Ȃ��B
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
			oss << "\"" << lpszSharingInfoFilePath << "\" �� \"" << lpszFilePath << "\" �̋��p���t�@�C���ł͂���܂���B";
			return oss.str();
		}
	};

	/**
	 * @brief @ref file_sharing_info_file�������t�@�C�������݂��Ȃ��B
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
			oss << "�t�@�C�����p���t�@�C�� \"" << lpszSharingInfoFilePath << "\" �������t�@�C���͋��p�ł��܂���B";
			return oss.str();
		}
	};

// �R���X�g���N�^�ƃf�X�g���N�^
	/**
	 * @post ! is_open() && use_count() == 0
	 */
	basic_file_sharing_reader()
	{
	}

	/**
	 * @brief ���p�t�@�C�����J������ԂŃC���X�^���X���\�z����B
	 *
	 * �n���ꂽ���������̂܂� open() �ɓn���B
	 * @sa open()
	 *
	 * @param lpszFilePath [in,nullable] ���p�t�@�C���p�X�B
	 * @param lpszSharingInfoFilePath [in,nullable,optional] @ref file_sharing_info_file�p�X�B
	 * @param nMode [in,optional] �t�@�C�����샂�[�h�B
	 * @exception sharing_info_error @ref file_sharing_info_file�������B
	 */
	explicit basic_file_sharing_reader(
		const char* lpszFilePath,
		const char* lpszSharingInfoFilePath = 0,
		int nMode = std::ios_base::in)
	{
		open(lpszFilePath, lpszSharingInfoFilePath, nMode);
	}
	
	/**
	 * @brief ���p�t�@�C�����J���Ă��������B
	 *
	 * ���� close() ���ĂԁB<br>
	 * ����������O�͝��ݏ������B
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

// ���o��
	/**
	 * @brief ���p�t�@�C�����J���B
	 *
	 * @param lpszFilePath [in,nullable] ���p�t�@�C���p�X�Bnull���w�肵���ꍇ�A@ref file_sharing_info_file������o�������̂��g�p����B
	 * @param lpszSharingInfoFilePath [in,nullable,optional] @ref file_sharing_info_file�p�X�Bnull���w�肵���ꍇ�A���p�t�@�C�����̌��� ".shr" ��t�������̂��g�p����B
	 * @param nMode [in,optional] �t�@�C�����샂�[�h�B
	 * @pre m_pLockedStream.expired() && (lpszFilePath || lpszSharingInfoFilePath)
	 * @exception sharing_info_error @ref file_sharing_info_file�������B
	 */
	void open(const char* lpszFilePath, const char* lpszSharingInfoFilePath = 0, int nMode = std::ios_base::in)
	{
		_ASSERT(lpszFilePath || lpszSharingInfoFilePath);

		// �܂��͕���B
		close();

		// ���p���t�@�C������ݒ�B
		if(lpszSharingInfoFilePath) {
			m_strSharingInfoFilePath = lpszSharingInfoFilePath;
		}
		else {
			m_strSharingInfoFilePath = lpszFilePath;
			m_strSharingInfoFilePath += ".shr";
		}

		// �t�@�C�����p�����C���N�������g���ĕۑ�����B
		sharing_info_file aSharingInfoFile(m_strSharingInfoFilePath, lpszFilePath);
		aSharingInfoFile.retain();
		aSharingInfoFile.write();

		// ���p�t�@�C�����J���B
		m_SharingStream.open(
#if _MSC_VER == 1400
			CA2WEX<MAX_PATH>(aSharingInfoFile.path().c_str()), // VC8�Ń}���`�o�C�g������̃p�X��n���̂͊댯�iWiki�Q�Ɓj
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
			// ���p�t�@�C�������B
			m_SharingStream.close();

			// �t�@�C�����p�����f�N�������g���ĕۑ�����B
			sharing_info aSharingInfo(m_strSharingInfoFilePath);
			aSharingInfo.release();
			aSharingInfo.write();
		}
	}

// �擾
	/**
	 * @brief �t�@�C�������p���Ă���͎̂�����l���B
	 *
	 * @retval true use_count() == 1
	 * @retval false use_count() != 1
	 */
	bool unique() const
	{
		return use_count() == 1;
	}

	/**
	 * @brief ���p�����擾����B
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

// ����
	/**
	 * @brief @ref file_sharing_info_file�����b�N����B
	 *
	 * @return ���g�Ƃ��̐󂢃R�s�[���j�󂳂��܂ŁA@ref file_sharing_info_file�����b�N����t�@�C�����̓X�g���[���B
	 * @post ! m_pLockedStream.expired()
	 */
	virtual stream_ref lock()
	{
		// ���Ƀ��b�N�ς݂Ȃ�A�V���Ƀ��b�N����K�v�͂Ȃ��B
		boost::shared_ptr<locked_ifstream> pLockedStream(m_pLockedStream.lock());

		if(! pLockedStream) {
			// �V�K�Ƀ��b�N�B
			pLockedStream.reset(new locked_ifstream(m_SharingStream, m_strSharingInfoFilePath));
			m_pLockedStream = pLockedStream;
		}

		return pLockedStream;
	}

private:
// �^��`
	/// �t�@�C�����̓X�g���[���̌^�B
#if _MSC_VER <= 1200 && defined(__SGI_STL_PORT)
	typedef basic_ifstream<CharType, Traits> sharing_stream;
#else
	typedef std::basic_ifstream<CharType, Traits> sharing_stream;
#endif // _MSC_VER <= 1200 && defined(__SGI_STL_PORT)

	/**
	 * @ingroup file_sharing_info_file
	 * @brief �t�@�C�����p���N���X
	 *
	 * �C���X�^���X�����݂���ԁA@ref file_sharing_info_file�̓��b�N�����B
	 */
	class sharing_info : boost::noncopyable
	{
	public:
	// �R���X�g���N�^
		/**
		 * @brief @ref file_sharing_info_file�̃w�b�_�����������J���B
		 *
		 * �J�����Ƃ��ł���܂ŁA10ms�̊Ԋu��u���ăA�N�Z�X��������B
		 *
		 * @param rSharingInfoFilePath [in] @ref file_sharing_info_file�p�X�B
		 * @param bCreatable [in,optional] @ref file_sharing_info_file��V�K�ɍ쐬���Ă��ǂ����B
		 * @pre ! rSharingInfoFilePath.empty()
		 * @post m_hFile != 0
		 * @exception cant_create_sharing_info_file @ref file_sharing_info_file���쐬�ł��Ȃ������B
		 * @exception not_existing_sharing_info_file @ref file_sharing_info_file���J���Ȃ������B
		 * @exception illegal_sharing_info_file @ref file_sharing_info_file�ł͂Ȃ����̂��w�肳��Ă���B
		 * @exception not_compatible_sharing_info_file @ref �݊����̂Ȃ�@ref file_sharing_info_file���w�肳�ꂽ�B
		 */
		sharing_info(const std::string& rSharingInfoFilePath, bool bCreatable = false) :
		m_hFile(0)
		{
			_ASSERT(! rSharingInfoFilePath.empty());

			using namespace std;

			USES_CONVERSION;

			// �t�@�C�����p���t�@�C���̃p�X��_TCHAR�z�񉻁B
			const LPCTSTR lpszSharingInfoFilePathT = A2CT(rSharingInfoFilePath.c_str());

			// �G���[���������Ȃ�����A�Ƃɂ����J�����Ƃ���B
			while(1) {
				// �t�@�C�����p���t�@�C���ւ̔r���A�N�Z�X���J�n����B
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

				// �t�@�C�������݂��Ȃ��ƌ������Ƃ́A�r�������Ɉ������������킯�ł͂Ȃ��B
				struct _stat buf;
				if(::_stat(rSharingInfoFilePath.c_str(), &buf) == -1 || buf.st_mode & _S_IFDIR) {
					if(bCreatable)
						throw cant_create_sharing_info_file(rSharingInfoFilePath.c_str());
					else
						throw not_existing_sharing_info_file(rSharingInfoFilePath.c_str());
				}

				// ������Ƒ҂B
				::Sleep(10);
			}

			try {
				if(read()) {
					// �Ŕԍ����m�F����B
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
		 * @brief @ref file_sharing_info_file�̃��b�N����������B
		 */
		virtual ~sharing_info() throw()
		{
			if(m_hFile) {
				::CloseHandle(m_hFile);
			}
		}

	// �擾�^����
		/**
		 * @brief ���p�����C���N�������g����B
		 */
		void retain() throw()
		{
			++m_Data.m_nReferenceCount; 
		}

		/**
		 * @brief ���p�����f�N�������g����B
		 */
		void release() throw()
		{
			--m_Data.m_nReferenceCount;
		}

		/**
		 * @brief ���p�����擾����B
		 */
		int use_count() const throw()
		{
			return m_Data.m_nReferenceCount;
		}

		/**
		 * @brief �t�@�C���|�W�V������ݒ肷��B
		 *
		 * @param nPos [in] �t�@�C���|�W�V�����B
		 */
		void pos(unsigned __int64 nPos) throw()
		{
			m_Data.m_nPos = nPos;
		}

		/**
		 * @brief �t�@�C���|�W�V�������擾����B
		 */
		unsigned __int64 pos() const throw()
		{
			return m_Data.m_nPos;
		}

	// ���o��
		/**
		 * @brief ���p�����t�@�C���ɏ������ށB
		 *
		 * @pre m_hFile != 0
		 */
		virtual void write() throw()
		{
			write_header();
		}

	protected:
	// �^��`
		class illegal_sharing_info_file_ : public std::exception {};

#pragma pack( push, 1 )
		/**
		 * @brief @ref file_sharing_info_file�擪16�o�C�g
		 *
		 * �\���̂̒��g�����̂܂܃t�@�C���ɏ������ނ̂ŁA�A���C�����g�͂��������`���Ă����B
		 */
		struct data
		{
			unsigned char m_nMajor; ///< ���p�t�@�C�����W���[�Ŕԍ��i��ʌ݊����Ȃ��j
			unsigned char m_nMinor; ///< ���p�t�@�C���}�C�i�[�Ŕԍ��i��ʌ݊�������j
			unsigned __int64 m_nPos; ///< ���p�t�@�C���|�W�V�����B
			int m_nReferenceCount; ///< ���p���Ă���v���Z�X�̐��B
		};
#pragma pack( pop )

	// �ϐ�
		HANDLE m_hFile; ///< @ref file_sharing_info_file�n���h���B

	private:
	// ���o��
		/**
		 * @brief @ref file_sharing_info_file��ǂݍ��ށB
		 *
		 * @retval true @ref file_sharing_info_file�ɏ������܂�Ă������ǂݍ��񂾁B
		 * @retval false @ref file_sharing_info_file�ɂ͉����������܂�Ă��Ȃ��B
		 * @pre m_hFile != 0
		 * @exception illegal_sharing_info_file_ @ref file_sharing_info_file�ł͂Ȃ��B
		 */
		bool read()
		{
			_ASSERT(m_hFile);

			// �t�@�C���|�C���^��擪�ֈړ��B
			::SetFilePointer(m_hFile, 0, 0, FILE_BEGIN);

			// �擪16�o�C�g��ǂݍ��ށB
			// 1�o�C�g���ǂݍ��߂Ȃ�����A����͐V�K�ɍ쐬���ꂽ�t�@�C���ł���B
			DWORD dw;
			if(::ReadFile(m_hFile, &m_Data, sizeof(m_Data), &dw, 0) && dw == 0)
				return false;

			// ���r���[�ɓǂݍ��߂���A�S���֌W�̂Ȃ��t�@�C���ł���B
			if(dw < sizeof(m_Data))
				throw illegal_sharing_info_file_();

			return true;
		}

		/**
		 * @brief �擪16�o�C�g���t�@�C���ɏ����o���B
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

	// �ϐ�
		data m_Data; ///< �t�@�C�����p���B
	};

	/**
	 * @brief @ref file_sharing_info_file���t���ɊJ���B
	 *
	 * @param rSharingInfoFilePath [in] @ref file_sharing_info_file�p�X�B
	 * @param lpszFilePath [in,optional] ���p�t�@�C���p�X�B�����null�ɂ����ꍇ�A@ref file_sharing_info_file�ɋ��p�t�@�C���p�X���L����Ă��Ȃ���΂Ȃ�Ȃ��B 
	 * @pre ! rSharingInfoFilePath.empty()
	 * @post m_hFile != 0
	 * @exception not_match_sharing_info @ref file_sharing_info_file�Ƌ��p�t�@�C���̊֌W�ɋ^�₪����B
	 * @exception not_existing_sharing_file @ref file_sharing_info_file�ɋL����Ă���t�@�C�������݂��Ȃ��B
	 */
	class sharing_info_file : public sharing_info
	{
	public:
		sharing_info_file(const std::string& rSharingInfoFilePath, const char* lpszFilePath = 0) :
		sharing_info(rSharingInfoFilePath, lpszFilePath != 0)
		{
			// ��΃p�X��p�ӂ���B
			char szSharingInfoFileAbsPath[_MAX_PATH] = { 0 };
			::_fullpath(szSharingInfoFileAbsPath, rSharingInfoFilePath.c_str(), _MAX_PATH);

			char szFileAbsPath[_MAX_PATH] = { 0 };
			if(lpszFilePath) {
				::_fullpath(szFileAbsPath, lpszFilePath, _MAX_PATH);
			}

			if(! read_path(szSharingInfoFileAbsPath)) {
				// ���p�t�@�C����ǂݍ��߂Ȃ������ꍇ�A�����I�Ɏw�肳��Ă��Ȃ���΂Ȃ�Ȃ��B
				if(lpszFilePath == 0)
					throw illegal_sharing_info_file(rSharingInfoFilePath.c_str());

				m_strFilePath = szFileAbsPath;

				USES_CONVERSION;

				// �t�@�C�����p���t�@�C������̑��΃p�X�����߂�B
				TCHAR szRelPath[_MAX_PATH];
				const char* lpszRelPath = 
					::PathRelativePathTo(szRelPath, A2CT(szSharingInfoFileAbsPath), 0, A2CT(szFileAbsPath), 0)
					? T2CA(szRelPath)
					: szFileAbsPath;

				// �p�X���������ށB
				DWORD dw;
				::WriteFile(m_hFile, lpszRelPath, ::strlen(lpszRelPath), &dw, 0);
			}
			else {
				if(lpszFilePath) {
					// ���p���ɋL����Ă���̂Ƃ͕ʂ̃t�@�C����������Ȃ��B
					if(::_stricmp(m_strFilePath.c_str(), szFileAbsPath) != 0)
						throw not_match_sharing_info(lpszFilePath, rSharingInfoFilePath.c_str());
				}
				else {
					// ���p���ɋL����Ă���t�@�C�������݂��Ȃ��B
					if(::_access(m_strFilePath.c_str(), 0) == -1)
						throw not_existing_sharing_file(rSharingInfoFilePath.c_str());
				}
			}
		}

		/**
		 * @brief ���p�t�@�C���p�X���擾����B
		 */
		const std::string& path() const throw()
		{
			return m_strFilePath;
		}

	private:
	// ���o��
		/**
		 * @brief @ref file_sharing_info_file��ǂݍ��ށB
		 *
		 * @param lpszSharingInfoFileAbsPath [in] file_sharing_info_file�̐�΃p�X�B
		 * @retval true ���p����t�@�C���̃p�X��ǂݍ��񂾁B
		 * @retval false ���p����t�@�C���̃p�X�͋L����Ă��Ȃ��B
		 * @pre m_hFile != 0
		 * @post ::GetFilePointer(m_hFile, 0, 0, FILE_END) == 0
		 * @exception illegal_sharing_info_file @ref file_sharing_info_file�ł͂Ȃ��B
		 */
		bool read_path(const char* lpszSharingInfoFileAbsPath)
		{
			USES_CONVERSION;

			_ASSERT(! ::PathIsRelative(A2CT(lpszSharingInfoFileAbsPath)));
			_ASSERT(m_hFile);

			// ���p����t�@�C���̃p�X���w���擪�̃A�h���X�܂ňړ�����B
			if(::SetFilePointer(m_hFile, sizeof(data), 0, FILE_BEGIN) != sizeof(data))
				throw illegal_sharing_info_file(lpszSharingInfoFileAbsPath);

			// �t�@�C���T�C�Y���w�b�_�T�C�Y�ɓ�����������A���p����t�@�C���̃p�X�͋L����Ă��Ȃ��B
			DWORD dw;
			const DWORD dwFileSize = ::GetFileSize(m_hFile, &dw);
			if(dwFileSize == sizeof(data))
				return false;

			// �t�@�C���p�X�̒��������߂�B
			const DWORD dwPathSize = dwFileSize - ::SetFilePointer(m_hFile, 0, 0, FILE_CURRENT);
			if(dwPathSize >= _MAX_PATH)
				throw illegal_sharing_info_file(lpszSharingInfoFileAbsPath);

			// �t�@�C�����p���t�@�C������̑��΃p�X����荞�ށB
			char szFilePath[_MAX_PATH] = { 0 };
			if(! (::ReadFile(m_hFile, szFilePath, _MAX_PATH, &dw, 0) && dw > 0 && dw < _MAX_PATH))
				throw illegal_sharing_info_file(lpszSharingInfoFileAbsPath);

			// �t�@�C�����p���t�@�C���̃f�B���N�g���p�X�����߂�B
			TCHAR szDirPathT[_MAX_PATH + 1] = { 0 };
			::_tcsncpy(szDirPathT, A2CT(lpszSharingInfoFileAbsPath), _MAX_PATH);
			::PathRemoveFileSpec(szDirPathT);

			// �t�@�C�����p���t�@�C������̑��΃p�X���A��΃p�X�ɏ��������B
			TCHAR szPathT[_MAX_PATH + 1] = { 0 };
			if(! ::PathCombine(szPathT, szDirPathT, A2CT(szFilePath))) {
				::_tcscpy(szPathT, A2CT(szFilePath));
			}

			m_strFilePath = T2CA(szPathT);

			return true;
		}

	// �ϐ�
		std::string m_strFilePath; ///< ���p�t�@�C���p�X�B
	};

	/**
	 * @brief ���p�������b�N���ăt�@�C����ǂݍ��ރX�g���[���N���X
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
	// �R���X�g���N�^�ƃf�X�g���N�^
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

	// �ϐ�
		stream_type& m_rStream; ///< ���p�t�@�C�����̓X�g���[���B
		sharing_info m_SharingInfo; ///< �t�@�C�����p���B
	};

// �ϐ�
	sharing_stream m_SharingStream; ///< ���p�t�@�C�����̓X�g���[���B
	std::string m_strSharingInfoFilePath; ///< @ref file_sharing_info_file���B

	boost::weak_ptr<locked_ifstream> m_pLockedStream; ///< ���p�t�@�C�����b�N�B
};

/**
 * @brief basic_file_reader �p�� istream_iterator �N���X
 *
 * �C���N�������g���� basic_file_reader �� basic_file_reader::lock() ���ėv�f�����o���A������ basic_file_reader::lock() ����������B
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
// �^��`
	typedef basic_file_reader<CharType, Traits> file_reader_type; ///< �t�@�C���ǂݍ��݃N���X�^�B

// �R���X�g���N�^
	/**
	 * @brief �I�[�����q�Ƃ��č\�z����B
	 */
	file_read_iterator() throw() :
	m_pFileReader(0)
	{
	}

	/**
	 * @brief ��I�[�����q�Ƃ��č\�z����B
	 *
	 * @param rFileReader [in,ref] �t�@�C���ǂݍ��݃N���X�B
	 */
	file_read_iterator(file_reader_type& rFileReader) :
	m_pFileReader(&rFileReader)
	{
		// �ŏ��̗v�f�����o���B
		increment();
	}

private:
// �����q��`
	friend class boost::iterator_core_access;

	/**
	 * @brief �Ō�Ɏ��o�����v�f��Ԃ��B
	 *
	 * @return �Ō�Ɏ��o�����v�f�B
	 * @pre *this != file_read_iterator()
	 */
	reference dereference() const throw()
	{
		return m_Value;
	}

	/**
	 * @brief �����q��������Ԃɂ��邩���ׂ�B
	 *
	 * @retval true ������Ԃɂ���B
	 * @retval false ������Ԃɂ͂Ȃ��B
	 */
	bool equal(const file_read_iterator& rhs) const throw()
	{
		return m_pFileReader == rhs.m_pFileReader;
	}

	/**
	 * @brief �t�@�C�����玟�̗v�f�����o���B
	 *
	 * �v�f�̎��o���Ɏ��s����ƁA�C���X�^���X�͏I�[�����q�ɂȂ�B
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
// �ϐ�
	file_reader_type* m_pFileReader; ///< ���p�t�@�C���ǂݍ��݃C���X�^���X�Bnull�Ȃ�ΏI�[�����q�ł���B
	ValueType m_Value; ///< �Ō�Ɏ��o�����v�f�B
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
