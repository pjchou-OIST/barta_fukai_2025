#include "code_objects/neurongroup_stateupdater_codeobject.h"
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
        
    static inline int64_t _timestep(double t, double dt)
    {
        return (int64_t)((t + 1e-3*dt)/dt);
    }
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



void _run_neurongroup_stateupdater_codeobject()
{
    using namespace brian;


    ///// CONSTANTS ///////////
    const double C = 2e-10;
const double EL = - 0.08;
const double Ee = 0.0;
const double Ei = - 0.08;
const size_t _numH1 = 8000;
const size_t _numH2 = 8000;
const int64_t N = 8000;
const size_t _numdt = 1;
const size_t _numgL = 8000;
const size_t _numge = 8000;
const size_t _numgi = 8000;
const size_t _numinhf = 8000;
const size_t _numlastspike = 8000;
const double nS = 1e-09;
const size_t _numnot_refractory = 8000;
const size_t _numq = 8000;
const size_t _numt = 1;
const double tau_coincidence = 0.15;
const double tau_rate = 10.0;
const double tau_stdp = 0.02;
const double tau_th1 = 0.01;
const double tau_th2 = 0.2;
const double taue = 0.006;
const double tidip = 0.16;
const size_t _numv = 8000;
const size_t _numx = 8000;
const size_t _numy = 8000;
const size_t _numz = 8000;
    ///// POINTERS ////////////
        
    double* __restrict  _ptr_array_neurongroup_H1 = _array_neurongroup_H1;
    double* __restrict  _ptr_array_neurongroup_H2 = _array_neurongroup_H2;
    double*   _ptr_array_defaultclock_dt = _array_defaultclock_dt;
    double* __restrict  _ptr_array_neurongroup_gL = _array_neurongroup_gL;
    double* __restrict  _ptr_array_neurongroup_ge = _array_neurongroup_ge;
    double* __restrict  _ptr_array_neurongroup_gi = _array_neurongroup_gi;
    double* __restrict  _ptr_array_neurongroup_inhf = _array_neurongroup_inhf;
    double* __restrict  _ptr_array_neurongroup_lastspike = _array_neurongroup_lastspike;
    char* __restrict  _ptr_array_neurongroup_not_refractory = _array_neurongroup_not_refractory;
    double* __restrict  _ptr_array_neurongroup_q = _array_neurongroup_q;
    double*   _ptr_array_defaultclock_t = _array_defaultclock_t;
    double* __restrict  _ptr_array_neurongroup_v = _array_neurongroup_v;
    double* __restrict  _ptr_array_neurongroup_x = _array_neurongroup_x;
    double* __restrict  _ptr_array_neurongroup_y = _array_neurongroup_y;
    double* __restrict  _ptr_array_neurongroup_z = _array_neurongroup_z;


    //// MAIN CODE ////////////
    // scalar code
    const size_t _vectorisation_idx = -1;
        
    const double dt = _ptr_array_defaultclock_dt[0];
    const double t = _ptr_array_defaultclock_t[0];
    const int64_t _lio_1 = _timestep(0.002, dt);
    const double _lio_2 = exp(1.0f*(- dt)/tau_th1);
    const double _lio_3 = exp(1.0f*(- dt)/tau_th2);
    const double _lio_4 = exp(1.0f*(- dt)/taue);
    const double _lio_5 = exp(1.0f*(- dt)/tau_coincidence);
    const double _lio_6 = 1.0f*EL/C;
    const double _lio_7 = 1.0f*Ee/C;
    const double _lio_8 = 1.0f*Ei/C;
    const double _lio_9 = 1.0f*1.0/C;
    const double _lio_10 = exp(1.0f*(- dt)/tau_stdp);
    const double _lio_11 = 1.0f*1.0/nS;
    const double _lio_12 = exp(1.0f*(- dt)/tidip);
    const double _lio_13 = exp(1.0f*(- dt)/tau_rate);


    const int _N = N;
    #pragma omp parallel for schedule(static)
    for(int _idx=0; _idx<_N; _idx++)
    {
        // vector code
        const size_t _vectorisation_idx = _idx;
                
        double H1 = _ptr_array_neurongroup_H1[_idx];
        double H2 = _ptr_array_neurongroup_H2[_idx];
        const double gL = _ptr_array_neurongroup_gL[_idx];
        double ge = _ptr_array_neurongroup_ge[_idx];
        double gi = _ptr_array_neurongroup_gi[_idx];
        const double inhf = _ptr_array_neurongroup_inhf[_idx];
        const double lastspike = _ptr_array_neurongroup_lastspike[_idx];
        char not_refractory = _ptr_array_neurongroup_not_refractory[_idx];
        double q = _ptr_array_neurongroup_q[_idx];
        double v = _ptr_array_neurongroup_v[_idx];
        double x = _ptr_array_neurongroup_x[_idx];
        double y = _ptr_array_neurongroup_y[_idx];
        double z = _ptr_array_neurongroup_z[_idx];
        not_refractory = _timestep(t - lastspike, dt) >= _lio_1;
        const double _H1 = _lio_2 * H1;
        const double _H2 = _lio_3 * H2;
        const double _ge = _lio_4 * ge;
        const double _gi = _lio_4 * gi;
        const double _q = _lio_5 * q;
        double _BA_v;
        if(!not_refractory)
            _BA_v = 0.0;
        else 
            _BA_v = 1.0f*(((_lio_6 * gL) + (_lio_7 * ge)) + (_lio_8 * (gi * inhf)))/((_lio_9 * (- gL)) - ((_lio_9 * ge) + (_lio_9 * (gi * inhf))));
        double _v;
        if(!not_refractory)
            _v = (- _BA_v) + (_BA_v + v);
        else 
            _v = (- _BA_v) + ((_BA_v + v) * exp(dt * ((_lio_9 * (- gL)) - ((_lio_9 * ge) + (_lio_9 * (gi * inhf))))));
        const double _x = _lio_10 * x;
        const double _BA_y = _lio_11 * (- ge);
        const double _y = (- _BA_y) + (_lio_12 * (_BA_y + y));
        const double _z = _lio_13 * z;
        H1 = _H1;
        H2 = _H2;
        ge = _ge;
        gi = _gi;
        q = _q;
        if(not_refractory)
            v = _v;
        x = _x;
        y = _y;
        z = _z;
        _ptr_array_neurongroup_H1[_idx] = H1;
        _ptr_array_neurongroup_H2[_idx] = H2;
        _ptr_array_neurongroup_ge[_idx] = ge;
        _ptr_array_neurongroup_gi[_idx] = gi;
        _ptr_array_neurongroup_not_refractory[_idx] = not_refractory;
        _ptr_array_neurongroup_q[_idx] = q;
        _ptr_array_neurongroup_v[_idx] = v;
        _ptr_array_neurongroup_x[_idx] = x;
        _ptr_array_neurongroup_y[_idx] = y;
        _ptr_array_neurongroup_z[_idx] = z;

    }

}


