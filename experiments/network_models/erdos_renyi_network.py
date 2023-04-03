import networkx as nx


def erdos_renyi_network(parameters):
    return nx.erdos_renyi_graph(
        parameters['num_nodes'],
        parameters['mean_degree'] / (parameters['num_nodes']-1),
    )

