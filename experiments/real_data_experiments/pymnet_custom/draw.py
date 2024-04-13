import pymnet
from pymnet.visuals.layouts import get_layout
from pymnet.visuals.drawassigners import (
    LayerColorAssigner, LayerAlphaAssigner, LayerLabelAssigner, LayerLabelLocAssigner,
    LayerLabelSizeAssigner, LayerLabelColorAssigner, LayerLabelStyleAssigner,
    LayerLabelAlphaAssigner, LayerOrderAssigner, NodeLabelAssigner, NodeLabelSizeAssigner,
    NodeLabelColorAssigner, NodeLabelStyleAssigner, NodeLabelAlphaAssigner, NodeSizeAssigner,
    NodeColorAssigner,
)


def draw(net, **kwargs):
    if 'backend' in kwargs and kwargs['backend'] == 'custom':

        nodeCoords = kwargs['nodeCoords']

        assert net.aspects == 1

        # Get coordinates
        ncoords, nlcoords = get_layout('spring', net, alignedNodes=True)

        for node, coord in nodeCoords.items():
            ncoords[node] = coord

        # Initialize assigners
        layerColor = LayerColorAssigner(layerColorDict, layerColorRule, defaultLayerColor, net)
        layerAlpha = LayerAlphaAssigner(layerAlphaDict, layerAlphaRule, defaultLayerAlpha, net)
        layerLabel = LayerLabelAssigner(layerLabelDict, layerLabelRule, defaultLayerLabel, net)
        layerLabelLoc = LayerLabelLocAssigner(layerLabelLocDict, layerLabelLocRule, defaultLayerLabelLoc, net)
        layerLabelSize = LayerLabelSizeAssigner(layerLabelSizeDict, layerLabelSizeRule, defaultLayerLabelSize, net)
        layerLabelColor = LayerLabelColorAssigner(layerLabelColorDict, layerLabelColorRule, defaultLayerLabelColor, net)
        layerLabelStyle = LayerLabelStyleAssigner(layerLabelStyleDict, layerLabelStyleRule, defaultLayerLabelStyle, net)
        layerLabelAlpha = LayerLabelAlphaAssigner(layerLabelAlphaDict, layerLabelAlphaRule, defaultLayerLabelAlpha, net)
        layerOrder = LayerOrderAssigner(layerOrderDict, layerOrderRule, defaultLayerOrder, net)
        nodeLabel = NodeLabelAssigner(nodeLabelDict, nodeLabelRule, defaultNodeLabel, net)
        nodeLabelSize = NodeLabelSizeAssigner(nodeLabelSizeDict, nodeLabelSizeRule, defaultNodeLabelSize, net)
        nodeLabelColor = NodeLabelColorAssigner(nodeLabelColorDict, nodeLabelColorRule, defaultNodeLabelColor, net)
        nodeLabelStyle = NodeLabelStyleAssigner(nodeLabelStyleDict, nodeLabelStyleRule, defaultNodeLabelStyle, net)
        nodeLabelAlpha = NodeLabelAlphaAssigner(nodeLabelAlphaDict, nodeLabelAlphaRule, defaultNodeLabelAlpha, net)
        nodeSize = NodeSizeAssigner(nodeSizeDict, nodeSizeRule, defaultNodeSize, net)
        nodeColor = NodeColorAssigner(nodeColorDict, nodeColorRule, defaultNodeColor, net)
        edgeColor = EdgeColorAssigner(edgeColorDict, edgeColorRule, defaultEdgeColor, net)
        edgeWidth = EdgeWidthAssigner(edgeWidthDict, edgeWidthRule, defaultEdgeWidth, net)
        edgeStyle = EdgeStyleAssigner(edgeStyleDict, edgeStyleRule, defaultEdgeStyle, net)
        edgeAlpha = EdgeAlphaAssigner(edgeAlphaDict, edgeAlphaRule, defaultEdgeAlpha, net)
        edgeZ = EdgeZAssigner(edgeZDict, edgeZRule, defaultEdgeZ, net)


        #Choose the backend for drawing
        if backend not in possible_backends:
            raise Exception("Unknown backend: "+str(backend))
        if backend not in imported_backends:
            print("There was an error importing backend '"+str(backend)+"'.")
            print("Please use one of the available backends: "+" ".join(imported_backends))
            print("The following error was raised:")
            raise import_errors[backend]
        if backend == "mpl":
            NetFigureBE, LayerBE, NodeBE, EdgeBE = NetFigureMPL, LayerMPL, NodeMPL, EdgeMPL
        elif backend == "threejs":
            NetFigureBE, LayerBE, NodeBE, EdgeBE = NetFigureThreeJS, LayerThreeJS, NodeThreeJS, EdgeThreeJS

        #Build the network
        layers = {}
        nodes = {}
        nf = NetFigureBE(figsize=figsize, layergap=layergap, padding=layerPadding, azim=azim, elev=elev, show=show, camera_dist=camera_dist, autoscale=autoscale)
        for layer in sorted(net.iter_layers(), key=lambda l:layerOrder[l]):
            layerLabelArgs = {"size":layerLabelSize[layer], "color":layerLabelColor[layer], "style":layerLabelStyle[layer], "alpha":layerLabelAlpha[layer]}
            layers[layer] = LayerBE(nf, shape=layershape, color=layerColor[layer], label=layerLabel[layer], alpha=layerAlpha[layer], labelloc=layerLabelLoc[layer], labelArgs=layerLabelArgs)

        for nl in net.iter_node_layers():
            if nl in nlcoords:
                xy = nlcoords[nl]
            elif nl[0] in ncoords:
                xy = ncoords[nl[0]]
            else:
                xy = (random.random(), random.random())
            nodeLabelArgs = {"size":nodeLabelSize[nl], "color":nodeLabelColor[nl], "style":nodeLabelStyle[nl], "alpha":nodeLabelAlpha[nl]}
            nodes[nl] = NodeBE(layers[nl[1]], xy[0], xy[1], label=nodeLabel[nl], color=nodeColor[nl], size=nodeSize[nl], labelArgs=nodeLabelArgs)

        for nl1 in net.iter_node_layers():
            for nl2 in net[nl1]:
                EdgeBE(nodes[nl1], nodes[nl2], color=edgeColor[(nl1, nl2)], width=edgeWidth[(nl1, nl2)], style=edgeStyle[(nl1, nl2)], z=edgeZ[(nl1, nl2)], alpha=edgeAlpha[(nl1, nl2)])

        return nf.draw(ax=ax)

