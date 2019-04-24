;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;x
;;; Domain Football world
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (domain nav)
  (:requirements :strips :typing :equality)
  (:types row col agent ball goalPost)
  (:predicates (connected ?Rx - row ?Cx - col ?Ry - row ?Cy - col)
		(at ?a - agent ?R - row ?C - col)
		(ver)
		(hoz)
		)

    
    (:action move-vertical
        :parameters (?Rx - row ?Ry - row ?C - col ?a - agent)
        :precondition (and (at ?a ?Rx ?C) (connected ?Rx ?C ?Ry ?C) (ver))
        :effect (and (at ?a ?Ry ?C) (hoz) (not (at ?a ?Rx ?C)) (not (ver))))
    (:action move-horizontal
        :parameters (?R - row ?Cx - col ?Cy - col ?a - agent)
        :precondition (and (at ?a ?R ?Cx) (connected ?R ?Cx ?R ?Cy) (hoz))
        :effect (and (at ?a ?R ?Cy) (ver) (not (at ?a ?R ?Cx)) (not (hoz))))
 
)
