
#include <stdio.h>
#include "page.h"

  

void
print_my_URL(page web){
    printf("my URL = %s\n", web.my_URL);
}

void
print_URLs(page web){   
   for (int i = 0; i < web.URL_count+1; i++){
      printf("URL%d = %s\n", i, web.URLs[i]);
   }
}

void
print_valid_URL(page web){
   printf("valid count = %d\n",web.vaild_URL_count);
   for (int i = 0; i < web.vaild_URL_count+1; i++){
      printf("Valid URL%d = %s\n", i, web.valid_URL[i]);
   }
}



