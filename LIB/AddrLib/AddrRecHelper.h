#pragma once

#include "stdafx.h"
#include "AddrRec.h"

#include <WinLib/ADOBase.h>
#include <TDC/sindylib_base/RowContainer.h>

namespace addr
{

// �����t���Z���R�[�h�ϊ�TB/�����t���Z���R�[�h�ϊ���TB �X�L�[�}��`
namespace cv_schema
{
	namespace cv_common
	{
		const static TCHAR kAddrCode[]		= _T("AddrCode");
		const static TCHAR kRegistDate[]	= _T("RegistDate");
	}

	namespace pnt_conv
	{
		const static TCHAR kTableName[]	= _T("Pnt_CodeConv");
		const static TCHAR kPntCode[]	= _T("PntCode");
		const static TCHAR kPntName[]	= _T("PntName");
		using namespace ::addr::cv_schema::cv_common;		
	}

	namespace gou_conv
	{
		const static TCHAR kTableName[]	= _T("Gou_CodeConv");
		const static TCHAR kGouCode[]	= _T("GouCode");
		const static TCHAR kGouName[]	= _T("GouName");
		using namespace ::addr::cv_schema::cv_common;
	}
}

/**
 * @brief ��؂蕶���i���ԍ������p�j

 */
static const CString SEP_HYPHEN = _T("-");

namespace AddrRecHelperEnum
{

/**
 * @brief �Z���X�e�[�^�X�iCharAddrConvert���痬�p�j
 */
enum AddrStatus
{
	kNormal,			//!< �ʏ�Z��
	kExtGaiku,			//!< �g���X�敄������
	kMojiGou,			//!< ���ԍ��ɕ�������
	kMojiGou_ExtGaiku,	//!< ���ԍ��ɕ�������(�g���X�敄����)
	kSepOver,			//!< ���ԍ��̋�؂肪����
	kSepOver_ExtGaiku,	//!< ���ԍ��̋�؂肪����(�g���X�敄����)
	kNoOver,			//!< �n�ԁE���ŕ\��������Ȃ��ԍ�
	kNoOver_ExtGaiku 	//!< �n�ԁE���ŕ\��������Ȃ��ԍ�(�g���X�敄����)
};

}

/**
 * @brief �Z�����R�[�h�iCRowCotainer�t���j
 */
typedef std::pair<AddrRec, sindy::CRowContainer> AddrRecPair;

/**
 * @brief �Z�����R�[�h���X�g
 */
typedef std::vector<AddrRecPair> AddrRecPairs;

/**
 * @brief �������E�����l�̃y�A
 */
typedef std::pair<CString, CString> AttributeValue;

using namespace AddrRecHelperEnum;

/**
 * @brief �Z�����R�[�h�����⏕�N���X
 */
class AddrRecHelper
{
public:

	/**
	 * @brief �R���X�g���N�^
	 */
	AddrRecHelper() {}

	/**
	 * @brief �����t���Z���R�[�h�ϊ��e�[�u���ڑ�
	 * @param[in] charConvFile �����t���Z���R�[�h�ϊ��e�[�u���t�@�C��
	 * @retval true ����
	 * @retval false ���s
	 */
	bool connectCharConvFile(const CString& charConvFile);

	/**
	 * @brief �����t���Z���R�[�h�ϊ����e�[�u���ڑ�
	 * @param[in] charConvTmpFile �����t���Z���R�[�h�ϊ����e�[�u��
	 * @retval true ����
	 * @retval false ���s
	 */
	bool connectCharConvTmpFile(const CString& charConvTmpFile);

	/**
	 * @brief �n�Ԗ��́E�����̎擾
	 * @param[in] record �Z�����R�[�h�R���e�i
	 * @retval true ����
	 * @retval false ���s
	 */
	bool makeAddrNames(AddrRec& record);

	/**
	 * @brief �n�Ԗ��̎擾
	 * @param[in] record �Z�����R�[�h�R���e�i
	 * @retval true ����
	 * @retval false ���s
	 */
	bool makeChibanName(AddrRec& record);

	/**
	 * @brief �����̎擾
	 * @param[in] record �Z�����R�[�h�R���e�i
	 * @retval true ����
	 * @retval false ���s
	 */
	bool makeGouName(AddrRec& record);

	/**
	 * @brief �s�s�n�}�s���E�|���S�����R�[�h�擾
	 * @param[in]  cityAdminT     �e�[�u���R���e�i�i�s�s�n�}�s���E�|���S���j
	 * @param[in]  addrcode11     �Ώۂ̏Z�����R�[�h�i11���j
	 * @param[out] caAddrRecPairs �Z�����R�[�h���X�g�i�s�s�n�}�s���E�|���S������擾�������́j
	 */
	void getCityAdminRecs(
			sindy::CTableContainer& cityAdminT,
			const CString& addrcode11,
			AddrRecPairs& caAddrRecPairs);

	/**
	 * @brief �Z�����R�[�h���X�g�i���|�C���g�j����
	 * @param[in]  gouPointT     �e�[�u���R���e�i�i���|�C���g�j
	 * @param[in]  csAdrRecPairs �Z�����R�[�h���X�g�i���|�C���g�j
	 * @param[out] gpAdrRecs     �Z�����R�[�h�i�s�s�n�}�s���E�|���S���j
	 */
	void makeAdrRecordsFromGp(
			sindy::CTableContainer& gouPointT,
			const AddrRecPairs& csAdrRecPairs,
			AddrRecs& gpAdrRecs);

	/**
	 * @brief �Z�����R�[�h���X�g�i���|�C���g�j����
	 * @param[in]  gouPointT  �e�[�u���R���e�i�i���|�C���g�j
	 * @param[in]  addrcode11 �ΏۏZ���R�[�h�i11���j
	 * @param[in]  caRow      Row�R���e�i�i�s�s�n�}�s���E�|���S���j
	 * @param[out] gpAdrRecs  �Z�����R�[�h���X�g�i���|�C���g�j
	 */
	void makeAdrRecordsFromGp(
			sindy::CTableContainer& gouPointT,
			const CString& addrcode11,
			const sindy::CRowContainer& caRow,
			AddrRecs& gpAdrRecs);

	/**
	 * @brief �Z�����R�[�h���X�g�i���|�C���g�j����
	 * @param[in]  caRow      Row�R���e�i�i�s�s�n�}�s���E�|���S���j
	 * @param[in]  gpRow      Row�R���e�i�i���|�C���g�j
	 * @param[in]  addrcode11 �ΏۏZ���R�[�h�i11���j
	 * @param[out] gpAdrRecs  �Z�����R�[�h���X�g�i���|�C���g�j
	 */
	void makeAdrRecordsFromGp(
			const sindy::CRowContainer& caRow,
			const sindy::CRowContainer& gpRow,
			const CString& addrcode11,
			AddrRecs& gpAdrRecs);

	/**
	 * @brief �Z�����R�[�h���X�g�iCS�|�C���g�j����
	 * @param[in]  csPointT   �e�[�u���R���e�i�iCS�|�C���g�j
	 * @param[in]  rec        �Z�����R�[�h�i�s�s�n�}�s���E�|���S���j
	 * @param[out] gpAdrRecs  �Z�����R�[�h���X�g�iCS�|�C���g�j
	 */
	void makeAdrRecordsFromCs(
			sindy::CTableContainer& csPointT,
			const CString& addrcode11,
			std::map<CString, AddrRecs>& addrRecs);

private:

	/**
	 * @brief Access�ւ̐ڑ�
	 * @param[in] ado DB�ڑ��Ǘ��N���X
	 * @param[in] file Access�t�@�C���p�X
	 * @retval true ����
	 * @retval false ���s
	 */
	bool openDb(CADOBase& ado, const CString& file);

	/**
	 * @brief Access���̃e�[�u������
	 * @note �������ʂ�1�t�B�[���h�ڂ̒l���擾����i���A�t�B�[���h�͕�����Ɍ���j
	 * @note �܂��A�������ʂ��������R�[�h�̏ꍇ�A�擾���ʂ�1���R�[�h�ڂ̒l���̗p����
	 * @param[in]  ado    �ڑ��Ǘ��N���X
	 * @param[in]  sqlStr �����pSQL������
	 * @param[out] value  �擾���ʁi������j
	 * @retval true ����
	 * @retval false ���s
	 */
	bool searchStrValue(const CADOBase& ado, const LPCSTR& sqlStr, CString& value);

	/**
	 * @brief �����t���Z�����̎擾
	 * @param[in]   sql          �擾�pSQL
	 * @param[out]  resultName   �擾���������t���Z������
	 * @retval true ����
	 * @retval false ���s
	 */
	bool getName(const uh::tstring& sql, CString& resultName);

	/**
	 * @brief �Z���X�e�[�^�X�擾
	 * @param[in] gaikuFugo    �X�敄��
	 * @param[in] extGaikuFugo �g���X�敄��
	 * @param[in] gouNo        ���ԍ�
	 * @param[in] expGouNo     �g�����ԍ��t���O
	 * @retval �Z���X�e�[�^�X
	 */
	AddrStatus getAddrStatus(
				const CString& gaikuFugo,
				const CString& extGaikuFugo, 
				const CString& gouNo,
				bool expGouNo);

	/**
	 * @brief �Z�����R�[�h�i�ʏ�Z���Łj�擾
	 * @param[out] addrRecs    �Z�����R�[�h���X�g
	 * @param[in]  addrcode    �Z���R�[�h�i11���j
	 * @param[in]  gaikuFugo   �X�敄��
	 * @param[in]  gouNo       ���ԍ�
	 * @param[in]  expGouNo    �g�����ԍ��t���O
	 * @param[in]  geo          �`��i���|�C���g�j
	 */
	void makeNormalAdrRecordFromGp(
			AddrRecs& addrRecs,
			const CString& addrcode,
			const CString& gaikuFugo,
			const CString& gouNo1,
			bool expGouNo,
			IGeometryPtr& geo);

	/**
	 * @brief �Z�����R�[�h�i�����t���Z���Łj�擾
	 * @param[out] addrRecs     �Z�����R�[�h���X�g
	 * @param[in]  addrcode     �Z���R�[�h�i11���j
	 * @param[in]  gaikuFugo    �X�敄��
	 * @param[in]  extGaikuFugo �g���X�敄��
	 * @param[in]  gouNo        ���ԍ�
	 * @param[in]  expGouNo     �g�����ԍ��t���O
	 * @param[in]  geo          �`��i���|�C���g�j
	 */
	void makeCharAdrRecordFromGp(
			AddrRecs& addrRecs,
			const CString& addrcode,
			const CString& extGaikuFugo,
			const CString& gaikuFugo,
			const CString& gouNo,
			bool expGouNo,
			IGeometryPtr& geo
		);

	/**
	 * @brief �ʏ�Z���𐶐����邩�𔻒�
	 * @param[in] addrClass    �Z���������
	 * @param[in] gaikuFugo    �X�敄��
	 * @retval true ��������
	 * @retval false �������Ȃ�
	 */
	bool isMakeNormalAddrRec(int addrClass, const CString& gaikuFugo);

	/**
	 * @brief �����t���Z���𐶐����邩�𔻒�
	 * @param[in] addrClass    �Z���������
	 * @param[in] gaikuFugo    �X�敄��
	 * @param[in] extGaikuFugo �g���X�敄��
	 * @param[in] gouNo        ���ԍ�
	 * @param[in] expGouNo     �g�����ԍ��t���O
	 * @retval true ��������
	 * @retval false �������Ȃ�
	 */
	bool isMakeCharAddrRec(
			int addrClass,
			const CString& gaikuFugo,
			const CString& extGaikuFugo, 
			const CString& gouNo,
			bool expGouNo);

private:

	CADOBase m_charConvAdo;     //!< DB�ڑ��Ǘ��N���X�i�����t���Z���R�[�h�ϊ��e�[�u���p�j
	CADOBase m_charConvTmpAdo;  //!< DB�ڑ��Ǘ��N���X�i�����t���Z���R�[�h�ϊ����e�[�u���p�j
};

} // namespace addr
