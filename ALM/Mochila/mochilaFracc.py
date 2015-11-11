#!/usr/bin/env python
# -*- coding: utf-8 -*-

##############################################################
###########	 Mochila con fraccionamiento	##############
##############################################################
# Julia Penadés

import sys

# Funcions
# ---------------------------------------------------------------------
def show_fractional_knapsack(x, v, w):
	print "Valor total: %s" % sum(x[i]*v[i] for i in xrange(len(x))), "euros."
	print "Carga total: %s" % sum(x[i]*w[i] for i in xrange(len(x))), "gramos."
	print "Detalle de la carga: %s\n" \
		% "".join("\n\t%.1f%% del producto %s" % (100*x[i], L[i]) \
					for i in xrange(len(x)) if x[i] > 0)

def fractional_knapsack(w, v, W):
	x = [0] * len(w)
	v = sorted([(v[i]/float(w[i]), i) for i in xrange(len(w))], reverse=True)
	
	for (ratio, i) in v:
		x[i] = min(1, W/float(w[i]))
		W -= x[i] * w[i]		
	return x

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
    print u"Ús: mochilaFracc.py <nomFitxer>"
    sys.exit(1)

nomf = sys.argv[1]
f = open(nomf, "r")

f.seek(0,0)
linies = []
while True:
	linia = f.readline() # Torna una linia
	linies.append(linia) # Guarda cada linia a la llista de linies
	if len(linia) == 0: break # Quan ja no en queden, acaba

# Carrega de la motxilla (W)
ultima = linies[len(linies)-2] # linies-1 seria una linia buida
W = ""
for i in ultima: 
	if i.isdigit(): W += i
W = int(W) 
W = W*1000

# Pesos i valors
v,w = [],[]
L = [] # Llista per guardar nom objectes
for i in range(1,len(linies)-2,1): # Mirem cada objecte
	x = linies[i].split(" pesa") # Dividim la linia en dos parts
	L.append(x[0]) # Noms objectes (1a part)
	
	x = x[1].split(" vale") # Ens quedem sols amb la 2a part
	pes = x[0].split( )
	valor = x[1]. split ( )	
	w.append(eq_pes(pes))
	v.append(eq_valor(valor))

# Mostrar per pantalla
q = fractional_knapsack(w, v, W)
show_fractional_knapsack(q, v, w)
