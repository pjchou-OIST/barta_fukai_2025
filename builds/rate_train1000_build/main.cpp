#include <stdlib.h>
#include "objects.h"
#include <csignal>
#include <ctime>
#include <time.h>
#include <omp.h>
#include "run.h"
#include "brianlib/common_math.h"

#include "code_objects/neurongroup_1_spike_resetter_codeobject.h"
#include "code_objects/neurongroup_1_spike_thresholder_codeobject.h"
#include "code_objects/after_run_neurongroup_1_spike_thresholder_codeobject.h"
#include "code_objects/neurongroup_1_stateupdater_codeobject.h"
#include "code_objects/neurongroup_2_stateupdater_codeobject.h"
#include "code_objects/neurongroup_spike_resetter_codeobject.h"
#include "code_objects/neurongroup_spike_thresholder_codeobject.h"
#include "code_objects/after_run_neurongroup_spike_thresholder_codeobject.h"
#include "code_objects/neurongroup_stateupdater_codeobject.h"
#include "code_objects/poissoninput_1_codeobject.h"
#include "code_objects/poissoninput_codeobject.h"
#include "code_objects/spikemonitor_1_codeobject.h"
#include "code_objects/spikemonitor_codeobject.h"
#include "code_objects/synapses_1_summed_variable_network_rate_post_codeobject.h"
#include "code_objects/synapses_1_synapses_create_generator_codeobject.h"
#include "code_objects/synapses_2_pre_codeobject.h"
#include "code_objects/synapses_2_pre_push_spikes.h"
#include "code_objects/before_run_synapses_2_pre_push_spikes.h"
#include "code_objects/synapses_2_stateupdater_codeobject.h"
#include "code_objects/synapses_2_synapses_create_array_codeobject.h"
#include "code_objects/synapses_3_pre_codeobject.h"
#include "code_objects/synapses_3_pre_push_spikes.h"
#include "code_objects/before_run_synapses_3_pre_push_spikes.h"
#include "code_objects/synapses_3_synapses_create_array_codeobject.h"
#include "code_objects/synapses_4_post_codeobject.h"
#include "code_objects/synapses_4_post_push_spikes.h"
#include "code_objects/before_run_synapses_4_post_push_spikes.h"
#include "code_objects/synapses_4_pre_codeobject.h"
#include "code_objects/synapses_4_pre_push_spikes.h"
#include "code_objects/before_run_synapses_4_pre_push_spikes.h"
#include "code_objects/synapses_4_synapses_create_array_codeobject.h"
#include "code_objects/synapses_5_pre_codeobject.h"
#include "code_objects/synapses_5_pre_push_spikes.h"
#include "code_objects/before_run_synapses_5_pre_push_spikes.h"
#include "code_objects/synapses_5_synapses_create_array_codeobject.h"
#include "code_objects/synapses_pre_codeobject.h"
#include "code_objects/synapses_pre_push_spikes.h"
#include "code_objects/before_run_synapses_pre_push_spikes.h"
#include "code_objects/synapses_synapses_create_generator_codeobject.h"


#include <iostream>
#include <fstream>
#include <string>


        std::string _format_time(float time_in_s)
        {
            float divisors[] = {24*60*60, 60*60, 60, 1};
            char letters[] = {'d', 'h', 'm', 's'};
            float remaining = time_in_s;
            std::string text = "";
            int time_to_represent;
            for (int i =0; i < sizeof(divisors)/sizeof(float); i++)
            {
                time_to_represent = int(remaining / divisors[i]);
                remaining -= time_to_represent * divisors[i];
                if (time_to_represent > 0 || text.length())
                {
                    if(text.length() > 0)
                    {
                        text += " ";
                    }
                    text += (std::to_string(time_to_represent)+letters[i]);
                }
            }
            //less than one second
            if(text.length() == 0)
            {
                text = "< 1s";
            }
            return text;
        }
        void report_progress(const double elapsed, const double completed, const double start, const double duration)
        {
            if (completed == 0.0)
            {
                std::cerr << "Starting simulation at t=" << start << " s for duration " << duration << " s";
            } else
            {
                std::cerr << completed*duration << " s (" << (int)(completed*100.) << "%) simulated in " << _format_time(elapsed);
                if (completed < 1.0)
                {
                    const int remaining = (int)((1-completed)/completed*elapsed+0.5);
                    std::cerr << ", estimated " << _format_time(remaining) << " remaining.";
                }
            }

            std::cerr << std::endl << std::flush;
        }
        


void set_from_command_line(const std::vector<std::string> args)
{
    for (const auto& arg : args) {
		// Split into two parts
		size_t equal_sign = arg.find("=");
		auto name = arg.substr(0, equal_sign);
		auto value = arg.substr(equal_sign + 1, arg.length());
		brian::set_variable_by_name(name, value);
	}
}

void _int_handler(int signal_num) {
	if (Network::_globally_running && !Network::_globally_stopped) {
		Network::_globally_stopped = true;
	} else {
		std::signal(signal_num, SIG_DFL);
		std::raise(signal_num);
	}
}

int main(int argc, char **argv)
{
	std::signal(SIGINT, _int_handler);
	std::random_device _rd;
	std::vector<std::string> args(argv + 1, argv + argc);
	if (args.size() >=2 && args[0] == "--results_dir")
	{
		brian::results_dir = args[1];
		#ifdef DEBUG
		std::cout << "Setting results dir to '" << brian::results_dir << "'" << std::endl;
		#endif
		args.erase(args.begin(), args.begin()+2);
	}
        

	brian_start();
        

	{
		using namespace brian;

		omp_set_dynamic(0);
omp_set_num_threads(8);
                
        _array_defaultclock_dt[0] = 0.0001;
        _array_defaultclock_dt[0] = 0.0001;
        _array_defaultclock_dt[0] = 0.0001;
        _array_defaultclock_dt[0] = 0.0001;
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_neurongroup_lastspike; i++)
                        {
                            _array_neurongroup_lastspike[i] = - 10000.0;
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_neurongroup_not_refractory; i++)
                        {
                            _array_neurongroup_not_refractory[i] = true;
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_neurongroup_1_lastspike; i++)
                        {
                            _array_neurongroup_1_lastspike[i] = - 10000.0;
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_neurongroup_1_not_refractory; i++)
                        {
                            _array_neurongroup_1_not_refractory[i] = true;
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_neurongroup_max_theta; i++)
                        {
                            _array_neurongroup_max_theta[i] = 0.0;
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_neurongroup_1_max_theta; i++)
                        {
                            _array_neurongroup_1_max_theta[i] = 0.0;
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_neurongroup_neuron_target_rate; i++)
                        {
                            _array_neurongroup_neuron_target_rate[i] = 3;
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_neurongroup_1_neuron_target_rate; i++)
                        {
                            _array_neurongroup_1_neuron_target_rate[i] = 10;
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_neurongroup_rech; i++)
                        {
                            _array_neurongroup_rech[i] = 0;
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_neurongroup_1_rech; i++)
                        {
                            _array_neurongroup_1_rech[i] = 0;
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_neurongroup_a1; i++)
                        {
                            _array_neurongroup_a1[i] = 0;
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_neurongroup_1_a1; i++)
                        {
                            _array_neurongroup_1_a1[i] = 0;
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_neurongroup_a2; i++)
                        {
                            _array_neurongroup_a2[i] = 0.0003;
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_neurongroup_1_a2; i++)
                        {
                            _array_neurongroup_1_a2[i] = 0.0003;
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_neurongroup_gL; i++)
                        {
                            _array_neurongroup_gL[i] = 1e-08;
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_neurongroup_1_gL; i++)
                        {
                            _array_neurongroup_1_gL[i] = 1e-08;
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_neurongroup_v; i++)
                        {
                            _array_neurongroup_v[i] = _static_array__array_neurongroup_v[i];
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_neurongroup_1_v; i++)
                        {
                            _array_neurongroup_1_v[i] = _static_array__array_neurongroup_1_v[i];
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_neurongroup_basethr; i++)
                        {
                            _array_neurongroup_basethr[i] = - 0.055;
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_neurongroup_1_basethr; i++)
                        {
                            _array_neurongroup_1_basethr[i] = - 0.055;
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_neurongroup_inhf; i++)
                        {
                            _array_neurongroup_inhf[i] = 1;
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_neurongroup_1_inhf; i++)
                        {
                            _array_neurongroup_1_inhf[i] = 1;
                        }
                        
        _run_synapses_synapses_create_generator_codeobject();
        _run_synapses_1_synapses_create_generator_codeobject();
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_dynamic_array_synapses_w.size(); i++)
                        {
                            _dynamic_array_synapses_w[i] = 1.25e-05;
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_synapses_2_sources; i++)
                        {
                            _array_synapses_2_sources[i] = _static_array__array_synapses_2_sources[i];
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_synapses_2_targets; i++)
                        {
                            _array_synapses_2_targets[i] = _static_array__array_synapses_2_targets[i];
                        }
                        
        _run_synapses_2_synapses_create_array_codeobject();
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_dynamic_array_synapses_2_x_std.size(); i++)
                        {
                            _dynamic_array_synapses_2_x_std[i] = 1.0;
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_dynamic_array_synapses_2_w.size(); i++)
                        {
                            _dynamic_array_synapses_2_w[i] = _static_array__dynamic_array_synapses_2_w[i];
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_dynamic_array_synapses_2_delay.size(); i++)
                        {
                            _dynamic_array_synapses_2_delay[i] = _static_array__dynamic_array_synapses_2_delay[i];
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_synapses_3_sources; i++)
                        {
                            _array_synapses_3_sources[i] = _static_array__array_synapses_3_sources[i];
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_synapses_3_targets; i++)
                        {
                            _array_synapses_3_targets[i] = _static_array__array_synapses_3_targets[i];
                        }
                        
        _run_synapses_3_synapses_create_array_codeobject();
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_dynamic_array_synapses_3_w.size(); i++)
                        {
                            _dynamic_array_synapses_3_w[i] = _static_array__dynamic_array_synapses_3_w[i];
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_dynamic_array_synapses_3_delay.size(); i++)
                        {
                            _dynamic_array_synapses_3_delay[i] = _static_array__dynamic_array_synapses_3_delay[i];
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_synapses_4_sources; i++)
                        {
                            _array_synapses_4_sources[i] = _static_array__array_synapses_4_sources[i];
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_synapses_4_targets; i++)
                        {
                            _array_synapses_4_targets[i] = _static_array__array_synapses_4_targets[i];
                        }
                        
        _run_synapses_4_synapses_create_array_codeobject();
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_dynamic_array_synapses_4_w.size(); i++)
                        {
                            _dynamic_array_synapses_4_w[i] = _static_array__dynamic_array_synapses_4_w[i];
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_dynamic_array_synapses_4_delay.size(); i++)
                        {
                            _dynamic_array_synapses_4_delay[i] = _static_array__dynamic_array_synapses_4_delay[i];
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_synapses_5_sources; i++)
                        {
                            _array_synapses_5_sources[i] = _static_array__array_synapses_5_sources[i];
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_num__array_synapses_5_targets; i++)
                        {
                            _array_synapses_5_targets[i] = _static_array__array_synapses_5_targets[i];
                        }
                        
        _run_synapses_5_synapses_create_array_codeobject();
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_dynamic_array_synapses_5_w.size(); i++)
                        {
                            _dynamic_array_synapses_5_w[i] = _static_array__dynamic_array_synapses_5_w[i];
                        }
                        
        
                        #pragma omp for schedule(static)
                        for(int i=0; i<_dynamic_array_synapses_5_delay.size(); i++)
                        {
                            _dynamic_array_synapses_5_delay[i] = _static_array__dynamic_array_synapses_5_delay[i];
                        }
                        
        _array_defaultclock_timestep[0] = 0;
        _array_defaultclock_t[0] = 0.0;
        _before_run_synapses_2_pre_push_spikes();
        _before_run_synapses_3_pre_push_spikes();
        _before_run_synapses_4_pre_push_spikes();
        _before_run_synapses_5_pre_push_spikes();
        _before_run_synapses_pre_push_spikes();
        _before_run_synapses_4_post_push_spikes();
        network.clear();
        network.add(&defaultclock, _run_synapses_1_summed_variable_network_rate_post_codeobject);
        network.add(&defaultclock, _run_neurongroup_1_stateupdater_codeobject);
        network.add(&defaultclock, _run_neurongroup_2_stateupdater_codeobject);
        network.add(&defaultclock, _run_neurongroup_stateupdater_codeobject);
        network.add(&defaultclock, _run_synapses_2_stateupdater_codeobject);
        network.add(&defaultclock, _run_neurongroup_1_spike_thresholder_codeobject);
        network.add(&defaultclock, _run_neurongroup_spike_thresholder_codeobject);
        network.add(&defaultclock, _run_spikemonitor_codeobject);
        network.add(&defaultclock, _run_spikemonitor_1_codeobject);
        network.add(&defaultclock, _run_synapses_2_pre_push_spikes);
        network.add(&defaultclock, _run_synapses_2_pre_codeobject);
        network.add(&defaultclock, _run_synapses_3_pre_push_spikes);
        network.add(&defaultclock, _run_synapses_3_pre_codeobject);
        network.add(&defaultclock, _run_synapses_4_pre_push_spikes);
        network.add(&defaultclock, _run_synapses_4_pre_codeobject);
        network.add(&defaultclock, _run_synapses_5_pre_push_spikes);
        network.add(&defaultclock, _run_synapses_5_pre_codeobject);
        network.add(&defaultclock, _run_synapses_pre_push_spikes);
        network.add(&defaultclock, _run_synapses_pre_codeobject);
        network.add(&defaultclock, _run_poissoninput_codeobject);
        network.add(&defaultclock, _run_poissoninput_1_codeobject);
        network.add(&defaultclock, _run_synapses_4_post_push_spikes);
        network.add(&defaultclock, _run_synapses_4_post_codeobject);
        network.add(&defaultclock, _run_neurongroup_1_spike_resetter_codeobject);
        network.add(&defaultclock, _run_neurongroup_spike_resetter_codeobject);
        set_from_command_line(args);
        network.run(2000.0, report_progress, 60.0);
        _after_run_neurongroup_1_spike_thresholder_codeobject();
        _after_run_neurongroup_spike_thresholder_codeobject();
        #ifdef DEBUG
        _debugmsg_spikemonitor_codeobject();
        #endif
        
        #ifdef DEBUG
        _debugmsg_spikemonitor_1_codeobject();
        #endif
        
        #ifdef DEBUG
        _debugmsg_synapses_2_pre_codeobject();
        #endif
        
        #ifdef DEBUG
        _debugmsg_synapses_3_pre_codeobject();
        #endif
        
        #ifdef DEBUG
        _debugmsg_synapses_4_pre_codeobject();
        #endif
        
        #ifdef DEBUG
        _debugmsg_synapses_5_pre_codeobject();
        #endif
        
        #ifdef DEBUG
        _debugmsg_synapses_pre_codeobject();
        #endif
        
        #ifdef DEBUG
        _debugmsg_synapses_4_post_codeobject();
        #endif

	}
        

	brian_end();
        

	return 0;
}