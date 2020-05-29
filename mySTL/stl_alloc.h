/* created by Chenyu ZHU on 2020/5/20*/

#ifndef MY_STL_STL_ALLOC_H
#define MY_STL_STL_ALLOC_H

#include<cstddef>	// for ptrdiff_t, size_t
#include<new>		// for placement new
#include<cstdlib>	// for exit()
#include<climits>	// for UNIT_MAX
#include<iostream>	// for cerr
#include "stl_config.h"
#include "stl_construct.h"

__STL_BEGIN_NAMESPACE

template<int __inst>
class __malloc_alloc_template {
public:
	static void* allocate(size_t __n) {
		void* __result = malloc(__n);
		if (__result == 0) {
			fprintf(stderr, "out of memory\n");
			exit(1);
		}
		return __result;
	}

	static void* deallocate(void* __o, size_t /* __n */) {
		free(p);
	}

	static void* reallocate(void* __p, size_t /* old_sz */, size_t __new_sz) {
		void* __result = realloc(__p, __new_sz);
		if (__result == 0) {
			fprintf(stderr, "out of memory\n");
			exit(1);
		}
		return __result;
	}
		

};

typedef __malloc_alloc_template<0> malloc_alloc;

template<class _Tp, class _Alloc>
class simple_alloc {
public:
	static _Tp* allocate(size_t __n) {
		return 0 == __n ? nullptr : (_Tp*)_Alloc::allocate(__n * sizeof(_Tp));
	}

	static _Tp* allocate(void) {
		return (_Tp*)_Alloc::allocate(sizeof(_Tp));
	}

	static void deallocate(_Tp* __p, size_t __n) {
		if (__n != 0) {
			_Alloc::deallocate(__p, __n, sizeof(_Tp));
		}
	}

	static void deallocate(_Tp* __p) {
		_Alloc::deallocate(__p, sizeof(_Tp));
	}
};

typedef malloc_alloc alloc;


__STL_END_NAMESPACE

#endif MY_STL_STL_ALLOC_H