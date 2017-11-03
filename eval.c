/**
 * A function (eval) for evaluating arithmetic expressions, along with 
 * supporting functions and datatypes.
 * 
 * Use this in your UCP assignment *as is*. You *do not* need to modify this 
 * code.
 * 
 * Author: David Cooper
 * Updated: 27-09-2014
 */

#include "eval.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

#define OPERATOR_CHARS "-+*/^()"
#define VARIABLE_CHAR 'x'


/**
 * Used to distinguish between different parts (tokens) of an expression.
 * Individual symbolic operators each have their own token type, corresponding
 * to the ASCII value of the symbol.
 */
typedef enum {
    TK_INVALID, /* A character that can't be part of a valid token. */
    TK_END,     /* The end of the expression. */
    TK_NUMBER,  
    TK_WORD_OPERATOR,
    TK_VARIABLE,
    TK_ADD = '+',
    TK_SUB = '-',
    TK_MUL = '*',
    TK_DIV = '/',
    TK_POW = '^',
    TK_OPEN_BRACKET = '(',
    TK_CLOSE_BRACKET = ')'
} TokenType;

/**
 * Stores information on a single token, including its location, type and 
 * length. The text field is *not* null-terminated (because the text is 
 * embedded within the original expression string); hence the importance of
 * the length field.
 */
typedef struct {
    char* text;
    TokenType type;
    int length;
} Token;


static Token nextToken(char* expr);
static void consumeToken(char** expr, Token token);
static EvalError newError(EvalErrorType type, Token token);
static void checkDefined(double number, Token token, EvalError* error);

static EvalError addExpr(double* result, char** expr, double x, OperatorEvaluator opEval);
static EvalError mulExpr(double* result, char** expr, double x, OperatorEvaluator opEval);
static EvalError prefixExpr(double* result, char** expr, double x, OperatorEvaluator opEval);
static EvalError expExpr(double* result, char** expr, double x, OperatorEvaluator opEval);
static EvalError primary(double* result, char** expr, double x, OperatorEvaluator opEval);


/** 
 * Evaluates an arithmetic expression (expr), placing the result in *result. 
 * The expression may contain zero or more occurrances of the x variable, 
 * replaced with the value of x. It may contain the operators +, -, *, / and ^,
 * and brackets ( and ), all with their usual meanings. A range of additional 
 * unary operators, such as sin, cos, log, etc. can be provided via opEval, 
 * which points to a function used to evaluate such expressions.
 * 
 * The return value is a struct indicating the details of any error that may 
 * occur.
 */
EvalError eval(double* result, char* expr, double x, OperatorEvaluator opEval)
{
    EvalError error;
    Token token;
    
    error = addExpr(result, &expr, x, opEval);
    if(error.type == ER_NONE)
    {
        token = nextToken(expr);
        switch(token.type)
        {
            case TK_INVALID:
                error = newError(ER_INVALID_SYMBOL, token);
                break;
                
            case TK_END:
                /* Everything worked as expected. */
                break;
                
            default:
                error = newError(ER_UNEXPECTED_TOKEN, token);
        }
    }
    return error;
}

/**
 * Evaluates an additive expression; that is, one with zero or more binary 
 * additive operators (+ or -), and whose terms are multiplicative expressions.
 * 
 * More precisely:
 * add-expression := mul-expression (("+" | "-") mul-expression)*
 */ 
static EvalError addExpr(double* result, char** expr, double x, OperatorEvaluator opEval)
{
    EvalError error;
    Token token;
    double nextOperand;
    int endOfExpression = FALSE;
    
    error = mulExpr(result, expr, x, opEval);
    while(!endOfExpression && error.type == ER_NONE)
    {
        token = nextToken(*expr);
        switch(token.type)
        {
            case TK_INVALID:
                error = newError(ER_INVALID_SYMBOL, token);
                break;
               
            case TK_ADD: 
                consumeToken(expr, token);
                error = mulExpr(&nextOperand, expr, x, opEval);
                if(error.type == ER_NONE)
                {
                    *result += nextOperand;
                    checkDefined(*result, token, &error);
                }
                break;
                
            case TK_SUB:
                consumeToken(expr, token);
                error = mulExpr(&nextOperand, expr, x, opEval);
                if(error.type == ER_NONE)
                {
                    *result -= nextOperand;
                    checkDefined(*result, token, &error);
                }
                break;
                
            default: 
                endOfExpression = TRUE;
        }
    }
    return error;
}


/**
 * Evaluates a multiplicative expression; that is, one with zero or more binary
 * multiplicative operators (* or /), and whose terms are prefix expressions. 
 * Implicit multiplication (e.g. 2x) is also supported.
 * 
 * More precisely:
 * mul-expression := prefix-expression (["*" | "/"] prefix-expression)*
 */
static EvalError mulExpr(double* result, char** expr, double x, OperatorEvaluator opEval)
{
    EvalError error;
    Token token;
    double nextOperand;
    int endOfExpression = FALSE;
    
    error = prefixExpr(result, expr, x, opEval);
    while(!endOfExpression && error.type == ER_NONE)
    {
        token = nextToken(*expr);
        switch(token.type)
        {
            case TK_INVALID:
                error = newError(ER_INVALID_SYMBOL, token);
                break;
                
            case TK_MUL: 
                consumeToken(expr, token);
                error = prefixExpr(&nextOperand, expr, x, opEval);
                if(error.type == ER_NONE)
                {
                    *result *= nextOperand;
                    checkDefined(*result, token, &error);
                }
                break;
                
            case TK_DIV:
                consumeToken(expr, token);
                error = prefixExpr(&nextOperand, expr, x, opEval);
                if(error.type == ER_NONE)
                {
                    *result /= nextOperand;
                    checkDefined(*result, token, &error);
                }
                break;
                
            case TK_ADD: case TK_SUB: case TK_END: case TK_CLOSE_BRACKET:
                endOfExpression = TRUE;
                break;
                
            default: 
                /* Implicit multiplication (e.g. 2x) */
                error = prefixExpr(&nextOperand, expr, x, opEval);
                if(error.type == ER_NONE)
                {
                    *result *= nextOperand;
                    checkDefined(*result, token, &error);
                }
        }
    }
    return error;
}

/**
 * Evaluates a prefix expression; that is, one with zero or more unary prefix 
 * operators, followed by a single exponential expression. Unary prefix 
 * operators include "-" (negation) as well as anything evaluated by the given
 * OperatorEvaluator function.
 * 
 * More precisely:
 * prefix-expression := ("-" | word-operator)* exp-expression
 */
static EvalError prefixExpr(double* result, char** expr, double x, OperatorEvaluator opEval)
{
    EvalError error;
    Token token;
    char* operator;
    
    token = nextToken(*expr);
    switch(token.type)
    {
        case TK_INVALID:
            error = newError(ER_INVALID_SYMBOL, token);
            break;
            
        case TK_END:
            error = newError(ER_UNEXPECTED_END, token);
            break;
            
        case TK_WORD_OPERATOR:
            consumeToken(expr, token);
            error = prefixExpr(result, expr, x, opEval);
            
            if(error.type == ER_NONE)
            {
                /* Create a proper null-terminated string. */
                operator = (char*)malloc((token.length + 1) * sizeof(char));
                memcpy(operator, token.text, token.length);
                operator[token.length] = '\0';
                
                if(opEval == NULL)
                {
                    error = newError(ER_INVALID_OPERATOR, token);
                }
                else
                {
                    /* Evaluate operator and operand. */
                    error.type = (*opEval)(result, operator);
                    if(error.type != ER_NONE)
                    {
                        error.location = token.text;
                    }
                }
                
                free(operator);
            }
            break;
            
        case TK_SUB: /* Unary negation operator */
            consumeToken(expr, token);
            error = prefixExpr(result, expr, x, opEval);
                
            if(error.type == ER_NONE)
            {
                *result = -*result;
                checkDefined(*result, token, &error);
            }
            break;
            
        default:
            error = expExpr(result, expr, x, opEval);
    }
    return error;
}


/**
 * Evaluates an exponential expression; that is, one with zero or more binary 
 * exponential operators (^), where the first term is a primary, and the 
 * second, if present, is a prefix expression (which by definition contains 
 * another exponential expression).
 * 
 * More precisely:
 * exp-expression := primary ("^" prefix-expression)?
 * 
 * (Note: exponentials are evaluated right-to-left, not left-to-right like
 * other binary operators. 2^3^4 is equivalent to 2^(3^4).)
 */
static EvalError expExpr(double* result, char** expr, double x, OperatorEvaluator opEval)
{
    EvalError error;
    Token token;
    double exponent;
    
    error = primary(result, expr, x, opEval);
    
    if(error.type == ER_NONE)
    {
        token = nextToken(*expr);
        switch(token.type)
        {
            case TK_INVALID: 
                error = newError(ER_INVALID_SYMBOL, token);
                break;
                
            case TK_POW:
                /* Consume token, find next operand and evaluate exponential. */
                consumeToken(expr, token);
                error = prefixExpr(&exponent, expr, x, opEval);
                if(error.type == ER_NONE)
                {
                    *result = pow(*result, exponent);
                    checkDefined(*result, token, &error);
                }
                break;
                
            default: break;
                /* End of expression; Do nothing */
        }
    }
    
    return error;
}


/**
 * Evaluates a primary expression; i.e. one that consists of:
 * (a) a literal real number,
 * (b) the variable x, or 
 * (c) an additive expression in brackets (a sub-expression).
 * 
 * More precisely:
 * primary := real-number | "x" | ("(" add-expression ")")
 */ 
static EvalError primary(double* result, char** expr, double x, OperatorEvaluator opEval)
{
    EvalError error;
    Token token;
    
    token = nextToken(*expr);
    consumeToken(expr, token);
    switch(token.type)
    {
        case TK_INVALID:
            error = newError(ER_INVALID_SYMBOL, token);
            break;
            
        case TK_END:
            error = newError(ER_UNEXPECTED_END, token);
            break;
            
        case TK_OPEN_BRACKET:
            error = addExpr(result, expr, x, opEval);
            if(error.type == ER_NONE)
            {
                token = nextToken(*expr);
                if(token.type == TK_CLOSE_BRACKET)
                {
                    consumeToken(expr, token);
                }
                else
                {
                    error = newError(ER_UNEXPECTED_TOKEN, token);
                }
            }
            break;
            
        case TK_WORD_OPERATOR:
            error = newError(ER_UNEXPECTED_TOKEN, token);
            break;
            
        case TK_VARIABLE:
            *result = x;
            error.type = ER_NONE;
            checkDefined(*result, token, &error);
            break;
            
        case TK_NUMBER:
            sscanf(token.text, "%lf", result);
            error.type = ER_NONE;
            checkDefined(*result, token, &error);
            break;
            
        default:
            error = newError(ER_UNEXPECTED_TOKEN, token);
            
    }
    return error;
}


/**
 * Used to tokenise an expression string. This function takes a char* pointing 
 * to the current location within the string, and determines the next token.
 * 
 * The returned value is a Token struct containing a char* pointing to the 
 * token itself (preceding whitespace skipped, if any), the type of token and 
 * the length of the token.
 * 
 * No copying or modification of the original string is done. The returned
 * token text is physically part of the original string, and so individual 
 * tokens are *not* null-terminated (except for the last token).
 * 
 * The calling code is responsible for advancing the expression pointer, using 
 * the consumeToken() function. The nextToken() function simply returns the 
 * first token at the given location, and if called twice in a row will return
 * exactly the same token.
 */
static Token nextToken(char* expr)
{
    static Token token;
    static char* lastExpr = NULL;
    
    if(lastExpr != expr)
    {
        lastExpr = expr;
        token.text = expr;
        token.type = TK_INVALID;
        token.length = 0;
        
        /* Consume whitespace before token. */
        while(*token.text == ' ' || *token.text == '\t')
        {
            token.text++;
        }
        
        /* Check if the string has ended (by \0, \n or \r). */
        if(*token.text == '\0' || *token.text == '\n' || *token.text == '\r')
        {
            token.type = TK_END;
            token.length = 0;
        }
        /* Check if the next character is a symbolic operator or bracket. */
        else if(strchr(OPERATOR_CHARS, *token.text) != NULL) 
        {
            token.type = *token.text;
            token.length = 1;
        }
        else
        {
            /* Check if the next token is a number. */
            sscanf(token.text, "%*f%n", &token.length);
            if(token.length > 0)
            {
                token.type = TK_NUMBER;
            }
            else 
            {
                /* Check if the next token is a word. */
                sscanf(token.text, "%*[a-zA-Z]%n", &token.length);
                if(token.length == 1 && *token.text == VARIABLE_CHAR)
                {
                    token.type = TK_VARIABLE;
                }
                else if(token.length > 0)
                {
                    token.type = TK_WORD_OPERATOR;
                }
                /* Otherwise, the token is invalid. */
            }
        }
    }
    
    return token;
}

/**
 * Advances an expression pointer to the start of the next token.
 */
static void consumeToken(char** expr, Token token)
{
    *expr = token.text + token.length;
}

/**
 * Initialises a new EvalError struct, given an error type and a token.
 */
static EvalError newError(EvalErrorType type, Token token)
{
    EvalError error;
    error.type = type;
    error.location = token.text;
    return error;
}

/**
 * Checks whether a number is defined; i.e. not infinity, negative infinity, or
 * "nan". If undefined, we set the error condition to ER_RESULT_UNDEFINED.
 */
static void checkDefined(double number, Token token, EvalError* error)
{
    if((number > DBL_MAX)  || /* Positive infinity */
       (number < -DBL_MAX) || /* Negative infinity */
       (number != number))    /* Not-a-number (nan) */
    {
        error->type = ER_RESULT_UNDEFINED;
        error->location = token.text;
    }
}
