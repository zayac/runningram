;;; Simple example of bot intellegence
;;; this file should be mentioned in ../settings.cfg
;;; to be used in deciding of bot behavour

(load "scripts/include/bot.api.lisp")

;; the main function which decides what bot should do
(defun just-decide ()
  (if (> (x (vel *current-car*)) 0)
  	(make-answer 'left 'backward)
	(make-answer 'right 'backward)))

