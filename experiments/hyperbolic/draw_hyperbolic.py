from copy import deepcopy
import networkx as nx
import matplotlib.pyplot as plt
import numpy as np


def find_component(node, connected_components):
    for i, component in enumerate(connected_components):
        if node in component:
            return i
    raise ValueError(f'Component for node {node} not found')


def cascade_remove_links(G):

    keep_going = True
    while keep_going:
        keep_going = False

        clusters_in_layers = [list(nx.connected_components(g)) for g in G]

        for i, g in enumerate(G):
            clusters_to_search = clusters_in_layers[:i] + clusters_in_layers[i + 1:]
            for edge in g.edges():
                if any(find_component(edge[0], clusters) != find_component(edge[1], clusters)
                       for clusters in clusters_to_search):
                    keep_going = True
                    g.remove_edge(*edge)

    return G


def articulation_points_multiplex(G):
    ap = set(nx.articulation_points(G[0]))
    return ap.union(*[nx.articulation_points(g) for g in G[1:]])


def max_comp(g):
    comps = list(nx.connected_components(g))
    if len(comps) == 0:
        return 0
    return max(comps, key=len)


def draw_hyperbolic_monoplex(links, r, theta, ax=None):
    plt.rc('text', usetex=True)
    plt.rc('font', size=22, **{'family': 'sans-serif', 'sans-serif': ['Helvetica']})
    plt.rcParams['xtick.major.pad'] = 8
    plt.rcParams['ytick.major.pad'] = 8

    if ax is None:
        fig = plt.figure()
        fig.set_size_inches(5, 5)

        ax = fig.add_subplot(1, 1, 1, projection='polar')
    else:
        fig = ax.figure

    print('\nStarting AP removal')

    r = np.array(r)
    theta = np.array(theta)

    G = [nx.from_edgelist(links)]

    for i, j in links:
        ax.plot(
            [theta[i], theta[j]], [r[i], r[j]], '-',
            color='k', linewidth=.01, alpha=.2, label=None,
        )

    it = 1
    while True:
        print(f'AP removal: iteration {it}')
        G = cascade_remove_links(G)

        removed_at_this_iteration = []

        gcc = max_comp(G[0])
        for g in G:
            to_remove = [n for n in g if n not in gcc]
            g.remove_nodes_from(to_remove)
            removed_at_this_iteration.extend(to_remove)

        art_points = articulation_points_multiplex(G)

        for g in G:
            g.remove_nodes_from(art_points)
        removed_at_this_iteration.extend(art_points)
        removed_at_this_iteration = list(set(removed_at_this_iteration))

        print(f'AP removal: {len(art_points)} APs')
        print(f'AP removal: removed {len(removed_at_this_iteration)} nodes')

        ax.plot(
            theta[removed_at_this_iteration], r[removed_at_this_iteration], 'o',
            markersize=1.5,
            alpha=.83,
            color='tab:blue',
            # label=fr'$\mathrm{{Removed\ at\ iteration\ {it}}}$',
        )
        it += 1

        if not art_points:
            break

    rgb_nodes = [n for n in G[0]]
    ax.plot(
        theta[list(rgb_nodes)], r[list(rgb_nodes)], 'o',
        markersize=1.5,
        alpha=.83,
        color='#f78fb3',
        label=fr'$\mathrm{{RGCC}}$',
    )

    ax.grid(False)
    ax.spines['polar'].set_visible(False)
    ax.set_yticklabels([])
    ax.set_xticklabels([])

    fig.tight_layout()


def draw_hyperbolic_multiplex(links1, links2, r1, r2, theta1, theta2, ax1, ax2):
    # plt.rc('text', usetex=True)
    # plt.rc('font', size=22, **{'family': 'sans-serif', 'sans-serif': ['Helvetica']})
    # plt.rcParams['xtick.major.pad'] = 8
    # plt.rcParams['ytick.major.pad'] = 8

    print('\nStarting AP removal')

    r1 = np.array(r1)
    r2 = np.array(r2)
    theta1 = np.array(theta1)
    theta2 = np.array(theta2)

    G = [nx.from_edgelist(links) for links in [links1, links2]]
    G[0].add_nodes_from([n for n in G[1]])
    G[1].add_nodes_from([n for n in G[0]])

    for i, j in links1:
        ax1.plot([theta1[i], theta1[j]], [r1[i], r1[j]], '-', color='k', linewidth=.01, alpha=.2, label=None)
    for i, j in links2:
        ax2.plot([theta2[i], theta2[j]], [r2[i], r2[j]], '-', color='k', linewidth=.01, alpha=.2, label=None)

    it = 1
    while True:
        print(f'AP removal: iteration {it}')
        G = cascade_remove_links(G)
        gcc = max_comp(G[0])
        for g in G:
            g.remove_nodes_from([n for n in g if n not in gcc])

        art_points = articulation_points_multiplex(G)

        for g in G:
            g.remove_nodes_from(art_points)

        print(f'AP removal: {len(art_points)} APs')
        it += 1

        if not art_points:
            break

    rgb_nodes = list(G[0].nodes)
    assert sorted(rgb_nodes) == sorted([n for n in G[1]]), 'RGBs must be the same'

    ax1.plot(
        np.delete(theta1, rgb_nodes), np.delete(r1, rgb_nodes), 'o',
        markersize=1.5,
        alpha=.83,
        label=None,
        color='orange',
    )
    ax2.plot(
        np.delete(theta2, rgb_nodes), np.delete(r2, rgb_nodes), 'o',
        markersize=1.5,
        alpha=.83,
        label=None,
        color='orange',
    )

    ax1.plot(
        theta1[rgb_nodes], r1[rgb_nodes], 'o',
        markersize=1.5,
        alpha=.83,
        color='#f78fb3',
        label=None,
    )
    ax2.plot(
        theta2[rgb_nodes], r2[rgb_nodes], 'o',
        markersize=1.5,
        alpha=.83,
        label=fr'$\mathrm{{RGCC}}$',
        color='#f78fb3',
    )

    ax1.grid(False)
    ax2.grid(False)
    ax1.spines['polar'].set_visible(False)
    ax2.spines['polar'].set_visible(False)
    ax1.set_yticklabels([])
    ax1.set_xticklabels([])
    ax2.set_yticklabels([])
    ax2.set_xticklabels([])

    #ax1.set_ylabel('Relative size of the structural set', fontsize=18)
    #ax1.set_xlabel('Relabeling probability', fontsize=18)
    #ax1.locator_params(nbins=1)
    #ax1.set_xlim(-0.05, 1.05)
    #ax1.set_ylim(0.15, 0.55)

    # ax2.legend(
    #     bbox_to_anchor=(1, 1, .5, .15),
    #     loc='upper left',
    #     numpoints=1,
    #     markerscale=1.,
    #     prop={'size': 26},
    # )

