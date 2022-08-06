#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "regexengine.h"

int main(void) 
{
    string expr = "^http://(\\a|\\d)+.(com|net|org)";
    string str = "http://numan123tepe.com/hey/there";

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
