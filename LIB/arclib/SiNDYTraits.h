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

#ifndef __SINDY_TRAITS_H__
#define __SINDY_TRAITS_H__


#include "../sindylib/exportimport.h"
#include "detail/iterator.h"

#define ACCESSOR_CLASS(d) template<> struct traits_base<Interface>::accessor<d>

#define GET_0(D, f) static HRESULT get(Interface* ip, D* pData) { return ip->f(pData); }
#define GET_1(D, f, A1) static HRESULT get(Interface* ip, A1 a1, D* pData) { return ip->f(a1, pData); }
#define GET_2(D, f, A1, A2) static HRESULT get(Interface* ip, A1 a1, A2 a2, D* pData) { return ip->f(a1, a2, pData); }

#define ACCESSOR_GET_0(d, f) ACCESSOR_CLASS(d) { GET_0(d, f); }
#define ACCESSOR_GET_1(d, f, a1) ACCESSOR_CLASS(d) { GET_1(d, f, a1); }
#define ACCESSOR_GET_2(d, f, a1, a2) ACCESSOR_CLASS(d) { GET_2(d, f, a1, a2); }

#define ACCESSOR_GET_INTERFACE_0(d, f) \
	ACCESSOR_GET_0(d::Interface*, f); \
	ACCESSOR_CLASS(d) : public accessor<d::Interface*> {}
#define ACCESSOR_GET_INTERFACE_1(d, f, a1) \
	ACCESSOR_GET_1(d::Interface*, f, a1); \
	ACCESSOR_CLASS(d) : public accessor<d::Interface*> {}
#define ACCESSOR_GET_INTERFACE_2(d, f, a1, a2) \
	ACCESSOR_GET_2(d::Interface*, f, a1, a2); \
	ACCESSOR_CLASS(d) : public accessor<d::Interface*> {}

#define INHERIT_STREAM_CONTAINER(d, f) public stream_container<Interface, d, &Interface::f>
#define INHERIT_INDEX_CONTAINER(d, fg, fc) public index_container<Interface, d, &Interface::fg, &Interface::fc>

/**
 * @brief �X�g���[���^�C���^�t�F�[�X�̑���N���X���`����
 *
 * - �}�N�������B
 *   - @a d : �v�f�^�B
 *   - @a f : �v�f�擾���\�b�h�i NextRow �Ȃǁj�B
 */
#define STREAM_CONTAINER(d, f) \
	ACCESSOR_CLASS(d) : INHERIT_STREAM_CONTAINER(d, f) {};

/**
 * @brief �C���^�t�F�[�X�v�f�𑖍�����A�X�g���[���^�C���^�t�F�[�X�̑���N���X���`����
 *
 * - �}�N�������B
 *   - @a d : �v�f�^�i _com_ptr_t �j�B
 *   - @a f : �v�f�擾���\�b�h�i NextRow �Ȃǁj�B
 */
#define INTERFACE_STREAM_CONTAINER(d, f) \
	STREAM_CONTAINER(d::Interface*, f) \
	ACCESSOR_CLASS(d) : public accessor<d::Interface*> {};

/**
 * @brief �����_���A�N�Z�X�^�C���^�t�F�[�X�̑���N���X���`����
 *
 * - �}�N�������B
 *   - @a d : �v�f�^�B
 *   - @a fg : �v�f�擾���\�b�h�i get_Field �Ȃǁj�B
 *   - @a fc : �v�f���擾���\�b�h�i get_FieldCount �Ȃǁj�B
 */
#define INDEX_CONTAINER(d, fg, fc) \
	ACCESSOR_CLASS(d) : INHERIT_INDEX_CONTAINER(d, fg, fc) {};

/**
 * @brief �C���^�t�F�[�X��v�f�Ƃ��Ă��i�[����A�����_���A�N�Z�X�^�C���^�t�F�[�X�̑���N���X���`����
 *
 * - �}�N�������B
 *   - @a d : �v�f�^�i _com_ptr_t �j�B
 *   - @a fg : �v�f�擾���\�b�h�i get_Field �Ȃǁj�B
 *   - @a fc : �v�f���擾���\�b�h�i get_FieldCount �Ȃǁj�B
 */
#define INTERFACE_INDEX_CONTAINER(d, fg, fc) \
	INDEX_CONTAINER(d::Interface*, fg, fc) \
	ACCESSOR_CLASS(d) : public accessor<d::Interface*> {};

/**
 * @brief �X�g���[���^�̗v�f����N���X�����C���Ƃ���B
 */
#define MAIN_STREAM_CONTAINER(element) \
	typedef accessor<element>::Container Container; \
	typedef accessor<element>::iterator iterator; \
	typedef Container::ElementTraits ElementTraits; \
	typedef ElementTraits::ElementHolder ElementHolder; \
	static iterator make_first(Interface* ipContainer) { return accessor<element>::make_first(ipContainer); } \
	static iterator make_last(Interface* ipContainer) { return accessor<element>::make_last(ipContainer); }

/**
 * @brief �����_���A�N�Z�X�^�̗v�f����N���X�����C���Ƃ���B
 */
#define MAIN_INDEX_CONTAINER(element) \
	typedef accessor<element>::Container Container; \
	typedef accessor<element>::iterator iterator; \
	typedef accessor<element>::reverse_iterator reverse_iterator; \
	typedef Container::ElementTraits ElementTraits; \
	typedef ElementTraits::ElementHolder ElementHolder; \
	static Container make_Container(Interface* ipContainer) { return accessor<element>::make_Container(ipContainer); } \
	static iterator make_first(Interface* ipContainer) { return accessor<element>::make_first(ipContainer); } \
	static iterator make_last(Interface* ipContainer) { return accessor<element>::make_last(ipContainer); } \
	static reverse_iterator make_rfirst(Interface* ipContainer) { return accessor<element>::make_rfirst(ipContainer); } \
	static reverse_iterator make_rlast(Interface* ipContainer) { return accessor<element>::make_rlast(ipContainer); }

namespace sindy {

template <typename TIContainer, typename TElement, HRESULT (__stdcall TIContainer::*TGetNextMethod)(TElement*)>
struct stream_container
{
	static HRESULT get(TIContainer* ip, TElement* pData) { return (ip->*TGetNextMethod)(pData); }

	typedef StreamContainerT<TIContainer, TElement, TGetNextMethod> Container;
	typedef StreamIteratorT<Container> iterator;
	typedef typename Container::ElementTraits ElementTraits;
	typedef typename ElementTraits::ElementHolder ElementHolder;
	static iterator make_first(TIContainer* ipContainer) { return iterator(ipContainer); }
	static iterator make_last(TIContainer*) { return iterator(); }
};

template <
	typename TIContainer,
	typename TElement,
	HRESULT (__stdcall TIContainer::*TGetMethod)(long, TElement*),
	HRESULT (__stdcall TIContainer::*TCountMethod)(long*)>
struct index_container
{
	static HRESULT get(TIContainer* ip, long nIndex, TElement* pData) { return (ip->*TGetMethod)(nIndex, pData); }
	static HRESULT count(TIContainer* ip, long* pCount) { return (ip->*TCountMethod)(pCount); }

	typedef IndexContainerT<TIContainer, TElement, TGetMethod, TCountMethod> Container;
	typedef IndexIteratorT<Container, true> iterator;
	typedef IndexIteratorT<Container, false> reverse_iterator;
	typedef typename Container::ElementTraits ElementTraits;
	typedef typename ElementTraits::ElementHolder ElementHolder;
	static Container make_Container(TIContainer* ipContainer) { return Container(ipContainer); }
	static iterator make_first(TIContainer* ipContainer) { return iterator(ipContainer, 0); }
	static iterator make_last(TIContainer* ipContainer) { return iterator(ipContainer); }
	static reverse_iterator make_rfirst(TIContainer* ipContainer) { return reverse_iterator(ipContainer, 0); }
	static reverse_iterator make_rlast(TIContainer* ipContainer) { return reverse_iterator(ipContainer); }
};

/**
 * @brief SiNDY�������^�̐��^
 */
template <typename T>
struct traits_base
{
	/// �A�N�Z�b�T�������^
	template <typename TElement>
	struct accessor
	{
	};
};

/**
 * @brief �C���^�t�F�[�X�������^
 */
template <typename TInterface>
struct interface_traits : public traits_base<TInterface> { typedef TInterface Interface; };

/// �s�������^
template <typename TRow> struct row_traits {};

template <>
struct row_traits<_IRow>
{
	typedef _IRow RowInterface;
	typedef _IRowPtr RowInterfacePtr;
	typedef _ICursor CursorInterface;
	typedef _ICursorPtr CursorInterfacePtr;
	typedef ITable TableInterface;
	typedef ITablePtr TableInterfacePtr;
	typedef IRowBuffer RowBufferInterface;
	typedef IRowBufferPtr RowBufferInterfacePtr;
};

template <>
struct row_traits<IFeature>
{
	typedef IFeature RowInterface;
	typedef IFeaturePtr RowInterfacePtr;
	typedef IFeatureCursor CursorInterface;
	typedef IFeatureCursorPtr CursorInterfacePtr;
	typedef IFeatureClass TableInterface;
	typedef IFeatureClassPtr TableInterfacePtr;
	typedef IFeatureBuffer RowBufferInterface;
	typedef IFeatureBufferPtr RowBufferInterfacePtr;
};

/// IRow����
template <>
struct pointer_traits<_IRow*> : public interface_traits<_IRow>, public row_traits<_IRow>
{
};

/// IFeature����
template <>
struct pointer_traits<IFeature*> : public interface_traits<IFeature>, public row_traits<IFeature>
{
	ACCESSOR_GET_INTERFACE_0(IGeometryPtr, get_Shape);
};

/// ITable����
template <>
struct pointer_traits<ITable*> : public interface_traits<ITable>, public row_traits<_IRow>
{
	typedef ITable Interface;

	ACCESSOR_GET_INTERFACE_0(IFieldsPtr, get_Fields);
	ACCESSOR_GET_INTERFACE_0(IRowBufferPtr, CreateRowBuffer);
	ACCESSOR_GET_INTERFACE_0(IIndexesPtr, get_Indexes);
};

/// IFeatureClass����
template <>
struct pointer_traits<IFeatureClass*> : public interface_traits<IFeatureClass>, public row_traits<IFeature>
{
	typedef IFeatureClass Interface;

	ACCESSOR_GET_0(esriGeometryType, get_ShapeType);
	ACCESSOR_GET_INTERFACE_0(IFieldsPtr, get_Fields);
	ACCESSOR_GET_INTERFACE_0(IFeatureBufferPtr, CreateFeatureBuffer);
	ACCESSOR_GET_INTERFACE_0(IIndexesPtr, get_Indexes);
};

/// ICursor����
template <>
struct pointer_traits<_ICursor*> : public interface_traits<_ICursor>
{
	INTERFACE_STREAM_CONTAINER(_IRowPtr, NextRow);
	MAIN_STREAM_CONTAINER(_IRow*);
	static HRESULT (__stdcall Interface::*insert())(IRowBuffer*, VARIANT*) { return _ICursor::InsertRow; }
};

/// IFeatureCursor����
template <>
struct pointer_traits<IFeatureCursor*> : public interface_traits<IFeatureCursor>
{
	INTERFACE_STREAM_CONTAINER(IFeaturePtr, NextFeature);
	MAIN_STREAM_CONTAINER(IFeature*);
	static HRESULT (__stdcall Interface::*insert())(IFeatureBuffer*, VARIANT*) { return IFeatureCursor::InsertFeature; }
};

/// IDataset����
template <>
struct pointer_traits<IDataset*> : public interface_traits<IDataset>
{
	ACCESSOR_GET_INTERFACE_0(IWorkspacePtr, get_Workspace);
};

/// IEnumDataset����
template <>
struct pointer_traits<IEnumDataset*> : public interface_traits<IEnumDataset>
{
	INTERFACE_STREAM_CONTAINER(IDatasetPtr, Next);
	MAIN_STREAM_CONTAINER(IDataset*);
};

/// IEnumDatasetName����
template <>
struct pointer_traits<IEnumDatasetName*> : public interface_traits<IEnumDatasetName>
{
	INTERFACE_STREAM_CONTAINER(IDatasetNamePtr, Next);
	MAIN_STREAM_CONTAINER(IDatasetName*);
};

/// IField����
template <>
struct pointer_traits<IField*> : public interface_traits<IField>
{
	ACCESSOR_GET_0(esriFieldType, get_Type);
	ACCESSOR_GET_INTERFACE_0(IGeometryDefPtr, get_GeometryDef);
	ACCESSOR_GET_INTERFACE_0(IDomainPtr, get_Domain);
};

/// IFields����
template <>
struct pointer_traits<IFields*> : public interface_traits<IFields>
{
	INTERFACE_INDEX_CONTAINER(IFieldPtr, get_Field, get_FieldCount);
	MAIN_INDEX_CONTAINER(IField*);
};

/// ICodedValueDomain����
template <>
struct pointer_traits<ICodedValueDomain*> : public interface_traits<ICodedValueDomain>
{
	INDEX_CONTAINER(VARIANT, get_Value, get_CodeCount);
	MAIN_INDEX_CONTAINER(VARIANT);
};

/// IGeometry����
template <>
struct pointer_traits<IGeometry*> : public interface_traits<IGeometry>
{
	ACCESSOR_GET_0(esriGeometryType, get_GeometryType);
	ACCESSOR_GET_INTERFACE_0(IEnvelopePtr, get_Envelope);
	ACCESSOR_GET_INTERFACE_0(ISpatialReferencePtr, get_SpatialReference);
};

/// IGeometryCollection����
template <>
struct pointer_traits<IGeometryCollection*> : public interface_traits<IGeometryCollection>
{
	INTERFACE_INDEX_CONTAINER(IGeometryPtr, get_Geometry, get_GeometryCount);
	MAIN_INDEX_CONTAINER(IGeometry*);
};

/// IPoint����
template <>
struct pointer_traits<IPoint*> : public interface_traits<IPoint>
{
	ACCESSOR_GET_INTERFACE_0(ISpatialReferencePtr, get_SpatialReference);
};

/// IPointCollection����
template <>
struct pointer_traits<IPointCollection*> : public interface_traits<IPointCollection>
{
	INTERFACE_INDEX_CONTAINER(IPointPtr, get_Point, get_PointCount);
	MAIN_INDEX_CONTAINER(IPoint*);
};

/// IWorkspace����
template <>
struct pointer_traits<IWorkspace*> : public interface_traits<IWorkspace>
{
	ACCESSOR_GET_INTERFACE_1(IEnumDatasetPtr, get_Datasets, esriDatasetType);
};

/// IFeatureWorkspace����
template <>
struct pointer_traits<IFeatureWorkspace*> : public interface_traits<IFeatureWorkspace>
{
	ACCESSOR_GET_INTERFACE_1(ITablePtr, OpenTable, BSTR);
	ACCESSOR_GET_INTERFACE_1(IFeatureClassPtr, OpenFeatureClass, BSTR);
};

/// IWorkspaceDomains����
template <>
struct pointer_traits<IWorkspaceDomains*> : public interface_traits<IWorkspaceDomains>
{
	ACCESSOR_GET_INTERFACE_0(IEnumDomainPtr, get_Domains);
};

/// IEnumDomain����
template <>
struct pointer_traits<IEnumDomain*> : public interface_traits<IEnumDomain>
{
	INTERFACE_STREAM_CONTAINER(IDomainPtr, Next);
	MAIN_STREAM_CONTAINER(IDomain*);
};

/// IMap����
template <>
struct pointer_traits<IMap*> : public interface_traits<IMap>
{
	INTERFACE_INDEX_CONTAINER(ILayerPtr, get_Layer, get_LayerCount);
	ACCESSOR_GET_INTERFACE_2(IEnumLayerPtr, get_Layers, IUID*, VARIANT_BOOL);
};

/// ICompositeLayer����
template <>
struct pointer_traits<ICompositeLayer*> : public interface_traits<ICompositeLayer>
{
	ACCESSOR_CLASS(ILayer*) : INHERIT_INDEX_CONTAINER(ILayer*, get_Layer, get_Count)
	{
		class sole_iterator
		{
		public:
		// �\�z/����
			sole_iterator()
			{
			}

			sole_iterator(ILayer* ipLayer)
			{
				if(! nest(ipLayer)) {
					m_Element = ipLayer;
				}
			}

			sole_iterator(ICompositeLayer* ipCompositeLayer)
			{
				nest(ipCompositeLayer);
			}


		// �����q�̋���
			sole_iterator& operator++()
			{
				m_Element.Release();

				if(! m_IteratorList.empty()) {
					// ���[�̔����q���P�i�߂Ȃ���擾
					Iterator& rIterator = ++*m_IteratorList.rbegin();

					// �����q���I�[�ɒB�����甽���q��؂�ւ���
					if(rIterator == last_iterator()) {
						m_IteratorList.pop_back();
						operator++();
					}
					else {
						getElement(rIterator);
					}
				}

				return *this;
			}

			bool operator==(const last_iterator)
			{
				return m_Element == 0;
			}

			bool operator!=(const last_iterator it)
			{
				return ! operator==(it);
			}

			ILayer* operator*()
			{
				return m_Element;
			}

		private:
		// ����J��`
			typedef IndexIteratorIT<Container, true> Iterator;

			typedef std::vector<Iterator> IteratorList;

		// ����J����
			bool nest(ILayer* ipLayer)
			{
				return nest(ICompositeLayerPtr(ipLayer));
			}

			bool nest(ICompositeLayer* ipCompositeLayer)
			{
				// �������C���łȂ��Ȃ�΁A����q��͕͂K�v�Ȃ�
				if(ipCompositeLayer == 0)
					return false;

				// �������C�����������锽���q�𖖒[�ɒǉ�
				m_IteratorList.push_back(Iterator(ipCompositeLayer));

				Iterator& rIterator = *m_IteratorList.rbegin();

				if(rIterator != last_iterator()) {
					// ����Ȃ����q��
					getElement(rIterator);
				}

				return true;
			}

			void getElement(Iterator& rIterator)
			{
				ILayer* ipLayer;
				rIterator.getElement(&ipLayer);

				if(nest(ipLayer)) {
					ipLayer->Release();
				}
				else {
					m_Element.Attach(ipLayer);
				}
			}

		// ����J�ϐ�
			IteratorList m_IteratorList;
			ILayerPtr m_Element;
		};

		static sole_iterator make_sole_first(ICompositeLayer* pContainer) { return sole_iterator(pContainer); }
		static sole_iterator make_sole_first(ILayer* pElement) { return sole_iterator(pElement); }
		static sole_iterator make_sole_last() { return sole_iterator(); }
	};
	ACCESSOR_CLASS(ILayerPtr) : public accessor<ILayer*> {};

};

/// ILayer����
template <>
struct pointer_traits<ILayer*> : public interface_traits<ILayer>
{
	ACCESSOR_CLASS(ILayer*) : public pointer_traits<ICompositeLayer*>::accessor<ILayer*> {};
	ACCESSOR_CLASS(ILayerPtr) : public accessor<ILayer*> {};

	typedef accessor<ILayer*>::sole_iterator sole_iterator;
	static sole_iterator make_sole_first(ICompositeLayer* pContainer) { return accessor<ILayer*>::sole_iterator(pContainer); }
	static sole_iterator make_sole_first(ILayer* pElement) { return accessor<ILayer*>::sole_iterator(pElement); }
	static sole_iterator make_sole_last() { return accessor<ILayer*>::sole_iterator(); }
};

/// IFeatureLayer����
template <>
struct pointer_traits<IFeatureLayer*> : public interface_traits<IFeatureLayer>, public row_traits<IFeature>
{
	ACCESSOR_GET_INTERFACE_0(IFeatureClassPtr, get_FeatureClass);
};

/// IEnumLayer����
template <>
struct pointer_traits<IEnumLayer*> : public interface_traits<IEnumLayer>
{
	INTERFACE_STREAM_CONTAINER(ILayerPtr, Next);
	MAIN_STREAM_CONTAINER(ILayer*);
};

/// IActiveView ����
template <>
struct pointer_traits<IActiveView*> : public interface_traits<IActiveView>
{
	ACCESSOR_GET_INTERFACE_0(IScreenDisplayPtr, get_ScreenDisplay);
};

/// IIndexes����
template <>
struct pointer_traits<IIndexes*> : public interface_traits<IIndexes>
{
	INTERFACE_INDEX_CONTAINER(IIndexPtr, get_Index, get_IndexCount);
	MAIN_INDEX_CONTAINER(IIndex*);
};

/// IClone����
template <>
struct pointer_traits<IClone*> : public interface_traits<IClone>
{
	ACCESSOR_GET_INTERFACE_0(IClonePtr, Clone);
};

// ���낢��

template <typename T> struct row_ptr_t_of { typedef T::RowInterfacePtr type; };
template <typename T> struct cursor_ptr_t_of { typedef T::CursorInterfacePtr type; };
template <typename T> struct iterator_of { typedef T::iterator type; };
template <typename T> struct element_holder_of { typedef T::ElementHolder type; };
template <typename T> struct interface_of { typedef T::Interface type; };

template <typename TContainer>
iterator_of< container_traits<TContainer> >::type begin(TContainer cContainer)
{
	return container_traits<TContainer>::make_first(cContainer);
}

/**
 * @defgroup getData �f�[�^�擾�֐��Q�B
 * @a TData �� IFieldPtr �Ȃǂ̃X�}�[�g�|�C���^���w�肷�邱�Ƃ��\�B
 */
/*@{*/

/**
 * @brief �C���^�t�F�[�X�|�C���^����A�����Ȃ��Œl���擾
 *
 * @param pInterface [in] �C���^�t�F�[�X�|�C���^�B�X�}�[�g�|�C���^��A���̔h���ł��B
 * @return �l�B
 */
template <typename TData, typename TInterface>
inline TData getData(TInterface pInterface)
{
	TData aData;
	traits<TInterface>::accessor<TData>::get(pInterface, &aData);
	return aData;
};

/**
 * @brief �C���^�t�F�[�X�|�C���^����A����1�Œl���擾
 *
 * @param pInterface [in] �C���^�t�F�[�X�|�C���^�B�X�}�[�g�|�C���^��A���̔h���ł��B
 * @param cArg1 [in] ����1�B
 * @return �l�B
 */
template <typename TData, typename TArg1, typename TInterface>
inline TData getData(TInterface pInterface, TArg1 cArg)
{
	TData aData;
	traits<TInterface>::accessor<TData>::get(pInterface, cArg, &aData);
	return aData;
};

/**
 * @brief �C���^�t�F�[�X�|�C���^����A����2�Œl���擾
 *
 * @param pInterface [in] �C���^�t�F�[�X�|�C���^�B�X�}�[�g�|�C���^��A���̔h���ł��B
 * @param cArg1 [in] ����1�B
 * @param cArg2 [in] ����2�B
 * @return �l�B
 */
template <typename TData, typename TArg1, typename TArg2, typename TInterface>
inline TData getData(TInterface pInterface, TArg1 cArg1, TArg2 cArg2)
{
	TData aData;
	traits<TInterface>::accessor<TData>::get(pInterface, cArg1, cArg2, &aData);
	return aData;
};

/**
 * @brief �C���^�t�F�[�X�|�C���^���i�[���Ă���v�f�̐������߂�
 *
 * @param pInterface [in] �C���^�t�F�[�X�|�C���^�B�X�}�[�g�|�C���^��A���̔h���ł��B
 * @return �v�f���B
 */
template <typename TData, typename TInterface>
inline long countElement(TInterface pInterface, TData* = 0)
{
	long nCount;
	traits<TInterface>::accessor<TData>::count(pInterface, &nCount);
	return nCount;
};

/*@}*/

template <typename T>
cursor_ptr_t_of< traits<T> >::type search(T const& ipInterface, IQueryFilter* ipQueryFilter, VARIANT_BOOL vbRecycling)
{
	cursor_ptr_t_of< traits<T> >::type ipCursor;
	ipInterface->Search(ipQueryFilter, vbRecycling, &ipCursor);
	return ipCursor;
}

} // namespace sindy

#undef ACCESSOR_CLASS
#undef GET_0
#undef GET_1
#undef GET_2
#undef ACCESSOR_GET_0
#undef ACCESSOR_GET_1
#undef ACCESSOR_GET_2
#undef ACCESSOR_GET_INTERFACE_0
#undef ACCESSOR_GET_INTERFACE_1
#undef ACCESSOR_GET_INTERFACE_2
#undef STREAM_CONTAINER
#undef INTERFACE_STREAM_CONTAINER
#undef INDEX_CONTAINER
#undef INTERFACE_INDEX_CONTAINER
#undef MAIN_STREAM_CONTAINER
#undef MAIN_INDEX_CONTAINER

#endif // __SINDY_TRAITS_H__
