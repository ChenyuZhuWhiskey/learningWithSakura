/* created by Chenyu ZHU on 2020/5/22*/

#ifndef MY_STL_STL_VECTOR_H
#define MY_STL_STL_VECTOR_H

#include "stl_config.h"
#include "stl_alloc.h"
#include "stl_iterator.h"
#include "stl_uninitialized.h"

__STL_BEGIN_NAMESPACE

template<class _Tp, class _Alloc>
class _Vector_base{
public:
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type() ;}

    _Vector_base(const _Alloc&)
    :_M_start(0), _M_finish(0), _M_end_of_storage(0) {}

    _Vector_base(size_t __n, const _Alloc&)
    :_M_start(0), _M_finish(0), _M_end_of_storage(0){
        _M_start = _M_allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + __n;
    }

    _Vector_base() {_M_deallocate (_M_start, _M_end_of_storage - _M_start) ;}



protected:
    _Tp *_M_start;
    _Tp *_M_finish;
    _Tp *_M_end_of_storage;

    typedef simple_alloc<_Tp, _Alloc> _M_data_allocator;

    _Tp* _M_allocate(size_t __n){
        return _M_data_allocator::allocate(__n);
    }

    void _M_deallocate(_Tp* __p, size_t __n){
        _M_data_allocator::deallocate(__p, __n);
    }

};

template<class _Tp, class _Alloc = alloc>
class vector:protected _Vector_base<_Tp, _Alloc>{
private:
    typedef _Vector_base<_Tp, _Alloc> _Base;

public:
    typedef _Tp value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type* iterator;
    typedef const value_type* const_iterator;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef typename _Base::allocator_type allocator_type;

    allocator_type get_allocator() const {return _Base::get_allocator() ;}

    typedef reverse_iterator<const_iterator> const_reverse_iterator;
    typedef reverse_iterator<iterator> reverse_iterator;

protected:
    using _Base::_M_allocate;
    using _Base::_M_deallocate;
    using _Base::_M_end_of_storage;
    using _Base::_M_finish;
    using _Base::_M_start;

protected:

    void _M_insert_aux(iterator __position, const _Tp &__x);
    void _M_inser_aux(iterator __position);

public:
    iterator begin(){
        return _M_start;
    }

    const_iterator begin() const {
        return _M_start;
    }

    const_iterator end() const {
        return _M_finish;
    }

    iterator end() {
        return _M_finish;
    }

    iterator rbegin(){
        return reverse_iterator(end());
    }

    const_iterator rbegin() const{
        return reverse_iterator(end());
    }

    reverse_iterator rend(){
        return reverse_iterator(begin());
    }

    cosnt_reverse_iterator rend() const{ 
        return reverse_iterator(begin());
    }

    size_type size() const {
        return size_type(end() - begin());
    }

    size_type max_size() const {
        return size_type(-1) / sizeof(_Tp);
    }

    size_type cpacity() const {
        return size_type(_M_end_of_storage - begin());
    }

    bool empty() const {
        return begin() == end();
    }

    reference operator[](size_type __n){
        return *(begin() + __n);
    }

    const_reference operator[](size_type __n) const {
        return *(begin() + __n);
    }

    void _M_range_check(size_type __n) const{
        if(__n >= size())
            //todo
        {throw;}
    }

    reference at(size_type __n){
        _M_range_check(__n);
        return (*this)[__n];
    }

    const_reverse_iterator at(size_type __n) const {
        _M_range_check(__n);
        return (*this)[__n];
    }

    explicit vector(const allocator_type& __a = allocator_type()): _Base(__a) {}

    explicit vector(size_type __n)
    :_Base(__n, allocator_type()){
        _M_finish = uninitilied_fill_n(_M_start, __n, _Tp(),);
    }

    vector(size_tpte __n, const _Tp &__value, const allocator_type& __a = allocator_type()) : _Base(__n, __a){
        _M_finish = uninitilized_fill_n(_M_start, __n, __value);
    }

    explicit vector(const vector<_Tp, _Alloc> &__x,):_Base(__x.size(),__x,get_allocator()){
        _M_finish = uninitilied_copy(__x.begin(), __x.end(), _M_start);
    }

    template<class _InputIterator>
    vector(_InputIterator __first, _InputIterator __last, const allocator_type &__a = allocator_type())
    :_Base(__a)
    {
        typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
        _M_initilize_aux(__first, __last, _Integral());
    }

    template<class _Integer>
    void _M_initilize_aux(_Integer __n, _Integer __value, __true__type){
        _M_start = _M_allocate(__n);
        _M_end_of_storage = _M_start + __n;
        _M_finish = uninitilied_fill_n(_M_start, __n, __value);
    }

    template<class _InputIterator>
    void _M_initilize_aux(_Integer __n, _Integer __value, __false__type){
        _M_range_initilize(__first, __last, __ITERATOR_CATEGORY(__first));
    }



    ~vector(){
        destroy(_M_start, _M_finish);
    }

    vector<_Tp, _Alloc> &operator=(const vector<_Tp, _Alloc> &__x);

    void reserve(size_type __n) {
        //增加 vector 的容量到大于或等于 new_cap 的值
        if (capacity() < __n) {
            const size_type __old_size = size();
            iterator __tmp = _M_allocate_and_copy(__n, _M_start, _M_finish);
            destroy(_M_start, _M_finish);
            _M_deallocate(_M_start, _M_end_of_storage - _M_start);
            _M_start = __tmp;
            _M_finish = __tmp + __old_size;
            _M_end_of_storage = _M_start + __n;
        }
    }

    // generalized assignment member function.
    void assign(size_type __n, const _Tp &__val){
        _M_fill_assign(__n, __val);
    }

    void _M_fill_assign(size_type __n, const _Tp &__val);

    template<class _InputIterator>
    void assign(_InputIterator __first, _InputIterator __last){
        typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
        _M_assign_dispatch(__first, __last, _Integral());
    }

    template<class _InputIterator>
    void _M_assign_dispatch(_InputIterator __first, _InputIterator __last, __false_type){
        _M_assign_aux(__first, __last, __ITERATOR_CATEGORY(__first));
    }

    template<class _InputIterator>
    void _M_assign_aux(_InputIterator __first, _InputIterator __last, input_iterator_tag);

    template<class _InputIterator>
    void _M_assign_aux(_InputIterator __first, _InputIterator __last, forward_iterator_tag);


    reference front(){
        return *begin();
    }

    const_reference front() const{
        return *begin();
    }

    reference back(){
        return *(end() -1);
    }
    
    const_reference back() const{
        return *(end() -1);
    }

    void push_back(const _Tp &__value){
        if(_M_finish != _M_end_of_storage){
            construct(_M_finish, __value);
            ++_M_finish;
        }
        else{
            _M_insert_aux(end(), __value);
        }
    }

    void push_back(){
        if(_M_finish != _M_end_of_storage){
            construct(_M_finish);
            ++_M_finish;
        }
        else{
            _M_insert_aux(end());
        }
    }

    void swap(vector<_Tp, _Alloc> &__x){
        std::swap(_M_start, __x._M_start);
        std::swap(_M_finish, __x._M_finish);
        std::swap(_M_end_of_storage, __x._M_end_of_storage);
    }


    iterator insert(iterator __position, const _Tp & __x){
        size_type __n = __position - begin();

        if(_M_finish != _M_end_of_storage && __position == end()){
            construct(_M_finish, __x);
            ++_M_finish;
        }
        else{
            _M_insert_aux(__position, __x);
        }

        return begin() + __n;
    }

    iterator insert(iterator __position){
        size_type __n = __position - begin();
        if(_M_finish != _M_end_of_storage && __position == end()){
            construct(_M_finish);
            ++_M_finish;
        }
        else
        {
                _M_insert_aux(__position);
        }
        return begin() + __n;
        
    }

    template<class _InputIterator>
    void insert(iterator __pos, _InputIterator __first, _InputIterator __last){
        typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
        _M_insert_dispatch(__pos, __first, __last, _Integral());
    }

    template<class _Integer>
    void _M_insert_dispatch(iterator __pos, _Integer __n, _Integer __val, __true_type){
        _M_fill_insert(__pos, (size_type) __n, (_Tp) __val);
    }

    template<class _InputIterator>
    void _M_insert_dispatch(iterator __pos, _InputIterator __first, _InputIterator __last, __false_type){
        _M_range_insert(__pos,__first,__last,__ITERATOR_CATEGORY(__first));
    }

    void insert(iterator __position, const_iterator __first, const_iterator __lase);

    void insert(iterator __pos, size_type __n, const _Tp& __x){
        _M_fil_insert(__pos, __n, __x);
    }

    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);

    void pop_back(){
        --_M_finish;
        destroy(_M_finish);
    }

    iterator erase(iterator __position){
        if(__position + 1 != end()){
            copy(__position + 1, _M_finish, __position);
        }
        --_M_finish;
        destroy(_M_finish);
        return __position;
    }

    iterator erase(iterator __first, iterator __last){
        iterator __i = copy(__last, _M_finish, __first);
        destroy(__i, _M_finish);
        _M_finish = _M_finish - (__last - __first);
    }

    void resize(size_type __new_size, const _Tp& __x){
        if(__new_size < size())
            erase(begin() + __new_size, end());
        else
        {
            insert(end(), __new_size - size(), __x);
        }

        
    }

    inline void resize(size_type __new_size){return resize(__new_size, _Tp());}
    inline void clear() {erase(begin(), end());}

protected:

    iterator _M_allocate_and_copy(size_type __n, const_iterator __first, 
                                                 const_iterator __last)
    {
        iterator __result = _M_allocate(__n);
        try{
            uninitialized_copy(__first, __last, __result);
            return __result;
        }
        catch(...){
            _M_deallocate(__result, __n);
        }
    }

    template<class _InputIterator>
    void _M_range_initialize(_InputIterator __first, _InputIterator __last, input_iterator_tag){
        for(; __first != __last; ++__first){
            push_back(*__first);
        }
    }

    template<class _ForwardIterator>
    void _M_range_initialize(_ForwardIterator __first, _ForwardIterator __last, forward_iterator_tag){
        size_type __n = 0;
        distance(__first, __last, __n);
        _M_start = _M_allocate(__n);
        _M_end_of_storage = _M_start + __n;
        _M_finish = uninitialized_copy(__first, __last, _M_start);
    }

    template<class _InputIterator>
    void _M_range_insert(iterator __pos, _InputIterator __first, _InputIterator __last, input_iterator_tag);

    template<class _ForwardIterator>
    void _M_range_insert(iterator __pos, _ForwardIterator __first, _ForwardIterator __last, forward_iterator_tag);


};

template<class _Tp, class _Alloc>
inline bool 
operator==(const vector<_Tp, _Alloc>& __lhs, const vector<_Tp, _Alloc>& __rhs){
    return __lhs.size() == __rhs.size() && equal(__lhs.begin(), __lhs.end(), __rhs,begin();
}

template<class _Tp, class _Alloc>
inline bool 
operator<(const vector<_Tp, _Alloc>& __lhs, const vector<_Tp, _Alloc>& __rhs){
    return lexicographical_compare(__lhs.begin(), __lhs.end(), __rhs.begin(), __rhs.end());
}

template<class _Tp, class _Alloc>
inline void swap(vector<_Tp, _Alloc>& __x, vector<_Tp, _Alloc>& __y){
    __x.swap(__y);
}

template<class _Tp, class _Alloc>
inline bool
operator!=(const vector<_Tp, _Alloc>& __lhs, const vector<_Tp, _Alloc>& __rhs){
    return !(__lhs == __rhs);
}

template<class _Tp, class _Alloc>
inline bool 
operator>(const vector<_Tp, _Alloc>& __lhs, const vector<_Tp, _Alloc>& __rhs){
    return  __rhs > __lhs;
}

template<class _Tp, class _Alloc>
inline bool 
operator>=(const vector<_Tp, _Alloc>& __lhs, const vector<_Tp, _Alloc>& __rhs){
    return  !(__rhs < __lhs);
}

template<class _Tp, class _Alloc>
inline bool 
operator<=(const vector<_Tp, _Alloc>& __lhs, const vector<_Tp, _Alloc>& __rhs){
    return  !(__rhs > __lhs);
}

template<class _Tp, class _Alloc>
vector<_Tp, _Alloc> &operator=(const vector<_Tp, _Alloc> &__x)
{
    if(&__x != this){
        const size_type __xlen = __x.size();
        if(__xlen > capacity()){
            iterator __tmp = _M_allocate_and_copy(__xlen, __x.begin(), __x.end());
            destroy(_M_start, _M_finish);
            _M_deallocate(_M_start, _M_end_of_storage - _M_start);
            _M_start = __tmp;
            _M_end_of_storage = _M_start + __xlen;
        }
        else if (size() >= __xlen){
            iterator __i = copy(__x.begin(),__x.end(), begin());
            destroy(__i, _M_finish);
        }
        else{
            copy(__x.begin(), __x.begin() + size(), _M_start);
            uninitialized_copy(__x.begin() + size(), __x.end(), _M_finish);
        }
        _M_finish = _M_start + __xlen;
    }
    return *this;
}


template<class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_assign(size_type __n, const value_type& __val){
    if(__n > capacity()){
        vector<_Tp, _Alloc> __tmp(__n, __val, get_allocator());
        __tmp.swap(*this);
    }
    else if(__n > size()){
        fill(begin(),end(), __val);
        _M_finish = uninitialized_fill_n(_M_finish, __n - size(), __val); 
    }
    else{
        erase(fill_n(begin(), __n, __val), end());
    }
}

template<class _Tp, class _Alloc> template<class _ForwardIter>
void vector<_Tp, _Alloc>::_M_assign_aux(_ForwardIter __first, _ForwardIter __last, forward_iterator_tag){
    size_type __len = 0;
    distance(__first, __last, __len);
    if(__len > capacity()){
        iterator __tmp = _M_allocate_and_copy(__len, __first, __last);
        destroy(_M_start, _M_finish);
        _M_deallocate(_M_start, _M_end_of_storage - _M_start);
        _M_start = __tmp;
        _M_end_Of_storage = _M_finish = _M_start + __len;
    }
    else if (size() >= __len){
        iterator __new_finish = copy(__first, __last, _M_start);
        destroy(__new_finish, _M_finish);
        _M_finish = __new_finish;
    }
    else{
        _ForwardIter __mid = first;
        advance(_mid,size());
        copy(__first, __mid, __M_start);
        _M_finish = uninitialized_copy(__mid,__last, _M_finish);
    }
}

template<class _Tp, class _Alloc>
void
vector<_Tp, _Alloc>::_M_insert_aux(iterator __position, const _Tp& __x){
    if(_M_finish != _M_end_of_storage){
        construct(_M_finish, *(_M_finish - 1));
        ++_M_finish;
        _Tp __x_copy = __x;
        copy_backward(__position, _M_finish - 2, _M_finish - 1);
        *__position = __x_copy;
    }
    else{
        const size_type __old_szie = size();
        const size_type __len = old_size != 0 ? 2* old_size : 1;
        iterator __new_start = _M_allocate(__len);
        iterator __new_finish = __new_start;
        try{
            __new_finish = uninitialized_copy(_M_start, __position, __new_start);
            construct(__new_finish, __x);
            ++__new_finish;
            __new_finish = uninitialized_copy(__position, _M_finish, __new_finish);
        }
        catch(...){
            destroy(__new_start, __new_finish, _M_deallocate(__new_start,__len));

        }
        destroy(begin(),end());
        _M_deallocate(_M_start,_M_end_of_storage - _M_start);
        _M_start = __new_start;
        _M_finish = __new_finish;
        _M_end_of_storage = __new_start + __len;

    }
}

template<class _TP, class _Alloc>
void
vector<_Tp, _Alloc>::_M_insert_aux(iterator __position){
    if(_M_finish != _M_end_of_storage){
        construct(_M_finish, *(_M_finish-1));
        ++_M_finish;
        copy_backward(__position, _M_finish - 2, _M_finish-1);
        *__position = _Tp();
    }
    else{
        const size_type __old_size = size();
        const size_type __len = __old_size != 0? 2* __old_size : 1;
        iterator __new_start = _M_allocate(__len);
        iterator __new_finish = __new_start;
        try{
            __new_finish = uninitialized_copy(_M_start, __position, __new_finish);
            construct(__new_finish);
            ++__new_finish;
            __new_finish = uninitialized_copy(__position, _M_finish, __new_finish);

        }
        catch(...){
            destroy(__new_start, __new_finish, _M_deallocate(__new_start,__len));
        }
        destroy(begin(),end());
        _M_deallocate(_M_start, _M_end_of_storage - _M_start);
        _M_start = __new_start;
        _M_finish = __new_finish;
        _M_end_of_storage = __new_start + __len;
    }
}


template<class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(iterator __position, size_type __n, const _Tp &__x){
    if(__n != 0){
        if(size_type(_M_end_of_storage - _M_finish) >= __n){
            _Tp __x_copy = __x;
            const size_type __elems_after = _M_finish - __position;
            iterator __old_finish = _M_finish;
            if(__elems_after > __n){
                uninitialized_copy(_M_finish - __n, _M_finish, _M_finish);
                _M_finish += __n;
                copy_backward(__position, old_finish - __n, __old_finish);
                fill(__position, __position + __n, __x_copy);
            }
            else{
                uninitialized_fill_n(_M_finish, __n - __elems_after, __x_copy);
                _M_finish += __n - elems_after;
                uninitialized_copy(__position, __old_finish, _M_finish);
                _M_finish += __elmes_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else{
            const size_type __old_size = size();
            const size_type __len = __old_size + max(__old_size, __n);
            iterator __new_start = _M_allocate(__len);
            iterator __new_finish = __new_start;
            try{
                __new_finish = uninitialized_copy(_M_start, __position, __new_start);
                __new_finish = uninitialized_fill_n(__new_finish, __n, __x);
                __new_finish = uninitialized_copy(__position, _M_finish, __new_finish);

            }
            catch(...){
                destroy(__new_start, __new_finish, _M_deallocate(__new_start, __len));
            }
            destroy(_M_start, _M_finish);
            _M_deallocate(_M_start, _M_end_of_storage - _M_start);
            _M_start = __new_start;
            _M_finish = __new_finish;
            _M_end_of_storage =__new_start + __len;
        }

    }
}




__STL_END_NAMESPACE


#endif //MY_STL_STL_VECTOR_H