#define MAX_RESPONSE 100000



#ifndef PAGE_H
#define PAGE_H


typedef struct page{
   char *error_code;
   char response[MAX_RESPONSE]; 
   char* my_URL;
   char *entry_URL;
   char* my_hostname;
   char* my_domain;
   char* my_path;
   char* first_URL;
   char *first_path;
   char* URLs[MAX_RESPONSE];
   int URL_count;
   int vaild_URL_count;
   char* valid_URL[1000];  
}page;


void
print_valid_URL(page web);
void
print_my_URL(page web);
void
print_URLs(page web);




#endif
