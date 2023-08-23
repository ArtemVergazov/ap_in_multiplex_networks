from bisect import bisect_left
import networkx as nx
import numpy as np


# def scale_free_network(const lint N, const double gamma, const double c, const ulint seed)
def scale_free_network(parameters):

    G = nx.Graph()
    G.add_nodes_from(range(parameters['num_nodes']))

    alpha = 1 / (parameters['exponent'] - 1)
    sum_ = 0.
    num_edges = (parameters['mean_degree'] * parameters['num_nodes'] / 2)

    if abs(alpha - .5) <= 1e-6 or alpha < .5:
        i0 = 1.
    else:
        i0 = (10 * 2**.5 * (1 - alpha))**(1/alpha) * parameters['num_nodes']**(1 - .5/alpha)
    # i0 = 1.

    weights = (i0 + np.arange(parameters['num_nodes']))**-alpha
    sum_ = weights.cumsum()
    cdf = sum_ / sum_[-1]

    # Create links
    for _ in range(int(num_edges)):

        condition = True

        while condition:

            condition = False

            source = bisect_left(cdf, np.random.uniform(0, 1))
            target = bisect_left(cdf, np.random.uniform(0, 1))

            if source == target or target in G.neighbors(source):
                condition = True

            # if target in G.neighbors(source):
            #     break
        
        G.add_edge(source, target)

    return G


# def sample_power_law_distribution(parameters):

#     stubs = 0

#     # Create CDF for degree sampling
#     weights = np.arange(parameters['kmin'], parameters['kmax'] + 1.)**-parameters['exponent']
#     cdf = weights.cumsum()
#     cdf /= weights.sum()

#     # Sample degrees
#     degrees = cdf.searchsorted(np.random.uniform(size=parameters['num_nodes'])) + parameters['kmin']
#     stubs = degrees.sum()

#     # Check total number of stubs to be even number
#     if stubs % 2 != 0:
#         degrees[-1] += 1

#     return degrees


# # Permute stubs and match them randomly: configuration model
# def match_stubs(G, stubs):

#     stubs = np.random.permutation(stubs)

#     for i in range(0, len(stubs), 2):

#         source = stubs[i]
#         target = stubs[i + 1]

#         if source == target or target in G.neighbors(source):
#             continue

#         G.add_edge(target, source)


# # Create a configuration model with a given degree distribution
# def configuration_model_network(degrees):

#     G = nx.Graph()
#     G.add_nodes_from(range(len(degrees)))
#     stubs = np.repeat(range(len(degrees)), degrees)
#     match_stubs(G, stubs)

#     return G


# def scale_free_network(parameters):

#     # Sample power law degree
#     degrees = sample_power_law_distribution(parameters)

#     # Create network
#     G = configuration_model_network(degrees)

#     return G
