#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "page.h"
#include "pcre.h"

char
*find_match(char* check_str, char *type){
    const char *error;
    int   erroffset;
    pcre *re;
    int   rc;
    int   i;
    int   ovector[MAX_RESPONSE];
    char *ans;
    char *regex;   
    int l = strlen(check_str);
    char *str = malloc(sizeof(char) * (l + 1));
    memcpy(str, check_str, l);
    str[l] = '\0';


    /* different regex for different type */
    if(strcmp(type, "hostname") == 0){
        regex = "^(?:http://)([^/]+)([-a-zA-Z0-9:._+~/=]{0,1024})$";
    }else if(strcmp(type, "domain") == 0){
        regex = "^(?:[-a-zA-Z0-9@:_+]{1,1024}.)([-a-zA-Z0-9@:._+~=]{1,1024})$";
    }else if(strcmp(type, "mime") == 0){
        regex = "(?:Content-Type): (.*?)[;\n]";
    }else if(strcmp(type, "filename") == 0){
        regex = "^(?:http://)([-a-zA-Z0-9:._+~/=]{0,1024}/)(?:[-a-zA-Z0-9:._+~=]{0,1024})$";
    }else{
        fprintf(stderr, "no regex\n"); 
        exit(EXIT_FAILURE);    
    }
    


    re = pcre_compile (regex,          /* the pattern */
                       PCRE_MULTILINE,
                       &error,         /* for error message */
                       &erroffset,     /* for error offset */
                       0);             /* use default character tables */
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
        //printf("str = %s\n",str);
        //printf("now = %s\n", res);
        offset = ovector[1];
        ans = res;
        
    }
    
    pcre_free(re);
    
    return ans;
}


page
parse_url(page web){

    char *hostname;
    char *domain;
    char *filepath;
    int valid_count = -1;
    

    /* parese every URL, check if valid */
    for(int i = 0; i < web.URL_count+1; i++){
        hostname = NULL;
        domain = NULL;
        filepath = NULL;
        int len = strlen(web.URLs[i]);
       
        char *URL = malloc(sizeof(char) * (len + 1));
        memcpy(URL,web.URLs[i],len);
        URL[len] = '\0';
       

        
        /* different type of URL, Absolute or Relative*/
        if(strncmp(URL, "http://",7) == 0){

        }else if(strncmp(URL, "//",2) == 0){
            char *res = malloc(sizeof(char) * (strlen(URL) + 6));
            res[0]='\0';
            strcat(res,"http:");
            strcat(res,URL); 
            URL = res;
        }else if(strncmp(URL, "/",1) == 0){
            char *res = malloc(sizeof(char) * (strlen(URL) + strlen(web.my_hostname)+8));
            len = strlen(web.my_hostname) +7;
            memcpy(res,web.first_URL,len);
            res[len] = '\0';
            strcat(res, URL);
            URL = res;
        }else if(strstr(URL,"./")==NULL ){ 
            if(strcmp(web.my_path,"/") == 0){
                len = strlen(web.my_hostname);
                char *hostn = malloc(sizeof(char) * (1 + len ));
                memcpy(hostn,web.my_hostname,len);
                hostn[len] = '\0';
                filepath = hostn;
                strcat(filepath,"/");
            }else{filepath = find_match(web.first_URL,"filename");}
            len = strlen(filepath) + 7;
            char *res = malloc(sizeof(char) * (len + 1 + strlen(URL)));
            res[0]='\0';
            strcat(res,"http://");
            strcat(res,filepath);
            strcat(res, URL);
            URL = res;
        }else{continue;}
       
        

        hostname = find_match(URL,"hostname");
        domain = find_match(hostname,"domain");
        
       
        /* check if all but the first components of the host match the host of the URL on the command line. */
        if((strcmp(domain,web.my_domain) == 0 || strcmp(domain,web.my_hostname) == 0)){
            web.valid_URL[++valid_count] = URL;
        }else{continue;}  
    }
    
    web.vaild_URL_count =  valid_count;

    return web;
}
