# -*- coding: utf-8 -*-
import sys

############################################################
#######	 Backtracking 2: Secuencia de Skolem	############
############################################################
# Julia Penadés

def mostra_solucions(s):
	for i in xrange(len(s)): print "Solucion",i+1,": ",s[i]

def secuencia(n, allSolutions):
	# crear i inicializar vectors accesibles per backtracking
	sol = [None] * (2*n)	# Solucions (long 2*n per guardar directament)
	ocu = [False] * (2*n)	# Ocupats (long n)
	sec = []				# Cadena amb solucions backtracking
	
	# Metode que busca secuencia directament
	def backtracking(index):

		if index == -1:	# Final de l'arbre
			sec.append(sol[:])
			return True
		
		# i+1, primer nombre en posicio 0
		for i in xrange(len(ocu)-index-1):
			if (ocu[i] == False and ocu[index+i+1] == False):
				ocu[i] = True
				ocu[index+i+1] = True
				sol[i] = index+1				# Pos 1 index (index+1)
				sol[i + index+1] = index+1		# Pos 2 index

				# Si acaba una branca (o demana totes les sol), reinicialitza
				if not backtracking (index-1) or allSolutions: 
					ocu[i] = False
					ocu[index+i+1] = False
					sol[i] = None
					sol[i + index+1] = None 
				else: return True
			
		return allSolutions
	
	# Recorrem l'abre desde els valors mes alts (sol rapida)	
	if n%4 not in [0,1] or not backtracking(n-1):
		return "NO HAY SOLUCION"
	# generar i retornar cadena resultant
	return sec

if __name__ == "__main__":
	if len(sys.argv) not in [2,3]:
		print "\nUsage: %s N [TODAS]\n" % (sys.argv[0],)
		sys.exit()
	N = int(sys.argv[1])
	allSolutions = len(sys.argv) == 3 and sys.argv[2] == "TODAS"

	# Mostra per pantall en funcio de peticio de l'usuari	
	s = secuencia(N,allSolutions)
	if allSolutions:
		mostra_solucions(s)
	else: print "Solucion:", s
