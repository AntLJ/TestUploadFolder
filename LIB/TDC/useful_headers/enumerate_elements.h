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

#pragma once

#include <sstream>
#include <vector>

namespace uh
{
	typedef std::vector< std::string > ElementsStrings;

	// TODO: �߂�l�^���e���v���[�g�����Ă����������B

	/**
	 * @brief �^����ꂽ�W���̗v�f��񋓂�����������쐬���ĕԂ��֐��e���v���[�g
	 * @note	�uID�╶�����1000���񋓂���Where��Q�v�����̂Ɏg����̂�z��B
	 * @note	���C�u�������̌o�܂͉��L���Q�Ƃ̂��ƁB
	 *			http://orias.mr.ipc.pioneer.co.jp/reviewboard/r/1957/#comment4609
	 * @param  elements   [in] �ΏۏW���i�����o�֐��Ƃ���begin(), end()�������A�����ɂ��iterator���擾�ł���^�ł��鎖�������j
	 * @param  max        [in] �񋓂��鐔�̍ő�l�i�f�t�H���g��1000�j
	 * @param  quote      [in] ���p���i�f�t�H���g�͋󕶎���j\n
	 *                        �i�񋓂��ꂽ�e�v�f���͂ޕ�����BIN��̒��ł͕�����v�f�̓V���O���N�H�[�g�ň͂ޕK�v������̂ŁA���̎w��Ɏg���鎖��z��B�j
	 * @param  delimiter  [in] �f���~�^�i�f�t�H���g��","�j
	 */
	template <typename T>
	ElementsStrings enumerate_elements(
		const T& elements,
		unsigned max = 1000,
		const std::string& quote = std::string(),
		const std::string& delimiter = ","
	)
	{
		// ���l�𕶎��񉻂������ɂR������','�������Ă��܂��̂�h�~
		std::stringstream ss;
		ss.imbue( std::locale("Japanese", std::locale::ctype) );

		ElementsStrings strs;

		long count = 1;			///< �񋓐�
		for ( T::const_iterator it = elements.begin(); it != elements.end(); ++it )
		{
			const auto& aID = *it;

			ss << quote << aID << quote;

			// �񋓐����ő�l�ɒB������A�����܂ł̕���������g�̃R���e�i�Ɋi�[���Ďd�؂蒼��
			if (count == max)
			{
				strs.push_back( ss.str() );
				ss.str("");
				ss.clear( std::stringstream::goodbit );
				count = 1;
			}
			// �Ō��ID�łȂ���΃f���~�^����������Ŏ���
			else if (it != boost::prior( elements.end() ))
			{
				ss << delimiter;
				++count;
			}
		}

		// �񋓐����ő�l�ɒB���Ȃ�����ǉ�
		if (! ss.str().empty() )
			strs.push_back( ss.str() );

		return strs;		// ����move�ɂ��R�s�[�R�X�g�͔������Ȃ��i�͂��j
	}

} // namespace uh
