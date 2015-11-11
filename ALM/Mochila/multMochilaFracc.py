#!/usr/bin/env python
# -*- coding: utf-8 -*-

##############################################################
######## Múltiples mochilas con fraccionamiento	##############
##############################################################
# Julia Penadés

import sys

# Funcions
# ---------------------------------------------------------------------
def show_fractional_knapsack(Q, v, w):
	i=0
	for x in Q:
		print M[i], ":"
		print "Valor total: %s" % sum(x[i]*v[i] for i in xrange(len(x))), "euros."
		print "Carga total: %s" % sum(x[i]*w[i] for i in xrange(len(x))), "gramos."
		print "Detalle de la carga: %s\n" \
			% "".join("\n\t%.1f%% del producto %s" % (100*x[i], L[i]) \
						for i in xrange(len(x)) if x[i] > 0)
		i+=1

def fractional_knapsack(w, v, W):	

	Q, pos = [],[]
	v = sorted([(v[i]/float(w[i]), i) for i in xrange(len(w))], reverse=True) # Ordena objectes (Tupla)
	pos_res, res = -1,-1
	medio = False 
	for j in range(0,len(W),1): # Para cada mochila
			
		x = [0] * len(w)
		for (ratio, i) in v: # Recorre vector objectes
			
			if pos_res == i: # Si encara tenim part de l'objecte anterior
				x[i] = res # Guardem en posicio de l'objecte
				medio = True
				pos_res = 0				

			if i not in pos:
				per = W[j]/float(w[i])
				if per < 1: # Si no cap
					res = 1 - per # Guardem el que no cap
					pos_res = i

				else: # Cap sencer
					if medio: # Si tenim l'objecte partit
						per = res
						medio = False
					else: per = 1
					pos.append(i) # Em guarde la pos del objecte que hem gastat
					pos_res = 0
				
				x[i] = per # Guardem en posicio de l'objecte	
				W[j] -= per * w[i] # Guardem el que cap
				if W[j] == 0: break # Ja no cap res mes
		
		Q.append(x)
	return Q
		
#def fractional_knapsack(w, v, W):
#	x = [0] * len(w)
#	v = sorted([(v[i]/float(w[i]), i) for i in xrange(len(w))], reverse=True)
#	
#	for (ratio, i) in v:
#		x[i] = min(1, W/float(w[i]))
#		W -= x[i] * w[i]		
#	return x		

def eq_pes(l): # Trau equivalencia de pes a g
	# {"kg":1000, "g":1}
	x = float(l[0])
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

if len(sys.argv) != 2:
    print u"Ús: multMochilaFracc.py <nomFitxer>"
    sys.exit(1)

nomf = sys.argv[1]
f = open(nomf, "r")

f.seek(0,0)
linies = []
compta = 0
while True:
	linia = f.readline() # Torna una linia
	if linia.startswith("En la mochila"): compta += 1 # num motxilles
	linies.append(linia) # Guarda cada linia a la llista de linies
	if len(linia) == 0: break # Quan ja no en queden, acaba

total = len(linies)-1 # Quantitat de linies

# Carrega de les motxilles (W)
W, M = [],[]
carrega = 0 
while compta != 0:
	ultima = linies[total-1]
	x = ultima.split(" caben")
	M.append(x[0]) # Noms motxilles
	
	carga = x[1].split( )
	W.append(eq_pes(carga)) # Carregues motxilles
	
	total -= 1 # Anem a la linia d'avans
	compta -= 1
W = list(reversed(W))
M = list(reversed(M))

# Pesos i valors
v,w = [],[]
L = [] # Llista per guardar nom objectes
for i in range(1,total-compta,1): # Mirem cada objecte
	x = linies[i].split(" pesa") # Dividim la linia en dos parts
	L.append(x[0]) # Noms objectes (1a part)
	
	x = x[1].split(" vale") # Ens quedem sols amb la 2a part
	pes = x[0].split( )
	valor = x[1]. split ( )	
	w.append(eq_pes(pes))
	v.append(eq_valor(valor))

# Mostrar per pantalla
Q = fractional_knapsack(w, v, W)
show_fractional_knapsack(Q, v, w)
