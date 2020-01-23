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

#include <map>
#include <set>
#include "RowSet.h"
#include "ADLib.h"

namespace adlib
{
	namespace road_link_set
	{
		using namespace row_set;

		typedef IFeaturePtr RoadLink;

		// FromNodeID�܂���ToNodeID�ɂ�錟���p�R���e�i�̌^�̒�`
		typedef std::multimap<NodeID, RoadLink> NodeIDTable;
		typedef std::pair<NodeIDTable::iterator, NodeIDTable::iterator> NodeIDTableRange;

		// From/To�m�[�hID�̑g�ɂ�錟���p�R���e�i�̌^�̒�`
		typedef NodeID FromNodeID;
		typedef NodeID ToNodeID;
		typedef std::pair<FromNodeID, ToNodeID> FromToNodeID;
		typedef std::map<FromToNodeID, RoadLink> FromToNodeIDTable;

		typedef std::pair<FromNodeID, ToNodeID> NodeIDPair;

		/**
		 * @brief RowSet�������N�p�ɓ��������N���X
		 * @note	�o�T�FSiNDYQToMPQ
		 */
		class RoadLinkSet : public RowSet
		{
		public:
			RoadLinkSet(void);
			~RoadLinkSet(void);

			/**
			 * @brief From�m�[�hID�ɂ�錟��
			 * @param[in]	cNodeID	From�m�[�hID
			 */
			NodeIDTableRange searchByFromNodeID(long cNodeID) const;

			/**
			 * @brief To�m�[�hID�ɂ�錟��
			 * @param[in]	cNodeID	To�m�[�hID
			 */
			NodeIDTableRange searchByToNodeID(long cNodeID) const;

			/**
			 * @brief From/To�m�[�hID�̑g�ɂ�錟��
			 * @note	�^����ꂽ�Q�̃m�[�h��From/To�m�[�h�Ƃ��郊���N��Ԃ�
			 * @param[in]	cNodeID1	From�܂���To�m�[�hID
			 * @param[in]	cNodeID2	From�܂���To�m�[�hID
			 */
			IFeaturePtr searchByFromToNodeID(long cNodeID1, long cNodeID2) const;

			/**
			 * @brief From�܂���To�m�[�hID�ɂ�錟��
			 * @note	�^����ꂽ�m�[�hID�����m�[�h��From�܂���To�m�[�h�Ƃ��郊���N�Q��ID��Ԃ�
			 * @param[in]	cNodeID	�Ώۃm�[�hID
			 * @return	�����NID�Q
			 */
			LinkIDs searchByNodeID( long cNodeID ) const;

			/**
			 * @brief �S�R���e�i�̑S�v�f���폜
			 */
			void clear(void);

			/**
			 * @brief �����NID����From/To�m�[�hID���擾
			 * @param[in]	cOID	�����N�̃I�u�W�F�N�gID
			 * @return	�������������N��From/To�m�[�hID�i������Ȃ��ꍇ��(0,0)�̃y�A��Ԃ��j
			 */
			NodeIDPair getFromToNodeID( long cOID ) const;

			/**
			 * �e�X�g�p RoadLinkSet�̒��g���w��X�g���[���Ƀ_���v����
			 */
			void dump(std::ostream& co) const;

		private:
			mutable NodeIDTable m_FromNodeIDTable;			//!< From�m�[�hID�ɂ�錟���p�R���e�i
			mutable NodeIDTable m_ToNodeIDTable;			//!< To�m�[�hID�ɂ�錟���p�R���e�i
			mutable FromToNodeIDTable m_FromToNodeIDTable;	//!< From/To�m�[�hID�̑g�ɂ�錟���p�R���e�i
		};
	} // namespace road_link_set
} // namespace adlib
