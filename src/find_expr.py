from sympy import symbols, sympify
from sympy.core.function import _coeff_isneg as f
import re

def extract_symbols(expression):
    """
    Extract words that can be treated as variables for SymPy.
    """
    word_pattern = r'\b[a-zA-Z_]\w*\b'
    words = set(re.findall(word_pattern, expression))
    return {word: symbols(word) for word in words}

def analyze_expression(lhs):
    """
    Analyze the input expression and split it into positive and negative variables.
    """
    sympy_symbols = extract_symbols(lhs)
    expr = sympify(lhs, sympy_symbols)  # Convert string to SymPy expression
    positive_var = None
    negative_var = None

    # Split terms and classify them
    for term in expr.as_ordered_terms():
        if f(term):
            negative_var = str(-term)  # Convert negative term to positive
        else:
            positive_var = str(term)

    return negative_var, positive_var

if __name__ == "__main__":
    import sys
    if len(sys.argv) < 2:
        print("Usage: python3 analyze_expression.py '<expression>'")
        sys.exit(1)

    lhs = sys.argv[1]  # Input string passed as a command-line argument
    neg_var, pos_var = analyze_expression(lhs)

    # Output results in a simple format (space-separated)
    print(neg_var, pos_var)
