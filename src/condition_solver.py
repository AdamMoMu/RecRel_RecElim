from sympy import symbols, simplify, parse_expr, expand, Rel
import re
import sys

def simplify_condition(expression: str):
    """
    Moves variables to the left-hand side and constants to the right-hand side of an inequality or equation.

    :param expression: A string representation of the condition (e.g., "n - 2 <= 0").
    :return: Simplified condition as a string (e.g., "n <= 2").
    """
    try:
        # Step 1: Detect all words (sequences of letters) as variables
        expression = expression.replace(" ", "")
        word_pattern = r'\b[a-zA-Z_]\w*(?:\.\w+)*(?:\(\))?'   # Match words (identifiers) like left, right, n, etc.
        words = set(re.findall(word_pattern, expression.replace(" ", "")))

        # Step 2: Declare all detected words as SymPy symbols
        sympy_symbols = {word: symbols(word) for word in words}

        # Step 3: Parse the input expression using the declared symbols
        expr = parse_expr(expression, local_dict=sympy_symbols, evaluate=False)

        # Step 4: Handle relational expressions
        if expr.is_Relational:
            lhs = expr.lhs
            rhs = expr.rhs

            # Move everything to the left side
            new_lhs = expand(lhs - rhs)

            # Split into terms: variables on the left, constants on the right
            terms = new_lhs.as_ordered_terms()
            variables = []
            constants = []

            for term in terms:
                if term.free_symbols:  # If the term contains variables
                    variables.append(term)
                else:  # Constant terms
                    constants.append(term)

            # Combine variables and constants
            if expr.rel_op == ">=" or expr.rel_op == ">": 
                lhs_final = simplify(-1 * sum(variables))
                rhs_final = simplify(sum(constants))
            
            else:
                lhs_final = simplify(sum(variables))
                rhs_final = simplify(-1 * sum(constants))
            
                

            # Rebuild the relation
            if expr.rel_op == "<=":
                return f"{lhs_final} <= {rhs_final}"
            elif expr.rel_op == ">=":
                return f"{lhs_final} <= {rhs_final}"
            elif expr.rel_op == "<":
                return f"{lhs_final} < {rhs_final}"
            elif expr.rel_op == ">":
                return f"{lhs_final} < {rhs_final}"
            elif expr.rel_op == "==":
                return f"{lhs_final} == {rhs_final}"

        # Step 5: Simplify non-relational expressions (fallback)
        return str(simplify(expr))

    except Exception as e:
        return f"Error: {e}"


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 condition_solver.py '<condition>'")
        sys.exit(1)
    
    cond = sys.argv[1]
    
    print(simplify_condition(cond))
