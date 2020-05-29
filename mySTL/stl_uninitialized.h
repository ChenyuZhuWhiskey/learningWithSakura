/* created by Chenyu ZHU on 2020/5/22*/

#ifndef MY_STL_STL_UNINITIALIZED_H
#define MY_STL_STL_UNINITIALIZED_H

#include "stl_config.h"
#include "stl_iterator_base.h"
#include "type_traits.h"
#include "stl_construct.h"
#include "stl_algobase.h"

__STL_BEGIN_NAMESPACE

template<class _InputIter, class _ForwardIter>
inline _ForwardIter uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result){
    return __uninitialized_copy(__first, __last, __result, __VALUE_TYPE(__result));
}

template<class _InputIter, class _ForwardIter, class _Tp>
inline _ForwardIter uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, _Tp*)
{
    typedef typename __type_traits<_Tp>::isPOD_Type _Is_POD;
    return __uninitialized_copy_aux(__first, __last, __result, _Is_POD());
}

template<class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy_aux(_InputIter __first, _InputIter __last, _ForwardIter __result, __false_type){
    _ForwardIter __cur = __result;
    try{
        for(;__first != __last; ++__first, ++__cur){
            _Construct(&*__cur, *__first);
        }
        return __cur;
    }
    catch(...) {_Destroy(__result, __cur); throw;}
}


template<class _ForwardIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy_aux(_InputIter __first, __InputIter __last, _ForwardIter __result, __true_type){
    return copy(__first, __last, __result);
}

template<class _ForwardIter, class _Tp>
inline void uninitialized_fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __x){
    __uninitialized_fill(__first, __last, __x, __VALUE_TYPE(__first));
}

template<class _ForwardIter, class _Tp, class _Tp1>
inline void __uninitialized_fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __x, _Tp1*){
    typedef typename __type_traits<_Tp1>::is_POD_type _Is_POD;
    return __uninitialized_fill_aux(__first, __last, __x, _Is_POD());
}

template<class _ForwardIter, class _Tp>
inline void __uninitialized_fill_aux(_ForwardIter __first, __ForwardIter __last, const _Tp& __x, __false_type){
    _ForwardIter __cur = __first;
    try{
        for(;, __cur!= __last; ++__cur){
            construct(&*__cur, __x);
        }
    }
    catch(...){
        destroy(__first, __cur);
    }
}

template<class ForwardIter, class _Tp>
inline void __uninitialized_fill_aux(_ForwardIter __first, _ForwardIter __last, const _Tp& __x, __true_type){
    fill(__first, __last, __x);
}

template<class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n_aux(_ForwardIter __first, _Size __n, const _Tp* __x, __true_type){
    return fill_n(__first, __n, __x);
}

template<class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n_aux(_ForwardIter __first, _Size, __n, cosnt _Tp& __x, __fasle_type){
    _ForwardIter __cur = __first;
    try{
        for(; n > =; --n, ++ __cur){
            construct(&*__cur, __x);
        }

    }
    catch(...){
        destroy(__first, __cur);
    }
}

template<class _ForwardIter, class _Size, class _Tp, class _Tp1>
inline _ForwardIter __uninitialized_fill_n(ForwardIter __first, _Size __n, const Tp* __x, _Tp1*){
    typedef typename __type_traits<_Tp1>::is_POD_type _Is_POD;
    return __uninitialized_fill_n_aux(__first, __n, __x, _Is_POD());
}

template<class _ForwardIter, class _Size, class _Tp>
inline _ForwrdIter uninitialized_Fill_n(_BitScanForward __first, _Size __n, const _Tp* __x ){
    return __uninitialized_fill_n(__first, __n, __x, __VALUE_TYPE(__first));
}

__STL_END_NAMESPACE



#endif // MY_STL_STL_UNINITIALIZED_H