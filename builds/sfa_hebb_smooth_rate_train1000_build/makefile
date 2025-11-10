PROGRAM = main

SRCS = code_objects/after_run_neurongroup_1_spike_thresholder_codeobject.cpp code_objects/after_run_neurongroup_spike_thresholder_codeobject.cpp code_objects/before_run_synapses_2_pre_push_spikes.cpp code_objects/before_run_synapses_3_pre_push_spikes.cpp code_objects/before_run_synapses_4_post_push_spikes.cpp code_objects/before_run_synapses_4_pre_push_spikes.cpp code_objects/before_run_synapses_5_pre_push_spikes.cpp code_objects/before_run_synapses_pre_push_spikes.cpp code_objects/neurongroup_1_spike_resetter_codeobject.cpp code_objects/neurongroup_1_spike_thresholder_codeobject.cpp code_objects/neurongroup_1_stateupdater_codeobject.cpp code_objects/neurongroup_2_stateupdater_codeobject.cpp code_objects/neurongroup_spike_resetter_codeobject.cpp code_objects/neurongroup_spike_thresholder_codeobject.cpp code_objects/neurongroup_stateupdater_codeobject.cpp code_objects/poissoninput_1_codeobject.cpp code_objects/poissoninput_codeobject.cpp code_objects/spikemonitor_1_codeobject.cpp code_objects/spikemonitor_codeobject.cpp code_objects/synapses_1_summed_variable_network_rate_post_codeobject.cpp code_objects/synapses_1_synapses_create_generator_codeobject.cpp code_objects/synapses_2_pre_codeobject.cpp code_objects/synapses_2_pre_push_spikes.cpp code_objects/synapses_2_stateupdater_codeobject.cpp code_objects/synapses_2_synapses_create_array_codeobject.cpp code_objects/synapses_3_pre_codeobject.cpp code_objects/synapses_3_pre_push_spikes.cpp code_objects/synapses_3_synapses_create_array_codeobject.cpp code_objects/synapses_4_post_codeobject.cpp code_objects/synapses_4_post_push_spikes.cpp code_objects/synapses_4_pre_codeobject.cpp code_objects/synapses_4_pre_push_spikes.cpp code_objects/synapses_4_synapses_create_array_codeobject.cpp code_objects/synapses_5_pre_codeobject.cpp code_objects/synapses_5_pre_push_spikes.cpp code_objects/synapses_5_synapses_create_array_codeobject.cpp code_objects/synapses_pre_codeobject.cpp code_objects/synapses_pre_push_spikes.cpp code_objects/synapses_synapses_create_generator_codeobject.cpp main.cpp network.cpp objects.cpp run.cpp synapses_classes.cpp
H_SRCS = brianlib/clocks.h brianlib/common_math.h brianlib/dynamic_array.h code_objects/after_run_neurongroup_1_spike_thresholder_codeobject.h code_objects/after_run_neurongroup_spike_thresholder_codeobject.h code_objects/before_run_synapses_2_pre_push_spikes.h code_objects/before_run_synapses_3_pre_push_spikes.h code_objects/before_run_synapses_4_post_push_spikes.h code_objects/before_run_synapses_4_pre_push_spikes.h code_objects/before_run_synapses_5_pre_push_spikes.h code_objects/before_run_synapses_pre_push_spikes.h code_objects/neurongroup_1_spike_resetter_codeobject.h code_objects/neurongroup_1_spike_thresholder_codeobject.h code_objects/neurongroup_1_stateupdater_codeobject.h code_objects/neurongroup_2_stateupdater_codeobject.h code_objects/neurongroup_spike_resetter_codeobject.h code_objects/neurongroup_spike_thresholder_codeobject.h code_objects/neurongroup_stateupdater_codeobject.h code_objects/poissoninput_1_codeobject.h code_objects/poissoninput_codeobject.h code_objects/spikemonitor_1_codeobject.h code_objects/spikemonitor_codeobject.h code_objects/synapses_1_summed_variable_network_rate_post_codeobject.h code_objects/synapses_1_synapses_create_generator_codeobject.h code_objects/synapses_2_pre_codeobject.h code_objects/synapses_2_pre_push_spikes.h code_objects/synapses_2_stateupdater_codeobject.h code_objects/synapses_2_synapses_create_array_codeobject.h code_objects/synapses_3_pre_codeobject.h code_objects/synapses_3_pre_push_spikes.h code_objects/synapses_3_synapses_create_array_codeobject.h code_objects/synapses_4_post_codeobject.h code_objects/synapses_4_post_push_spikes.h code_objects/synapses_4_pre_codeobject.h code_objects/synapses_4_pre_push_spikes.h code_objects/synapses_4_synapses_create_array_codeobject.h code_objects/synapses_5_pre_codeobject.h code_objects/synapses_5_pre_push_spikes.h code_objects/synapses_5_synapses_create_array_codeobject.h code_objects/synapses_pre_codeobject.h code_objects/synapses_pre_push_spikes.h code_objects/synapses_synapses_create_generator_codeobject.h network.h objects.h run.h synapses_classes.h
OBJS = ${SRCS:.cpp=.o}
OBJS := ${OBJS:.c=.o}
OPTIMISATIONS = -I"/home/p/p-chou/barta_fukai_2025/.pixi/envs/default/include" -w -O3 -ffast-math -fno-finite-math-only -march=native -std=c++11
CXXFLAGS = -c -Wno-write-strings $(OPTIMISATIONS) -I. -fopenmp 
LFLAGS = -fopenmp -L"/home/p/p-chou/barta_fukai_2025/.pixi/envs/default/lib" -Wl,-R"/home/p/p-chou/barta_fukai_2025/.pixi/envs/default/lib" 
DEPS = make.deps

all: $(PROGRAM)

.PHONY: all clean

$(PROGRAM): $(OBJS) $(DEPS) makefile
	$(CXX) $(OBJS) -o $(PROGRAM) $(LFLAGS)

clean:
	rm $(OBJS) $(PROGRAM) $(DEPS)

make.deps: $(SRCS) $(H_SRCS)
	$(CXX) $(CXXFLAGS) -MM $(SRCS) > make.deps
	
ifneq ($(wildcard $(DEPS)), )
include $(DEPS)
endif

%.o : %.cpp makefile
	$(CXX) $(CXXFLAGS) $< -o $@