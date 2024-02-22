Implementation of the geometric multiplex model by Saeed Osat (thanks a lot). 

Cite: Hidden geometric correlations in real multiplex networks, Nature Physics 12, 1076–1081 (2016), K. Kleineberg, M. Boguna, M. A. Serrano, F. Papadopoulos
[The model is described in detail in the Supplementary Material of the paper]
&
Geometric correlations mitigate the extreme vulnerability of multiplex networks against targeted attacks,
Phys. Rev. Lett. 118, 218301 (2017), K. Kleineberg, L. Buzna, F. Papadopoulos, M. Boguñá, and M. A. Serrano

Run:
python Main.py

Parameters can be adjusted in Main.py. 
L=2 #Number of Layers (for now only 2 layers are supported)
N=5000 #Number of nodes in each layer
nu=0.5 #Strength of radial (degree) correlations, 0=none, 1=max.
g=0.5 #Strength of angular (similarity) correlations, 0=none, 1=max.

gamma1=2.5 #Power law degree distribution exponent in layer 1
kbar1=6.0 #Expected mean degree in layer 1. For gamma close to 2, the realized mean degree might differ. See Nature communications 1 : 62 [Supplementary Materials] for details.
T1=0.4 #Temperature in layer 1. Low T means high clustering. T in (0,1). 

#the same for layer 2.
gamma2=2.5
kbar2=6.0
T2=0.4

Output:
coords1.txt #Coordinates in layer 1
links.coords1.txt #Edgelist for layer 1
coords2.txt #Coordinates in layer 2
links.coords2.txt #Edgelist for layer 2
Note: Node IDs are the same in both layers, i.e. node with ID 3 in layer 1 is also node 3 in layer 2. 
Coordinates file has the format: 
Node_ID	Radial_Coordinate (r)	Angular_Coordinate (theta)	Kappa 
0 	18.3047358711 		4.40048774755 			5.3482539559
Edgelist files have the format:
Node_ID1	Node_ID2
1		2		-> Means an undirected edge between nodes 1 and 2. 




For feedback, please contact 
Saeed Osat: saeedosat13@gmail.com
and
Kaj-Kolja Kleineberg: kkleineberg@ethz.ch
