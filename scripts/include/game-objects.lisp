;; The definitions of game ogjects, which can be translated between script and main program

(defclass vec ()
  ((x :initarg :x
	  :accessor x)
   (y :initarg :y
	  :accessor y)))


(defmethod print-object ((v vec) stream)
  (princ (list 'vec (x v) (y v)) stream))


(defclass car-properties ()
  ((pos :initarg :pos
		  :accessor pos)
   (vel :initarg :vel
		 :accessor vel)))

(defmethod print-object ((c car-properties) stream)
  (with-slots (front back) c
	(princ (list 'car 'front front 'back back) stream)))
