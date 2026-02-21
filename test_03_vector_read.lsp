"=== 3. Vector Read Operations ==="

"--- Creation & Length ---"
"> (= v [10 20 30 40 50])"
(= v [10 20 30 40 50])

"> v"
v

"> (len v)"
(len v)

"--- Indexing (idx) ---"
"> (idx v 0)"
(idx v 0)

"> (idx v 4)"
(idx v 4)

"Error Case (Out of Bound) -> NIL:"
"> (idx v 999)"
(idx v 999)

"--- Slicing (slice) ---"
"Slice [1:4]:"
"> (slice v 1 4)"
(slice v 1 4)

"--- Linear Search (find) ---"
"Find 30:"
"> (find v 30)"
(find v 30)

"Find 99 (Missing) -> NIL:"
"> (find v 99)"
(find v 99)
