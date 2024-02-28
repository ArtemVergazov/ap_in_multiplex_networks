import re
import os, sys
import numpy as np
import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
import collections
import operator
from operator import itemgetter
from scipy import stats
from collections import namedtuple
import subprocess
import time
import numpy.random as rnd
from scipy.special import lambertw, erf, erfinv

####################################################################################

def CalculateKmin (kbar, gamma):
    return ( kbar*(gamma-2.0)/(gamma-1) )

def CalculateC (kbar, T, gamma):
    return ( kbar*np.sin(T*np.pi)/(2.0*T)*np.power( ((gamma-2.0)/(gamma-1.0)),2.0) )

def CalculateR (N,C):
    return (2.0*np.log(N/C))

def SampleKappa(N, kmin1, gamma1):
    kappa=[None]*N
    for i in range(N):
        kappa[i]=kmin1*np.power(1.0-rnd.random_sample(),1.0/(1.0-gamma1))
    return kappa

def SampleTheta(N):
    theta=[None]*N
    for i in range(N):
        theta[i]=2.0*np.pi*rnd.random_sample()
    return theta

def SampleConditionalKappa(N, nu, kappa1, kmin1, gamma1, kmin2, gamma2):
    kappa2=[None]*N
    if nu==1:
        for i in range(N):
            kappa2[i]=kmin2*np.power( (kappa1[i]/kmin1), ((1.0-gamma1)/(1.0-gamma2)) )
    elif nu==0:
        for i in range(N):
            kappa2[i]=kmin2*np.power(1.0-rnd.random_sample(),1.0/(1.0-gamma2))
    else:
        for i in range(N):
            phi=-np.log( 1.0- np.power((kmin1/kappa1[i]),(gamma1-1.0)) )
            z=1.0/kmin1* np.power(phi,(nu/(nu-1.0)))*np.power(kappa1[i],-gamma1)
            z= z * ( kmin1*np.power(kappa1[i],gamma1) - np.power(kmin1,gamma1)*kappa1[i] )
            zr= z*rnd.random_sample()
            zr = (nu/(1.0-nu))*lambertw( np.power(zr,((nu-1.0)/nu))/(nu/(1.0-nu)) )
            zr = np.power(zr,(1.0/(1.0-nu)))-np.power(phi,(1.0/(1.0-nu)))
            zr = np.exp(-np.power(zr, (1.0-nu)) )
            zr=kmin2*np.power(1.0-zr, (1.0/(1.0-gamma2)))
            kappa2[i]=zr
    return np.array(kappa2).real

def SampleConditionalTheta(N, g, theta1):
    theta2=[None]*N
    if g==1:
        theta2=[i for i in theta1]
    elif g==0:
        for i in range(N):
            theta2[i]=2.0*np.pi*rnd.random_sample()
    else:
        twoPI= 2*np.pi
        sigma0=N/(4.0*np.pi)
        if sigma0>100.0:
            sigma0=100.0
        sigma=sigma0*(1.0/g-1.0)
        for i in range(N):
            l=np.sqrt(2.0)*sigma*erfinv( (-1.0+2.0*rnd.random_sample()) * erf(N/(2*np.sqrt(2)*sigma)) )
            theta2[i]=(theta1[i]+twoPI*l/N)%(twoPI)
    return theta2

def ChangeVariablesFromS1ToH2(N, kappa, R, kmin):
    r=[None]*N
    for i in range(N):
        r[i]=R-2*np.log(kappa[i]/kmin)
        if r[i]<0.0:
            r[i]=0.0
    return r

def PrintCoordinates(r,theta,kappa,name):
    file = open(name,"w")
    N=len(r)
    for i in range(N):
        print(i, r[i], theta[i], kappa[i], file=file)
    file.close()

def CreateNetworks(kappa, theta, T, kbar):
    N=len(kappa)
    twoPI= 2*np.pi
    links=[]
    edge=0
    invT=1.0/T
    for i in range(N-1):
        for j in range(i+1,N):
            dTheta=N/(twoPI)*np.abs(np.pi-np.abs(np.pi-np.abs(theta[i]-theta[j])))
            mu=np.sin(T*np.pi)/(twoPI*T*kbar)
            r=dTheta/(mu*kappa[i]*kappa[j])
            if rnd.random_sample()<(1.0/(1.0+np.power(r,invT))):
                links.append((i,j))
                edge+=1
    print("number of links:\t", edge)
    return links

def PrintNetwork(links,name):
    file = open(name,"w")
    for i in links:
        print(i[0], i[1], file=file)

def ReadLinks(name):
    links=[]
    edge=0
    file = open(name,"r")
    for row in file:
        if row and not "#" in row:
            i = int(row.split()[0])
            j = int(row.split()[1])
            links.append((i,j))
            edge+=1
    file.close()
    print("number of links:\t", edge)
    return links

def ReadCoordinates(name):
    coords = []
    file = open(name,"r")
    for row in file:
        if row and not "#" in row:
            coords.append( (int(row.split()[0]) , float(row.split()[1]) , float(row.split()[2])) )

    file.close()
    coords.sort(key=lambda x:x[0])
    return coords

def PlotNetwork(links, r, theta, name, ax=None, savefig=False):

    plt.rc('text', usetex=True)
    plt.rc('font', size=22, **{'family':'sans-serif','sans-serif':['Helvetica']})
    plt.rcParams['xtick.major.pad'] = 8
    plt.rcParams['ytick.major.pad'] = 8

    if ax is None:
        fig = plt.figure()
        fig.set_size_inches(5,5)
        ax = fig.add_subplot(1,1,1,projection='polar')
    else:
        fig = ax.figure

    for x in links:
        i = x[0]
        j = x[1]
        ax.plot([theta[i],theta[j]], [r[i],r[j]],'-',color = 'k', linewidth = 0.01, alpha=0.2)

    ax.plot(theta, r,'o', color = 'orange', markeredgecolor='orange', markersize = 1.5, alpha=0.83)



    ax.grid(False)

    ax.spines['polar'].set_visible(False)

    ax.set_yticklabels([])
    ax.set_xticklabels([])

    #ax.set_ylabel('Relative size of the structural set', fontsize=18)
    #ax.set_xlabel('Relabeling probability', fontsize=18)
    #ax.locator_params(nbins=1)
    #ax.set_xlim(-0.05, 1.05)
    #ax.set_ylim(0.15, 0.55)

    #ax.legend(loc='upper left', numpoints=1, markerscale=1.0,  prop={'size':11})

    fig.tight_layout()
    if savefig:
        fig.savefig(name+'.pdf', bbox_inches = 'tight')

####################################################################################

