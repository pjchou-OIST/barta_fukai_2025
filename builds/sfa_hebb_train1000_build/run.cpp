#include<stdlib.h>
#include "objects.h"
#include<ctime>
#include<random>

#include "code_objects/neurongroup_1_spike_resetter_codeobject.h"
#include "code_objects/neurongroup_1_spike_thresholder_codeobject.h"
#include "code_objects/neurongroup_1_stateupdater_codeobject.h"
#include "code_objects/neurongroup_2_stateupdater_codeobject.h"
#include "code_objects/neurongroup_spike_resetter_codeobject.h"
#include "code_objects/neurongroup_spike_thresholder_codeobject.h"
#include "code_objects/neurongroup_stateupdater_codeobject.h"
#include "code_objects/poissoninput_1_codeobject.h"
#include "code_objects/poissoninput_codeobject.h"
#include "code_objects/spikemonitor_1_codeobject.h"
#include "code_objects/spikemonitor_codeobject.h"
#include "code_objects/synapses_1_summed_variable_network_rate_post_codeobject.h"
#include "code_objects/synapses_1_synapses_create_generator_codeobject.h"
#include "code_objects/synapses_2_pre_codeobject.h"
#include "code_objects/synapses_2_pre_push_spikes.h"
#include "code_objects/synapses_2_stateupdater_codeobject.h"
#include "code_objects/synapses_2_synapses_create_array_codeobject.h"
#include "code_objects/synapses_3_pre_codeobject.h"
#include "code_objects/synapses_3_pre_push_spikes.h"
#include "code_objects/synapses_3_synapses_create_array_codeobject.h"
#include "code_objects/synapses_4_post_codeobject.h"
#include "code_objects/synapses_4_post_push_spikes.h"
#include "code_objects/synapses_4_pre_codeobject.h"
#include "code_objects/synapses_4_pre_push_spikes.h"
#include "code_objects/synapses_4_synapses_create_array_codeobject.h"
#include "code_objects/synapses_5_pre_codeobject.h"
#include "code_objects/synapses_5_pre_push_spikes.h"
#include "code_objects/synapses_5_synapses_create_array_codeobject.h"
#include "code_objects/synapses_pre_codeobject.h"
#include "code_objects/synapses_pre_push_spikes.h"
#include "code_objects/synapses_synapses_create_generator_codeobject.h"


void brian_start()
{
	_init_arrays();
	_load_arrays();
	// Initialize clocks (link timestep and dt to the respective arrays)
    brian::defaultclock.timestep = brian::_array_defaultclock_timestep;
    brian::defaultclock.dt = brian::_array_defaultclock_dt;
    brian::defaultclock.t = brian::_array_defaultclock_t;
}

void brian_end()
{
	_write_arrays();
	_dealloc_arrays();
}


