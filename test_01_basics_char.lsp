"=== 1. Variables & Character Arithmetic ==="

"--- Variables ---"
"> (= a 10)"
(= a 10)

"> (= b 20)"
(= b 20)

"> a"
a

"> b"
b

"> (+ a b)"
(+ a b)

"> (* a b)"
(* a b)

"--- Character Math (ASCII) ---"
"Note: 'a' is 97, 'b' is 98"

"> (= ch 'a')"
(= ch 'a')

"1. Char + Int ('a' + 3 -> 100)"
"> (+ ch 3)"
(+ ch 3)

"2. Char + Char ('a' + 'b' -> 195)"
"> (+ 'a' 'b')"
(+ 'a' 'b')

"3. Char * Int ('a' * 2 -> 194)"
"> (* 'a' 2)"
(* 'a' 2)

"--- Cons Cells (LISP List) ---"
"> (= list (cons 1 (cons 2 (cons 3 ()))))"
(= list (cons 1 (cons 2 (cons 3 ()))))

"> (car list)"
(car list)

"> (cdr list)"
(cdr list)

"> (car (cdr list))"
(car (cdr list))
