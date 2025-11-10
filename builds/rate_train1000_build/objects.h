
#ifndef _BRIAN_OBJECTS_H
#define _BRIAN_OBJECTS_H

#include "synapses_classes.h"
#include "brianlib/clocks.h"
#include "brianlib/dynamic_array.h"
#include "brianlib/stdint_compat.h"
#include "network.h"
#include<random>
#include<vector>
#include <omp.h>

namespace brian {

extern std::string results_dir;

class RandomGenerator {
    private:
        std::mt19937 gen;
        double stored_gauss;
        bool has_stored_gauss = false;
    public:
        RandomGenerator() {
            seed();
        }
        void seed() {
            std::random_device rd;
            gen.seed(rd());
            has_stored_gauss = false;
        }
        void seed(unsigned long seed) {
            gen.seed(seed);
            has_stored_gauss = false;
        }
        double rand() {
            /* shifts : 67108864 = 0x4000000, 9007199254740992 = 0x20000000000000 */
            const long a = gen() >> 5;
            const long b = gen() >> 6;
            return (a * 67108864.0 + b) / 9007199254740992.0;
        }

        double randn() {
            if (has_stored_gauss) {
                const double tmp = stored_gauss;
                has_stored_gauss = false;
                return tmp;
            }
            else {
                double f, x1, x2, r2;

                do {
                    x1 = 2.0*rand() - 1.0;
                    x2 = 2.0*rand() - 1.0;
                    r2 = x1*x1 + x2*x2;
                }
                while (r2 >= 1.0 || r2 == 0.0);

                /* Box-Muller transform */
                f = sqrt(-2.0*log(r2)/r2);
                /* Keep for next call */
                stored_gauss = f*x1;
                has_stored_gauss = true;
                return f*x2;
            }
        }
};

// In OpenMP we need one state per thread
extern std::vector< RandomGenerator > _random_generators;

//////////////// clocks ///////////////////
extern Clock defaultclock;

//////////////// networks /////////////////
extern Network network;



void set_variable_by_name(std::string, std::string);

//////////////// dynamic arrays ///////////
extern std::vector<int32_t> _dynamic_array_spikemonitor_1_i;
extern std::vector<double> _dynamic_array_spikemonitor_1_t;
extern std::vector<int32_t> _dynamic_array_spikemonitor_i;
extern std::vector<double> _dynamic_array_spikemonitor_t;
extern std::vector<int32_t> _dynamic_array_synapses_1__synaptic_post;
extern std::vector<int32_t> _dynamic_array_synapses_1__synaptic_pre;
extern std::vector<int32_t> _dynamic_array_synapses_1_N_incoming;
extern std::vector<int32_t> _dynamic_array_synapses_1_N_outgoing;
extern std::vector<int32_t> _dynamic_array_synapses_2__synaptic_post;
extern std::vector<int32_t> _dynamic_array_synapses_2__synaptic_pre;
extern std::vector<double> _dynamic_array_synapses_2_delay;
extern std::vector<int32_t> _dynamic_array_synapses_2_N_incoming;
extern std::vector<int32_t> _dynamic_array_synapses_2_N_outgoing;
extern std::vector<double> _dynamic_array_synapses_2_w;
extern std::vector<double> _dynamic_array_synapses_2_x_std;
extern std::vector<int32_t> _dynamic_array_synapses_3__synaptic_post;
extern std::vector<int32_t> _dynamic_array_synapses_3__synaptic_pre;
extern std::vector<double> _dynamic_array_synapses_3_delay;
extern std::vector<int32_t> _dynamic_array_synapses_3_N_incoming;
extern std::vector<int32_t> _dynamic_array_synapses_3_N_outgoing;
extern std::vector<double> _dynamic_array_synapses_3_w;
extern std::vector<int32_t> _dynamic_array_synapses_4__synaptic_post;
extern std::vector<int32_t> _dynamic_array_synapses_4__synaptic_pre;
extern std::vector<double> _dynamic_array_synapses_4_alpha;
extern std::vector<double> _dynamic_array_synapses_4_delay;
extern std::vector<double> _dynamic_array_synapses_4_delay_1;
extern std::vector<int32_t> _dynamic_array_synapses_4_N_incoming;
extern std::vector<int32_t> _dynamic_array_synapses_4_N_outgoing;
extern std::vector<double> _dynamic_array_synapses_4_w;
extern std::vector<int32_t> _dynamic_array_synapses_5__synaptic_post;
extern std::vector<int32_t> _dynamic_array_synapses_5__synaptic_pre;
extern std::vector<double> _dynamic_array_synapses_5_delay;
extern std::vector<int32_t> _dynamic_array_synapses_5_N_incoming;
extern std::vector<int32_t> _dynamic_array_synapses_5_N_outgoing;
extern std::vector<double> _dynamic_array_synapses_5_w;
extern std::vector<int32_t> _dynamic_array_synapses__synaptic_post;
extern std::vector<int32_t> _dynamic_array_synapses__synaptic_pre;
extern std::vector<double> _dynamic_array_synapses_delay;
extern std::vector<int32_t> _dynamic_array_synapses_N_incoming;
extern std::vector<int32_t> _dynamic_array_synapses_N_outgoing;
extern std::vector<double> _dynamic_array_synapses_w;

//////////////// arrays ///////////////////
extern double *_array_defaultclock_dt;
extern const int _num__array_defaultclock_dt;
extern double *_array_defaultclock_t;
extern const int _num__array_defaultclock_t;
extern int64_t *_array_defaultclock_timestep;
extern const int _num__array_defaultclock_timestep;
extern int32_t *_array_neurongroup_1__spikespace;
extern const int _num__array_neurongroup_1__spikespace;
extern double *_array_neurongroup_1_a1;
extern const int _num__array_neurongroup_1_a1;
extern double *_array_neurongroup_1_a2;
extern const int _num__array_neurongroup_1_a2;
extern double *_array_neurongroup_1_basethr;
extern const int _num__array_neurongroup_1_basethr;
extern double *_array_neurongroup_1_ge;
extern const int _num__array_neurongroup_1_ge;
extern double *_array_neurongroup_1_gi;
extern const int _num__array_neurongroup_1_gi;
extern double *_array_neurongroup_1_gL;
extern const int _num__array_neurongroup_1_gL;
extern double *_array_neurongroup_1_H1;
extern const int _num__array_neurongroup_1_H1;
extern double *_array_neurongroup_1_H2;
extern const int _num__array_neurongroup_1_H2;
extern int32_t *_array_neurongroup_1_i;
extern const int _num__array_neurongroup_1_i;
extern double *_array_neurongroup_1_inhf;
extern const int _num__array_neurongroup_1_inhf;
extern double *_array_neurongroup_1_lastspike;
extern const int _num__array_neurongroup_1_lastspike;
extern double *_array_neurongroup_1_max_theta;
extern const int _num__array_neurongroup_1_max_theta;
extern double *_array_neurongroup_1_network_rate;
extern const int _num__array_neurongroup_1_network_rate;
extern double *_array_neurongroup_1_neuron_target_rate;
extern const int _num__array_neurongroup_1_neuron_target_rate;
extern char *_array_neurongroup_1_not_refractory;
extern const int _num__array_neurongroup_1_not_refractory;
extern double *_array_neurongroup_1_q;
extern const int _num__array_neurongroup_1_q;
extern double *_array_neurongroup_1_rech;
extern const int _num__array_neurongroup_1_rech;
extern double *_array_neurongroup_1_v;
extern const int _num__array_neurongroup_1_v;
extern double *_array_neurongroup_1_x;
extern const int _num__array_neurongroup_1_x;
extern double *_array_neurongroup_1_y;
extern const int _num__array_neurongroup_1_y;
extern double *_array_neurongroup_1_z;
extern const int _num__array_neurongroup_1_z;
extern int32_t *_array_neurongroup_2_i;
extern const int _num__array_neurongroup_2_i;
extern double *_array_neurongroup_2_r;
extern const int _num__array_neurongroup_2_r;
extern int32_t *_array_neurongroup__spikespace;
extern const int _num__array_neurongroup__spikespace;
extern double *_array_neurongroup_a1;
extern const int _num__array_neurongroup_a1;
extern double *_array_neurongroup_a2;
extern const int _num__array_neurongroup_a2;
extern double *_array_neurongroup_basethr;
extern const int _num__array_neurongroup_basethr;
extern double *_array_neurongroup_ge;
extern const int _num__array_neurongroup_ge;
extern double *_array_neurongroup_gi;
extern const int _num__array_neurongroup_gi;
extern double *_array_neurongroup_gL;
extern const int _num__array_neurongroup_gL;
extern double *_array_neurongroup_H1;
extern const int _num__array_neurongroup_H1;
extern double *_array_neurongroup_H2;
extern const int _num__array_neurongroup_H2;
extern int32_t *_array_neurongroup_i;
extern const int _num__array_neurongroup_i;
extern double *_array_neurongroup_inhf;
extern const int _num__array_neurongroup_inhf;
extern double *_array_neurongroup_lastspike;
extern const int _num__array_neurongroup_lastspike;
extern double *_array_neurongroup_max_theta;
extern const int _num__array_neurongroup_max_theta;
extern double *_array_neurongroup_network_rate;
extern const int _num__array_neurongroup_network_rate;
extern double *_array_neurongroup_neuron_target_rate;
extern const int _num__array_neurongroup_neuron_target_rate;
extern char *_array_neurongroup_not_refractory;
extern const int _num__array_neurongroup_not_refractory;
extern double *_array_neurongroup_q;
extern const int _num__array_neurongroup_q;
extern double *_array_neurongroup_rech;
extern const int _num__array_neurongroup_rech;
extern double *_array_neurongroup_v;
extern const int _num__array_neurongroup_v;
extern double *_array_neurongroup_x;
extern const int _num__array_neurongroup_x;
extern double *_array_neurongroup_y;
extern const int _num__array_neurongroup_y;
extern double *_array_neurongroup_z;
extern const int _num__array_neurongroup_z;
extern int32_t *_array_spikemonitor_1__source_idx;
extern const int _num__array_spikemonitor_1__source_idx;
extern int32_t *_array_spikemonitor_1_count;
extern const int _num__array_spikemonitor_1_count;
extern int32_t *_array_spikemonitor_1_N;
extern const int _num__array_spikemonitor_1_N;
extern int32_t *_array_spikemonitor__source_idx;
extern const int _num__array_spikemonitor__source_idx;
extern int32_t *_array_spikemonitor_count;
extern const int _num__array_spikemonitor_count;
extern int32_t *_array_spikemonitor_N;
extern const int _num__array_spikemonitor_N;
extern int32_t *_array_synapses_1_N;
extern const int _num__array_synapses_1_N;
extern int32_t *_array_synapses_2_N;
extern const int _num__array_synapses_2_N;
extern int32_t *_array_synapses_2_sources;
extern const int _num__array_synapses_2_sources;
extern int32_t *_array_synapses_2_targets;
extern const int _num__array_synapses_2_targets;
extern int32_t *_array_synapses_3_N;
extern const int _num__array_synapses_3_N;
extern int32_t *_array_synapses_3_sources;
extern const int _num__array_synapses_3_sources;
extern int32_t *_array_synapses_3_targets;
extern const int _num__array_synapses_3_targets;
extern int32_t *_array_synapses_4_N;
extern const int _num__array_synapses_4_N;
extern int32_t *_array_synapses_4_sources;
extern const int _num__array_synapses_4_sources;
extern int32_t *_array_synapses_4_targets;
extern const int _num__array_synapses_4_targets;
extern int32_t *_array_synapses_5_N;
extern const int _num__array_synapses_5_N;
extern int32_t *_array_synapses_5_sources;
extern const int _num__array_synapses_5_sources;
extern int32_t *_array_synapses_5_targets;
extern const int _num__array_synapses_5_targets;
extern int32_t *_array_synapses_N;
extern const int _num__array_synapses_N;

//////////////// dynamic arrays 2d /////////

/////////////// static arrays /////////////
extern double *_static_array__array_neurongroup_1_v;
extern const int _num__static_array__array_neurongroup_1_v;
extern double *_static_array__array_neurongroup_v;
extern const int _num__static_array__array_neurongroup_v;
extern int32_t *_static_array__array_synapses_2_sources;
extern const int _num__static_array__array_synapses_2_sources;
extern int32_t *_static_array__array_synapses_2_targets;
extern const int _num__static_array__array_synapses_2_targets;
extern int32_t *_static_array__array_synapses_3_sources;
extern const int _num__static_array__array_synapses_3_sources;
extern int32_t *_static_array__array_synapses_3_targets;
extern const int _num__static_array__array_synapses_3_targets;
extern int32_t *_static_array__array_synapses_4_sources;
extern const int _num__static_array__array_synapses_4_sources;
extern int32_t *_static_array__array_synapses_4_targets;
extern const int _num__static_array__array_synapses_4_targets;
extern int32_t *_static_array__array_synapses_5_sources;
extern const int _num__static_array__array_synapses_5_sources;
extern int32_t *_static_array__array_synapses_5_targets;
extern const int _num__static_array__array_synapses_5_targets;
extern double *_static_array__dynamic_array_synapses_2_delay;
extern const int _num__static_array__dynamic_array_synapses_2_delay;
extern double *_static_array__dynamic_array_synapses_2_w;
extern const int _num__static_array__dynamic_array_synapses_2_w;
extern double *_static_array__dynamic_array_synapses_3_delay;
extern const int _num__static_array__dynamic_array_synapses_3_delay;
extern double *_static_array__dynamic_array_synapses_3_w;
extern const int _num__static_array__dynamic_array_synapses_3_w;
extern double *_static_array__dynamic_array_synapses_4_delay;
extern const int _num__static_array__dynamic_array_synapses_4_delay;
extern double *_static_array__dynamic_array_synapses_4_w;
extern const int _num__static_array__dynamic_array_synapses_4_w;
extern double *_static_array__dynamic_array_synapses_5_delay;
extern const int _num__static_array__dynamic_array_synapses_5_delay;
extern double *_static_array__dynamic_array_synapses_5_w;
extern const int _num__static_array__dynamic_array_synapses_5_w;

//////////////// synapses /////////////////
// synapses
extern SynapticPathway synapses_pre;
// synapses_1
// synapses_2
extern SynapticPathway synapses_2_pre;
// synapses_3
extern SynapticPathway synapses_3_pre;
// synapses_4
extern SynapticPathway synapses_4_post;
extern SynapticPathway synapses_4_pre;
// synapses_5
extern SynapticPathway synapses_5_pre;

// Profiling information for each code object
}

void _init_arrays();
void _load_arrays();
void _write_arrays();
void _dealloc_arrays();

#endif


