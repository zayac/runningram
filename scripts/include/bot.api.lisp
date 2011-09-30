;;; The file defines all functions to controlling
;;; any bot in RunningRram.
;;;
;;; author: necto

(defun make-lor (name)
  (case name ('left    -1)
			 ('straight 0)
			 ('right    1)))

(defun make-fob (name)
  (case name ('forward -1)
			 ('stop     0)
			 ('backward 1)))

(defun make-answer (lor fob)
  (list (make-lor lor)
		(make-fob fob)))

(defvar *current-car* nil)

(defun set-current-car (c)
  (setq *current-car* c))
