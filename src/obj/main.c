#define _XOPEN_SOURCE 700
#include <arpa/inet.h>
#include <assert.h>
#include <netdb.h> /* getprotobyname */
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>  
#include <unistd.h>
#include "request.h"
#include "parse_html.h"
#include "parse_url.h"
#include "queue.h"

/* check if the array fetched_URL contains URL*/
int
contain_URL(char *fetched_URL[], int count, char *URL){
    for(int i = 0; i <  count; i++ ){
        if(strcmp(fetched_URL[i],URL)==0){
            return 0;
        }
    }
    return -1;
}

/* print */

void
print_fetch(char *fetched_URL[], int fetch_count){
    printf("fetched num = %d\n", fetch_count);
        for (int i = 0; i < fetch_count; i++){
            printf("fetched URL%d = %s\n", i, fetched_URL[i]);
        }
}

int main(int argc, char** argv) {
    page web;
    Queue links;
    char *fetched_URL[100];
    int fetch_count;
    char *hostname;
    char *domain;
    //char entry[] = "http://info.cern.ch";
    //char entry[] = "http://www.ibdhost.com/help/html/";
   

    char *entry;
    entry = argv[1];
    fetch_count = 0;
    char *mime;
    QueueInit(&links); 
    QueuePush(&links,entry);
    web.first_URL = entry;
    char *ori_str;
    char *path = "";

    while (!QueueEmpty(&links) && fetch_count < 100)
    {
        QueuePop(&links,&ori_str);
        int len = strlen(ori_str);
        char *entry_str = malloc(sizeof(char) * (len+1));
        memcpy(entry_str, ori_str , len);
        entry_str[len] = '\0';
        web.entry_URL = entry_str;
       
        /*check if the URlL already be fecthed*/
        if(contain_URL(fetched_URL,fetch_count,entry_str)==-1){
        
            fetched_URL[fetch_count++] = entry_str;

        }else{continue;}
      
        char *str = malloc(sizeof(char) * (len+1));
        memcpy(str, ori_str , len);
        str[len] = '\0';
        strcat(str,"/"); 
        
        web.my_URL = str;
        
        hostname = find_match(str, "hostname");
        memcpy(str, ori_str, len);
        str[len] = '\0';
        strcat(str,"/"); 

        domain =  find_match(hostname,"domain"); 
        web.URL_count = 0;
        web.my_hostname = hostname;
        web.my_domain = domain;
        
        /* find the file path of this URL */
        len = strlen(str)-8-strlen(web.my_hostname);
        if(len == 0){
            char *path = malloc(sizeof(char) * (2));
            memcpy(path, str + strlen(web.my_hostname)+7 , 1);
            path[1] = '\0';
            web.my_path = path;
        }else{
            char *path = malloc(sizeof(char) * (len+1));
            memcpy(path, str + strlen(web.my_hostname)+7 , len);
            path[len] = '\0';
            web.my_path = path;
        }
        /* print out the log */
        fprintf(stdout, "%s\n",web.entry_URL);

        /* request for response */
        web = request(web,1);

        //fprintf(stdout, "error code = %s\n", web.error_code);
        

        /* check the error code */
        if(strcmp(web.error_code,"200") == 0){

        }else if(strcmp(web.error_code,"503")==0||strcmp(web.error_code,"504")==0){
            web = request(web,2);
        }else if(strcmp(web.error_code,"301") == 0){
            continue;
        }else if(strcmp(web.error_code,"404") == 0 || strcmp(web.error_code,"410") || strcmp(web.error_code, "414") == 0){
            //permanent failure
            continue;
        }else if(strcmp(web.error_code,"401") == 0 ){
            web = request(web,401);
        }else{
            continue;
        }


        /* check the MIME-TYPE  */
        mime = find_match(web.response,"mime"); 
        mime[strlen(mime)-1] = 0;
        //printf("mime = %s\n",mime);
        if(!(strcmp(mime, "text/htm") == 0 || strcmp(mime, "text/html") == 0)){
            //printf("not text/html,pass, mime = %s\n",mime);
            continue;
        }


       
        web = parse_html(web);    
        web = parse_url(web);
       
        /* push every valid URL to the queue*/
        for (int i = 0; i < web.vaild_URL_count+1; i++)
        {   
            QueuePush(&links,web.valid_URL[i]);
        }  

    }
    //print_fetch(fetched_URL,fetch_count);

    
    return 0;
}

