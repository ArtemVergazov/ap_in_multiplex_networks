def create_multiplex_network(parameters):

    network_type = parameters.pop('type').lower()

    if network_type == 'erdos_renyi':
        from erdos_renyi_network import erdos_renyi_network
        generator = erdos_renyi_network

    elif network_type == 'scale_free':
        from scale_free_network import scale_free_network
        generator = scale_free_network

    else:
        raise NotImplementedError(f'{network_type} networks are not supported')

    G1 = generator(parameters)
    G2 = generator(parameters)
    return G1, G2

