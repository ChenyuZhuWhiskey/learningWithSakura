/* created by Chenyu ZHU on 2020/5/20*/

#ifndef MY_STL_TYPE_TRAITS_H
#define MY_STL_TYPE_TRAITS_H

struct __true_type{};
struct __false_type{};

template<class _Tp>
struct __type_traits {
	typedef __true_type this_dummy_member_must_be_first;

	typedef __false_type has_trivial_default_constructor;
	typedef __false_type has_trivial_copy_constructor;
	typedef __false_type has_trivial_assignment_operator;
	typedef __false_type has_trivial_destructor;
	typedef __false_type is_POD_type;
};


//Partial specialization of Plain old data structure:
template<>
struct __type_traits<bool> {
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};


template<>
struct __type_traits<char> {
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};

template<>
struct __type_traits<signed char> {
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};

template<>
struct __type_traits<unsigned char> {
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};

template<>
struct __type_traits<short> {
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};

template<>
struct __type_traits<unsigned short> {
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};

template<>
struct __type_traits<int> {
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};

template<>
struct __type_traits<unsigned int> {
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};

template<>
struct __type_traits<long> {
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};

template<>
struct __type_traits<unsigned long> {
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};

template<>
struct __type_traits<long long> {
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};


template<>
struct __type_traits<unsigned long long> {
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};

template<>
struct __type_traits<double> {
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};

template<>
struct __type_traits<long double> {
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};


//integer traits
template<class _Tp>
struct _is_integer{
	typedef __false_type _integral;
};

template<>
struct _is_integer<int> {
	typedef __true_type _integral;
};

template<>
struct _is_integer<unsigned int> {
	typedef __true_type _integral;
};

template<>
struct _is_integer<bool> {
	typedef __true_type _integral;
};

template<>
struct _is_integer<char> {
	typedef __true_type _integral;
};

template<>
struct _is_integer<signed char> {
	typedef __true_type _integral;
};

template<>
struct _is_integer<unsigned char> {
	typedef __true_type _integral;
};

template<>
struct _is_integer<short> {
	typedef __true_type _integral;
};

template<>
struct _is_integer<unsigned short> {
	typedef __true_type _integral;
};

template<>
struct _is_integer<long> {
	typedef __true_type _integral;
};

template<>
struct _is_integer<unsigned long> {
	typedef __true_type _integral;
};

#endif //MY_STL_TYPE_TRAITS_H