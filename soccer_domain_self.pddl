;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;x
;;; Domain Football world
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (domain nav)
  (:requirements :strips :typing :equality)
  (:types row col agent ball goalPost)
  (:predicates (connected ?Rx - row ?Cx - col ?Ry - row ?Cy - col)
		(at ?a - agent ?R - row ?C - col)
		)

    
    (:action move-vertical
        :parameters (?Rx - row ?Ry - row ?C - col ?a - agent)
        :precondition (and (at ?a ?Rx ?C) (connected ?Rx ?C ?Ry ?C))
        :effect (and (at ?a ?Ry ?C) (not (at ?a ?Rx ?C)) ))
    (:action move-horizontal
        :parameters (?R - row ?Cx - col ?Cy - col ?a - agent)
        :precondition (and (at ?a ?R ?Cx) (connected ?R ?Cx ?R ?Cy))
        :effect (and (at ?a ?R ?Cy) (not (at ?a ?R ?Cx)) ))
(:action stay
        :parameters (?Rx - row ?Cx - col ?a - agent)
        :precondition (and (at ?a ?R ?Cx))
        :effect (and (at ?a ?Rx ?Cx) ))
 
)
