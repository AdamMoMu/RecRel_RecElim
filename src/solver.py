import sympy as sp
import sys

# Simplifies expressions (mathematically)
def simplify_expressions(expressions):
    simplified = []
    for expr in expressions:
        try:
            sympy_expr = sp.sympify(expr)
            simplified_expr = sp.simplify(sympy_expr)
            simplified.append(str(simplified_expr))
        except Exception as e:
            simplified.append(f"Error simplifying: {e}")
    return simplified

if __name__ == "__main__":
    # Read input expressions from command-line arguments
    expressions = sys.argv[1:]  # Expressions passed as arguments
    simplified = simplify_expressions(expressions)
    for result in simplified:
        print(result)
