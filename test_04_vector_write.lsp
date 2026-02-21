"=== 4. Vector Modification (Dynamic Array) ==="

"> (= st [1 2])"
(= st [1 2])

"--- Stack: Push & Pop ---"
"> (push st 3)"
(push st 3)

"> (push st 4)"
(push st 4)

"> st"
st

"Pop returns:"
"> (pop st)"
(pop st)

"Stack after pop:"
"> st"
st

"--- Random Access Modification ---"
"> (= v [\"a\" \"b\" \"c\"])"
(= v ["a" "b" "c"])

"> (set v 1 \"changed\")"
(set v 1 "changed")

"> v"
v

"--- Insert & Delete ---"
"> (ins v 1 \"inserted\")"
(ins v 1 "inserted")

"> v"
v

"> (del v 0)"
(del v 0)

"> v"
v
