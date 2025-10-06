from sympy import simplify, symbols, lcm, denom, expand, fraction
import sys
import json

def extract_symbols(expression):
    """
    Extract words that can be treated as variables for SymPy.
    """
    import re
    word_pattern = r'\b[a-zA-Z_]\w*\b'
    words = set(re.findall(word_pattern, expression))
    return {word: symbols(word) for word in words}


def find_lcm_of_denominators(expr, symbols_map):
    """
    Find the LCM of denominators for all terms in the expression.
    """
    expr = expand(expr)
    terms = expr.as_ordered_terms()
    denominators = []

    for term in terms:
        _, denom_expr = fraction(term)  # Extract the denominator of the term
        if denom_expr != 1:  # Only include non-trivial denominators
            denominators.append(denom_expr)

    if denominators:
        return lcm(denominators)
    return 1


def canonicalize_expression(expression, symbols_dict):
    """
    Rewrites an expression into a canonical form.
    """
    return expand(simplify(expression, locals=symbols_dict))


def simplify_and_replace(expression, replacements):
    """
    Simplifies an algebraic expression, removes fractions, and replaces terms with their replacements.
    """
    try:
        # Extract symbols from the expression
        sympy_symbols = extract_symbols(expression)
        expr = canonicalize_expression(expression, sympy_symbols)

        # Step 1: Find the LCM of denominators
        expr = expand(expr)
        lcm_value = find_lcm_of_denominators(expr, sympy_symbols)

        # Step 2: Multiply the expression by the LCM to clear denominators
        cleared_expr = simplify(lcm_value * expr)

        # Step 4: Canonicalize and perform replacements
        replacements_dict = {canonicalize_expression(k, sympy_symbols): symbols(v) for k, v in replacements.items()}
        for target, replacement in replacements_dict.items():
            cleared_expr = cleared_expr.subs(target, replacement)

        # Step 4: Divide back by the LCM
        final_expr = simplify(cleared_expr / lcm_value)

        # Return the simplified and replaced expression
        return str(final_expr)
    except Exception as e:
        return f"Error: {e}"

if __name__ == "__main__":
    # Read input arguments
    input_expression = sys.argv[1]
    replacements = json.loads(sys.argv[2])

    # Simplify and replace
    result = simplify_and_replace(input_expression, replacements)
    print(result)




