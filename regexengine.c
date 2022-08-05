#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define TRUE 1
#define FALSE 0
#define NONE -1
#define INFINITY INT_MAX 

typedef int bool;
typedef char* string;

typedef struct {
    string head;
    string operator;
    string rest;
} split_expr_output;

typedef struct {
    bool matched;
    int chr_count;
    string next_cmp_str;
} does_unit_match_output;

typedef struct {
    bool matched;
    int chr_count;
    string next_cmp_str;
} match_multiple_output;

typedef struct {
    bool matched;
    int match_len;
} match_expr_output;

typedef struct {
    bool matched;
    int match_pos;
    int match_len;
} match_output;

bool is_escape_sequence(string expr);
bool is_escape(char chr);
bool is_start(char chr);
bool is_end(char chr);
bool is_dot(char chr);
bool is_star(char chr);
bool is_plus(char chr);
bool is_question_mark(char chr);
bool is_operator(string expr);
bool is_open_set(char chr);
bool is_close_set(char chr);
bool is_set(string expr);
string split_set(string expr);
bool is_open_alternative(char chr);
bool is_close_alternative(char chr);
bool is_alternative(string expr);
string split_alternative(string expr);
bool is_literal(char chr);
bool is_unit(string expr);
split_expr_output split_expr(string expr);
does_unit_match_output does_unit_match(string expr, string str);
match_multiple_output match_multiple(string expr, string str, int min, int max);
match_expr_output match_expr(string expr, string str, int match_len);
match_output match(string expr, string str);

bool is_escape_sequence(string expr) 
{
    if(is_escape(expr[0]) == TRUE)
        return TRUE;
    else
        return FALSE;
}

bool is_escape(char chr)
{
    if(chr == '\\')
        return TRUE;
    else
        return FALSE;
}

bool is_start(char chr)
{
    if(chr == '^')
        return TRUE;
    else
        return FALSE;
}

bool is_end(char chr)
{
    if(chr == '$')
        return TRUE;
    else
        return FALSE;
}

bool is_dot(char chr)
{
    if(chr == '.')
        return TRUE;
    else
        return FALSE;
}

bool is_star(char chr)
{
    if(chr == '*')
        return TRUE;
    else
        return FALSE;
}

bool is_plus(char chr)
{
    if(chr == '+')
        return TRUE;
    else
        return FALSE;
}

bool is_question_mark(char chr)
{
    if(chr == '?')
        return TRUE;
    else
        return FALSE;
}

bool is_operator(string expr)
{
    if(is_star(expr[0]) == TRUE || is_plus(expr[0]) == TRUE || is_question_mark(expr[0]) == TRUE)
        return TRUE;
    else
        return FALSE;
}

bool is_open_set(char chr)
{
    if(chr == '[')
        return TRUE;
    else
        return FALSE;
}

bool is_close_set(char chr)
{
    if(chr == ']')
        return TRUE;
    else
        return FALSE;
}

bool is_set(string expr)
{
    int expr_len = strlen(expr);

    if(is_open_set(expr[0]) == TRUE && is_close_set(expr[expr_len - 1]) == TRUE)
        return TRUE;
    else
        return FALSE;
}

string split_set(string expr)
{
    int expr_len = strlen(expr); 

    string split_set_out = strndup(expr + 1, expr_len - 2);
    
    return split_set_out;
}

bool is_open_alternative(char chr)
{
    if(chr == '(')
        return TRUE;
    else
        return FALSE;
}

bool is_close_alternative(char chr)
{
    if(chr == ')')
        return TRUE;
    else
        return FALSE;
}

bool is_alternative(string expr)
{
    int expr_len = strlen(expr);

    if(is_open_alternative(expr[0]) == TRUE && is_close_alternative(expr[expr_len - 1]) == TRUE)
        return TRUE;
    else
        return FALSE;
}

string split_alternative(string expr)
{
    int expr_len = strlen(expr); 

    string split_set_out = strndup(expr + 1, expr_len - 2);
    
    return split_set_out;
}

bool is_literal(char chr)
{
    if(isalnum(chr) != 0 || chr == ' ' || chr == '/' || chr == ':')
        return TRUE;
    else
        return FALSE;
}

bool is_unit(string expr)
{
    if(is_literal(expr[0]) == TRUE || is_set(expr) == TRUE || 
        is_dot(expr[0]) == TRUE || is_alternative(expr) == TRUE || is_escape_sequence(expr) == TRUE)
        return TRUE;
    else
        return FALSE;
}

split_expr_output split_expr(string expr)
{
    split_expr_output split_expr_out = {0};

    if(is_open_set(expr[0]) == TRUE)
    {
        string close_set_str = strstr(expr, "]"); // "]..."

        split_expr_out.head = strndup(expr, close_set_str - expr + 1);
        split_expr_out.rest = close_set_str + 1;
    }
    if(is_open_alternative(expr[0]) == TRUE)
    {
        string close_alternative_str = strstr(expr, ")"); // "]..."

        split_expr_out.head = strndup(expr, close_alternative_str - expr + 1);
        split_expr_out.rest = close_alternative_str + 1;
    }
    else if(is_escape(expr[0]) == TRUE)
    {
        split_expr_out.head = strndup(expr, 2);
        split_expr_out.rest = expr + 2;
    }
    else if(is_literal(expr[0]) == TRUE)
    {
        split_expr_out.head = strndup(expr, 1);
        split_expr_out.rest = expr + 1;
    }
    else if(is_dot(expr[0]) == TRUE)
    {
        split_expr_out.head = strndup(expr, 1);
        split_expr_out.rest = expr + 1;
    }
    else{}

    if(is_operator(split_expr_out.rest))
    {
        split_expr_out.operator = split_expr_out.rest;
        split_expr_out.rest += 1;
    } 
    else
    {
        split_expr_out.operator = "";
    }

    return split_expr_out;
}

does_unit_match_output does_unit_match(string expr, string str)
{
    does_unit_match_output does_unit_match_out = {0};

    split_expr_output split_expr_out = split_expr(expr);
    string head = split_expr_out.head;
    string rest = split_expr_out.rest;

    if(is_set(head) == TRUE)
    {
        string inside_set = split_set(head);
        int inside_set_len = strlen(inside_set);

        for(int i = 0; i < inside_set_len; i++)
            if(inside_set[i] == str[0])
            {
                does_unit_match_out.matched = TRUE;
                does_unit_match_out.next_cmp_str = str + 1;
                does_unit_match_out.chr_count = 1;

                return does_unit_match_out;
            }
    }
    else if(is_alternative(head) == TRUE)
    {
        string inside_alternative = split_alternative(head);
        string verticalbar = "|";
        string token;

        token = strtok(inside_alternative, verticalbar);

        while(token != NULL)
        {
            match_expr_output ret = match_expr(token, str, 0); //we shouldve used match_expr apparently lmfao

            if(ret.matched == TRUE)
            {
                does_unit_match_out.matched = TRUE;
                does_unit_match_out.next_cmp_str = str + ret.match_len;
                does_unit_match_out.chr_count = ret.match_len;

                return does_unit_match_out;
            }

            token = strtok(NULL, verticalbar);
        }
    }
    else if(is_escape_sequence(head) == TRUE)
    {
        if(head[1] == 'a')
        {
            if(isalpha(str[0]))
            {
                does_unit_match_out.matched = TRUE;
                does_unit_match_out.next_cmp_str = str + 1;
                does_unit_match_out.chr_count = 1;

                return does_unit_match_out;
            }
        }
        else if(head[1] == 'd')
        {
            if(isdigit(str[0]))
            {
                does_unit_match_out.matched = TRUE;
                does_unit_match_out.next_cmp_str = str + 1;
                does_unit_match_out.chr_count = 1;

                return does_unit_match_out;
            }
        }
        else {}
    }
    else if(is_literal(head[0]) == TRUE)
    {
        if(head[0] == str[0])
        {
            does_unit_match_out.matched = TRUE;
            does_unit_match_out.next_cmp_str = str + 1;
            does_unit_match_out.chr_count = 1;

            return does_unit_match_out;
        }
        else{}
    }
    else if(is_dot(head[0]) == TRUE)
    {
        does_unit_match_out.matched = TRUE;
        does_unit_match_out.next_cmp_str = str + 1;
        does_unit_match_out.chr_count = 1;

        return does_unit_match_out;
    }
    else{}

    does_unit_match_out.matched = FALSE;
    does_unit_match_out.next_cmp_str = NULL;
    does_unit_match_out.chr_count = 0;

    return does_unit_match_out;
}

match_multiple_output match_multiple(string expr, string str, int min, int max)
{
    match_multiple_output match_multiple_out = {0};

    int count = 0;
    int chr_count = 0;

    string next_cmp_str = str;

    while(next_cmp_str[0] != '\0' && count < max)
    {
        does_unit_match_output does_unit_match_out = does_unit_match(expr, next_cmp_str);
        if(does_unit_match_out.matched == TRUE)
        {
            chr_count += does_unit_match_out.chr_count;
            next_cmp_str = does_unit_match_out.next_cmp_str;
            
            count += 1;
        }
        else
        {
            break;
        }
    }

    if(min <= count)
    {
        match_multiple_out.matched = TRUE;
        match_multiple_out.next_cmp_str = next_cmp_str;
        match_multiple_out.chr_count = chr_count;
    }
    else
    {
        match_multiple_out.matched = FALSE;
        match_multiple_out.next_cmp_str = NULL;
        match_multiple_out.chr_count = 0;
    }

    return match_multiple_out;
}

match_expr_output match_expr(string expr, string str, int match_len)
{
    match_expr_output match_expr_out = {0};

    if(expr[0] == '\0')
    {
        match_expr_out.matched = TRUE;
        match_expr_out.match_len = match_len;

        return match_expr_out;
    }
    else if(is_end(expr[0]) == TRUE)
    {
        if(str[0] != '\0')
        {
            match_expr_out.matched = FALSE;
            match_expr_out.match_len = 0;

            return match_expr_out;
        }
        else
        {
            match_expr_out.matched = TRUE;
            match_expr_out.match_len = match_len;

            return match_expr_out;
        }
    }
    else
    {
        if(str[0] == '\0')
        {
            match_expr_out.matched = FALSE;
            match_expr_out.match_len = 0;

            return match_expr_out;
        }
    }

    split_expr_output split_expr_out = split_expr(expr);
    string head = split_expr_out.head;
    string operator = split_expr_out.operator;
    string rest = split_expr_out.rest;

    if(is_operator(operator) == TRUE)
    {
        match_multiple_output match_multiple_out = {0};

        if(is_star(operator[0]) == TRUE)
            match_multiple_out = match_multiple(expr, str, 0, INFINITY);
        else if(is_plus(operator[0]) == TRUE)
            match_multiple_out = match_multiple(expr, str, 1, INFINITY);
        else if(is_question_mark(operator[0]) == TRUE)
            match_multiple_out = match_multiple(expr, str, 0, 1);
        else{}

        if(match_multiple_out.matched == TRUE)
        {
            return match_expr(rest, match_multiple_out.next_cmp_str, match_len + match_multiple_out.chr_count);
        }
        else{}
    }
    else if(is_unit(head) == TRUE)
    {
        does_unit_match_output does_unit_match_out = does_unit_match(expr, str); 

        if(does_unit_match_out.matched == TRUE)
        {
            return match_expr(rest, does_unit_match_out.next_cmp_str, match_len + does_unit_match_out.chr_count);
        }
        else{}
    }
    else{}

    match_expr_out.matched = FALSE;
    match_expr_out.match_len = 0;

    return match_expr_out;
}

match_output match(string expr, string str)
{
    match_output match_out = {0};

    int str_len = strlen(str);

    bool key = FALSE;

    if(is_start(expr[0]) == TRUE)
    {
        expr = expr + 1;
        key = TRUE;
    }

    for(int match_pos = 0; match_pos < str_len; match_pos++)
    {
        match_expr_output match_expr_out = match_expr(expr, str + match_pos, 0);

        if(match_expr_out.matched == TRUE)
        {
            match_out.matched = TRUE;
            match_out.match_pos = match_pos;
            match_out.match_len = match_expr_out.match_len;

            return match_out;
        }
        else if(key)
            break;
    }

    match_out.matched = FALSE;
    match_out.match_pos = NONE;
    match_out.match_len = 0;

    return match_out;
}

int main(void) 
{
    string expr = "^http://(\\a|\\d)+.(com|net|org)";
    string str = "http://numan1234tepe.com/hey/there'";

    match_output match_out = match(expr, str);

    printf("expr: %s\n", expr);
    printf("str: %s\n", str);
    printf("matched: %d\n", match_out.matched);
    printf("match_pos: %d\n", match_out.match_pos);
    printf("matched_len: %d\n", match_out.match_len);

    if(match_out.matched == TRUE)
    {
        string matched_string = (string) malloc(match_out.match_len * sizeof(char) + 1);

        strncpy(matched_string, str + match_out.match_pos, match_out.match_len);

        printf("Matched string: %s\n", matched_string);
    }
    else
    {
        printf("No match.");
    }

    return 0;
}
