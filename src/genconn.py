"""Generate and export initial E/I connectivity matrices with embedded patterns.

This script builds a dense weight matrix `Z` for an E/I network and a set of
binary **patterns** to embed via Hebbian co‑activation. It optionally adds
circular (sequence) couplings between neighboring patterns, imposes blockwise
sparsity, overlays a chosen weight **distribution** (lognormal by default) on
E→E edges, rescales by pattern participation, and then writes the connectivity
(in COO form) and pattern metadata to an HDF5 file.
"""

import numpy as np
import os
from tqdm import tqdm
from itertools import combinations, product
import argparse
import yaml
import h5py
from scipy.sparse import csr_array

from utils import *


def genconn(N, N_exc, P, f, sparsity, weights, circular=False, chain=False, segmented_chain=False, segmented_circular=False, chain_length=10, rescale=True, fix_size=False, spread=0, seed=42, distribution='lognormal'):
    """Construct a dense weight matrix and embedded patterns.

    Generate connectivity by computing Hebbian co‑activations within E→E, then
    **keeping only the strongest** E→E entries and **overlaying** a weight
    distribution (lognormal/normal). Non‑E→E blocks (I→E, I→I, E→I) are sampled
    with independent Bernoulli masks at requested sparsities.

    Parameters
    ----------
    N : int
        Total neurons (E+I).
    N_exc : int
        Number of **excitatory** neurons; inhibitory count is `N - N_exc`.
    P : int
        Number of binary patterns to embed.
    f : float
        Per‑pattern sparsity (probability of an E neuron being active in a pattern).
    sparsity : tuple(float, float, float, float)
        `(sparse_ee, sparse_ie, sparse_ii, sparse_ei)`, i.e., **kept** fractions.
    i_factor : float
        Base weight factor for **non‑E→E** blocks (applied post mask).
    circular : bool, default False
        If True, add couplings between **adjacent** patterns in both directions
        (pattern t ↔ t±1) to encourage sequential recall.
    rescale : bool, default True
        If True, rescale **columns** (presynaptic) by exp of pattern
        participation deviation to compensate for neurons active in many patterns.
    var_factor : float, default 0.5
        Variance multiplier for the E→E weight distribution (relative to mean E=0.25).
    fix_size : bool, default False
        If True, each pattern has **exactly** `round(N_exc*f)` active E neurons.
        If False, active set is Bernoulli(f).
    spread : float, default 0
        Modulates per‑pattern Hebbian strength linearly across patterns;
        `q = linspace(-1,1,P) * spread + 1` multiplies contributions for each pattern.
    seed : int, default 42
        RNG seed for reproducibility.
    distribution : {'lognormal', 'normal'}
        Distribution used to overlay E→E weights after selecting strongest edges.

    Returns
    -------
    Z : (N, N) ndarray
        Dense weight matrix with E and I blocks (no self‑synapses).
    patterns : (P, N_exc) ndarray of {0,1}
        Binary pattern matrix over **E** neurons (I neurons are not patterned here).
    """
    np.random.seed(seed)

    # --- Build binary patterns over E neurons --------------------------------
    if not fix_size:
        patterns = (np.random.rand(P, N_exc) < f).astype(float)
    else:
        patterns = []
        pat_len = int(N_exc * f)
        for ii in range(P):
            pattern = np.random.permutation(np.concatenate([np.ones(pat_len), np.zeros(N_exc-pat_len)]))
            patterns.append(pattern)
        patterns = np.array(patterns)

    # Z0 accumulates raw Hebbian co‑occurrence before pruning/scaling
    Z0 = np.zeros((N,N))

    # Optional monotonic spread across patterns (q multipliers)
    spread_arr = np.linspace(-1, 1, len(patterns)) * spread + 1

    # Hebbian within‑pattern pairs (E→E): add symmetric increments
    for pattern, q in tqdm(zip(patterns, spread_arr), total=len(patterns)):
        pairs = np.array(list(combinations(np.argwhere(pattern).flatten(), 2)))
        x, y = pairs.T
        Z0[x,y] += 1 * q
        Z0[y,x] += 1 * q

    # Optional circular couplings between successive patterns (both directions)
    print('Current setting: circular =', circular, ', chain =', chain)
    inter_pattern_strength = 1.0
    
    if segmented_chain:
        print(f"Segmented chain coupling (chain length: {chain_length}) (Inter-pattern strength: {inter_pattern_strength})")
        if P % chain_length != 0:
            print(f"Warning: Total number of patterns ({P}) is not divisible by chain_length ({chain_length}).")
        
        num_segments = P // chain_length
        
        for seg_idx in tqdm(range(num_segments), total=num_segments, desc="Chain Segments"):
            # Calculate the starting pattern index for this segment
            start_pattern_idx = seg_idx * chain_length
            
            # Inner loop: create (L-1) bidirectional connections within this segment
            # e.g. L=10, i goes from 0 to 8
            for i in range(chain_length - 1):
                # Connect (P[0], P[1]), (P[1], P[2]), ..., (P[8], P[9])
                # Then outer loop +1, connect (P[10], P[11]) ... (P[18], P[19])
                idx1 = start_pattern_idx + i
                idx2 = start_pattern_idx + i + 1
                
                pattern1 = patterns[idx1]
                pattern2 = patterns[idx2]

                neurons1 = np.argwhere(pattern1).flatten()
                neurons2 = np.argwhere(pattern2).flatten()
                pairs = np.array(list(product(neurons1, neurons2)))

                if pairs.size > 0:
                    x, y = pairs.T
                    # Build bidirectional (symmetric) connections
                    Z0[x,y] += inter_pattern_strength 
                    Z0[y,x] += inter_pattern_strength
    
    elif segmented_circular:
        print(f"Encoding symmetric *segmented circular* connections (chain length: {chain_length}) (base strength: {inter_pattern_strength})...")
        if P % chain_length != 0:
            print(f'Warning: Total number of patterns ({P}) is not divisible by chain_length ({chain_length}).')
        
        num_segments = P // chain_length
        
        for seg_idx in tqdm(range(num_segments), total=num_segments, desc="Circular Segments"):
            # Select patterns for this segment
            start_idx = seg_idx * chain_length
            end_idx = (seg_idx + 1) * chain_length
            segment_patterns = patterns[start_idx:end_idx] # this is a slice of P[0]...P[9]

            # Apply 'circular' logic on this slice
            # (P[0],P[1]), (P[1],P[2]), ..., (P[8],P[9]), (P[9],P[0])
            for pattern1, pattern2 in zip(segment_patterns, np.roll(segment_patterns, shift=-1, axis=0)):
                neurons1 = np.argwhere(pattern1).flatten()
                neurons2 = np.argwhere(pattern2).flatten()
                pairs = np.array(list(product(neurons1, neurons2)))

                if pairs.size > 0:
                    x, y = pairs.T
                    Z0[x,y] += inter_pattern_strength 
                    Z0[y,x] += inter_pattern_strength
                    
    elif chain:
        # P[t] <-> P[t+1] for t=0 to P-2
        for i in tqdm(range(P - 1), total=P-1):
            pattern1 = patterns[i]
            pattern2 = patterns[i+1]

            neurons1 = np.argwhere(pattern1).flatten()
            neurons2 = np.argwhere(pattern2).flatten()
            pairs = np.array(list(product(neurons1, neurons2)))

            if pairs.size > 0:
                x, y = pairs.T
                Z0[x,y] += 1.0 
                Z0[y,x] += 1.0
                
    elif circular:
        for pattern1, pattern2 in tqdm(zip(patterns, np.roll(patterns, shift=1, axis=0)), total=len(patterns)):
            pairs = np.array(list(product(np.argwhere(pattern1).flatten(), np.argwhere(pattern2).flatten())))
            x, y = pairs.T
            Z0[x,y] += 0.5
            Z0[y,x] += 0.5

        for pattern1, pattern2 in tqdm(zip(patterns, np.roll(patterns, shift=-1, axis=0)), total=len(patterns)):
            pairs = np.array(list(product(np.argwhere(pattern1).flatten(), np.argwhere(pattern2).flatten())))
            x, y = pairs.T
            Z0[x,y] += 0.5
            Z0[y,x] += 0.5

    # Remove all self‑synapses explicitly
    for i in range(N):
        Z0[i,i] = 0

    # --- Impose sparsity and assign E→E weights by rank ----------------------
    sparse_ee, sparse_ie, sparse_ii, sparse_ei = sparsity

    Z = np.copy(Z0)

    # Prune to strongest E→E entries via percentile thresholding
    Z_slice = Z[:N_exc,:N_exc]
    Z_slice += np.random.rand(N_exc, N_exc)  # random tie‑breaking/noise
    limit = np.percentile(Z_slice, 100-100*sparse_ee)
    Z_slice[Z_slice < limit] = 0

    # Extract surviving E→E raw scores and map to target distribution by rank
    weights_ee = (Z[:N_exc,:N_exc][Z[:N_exc,:N_exc] != 0])

    E = weights['ee_mean']
    Var = weights['ee_var']

    sig = np.sqrt(np.log(Var/(E*E) + 1))
    mu = np.log(E) - sig**2 / 2

    if distribution == 'lognormal':
        rvs_exc = np.sort(np.exp(np.random.randn(len(weights_ee))*sig + mu))
    elif distribution == 'normal':
        rvs_exc = np.sort(np.random.randn(len(weights_ee))*np.sqrt(Var) + E)
    else:
        raise ValueError(f'Distribution "{distribution}" is not supported.')

    # Rank‑preserving overlay: highest Hebbian gets highest sampled weight
    Z[:N_exc,:N_exc][Z[:N_exc,:N_exc] != 0] = rvs_exc[np.argsort(np.argsort(weights_ee))]

    # --- Fill non‑E→E blocks by independent Bernoulli masks ------------------
    slices = [
        (N_exc, N, 0, N_exc),      # I←E (rows I, cols E)
        (N_exc, N, N_exc, N),      # I←I (rows I, cols I)
        (0, N_exc, N_exc, N)       # E←I (rows E, cols I)
    ]


    for slice, sp_val in zip(slices, [sparse_ie, sparse_ii, sparse_ei]):
        Z_slice = Z[slice[0]:slice[1],slice[2]:slice[3]]
        Z[slice[0]:slice[1],slice[2]:slice[3]] = (np.random.rand(*Z_slice.shape) < sp_val).astype(float)

    # Optional presynaptic column rescaling by pattern participation
    if rescale:
        pattern_participation = patterns.sum(axis=0)
        for ii in range(N_exc):
            factor = np.exp((pattern_participation[ii] - P*f) / P*f)
            Z[:,ii] = Z[:,ii] / factor

    # --- Final E/I block scaling --------------------------------------------
    Z[N_exc:,N_exc:] = Z[N_exc:,N_exc:] * weights['ii_hom']
    Z[:N_exc,N_exc:] = Z[:N_exc,N_exc:] * weights['ei_hom']
    Z[N_exc:,:N_exc] = Z[N_exc:,:N_exc] * weights['ie_hom']

    # if rescale:
    #     for i in tqdm(range(N_exc)):
    #         Z[i,:8000] = Z[i,:8000] / Z[i,:8000].sum()

    return Z, patterns


def get_aux_prop(Z):
    """Generate per‑synapse delays and a random `exc_alpha` per excitatory neuron.

    Parameters
    ----------
    Z : (N, N) ndarray
        Dense weight matrix produced by `genconn`.

    Returns
    -------
    delays_tuple : tuple(ndarray, ndarray, ndarray, ndarray)
        Flat arrays of delays for existing synapses in the order `(EE, IE, II, EI)`.
    exc_alpha : (N_exc,) ndarray
        Random normal vector for E neurons (used as an excitability/adaptation param elsewhere).
    """
    exc_alpha = np.random.randn(8000)
    delays = np.random.rand(*Z.shape) * 2
    delays[:,:8000] += 1

    delays_ee = delays[:8000,:8000][Z[:8000,:8000] != 0]
    delays_ie = delays[8000:,:8000][Z[8000:,:8000] != 0]
    delays_ii = delays[8000:,8000:][Z[8000:,8000:] != 0]
    delays_ei = delays[:8000,8000:][Z[:8000,8000:] != 0]

    delays_tuple = (delays_ee, delays_ie, delays_ii, delays_ei)

    return delays_tuple, exc_alpha


if __name__ == '__main__':
    parser = argparse.ArgumentParser()

    parser.add_argument('--config', type=str, default='config/networks/segment.yml')
    parser.add_argument('-p', '--patterns', type=int, default=2000)
    parser.add_argument('--namespace', type=str, default='lognormal')
    parser.add_argument('--seed', type=int, default=42)

    # parser.add_argument('-n', '--neurons', type=int, default=10000)
    # parser.add_argument('--nexc', type=int, default=8000)
    
    # parser.add_argument('--circular', action='store_true')
    # parser.add_argument('--fix_size', action='store_true')
    # parser.add_argument('--ee_sparse', type=float, default=0.05)
    # parser.add_argument('--ii_sparse', type=float, default=0.1)
    # parser.add_argument('--ei_sparse', type=float, default=0.1)
    # parser.add_argument('--ie_sparse', type=float, default=0.1)
    # parser.add_argument('--i_factor', type=float, default=1)
    # parser.add_argument('--var', type=float, default=0.5)
    # parser.add_argument('--spread', type=float, default=0)

    args = parser.parse_args()

    with open(args.config) as f:
        config = yaml.safe_load(f)

    N_exc = config['neurons']['exc']
    N_inh = config['neurons']['inh']
    N = N_inh + N_exc

    ee_sparse = config['sparsity']['ee']
    ei_sparse = config['sparsity']['ei']
    ie_sparse = config['sparsity']['ie']
    ii_sparse = config['sparsity']['ii']

    # Block sparsities: (E←E, I←E, I←I, E←I)
    network_sparsity = (ee_sparse, ie_sparse, ii_sparse, ei_sparse)

    # Check coupling types
    segmented_chain_coupling = config.get('segmented_chain', False)  
    segmented_circular_coupling = config.get('segmented_circular', False)
    chain_length = config.get('chain_length', 10)                     
    chain_coupling = config.get('chain', False)
    circular_coupling = config.get('circular', False)

    if (segmented_chain_coupling + segmented_circular_coupling + chain_coupling + circular_coupling) > 1:
        print("Warning: Multiple coupling types enabled. Priority: segmented_chain > segmented_circular > chain > circular.")
        if segmented_chain_coupling:
            segmented_circular_coupling = False
            chain_coupling = False
            circular_coupling = False
        elif segmented_circular_coupling:
            chain_coupling = False
            circular_coupling = False
        elif chain_coupling:
            circular_coupling = False
            
    # Build connectivity and patterns (dense)
    Z, patterns = genconn(N=N, N_exc=N_exc, P=args.patterns, f=config['assemblies']['pattern_sparsity'],
                sparsity=network_sparsity, circular=circular_coupling, chain=chain_coupling,
                segmented_chain=segmented_chain_coupling, segmented_circular=segmented_circular_coupling, chain_length=chain_length, seed=args.seed,
                weights=config['weights'], fix_size=config['fix_size'], spread=config['assemblies']['spread'],
                distribution=config['distribution'])

    # Auxiliary properties: per‑synapse delays and E‑neuron exc_alpha
    delays_tuple, exc_alpha = get_aux_prop(Z)

    # Store patterns sparsely (CSR components)
    csr_patterns = csr_array(patterns) 

    # Population index limits for slicing blocks
    limits = {
        'E': (0,8000),
        'I': (8000,10000)
    }
    
    # Resolve base data path
    with open('config/server_config.yaml') as f:
        server_config = yaml.safe_load(f)

    folder_path = data_path(args.namespace)

    create_directory(folder_path)

    os.makedirs(folder_path, exist_ok=True)
    output_filename = f'{folder_path}/init{args.patterns}.h5'

    # Write connectivity to HDF5 in COO form per block (EE, IE, II, EI)
    with h5py.File(output_filename, "w") as h5f:  # Open file in append mode
        connectivity_group = h5f.require_group("connectivity")
        weights_group = connectivity_group.require_group("weights")  # Ensure "weights" group exists

        for pre in ['E','I']:
            for post in ['E','I']:
                # Cut block as (rows=post, cols=pre), then transpose to get sources in rows
                cut = Z[limits[post][0]:limits[post][1], limits[pre][0]:limits[pre][1]].T
                sources, targets = np.nonzero(cut)
                weights = cut[cut != 0]

                label = f'{post}{pre}'
                create_weight_dataset(weights_group.require_group(label), "sources", sources, dtype=np.uint16)
                create_weight_dataset(weights_group.require_group(label), "targets", targets, dtype=np.uint16)
                create_weight_dataset(weights_group.require_group(label), "weights", weights)
                

        # Scalar/vector extras
        create_weight_dataset(connectivity_group, 'exc_alpha', exc_alpha)

        # Delays per block
        delays_group = connectivity_group.require_group('delays')
        create_weight_dataset(delays_group, "EE", delays_tuple[0], dtype=np.float32)
        create_weight_dataset(delays_group, "IE", delays_tuple[1], dtype=np.float32)
        create_weight_dataset(delays_group, "II", delays_tuple[2], dtype=np.float32)
        create_weight_dataset(delays_group, "EI", delays_tuple[3], dtype=np.float32)

        # Sparse pattern metadata (CSR)
        patterns_group = connectivity_group.require_group('patterns')
        create_weight_dataset(patterns_group, "indices", csr_patterns.indices)
        create_weight_dataset(patterns_group, "splits", csr_patterns.indptr[1:-1])

        # Attributes (counts)
        connectivity_group.attrs['N_exc'] = N_exc
        connectivity_group.attrs['N_inh'] = N_inh
        connectivity_group.attrs['N_patterns'] = args.patterns