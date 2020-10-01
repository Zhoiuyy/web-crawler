#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "pcre.h"
#include "page.h"

#ifndef PARSE_HTML_H
#define PARSE_HTML_H

typedef struct match_ans{
   char *match_str[MAX_RESPONSE];
   int match_count;
}match_ans;




page parse_html(page web);

page parse_urls(page web);
#endif
