

#include "objects.h"
#include "synapses_classes.h"
#include "brianlib/clocks.h"
#include "brianlib/dynamic_array.h"
#include "brianlib/stdint_compat.h"
#include "network.h"
#include<random>
#include<vector>
#include<iostream>
#include<fstream>
#include<map>
#include<tuple>
#include<cstdlib>
#include<string>

namespace brian {

std::string results_dir = "results/";  // can be overwritten by --results_dir command line arg

// For multhreading, we need one generator for each thread. We also create a distribution for
// each thread, even though this is not strictly necessary for the uniform distribution, as
// the distribution is stateless.
std::vector< RandomGenerator > _random_generators;

//////////////// networks /////////////////
Network network;

void set_variable_from_value(std::string varname, char* var_pointer, size_t size, char value) {
    #ifdef DEBUG
    std::cout << "Setting '" << varname << "' to " << (value == 1 ? "True" : "False") << std::endl;
    #endif
    std::fill(var_pointer, var_pointer+size, value);
}

template<class T> void set_variable_from_value(std::string varname, T* var_pointer, size_t size, T value) {
    #ifdef DEBUG
    std::cout << "Setting '" << varname << "' to " << value << std::endl;
    #endif
    std::fill(var_pointer, var_pointer+size, value);
}

template<class T> void set_variable_from_file(std::string varname, T* var_pointer, size_t data_size, std::string filename) {
    ifstream f;
    streampos size;
    #ifdef DEBUG
    std::cout << "Setting '" << varname << "' from file '" << filename << "'" << std::endl;
    #endif
    f.open(filename, ios::in | ios::binary | ios::ate);
    size = f.tellg();
    if (size != data_size) {
        std::cerr << "Error reading '" << filename << "': file size " << size << " does not match expected size " << data_size << std::endl;
        return;
    }
    f.seekg(0, ios::beg);
    if (f.is_open())
        f.read(reinterpret_cast<char *>(var_pointer), data_size);
    else
        std::cerr << "Could not read '" << filename << "'" << std::endl;
    if (f.fail())
        std::cerr << "Error reading '" << filename << "'" << std::endl;
}

//////////////// set arrays by name ///////
void set_variable_by_name(std::string name, std::string s_value) {
    size_t var_size;
    size_t data_size;
    // C-style or Python-style capitalization is allowed for boolean values
    if (s_value == "true" || s_value == "True")
        s_value = "1";
    else if (s_value == "false" || s_value == "False")
        s_value = "0";
    // non-dynamic arrays
    if (name == "neurongroup_1._spikespace") {
        var_size = 2001;
        data_size = 2001*sizeof(int32_t);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<int32_t>(name, _array_neurongroup_1__spikespace, var_size, (int32_t)atoi(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_1__spikespace, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup_1.a1") {
        var_size = 2000;
        data_size = 2000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_1_a1, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_1_a1, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup_1.a2") {
        var_size = 2000;
        data_size = 2000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_1_a2, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_1_a2, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup_1.basethr") {
        var_size = 2000;
        data_size = 2000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_1_basethr, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_1_basethr, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup_1.ge") {
        var_size = 2000;
        data_size = 2000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_1_ge, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_1_ge, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup_1.gi") {
        var_size = 2000;
        data_size = 2000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_1_gi, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_1_gi, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup_1.gL") {
        var_size = 2000;
        data_size = 2000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_1_gL, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_1_gL, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup_1.H1") {
        var_size = 2000;
        data_size = 2000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_1_H1, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_1_H1, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup_1.H2") {
        var_size = 2000;
        data_size = 2000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_1_H2, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_1_H2, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup_1.inhf") {
        var_size = 2000;
        data_size = 2000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_1_inhf, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_1_inhf, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup_1.lastspike") {
        var_size = 2000;
        data_size = 2000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_1_lastspike, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_1_lastspike, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup_1.max_theta") {
        var_size = 2000;
        data_size = 2000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_1_max_theta, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_1_max_theta, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup_1.network_rate") {
        var_size = 2000;
        data_size = 2000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_1_network_rate, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_1_network_rate, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup_1.neuron_target_rate") {
        var_size = 2000;
        data_size = 2000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_1_neuron_target_rate, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_1_neuron_target_rate, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup_1.not_refractory") {
        var_size = 2000;
        data_size = 2000*sizeof(char);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value(name, _array_neurongroup_1_not_refractory, var_size, (char)atoi(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_1_not_refractory, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup_1.q") {
        var_size = 2000;
        data_size = 2000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_1_q, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_1_q, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup_1.rech") {
        var_size = 2000;
        data_size = 2000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_1_rech, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_1_rech, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup_1.v") {
        var_size = 2000;
        data_size = 2000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_1_v, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_1_v, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup_1.x") {
        var_size = 2000;
        data_size = 2000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_1_x, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_1_x, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup_1.y") {
        var_size = 2000;
        data_size = 2000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_1_y, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_1_y, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup_1.z") {
        var_size = 2000;
        data_size = 2000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_1_z, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_1_z, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup_2.r") {
        var_size = 1;
        data_size = 1*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_2_r, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_2_r, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup._spikespace") {
        var_size = 8001;
        data_size = 8001*sizeof(int32_t);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<int32_t>(name, _array_neurongroup__spikespace, var_size, (int32_t)atoi(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup__spikespace, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup.a1") {
        var_size = 8000;
        data_size = 8000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_a1, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_a1, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup.a2") {
        var_size = 8000;
        data_size = 8000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_a2, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_a2, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup.basethr") {
        var_size = 8000;
        data_size = 8000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_basethr, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_basethr, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup.ge") {
        var_size = 8000;
        data_size = 8000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_ge, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_ge, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup.gi") {
        var_size = 8000;
        data_size = 8000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_gi, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_gi, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup.gL") {
        var_size = 8000;
        data_size = 8000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_gL, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_gL, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup.H1") {
        var_size = 8000;
        data_size = 8000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_H1, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_H1, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup.H2") {
        var_size = 8000;
        data_size = 8000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_H2, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_H2, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup.inhf") {
        var_size = 8000;
        data_size = 8000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_inhf, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_inhf, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup.lastspike") {
        var_size = 8000;
        data_size = 8000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_lastspike, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_lastspike, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup.max_theta") {
        var_size = 8000;
        data_size = 8000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_max_theta, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_max_theta, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup.network_rate") {
        var_size = 8000;
        data_size = 8000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_network_rate, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_network_rate, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup.neuron_target_rate") {
        var_size = 8000;
        data_size = 8000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_neuron_target_rate, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_neuron_target_rate, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup.not_refractory") {
        var_size = 8000;
        data_size = 8000*sizeof(char);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value(name, _array_neurongroup_not_refractory, var_size, (char)atoi(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_not_refractory, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup.q") {
        var_size = 8000;
        data_size = 8000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_q, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_q, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup.rech") {
        var_size = 8000;
        data_size = 8000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_rech, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_rech, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup.v") {
        var_size = 8000;
        data_size = 8000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_v, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_v, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup.x") {
        var_size = 8000;
        data_size = 8000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_x, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_x, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup.y") {
        var_size = 8000;
        data_size = 8000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_y, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_y, data_size, s_value);
        }
        return;
    }
    if (name == "neurongroup.z") {
        var_size = 8000;
        data_size = 8000*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, _array_neurongroup_z, var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, _array_neurongroup_z, data_size, s_value);
        }
        return;
    }
    // dynamic arrays (1d)
    if (name == "synapses_2.delay") {
        var_size = _dynamic_array_synapses_2_delay.size();
        data_size = var_size*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, &_dynamic_array_synapses_2_delay[0], var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, &_dynamic_array_synapses_2_delay[0], data_size, s_value);
        }
        return;
    }
    if (name == "synapses_2.w") {
        var_size = _dynamic_array_synapses_2_w.size();
        data_size = var_size*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, &_dynamic_array_synapses_2_w[0], var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, &_dynamic_array_synapses_2_w[0], data_size, s_value);
        }
        return;
    }
    if (name == "synapses_2.x_std") {
        var_size = _dynamic_array_synapses_2_x_std.size();
        data_size = var_size*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, &_dynamic_array_synapses_2_x_std[0], var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, &_dynamic_array_synapses_2_x_std[0], data_size, s_value);
        }
        return;
    }
    if (name == "synapses_3.delay") {
        var_size = _dynamic_array_synapses_3_delay.size();
        data_size = var_size*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, &_dynamic_array_synapses_3_delay[0], var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, &_dynamic_array_synapses_3_delay[0], data_size, s_value);
        }
        return;
    }
    if (name == "synapses_3.w") {
        var_size = _dynamic_array_synapses_3_w.size();
        data_size = var_size*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, &_dynamic_array_synapses_3_w[0], var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, &_dynamic_array_synapses_3_w[0], data_size, s_value);
        }
        return;
    }
    if (name == "synapses_4.alpha") {
        var_size = _dynamic_array_synapses_4_alpha.size();
        data_size = var_size*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, &_dynamic_array_synapses_4_alpha[0], var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, &_dynamic_array_synapses_4_alpha[0], data_size, s_value);
        }
        return;
    }
    if (name == "synapses_4.delay") {
        var_size = _dynamic_array_synapses_4_delay.size();
        data_size = var_size*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, &_dynamic_array_synapses_4_delay[0], var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, &_dynamic_array_synapses_4_delay[0], data_size, s_value);
        }
        return;
    }
    if (name == "synapses_4.delay") {
        var_size = _dynamic_array_synapses_4_delay_1.size();
        data_size = var_size*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, &_dynamic_array_synapses_4_delay_1[0], var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, &_dynamic_array_synapses_4_delay_1[0], data_size, s_value);
        }
        return;
    }
    if (name == "synapses_4.w") {
        var_size = _dynamic_array_synapses_4_w.size();
        data_size = var_size*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, &_dynamic_array_synapses_4_w[0], var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, &_dynamic_array_synapses_4_w[0], data_size, s_value);
        }
        return;
    }
    if (name == "synapses_5.delay") {
        var_size = _dynamic_array_synapses_5_delay.size();
        data_size = var_size*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, &_dynamic_array_synapses_5_delay[0], var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, &_dynamic_array_synapses_5_delay[0], data_size, s_value);
        }
        return;
    }
    if (name == "synapses_5.w") {
        var_size = _dynamic_array_synapses_5_w.size();
        data_size = var_size*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, &_dynamic_array_synapses_5_w[0], var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, &_dynamic_array_synapses_5_w[0], data_size, s_value);
        }
        return;
    }
    if (name == "synapses.delay") {
        var_size = _dynamic_array_synapses_delay.size();
        data_size = var_size*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, &_dynamic_array_synapses_delay[0], var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, &_dynamic_array_synapses_delay[0], data_size, s_value);
        }
        return;
    }
    if (name == "synapses.w") {
        var_size = _dynamic_array_synapses_w.size();
        data_size = var_size*sizeof(double);
        if (s_value[0] == '-' || (s_value[0] >= '0' && s_value[0] <= '9')) {
            // set from single value
            set_variable_from_value<double>(name, &_dynamic_array_synapses_w[0], var_size, (double)atof(s_value.c_str()));

        } else {
            // set from file
            set_variable_from_file(name, &_dynamic_array_synapses_w[0], data_size, s_value);
        }
        return;
    }
    std::cerr << "Cannot set unknown variable '" << name << "'." << std::endl;
    exit(1);
}
//////////////// arrays ///////////////////
double * _array_defaultclock_dt;
const int _num__array_defaultclock_dt = 1;
double * _array_defaultclock_t;
const int _num__array_defaultclock_t = 1;
int64_t * _array_defaultclock_timestep;
const int _num__array_defaultclock_timestep = 1;
int32_t * _array_neurongroup_1__spikespace;
const int _num__array_neurongroup_1__spikespace = 2001;
double * _array_neurongroup_1_a1;
const int _num__array_neurongroup_1_a1 = 2000;
double * _array_neurongroup_1_a2;
const int _num__array_neurongroup_1_a2 = 2000;
double * _array_neurongroup_1_basethr;
const int _num__array_neurongroup_1_basethr = 2000;
double * _array_neurongroup_1_ge;
const int _num__array_neurongroup_1_ge = 2000;
double * _array_neurongroup_1_gi;
const int _num__array_neurongroup_1_gi = 2000;
double * _array_neurongroup_1_gL;
const int _num__array_neurongroup_1_gL = 2000;
double * _array_neurongroup_1_H1;
const int _num__array_neurongroup_1_H1 = 2000;
double * _array_neurongroup_1_H2;
const int _num__array_neurongroup_1_H2 = 2000;
int32_t * _array_neurongroup_1_i;
const int _num__array_neurongroup_1_i = 2000;
double * _array_neurongroup_1_inhf;
const int _num__array_neurongroup_1_inhf = 2000;
double * _array_neurongroup_1_lastspike;
const int _num__array_neurongroup_1_lastspike = 2000;
double * _array_neurongroup_1_max_theta;
const int _num__array_neurongroup_1_max_theta = 2000;
double * _array_neurongroup_1_network_rate;
const int _num__array_neurongroup_1_network_rate = 2000;
double * _array_neurongroup_1_neuron_target_rate;
const int _num__array_neurongroup_1_neuron_target_rate = 2000;
char * _array_neurongroup_1_not_refractory;
const int _num__array_neurongroup_1_not_refractory = 2000;
double * _array_neurongroup_1_q;
const int _num__array_neurongroup_1_q = 2000;
double * _array_neurongroup_1_rech;
const int _num__array_neurongroup_1_rech = 2000;
double * _array_neurongroup_1_v;
const int _num__array_neurongroup_1_v = 2000;
double * _array_neurongroup_1_x;
const int _num__array_neurongroup_1_x = 2000;
double * _array_neurongroup_1_y;
const int _num__array_neurongroup_1_y = 2000;
double * _array_neurongroup_1_z;
const int _num__array_neurongroup_1_z = 2000;
int32_t * _array_neurongroup_2_i;
const int _num__array_neurongroup_2_i = 1;
double * _array_neurongroup_2_r;
const int _num__array_neurongroup_2_r = 1;
int32_t * _array_neurongroup__spikespace;
const int _num__array_neurongroup__spikespace = 8001;
double * _array_neurongroup_a1;
const int _num__array_neurongroup_a1 = 8000;
double * _array_neurongroup_a2;
const int _num__array_neurongroup_a2 = 8000;
double * _array_neurongroup_basethr;
const int _num__array_neurongroup_basethr = 8000;
double * _array_neurongroup_ge;
const int _num__array_neurongroup_ge = 8000;
double * _array_neurongroup_gi;
const int _num__array_neurongroup_gi = 8000;
double * _array_neurongroup_gL;
const int _num__array_neurongroup_gL = 8000;
double * _array_neurongroup_H1;
const int _num__array_neurongroup_H1 = 8000;
double * _array_neurongroup_H2;
const int _num__array_neurongroup_H2 = 8000;
int32_t * _array_neurongroup_i;
const int _num__array_neurongroup_i = 8000;
double * _array_neurongroup_inhf;
const int _num__array_neurongroup_inhf = 8000;
double * _array_neurongroup_lastspike;
const int _num__array_neurongroup_lastspike = 8000;
double * _array_neurongroup_max_theta;
const int _num__array_neurongroup_max_theta = 8000;
double * _array_neurongroup_network_rate;
const int _num__array_neurongroup_network_rate = 8000;
double * _array_neurongroup_neuron_target_rate;
const int _num__array_neurongroup_neuron_target_rate = 8000;
char * _array_neurongroup_not_refractory;
const int _num__array_neurongroup_not_refractory = 8000;
double * _array_neurongroup_q;
const int _num__array_neurongroup_q = 8000;
double * _array_neurongroup_rech;
const int _num__array_neurongroup_rech = 8000;
double * _array_neurongroup_v;
const int _num__array_neurongroup_v = 8000;
double * _array_neurongroup_x;
const int _num__array_neurongroup_x = 8000;
double * _array_neurongroup_y;
const int _num__array_neurongroup_y = 8000;
double * _array_neurongroup_z;
const int _num__array_neurongroup_z = 8000;
int32_t * _array_spikemonitor_1__source_idx;
const int _num__array_spikemonitor_1__source_idx = 2000;
int32_t * _array_spikemonitor_1_count;
const int _num__array_spikemonitor_1_count = 2000;
int32_t * _array_spikemonitor_1_N;
const int _num__array_spikemonitor_1_N = 1;
int32_t * _array_spikemonitor__source_idx;
const int _num__array_spikemonitor__source_idx = 8000;
int32_t * _array_spikemonitor_count;
const int _num__array_spikemonitor_count = 8000;
int32_t * _array_spikemonitor_N;
const int _num__array_spikemonitor_N = 1;
int32_t * _array_synapses_1_N;
const int _num__array_synapses_1_N = 1;
int32_t * _array_synapses_2_N;
const int _num__array_synapses_2_N = 1;
int32_t * _array_synapses_2_sources;
const int _num__array_synapses_2_sources = 3200000;
int32_t * _array_synapses_2_targets;
const int _num__array_synapses_2_targets = 3200000;
int32_t * _array_synapses_3_N;
const int _num__array_synapses_3_N = 1;
int32_t * _array_synapses_3_sources;
const int _num__array_synapses_3_sources = 1599223;
int32_t * _array_synapses_3_targets;
const int _num__array_synapses_3_targets = 1599223;
int32_t * _array_synapses_4_N;
const int _num__array_synapses_4_N = 1;
int32_t * _array_synapses_4_sources;
const int _num__array_synapses_4_sources = 1598726;
int32_t * _array_synapses_4_targets;
const int _num__array_synapses_4_targets = 1598726;
int32_t * _array_synapses_5_N;
const int _num__array_synapses_5_N = 1;
int32_t * _array_synapses_5_sources;
const int _num__array_synapses_5_sources = 399812;
int32_t * _array_synapses_5_targets;
const int _num__array_synapses_5_targets = 399812;
int32_t * _array_synapses_N;
const int _num__array_synapses_N = 1;

//////////////// dynamic arrays 1d /////////
std::vector<int32_t> _dynamic_array_spikemonitor_1_i;
std::vector<double> _dynamic_array_spikemonitor_1_t;
std::vector<int32_t> _dynamic_array_spikemonitor_i;
std::vector<double> _dynamic_array_spikemonitor_t;
std::vector<int32_t> _dynamic_array_synapses_1__synaptic_post;
std::vector<int32_t> _dynamic_array_synapses_1__synaptic_pre;
std::vector<int32_t> _dynamic_array_synapses_1_N_incoming;
std::vector<int32_t> _dynamic_array_synapses_1_N_outgoing;
std::vector<int32_t> _dynamic_array_synapses_2__synaptic_post;
std::vector<int32_t> _dynamic_array_synapses_2__synaptic_pre;
std::vector<double> _dynamic_array_synapses_2_delay;
std::vector<int32_t> _dynamic_array_synapses_2_N_incoming;
std::vector<int32_t> _dynamic_array_synapses_2_N_outgoing;
std::vector<double> _dynamic_array_synapses_2_w;
std::vector<double> _dynamic_array_synapses_2_x_std;
std::vector<int32_t> _dynamic_array_synapses_3__synaptic_post;
std::vector<int32_t> _dynamic_array_synapses_3__synaptic_pre;
std::vector<double> _dynamic_array_synapses_3_delay;
std::vector<int32_t> _dynamic_array_synapses_3_N_incoming;
std::vector<int32_t> _dynamic_array_synapses_3_N_outgoing;
std::vector<double> _dynamic_array_synapses_3_w;
std::vector<int32_t> _dynamic_array_synapses_4__synaptic_post;
std::vector<int32_t> _dynamic_array_synapses_4__synaptic_pre;
std::vector<double> _dynamic_array_synapses_4_alpha;
std::vector<double> _dynamic_array_synapses_4_delay;
std::vector<double> _dynamic_array_synapses_4_delay_1;
std::vector<int32_t> _dynamic_array_synapses_4_N_incoming;
std::vector<int32_t> _dynamic_array_synapses_4_N_outgoing;
std::vector<double> _dynamic_array_synapses_4_w;
std::vector<int32_t> _dynamic_array_synapses_5__synaptic_post;
std::vector<int32_t> _dynamic_array_synapses_5__synaptic_pre;
std::vector<double> _dynamic_array_synapses_5_delay;
std::vector<int32_t> _dynamic_array_synapses_5_N_incoming;
std::vector<int32_t> _dynamic_array_synapses_5_N_outgoing;
std::vector<double> _dynamic_array_synapses_5_w;
std::vector<int32_t> _dynamic_array_synapses__synaptic_post;
std::vector<int32_t> _dynamic_array_synapses__synaptic_pre;
std::vector<double> _dynamic_array_synapses_delay;
std::vector<int32_t> _dynamic_array_synapses_N_incoming;
std::vector<int32_t> _dynamic_array_synapses_N_outgoing;
std::vector<double> _dynamic_array_synapses_w;

//////////////// dynamic arrays 2d /////////

/////////////// static arrays /////////////
double * _static_array__array_neurongroup_1_v;
const int _num__static_array__array_neurongroup_1_v = 2000;
double * _static_array__array_neurongroup_a1;
const int _num__static_array__array_neurongroup_a1 = 8000;
double * _static_array__array_neurongroup_v;
const int _num__static_array__array_neurongroup_v = 8000;
int32_t * _static_array__array_synapses_2_sources;
const int _num__static_array__array_synapses_2_sources = 3200000;
int32_t * _static_array__array_synapses_2_targets;
const int _num__static_array__array_synapses_2_targets = 3200000;
int32_t * _static_array__array_synapses_3_sources;
const int _num__static_array__array_synapses_3_sources = 1599223;
int32_t * _static_array__array_synapses_3_targets;
const int _num__static_array__array_synapses_3_targets = 1599223;
int32_t * _static_array__array_synapses_4_sources;
const int _num__static_array__array_synapses_4_sources = 1598726;
int32_t * _static_array__array_synapses_4_targets;
const int _num__static_array__array_synapses_4_targets = 1598726;
int32_t * _static_array__array_synapses_5_sources;
const int _num__static_array__array_synapses_5_sources = 399812;
int32_t * _static_array__array_synapses_5_targets;
const int _num__static_array__array_synapses_5_targets = 399812;
double * _static_array__dynamic_array_synapses_2_delay;
const int _num__static_array__dynamic_array_synapses_2_delay = 3200000;
double * _static_array__dynamic_array_synapses_2_w;
const int _num__static_array__dynamic_array_synapses_2_w = 3200000;
double * _static_array__dynamic_array_synapses_3_delay;
const int _num__static_array__dynamic_array_synapses_3_delay = 1599223;
double * _static_array__dynamic_array_synapses_3_w;
const int _num__static_array__dynamic_array_synapses_3_w = 1599223;
double * _static_array__dynamic_array_synapses_4_delay;
const int _num__static_array__dynamic_array_synapses_4_delay = 1598726;
double * _static_array__dynamic_array_synapses_4_w;
const int _num__static_array__dynamic_array_synapses_4_w = 1598726;
double * _static_array__dynamic_array_synapses_5_delay;
const int _num__static_array__dynamic_array_synapses_5_delay = 399812;
double * _static_array__dynamic_array_synapses_5_w;
const int _num__static_array__dynamic_array_synapses_5_w = 399812;

//////////////// synapses /////////////////
// synapses
SynapticPathway synapses_pre(
    _dynamic_array_synapses__synaptic_pre,
    0, 8000);
// synapses_1
// synapses_2
SynapticPathway synapses_2_pre(
    _dynamic_array_synapses_2__synaptic_pre,
    0, 8000);
// synapses_3
SynapticPathway synapses_3_pre(
    _dynamic_array_synapses_3__synaptic_pre,
    0, 8000);
// synapses_4
SynapticPathway synapses_4_post(
    _dynamic_array_synapses_4__synaptic_post,
    0, 8000);
SynapticPathway synapses_4_pre(
    _dynamic_array_synapses_4__synaptic_pre,
    0, 2000);
// synapses_5
SynapticPathway synapses_5_pre(
    _dynamic_array_synapses_5__synaptic_pre,
    0, 2000);

//////////////// clocks ///////////////////
Clock defaultclock;  // attributes will be set in run.cpp

// Profiling information for each code object
}

void _init_arrays()
{
    using namespace brian;

    // Arrays initialized to 0
    _array_defaultclock_dt = new double[1];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<1; i++) _array_defaultclock_dt[i] = 0;

    _array_defaultclock_t = new double[1];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<1; i++) _array_defaultclock_t[i] = 0;

    _array_defaultclock_timestep = new int64_t[1];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<1; i++) _array_defaultclock_timestep[i] = 0;

    _array_neurongroup_1__spikespace = new int32_t[2001];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<2001; i++) _array_neurongroup_1__spikespace[i] = 0;

    _array_neurongroup_1_a1 = new double[2000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<2000; i++) _array_neurongroup_1_a1[i] = 0;

    _array_neurongroup_1_a2 = new double[2000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<2000; i++) _array_neurongroup_1_a2[i] = 0;

    _array_neurongroup_1_basethr = new double[2000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<2000; i++) _array_neurongroup_1_basethr[i] = 0;

    _array_neurongroup_1_ge = new double[2000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<2000; i++) _array_neurongroup_1_ge[i] = 0;

    _array_neurongroup_1_gi = new double[2000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<2000; i++) _array_neurongroup_1_gi[i] = 0;

    _array_neurongroup_1_gL = new double[2000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<2000; i++) _array_neurongroup_1_gL[i] = 0;

    _array_neurongroup_1_H1 = new double[2000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<2000; i++) _array_neurongroup_1_H1[i] = 0;

    _array_neurongroup_1_H2 = new double[2000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<2000; i++) _array_neurongroup_1_H2[i] = 0;

    _array_neurongroup_1_i = new int32_t[2000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<2000; i++) _array_neurongroup_1_i[i] = 0;

    _array_neurongroup_1_inhf = new double[2000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<2000; i++) _array_neurongroup_1_inhf[i] = 0;

    _array_neurongroup_1_lastspike = new double[2000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<2000; i++) _array_neurongroup_1_lastspike[i] = 0;

    _array_neurongroup_1_max_theta = new double[2000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<2000; i++) _array_neurongroup_1_max_theta[i] = 0;

    _array_neurongroup_1_network_rate = new double[2000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<2000; i++) _array_neurongroup_1_network_rate[i] = 0;

    _array_neurongroup_1_neuron_target_rate = new double[2000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<2000; i++) _array_neurongroup_1_neuron_target_rate[i] = 0;

    _array_neurongroup_1_not_refractory = new char[2000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<2000; i++) _array_neurongroup_1_not_refractory[i] = 0;

    _array_neurongroup_1_q = new double[2000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<2000; i++) _array_neurongroup_1_q[i] = 0;

    _array_neurongroup_1_rech = new double[2000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<2000; i++) _array_neurongroup_1_rech[i] = 0;

    _array_neurongroup_1_v = new double[2000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<2000; i++) _array_neurongroup_1_v[i] = 0;

    _array_neurongroup_1_x = new double[2000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<2000; i++) _array_neurongroup_1_x[i] = 0;

    _array_neurongroup_1_y = new double[2000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<2000; i++) _array_neurongroup_1_y[i] = 0;

    _array_neurongroup_1_z = new double[2000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<2000; i++) _array_neurongroup_1_z[i] = 0;

    _array_neurongroup_2_i = new int32_t[1];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<1; i++) _array_neurongroup_2_i[i] = 0;

    _array_neurongroup_2_r = new double[1];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<1; i++) _array_neurongroup_2_r[i] = 0;

    _array_neurongroup__spikespace = new int32_t[8001];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<8001; i++) _array_neurongroup__spikespace[i] = 0;

    _array_neurongroup_a1 = new double[8000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<8000; i++) _array_neurongroup_a1[i] = 0;

    _array_neurongroup_a2 = new double[8000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<8000; i++) _array_neurongroup_a2[i] = 0;

    _array_neurongroup_basethr = new double[8000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<8000; i++) _array_neurongroup_basethr[i] = 0;

    _array_neurongroup_ge = new double[8000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<8000; i++) _array_neurongroup_ge[i] = 0;

    _array_neurongroup_gi = new double[8000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<8000; i++) _array_neurongroup_gi[i] = 0;

    _array_neurongroup_gL = new double[8000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<8000; i++) _array_neurongroup_gL[i] = 0;

    _array_neurongroup_H1 = new double[8000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<8000; i++) _array_neurongroup_H1[i] = 0;

    _array_neurongroup_H2 = new double[8000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<8000; i++) _array_neurongroup_H2[i] = 0;

    _array_neurongroup_i = new int32_t[8000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<8000; i++) _array_neurongroup_i[i] = 0;

    _array_neurongroup_inhf = new double[8000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<8000; i++) _array_neurongroup_inhf[i] = 0;

    _array_neurongroup_lastspike = new double[8000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<8000; i++) _array_neurongroup_lastspike[i] = 0;

    _array_neurongroup_max_theta = new double[8000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<8000; i++) _array_neurongroup_max_theta[i] = 0;

    _array_neurongroup_network_rate = new double[8000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<8000; i++) _array_neurongroup_network_rate[i] = 0;

    _array_neurongroup_neuron_target_rate = new double[8000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<8000; i++) _array_neurongroup_neuron_target_rate[i] = 0;

    _array_neurongroup_not_refractory = new char[8000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<8000; i++) _array_neurongroup_not_refractory[i] = 0;

    _array_neurongroup_q = new double[8000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<8000; i++) _array_neurongroup_q[i] = 0;

    _array_neurongroup_rech = new double[8000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<8000; i++) _array_neurongroup_rech[i] = 0;

    _array_neurongroup_v = new double[8000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<8000; i++) _array_neurongroup_v[i] = 0;

    _array_neurongroup_x = new double[8000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<8000; i++) _array_neurongroup_x[i] = 0;

    _array_neurongroup_y = new double[8000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<8000; i++) _array_neurongroup_y[i] = 0;

    _array_neurongroup_z = new double[8000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<8000; i++) _array_neurongroup_z[i] = 0;

    _array_spikemonitor_1__source_idx = new int32_t[2000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<2000; i++) _array_spikemonitor_1__source_idx[i] = 0;

    _array_spikemonitor_1_count = new int32_t[2000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<2000; i++) _array_spikemonitor_1_count[i] = 0;

    _array_spikemonitor_1_N = new int32_t[1];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<1; i++) _array_spikemonitor_1_N[i] = 0;

    _array_spikemonitor__source_idx = new int32_t[8000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<8000; i++) _array_spikemonitor__source_idx[i] = 0;

    _array_spikemonitor_count = new int32_t[8000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<8000; i++) _array_spikemonitor_count[i] = 0;

    _array_spikemonitor_N = new int32_t[1];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<1; i++) _array_spikemonitor_N[i] = 0;

    _array_synapses_1_N = new int32_t[1];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<1; i++) _array_synapses_1_N[i] = 0;

    _array_synapses_2_N = new int32_t[1];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<1; i++) _array_synapses_2_N[i] = 0;

    _array_synapses_2_sources = new int32_t[3200000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<3200000; i++) _array_synapses_2_sources[i] = 0;

    _array_synapses_2_targets = new int32_t[3200000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<3200000; i++) _array_synapses_2_targets[i] = 0;

    _array_synapses_3_N = new int32_t[1];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<1; i++) _array_synapses_3_N[i] = 0;

    _array_synapses_3_sources = new int32_t[1599223];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<1599223; i++) _array_synapses_3_sources[i] = 0;

    _array_synapses_3_targets = new int32_t[1599223];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<1599223; i++) _array_synapses_3_targets[i] = 0;

    _array_synapses_4_N = new int32_t[1];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<1; i++) _array_synapses_4_N[i] = 0;

    _array_synapses_4_sources = new int32_t[1598726];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<1598726; i++) _array_synapses_4_sources[i] = 0;

    _array_synapses_4_targets = new int32_t[1598726];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<1598726; i++) _array_synapses_4_targets[i] = 0;

    _array_synapses_5_N = new int32_t[1];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<1; i++) _array_synapses_5_N[i] = 0;

    _array_synapses_5_sources = new int32_t[399812];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<399812; i++) _array_synapses_5_sources[i] = 0;

    _array_synapses_5_targets = new int32_t[399812];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<399812; i++) _array_synapses_5_targets[i] = 0;

    _array_synapses_N = new int32_t[1];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<1; i++) _array_synapses_N[i] = 0;


    // Arrays initialized to an "arange"
    _array_neurongroup_1_i = new int32_t[2000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<2000; i++) _array_neurongroup_1_i[i] = 0 + i;

    _array_neurongroup_2_i = new int32_t[1];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<1; i++) _array_neurongroup_2_i[i] = 0 + i;

    _array_neurongroup_i = new int32_t[8000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<8000; i++) _array_neurongroup_i[i] = 0 + i;

    _array_spikemonitor_1__source_idx = new int32_t[2000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<2000; i++) _array_spikemonitor_1__source_idx[i] = 0 + i;

    _array_spikemonitor__source_idx = new int32_t[8000];
    #pragma omp parallel for schedule(static)
    for(int i=0; i<8000; i++) _array_spikemonitor__source_idx[i] = 0 + i;


    // static arrays
    _static_array__array_neurongroup_1_v = new double[2000];
    _static_array__array_neurongroup_a1 = new double[8000];
    _static_array__array_neurongroup_v = new double[8000];
    _static_array__array_synapses_2_sources = new int32_t[3200000];
    _static_array__array_synapses_2_targets = new int32_t[3200000];
    _static_array__array_synapses_3_sources = new int32_t[1599223];
    _static_array__array_synapses_3_targets = new int32_t[1599223];
    _static_array__array_synapses_4_sources = new int32_t[1598726];
    _static_array__array_synapses_4_targets = new int32_t[1598726];
    _static_array__array_synapses_5_sources = new int32_t[399812];
    _static_array__array_synapses_5_targets = new int32_t[399812];
    _static_array__dynamic_array_synapses_2_delay = new double[3200000];
    _static_array__dynamic_array_synapses_2_w = new double[3200000];
    _static_array__dynamic_array_synapses_3_delay = new double[1599223];
    _static_array__dynamic_array_synapses_3_w = new double[1599223];
    _static_array__dynamic_array_synapses_4_delay = new double[1598726];
    _static_array__dynamic_array_synapses_4_w = new double[1598726];
    _static_array__dynamic_array_synapses_5_delay = new double[399812];
    _static_array__dynamic_array_synapses_5_w = new double[399812];

    // Random number generator states
    std::random_device rd;
    for (int i=0; i<8; i++)
        _random_generators.push_back(RandomGenerator());
}

void _load_arrays()
{
    using namespace brian;

    ifstream f_static_array__array_neurongroup_1_v;
    f_static_array__array_neurongroup_1_v.open("static_arrays/_static_array__array_neurongroup_1_v", ios::in | ios::binary);
    if(f_static_array__array_neurongroup_1_v.is_open())
    {
        f_static_array__array_neurongroup_1_v.read(reinterpret_cast<char*>(_static_array__array_neurongroup_1_v), 2000*sizeof(double));
    } else
    {
        std::cout << "Error opening static array _static_array__array_neurongroup_1_v." << endl;
    }
    ifstream f_static_array__array_neurongroup_a1;
    f_static_array__array_neurongroup_a1.open("static_arrays/_static_array__array_neurongroup_a1", ios::in | ios::binary);
    if(f_static_array__array_neurongroup_a1.is_open())
    {
        f_static_array__array_neurongroup_a1.read(reinterpret_cast<char*>(_static_array__array_neurongroup_a1), 8000*sizeof(double));
    } else
    {
        std::cout << "Error opening static array _static_array__array_neurongroup_a1." << endl;
    }
    ifstream f_static_array__array_neurongroup_v;
    f_static_array__array_neurongroup_v.open("static_arrays/_static_array__array_neurongroup_v", ios::in | ios::binary);
    if(f_static_array__array_neurongroup_v.is_open())
    {
        f_static_array__array_neurongroup_v.read(reinterpret_cast<char*>(_static_array__array_neurongroup_v), 8000*sizeof(double));
    } else
    {
        std::cout << "Error opening static array _static_array__array_neurongroup_v." << endl;
    }
    ifstream f_static_array__array_synapses_2_sources;
    f_static_array__array_synapses_2_sources.open("static_arrays/_static_array__array_synapses_2_sources", ios::in | ios::binary);
    if(f_static_array__array_synapses_2_sources.is_open())
    {
        f_static_array__array_synapses_2_sources.read(reinterpret_cast<char*>(_static_array__array_synapses_2_sources), 3200000*sizeof(int32_t));
    } else
    {
        std::cout << "Error opening static array _static_array__array_synapses_2_sources." << endl;
    }
    ifstream f_static_array__array_synapses_2_targets;
    f_static_array__array_synapses_2_targets.open("static_arrays/_static_array__array_synapses_2_targets", ios::in | ios::binary);
    if(f_static_array__array_synapses_2_targets.is_open())
    {
        f_static_array__array_synapses_2_targets.read(reinterpret_cast<char*>(_static_array__array_synapses_2_targets), 3200000*sizeof(int32_t));
    } else
    {
        std::cout << "Error opening static array _static_array__array_synapses_2_targets." << endl;
    }
    ifstream f_static_array__array_synapses_3_sources;
    f_static_array__array_synapses_3_sources.open("static_arrays/_static_array__array_synapses_3_sources", ios::in | ios::binary);
    if(f_static_array__array_synapses_3_sources.is_open())
    {
        f_static_array__array_synapses_3_sources.read(reinterpret_cast<char*>(_static_array__array_synapses_3_sources), 1599223*sizeof(int32_t));
    } else
    {
        std::cout << "Error opening static array _static_array__array_synapses_3_sources." << endl;
    }
    ifstream f_static_array__array_synapses_3_targets;
    f_static_array__array_synapses_3_targets.open("static_arrays/_static_array__array_synapses_3_targets", ios::in | ios::binary);
    if(f_static_array__array_synapses_3_targets.is_open())
    {
        f_static_array__array_synapses_3_targets.read(reinterpret_cast<char*>(_static_array__array_synapses_3_targets), 1599223*sizeof(int32_t));
    } else
    {
        std::cout << "Error opening static array _static_array__array_synapses_3_targets." << endl;
    }
    ifstream f_static_array__array_synapses_4_sources;
    f_static_array__array_synapses_4_sources.open("static_arrays/_static_array__array_synapses_4_sources", ios::in | ios::binary);
    if(f_static_array__array_synapses_4_sources.is_open())
    {
        f_static_array__array_synapses_4_sources.read(reinterpret_cast<char*>(_static_array__array_synapses_4_sources), 1598726*sizeof(int32_t));
    } else
    {
        std::cout << "Error opening static array _static_array__array_synapses_4_sources." << endl;
    }
    ifstream f_static_array__array_synapses_4_targets;
    f_static_array__array_synapses_4_targets.open("static_arrays/_static_array__array_synapses_4_targets", ios::in | ios::binary);
    if(f_static_array__array_synapses_4_targets.is_open())
    {
        f_static_array__array_synapses_4_targets.read(reinterpret_cast<char*>(_static_array__array_synapses_4_targets), 1598726*sizeof(int32_t));
    } else
    {
        std::cout << "Error opening static array _static_array__array_synapses_4_targets." << endl;
    }
    ifstream f_static_array__array_synapses_5_sources;
    f_static_array__array_synapses_5_sources.open("static_arrays/_static_array__array_synapses_5_sources", ios::in | ios::binary);
    if(f_static_array__array_synapses_5_sources.is_open())
    {
        f_static_array__array_synapses_5_sources.read(reinterpret_cast<char*>(_static_array__array_synapses_5_sources), 399812*sizeof(int32_t));
    } else
    {
        std::cout << "Error opening static array _static_array__array_synapses_5_sources." << endl;
    }
    ifstream f_static_array__array_synapses_5_targets;
    f_static_array__array_synapses_5_targets.open("static_arrays/_static_array__array_synapses_5_targets", ios::in | ios::binary);
    if(f_static_array__array_synapses_5_targets.is_open())
    {
        f_static_array__array_synapses_5_targets.read(reinterpret_cast<char*>(_static_array__array_synapses_5_targets), 399812*sizeof(int32_t));
    } else
    {
        std::cout << "Error opening static array _static_array__array_synapses_5_targets." << endl;
    }
    ifstream f_static_array__dynamic_array_synapses_2_delay;
    f_static_array__dynamic_array_synapses_2_delay.open("static_arrays/_static_array__dynamic_array_synapses_2_delay", ios::in | ios::binary);
    if(f_static_array__dynamic_array_synapses_2_delay.is_open())
    {
        f_static_array__dynamic_array_synapses_2_delay.read(reinterpret_cast<char*>(_static_array__dynamic_array_synapses_2_delay), 3200000*sizeof(double));
    } else
    {
        std::cout << "Error opening static array _static_array__dynamic_array_synapses_2_delay." << endl;
    }
    ifstream f_static_array__dynamic_array_synapses_2_w;
    f_static_array__dynamic_array_synapses_2_w.open("static_arrays/_static_array__dynamic_array_synapses_2_w", ios::in | ios::binary);
    if(f_static_array__dynamic_array_synapses_2_w.is_open())
    {
        f_static_array__dynamic_array_synapses_2_w.read(reinterpret_cast<char*>(_static_array__dynamic_array_synapses_2_w), 3200000*sizeof(double));
    } else
    {
        std::cout << "Error opening static array _static_array__dynamic_array_synapses_2_w." << endl;
    }
    ifstream f_static_array__dynamic_array_synapses_3_delay;
    f_static_array__dynamic_array_synapses_3_delay.open("static_arrays/_static_array__dynamic_array_synapses_3_delay", ios::in | ios::binary);
    if(f_static_array__dynamic_array_synapses_3_delay.is_open())
    {
        f_static_array__dynamic_array_synapses_3_delay.read(reinterpret_cast<char*>(_static_array__dynamic_array_synapses_3_delay), 1599223*sizeof(double));
    } else
    {
        std::cout << "Error opening static array _static_array__dynamic_array_synapses_3_delay." << endl;
    }
    ifstream f_static_array__dynamic_array_synapses_3_w;
    f_static_array__dynamic_array_synapses_3_w.open("static_arrays/_static_array__dynamic_array_synapses_3_w", ios::in | ios::binary);
    if(f_static_array__dynamic_array_synapses_3_w.is_open())
    {
        f_static_array__dynamic_array_synapses_3_w.read(reinterpret_cast<char*>(_static_array__dynamic_array_synapses_3_w), 1599223*sizeof(double));
    } else
    {
        std::cout << "Error opening static array _static_array__dynamic_array_synapses_3_w." << endl;
    }
    ifstream f_static_array__dynamic_array_synapses_4_delay;
    f_static_array__dynamic_array_synapses_4_delay.open("static_arrays/_static_array__dynamic_array_synapses_4_delay", ios::in | ios::binary);
    if(f_static_array__dynamic_array_synapses_4_delay.is_open())
    {
        f_static_array__dynamic_array_synapses_4_delay.read(reinterpret_cast<char*>(_static_array__dynamic_array_synapses_4_delay), 1598726*sizeof(double));
    } else
    {
        std::cout << "Error opening static array _static_array__dynamic_array_synapses_4_delay." << endl;
    }
    ifstream f_static_array__dynamic_array_synapses_4_w;
    f_static_array__dynamic_array_synapses_4_w.open("static_arrays/_static_array__dynamic_array_synapses_4_w", ios::in | ios::binary);
    if(f_static_array__dynamic_array_synapses_4_w.is_open())
    {
        f_static_array__dynamic_array_synapses_4_w.read(reinterpret_cast<char*>(_static_array__dynamic_array_synapses_4_w), 1598726*sizeof(double));
    } else
    {
        std::cout << "Error opening static array _static_array__dynamic_array_synapses_4_w." << endl;
    }
    ifstream f_static_array__dynamic_array_synapses_5_delay;
    f_static_array__dynamic_array_synapses_5_delay.open("static_arrays/_static_array__dynamic_array_synapses_5_delay", ios::in | ios::binary);
    if(f_static_array__dynamic_array_synapses_5_delay.is_open())
    {
        f_static_array__dynamic_array_synapses_5_delay.read(reinterpret_cast<char*>(_static_array__dynamic_array_synapses_5_delay), 399812*sizeof(double));
    } else
    {
        std::cout << "Error opening static array _static_array__dynamic_array_synapses_5_delay." << endl;
    }
    ifstream f_static_array__dynamic_array_synapses_5_w;
    f_static_array__dynamic_array_synapses_5_w.open("static_arrays/_static_array__dynamic_array_synapses_5_w", ios::in | ios::binary);
    if(f_static_array__dynamic_array_synapses_5_w.is_open())
    {
        f_static_array__dynamic_array_synapses_5_w.read(reinterpret_cast<char*>(_static_array__dynamic_array_synapses_5_w), 399812*sizeof(double));
    } else
    {
        std::cout << "Error opening static array _static_array__dynamic_array_synapses_5_w." << endl;
    }
}

void _write_arrays()
{
    using namespace brian;

    ofstream outfile__array_defaultclock_dt;
    outfile__array_defaultclock_dt.open(results_dir + "_array_defaultclock_dt_1978099143", ios::binary | ios::out);
    if(outfile__array_defaultclock_dt.is_open())
    {
        outfile__array_defaultclock_dt.write(reinterpret_cast<char*>(_array_defaultclock_dt), 1*sizeof(_array_defaultclock_dt[0]));
        outfile__array_defaultclock_dt.close();
    } else
    {
        std::cout << "Error writing output file for _array_defaultclock_dt." << endl;
    }
    ofstream outfile__array_defaultclock_t;
    outfile__array_defaultclock_t.open(results_dir + "_array_defaultclock_t_2669362164", ios::binary | ios::out);
    if(outfile__array_defaultclock_t.is_open())
    {
        outfile__array_defaultclock_t.write(reinterpret_cast<char*>(_array_defaultclock_t), 1*sizeof(_array_defaultclock_t[0]));
        outfile__array_defaultclock_t.close();
    } else
    {
        std::cout << "Error writing output file for _array_defaultclock_t." << endl;
    }
    ofstream outfile__array_defaultclock_timestep;
    outfile__array_defaultclock_timestep.open(results_dir + "_array_defaultclock_timestep_144223508", ios::binary | ios::out);
    if(outfile__array_defaultclock_timestep.is_open())
    {
        outfile__array_defaultclock_timestep.write(reinterpret_cast<char*>(_array_defaultclock_timestep), 1*sizeof(_array_defaultclock_timestep[0]));
        outfile__array_defaultclock_timestep.close();
    } else
    {
        std::cout << "Error writing output file for _array_defaultclock_timestep." << endl;
    }
    ofstream outfile__array_neurongroup_1__spikespace;
    outfile__array_neurongroup_1__spikespace.open(results_dir + "_array_neurongroup_1__spikespace_3155027917", ios::binary | ios::out);
    if(outfile__array_neurongroup_1__spikespace.is_open())
    {
        outfile__array_neurongroup_1__spikespace.write(reinterpret_cast<char*>(_array_neurongroup_1__spikespace), 2001*sizeof(_array_neurongroup_1__spikespace[0]));
        outfile__array_neurongroup_1__spikespace.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_1__spikespace." << endl;
    }
    ofstream outfile__array_neurongroup_1_a1;
    outfile__array_neurongroup_1_a1.open(results_dir + "_array_neurongroup_1_a1_4040499342", ios::binary | ios::out);
    if(outfile__array_neurongroup_1_a1.is_open())
    {
        outfile__array_neurongroup_1_a1.write(reinterpret_cast<char*>(_array_neurongroup_1_a1), 2000*sizeof(_array_neurongroup_1_a1[0]));
        outfile__array_neurongroup_1_a1.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_1_a1." << endl;
    }
    ofstream outfile__array_neurongroup_1_a2;
    outfile__array_neurongroup_1_a2.open(results_dir + "_array_neurongroup_1_a2_1776054580", ios::binary | ios::out);
    if(outfile__array_neurongroup_1_a2.is_open())
    {
        outfile__array_neurongroup_1_a2.write(reinterpret_cast<char*>(_array_neurongroup_1_a2), 2000*sizeof(_array_neurongroup_1_a2[0]));
        outfile__array_neurongroup_1_a2.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_1_a2." << endl;
    }
    ofstream outfile__array_neurongroup_1_basethr;
    outfile__array_neurongroup_1_basethr.open(results_dir + "_array_neurongroup_1_basethr_939785921", ios::binary | ios::out);
    if(outfile__array_neurongroup_1_basethr.is_open())
    {
        outfile__array_neurongroup_1_basethr.write(reinterpret_cast<char*>(_array_neurongroup_1_basethr), 2000*sizeof(_array_neurongroup_1_basethr[0]));
        outfile__array_neurongroup_1_basethr.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_1_basethr." << endl;
    }
    ofstream outfile__array_neurongroup_1_ge;
    outfile__array_neurongroup_1_ge.open(results_dir + "_array_neurongroup_1_ge_3397980901", ios::binary | ios::out);
    if(outfile__array_neurongroup_1_ge.is_open())
    {
        outfile__array_neurongroup_1_ge.write(reinterpret_cast<char*>(_array_neurongroup_1_ge), 2000*sizeof(_array_neurongroup_1_ge[0]));
        outfile__array_neurongroup_1_ge.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_1_ge." << endl;
    }
    ofstream outfile__array_neurongroup_1_gi;
    outfile__array_neurongroup_1_gi.open(results_dir + "_array_neurongroup_1_gi_3275710158", ios::binary | ios::out);
    if(outfile__array_neurongroup_1_gi.is_open())
    {
        outfile__array_neurongroup_1_gi.write(reinterpret_cast<char*>(_array_neurongroup_1_gi), 2000*sizeof(_array_neurongroup_1_gi[0]));
        outfile__array_neurongroup_1_gi.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_1_gi." << endl;
    }
    ofstream outfile__array_neurongroup_1_gL;
    outfile__array_neurongroup_1_gL.open(results_dir + "_array_neurongroup_1_gL_2285603465", ios::binary | ios::out);
    if(outfile__array_neurongroup_1_gL.is_open())
    {
        outfile__array_neurongroup_1_gL.write(reinterpret_cast<char*>(_array_neurongroup_1_gL), 2000*sizeof(_array_neurongroup_1_gL[0]));
        outfile__array_neurongroup_1_gL.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_1_gL." << endl;
    }
    ofstream outfile__array_neurongroup_1_H1;
    outfile__array_neurongroup_1_H1.open(results_dir + "_array_neurongroup_1_H1_3029581669", ios::binary | ios::out);
    if(outfile__array_neurongroup_1_H1.is_open())
    {
        outfile__array_neurongroup_1_H1.write(reinterpret_cast<char*>(_array_neurongroup_1_H1), 2000*sizeof(_array_neurongroup_1_H1[0]));
        outfile__array_neurongroup_1_H1.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_1_H1." << endl;
    }
    ofstream outfile__array_neurongroup_1_H2;
    outfile__array_neurongroup_1_H2.open(results_dir + "_array_neurongroup_1_H2_765128415", ios::binary | ios::out);
    if(outfile__array_neurongroup_1_H2.is_open())
    {
        outfile__array_neurongroup_1_H2.write(reinterpret_cast<char*>(_array_neurongroup_1_H2), 2000*sizeof(_array_neurongroup_1_H2[0]));
        outfile__array_neurongroup_1_H2.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_1_H2." << endl;
    }
    ofstream outfile__array_neurongroup_1_i;
    outfile__array_neurongroup_1_i.open(results_dir + "_array_neurongroup_1_i_3674354357", ios::binary | ios::out);
    if(outfile__array_neurongroup_1_i.is_open())
    {
        outfile__array_neurongroup_1_i.write(reinterpret_cast<char*>(_array_neurongroup_1_i), 2000*sizeof(_array_neurongroup_1_i[0]));
        outfile__array_neurongroup_1_i.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_1_i." << endl;
    }
    ofstream outfile__array_neurongroup_1_inhf;
    outfile__array_neurongroup_1_inhf.open(results_dir + "_array_neurongroup_1_inhf_1947603847", ios::binary | ios::out);
    if(outfile__array_neurongroup_1_inhf.is_open())
    {
        outfile__array_neurongroup_1_inhf.write(reinterpret_cast<char*>(_array_neurongroup_1_inhf), 2000*sizeof(_array_neurongroup_1_inhf[0]));
        outfile__array_neurongroup_1_inhf.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_1_inhf." << endl;
    }
    ofstream outfile__array_neurongroup_1_lastspike;
    outfile__array_neurongroup_1_lastspike.open(results_dir + "_array_neurongroup_1_lastspike_1163579662", ios::binary | ios::out);
    if(outfile__array_neurongroup_1_lastspike.is_open())
    {
        outfile__array_neurongroup_1_lastspike.write(reinterpret_cast<char*>(_array_neurongroup_1_lastspike), 2000*sizeof(_array_neurongroup_1_lastspike[0]));
        outfile__array_neurongroup_1_lastspike.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_1_lastspike." << endl;
    }
    ofstream outfile__array_neurongroup_1_max_theta;
    outfile__array_neurongroup_1_max_theta.open(results_dir + "_array_neurongroup_1_max_theta_3656250561", ios::binary | ios::out);
    if(outfile__array_neurongroup_1_max_theta.is_open())
    {
        outfile__array_neurongroup_1_max_theta.write(reinterpret_cast<char*>(_array_neurongroup_1_max_theta), 2000*sizeof(_array_neurongroup_1_max_theta[0]));
        outfile__array_neurongroup_1_max_theta.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_1_max_theta." << endl;
    }
    ofstream outfile__array_neurongroup_1_network_rate;
    outfile__array_neurongroup_1_network_rate.open(results_dir + "_array_neurongroup_1_network_rate_910368604", ios::binary | ios::out);
    if(outfile__array_neurongroup_1_network_rate.is_open())
    {
        outfile__array_neurongroup_1_network_rate.write(reinterpret_cast<char*>(_array_neurongroup_1_network_rate), 2000*sizeof(_array_neurongroup_1_network_rate[0]));
        outfile__array_neurongroup_1_network_rate.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_1_network_rate." << endl;
    }
    ofstream outfile__array_neurongroup_1_neuron_target_rate;
    outfile__array_neurongroup_1_neuron_target_rate.open(results_dir + "_array_neurongroup_1_neuron_target_rate_4278302497", ios::binary | ios::out);
    if(outfile__array_neurongroup_1_neuron_target_rate.is_open())
    {
        outfile__array_neurongroup_1_neuron_target_rate.write(reinterpret_cast<char*>(_array_neurongroup_1_neuron_target_rate), 2000*sizeof(_array_neurongroup_1_neuron_target_rate[0]));
        outfile__array_neurongroup_1_neuron_target_rate.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_1_neuron_target_rate." << endl;
    }
    ofstream outfile__array_neurongroup_1_not_refractory;
    outfile__array_neurongroup_1_not_refractory.open(results_dir + "_array_neurongroup_1_not_refractory_897855399", ios::binary | ios::out);
    if(outfile__array_neurongroup_1_not_refractory.is_open())
    {
        outfile__array_neurongroup_1_not_refractory.write(reinterpret_cast<char*>(_array_neurongroup_1_not_refractory), 2000*sizeof(_array_neurongroup_1_not_refractory[0]));
        outfile__array_neurongroup_1_not_refractory.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_1_not_refractory." << endl;
    }
    ofstream outfile__array_neurongroup_1_q;
    outfile__array_neurongroup_1_q.open(results_dir + "_array_neurongroup_1_q_3362695907", ios::binary | ios::out);
    if(outfile__array_neurongroup_1_q.is_open())
    {
        outfile__array_neurongroup_1_q.write(reinterpret_cast<char*>(_array_neurongroup_1_q), 2000*sizeof(_array_neurongroup_1_q[0]));
        outfile__array_neurongroup_1_q.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_1_q." << endl;
    }
    ofstream outfile__array_neurongroup_1_rech;
    outfile__array_neurongroup_1_rech.open(results_dir + "_array_neurongroup_1_rech_4212643892", ios::binary | ios::out);
    if(outfile__array_neurongroup_1_rech.is_open())
    {
        outfile__array_neurongroup_1_rech.write(reinterpret_cast<char*>(_array_neurongroup_1_rech), 2000*sizeof(_array_neurongroup_1_rech[0]));
        outfile__array_neurongroup_1_rech.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_1_rech." << endl;
    }
    ofstream outfile__array_neurongroup_1_v;
    outfile__array_neurongroup_1_v.open(results_dir + "_array_neurongroup_1_v_1443512128", ios::binary | ios::out);
    if(outfile__array_neurongroup_1_v.is_open())
    {
        outfile__array_neurongroup_1_v.write(reinterpret_cast<char*>(_array_neurongroup_1_v), 2000*sizeof(_array_neurongroup_1_v[0]));
        outfile__array_neurongroup_1_v.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_1_v." << endl;
    }
    ofstream outfile__array_neurongroup_1_x;
    outfile__array_neurongroup_1_x.open(results_dir + "_array_neurongroup_1_x_2981237319", ios::binary | ios::out);
    if(outfile__array_neurongroup_1_x.is_open())
    {
        outfile__array_neurongroup_1_x.write(reinterpret_cast<char*>(_array_neurongroup_1_x), 2000*sizeof(_array_neurongroup_1_x[0]));
        outfile__array_neurongroup_1_x.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_1_x." << endl;
    }
    ofstream outfile__array_neurongroup_1_y;
    outfile__array_neurongroup_1_y.open(results_dir + "_array_neurongroup_1_y_3333759697", ios::binary | ios::out);
    if(outfile__array_neurongroup_1_y.is_open())
    {
        outfile__array_neurongroup_1_y.write(reinterpret_cast<char*>(_array_neurongroup_1_y), 2000*sizeof(_array_neurongroup_1_y[0]));
        outfile__array_neurongroup_1_y.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_1_y." << endl;
    }
    ofstream outfile__array_neurongroup_1_z;
    outfile__array_neurongroup_1_z.open(results_dir + "_array_neurongroup_1_z_1606185835", ios::binary | ios::out);
    if(outfile__array_neurongroup_1_z.is_open())
    {
        outfile__array_neurongroup_1_z.write(reinterpret_cast<char*>(_array_neurongroup_1_z), 2000*sizeof(_array_neurongroup_1_z[0]));
        outfile__array_neurongroup_1_z.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_1_z." << endl;
    }
    ofstream outfile__array_neurongroup_2_i;
    outfile__array_neurongroup_2_i.open(results_dir + "_array_neurongroup_2_i_3645148396", ios::binary | ios::out);
    if(outfile__array_neurongroup_2_i.is_open())
    {
        outfile__array_neurongroup_2_i.write(reinterpret_cast<char*>(_array_neurongroup_2_i), 1*sizeof(_array_neurongroup_2_i[0]));
        outfile__array_neurongroup_2_i.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_2_i." << endl;
    }
    ofstream outfile__array_neurongroup_2_r;
    outfile__array_neurongroup_2_r.open(results_dir + "_array_neurongroup_2_r_1394689280", ios::binary | ios::out);
    if(outfile__array_neurongroup_2_r.is_open())
    {
        outfile__array_neurongroup_2_r.write(reinterpret_cast<char*>(_array_neurongroup_2_r), 1*sizeof(_array_neurongroup_2_r[0]));
        outfile__array_neurongroup_2_r.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_2_r." << endl;
    }
    ofstream outfile__array_neurongroup__spikespace;
    outfile__array_neurongroup__spikespace.open(results_dir + "_array_neurongroup__spikespace_3522821529", ios::binary | ios::out);
    if(outfile__array_neurongroup__spikespace.is_open())
    {
        outfile__array_neurongroup__spikespace.write(reinterpret_cast<char*>(_array_neurongroup__spikespace), 8001*sizeof(_array_neurongroup__spikespace[0]));
        outfile__array_neurongroup__spikespace.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup__spikespace." << endl;
    }
    ofstream outfile__array_neurongroup_a1;
    outfile__array_neurongroup_a1.open(results_dir + "_array_neurongroup_a1_2787115630", ios::binary | ios::out);
    if(outfile__array_neurongroup_a1.is_open())
    {
        outfile__array_neurongroup_a1.write(reinterpret_cast<char*>(_array_neurongroup_a1), 8000*sizeof(_array_neurongroup_a1[0]));
        outfile__array_neurongroup_a1.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_a1." << endl;
    }
    ofstream outfile__array_neurongroup_a2;
    outfile__array_neurongroup_a2.open(results_dir + "_array_neurongroup_a2_1059673044", ios::binary | ios::out);
    if(outfile__array_neurongroup_a2.is_open())
    {
        outfile__array_neurongroup_a2.write(reinterpret_cast<char*>(_array_neurongroup_a2), 8000*sizeof(_array_neurongroup_a2[0]));
        outfile__array_neurongroup_a2.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_a2." << endl;
    }
    ofstream outfile__array_neurongroup_basethr;
    outfile__array_neurongroup_basethr.open(results_dir + "_array_neurongroup_basethr_1157764418", ios::binary | ios::out);
    if(outfile__array_neurongroup_basethr.is_open())
    {
        outfile__array_neurongroup_basethr.write(reinterpret_cast<char*>(_array_neurongroup_basethr), 8000*sizeof(_array_neurongroup_basethr[0]));
        outfile__array_neurongroup_basethr.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_basethr." << endl;
    }
    ofstream outfile__array_neurongroup_ge;
    outfile__array_neurongroup_ge.open(results_dir + "_array_neurongroup_ge_2625384453", ios::binary | ios::out);
    if(outfile__array_neurongroup_ge.is_open())
    {
        outfile__array_neurongroup_ge.write(reinterpret_cast<char*>(_array_neurongroup_ge), 8000*sizeof(_array_neurongroup_ge[0]));
        outfile__array_neurongroup_ge.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_ge." << endl;
    }
    ofstream outfile__array_neurongroup_gi;
    outfile__array_neurongroup_gi.open(results_dir + "_array_neurongroup_gi_2513075246", ios::binary | ios::out);
    if(outfile__array_neurongroup_gi.is_open())
    {
        outfile__array_neurongroup_gi.write(reinterpret_cast<char*>(_array_neurongroup_gi), 8000*sizeof(_array_neurongroup_gi[0]));
        outfile__array_neurongroup_gi.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_gi." << endl;
    }
    ofstream outfile__array_neurongroup_gL;
    outfile__array_neurongroup_gL.open(results_dir + "_array_neurongroup_gL_3738085481", ios::binary | ios::out);
    if(outfile__array_neurongroup_gL.is_open())
    {
        outfile__array_neurongroup_gL.write(reinterpret_cast<char*>(_array_neurongroup_gL), 8000*sizeof(_array_neurongroup_gL[0]));
        outfile__array_neurongroup_gL.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_gL." << endl;
    }
    ofstream outfile__array_neurongroup_H1;
    outfile__array_neurongroup_H1.open(results_dir + "_array_neurongroup_H1_3798375813", ios::binary | ios::out);
    if(outfile__array_neurongroup_H1.is_open())
    {
        outfile__array_neurongroup_H1.write(reinterpret_cast<char*>(_array_neurongroup_H1), 8000*sizeof(_array_neurongroup_H1[0]));
        outfile__array_neurongroup_H1.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_H1." << endl;
    }
    ofstream outfile__array_neurongroup_H2;
    outfile__array_neurongroup_H2.open(results_dir + "_array_neurongroup_H2_2070924351", ios::binary | ios::out);
    if(outfile__array_neurongroup_H2.is_open())
    {
        outfile__array_neurongroup_H2.write(reinterpret_cast<char*>(_array_neurongroup_H2), 8000*sizeof(_array_neurongroup_H2[0]));
        outfile__array_neurongroup_H2.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_H2." << endl;
    }
    ofstream outfile__array_neurongroup_i;
    outfile__array_neurongroup_i.open(results_dir + "_array_neurongroup_i_2649026944", ios::binary | ios::out);
    if(outfile__array_neurongroup_i.is_open())
    {
        outfile__array_neurongroup_i.write(reinterpret_cast<char*>(_array_neurongroup_i), 8000*sizeof(_array_neurongroup_i[0]));
        outfile__array_neurongroup_i.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_i." << endl;
    }
    ofstream outfile__array_neurongroup_inhf;
    outfile__array_neurongroup_inhf.open(results_dir + "_array_neurongroup_inhf_4278777722", ios::binary | ios::out);
    if(outfile__array_neurongroup_inhf.is_open())
    {
        outfile__array_neurongroup_inhf.write(reinterpret_cast<char*>(_array_neurongroup_inhf), 8000*sizeof(_array_neurongroup_inhf[0]));
        outfile__array_neurongroup_inhf.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_inhf." << endl;
    }
    ofstream outfile__array_neurongroup_lastspike;
    outfile__array_neurongroup_lastspike.open(results_dir + "_array_neurongroup_lastspike_1647074423", ios::binary | ios::out);
    if(outfile__array_neurongroup_lastspike.is_open())
    {
        outfile__array_neurongroup_lastspike.write(reinterpret_cast<char*>(_array_neurongroup_lastspike), 8000*sizeof(_array_neurongroup_lastspike[0]));
        outfile__array_neurongroup_lastspike.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_lastspike." << endl;
    }
    ofstream outfile__array_neurongroup_max_theta;
    outfile__array_neurongroup_max_theta.open(results_dir + "_array_neurongroup_max_theta_4271602104", ios::binary | ios::out);
    if(outfile__array_neurongroup_max_theta.is_open())
    {
        outfile__array_neurongroup_max_theta.write(reinterpret_cast<char*>(_array_neurongroup_max_theta), 8000*sizeof(_array_neurongroup_max_theta[0]));
        outfile__array_neurongroup_max_theta.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_max_theta." << endl;
    }
    ofstream outfile__array_neurongroup_network_rate;
    outfile__array_neurongroup_network_rate.open(results_dir + "_array_neurongroup_network_rate_1512603307", ios::binary | ios::out);
    if(outfile__array_neurongroup_network_rate.is_open())
    {
        outfile__array_neurongroup_network_rate.write(reinterpret_cast<char*>(_array_neurongroup_network_rate), 8000*sizeof(_array_neurongroup_network_rate[0]));
        outfile__array_neurongroup_network_rate.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_network_rate." << endl;
    }
    ofstream outfile__array_neurongroup_neuron_target_rate;
    outfile__array_neurongroup_neuron_target_rate.open(results_dir + "_array_neurongroup_neuron_target_rate_4264460859", ios::binary | ios::out);
    if(outfile__array_neurongroup_neuron_target_rate.is_open())
    {
        outfile__array_neurongroup_neuron_target_rate.write(reinterpret_cast<char*>(_array_neurongroup_neuron_target_rate), 8000*sizeof(_array_neurongroup_neuron_target_rate[0]));
        outfile__array_neurongroup_neuron_target_rate.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_neuron_target_rate." << endl;
    }
    ofstream outfile__array_neurongroup_not_refractory;
    outfile__array_neurongroup_not_refractory.open(results_dir + "_array_neurongroup_not_refractory_1422681464", ios::binary | ios::out);
    if(outfile__array_neurongroup_not_refractory.is_open())
    {
        outfile__array_neurongroup_not_refractory.write(reinterpret_cast<char*>(_array_neurongroup_not_refractory), 8000*sizeof(_array_neurongroup_not_refractory[0]));
        outfile__array_neurongroup_not_refractory.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_not_refractory." << endl;
    }
    ofstream outfile__array_neurongroup_q;
    outfile__array_neurongroup_q.open(results_dir + "_array_neurongroup_q_2391304662", ios::binary | ios::out);
    if(outfile__array_neurongroup_q.is_open())
    {
        outfile__array_neurongroup_q.write(reinterpret_cast<char*>(_array_neurongroup_q), 8000*sizeof(_array_neurongroup_q[0]));
        outfile__array_neurongroup_q.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_q." << endl;
    }
    ofstream outfile__array_neurongroup_rech;
    outfile__array_neurongroup_rech.open(results_dir + "_array_neurongroup_rech_1879653577", ios::binary | ios::out);
    if(outfile__array_neurongroup_rech.is_open())
    {
        outfile__array_neurongroup_rech.write(reinterpret_cast<char*>(_array_neurongroup_rech), 8000*sizeof(_array_neurongroup_rech[0]));
        outfile__array_neurongroup_rech.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_rech." << endl;
    }
    ofstream outfile__array_neurongroup_v;
    outfile__array_neurongroup_v.open(results_dir + "_array_neurongroup_v_283966581", ios::binary | ios::out);
    if(outfile__array_neurongroup_v.is_open())
    {
        outfile__array_neurongroup_v.write(reinterpret_cast<char*>(_array_neurongroup_v), 8000*sizeof(_array_neurongroup_v[0]));
        outfile__array_neurongroup_v.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_v." << endl;
    }
    ofstream outfile__array_neurongroup_x;
    outfile__array_neurongroup_x.open(results_dir + "_array_neurongroup_x_4149530994", ios::binary | ios::out);
    if(outfile__array_neurongroup_x.is_open())
    {
        outfile__array_neurongroup_x.write(reinterpret_cast<char*>(_array_neurongroup_x), 8000*sizeof(_array_neurongroup_x[0]));
        outfile__array_neurongroup_x.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_x." << endl;
    }
    ofstream outfile__array_neurongroup_y;
    outfile__array_neurongroup_y.open(results_dir + "_array_neurongroup_y_2152980964", ios::binary | ios::out);
    if(outfile__array_neurongroup_y.is_open())
    {
        outfile__array_neurongroup_y.write(reinterpret_cast<char*>(_array_neurongroup_y), 8000*sizeof(_array_neurongroup_y[0]));
        outfile__array_neurongroup_y.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_y." << endl;
    }
    ofstream outfile__array_neurongroup_z;
    outfile__array_neurongroup_z.open(results_dir + "_array_neurongroup_z_425373790", ios::binary | ios::out);
    if(outfile__array_neurongroup_z.is_open())
    {
        outfile__array_neurongroup_z.write(reinterpret_cast<char*>(_array_neurongroup_z), 8000*sizeof(_array_neurongroup_z[0]));
        outfile__array_neurongroup_z.close();
    } else
    {
        std::cout << "Error writing output file for _array_neurongroup_z." << endl;
    }
    ofstream outfile__array_spikemonitor_1__source_idx;
    outfile__array_spikemonitor_1__source_idx.open(results_dir + "_array_spikemonitor_1__source_idx_3609292218", ios::binary | ios::out);
    if(outfile__array_spikemonitor_1__source_idx.is_open())
    {
        outfile__array_spikemonitor_1__source_idx.write(reinterpret_cast<char*>(_array_spikemonitor_1__source_idx), 2000*sizeof(_array_spikemonitor_1__source_idx[0]));
        outfile__array_spikemonitor_1__source_idx.close();
    } else
    {
        std::cout << "Error writing output file for _array_spikemonitor_1__source_idx." << endl;
    }
    ofstream outfile__array_spikemonitor_1_count;
    outfile__array_spikemonitor_1_count.open(results_dir + "_array_spikemonitor_1_count_3862916462", ios::binary | ios::out);
    if(outfile__array_spikemonitor_1_count.is_open())
    {
        outfile__array_spikemonitor_1_count.write(reinterpret_cast<char*>(_array_spikemonitor_1_count), 2000*sizeof(_array_spikemonitor_1_count[0]));
        outfile__array_spikemonitor_1_count.close();
    } else
    {
        std::cout << "Error writing output file for _array_spikemonitor_1_count." << endl;
    }
    ofstream outfile__array_spikemonitor_1_N;
    outfile__array_spikemonitor_1_N.open(results_dir + "_array_spikemonitor_1_N_2390248205", ios::binary | ios::out);
    if(outfile__array_spikemonitor_1_N.is_open())
    {
        outfile__array_spikemonitor_1_N.write(reinterpret_cast<char*>(_array_spikemonitor_1_N), 1*sizeof(_array_spikemonitor_1_N[0]));
        outfile__array_spikemonitor_1_N.close();
    } else
    {
        std::cout << "Error writing output file for _array_spikemonitor_1_N." << endl;
    }
    ofstream outfile__array_spikemonitor__source_idx;
    outfile__array_spikemonitor__source_idx.open(results_dir + "_array_spikemonitor__source_idx_1477951789", ios::binary | ios::out);
    if(outfile__array_spikemonitor__source_idx.is_open())
    {
        outfile__array_spikemonitor__source_idx.write(reinterpret_cast<char*>(_array_spikemonitor__source_idx), 8000*sizeof(_array_spikemonitor__source_idx[0]));
        outfile__array_spikemonitor__source_idx.close();
    } else
    {
        std::cout << "Error writing output file for _array_spikemonitor__source_idx." << endl;
    }
    ofstream outfile__array_spikemonitor_count;
    outfile__array_spikemonitor_count.open(results_dir + "_array_spikemonitor_count_598337445", ios::binary | ios::out);
    if(outfile__array_spikemonitor_count.is_open())
    {
        outfile__array_spikemonitor_count.write(reinterpret_cast<char*>(_array_spikemonitor_count), 8000*sizeof(_array_spikemonitor_count[0]));
        outfile__array_spikemonitor_count.close();
    } else
    {
        std::cout << "Error writing output file for _array_spikemonitor_count." << endl;
    }
    ofstream outfile__array_spikemonitor_N;
    outfile__array_spikemonitor_N.open(results_dir + "_array_spikemonitor_N_225734567", ios::binary | ios::out);
    if(outfile__array_spikemonitor_N.is_open())
    {
        outfile__array_spikemonitor_N.write(reinterpret_cast<char*>(_array_spikemonitor_N), 1*sizeof(_array_spikemonitor_N[0]));
        outfile__array_spikemonitor_N.close();
    } else
    {
        std::cout << "Error writing output file for _array_spikemonitor_N." << endl;
    }
    ofstream outfile__array_synapses_1_N;
    outfile__array_synapses_1_N.open(results_dir + "_array_synapses_1_N_1771729519", ios::binary | ios::out);
    if(outfile__array_synapses_1_N.is_open())
    {
        outfile__array_synapses_1_N.write(reinterpret_cast<char*>(_array_synapses_1_N), 1*sizeof(_array_synapses_1_N[0]));
        outfile__array_synapses_1_N.close();
    } else
    {
        std::cout << "Error writing output file for _array_synapses_1_N." << endl;
    }
    ofstream outfile__array_synapses_2_N;
    outfile__array_synapses_2_N.open(results_dir + "_array_synapses_2_N_1809632310", ios::binary | ios::out);
    if(outfile__array_synapses_2_N.is_open())
    {
        outfile__array_synapses_2_N.write(reinterpret_cast<char*>(_array_synapses_2_N), 1*sizeof(_array_synapses_2_N[0]));
        outfile__array_synapses_2_N.close();
    } else
    {
        std::cout << "Error writing output file for _array_synapses_2_N." << endl;
    }
    ofstream outfile__array_synapses_2_sources;
    outfile__array_synapses_2_sources.open(results_dir + "_array_synapses_2_sources_1006753409", ios::binary | ios::out);
    if(outfile__array_synapses_2_sources.is_open())
    {
        outfile__array_synapses_2_sources.write(reinterpret_cast<char*>(_array_synapses_2_sources), 3200000*sizeof(_array_synapses_2_sources[0]));
        outfile__array_synapses_2_sources.close();
    } else
    {
        std::cout << "Error writing output file for _array_synapses_2_sources." << endl;
    }
    ofstream outfile__array_synapses_2_targets;
    outfile__array_synapses_2_targets.open(results_dir + "_array_synapses_2_targets_1092595040", ios::binary | ios::out);
    if(outfile__array_synapses_2_targets.is_open())
    {
        outfile__array_synapses_2_targets.write(reinterpret_cast<char*>(_array_synapses_2_targets), 3200000*sizeof(_array_synapses_2_targets[0]));
        outfile__array_synapses_2_targets.close();
    } else
    {
        std::cout << "Error writing output file for _array_synapses_2_targets." << endl;
    }
    ofstream outfile__array_synapses_3_N;
    outfile__array_synapses_3_N.open(results_dir + "_array_synapses_3_N_1780393473", ios::binary | ios::out);
    if(outfile__array_synapses_3_N.is_open())
    {
        outfile__array_synapses_3_N.write(reinterpret_cast<char*>(_array_synapses_3_N), 1*sizeof(_array_synapses_3_N[0]));
        outfile__array_synapses_3_N.close();
    } else
    {
        std::cout << "Error writing output file for _array_synapses_3_N." << endl;
    }
    ofstream outfile__array_synapses_3_sources;
    outfile__array_synapses_3_sources.open(results_dir + "_array_synapses_3_sources_729465538", ios::binary | ios::out);
    if(outfile__array_synapses_3_sources.is_open())
    {
        outfile__array_synapses_3_sources.write(reinterpret_cast<char*>(_array_synapses_3_sources), 1599223*sizeof(_array_synapses_3_sources[0]));
        outfile__array_synapses_3_sources.close();
    } else
    {
        std::cout << "Error writing output file for _array_synapses_3_sources." << endl;
    }
    ofstream outfile__array_synapses_3_targets;
    outfile__array_synapses_3_targets.open(results_dir + "_array_synapses_3_targets_1449441571", ios::binary | ios::out);
    if(outfile__array_synapses_3_targets.is_open())
    {
        outfile__array_synapses_3_targets.write(reinterpret_cast<char*>(_array_synapses_3_targets), 1599223*sizeof(_array_synapses_3_targets[0]));
        outfile__array_synapses_3_targets.close();
    } else
    {
        std::cout << "Error writing output file for _array_synapses_3_targets." << endl;
    }
    ofstream outfile__array_synapses_4_N;
    outfile__array_synapses_4_N.open(results_dir + "_array_synapses_4_N_1867624580", ios::binary | ios::out);
    if(outfile__array_synapses_4_N.is_open())
    {
        outfile__array_synapses_4_N.write(reinterpret_cast<char*>(_array_synapses_4_N), 1*sizeof(_array_synapses_4_N[0]));
        outfile__array_synapses_4_N.close();
    } else
    {
        std::cout << "Error writing output file for _array_synapses_4_N." << endl;
    }
    ofstream outfile__array_synapses_4_sources;
    outfile__array_synapses_4_sources.open(results_dir + "_array_synapses_4_sources_1327214347", ios::binary | ios::out);
    if(outfile__array_synapses_4_sources.is_open())
    {
        outfile__array_synapses_4_sources.write(reinterpret_cast<char*>(_array_synapses_4_sources), 1598726*sizeof(_array_synapses_4_sources[0]));
        outfile__array_synapses_4_sources.close();
    } else
    {
        std::cout << "Error writing output file for _array_synapses_4_sources." << endl;
    }
    ofstream outfile__array_synapses_4_targets;
    outfile__array_synapses_4_targets.open(results_dir + "_array_synapses_4_targets_839242986", ios::binary | ios::out);
    if(outfile__array_synapses_4_targets.is_open())
    {
        outfile__array_synapses_4_targets.write(reinterpret_cast<char*>(_array_synapses_4_targets), 1598726*sizeof(_array_synapses_4_targets[0]));
        outfile__array_synapses_4_targets.close();
    } else
    {
        std::cout << "Error writing output file for _array_synapses_4_targets." << endl;
    }
    ofstream outfile__array_synapses_5_N;
    outfile__array_synapses_5_N.open(results_dir + "_array_synapses_5_N_1855183539", ios::binary | ios::out);
    if(outfile__array_synapses_5_N.is_open())
    {
        outfile__array_synapses_5_N.write(reinterpret_cast<char*>(_array_synapses_5_N), 1*sizeof(_array_synapses_5_N[0]));
        outfile__array_synapses_5_N.close();
    } else
    {
        std::cout << "Error writing output file for _array_synapses_5_N." << endl;
    }
    ofstream outfile__array_synapses_5_sources;
    outfile__array_synapses_5_sources.open(results_dir + "_array_synapses_5_sources_1482734408", ios::binary | ios::out);
    if(outfile__array_synapses_5_sources.is_open())
    {
        outfile__array_synapses_5_sources.write(reinterpret_cast<char*>(_array_synapses_5_sources), 399812*sizeof(_array_synapses_5_sources[0]));
        outfile__array_synapses_5_sources.close();
    } else
    {
        std::cout << "Error writing output file for _array_synapses_5_sources." << endl;
    }
    ofstream outfile__array_synapses_5_targets;
    outfile__array_synapses_5_targets.open(results_dir + "_array_synapses_5_targets_629063849", ios::binary | ios::out);
    if(outfile__array_synapses_5_targets.is_open())
    {
        outfile__array_synapses_5_targets.write(reinterpret_cast<char*>(_array_synapses_5_targets), 399812*sizeof(_array_synapses_5_targets[0]));
        outfile__array_synapses_5_targets.close();
    } else
    {
        std::cout << "Error writing output file for _array_synapses_5_targets." << endl;
    }
    ofstream outfile__array_synapses_N;
    outfile__array_synapses_N.open(results_dir + "_array_synapses_N_483293785", ios::binary | ios::out);
    if(outfile__array_synapses_N.is_open())
    {
        outfile__array_synapses_N.write(reinterpret_cast<char*>(_array_synapses_N), 1*sizeof(_array_synapses_N[0]));
        outfile__array_synapses_N.close();
    } else
    {
        std::cout << "Error writing output file for _array_synapses_N." << endl;
    }

    ofstream outfile__dynamic_array_spikemonitor_1_i;
    outfile__dynamic_array_spikemonitor_1_i.open(results_dir + "_dynamic_array_spikemonitor_1_i_2680224553", ios::binary | ios::out);
    if(outfile__dynamic_array_spikemonitor_1_i.is_open())
    {
        if (! _dynamic_array_spikemonitor_1_i.empty() )
        {
            outfile__dynamic_array_spikemonitor_1_i.write(reinterpret_cast<char*>(&_dynamic_array_spikemonitor_1_i[0]), _dynamic_array_spikemonitor_1_i.size()*sizeof(_dynamic_array_spikemonitor_1_i[0]));
            outfile__dynamic_array_spikemonitor_1_i.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_spikemonitor_1_i." << endl;
    }
    ofstream outfile__dynamic_array_spikemonitor_1_t;
    outfile__dynamic_array_spikemonitor_1_t.open(results_dir + "_dynamic_array_spikemonitor_1_t_4240873456", ios::binary | ios::out);
    if(outfile__dynamic_array_spikemonitor_1_t.is_open())
    {
        if (! _dynamic_array_spikemonitor_1_t.empty() )
        {
            outfile__dynamic_array_spikemonitor_1_t.write(reinterpret_cast<char*>(&_dynamic_array_spikemonitor_1_t[0]), _dynamic_array_spikemonitor_1_t.size()*sizeof(_dynamic_array_spikemonitor_1_t[0]));
            outfile__dynamic_array_spikemonitor_1_t.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_spikemonitor_1_t." << endl;
    }
    ofstream outfile__dynamic_array_spikemonitor_i;
    outfile__dynamic_array_spikemonitor_i.open(results_dir + "_dynamic_array_spikemonitor_i_1976709050", ios::binary | ios::out);
    if(outfile__dynamic_array_spikemonitor_i.is_open())
    {
        if (! _dynamic_array_spikemonitor_i.empty() )
        {
            outfile__dynamic_array_spikemonitor_i.write(reinterpret_cast<char*>(&_dynamic_array_spikemonitor_i[0]), _dynamic_array_spikemonitor_i.size()*sizeof(_dynamic_array_spikemonitor_i[0]));
            outfile__dynamic_array_spikemonitor_i.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_spikemonitor_i." << endl;
    }
    ofstream outfile__dynamic_array_spikemonitor_t;
    outfile__dynamic_array_spikemonitor_t.open(results_dir + "_dynamic_array_spikemonitor_t_383009635", ios::binary | ios::out);
    if(outfile__dynamic_array_spikemonitor_t.is_open())
    {
        if (! _dynamic_array_spikemonitor_t.empty() )
        {
            outfile__dynamic_array_spikemonitor_t.write(reinterpret_cast<char*>(&_dynamic_array_spikemonitor_t[0]), _dynamic_array_spikemonitor_t.size()*sizeof(_dynamic_array_spikemonitor_t[0]));
            outfile__dynamic_array_spikemonitor_t.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_spikemonitor_t." << endl;
    }
    ofstream outfile__dynamic_array_synapses_1__synaptic_post;
    outfile__dynamic_array_synapses_1__synaptic_post.open(results_dir + "_dynamic_array_synapses_1__synaptic_post_1999337987", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_1__synaptic_post.is_open())
    {
        if (! _dynamic_array_synapses_1__synaptic_post.empty() )
        {
            outfile__dynamic_array_synapses_1__synaptic_post.write(reinterpret_cast<char*>(&_dynamic_array_synapses_1__synaptic_post[0]), _dynamic_array_synapses_1__synaptic_post.size()*sizeof(_dynamic_array_synapses_1__synaptic_post[0]));
            outfile__dynamic_array_synapses_1__synaptic_post.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_1__synaptic_post." << endl;
    }
    ofstream outfile__dynamic_array_synapses_1__synaptic_pre;
    outfile__dynamic_array_synapses_1__synaptic_pre.open(results_dir + "_dynamic_array_synapses_1__synaptic_pre_681065502", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_1__synaptic_pre.is_open())
    {
        if (! _dynamic_array_synapses_1__synaptic_pre.empty() )
        {
            outfile__dynamic_array_synapses_1__synaptic_pre.write(reinterpret_cast<char*>(&_dynamic_array_synapses_1__synaptic_pre[0]), _dynamic_array_synapses_1__synaptic_pre.size()*sizeof(_dynamic_array_synapses_1__synaptic_pre[0]));
            outfile__dynamic_array_synapses_1__synaptic_pre.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_1__synaptic_pre." << endl;
    }
    ofstream outfile__dynamic_array_synapses_1_N_incoming;
    outfile__dynamic_array_synapses_1_N_incoming.open(results_dir + "_dynamic_array_synapses_1_N_incoming_3469555706", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_1_N_incoming.is_open())
    {
        if (! _dynamic_array_synapses_1_N_incoming.empty() )
        {
            outfile__dynamic_array_synapses_1_N_incoming.write(reinterpret_cast<char*>(&_dynamic_array_synapses_1_N_incoming[0]), _dynamic_array_synapses_1_N_incoming.size()*sizeof(_dynamic_array_synapses_1_N_incoming[0]));
            outfile__dynamic_array_synapses_1_N_incoming.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_1_N_incoming." << endl;
    }
    ofstream outfile__dynamic_array_synapses_1_N_outgoing;
    outfile__dynamic_array_synapses_1_N_outgoing.open(results_dir + "_dynamic_array_synapses_1_N_outgoing_3922806560", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_1_N_outgoing.is_open())
    {
        if (! _dynamic_array_synapses_1_N_outgoing.empty() )
        {
            outfile__dynamic_array_synapses_1_N_outgoing.write(reinterpret_cast<char*>(&_dynamic_array_synapses_1_N_outgoing[0]), _dynamic_array_synapses_1_N_outgoing.size()*sizeof(_dynamic_array_synapses_1_N_outgoing[0]));
            outfile__dynamic_array_synapses_1_N_outgoing.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_1_N_outgoing." << endl;
    }
    ofstream outfile__dynamic_array_synapses_2__synaptic_post;
    outfile__dynamic_array_synapses_2__synaptic_post.open(results_dir + "_dynamic_array_synapses_2__synaptic_post_1591987953", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_2__synaptic_post.is_open())
    {
        if (! _dynamic_array_synapses_2__synaptic_post.empty() )
        {
            outfile__dynamic_array_synapses_2__synaptic_post.write(reinterpret_cast<char*>(&_dynamic_array_synapses_2__synaptic_post[0]), _dynamic_array_synapses_2__synaptic_post.size()*sizeof(_dynamic_array_synapses_2__synaptic_post[0]));
            outfile__dynamic_array_synapses_2__synaptic_post.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_2__synaptic_post." << endl;
    }
    ofstream outfile__dynamic_array_synapses_2__synaptic_pre;
    outfile__dynamic_array_synapses_2__synaptic_pre.open(results_dir + "_dynamic_array_synapses_2__synaptic_pre_971331175", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_2__synaptic_pre.is_open())
    {
        if (! _dynamic_array_synapses_2__synaptic_pre.empty() )
        {
            outfile__dynamic_array_synapses_2__synaptic_pre.write(reinterpret_cast<char*>(&_dynamic_array_synapses_2__synaptic_pre[0]), _dynamic_array_synapses_2__synaptic_pre.size()*sizeof(_dynamic_array_synapses_2__synaptic_pre[0]));
            outfile__dynamic_array_synapses_2__synaptic_pre.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_2__synaptic_pre." << endl;
    }
    ofstream outfile__dynamic_array_synapses_2_delay;
    outfile__dynamic_array_synapses_2_delay.open(results_dir + "_dynamic_array_synapses_2_delay_3163926887", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_2_delay.is_open())
    {
        if (! _dynamic_array_synapses_2_delay.empty() )
        {
            outfile__dynamic_array_synapses_2_delay.write(reinterpret_cast<char*>(&_dynamic_array_synapses_2_delay[0]), _dynamic_array_synapses_2_delay.size()*sizeof(_dynamic_array_synapses_2_delay[0]));
            outfile__dynamic_array_synapses_2_delay.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_2_delay." << endl;
    }
    ofstream outfile__dynamic_array_synapses_2_N_incoming;
    outfile__dynamic_array_synapses_2_N_incoming.open(results_dir + "_dynamic_array_synapses_2_N_incoming_3109283082", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_2_N_incoming.is_open())
    {
        if (! _dynamic_array_synapses_2_N_incoming.empty() )
        {
            outfile__dynamic_array_synapses_2_N_incoming.write(reinterpret_cast<char*>(&_dynamic_array_synapses_2_N_incoming[0]), _dynamic_array_synapses_2_N_incoming.size()*sizeof(_dynamic_array_synapses_2_N_incoming[0]));
            outfile__dynamic_array_synapses_2_N_incoming.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_2_N_incoming." << endl;
    }
    ofstream outfile__dynamic_array_synapses_2_N_outgoing;
    outfile__dynamic_array_synapses_2_N_outgoing.open(results_dir + "_dynamic_array_synapses_2_N_outgoing_2656015824", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_2_N_outgoing.is_open())
    {
        if (! _dynamic_array_synapses_2_N_outgoing.empty() )
        {
            outfile__dynamic_array_synapses_2_N_outgoing.write(reinterpret_cast<char*>(&_dynamic_array_synapses_2_N_outgoing[0]), _dynamic_array_synapses_2_N_outgoing.size()*sizeof(_dynamic_array_synapses_2_N_outgoing[0]));
            outfile__dynamic_array_synapses_2_N_outgoing.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_2_N_outgoing." << endl;
    }
    ofstream outfile__dynamic_array_synapses_2_w;
    outfile__dynamic_array_synapses_2_w.open(results_dir + "_dynamic_array_synapses_2_w_1828017567", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_2_w.is_open())
    {
        if (! _dynamic_array_synapses_2_w.empty() )
        {
            outfile__dynamic_array_synapses_2_w.write(reinterpret_cast<char*>(&_dynamic_array_synapses_2_w[0]), _dynamic_array_synapses_2_w.size()*sizeof(_dynamic_array_synapses_2_w[0]));
            outfile__dynamic_array_synapses_2_w.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_2_w." << endl;
    }
    ofstream outfile__dynamic_array_synapses_2_x_std;
    outfile__dynamic_array_synapses_2_x_std.open(results_dir + "_dynamic_array_synapses_2_x_std_3319446945", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_2_x_std.is_open())
    {
        if (! _dynamic_array_synapses_2_x_std.empty() )
        {
            outfile__dynamic_array_synapses_2_x_std.write(reinterpret_cast<char*>(&_dynamic_array_synapses_2_x_std[0]), _dynamic_array_synapses_2_x_std.size()*sizeof(_dynamic_array_synapses_2_x_std[0]));
            outfile__dynamic_array_synapses_2_x_std.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_2_x_std." << endl;
    }
    ofstream outfile__dynamic_array_synapses_3__synaptic_post;
    outfile__dynamic_array_synapses_3__synaptic_post.open(results_dir + "_dynamic_array_synapses_3__synaptic_post_4035665760", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_3__synaptic_post.is_open())
    {
        if (! _dynamic_array_synapses_3__synaptic_post.empty() )
        {
            outfile__dynamic_array_synapses_3__synaptic_post.write(reinterpret_cast<char*>(&_dynamic_array_synapses_3__synaptic_post[0]), _dynamic_array_synapses_3__synaptic_post.size()*sizeof(_dynamic_array_synapses_3__synaptic_post[0]));
            outfile__dynamic_array_synapses_3__synaptic_post.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_3__synaptic_post." << endl;
    }
    ofstream outfile__dynamic_array_synapses_3__synaptic_pre;
    outfile__dynamic_array_synapses_3__synaptic_pre.open(results_dir + "_dynamic_array_synapses_3__synaptic_pre_2149485967", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_3__synaptic_pre.is_open())
    {
        if (! _dynamic_array_synapses_3__synaptic_pre.empty() )
        {
            outfile__dynamic_array_synapses_3__synaptic_pre.write(reinterpret_cast<char*>(&_dynamic_array_synapses_3__synaptic_pre[0]), _dynamic_array_synapses_3__synaptic_pre.size()*sizeof(_dynamic_array_synapses_3__synaptic_pre[0]));
            outfile__dynamic_array_synapses_3__synaptic_pre.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_3__synaptic_pre." << endl;
    }
    ofstream outfile__dynamic_array_synapses_3_delay;
    outfile__dynamic_array_synapses_3_delay.open(results_dir + "_dynamic_array_synapses_3_delay_451066579", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_3_delay.is_open())
    {
        if (! _dynamic_array_synapses_3_delay.empty() )
        {
            outfile__dynamic_array_synapses_3_delay.write(reinterpret_cast<char*>(&_dynamic_array_synapses_3_delay[0]), _dynamic_array_synapses_3_delay.size()*sizeof(_dynamic_array_synapses_3_delay[0]));
            outfile__dynamic_array_synapses_3_delay.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_3_delay." << endl;
    }
    ofstream outfile__dynamic_array_synapses_3_N_incoming;
    outfile__dynamic_array_synapses_3_N_incoming.open(results_dir + "_dynamic_array_synapses_3_N_incoming_586590565", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_3_N_incoming.is_open())
    {
        if (! _dynamic_array_synapses_3_N_incoming.empty() )
        {
            outfile__dynamic_array_synapses_3_N_incoming.write(reinterpret_cast<char*>(&_dynamic_array_synapses_3_N_incoming[0]), _dynamic_array_synapses_3_N_incoming.size()*sizeof(_dynamic_array_synapses_3_N_incoming[0]));
            outfile__dynamic_array_synapses_3_N_incoming.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_3_N_incoming." << endl;
    }
    ofstream outfile__dynamic_array_synapses_3_N_outgoing;
    outfile__dynamic_array_synapses_3_N_outgoing.open(results_dir + "_dynamic_array_synapses_3_N_outgoing_99277247", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_3_N_outgoing.is_open())
    {
        if (! _dynamic_array_synapses_3_N_outgoing.empty() )
        {
            outfile__dynamic_array_synapses_3_N_outgoing.write(reinterpret_cast<char*>(&_dynamic_array_synapses_3_N_outgoing[0]), _dynamic_array_synapses_3_N_outgoing.size()*sizeof(_dynamic_array_synapses_3_N_outgoing[0]));
            outfile__dynamic_array_synapses_3_N_outgoing.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_3_N_outgoing." << endl;
    }
    ofstream outfile__dynamic_array_synapses_3_w;
    outfile__dynamic_array_synapses_3_w.open(results_dir + "_dynamic_array_synapses_3_w_1832337320", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_3_w.is_open())
    {
        if (! _dynamic_array_synapses_3_w.empty() )
        {
            outfile__dynamic_array_synapses_3_w.write(reinterpret_cast<char*>(&_dynamic_array_synapses_3_w[0]), _dynamic_array_synapses_3_w.size()*sizeof(_dynamic_array_synapses_3_w[0]));
            outfile__dynamic_array_synapses_3_w.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_3_w." << endl;
    }
    ofstream outfile__dynamic_array_synapses_4__synaptic_post;
    outfile__dynamic_array_synapses_4__synaptic_post.open(results_dir + "_dynamic_array_synapses_4__synaptic_post_225617685", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_4__synaptic_post.is_open())
    {
        if (! _dynamic_array_synapses_4__synaptic_post.empty() )
        {
            outfile__dynamic_array_synapses_4__synaptic_post.write(reinterpret_cast<char*>(&_dynamic_array_synapses_4__synaptic_post[0]), _dynamic_array_synapses_4__synaptic_post.size()*sizeof(_dynamic_array_synapses_4__synaptic_post[0]));
            outfile__dynamic_array_synapses_4__synaptic_post.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_4__synaptic_post." << endl;
    }
    ofstream outfile__dynamic_array_synapses_4__synaptic_pre;
    outfile__dynamic_array_synapses_4__synaptic_pre.open(results_dir + "_dynamic_array_synapses_4__synaptic_pre_455049877", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_4__synaptic_pre.is_open())
    {
        if (! _dynamic_array_synapses_4__synaptic_pre.empty() )
        {
            outfile__dynamic_array_synapses_4__synaptic_pre.write(reinterpret_cast<char*>(&_dynamic_array_synapses_4__synaptic_pre[0]), _dynamic_array_synapses_4__synaptic_pre.size()*sizeof(_dynamic_array_synapses_4__synaptic_pre[0]));
            outfile__dynamic_array_synapses_4__synaptic_pre.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_4__synaptic_pre." << endl;
    }
    ofstream outfile__dynamic_array_synapses_4_alpha;
    outfile__dynamic_array_synapses_4_alpha.open(results_dir + "_dynamic_array_synapses_4_alpha_3175038380", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_4_alpha.is_open())
    {
        if (! _dynamic_array_synapses_4_alpha.empty() )
        {
            outfile__dynamic_array_synapses_4_alpha.write(reinterpret_cast<char*>(&_dynamic_array_synapses_4_alpha[0]), _dynamic_array_synapses_4_alpha.size()*sizeof(_dynamic_array_synapses_4_alpha[0]));
            outfile__dynamic_array_synapses_4_alpha.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_4_alpha." << endl;
    }
    ofstream outfile__dynamic_array_synapses_4_delay;
    outfile__dynamic_array_synapses_4_delay.open(results_dir + "_dynamic_array_synapses_4_delay_3745875037", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_4_delay.is_open())
    {
        if (! _dynamic_array_synapses_4_delay.empty() )
        {
            outfile__dynamic_array_synapses_4_delay.write(reinterpret_cast<char*>(&_dynamic_array_synapses_4_delay[0]), _dynamic_array_synapses_4_delay.size()*sizeof(_dynamic_array_synapses_4_delay[0]));
            outfile__dynamic_array_synapses_4_delay.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_4_delay." << endl;
    }
    ofstream outfile__dynamic_array_synapses_4_delay_1;
    outfile__dynamic_array_synapses_4_delay_1.open(results_dir + "_dynamic_array_synapses_4_delay_1_3370444859", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_4_delay_1.is_open())
    {
        if (! _dynamic_array_synapses_4_delay_1.empty() )
        {
            outfile__dynamic_array_synapses_4_delay_1.write(reinterpret_cast<char*>(&_dynamic_array_synapses_4_delay_1[0]), _dynamic_array_synapses_4_delay_1.size()*sizeof(_dynamic_array_synapses_4_delay_1[0]));
            outfile__dynamic_array_synapses_4_delay_1.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_4_delay_1." << endl;
    }
    ofstream outfile__dynamic_array_synapses_4_N_incoming;
    outfile__dynamic_array_synapses_4_N_incoming.open(results_dir + "_dynamic_array_synapses_4_N_incoming_1450066154", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_4_N_incoming.is_open())
    {
        if (! _dynamic_array_synapses_4_N_incoming.empty() )
        {
            outfile__dynamic_array_synapses_4_N_incoming.write(reinterpret_cast<char*>(&_dynamic_array_synapses_4_N_incoming[0]), _dynamic_array_synapses_4_N_incoming.size()*sizeof(_dynamic_array_synapses_4_N_incoming[0]));
            outfile__dynamic_array_synapses_4_N_incoming.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_4_N_incoming." << endl;
    }
    ofstream outfile__dynamic_array_synapses_4_N_outgoing;
    outfile__dynamic_array_synapses_4_N_outgoing.open(results_dir + "_dynamic_array_synapses_4_N_outgoing_1903308848", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_4_N_outgoing.is_open())
    {
        if (! _dynamic_array_synapses_4_N_outgoing.empty() )
        {
            outfile__dynamic_array_synapses_4_N_outgoing.write(reinterpret_cast<char*>(&_dynamic_array_synapses_4_N_outgoing[0]), _dynamic_array_synapses_4_N_outgoing.size()*sizeof(_dynamic_array_synapses_4_N_outgoing[0]));
            outfile__dynamic_array_synapses_4_N_outgoing.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_4_N_outgoing." << endl;
    }
    ofstream outfile__dynamic_array_synapses_4_w;
    outfile__dynamic_array_synapses_4_w.open(results_dir + "_dynamic_array_synapses_4_w_1752705325", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_4_w.is_open())
    {
        if (! _dynamic_array_synapses_4_w.empty() )
        {
            outfile__dynamic_array_synapses_4_w.write(reinterpret_cast<char*>(&_dynamic_array_synapses_4_w[0]), _dynamic_array_synapses_4_w.size()*sizeof(_dynamic_array_synapses_4_w[0]));
            outfile__dynamic_array_synapses_4_w.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_4_w." << endl;
    }
    ofstream outfile__dynamic_array_synapses_5__synaptic_post;
    outfile__dynamic_array_synapses_5__synaptic_post.open(results_dir + "_dynamic_array_synapses_5__synaptic_post_2736404100", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_5__synaptic_post.is_open())
    {
        if (! _dynamic_array_synapses_5__synaptic_post.empty() )
        {
            outfile__dynamic_array_synapses_5__synaptic_post.write(reinterpret_cast<char*>(&_dynamic_array_synapses_5__synaptic_post[0]), _dynamic_array_synapses_5__synaptic_post.size()*sizeof(_dynamic_array_synapses_5__synaptic_post[0]));
            outfile__dynamic_array_synapses_5__synaptic_post.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_5__synaptic_post." << endl;
    }
    ofstream outfile__dynamic_array_synapses_5__synaptic_pre;
    outfile__dynamic_array_synapses_5__synaptic_pre.open(results_dir + "_dynamic_array_synapses_5__synaptic_pre_2732874109", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_5__synaptic_pre.is_open())
    {
        if (! _dynamic_array_synapses_5__synaptic_pre.empty() )
        {
            outfile__dynamic_array_synapses_5__synaptic_pre.write(reinterpret_cast<char*>(&_dynamic_array_synapses_5__synaptic_pre[0]), _dynamic_array_synapses_5__synaptic_pre.size()*sizeof(_dynamic_array_synapses_5__synaptic_pre[0]));
            outfile__dynamic_array_synapses_5__synaptic_pre.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_5__synaptic_pre." << endl;
    }
    ofstream outfile__dynamic_array_synapses_5_delay;
    outfile__dynamic_array_synapses_5_delay.open(results_dir + "_dynamic_array_synapses_5_delay_2033356777", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_5_delay.is_open())
    {
        if (! _dynamic_array_synapses_5_delay.empty() )
        {
            outfile__dynamic_array_synapses_5_delay.write(reinterpret_cast<char*>(&_dynamic_array_synapses_5_delay[0]), _dynamic_array_synapses_5_delay.size()*sizeof(_dynamic_array_synapses_5_delay[0]));
            outfile__dynamic_array_synapses_5_delay.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_5_delay." << endl;
    }
    ofstream outfile__dynamic_array_synapses_5_N_incoming;
    outfile__dynamic_array_synapses_5_N_incoming.open(results_dir + "_dynamic_array_synapses_5_N_incoming_3452636293", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_5_N_incoming.is_open())
    {
        if (! _dynamic_array_synapses_5_N_incoming.empty() )
        {
            outfile__dynamic_array_synapses_5_N_incoming.write(reinterpret_cast<char*>(&_dynamic_array_synapses_5_N_incoming[0]), _dynamic_array_synapses_5_N_incoming.size()*sizeof(_dynamic_array_synapses_5_N_incoming[0]));
            outfile__dynamic_array_synapses_5_N_incoming.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_5_N_incoming." << endl;
    }
    ofstream outfile__dynamic_array_synapses_5_N_outgoing;
    outfile__dynamic_array_synapses_5_N_outgoing.open(results_dir + "_dynamic_array_synapses_5_N_outgoing_3939990623", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_5_N_outgoing.is_open())
    {
        if (! _dynamic_array_synapses_5_N_outgoing.empty() )
        {
            outfile__dynamic_array_synapses_5_N_outgoing.write(reinterpret_cast<char*>(&_dynamic_array_synapses_5_N_outgoing[0]), _dynamic_array_synapses_5_N_outgoing.size()*sizeof(_dynamic_array_synapses_5_N_outgoing[0]));
            outfile__dynamic_array_synapses_5_N_outgoing.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_5_N_outgoing." << endl;
    }
    ofstream outfile__dynamic_array_synapses_5_w;
    outfile__dynamic_array_synapses_5_w.open(results_dir + "_dynamic_array_synapses_5_w_1773814554", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_5_w.is_open())
    {
        if (! _dynamic_array_synapses_5_w.empty() )
        {
            outfile__dynamic_array_synapses_5_w.write(reinterpret_cast<char*>(&_dynamic_array_synapses_5_w[0]), _dynamic_array_synapses_5_w.size()*sizeof(_dynamic_array_synapses_5_w[0]));
            outfile__dynamic_array_synapses_5_w.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_5_w." << endl;
    }
    ofstream outfile__dynamic_array_synapses__synaptic_post;
    outfile__dynamic_array_synapses__synaptic_post.open(results_dir + "_dynamic_array_synapses__synaptic_post_1801389495", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses__synaptic_post.is_open())
    {
        if (! _dynamic_array_synapses__synaptic_post.empty() )
        {
            outfile__dynamic_array_synapses__synaptic_post.write(reinterpret_cast<char*>(&_dynamic_array_synapses__synaptic_post[0]), _dynamic_array_synapses__synaptic_post.size()*sizeof(_dynamic_array_synapses__synaptic_post[0]));
            outfile__dynamic_array_synapses__synaptic_post.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses__synaptic_post." << endl;
    }
    ofstream outfile__dynamic_array_synapses__synaptic_pre;
    outfile__dynamic_array_synapses__synaptic_pre.open(results_dir + "_dynamic_array_synapses__synaptic_pre_814148175", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses__synaptic_pre.is_open())
    {
        if (! _dynamic_array_synapses__synaptic_pre.empty() )
        {
            outfile__dynamic_array_synapses__synaptic_pre.write(reinterpret_cast<char*>(&_dynamic_array_synapses__synaptic_pre[0]), _dynamic_array_synapses__synaptic_pre.size()*sizeof(_dynamic_array_synapses__synaptic_pre[0]));
            outfile__dynamic_array_synapses__synaptic_pre.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses__synaptic_pre." << endl;
    }
    ofstream outfile__dynamic_array_synapses_delay;
    outfile__dynamic_array_synapses_delay.open(results_dir + "_dynamic_array_synapses_delay_3246960869", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_delay.is_open())
    {
        if (! _dynamic_array_synapses_delay.empty() )
        {
            outfile__dynamic_array_synapses_delay.write(reinterpret_cast<char*>(&_dynamic_array_synapses_delay[0]), _dynamic_array_synapses_delay.size()*sizeof(_dynamic_array_synapses_delay[0]));
            outfile__dynamic_array_synapses_delay.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_delay." << endl;
    }
    ofstream outfile__dynamic_array_synapses_N_incoming;
    outfile__dynamic_array_synapses_N_incoming.open(results_dir + "_dynamic_array_synapses_N_incoming_1151751685", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_N_incoming.is_open())
    {
        if (! _dynamic_array_synapses_N_incoming.empty() )
        {
            outfile__dynamic_array_synapses_N_incoming.write(reinterpret_cast<char*>(&_dynamic_array_synapses_N_incoming[0]), _dynamic_array_synapses_N_incoming.size()*sizeof(_dynamic_array_synapses_N_incoming[0]));
            outfile__dynamic_array_synapses_N_incoming.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_N_incoming." << endl;
    }
    ofstream outfile__dynamic_array_synapses_N_outgoing;
    outfile__dynamic_array_synapses_N_outgoing.open(results_dir + "_dynamic_array_synapses_N_outgoing_1673144031", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_N_outgoing.is_open())
    {
        if (! _dynamic_array_synapses_N_outgoing.empty() )
        {
            outfile__dynamic_array_synapses_N_outgoing.write(reinterpret_cast<char*>(&_dynamic_array_synapses_N_outgoing[0]), _dynamic_array_synapses_N_outgoing.size()*sizeof(_dynamic_array_synapses_N_outgoing[0]));
            outfile__dynamic_array_synapses_N_outgoing.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_N_outgoing." << endl;
    }
    ofstream outfile__dynamic_array_synapses_w;
    outfile__dynamic_array_synapses_w.open(results_dir + "_dynamic_array_synapses_w_441891901", ios::binary | ios::out);
    if(outfile__dynamic_array_synapses_w.is_open())
    {
        if (! _dynamic_array_synapses_w.empty() )
        {
            outfile__dynamic_array_synapses_w.write(reinterpret_cast<char*>(&_dynamic_array_synapses_w[0]), _dynamic_array_synapses_w.size()*sizeof(_dynamic_array_synapses_w[0]));
            outfile__dynamic_array_synapses_w.close();
        }
    } else
    {
        std::cout << "Error writing output file for _dynamic_array_synapses_w." << endl;
    }

    // Write last run info to disk
    ofstream outfile_last_run_info;
    outfile_last_run_info.open(results_dir + "last_run_info.txt", ios::out);
    if(outfile_last_run_info.is_open())
    {
        outfile_last_run_info << (Network::_last_run_time) << " " << (Network::_last_run_completed_fraction) << std::endl;
        outfile_last_run_info.close();
    } else
    {
        std::cout << "Error writing last run info to file." << std::endl;
    }
}

void _dealloc_arrays()
{
    using namespace brian;


    // static arrays
    if(_static_array__array_neurongroup_1_v!=0)
    {
        delete [] _static_array__array_neurongroup_1_v;
        _static_array__array_neurongroup_1_v = 0;
    }
    if(_static_array__array_neurongroup_a1!=0)
    {
        delete [] _static_array__array_neurongroup_a1;
        _static_array__array_neurongroup_a1 = 0;
    }
    if(_static_array__array_neurongroup_v!=0)
    {
        delete [] _static_array__array_neurongroup_v;
        _static_array__array_neurongroup_v = 0;
    }
    if(_static_array__array_synapses_2_sources!=0)
    {
        delete [] _static_array__array_synapses_2_sources;
        _static_array__array_synapses_2_sources = 0;
    }
    if(_static_array__array_synapses_2_targets!=0)
    {
        delete [] _static_array__array_synapses_2_targets;
        _static_array__array_synapses_2_targets = 0;
    }
    if(_static_array__array_synapses_3_sources!=0)
    {
        delete [] _static_array__array_synapses_3_sources;
        _static_array__array_synapses_3_sources = 0;
    }
    if(_static_array__array_synapses_3_targets!=0)
    {
        delete [] _static_array__array_synapses_3_targets;
        _static_array__array_synapses_3_targets = 0;
    }
    if(_static_array__array_synapses_4_sources!=0)
    {
        delete [] _static_array__array_synapses_4_sources;
        _static_array__array_synapses_4_sources = 0;
    }
    if(_static_array__array_synapses_4_targets!=0)
    {
        delete [] _static_array__array_synapses_4_targets;
        _static_array__array_synapses_4_targets = 0;
    }
    if(_static_array__array_synapses_5_sources!=0)
    {
        delete [] _static_array__array_synapses_5_sources;
        _static_array__array_synapses_5_sources = 0;
    }
    if(_static_array__array_synapses_5_targets!=0)
    {
        delete [] _static_array__array_synapses_5_targets;
        _static_array__array_synapses_5_targets = 0;
    }
    if(_static_array__dynamic_array_synapses_2_delay!=0)
    {
        delete [] _static_array__dynamic_array_synapses_2_delay;
        _static_array__dynamic_array_synapses_2_delay = 0;
    }
    if(_static_array__dynamic_array_synapses_2_w!=0)
    {
        delete [] _static_array__dynamic_array_synapses_2_w;
        _static_array__dynamic_array_synapses_2_w = 0;
    }
    if(_static_array__dynamic_array_synapses_3_delay!=0)
    {
        delete [] _static_array__dynamic_array_synapses_3_delay;
        _static_array__dynamic_array_synapses_3_delay = 0;
    }
    if(_static_array__dynamic_array_synapses_3_w!=0)
    {
        delete [] _static_array__dynamic_array_synapses_3_w;
        _static_array__dynamic_array_synapses_3_w = 0;
    }
    if(_static_array__dynamic_array_synapses_4_delay!=0)
    {
        delete [] _static_array__dynamic_array_synapses_4_delay;
        _static_array__dynamic_array_synapses_4_delay = 0;
    }
    if(_static_array__dynamic_array_synapses_4_w!=0)
    {
        delete [] _static_array__dynamic_array_synapses_4_w;
        _static_array__dynamic_array_synapses_4_w = 0;
    }
    if(_static_array__dynamic_array_synapses_5_delay!=0)
    {
        delete [] _static_array__dynamic_array_synapses_5_delay;
        _static_array__dynamic_array_synapses_5_delay = 0;
    }
    if(_static_array__dynamic_array_synapses_5_w!=0)
    {
        delete [] _static_array__dynamic_array_synapses_5_w;
        _static_array__dynamic_array_synapses_5_w = 0;
    }
}

