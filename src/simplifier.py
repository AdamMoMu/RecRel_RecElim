from sympy import simplify, symbols
import sys
import re

def extract_symbols(expression):
    """
    Extract words that can be treated as variables for SymPy.
    """
    word_pattern = r'\b[a-zA-Z_]\w*\b'
    words = set(re.findall(word_pattern, expression))
    return {word: symbols(word) for word in words}

def simplify_expression(expression):
    """
    Simplifies an algebraic expression using SymPy.
    """
    try:
        # Extract variables
        sympy_symbols = extract_symbols(expression)

        # Simplify only the algebraic expression
        expr = simplify(expression, locals=sympy_symbols)

        # Return the simplified expression as a string
        return str(expr)
    except Exception as e:
        return f"Error: {e}"

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 simplifier.py '<expression>'")
        sys.exit(1)
        
    input_expression = sys.argv[1]
    simplified = simplify_expression(input_expression)
    print(simplified)
