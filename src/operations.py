from sympy import sympify, Mul, Add, Pow, simplify, symbols, lcm, denom, expand, fraction
import sys


def extract_symbols(expression):
    """
    Extract words that can be treated as variables for SymPy.
    """
    import re
    word_pattern = r'\b[a-zA-Z_]\w*\b'
    words = set(re.findall(word_pattern, expression))
    return {word: symbols(word) for word in words}



def get_dominant_operation(expr):
    try:
        sympy_symbols = extract_symbols(expr)
        var = list(sympy_symbols.values())[0]
        parsed_expr = expand(simplify(expression, locals=sympy_symbols))
        
        dominant_op = "none"
        operands = []

        # Inspect terms for division first
        terms = parsed_expr.as_ordered_terms()
        for term in terms:
            if term.is_Mul:
                fract = fraction(term)
                if fract[1] != 1:
                    dominant_op = "div"
                    operands.append(fract[1])
                    
        if len(terms) >= 2 and len(operands) == len(terms):
            return dominant_op, operands

        # If no division is found, check for multiplication
        for term in terms:
            if term.is_Mul:
                if dominant_op == "none" or dominant_op == "mul":
                    dominant_op = "mul"
                    operands.append(term.coeff(var))

        # If no multiplication, check for addition/subtraction
        if parsed_expr.is_Add:
            if dominant_op != "none":
                dominant_op = "none"
            else:    
                dominant_op = "add"
                for term in terms:
                    if term.is_negative and not term.has(var):
                        if dominant_op == "add":
                            operands = []
                        
                        dominant_op = "sub"
                        operands.append(simplify(-1 * term))
                        
                    elif dominant_op == "add" and not term.has(var):
                        operands.append(simplify(term))
                    

        # Default to no operation
        if not dominant_op:
            dominant_op = "none"

        return dominant_op, operands
    except Exception as e:
        print(f"Error analyzing expression: {expr}, Error: {e}")
        return "error", []

if __name__ == "__main__":
    expression = sys.argv[1]
    op, operands = get_dominant_operation(expression)
    print(f"{op}:{','.join(map(str, operands))}")
