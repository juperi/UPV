#!/usr/bin/env python
# -*- coding: utf-8 -*-

##############################################################
###########	 Mochila sin fraccionamiento	##############
##############################################################
# Julia Penadés

import random

# Funcions
# ---------------------------------------------------------------------

def generateKnapsack(Nmin,Nmax,Wmin,Wmax):
	#"devuelve v,w"
	N = random.randrange(Nmin,Nmax)
	W = random.randrange(Wmin,Wmax)
	v = [ random.randrange(1, 10000) for i in range(N) ]
	w = [ random.randrange(1, W) for i in range(N) ]
	return N,W,v,w
	
def iterative_knapsack_profit(W, v, w):
	V = {}
	for c in xrange(W+1):
		V[0,c] = 0
	for i in xrange(1, len(v)+1):
		V[i,0] = 0
		for c in xrange(1, min(W+1,w[i-1])):
			V[i,c] = V[i-1,c]
		for c in xrange(w[i-1], W+1):
			V[i,c] = max(V[i-1,c], V[i-1,c-w[i-1]] + v[i-1])
	return V[len(v), W]

def greedy_knapsack(w, v, W):
	V = 0 # Valor motxilla
	vini = v
	x = [0] * len(w)
	v = sorted([(v[i]/float(w[i]), i) for i in xrange(len(w))], reverse=True)
	
	for (ratio, i) in v:
		x[i] = min(1, W/float(w[i]))
		if x[i] != 1: 
			x[i] = 0 # Si no cap sencer, no el posem
		W -= x[i] * w[i]
	V = sum(x[i]*vini[i] for i in xrange(len(x)))	
	return V	

def mostrar_soluciones(soluciones):
	histograma = 101*[0]
	acumulado = 101*[0]
	total = float(len(soluciones))
	for (exacta,aprox) in soluciones:
		ratio = int(100*aprox/float(exacta))
		histograma[ratio] += 1
	print histograma
	acumulado[100] = histograma[100]
	for i in xrange(99,-1,-1):
		acumulado[i] = histograma[i]+acumulado[i+1]
	i = 0
	while i<100 and acumulado[i+1]==100:
		i+=1
	while i<100:
		print "El %7.2f%% de las soluciones voraces estan a o por encima del "\
		"%3d%% respecto a la optima" % (100*acumulado[i]/total,i)
		i+=1
	print "El %7.2f%% de las soluciones voraces coinciden con la optima."\
		% (100*acumulado[100]/total)

def eq_pes(l): # Trau equivalencia de pes a g
	# {"kg":1000, "g":1}
	x = int(l[0])
	if l[1] == "kg":
		x = x*1000
	return x
	
def eq_valor(l): # Trau equivalencia de valor a euros
	# { "euros":1, "dolares":0.74, "libras":1.19}
	x = float(l[0])
	if l[1] == "dolares":
		x = x*0.74
	elif l[1] == "libras":
		x = x*1.19
	return x

# ---------------------------------------------------------------------

# Histograma
soluciones = []
for i in range(100):
	N,W,v,w = generateKnapsack(50,100,1000,5000)
	soloptima = iterative_knapsack_profit(W,v,w)
	solaprox = greedy_knapsack(w,v,W)
	soluciones.append((soloptima,solaprox))

mostrar_soluciones(soluciones)

