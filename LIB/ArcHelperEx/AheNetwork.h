/**
 * @file AheNetwork.h
 * 
 * @brief SiNDY�f�[�^���f���ɂ�����l�b�g���[�N���f���Ɋւ���O���[�o���֐��Ȃ�
 * ��`����t�@�C���ł��B\n
 * ���݃l�b�g���[�N���f���ƌĂ΂����͈̂ȉ��̒ʂ�F
 * @li ���H�l�b�g���[�N
 * @li ���s�҃l�b�g���[�N�i2007�N�x���J���J�n�j
 * @li �n�����ԏ�l�b�g���[�N�i���݂͓��H�l�b�g���[�N�̈ꕔ�j
 * 
 * @author �R���e���c�{���n�}DB���암�V�X�e���J���O���[�v �Ð�M�G
 * @date 2007/04/20
 * @copy Copyright(c) 2007 INCREMENT P CORP., All Rights Reserved.
 */
#pragma once

class CTargetItem;
template <class T> class CTargetItemsT;
typedef CTargetItemsT<CTargetItem> CTargetItems;

/// �l�b�g���[�N���f���^�C�v
enum sindyeNetworkModel
{
	sindyeNotNetwork,        //!< NW�ł͂Ȃ�
	sindyeNetworkRoad,       //!< ���HNW
	sindyeNetworkWalk,       //!< ���s��NW
	sindyeNetworkSubway,     //!< �n���S�H��NW
	sindyeNetworkRailway,    //!< �S��NW
	sindyeNetworkLane, 		 //!< �Ԑ�NW (ADAM)
	sindyeNetworkUpDown, 	 //!< �㉺��NW (ADAM)
	sindyeNetworkCompart, 	 //!< ����NW (ADAM)
	sindyeNetworkBorder 	 //!< ���H���E��NW (ADAM)
};

/// �l�b�g���[�N���f���̃��C���t�B�[�`���N���X���ǂ����𔻒肷��
bool AheIsNetworkLink( LPCTSTR lpcszTableName );
bool AheIsNetworkLink( ILayer* ipLayer );
bool AheIsNetworkLink( IFeatureClass* ipFeatureClass );
bool AheIsNetworkLink( IFeature* ipFeature );

/// ADAM�l�b�g���[�N���f���̃��C���t�B�[�`���N���X���ǂ����𔻒肷��
bool AheIsAdamNetworkLink(LPCTSTR lpcszTableName);
bool AheIsAdamNetworkLink(ILayer* ipLayer);
bool AheIsAdamNetworkLink(IFeatureClass* ipFeatureClass);
bool AheIsAdamNetworkLink(IFeature* ipFeature);

/// �l�b�g���[�N���f���̃|�C���g�t�B�[�`���N���X���ǂ����𔻒肷��
bool AheIsNetworkNode( LPCTSTR lpcszTableName );
bool AheIsNetworkNode( ILayer* ipLayer );
bool AheIsNetworkNode( IFeatureClass* ipFeatureClass );
bool AheIsNetworkNode( IFeature* ipFeature );

/// ADAM�l�b�g���[�N���f���̃|�C���g�t�B�[�`���N���X���ǂ����𔻒肷��
bool AheIsAdamNetworkNode(LPCTSTR lpcszTableName);
bool AheIsAdamNetworkNode(ILayer* ipLayer);
bool AheIsAdamNetworkNode(IFeatureClass* ipFeatureClass);
bool AheIsAdamNetworkNode(IFeature* ipFeature);

/// �l�b�g���[�N���f�����ǂ������肷��
bool AheIsNetwork( LPCTSTR lpcszTableName );
bool AheIsNetwork( ILayer* ipLayer );
bool AheIsNetwork( IFeatureClass* ipFeatureClass );
bool AheIsNetwork( IFeature* ipFeature );

/// ADAM�l�b�g���[�N���f�����ǂ������肷��
bool AheIsAdamNetwork(LPCTSTR lpcszTableName);
bool AheIsAdamNetwork(ILayer* ipLayer);
bool AheIsAdamNetwork(IFeatureClass* ipFeatureClass);
bool AheIsAdamNetwork(IFeature* ipFeature);

/// �l�b�g���[�N���f���^�C�v����
sindyeNetworkModel AheGetNetWorkType( LPCTSTR lpcszTableName );
sindyeNetworkModel AheGetNetWorkType( ILayer* ipLayer );
sindyeNetworkModel AheGetNetWorkType( IFeatureClass* ipFeatureClass );
sindyeNetworkModel AheGetNetWorkType( IFeature* ipFeature );

/**
 * @brief �l�b�g���[�N���f���Ɋ֘A���邩�ǂ������肷��
 * @note �l�b�g���[�N���f�����g���܂܂�܂�
 */
bool AheIsRelateWithNetwork( LPCTSTR lpcszTableName );
bool AheIsRelateWithNetwork( ILayer* ipLayer );
bool AheIsRelateWithNetwork( IFeatureClass* ipFeatureClass );
bool AheIsRelateWithNetwork( IFeature* ipFeature );

/**
 * @brief �l�b�g���[�N���f���ł̊֘A�e�[�u�����ꗗ���擾����
 * @note AheGetNetworkSplitTarget + AheGetNetworkNodeFeatureClassName
 * @param lpcszTableName [in] �l�b�g���[�N���f���Ɋ֘A����e�[�u�����i���ł��ǂ��j
 * @return �e�[�u�����ꗗ�i�c�_��؂�j
 */
CString AheGetNetworkTarget( LPCTSTR lpcszTableName );

/**
 * @brief �l�b�g���[�N���f���ł̃X�v���b�g�����ɕK�v�ȃ��C���n�e�[�u�����ꗗ���擾����
 * @note CEditHelper::SplitRoadByRoadAndMesh�łǂ����Ă��K�v�������̂ō쐬
 * @param lpcszTableName [in] �l�b�g���[�N���f���Ɋ֘A����e�[�u�����i���ł��ǂ��j
 * @param bContainNW     [in] �ꗗ�Ƀl�b�g���[�N�����N���̂��̂��܂ނ�
 * true�Ȃ� �l�b�g���[�N�����N���̂��̂��܂񂾈ꗗ��Ԃ�(��F|ROAD_LINK|SEGMENT_ATTR|GRAD|)
 * false�Ȃ� �l�b�g���[�N�����N���܂܂Ȃ��ꗗ��Ԃ�(��F|SEGMENT_ATTR|GRAD|) [�f�t�H���g:true]
 * @return �e�[�u�����ꗗ�i�c�_��؂�j
 */
CString AheGetNetworkSplitTarget( LPCTSTR lpcszTableName, bool bContainNW = true );

/**
 * @brief �l�b�g���[�N�����N�t�B�[�`���N���X���̈ꗗ���擾����
 * @note CTargetFeatures::GetNetworkLinkFeatures�łǂ����Ă��K�v�������̂ō쐬
 */
CString AheGetNetworkLinks();

/**
 * @brief �l�b�g���[�N�m�[�h�t�B�[�`���N���X���̈ꗗ���擾����
 * @note CTargetFeatures::GetNetworkNodeFeatures�łǂ����Ă��K�v�������̂ō쐬
 */
CString AheGetNetworkNodes();

/// �l�b�g���[�N�����N�t�B�[�`���N���X����Ԃ�
CString AheGetNetworkLinkFeatureClassName( LPCTSTR lpcszTableName );

/// �l�b�g���[�N�m�[�h�t�B�[�`���N���X����Ԃ�
CString AheGetNetworkNodeFeatureClassName( LPCTSTR lpcszTableName );

/**
 * @brief �l�b�g���[�N�����N�t�B�[�`���N���X��Ԃ�
 * �n���ꂽ�t�B�[�`���Ɋ֘A����l�b�g���[�N���f���̃����N�t�B�[�`���N���X��Ԃ��܂��B
 */
IFeatureClassPtr AheGetNetworkLinkFeatureClass( IFeature* ipFeature );

/**
 * @brief �l�b�g���[�N�m�[�h�t�B�[�`���N���X��Ԃ�
 * �n���ꂽ�t�B�[�`���Ɋ֘A����l�b�g���[�N���f���̃m�[�h�t�B�[�`���N���X��Ԃ��܂��B
 */
IFeatureClassPtr AheGetNetworkNodeFeatureClass( IFeature* ipFeature );

/**
 * @brief �n���ꂽ�e�[�u�����ɕt������֘A�t���O�t�B�[���h���̃��X�g���擾����
 * @param lpcszTableName [in] �֘A�t���O���t���Ă���e�[�u�����ie.x. ROAD_LINK�j
 * @return �֘A�t���O�t�B�[���h���̃��X�g�ie.x. ROAD_LINK �Ȃ� ONWAY_RF �� NOPASSAGE_RF�j
 */
std::list<CString> AheGetRelationFlagFieldName( LPCTSTR lpcszTableName );

/**
 * @biref �֘A�t���O�t�B�[���h���ƑΉ�����O���e�[�u���̊O���L�[�t�B�[���h�����擾����
 * @param lpcszTableName    [in] �֘A�t���O���t���Ă���e�[�u�����ie.x. ROAD_LINK�j
 * @param lpcszRelFieldname [in] �֘A�t���O�t�B�[���h���ie.x. ONWAY_RF�j
 * @return �O���e�[�u���̊O���L�[�t�B�[���h���ie.x. LINK_ID�j
 */
CString AheGetRelationOuterKeyFieldName( LPCTSTR lpcszTableName, LPCTSTR lpcszRelFieldName );

/** �֘A�t���O�t�B�[���h���ƑΉ�����O���e�[�u�������擾����
 * @param lpcszTableName    [in] �֘A�t���O���t���Ă���e�[�u�����ie.x. ROAD_LINK�j
 * @param lpcszRelFieldname [in] �֘A�t���O�t�B�[���h���ie.x. ONWAY_RF�j
 * @return �O���e�[�u�����ie.x. TIME_ONEWAY�j
 */
CString AheGetRelationTableName( LPCTSTR lpcszTableName, LPCTSTR lpcszRelFieldName );

/**
 * @brief �t�B�[�`���Ɋ֘A����O���e�[�u�����I�[�v������
 * @param ipFeature         [in] �x�[�X�t�B�[�`��
 * @param lpcszRelFieldName [in] �֘A�t�B�[���h��
 * @return �֘A�e�[�u��
 */
ITablePtr AheOpenRelationTable( IFeature* ipFeature, LPCTSTR lpcszRelFieldName );

/**
 * @brief �l�b�g���[�N�m�[�h�ɐڑ�����Ă���l�b�g���[�N�����N���擾
 *
 * AheGetConnectedRoad()�Ə����͓���
 */
LONG AheGetConnectedNetworkLink( IFeaturePtr ipFeature, CTargetItems& cTargetItems );

/**
 * @brief �l�b�g���[�N�����N���ڑ����Ă���m�[�h�t�B�[�`�����擾
 *
 * AheGetConnectedNode()�Ə����͓���
 */
LONG AheGetConnectedNetworkNode( IFeaturePtr ipFeature, CTargetItems& cTargetItems );

/**
 * @brief �I�������l�b�g���[�N�����N�̔��Α��̃����N��Ԃ��A���Α��̃m�[�h��2�����b�V�����ӏ�m�[�h���ǂ���
 *
 * AheGetConnectedRoadAndNode()�Ə����͓���
 */
bool AheGetConnectedNetworkLinkAndNode( IFeaturePtr ipSelectRoad, IFeaturePtr ipSelectNode, IFeaturePtr &ipConnectedNetworkLink, IFeaturePtr &ipOppNode );

/**
 * @brief 2�����b�V�����ׂ��ł���l�b�g���[�N�����N���擾����
 *
 * AheIsRoadCrossMesh()�Ə����͓����ŁA�l�b�g���[�N�����N�Ŕ��f�ł���
 */
bool AheIsNetworkLinkCrossMesh( IMapPtr ipMap, IFeaturePtr ipFeature, CTargetItems &cNetworkLinkCrossMesh );

/**
 * @brief Contain���[���֌W�ɂ�����̂̂����֘A������ׂ����̂��擾����
 *
 * ���H�����N�ƃ����N�������̂悤�Ȋ֌W�̏ꍇ�ɁA���m��ȃ����N�������`�󂩂�
 * �֘A���铹�H�����N���擾����ꍇ�Ɏg�p���܂��B
 * ���̍ہA�֘AID�����m��Ȃ��̂Ɣ��f���A�g�p���܂���B
 * 
 * @param ipWithinFeature [in] �u�܂܂��v���̃t�B�[�`��
 * @param cContainFeature [in] �u�܂ށv���̃t�B�[�`���Q
 * @return �֘A������ׂ��Ɣ��f���ꂽ�u�܂ށv���t�B�[�`���Q
 */
CTargetItems AheGetRelationshipContainFeature( IFeaturePtr ipWithinFeature, const CTargetItems& cContainFeatures );

/**
 * @brief ���s��NW�̔ėp���f�����_�E���R���o�[�g���f�����𔻒肷��֐�
 *
 * @note ���̊֐��͕��s��NW�܂��͕��s�҃m�[�h�̎q�_�C�A���O�ł̂ݗ��p���邱��
 *       �_�E���R���o�[�g�ł�2�N�Ŋ������錩���݂̂��߁A���̌�1�N�قǕێ��������
 *       �f�[�^���폜���邱�Ƃ𐄏����A�{�֐����폜����̂��]�܂����B
 *
 * @param ipFeature [in] ���s��NW�܂��͕��s�҃m�[�h
 *
 * @return kDownConvert �_�E���R���o�[�g��
 * @return kGeneral		�ėp��
 */
sindy::schema::walk_type::type_class::ECode AheIsDownconvert( IFeaturePtr ipFeature );