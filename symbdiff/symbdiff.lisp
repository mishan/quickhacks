;; Mikhail Nasledov
;; CS198-S07
;; Symbolic Differentiation Program written in Common Lisp

(defun dplusminuslist (list x op)
  (if (null list) 0
  (cons op (list (derive (first list) x)
           (dplusminuslist (rest list) x op)))))

(defun derive (f x)
  (if (null f) nil
  (cond
   ((listp f)
    (cond
     ((eq (first f) '+)
    (dplusminuslist (rest f) x '+))
     ((eq (first f) '-)
    (if (>= (length f) 3)
      (cons '- (dplusminuslist (rest f) x '-))
      (cons '- (list (derive (second f) x)))))
     ((eq (first f) '*)
    (cons '+ (list (cons '* (list (second f) (derive (third f) x)))
             (cons '* (list (third f)  (derive (second f) x))))))
     ((eq (first f) '/)
    (cons '/ (cons '- (list
               (cons '* (list (third f) (derive (second f) x)))
               (cons '* (list (second f) (derive (third f) x)))))))
     ((eq (first f) 'expt)
    (cons '* (list (third f) (list 'expt (second f) (- (third f) 1)))))
     ((eq (first f) 'sqrt)
    (cons '* (list (list '/ 1 2) (derive (second f) x)
             (list 'sqrt (second f)))))
     ;; hopefully we've covered all cases here -- go further in if it's just
     ;; a paren
     (t (derive (first f) x)))
    )
   ((eq f x) 1)
   (t 0))
  ))
