; Control de Caldera a Presión
; Julia Penadés

(do-backward-chaining Pm)
(do-backward-chaining Tm)

; Hechos y reglas

(deffacts caldera
    (peligro peligroBajo)
    (peligro peligroMedio)
    (peligro peligroAlto)
    (peligro peligroMuyAlto)
)

(defrule peligroBajo
    (Presion ?p)
    (test (eq ?p BAJA)) ; Si la presión es baja
    (Temperatura ?t)
    (test (or (eq ?t BAJA) (eq ?t MEDIA))) ;Si temp es baja o media
    => (printout t "Peligro Bajo" crlf) 
    )

(defrule peligroMedio
    (Presion ?p)
    (Temperatura ?t)
    (test (or (and (eq ?p BAJA)(eq ?t ALTA)) (and (eq ?p ALTA)(eq ?t BAJA)) (and (eq ?p MEDIA)(or (eq ?t BAJA)(eq ?t MEDIA)))))
    => (printout t "Peligro Medio" crlf) 
    )

(defrule peligroAlto
    (Presion ?p)
    (Temperatura ?t)   
    (test (and (eq ?p ALTA)(eq ?t MEDIA)))
    => (printout t "Peligro Alto" crlf) 
    )

(defrule peligroMuyAlto
    (Presion ?p)
    (test (or (eq ?p MEDIA) (eq ?p ALTA)))
    (Temperatura ?t)
    (test (eq ?t ALTA))
    => (printout t "Peligro Muy Alto" crlf) 
    )

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defrule PresBaja
    (Pm ?Pm)
    (test (and(< ?Pm 10)(>= ?Pm 0))) ; Si la presión < 10 y >= 0
    => (assert(Presion BAJA))
    )
        
(defrule PresMedia
    (Pm ?Pm)                    
    (test (and(>= ?Pm 10)(< ?Pm 20))) ; Si la presión < 20 y >= 10
    => (assert(Presion MEDIA))
    )
    
(defrule PresAlta
    (Pm ?Pm)     
    (test (>= ?Pm 20)) ; Si la presión es >= 20
    => (assert(Presion ALTA)) 
    )

(defrule TempBaja
    (Tm ?Tm)
    (test (and(< ?Tm 100)(>= ?Tm 0)))
    => (assert(Temperatura BAJA))
    )

(defrule TempMedia
    (Tm ?Tm)    
    (test (and(>= ?Tm 100)(< ?Tm 200)))
    => (assert(Temperatura MEDIA))
    )

(defrule TempAlta
   (Tm ?Tm)
    (test (>= ?Tm 200))
    => (assert(Temperatura ALTA)) 
    )

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defrule PresAveria
    (Pm ?Pm)
    (test (< ?Pm 0))
 
 =>
 (printout t "Error: Introduce la presión del calentador: ") 
    (assert (Pm (read)))   
    )

(defrule TempAveria
    (Tm ?Tm)
    (test (< ?Tm 0))
 
 =>
 (printout t "Error: Introduce la temperatura del calentador: ") 
    (assert (Tm (read)))   
    )

(defrule pregunta-presion
    (declare(salience 100))
    (need-Pm ?)
 =>
 (printout t "Introduce la presión del calentador: ") 
    (assert (Pm (read)))   
    )

(defrule pregunta-temperatura 
    (need-Tm ?)
 =>
 (printout t "Introduce la temperatura del calentador: ") 
    (assert (Tm (read)))   
    )

(reset)
(run)
(facts)