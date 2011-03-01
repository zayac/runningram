;;; The file defines all functions to controlling
;;; any bot in RunningRram.
;;;
;;; author: necto

(defun make-lor (name)
  (cond ((eq name 'left)     -1)
	((eq name 'straight)  0)
	((eq name 'right)     1)))

(defun make-fob (name)
  (cond ((eq name 'forward) -1)
	((eq name 'stop)     0)
	((eq name 'backward) 1)))

(defun make-answer (lor fob)
  (list (make-lor lor)
	(make-fob fob)))
