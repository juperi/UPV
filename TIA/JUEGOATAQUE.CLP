;;; TIA Práctica 2 Fuzzy-CLIPS
;;; Julia Penadés

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;; Declaración variables difusas ;;;;;;;;;;;;;;;

;;; Entrada: Puntos de Salud Jugador

(deftemplate saludPJ		;Variable difusa 
	0 100 Puntos 			;Universo
	((cercana_a_la_muerte (0 1) (50 0))	;Valores difusos
	(buena (15 0) (50 1) (85 0))
	(excelente (50 0) (100 1))))

;;; Entrada: Puntos de Salud Enemigo

(deftemplate saludEN		;Variable difusa 
	0 100 Puntos 			;Universo
	((cercana_a_la_muerte (0 1) (50 0))	;Valores difusos
	(buena (15 0) (50 1) (85 0))
	(excelente (50 0) (100 1))))

;;; Salida: Fuerza a aplicar (Tipo de Estrategia)

(deftemplate fuerza		;Variable difusa 
	0 5 Puntos 			;Universo
	((salir_corriendo (0 1) (1 1) (2 0))	;Valores difusos
	(ataque_defensivo (1 0) (2 1) (3 1) (4 0))
	(dejar_contrario_ko (3 0) (4 1) (5 1))))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;; Función fuzzify ;;;;;;;;;;;;;;;;;;;;;;

(deffunction fuzzify (?fztemplate ?value ?delta)
	(bind ?low (get‐u‐from ?fztemplate))
	(bind ?hi (get‐u‐to ?fztemplate))
	(if (<= ?value ?low)
		then
			(assert‐string
				(format nil "(%s (%g 1.0) (%g 0.0))" ?fztemplate ?low ?delta))
		else
			(if (>= ?value ?hi)
				then
					(assert‐string
						(format nil "(%s (%g 0.0) (%g 1.0))"
							?fztemplate (‐ ?hi ?delta) ?hi))
			else
				(assert‐string
					(format nil "(%s (%g 0.0) (%g 1.0) (%g 0.0))"
						?fztemplate (max ?low (‐ ?value ?delta))
						?value (min ?hi (+ ?value ?delta)) ))
)))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;; Entrada de datos ;;;;;;;;;;;;;;;;;;;;;;

(defrule leerconsola
	(initial‐fact)
	=>
	(printout t "Introduce tus puntos de salud (Del 1 al 100): ")
	(bind ?tu_salud (read))
	(fuzzify saludPJ ?tu_salud 0)
	(printout t "Introduce los puntos de salud del enemigo (Del 1 al 100): ")
	(bind ?en_salud (read))
	(fuzzify saludEN ?en_salud 0))
	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;; Estrategias a seguir ;;;;;;;;;;;;;;;;;;;;;

;;; Estrategia 1: Salir corriendo

(defrule corre 		;Regla asignar estrategia
	(saludPJ cercana_a_la_muerte)
	(saludEN not cercana_a_la_muerte)
	=>
	(printout t "Estas en peligro, corre!"crlf)
	(assert (fuerza salir_corriendo)))

;;; Estretegia 2: Dejar al contrario fuera de combate.

(defrule dejaKO1
	(saludPJ not cercana_a_la_muerte)
	(saludEN cercana_a_la_muerte)
	=>
	(printout t "Tu enemigo está a punto de morir, déjalo KO!"crlf)
	(assert (fuerza dejar_contrario_ko)))

(defrule dejaKO2
	(saludPJ excelente)
	(saludEN not excelente)
	=>
	(printout t "Tienes suficiente salud para derrotar a tu enemigo, déjalo KO!"crlf)
	(assert (fuerza dejar_contrario_ko)))

;;; Estrategia 3: Ataque defensivo 

(defrule defiende1 
	(saludPJ cercana_a_la_muerte)
	(saludEN cercana_a_la_muerte)
	=>
	(printout t "La cosa está igualada... defiéndete!"crlf)
	(assert (fuerza ataque_defensivo)))

(defrule defiende2 
	(saludPJ buena)
	(saludEN not cercana_a_la_muerte)
	=>
	(printout t "La cosa está igualada... defiéndete!"crlf)
	(assert (fuerza ataque_defensivo)))

(defrule defiende3 
	(saludPJ excelente)
	(saludEN excelente)
	=>
	(printout t "La cosa está igualada... defiéndete!"crlf)
	(assert (fuerza ataque_defensivo)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;; Funciones defusificación ;;;;;;;;;;;;;;;;;;;

(defrule defusificar
	(fuerza ?f)
	=> 
	(bind ?e (maximum-defuzzify ?f)) ;Media de maximos
	(printout t "Tu fuerza (Del 1 al 5) es de " ?e crlf)
	(halt)) ;Acaba

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;; Dibuja funciones pertenencia ;;;;;;;;;;;;;;;

;;; Valores de Salud jugador (idem enemigo)

(plot-fuzzy-value t "*+#" 0 100
	(create-fuzzy-value saludPJ cercana_a_la_muerte)
	(create-fuzzy-value saludPJ buena)
	(create-fuzzy-value saludPJ excelente))

;;; Valores de Fuerza

(plot-fuzzy-value t "*+#" 0 5
	(create-fuzzy-value fuerza salir_corriendo)
	(create-fuzzy-value fuerza ataque_defensivo)
	(create-fuzzy-value fuerza dejar_contrario_ko))