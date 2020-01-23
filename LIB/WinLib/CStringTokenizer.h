/////////////////////////////////////////////////////////////////////////////
// Name:        wx/tokenzr.h
// Purpose:     CString tokenizer - a C++ replacement for strtok(3)
// Author:      Guilhem Lavaux
// Modified by: (or rather rewritten by) Vadim Zeitlin
// Created:     04/22/98
// RCS-ID:      $Id: CStringTokenizer.h,v 1.1.1.1 2003/02/19 05:41:36 yonezawa Exp $
// Copyright:   (c) Guilhem Lavaux
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _CSTRRING_TOKENZRH
#define _CSTRRING_TOKENZRH


//#include	<Str.h> <- CString�N���X�͂ǂ���g�����킩��Ȃ��̂ŁA�����ŃC���N���[�h����

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------
const unsigned int STRING_MAXLEN = UINT_MAX - 100;

// default: delimiters are usual white space characters
#define DEFAULT_DELIMITERS (_T(" \t\r\n"))

/**
	�g�[�N���؂�o�����[�h
*/
enum CStringTokenizerMode
{
    TOKEN_INVALID = -1,   //<! set by def ctor until SetString() is called
    TOKEN_DEFAULT,        //<! strtok() for whitespace delims, RET_EMPTY else
    TOKEN_RET_EMPTY,      //<! return empty token in the middle of the string
    TOKEN_RET_EMPTY_ALL,  //<! return trailing empty tokens too
    TOKEN_RET_DELIMS,     //<! return the delim with token (implies RET_EMPTY)
    TOKEN_STRTOK          //<! behave exactly like strtok(3)
};

// ----------------------------------------------------------------------------
// CStringTokenizer: replaces infamous strtok() and has some other features
// ----------------------------------------------------------------------------
/**
	�����񂩂�g�[�N���؂�o�����s���N���X
*/
class CStringTokenizer //: public wxObject
{
public:
	/**
	* �f�t�H���g�R���X�g���N�^�B\n
	* setString()�Ńg�[�N���؂�o�����s����������Z�b�g���邱�ƁB
	*
	* @param	�Ȃ�
	* @param	�Ȃ�
	*/
    CStringTokenizer() { m_mode = TOKEN_INVALID; }

    
	/**
	* �R���X�g���N�^�B
	*
	* @param	const CString	[in]	�g�[�N���؂�o�����s��������
	* @param	const CString	[in]	�f���~�^
	* @return	�Ȃ�
	*/
	CStringTokenizer(const CString& str,
                      const CString& delims = DEFAULT_DELIMITERS,
                      CStringTokenizerMode mode = TOKEN_DEFAULT)
	{
	    SetString(str, delims, mode);
	}

	/**
	* �g�[�N���؂�o�����s����������Z�b�g����B
	* �f�t�H���g�R���X�g���N�^�ŏ��������ꂽ�Ƃ��̓R�[������K�v������B
	*
	* @param	const CString	[in]	�g�[�N���؂�o�����s��������
	* @param	const CString	[in]	�f���~�^
	* @return	�Ȃ�
	*/
    void SetString(const CString& str,
                   const CString& delims = DEFAULT_DELIMITERS,
                   CStringTokenizerMode mode = TOKEN_DEFAULT)
	{
		if ( mode == TOKEN_DEFAULT )
		{
			// by default, we behave like strtok() if the delimiters are only
			// whitespace characters and as TOKEN_RET_EMPTY otherwise (for
			// whitespace delimiters, strtok() behaviour is better because we want
			// to count consecutive spaces as one delimiter)
			const TCHAR *p;
			//for ( p = delims.c_str(); *p; p++ )
			for ( p = LPCTSTR( delims ); *p; p++ )
			{
				if ( !_istspace(*p) )
					break;
			}

			if ( *p )
			{
				// not whitespace char in delims
				mode = TOKEN_RET_EMPTY;
			}
			else
			{
				// only whitespaces
				mode = TOKEN_STRTOK;
			}
		}

		m_delims = delims;
		m_mode = mode;

		Reinit(str);
	}

	/**
	* �C���X�^���X�̍ŏ������������Ȃ��B
	*
	* @param	const CString	[in]	�g�[�N���؂�o�����s��������
	* @return	�Ȃ�
	*/
    void Reinit(const CString& str)
	{
		//ASSERT( IsOk(), _T("you should call SetString() first") );

		m_string = str;
		m_pos = 0;

		// empty string doesn't have any tokens
		m_hasMore = !m_string.IsEmpty();
	}

	/**
	* �g�[�N�������擾����
	*
	* @param		�Ȃ�
	* @return	size_t		�g�[�N����
	*/
    size_t CountTokens() const
	{
		//wxCHECK_MSG( IsOk(), 0, _T("you should call SetString() first") );

		// VZ: this function is IMHO not very useful, so it's probably not very
		//     important if it's implementation here is not as efficient as it
		//     could be - but OTOH like this we're sure to get the correct answer
		//     in all modes
		CStringTokenizer *self = (CStringTokenizer *)this;    // const_cast
		CString stringInitial = m_string;

		size_t count = 0;
		while ( self->HasMoreTokens() )
		{
			count++;

			(void)self->GetNextToken();
		}

		self->Reinit(stringInitial);

		return count;
	}

	/**
	* �g�[�N���������݂��邩�m�F����
	*
	* @param		�Ȃ�
	* @return	bool	true�F�g�[�N���L��
	*/
    bool HasMoreTokens() const
	{
		//wxCHECK_MSG( IsOk(), FALSE, _T("you should call SetString() first") );

		//if ( m_string.find_first_not_of(m_delims) == STRING_MAXLEN )
		//if ( m_string.SpanIncluding ( m_delims ).GetLength() == 0  )
		int nAccept = _tcsspn( m_string, m_delims );
		if ( nAccept >= m_string.GetLength() )
		{
			// no non empty tokens left, but in 2 cases we still may return TRUE if
			// GetNextToken() wasn't called yet for this empty token:
			//
			//   a) in TOKEN_RET_EMPTY_ALL mode we always do it
			//   b) in TOKEN_RET_EMPTY mode we do it in the special case of a
			//      string containing only the delimiter: then there is an empty
			//      token just before it
			return (m_mode == TOKEN_RET_EMPTY_ALL) ||
				   (m_mode == TOKEN_RET_EMPTY && m_pos == 0)
						? m_hasMore : FALSE;
		}
		else
		{
			// there are non delimiter characters left, hence we do have more
			// tokens
			return TRUE;
		}
	}

	/**
	* �g�[�N�����擾����
	*
	* @param		�Ȃ�
	* @return	CString	�؂�o���ꂽ�g�[�N��
	*/
    CString GetNextToken()
	{
		// strtok() doesn't return empty tokens, all other modes do
		bool allowEmpty = m_mode != TOKEN_STRTOK;

		CString token;
		do
		{
			if ( !HasMoreTokens() )
			{
				break;
			}
			// find the end of this token
			//size_t pos = m_string.find_first_of(m_delims);
			size_t pos = m_string.FindOneOf(m_delims);

			// and the start of the next one
			//if ( pos == STRING_MAXLEN )
			if ( pos == -1 )
			{
				// no more delimiters, the token is everything till the end of
				// string
				token = m_string;

				m_pos += m_string.GetLength();
				m_string.Empty();

				// no more tokens in this string, even in TOKEN_RET_EMPTY_ALL
				// mode (we will return the trailing one right now in this case)
				m_hasMore = FALSE;
			}
			else
			{
				size_t pos2 = pos + 1;

				// in TOKEN_RET_DELIMS mode we return the delimiter character
				// with token
				token = CString(m_string, m_mode == TOKEN_RET_DELIMS ? pos2
																		: pos);

				// remove token with the following it delimiter from string
				//m_string.erase(0, pos2);
				m_string.Delete(0, pos2);

				// keep track of the position in the original string too
				m_pos += pos2;
			}
		}
		while ( !allowEmpty && token.IsEmpty() );

		return token;
	}

	/**
	* �J�����g�̃g�[�N���؂�o���X�e�[�g���擾����B�g�[�N���؂�o�����s�������ʂ̎c��̕�����i����������ł͂Ȃ��j
	*
	* @param		�Ȃ�
	* @return	CString	�c��̕�����
	*/
    CString GetString() const { return m_string; }

	/**
	* �J�����g�̃g�[�N���ʒu���擾����B���^�[���l��0�̏ꍇ��GetNextToken()���R�[������Ă��Ȃ��B
	*
	* @param		�Ȃ�
	* @return	size_t	�J�����g�̃g�[�N���ʒu
	*/
    size_t GetPosition() const { return m_pos; }

	/**
	* �J�����g�̃g�[�N���؂�o�����[�h���擾����B
	*
	* @param		�Ȃ�
	* @return	CStringTokenizerMode	�J�����g�̃g�[�N���؂�o�����[�h
	*/
    CStringTokenizerMode GetMode() const { return m_mode; }

    // backwards compatibility section from now on
    // -------------------------------------------

    // for compatibility only, use GetNextToken() instead
/*    CString NextToken() { return GetNextToken(); }

    // compatibility only, don't use
    void SetString(const CString& to_tokenize,
                   const CString& delims,
                   bool ret_delim)
    {
        SetString(to_tokenize, delims, TOKEN_RET_DELIMS);
    }

    CStringTokenizer(const CString& to_tokenize,
                      const CString& delims,
                      bool ret_delim)
    {
        SetString(to_tokenize, delims, ret_delim);
    }
*/
protected:
    bool IsOk() const { return m_mode != TOKEN_INVALID; }

    CString m_string;              //!< �g�[�N���؂�o�������c��̕�����
    CString m_delims;              //!< ���ׂẴf���~�^

    size_t   m_pos;                //!< ����������ł̃g�[�N���ʒu

    CStringTokenizerMode m_mode;   //!< �g�[�N���؂�o�����[�hs

    bool     m_hasMore;            //!< �g�[�N�����܂����݂��邩�ۂ��̃t���O
};

// ----------------------------------------------------------------------------
// convenience function which returns all tokens at once
// ----------------------------------------------------------------------------

// the function takes the same parameters as CStringTokenizer ctor and returns
// the array containing all tokens
//wxArrayString WXDLLEXPORT
//wxStringTokenize(const CString& str,
  //               const CString& delims = DEFAULT_DELIMITERS,
  //               CStringTokenizerMode mode = TOKEN_DEFAULT);

#endif // _CSTRRING_TOKENZRH
