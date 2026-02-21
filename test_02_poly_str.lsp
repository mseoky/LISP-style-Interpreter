"=== 2. Polymorphic String Operations ==="

"--- String Concatenation ---"
"> (= s1 \"Hello\")"
(= s1 "Hello")

"> (= s2 \" World\")"
(= s2 " World")

"> (+ s1 s2)"
(+ s1 s2)

"--- Mixed Type Concatenation ---"
"1. String + Integer"
"> (+ \"Value: \" 100)"
(+ "Value: " 100)

"2. String + Character (Append Char)"
"> (+ \"Grade: \" 'A')"
(+ "Grade: " 'A')

"3. Complex Mix"
"> (+ \"Result: \" 10 \" points (\" 'P' \")\" )"
(+ "Result: " 10 " points (" 'P' ")" )

"--- String Repetition ---"
"> (* \"Go! \" 3)"
(* "Go! " 3)
