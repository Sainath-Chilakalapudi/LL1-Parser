# LL(1) Parsing Table Generator in C

This repository contains a C program that allows users to:

- Input a regular grammar and number of productions.
- Automatically compute FIRST and FOLLOW sets for non-terminals.
- Construct the LL(1) parsing table.
- Parse input strings and simulate step-by-step LL(1) parsing actions.
- Determine whether the input string is accepted or rejected.

---

## 🧾 Features

- Input grammar using the form: `A=B`  
- Displays:
  - FIRST and FOLLOW sets
  - LL(1) parsing table
  - Parsing steps
- Accepts/rejects strings based on the parsing result
- Includes sample test cases and output

---

## ⚙️ Usage Instructions

1. Enter the number of productions.
2. Input each production in the form `A=B`, where:
   - `A` is a single uppercase non-terminal.
   - `B` is a sequence of terminals and non-terminals.
3. Input string must end with `$`.

---

## 📌 Assumptions

1. Epsilon (ε) is represented by `#`.
2. Productions must follow the format `A=B`, one per line.
3. The LHS of the **first** production is treated as the start symbol.
4. Grammar must **not** be left-recursive.
5. Only **uppercase letters** are considered non-terminals.
6. All other characters are considered terminals.
7. **`!`** and **`$`** are reserved for internal use.
8. Input strings must **end with `$`**.

---

## 🔍 Sample Test Case

**Input:**

```
How many productions ? :8

Enter 8 productions in form A=B where A and B are grammar symbols :

E=TR

R=+TR

R=#

T=FY

Y=*FY

Y=#

F=(E)

F=i

----------------------------------------------
 First(E)= { (, i, }

 First(R)= { +, #, }

 First(T)= { (, i, }

 First(Y)= { *, #, }

 First(F)= { (, i, }

-----------------------------------------------

 Follow(E) = { $, ),  }

 Follow(R) = { $, ),  }

 Follow(T) = { +, $, ),  }

 Follow(Y) = { +, $, ),  }

 Follow(F) = { *, +, $, ),  }


                              The LL(1) Parsing Table for the above grammer :-
                             ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

      ======================================================================================================
              |       +               *               (               )               i               $
      ======================================================================================================
         E    |                                       E=TR                            E=TR
      ------------------------------------------------------------------------------------------------------
         R    |       R=+TR                                           R=#                             R=#
      ------------------------------------------------------------------------------------------------------
         T    |                                       T=FY                            T=FY
      ------------------------------------------------------------------------------------------------------
         Y    |       Y=#             Y=*FY                           Y=#                             Y=#
      ------------------------------------------------------------------------------------------------------
         F    |                                       F=(E)                           F=i
      ------------------------------------------------------------------------------------------------------


Please enter the desired INPUT STRING = i+i*i$

                    ===========================================================================
                            Stack                   Input                   Action
                    ===========================================================================
                            $E                      i+i*i$                  E=TR
                            $RT                     i+i*i$                  T=FY
                            $RYF                    i+i*i$                  F=i
                            $RYi                    i+i*i$                  POP ACTION
                            $RY                     +i*i$                   Y=#
                            $R                      +i*i$                   R=+TR
                            $RT+                    +i*i$                   POP ACTION
                            $RT                     i*i$                    T=FY
                            $RYF                    i*i$                    F=i
                            $RYi                    i*i$                    POP ACTION
                            $RY                     *i$                     Y=*FY
                            $RYF*                   *i$                     POP ACTION
                            $RYF                    i$                      F=i
                            $RYi                    i$                      POP ACTION
                            $RY                     $                       Y=#
                            $R                      $                       R=#
                            $                       $                       POP ACTION

            =======================================================================================
                                            YOUR STRING HAS BEEN ACCEPTED !!
            =======================================================================================
```