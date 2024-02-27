import re
import os, sys
import subprocess
import time

import numpy as np
import numpy.random as rnd

import Hyperbolic as hyp
from draw_hyperbolic import draw_hyperbolic

############################# Adjust GMM Parameters ###############################
#random number generator seed
seed=10

L=2
N=5000
nu=1.
g=1.

gamma1=3.
kbar1=6.0
T1=0.4

gamma2=3.
kbar2=6.0
T2=0.4

####################################################################################

t1=time.time()

np.random.seed(seed)

kmin1=hyp.CalculateKmin(kbar1, gamma1)
print("kmin1: ",kmin1)

C1=hyp.CalculateC(kbar1, T1, gamma1)
print("C1: ",C1)

R1=hyp.CalculateR(N,C1)
print("R1: ",R1)

kmin2=hyp.CalculateKmin(kbar2, gamma2)
print("kmin2: ",kmin2)

C2=hyp.CalculateC(kbar2, T2, gamma2)
print("C2: ",C2)

R2=hyp.CalculateR(N,C2)
print("R2: ",R2)


kappa1=hyp.SampleKappa(N, kmin1, gamma1)

kappa2=hyp.SampleConditionalKappa(N, nu, kappa1[:], kmin1, gamma1, kmin2, gamma2)

theta1=hyp.SampleTheta(N)

theta2=hyp.SampleConditionalTheta(N, g, theta1[:])


r1=hyp.ChangeVariablesFromS1ToH2(N, kappa1[:], R1, kmin1)
r2=hyp.ChangeVariablesFromS1ToH2(N, kappa2[:], R2, kmin2)

hyp.PrintCoordinates(r1[:],theta1[:],kappa1[:],"coords1.txt")
hyp.PrintCoordinates(r2[:],theta2[:],kappa2[:],"coords2.txt")




#Do this part Fast in C++

# links1=hyp.CreateNetworks(kappa1,theta1,T1,kbar1)
# links2=hyp.CreateNetworks(kappa2,theta2,T2,kbar2)

# hyp.PrintNetwork(links1,"el1.txt")
# hyp.PlotNetwork(links1,r1,theta1,"layer1")

# hyp.PrintNetwork(links2,"el2.txt")
# hyp.PlotNetwork(links2,r2,theta2,"layer2")


# os.chdir('experiments/hyperbolic')

subprocess.call(["hyperb.exe","coords1.txt",str(seed),str(N),str(kbar1),str(T1)])
links1=hyp.ReadLinks("links.coords1.txt")
# hyp.PlotNetwork(links1[:],r1[:],theta1[:],"layer1")


subprocess.call(["hyperb.exe","coords2.txt",str(seed+1),str(N),str(kbar2),str(T2)])
links2=hyp.ReadLinks("links.coords2.txt")
# hyp.PlotNetwork(links2[:],r2[:],theta2[:],"layer2")

draw_hyperbolic(links1, links2, r1, r2, theta1, theta2, 'corr_0')

t2=time.time()

print("running time",(t2-t1))

####################################################################################
