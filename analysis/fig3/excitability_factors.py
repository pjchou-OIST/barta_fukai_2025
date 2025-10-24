import numpy as np
import pandas as pd
import h5py
from tqdm import tqdm
import numpy as np
import sys
import os

current_dir = os.path.dirname(os.path.abspath(__file__)) # -> /my_project/src/app
src_dir = os.path.dirname(current_dir)                    # -> /my_project/src
parent_dir = os.path.dirname(src_dir)                   # -> /my_project
utils_path = os.path.join(parent_dir, 'src/')

sys.path.append(utils_path)
from utils import *
from analysis import *
from eigenvalues import get_W


if __name__ == '__main__':
    namespace = 'lognormal'

    for npat in tqdm([1000]):
    # for npat in tqdm([800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 3000]):
        res = {}
        patterns = load_patterns(npat, namespace=namespace).dense()
        norm_patterns = (patterns.T / patterns.sum(axis=1)).T

        for system in ['hebb','hebb_smooth_rate','rate']:
            W = get_W(system=system, npat=npat, exc_vals=False, namespace=namespace)

            Wei = W[:8000,8000:]
            Wee = W[:8000,:8000]
            
            var_stats = pd.read_csv(f'{data_path(namespace)}/var_stats/{system}_conductances{npat}_stats.csv', index_col=[0,1])

            totexcit = var_stats.loc['exc']['mean_e']
            totinhib = var_stats.loc['exc']['mean_i']

            act_counts = get_act_counts(system, npat, namespace=namespace)

            res[(system, npat, 'tot_excit')] = norm_patterns @ totexcit
            res[(system, npat, 'tot_inhib')] = norm_patterns @ totinhib
            res[(system, npat, 'act_counts')] = act_counts
            break
        os.makedirs('new/plotting/data/totweights', exist_ok=True)
        pd.DataFrame(res).to_csv(f'new/plotting/data/totweights/totweights_{npat}.csv')