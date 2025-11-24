import h5py
import numpy as np
import sys
import os

current_dir = os.path.dirname(os.path.abspath(__file__)) # -> /my_project/src/app
src_dir = os.path.dirname(current_dir)                    # -> /my_project/src
parent_dir = os.path.dirname(src_dir)                   # -> /my_project
utils_path = os.path.join(parent_dir, 'src/')

sys.path.append(utils_path)
from utils import data_path, load_patterns
from analysis import get_spike_counts


if __name__ == '__main__':
    namespace = 'lognormal'
    # npat = 1800 # original
    npat = 1000
    system = 'hebb'
    output_dir = 'dist-STD/plotting/data/assembly_traces_hebb/'

    path_to_folder = data_path(namespace)
    filename = f"{path_to_folder}/{system}_spontaneous{npat}.h5"

    rates_exc = []
    rates_inh = []

    with h5py.File(filename, 'r', swmr=True) as h5f:
        N_exc = h5f['connectivity'].attrs['N_exc']
        N_inh = h5f['connectivity'].attrs['N_inh']

        spikes_exc = h5f['spikes_exc'][:].T
        spikes_inh = h5f['spikes_inh'][:].T

    _, sc = get_spike_counts(*spikes_exc, 21, dt=0.01, offset=0)
    sc = sc[:,1500:]

    run = 'spontaneous'

    patterns = load_patterns(npat, namespace=namespace)
    filename = f"{path_to_folder}/{system}_{run}{npat}_activations.h5"

    with h5py.File(filename, "r", swmr=True) as h5f:
        act_times, durations, pattern_ixs = h5f['activations'][:]

    act_times = act_times / 100
    mask = (act_times > 5) & (act_times < 10)

    pattern_ix_list = np.unique(pattern_ixs[mask])
    print(pattern_ix_list)

    pattern_rates = []
    pattern_activations = []

    for i, patix in enumerate(pattern_ix_list):
        print(i, patix)
        pattern = patterns[int(patix)]

        pattern_rates.append((sc[pattern]).mean(axis=0)[:500]*100)

        act_trace = np.zeros(500)

        for i in range(10):
            act_trace[i::10] = (sc[patterns[int(patix)]][:,i:500+i].reshape(len(pattern), 50, 10).sum(axis=2) > 0).mean(axis=0)

        print(act_trace)
        pattern_activations.append(act_trace)

    os.makedirs(output_dir, exist_ok=True)
    np.savetxt(f'{output_dir}rates.csv', pattern_rates)
    np.savetxt(f'{output_dir}activations.csv', pattern_activations)