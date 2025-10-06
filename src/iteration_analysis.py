import sys
from sympy import Rational, nsimplify, root, solve, Eq, symbols, simplify, gcd, log
from functools import reduce



def get_relative_order(values):
    """Return the relative order of the elements in the list."""
    return [rank for _, rank in sorted((v, i) for i, v in enumerate(values))]


def is_rational_power(base, value):
    """
    Check if value can be expressed as base**k for some rational k.
    Returns k if rational, otherwise None.
    """
    x = symbols('x', real=True)
    try:
        # Solve base**x = value
        equation = Eq(base**x, value)
        result = solve(equation, x)
        if not result:
            return None

        # Check if any solution is rational
        for res in result:
            simplified_res = nsimplify(res, rational=True)
            if simplified_res.is_rational:
                return simplified_res
        return None
    except:
        return None
    

def iteration_analysis(mapping):
    """
    Perform analysis on the parameter mapping.
    - For 'div', check logs and roots.
    - For 'sub', compute the GCD and divide operands.
    - Ensure each list has unique max values and consistent order.
    :param mapping: Dictionary {param: (operation, [operands])}
    :return: Tuple (bool, set of integers)
    """
    result_bool = True  # Default result
    basis_map = {}  # Store basis for each parameter
    new_mapping = {}  # New mapping for log or division results
    relative_orders = []

    for param, (operation, operands) in mapping.items():
        if operation == "div":
            # Find the smallest operand
            min_op = min(operands)
            max_denominator = 1
            log_results = []

            # Compute log(min_op, operand) for all other operands
            for operand in operands:
                try:
                    # Compute log and simplify
                    result = is_rational_power(min_op, operand)
                    if result is None:
                        return False, 0, {}, {}

                    # Extract the denominator and track maximum denominator
                    max_denominator = max(max_denominator, result.q)
                    log_results.append(operand)
                except:
                    return False, 0, {}, {}

            # Find the root of min_op with the maximum denominator
            try:
                root_result = root(min_op, max_denominator)
                if not root_result.is_Integer:
                    return False, 0, {}, {}
            except:
                return False, 0, {}, {}

            # Basis for this parameter
            basis = int(root_result)
            basis_map[param] = basis

            # Store the log values with respect to the basis
            new_mapping[param] = [nsimplify(log(log_result, basis), rational=True) for log_result in log_results]

        elif operation == "sub":
            # Compute the GCD of all operands
            param_gcd = reduce(gcd, operands)
            if param_gcd <= 0:
                return False, 0, {}, {}

            # Divide each operand by the GCD
            new_mapping[param] = [operand // param_gcd for operand in operands]
            basis_map[param] = param_gcd

        else:
            # Unsupported operation
            return False, 0, {}, {}
        
        relative_orders.append(get_relative_order(new_mapping[param]))


    
    # Step 1: Check for unique maximum values
    max_values = set([max(values) for values in new_mapping.values()])
    if len(max_values) != 1:
        return False, 0, {}, {}
    stack_size = max_values.pop()
    

    # Step 2: Check for consistent order across all lists
    for i in range(1, len(relative_orders)):
        if relative_orders[i] != relative_orders[0]:
            return False, 0, {}, {}


    return result_bool, stack_size, basis_map, list(new_mapping.values())[0]

if __name__ == "__main__":
    # Input: space-separated entries like "param:operation,operand1,operand2 ..."
    input_str = sys.argv[1]
    mapping = {}

    for entry in input_str.split():
        param, rest = entry.split(":")
        operation, *operands = rest.split(",")
        operands = [int(op) for op in operands]
        mapping[param] = (operation, operands)

    result_bool, stack_size, basis_map, new_mapping = iteration_analysis(mapping)
    
    # Prepare the output
    basis_map_str = " ".join(f"{k}:{v}" for k, v in basis_map.items())
    new_mapping_str = " ".join(map(str, new_mapping))
    print(f"{result_bool} {stack_size} {basis_map_str} {new_mapping_str}")
