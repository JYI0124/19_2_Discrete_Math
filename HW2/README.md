# DiscreteMathematics_HW2 

# 5. DNF_Converter
 DNF converter is a program that transforms a propositional into a disjunctive normal form, called DNF. DNF is a two-level propositional formulas which has form of product of sum. Our program will consume the propositional formula and produce the result through several procedures.
###### Input : The input should be propositional formula that is prefixed in the parentheses.
* The number of left brackets and right brackets must be the same.
* Input must be the complete form of propositional formula.
###### Input example
    (or a1 (not (or (not (or a2 a3)) a4)))


###### Output : At output expression, the numbers in the same row are all connected by conjunction, and each row is connected by conjunction. And there is a '0' for the delimeter between 1st output and 2nd output. 2nd output will be the list of truth value of our result. And the program must print out an error message if the given input does not follow the rule.

###### Output Example
    1
    2 -4
    3 -4
    0
    1 2 -4 3   


###### How to Execute

    $ make

    $ ./dnf
