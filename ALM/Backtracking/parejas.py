# -*- coding: utf-8 -*-
import sys

########################################################
########  Backtracking 1: Parejas de Skolem	########
########################################################
# Julia Penadés

def mostra_solucions(p):
	for i in xrange(len(p)): print "Solucion",i+1,": ",p[i]

def parejas(n,allSolutions):
	# crear i inicializar vectors accesibles per backtracking
	sol = [None] * n		# Solucions
	ocu = [False] * (2*n)		# Ocupats (long solucions*2)
	par = []			# Cadena amb solucions backtracking
	
	def backtracking(index):
		
		if index == -1:	# Final de l'arbre
			par.append(sol[:])
			return True
		
		# i+1, primer nombre en posicio 0
		for i in xrange(len(ocu)-index-1):
			if (ocu[i] == False and ocu[index+i+1] == False):
				ocu[i] = True
				ocu[index+i+1] = True
				sol[index] = i+1, index+i+2	# Tupla posicions index

				# Si acaba una branca (o demana totes les sol), reinicialitza
				if not backtracking (index-1) or allSolutions: 
					ocu[i] = False
					ocu[index+i+1] = False
				else: return True
			
		return allSolutions
	
	# Recorrem l'arbre desde els valors mes alts (sol rapida)	
	if n%4 not in [0,1] or not backtracking(n-1):
		return "NO HAY SOLUCION"
	# generar i retornar cadena resultant
	return par

if __name__ == "__main__":
	if len(sys.argv) not in [2,3]:
		print "\nUsage: %s N [TODAS]\n" % (sys.argv[0],)
		sys.exit()
	N = int(sys.argv[1])
	allSolutions = len(sys.argv) == 3 and sys.argv[2] == "TODAS"

	# Mostra per pantall en funcio de peticio de l'usuari	
	p = parejas(N,allSolutions)
	if allSolutions:
		mostra_solucions(p)
	else: print "Solucion:", p
