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
    output_dir = 'SFA-STD/plotting/data/firing_rates/'

    fr = get_firing_rates(args.system, args.npat, namespace=args.namespace)
    os.makedirs(f'{output_dir}', exist_ok=True)
    np.savetxt(f'{output_dir}{args.system}{args.npat}.csv', np.concatenate([fr['exc'], fr['inh']]))
    logger.info("Firing rates saved.")

    duration = time.time() - start_time
    logger.info(f"Execution completed in {duration:.2f} seconds.")