/**
 * A function (eval) and associated datatypes for evaluating arithmetic 
 * expressions.
 * 
 * Use this in your UCP assignment *as is*. You *do not* need to modify this 
 * code.
 * 
 * Author: David Cooper
 * (Modified to use const parameters whenever possible.)
 */


/** The conventional definitions of TRUE and FALSE. */
#define FALSE 0
#define TRUE !FALSE 

/**
 * Indicates one of several error types (or ER_NONE for no error) that may 
 * arise when evaluating an expression.
 */
typedef enum {
    ER_NONE, 
    ER_RESULT_UNDEFINED,
    ER_INVALID_SYMBOL, 
    ER_UNEXPECTED_TOKEN, 
    ER_UNEXPECTED_END, 
    ER_INVALID_OPERATOR 
} EvalErrorType;

/**
 * Stores details on an expression evaluation error. If type is not ER_NONE,
 * then location points to the character within the expression that triggered
 * the error.
 */
typedef struct {
    EvalErrorType type;
    char* location;
} EvalError;   

/**
 * An OperatorEvaluator is a function that evaluates unary operators, such as 
 * sin, cos, log. Any such function should use operatorText to determine which
 * operator to apply, and then (if valid) apply it to location that result 
 * points to, overwriting the original value with the new one.
 * 
 * The function should return ER_NONE, ER_RESULT_UNDEFINED or 
 * ER_INVALID_OPERATOR as appropriate.
 */
typedef EvalErrorType (*OperatorEvaluator)(double* result, char* operatorText);

/** 
 * Evaluates an arithmetic expression (expr), placing the result in *result. 
 * The expression may contain zero or more occurrances of the x variable, 
 * replaced with the value of x. It may contain the operators +, -, *, / and ^,
 * and brackets ( and ), all with their usual meanings. A range of additional 
 * unary operators, such as sin, cos, log, etc. can be provided via an 
 * OperatorEvaluator function.
 * 
 * The return value is a struct indicating the details of any error that may 
 * occur.
 */
EvalError eval(double* result, char* expr, double x, OperatorEvaluator opEval);
