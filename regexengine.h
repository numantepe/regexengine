#ifndef REGEXENGINE_H
#define REGEXENGINE_H

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

#endif