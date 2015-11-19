;; Juego del siete y medio
;; Julia Penadés Ribera

(do-backward-chaining empieza)
(do-backward-chaining repartepc)
(do-backward-chaining reparte)
(do-backward-chaining contador)
(do-backward-chaining contadorpc)
(do-backward-chaining ganado)

;; Hechos y reglas

(deffacts sieteymedio
    (contador 0) ; Inicialmente el jugador tiene 0 puntos 
    (contadorpc 0) ; Inicialmente el ordenador tiene 0 puntos
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defrule empezar
    (empieza ?e)
    (test (eq ?e si))
=> 
    (printout t "Allá vamos! " crlf)
    (assert (repartepc si)) ; Da permiso al pc para empezar
)

(defrule acabar
    (empieza ?e)
    (test (eq ?e no))
=> 
    (printout t "Fin del juego!" crlf) 
    (facts) ; Mirar lo hechos ocurridos a lo largo del programa
    (exit)
)

(defrule plantarse
    (reparte ?r)
    (test (eq ?r no))
    (contador ?c)
    (contadorpc ?pc)
=> 
    (printout t "Te has plantado con " ?c " puntos." crlf)
    (printout t "El ordenador tiene " ?pc " puntos." crlf)
    (if (or(eq ?c 7.5)(and(< ?c 7.5)(or(>= ?c ?pc)(> ?pc 7.5)))) then ; El jugador gana si = 7.5 o si es <7.5 y > que el pc
        (assert (ganado si))
	else
        (assert (ganado no))
    )
)

(defrule repartir_ordenador
    (repartepc ?rpc)
    (test (eq ?rpc si))
	(contadorpc ?pc)   
 =>
    (if (<= ?pc 5) then ; Mientras la carta del ordenador sea menor o igual que 5, pedirá otra  
    	(bind ?carta (mod (random) 13)) ; Numeros aleatorios entre 0 y 12
   		(while (or (or (eq ?carta 0)(eq ?carta 8))(eq ?carta 9)) ; Si la carta es 0,8 o 9
			(bind ?carta (mod (random) 13)) ; Reparte otra
		)
    	;(printout t "La carta del ordenador es: " ?carta crlf)
    	(if (>= ?carta 10) then ; Si la carta es sota, caballo o rey, valdrá 0.5
        	(bind ?carta 0.5)
    	)
    	(bind ?pc (+ ?pc ?carta)) ; Se suma la puntuación de la carta al contador
        (assert (contadorpc ?pc))
        ;(printout t "El ordenador lleva " ?pc " puntos." crlf)
     else
        (assert (reparte si)); Empieza a repartir al jugador
     ) 
) 

(defrule repartir
    (reparte ?r)
    (test (eq ?r si))
	(contador ?c)
 =>
    (bind ?carta (mod (random) 13)) ; Numeros aleatorios entre 0 y 12
    (while (or (or (eq ?carta 0)(eq ?carta 8))(eq ?carta 9)) ; Si la carta es 0,8 o 9
		(bind ?carta (mod (random) 13)) ; Reparte otra
	)
    (printout t "Tu carta es: " ?carta crlf)
    (if (>= ?carta 10) then ; Si la carta es sota, caballo o rey, valdrá 0.5
        (bind ?carta 0.5)
    )
    (bind ?c (+ ?c ?carta))
    (assert (contador ?c))
    (printout t "Llevas " ?c " puntos." crlf)
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defrule perder
    (ganado ?g)
    (test (eq ?g no))
 => 
 	(printout t "Has perdido!" crlf)
    (assert (empieza no )) ; Acaba el juego
)

(defrule ganar
    (ganado ?g)
    (test (eq ?g si))
    (contador ?c)
 => 
    (if (eq ?c 7.5) then ; El jugador gana si = 7.5 o si es <7.5 y > que el pc
        (printout t "Has ganado! 7.5 exactos, que suerte!" crlf)
	else
        (printout t "Has ganado!" crlf)
    )
 	
    (assert (empieza no )) ; Acaba el juego
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defrule pregunta-empezar
    (declare(salience 100))
    (need-empieza ?)
 =>
 (printout t "Quieres empezar la partida?: ")  
    (assert (empieza (read)))  
    )

(defrule pregunta-carta
    (declare(salience 50))
    (reparte ?r)
    (contador ?c) 
    (test (and(eq ?r si)(> ?c 0))) ; Cada vez que actualize el contador, preguntará si quiere otra carta
 =>
    (printout t "Quieres una carta?: ")
    (assert (reparte (read)))
    )

(reset)
(run)