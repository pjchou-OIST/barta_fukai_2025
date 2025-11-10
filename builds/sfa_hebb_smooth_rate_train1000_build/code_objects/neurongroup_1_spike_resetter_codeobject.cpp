#include "code_objects/neurongroup_1_spike_resetter_codeobject.h"
#include "objects.h"
#include "brianlib/common_math.h"
#include "brianlib/stdint_compat.h"
#include<cmath>
#include<ctime>
#include<iostream>
#include<fstream>
#include<climits>

////// SUPPORT CODE ///////
namespace {
        
    template < typename T1, typename T2 > struct _higher_type;
    template < > struct _higher_type<int32_t,int32_t> { typedef int32_t type; };
    template < > struct _higher_type<int32_t,int64_t> { typedef int64_t type; };
    template < > struct _higher_type<int32_t,float> { typedef float type; };
    template < > struct _higher_type<int32_t,double> { typedef double type; };
    template < > struct _higher_type<int32_t,long double> { typedef long double type; };
    template < > struct _higher_type<int64_t,int32_t> { typedef int64_t type; };
    template < > struct _higher_type<int64_t,int64_t> { typedef int64_t type; };
    template < > struct _higher_type<int64_t,float> { typedef float type; };
    template < > struct _higher_type<int64_t,double> { typedef double type; };
    template < > struct _higher_type<int64_t,long double> { typedef long double type; };
    template < > struct _higher_type<float,int32_t> { typedef float type; };
    template < > struct _higher_type<float,int64_t> { typedef float type; };
    template < > struct _higher_type<float,float> { typedef float type; };
    template < > struct _higher_type<float,double> { typedef double type; };
    template < > struct _higher_type<float,long double> { typedef long double type; };
    template < > struct _higher_type<double,int32_t> { typedef double type; };
    template < > struct _higher_type<double,int64_t> { typedef double type; };
    template < > struct _higher_type<double,float> { typedef double type; };
    template < > struct _higher_type<double,double> { typedef double type; };
    template < > struct _higher_type<double,long double> { typedef long double type; };
    template < > struct _higher_type<long double,int32_t> { typedef long double type; };
    template < > struct _higher_type<long double,int64_t> { typedef long double type; };
    template < > struct _higher_type<long double,float> { typedef long double type; };
    template < > struct _higher_type<long double,double> { typedef long double type; };
    template < > struct _higher_type<long double,long double> { typedef long double type; };
    // General template, used for floating point types
    template < typename T1, typename T2 >
    static inline typename _higher_type<T1,T2>::type
    _brian_mod(T1 x, T2 y)
    {
        return x-y*floor(1.0*x/y);
    }
    // Specific implementations for integer types
    // (from Cython, see LICENSE file)
    template <>
    inline int32_t _brian_mod(int32_t x, int32_t y)
    {
        int32_t r = x % y;
        r += ((r != 0) & ((r ^ y) < 0)) * y;
        return r;
    }
    template <>
    inline int64_t _brian_mod(int32_t x, int64_t y)
    {
        int64_t r = x % y;
        r += ((r != 0) & ((r ^ y) < 0)) * y;
        return r;
    }
    template <>
    inline int64_t _brian_mod(int64_t x, int32_t y)
    {
        int64_t r = x % y;
        r += ((r != 0) & ((r ^ y) < 0)) * y;
        return r;
    }
    template <>
    inline int64_t _brian_mod(int64_t x, int64_t y)
    {
        int64_t r = x % y;
        r += ((r != 0) & ((r ^ y) < 0)) * y;
        return r;
    }
    // General implementation, used for floating point types
    template < typename T1, typename T2 >
    static inline typename _higher_type<T1,T2>::type
    _brian_floordiv(T1 x, T2 y)
    {{
        return floor(1.0*x/y);
    }}
    // Specific implementations for integer types
    // (from Cython, see LICENSE file)
    template <>
    inline int32_t _brian_floordiv<int32_t, int32_t>(int32_t a, int32_t b) {
        int32_t q = a / b;
        int32_t r = a - q*b;
        q -= ((r != 0) & ((r ^ b) < 0));
        return q;
    }
    template <>
    inline int64_t _brian_floordiv<int32_t, int64_t>(int32_t a, int64_t b) {
        int64_t q = a / b;
        int64_t r = a - q*b;
        q -= ((r != 0) & ((r ^ b) < 0));
        return q;
    }
    template <>
    inline int64_t _brian_floordiv<int64_t, int>(int64_t a, int32_t b) {
        int64_t q = a / b;
        int64_t r = a - q*b;
        q -= ((r != 0) & ((r ^ b) < 0));
        return q;
    }
    template <>
    inline int64_t _brian_floordiv<int64_t, int64_t>(int64_t a, int64_t b) {
        int64_t q = a / b;
        int64_t r = a - q*b;
        q -= ((r != 0) & ((r ^ b) < 0));
        return q;
    }
    #ifdef _MSC_VER
    #define _brian_pow(x, y) (pow((double)(x), (y)))
    #else
    #define _brian_pow(x, y) (pow((x), (y)))
    #endif

}

////// HASH DEFINES ///////



void _run_neurongroup_1_spike_resetter_codeobject()
{
    using namespace brian;


    ///// CONSTANTS ///////////
    const double EL = - 0.08;
const size_t _numH1 = 2000;
const size_t _numH2 = 2000;
const int64_t N = 2000;
const size_t _num_spikespace = 2001;
const size_t _numa1 = 2000;
const size_t _numa2 = 2000;
const size_t _numq = 2000;
const size_t _numrech = 2000;
const size_t _numv = 2000;
const size_t _numx = 2000;
const size_t _numnot_refractory = 2000;
    ///// POINTERS ////////////
        
    double* __restrict  _ptr_array_neurongroup_1_H1 = _array_neurongroup_1_H1;
    double* __restrict  _ptr_array_neurongroup_1_H2 = _array_neurongroup_1_H2;
    int32_t* __restrict  _ptr_array_neurongroup_1__spikespace = _array_neurongroup_1__spikespace;
    double* __restrict  _ptr_array_neurongroup_1_a1 = _array_neurongroup_1_a1;
    double* __restrict  _ptr_array_neurongroup_1_a2 = _array_neurongroup_1_a2;
    double* __restrict  _ptr_array_neurongroup_1_q = _array_neurongroup_1_q;
    double* __restrict  _ptr_array_neurongroup_1_rech = _array_neurongroup_1_rech;
    double* __restrict  _ptr_array_neurongroup_1_v = _array_neurongroup_1_v;
    double* __restrict  _ptr_array_neurongroup_1_x = _array_neurongroup_1_x;
    char* __restrict  _ptr_array_neurongroup_1_not_refractory = _array_neurongroup_1_not_refractory;



	const int32_t *_events = _ptr_array_neurongroup_1__spikespace;
	const int32_t _num_events = _ptr_array_neurongroup_1__spikespace[N];

	//// MAIN CODE ////////////	
	// scalar code
	const size_t _vectorisation_idx = -1;
 	


	#pragma omp parallel for schedule(static)
	for(int32_t _index_events=0; _index_events<_num_events; _index_events++)
	{
	    // vector code
		const size_t _idx = _events[_index_events];
		const size_t _vectorisation_idx = _idx;
                
        double H1 = _ptr_array_neurongroup_1_H1[_idx];
        double H2 = _ptr_array_neurongroup_1_H2[_idx];
        const double a1 = _ptr_array_neurongroup_1_a1[_idx];
        const double a2 = _ptr_array_neurongroup_1_a2[_idx];
        double q = _ptr_array_neurongroup_1_q[_idx];
        const double rech = _ptr_array_neurongroup_1_rech[_idx];
        double x = _ptr_array_neurongroup_1_x[_idx];
        double v;
        H1 += a1;
        H2 += a2;
        x += (1.0 + ((_brian_pow(1.0 - q, 8)) * rech)) - rech;
        q = 1;
        v = EL;
        _ptr_array_neurongroup_1_H1[_idx] = H1;
        _ptr_array_neurongroup_1_H2[_idx] = H2;
        _ptr_array_neurongroup_1_q[_idx] = q;
        _ptr_array_neurongroup_1_v[_idx] = v;
        _ptr_array_neurongroup_1_x[_idx] = x;

	}

}


