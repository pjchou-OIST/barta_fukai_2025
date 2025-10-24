import argparse
import numpy as np
import time
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

import logging
logger = logging.getLogger(__name__)

if __name__ == '__main__':
    start_time = time.time()
    
    logging.basicConfig(level=logging.INFO)

    parser = argparse.ArgumentParser()
    parser.add_argument('--system', type=str, default='hebb')
    parser.add_argument('--namespace', type=str, default='lognormal')
    parser.add_argument('--npat', type=int, default=1000)
    args = parser.parse_args()


    logger.info(f"Arguments received: system={args.system}, npat={args.npat}")

    W = get_W(system=args.system, npat=args.npat, exc_vals=False, namespace=args.namespace)

    Wei = W[:8000,8000:]
    wei = Wei[Wei != 0]
    totinhib = Wei.sum(axis=1)

    Wee = W[:8000,:8000]
    totexcit = Wee.sum(axis=1)

    os.makedirs('add-STD/plotting/data/ei_weights', exist_ok=True)
    os.makedirs('add-STD/plotting/data/tot_inhib', exist_ok=True)
    os.makedirs('add-STD/plotting/data/tot_excit', exist_ok=True)
    np.savetxt(f'add-STD/plotting/data/ei_weights/{args.system}{args.npat}.csv', wei)
    np.savetxt(f'add-STD/plotting/data/tot_inhib/{args.system}{args.npat}.csv', totinhib)
    np.savetxt(f'add-STD/plotting/data/tot_excit/{args.system}{args.npat}.csv', totexcit)
    logger.info("Weights saved.")