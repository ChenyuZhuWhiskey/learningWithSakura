/* created by Chenyu ZHU on 2020/5/22*/

#ifndef MY_STL_STL_LIST_H
#define MY_STL_STL_LIST_H

#include<cstddef>	// for ptrdiff_t, size_t
#include "stl_config.h"
#include "stl_iterator_base.h"
#include "stl_iterator.h"
#include "stl_alloc.h"
#include "stl_algobase.h"

__STL_BEGIN_NAMESPACE

struct _List_node_base{
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template<class _Tp>
struct _List_node : public _List_node_base{
    _Tp _M_data;
};

struct _List_Iterator_base{
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef bidirectional_iterator_tag iterator_category;
    
    _List_node_base* _M_node;

    _List_Iterator_base(_List_node_base* __x): _M_node(__x){}
    _List_Iterator_base(){}

    void _M_incr() {_M_node = _M_node -> _M_next;}
    void _M_decr() {_M_node = _M_node -> _M_prev;}

    bool operator==(const _List_Iterator_base& __x) const{
        return _M_node == __x._M_node;
    }

    bool operator!=(const _List_Iterator_base& __x){
        return _M_node != __x._M_node;
    }

};

template<class _Tp, class _Ref, class _Ptr>
struct _List_iterator:public _List_iterator_base{
    typedef _List_iterator<_Tp, _Tp&, _Tp*> iterator;
    typedef _List_iterator<_Tp, const _Tp&, const _Tp*> const_iterator;
    typedef _List_iterator<_Tp, _Ref, _Ptr> _Self;

    typedef _Tp value_type;
    typedef _Ptr pointer;
    typedef _Ref reference;
    typedef _List_node<_Tp> _Node;

    _List_iterator(_Node* __x): _List_iterator_base(__x){}
    _List_iterator(){}
    _List_iterator(const iterator& __x): _List_Iterator_base(__x._M_node){}

    reference operator*() const { return ((_Node*) _M_node) -> _M_data;  }


    pointer operator->() const { return &(operator*()); }


    _Self& operator++(){
        this -> _M_incr();
        return *this;
    }

    _Self operator++(int){
        _Self tmp = *this;
        this -> _M_incr();
        return __tmp;
    }

    _Self& operator--(){
        this -> _M_decr();
        return *this;
    }

    _Self operator--(int){
        _Self __tmp = *this;
        this -> _M_decr();
        return __tmp;
    }

};

// #ifndef __STL__CLASS_PARTIAL_SPECILIZATION
// inline bidirectional_iterator_tag
// iterator_category(const _List_Iterator_base&)
// {
//     return bidirectional_iterator_tag();
// }

// template<class _Tp, class _Ref, class _Ptr>
// inline _Tp*
// value_type(const _List_iterator<_Tp, _Ref, _Ptr>&){
//     return 0;
// }

// inline ptrdiff_t*
// distance_type(const _List_Iterator_base&){
//     return 0;
// }

// #endif __STL__CLASS_PARTIAL_SPECILIZATION

template<class _Tp, class _Alloc>
class _List_base
{
public:
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const {return allocator_type();}

    _List_base(const allocator_type&){
        _M_node = _M_get_node();
        _M_node -> _M_next = _M_node;
        _M_node -> _M_prev = _M_node;
    }
    ~_List_base(){
        clear();
        _M_put_node(_M_node);
    }

    void clear();

protected:
    typedef simple_alloc<_List_node<_Tp>, _Alloc> _Alloc_type;
    _List_node<_Tp>* _M_get_node(){return _Alloc_type::allocate(1);}
    void _M_put_node(_List_node<_Tp>* __p){_Alloc_type::deallocate(__p, 1);}

protected:
    _List_node<_Tp>* _M_node;
};

template<class _Tp, class _Alloc>
void
_List_base<_Tp,_Alloc>::clear(){
    _List_node<_Tp>* __cur = (_List_node<_Tp>*) _M_node;
    while(__cur != _M_node){
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*) __cur -> _M_next;
        _Destroy(&__tmp->_M_data);
        _M_put_node(__tmp);
    }

    _M_node -> _M_next = _M_node;
    _M_node -> _M_prev = _M_node;
}


template<class _Tp, class _Alloc = alloc>
class list : protected _List_base<_Tp, _Alloc>{
    typedef _List_base<_Tp, _Alloc> _Base;
protected:
    typedef void* _Void_pointer;
public:
    typedef _Tp value_type;
    typedef Tp* pointer;
    typedef const Tp* const_pointer;
    typedef _Tp& reference;
    typedef _List_node<_Tp> _Node;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    typedef typename _Base::allocator_type allocator_type;
    allocator_type get_allocator() cosnt {return allocator_type();}

public:
    typedef _List_iterator<_Tp, _Tp&, _Tp*> iterator;
    typedef _List_iterator<_Tp, const _Tp&, const _Tp*> const_iterator;

    typedef reverse_iterator<const_iterator> const_reverse_iterator;
    typedef reverse_iterator<iterator> reverse_iterator;

protected:
    using _Base::_M_node;
    using _BAse::_M_put_node;
    using _Base::_M_get_node;


protected:
    _Node* _M_create_node(const _Tp& __x){
        _Node* __p = _M_get_node();
        try{
            _Construct(&__p -> _M_data, __x);
        }
        catch(...){
            _M_put_node(__p);
            throw;
        }
        return __p;
    }
    
    _node* _M_create_node(){
        _Node* __p = _M_get_node();
        try{
            _Construct(&__p -> _M_data);
        }
        catch(...){
            throw;
        }
        return __p;
    }

public:
    explicit list(const allocator_type &__a = allocator_type()) : _Base(__a){}

    iterator begin(){
        return (_Node*) (_M_node -> _M_next);
    }

    const_iterator begin() const {
        return (_Node*) (_M_node -> _M_next);
    }

    iterator end(){
        return (_Node*) (_M_node);
    }

    iterator end() const{
        return (_Node*) (_M_node);
    }

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const{
        return const_reverse_iterator(end());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const{
        return const_reverse_iterator(begin());
    }

    bool empty() const {
        return _M_node -> _M_next ==_M_node;
    }

    size_type size() const {
        size_type __result = 0;
        distance(begin(),end(),__result);
        return __result;
    }

    size_type max_size() const{
        return size_type(-1);
    }

    reference front(){
        return *begin();
    }

    const_reference front() const{
        return *begin();
    }

    reference back(){
        return *--end();
    }

    const_reference back() const{
        return *--end()
    }
};

__STL_END_NAMESPACE

#endif