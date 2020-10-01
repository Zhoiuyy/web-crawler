#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "pcre.h"
#include "page.h"

typedef struct match_ans{
   char *match_str[MAX_RESPONSE];
   int match_count;
}match_ans;


page
parse_html(page web){
    const char *error;
    int   erroffset;
    pcre *re;
    int   rc;
    int   i;
    int   ovector[MAX_RESPONSE];
    char *regex = "<(?:a|A) ?[^>]+(?:h|H)(?:r|R)(?:e|E)(?:f|F) ?= ?[\"|\'](.*?)[\"|\'][^>]*>(?:.*?\n*?.*?)*?</(?:a|A)>";
    char *str  = web.response;
    

    
    
   
    re = pcre_compile (regex,          // the pattern 
                       PCRE_MULTILINE,
                       &error,         // for error message 
                       &erroffset,     // for error offset 
                       0);             // use default character tables 
    if (!re)
    {
        printf("pcre_compile failed (offset: %d), %s\n", erroffset, error);
        exit(EXIT_FAILURE);
    }

    unsigned int offset = 0;
    unsigned int len    = strlen(str);
    int count = -1;
    while (offset < len && (rc = pcre_exec(re, 0, str, len, offset, 0, ovector, sizeof(ovector))) >= 0)
    {   

        int len = ovector[3] - ovector[2];
        char *res = malloc(sizeof(char) * (len + 1));
        memcpy(res, str + ovector[2], len);
        res[len] = '\0';
        offset = ovector[1];
        web.URLs[++count] = res;
        
    }
    web.URL_count = count;

    
    pcre_free(re);
    

    return web;
}
