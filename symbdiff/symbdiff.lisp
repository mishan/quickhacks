;; CS198-S07
;; Symbolic Differentiation Program written in Common Lisp
;; Copyright (C) 2012-2013 Misha Nasledov <misha@nasledov.com>
;;
;; This program is free software; you can redistribute it and/or
;; modify it under the terms of the GNU General Public License
;; as published by the Free Software Foundation; either version 2
;; of the License, or (at your option) any later version.
;;
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this program; if not, write to the Free Software
;; Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

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
