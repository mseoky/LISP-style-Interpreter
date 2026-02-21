#!/bin/bash

echo "=========================================="
echo "    LISP Interpreter Final Demo"
echo "=========================================="
echo ""

echo ">>> Test 1: Basics & Character Math"
./main.exe < test_01_basics_char.lsp
echo ""

echo ">>> Test 2: Polymorphic Strings"
./main.exe < test_02_poly_str.lsp
echo ""

echo ">>> Test 3: Vector Read (Slice/Find)"
./main.exe < test_03_vector_read.lsp
echo ""

echo ">>> Test 4: Vector Write (Stack/Insert)"
./main.exe < test_04_vector_write.lsp
echo ""

echo ">>> Test 5: Advanced Sort & Vector Math"
./main.exe < test_05_adv_sort.lsp
echo ""

echo "=========================================="
echo "          Demo Completed"
echo "=========================================="
