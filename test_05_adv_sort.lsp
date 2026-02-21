"=== 5. Advanced Sort & Vector Math ==="

"--- Vector Arithmetic ---"
"> (= v1 [1 2])"
(= v1 [1 2])

"> (= v2 [3 4])"
(= v2 [3 4])

"1. Vector + Vector (Concat):"
"> (+ v1 v2)"
(+ v1 v2)

"2. Vector * Int (Repeat):"
"> (* v1 3)"
(* v1 3)

"--- Sorting (Basic) ---"
"> (= nums [5 2 8 1 9])"
(= nums [5 2 8 1 9])

"> (sort nums asc)"
(sort nums asc)

"> (sort nums desc)"
(sort nums desc)

"--- Deep Recursive Sort (Nested Vectors) ---"
"Before: [[3 1] [1 2] [1] [1 2 3]]"
"> (= nested [[3 1] [1 2] [1] [1 2 3]])"
(= nested [[3 1] [1 2] [1] [1 2 3]])

"After Sort (Python-style comparison):"
"Expected Order: [1] < [1 2] < [1 2 3] < [3 1]"
"> (sort nested)"
(sort nested)
