#include "page.h"
#include "pcre.h"

#ifndef PARSE_URL_H
#define PARSE_URL_H




char *find_match(char* str, char *type);
char *get_host(char *link);
    

page parse_url(page web);

#endif
