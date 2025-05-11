# LL(1) Parsing Table Generator in C

This repository contains a C program that allows users to:

- Read a user-defined grammar from a file.
- Automatically compute FIRST and FOLLOW sets for non-terminals.
- Construct the LL(1) parsing table.
- Parse input strings and simulate step-by-step LL(1) parsing actions.
- Determine whether the input string is accepted or rejected.

*Note:* The grammar must be in LL(1) format—i.e., free from left recursion and ambiguity.

---
## **Assumptions**

* Grammar must be LL(1) (no left recursion or conflicts).
* Grammar must be in the format:

  ```
  A->b|c|Db
  B->d|#
  ```
* Use `#` to denote epsilon (ε).
* Use `->` to separate LHS and RHS.
* Multiple productions must be combined using \`|` on the same line.
* Each production line must end with a newline.
* File must end with a **blank line** (an empty newline).

**Important Notes:**

* **Do not use `-` or `>`** in terminals.
* Non-terminals must be single uppercase letters (A-Z).
* Terminals must be single characters and not uppercase letters.
* Input strings must contain **only terminals** followed by `$`.

---
## **Test Case**

### Sample `grammer.txt`:

```
E->TA
A->+TA|#
T->FB
B->*FB|#
F->(E)|i
```

> Note: Make sure to add a **blank line** at the end of the file.

### Input:

```
i+i*i$
```

### Output:

```
E->TA
A->+TA|#
T->FB
B->*FB|#
F->(E)|i

FIRST OF E: ( i
FIRST OF A: # +
FIRST OF T: ( i
FIRST OF B: # *
FIRST OF F: ( i

FOLLOW OF E: $ )
FOLLOW OF A: $ )
FOLLOW OF T: $ ) +
FOLLOW OF B: $ ) +
FOLLOW OF F: $ ) * +

FIRST OF E->TA: ( i
FIRST OF A->+TA: +
FIRST OF A->#: #
FIRST OF T->FB: ( i
FIRST OF B->*FB: *
FIRST OF B->#: #
FIRST OF F->(E): (
FIRST OF F->i: i


        **************** LL(1) PARSING TABLE *******************
        --------------------------------------------------------
          $         (         )         *         +         i
E                   E->TA                                   E->TA
A         A->#                A->#                A->+TA
T                   T->FB                                   T->FB
B         B->#                B->#      B->*FB    B->#
F                   F->(E)                                  F->i


Please enter the desired INPUT STRING = i+i*i$

                ===================================================================
                        Stack                   Input                   Action
                ===================================================================
                        $E                      i+i*i$                  E->TA
                        $AT                     i+i*i$                  T->FB
                        $ABF                    i+i*i$                  F->i
                        $ABi                    i+i*i$                  POP
                        $AB                     +i*i$                   Epslon production is added
                        $A                      +i*i$                   A->+TA
                        $AT+                    +i*i$                   POP
                        $AT                     i*i$                    T->FB
                        $ABF                    i*i$                    F->i
                        $ABi                    i*i$                    POP
                        $AB                     *i$                     B->*FB
                        $ABF*                   *i$                     POP
                        $ABF                    i$                      F->i
                        $ABi                    i$                      POP
                        $AB                     $                       Epslon production is added
                        $A                      $                       Epslon production is added
                        $                       $                       POP

                        ================================================
                                 YOUR STRING HAS BEEN ACCEPTED !
                        ================================================


```

---

## **How to Run**

1. **Write grammar** in `grammer.txt` (example provided above).
2. Ensure there's a **blank line** at the end of the file.
3. Run:

   ```
   make
   ```
4. The program will:

   * Compile and run
   * Display FIRST, FOLLOW, Parsing Table
   * Ask for input string (like `i+i*i$`)
5. It will show step-by-step parsing and whether the string is accepted or rejected.

