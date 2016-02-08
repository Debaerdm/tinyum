#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include "http_request.h"

#define CR '\n'
#define LF '\r'
#define OUT 0
#define IN 1

/*
 * Keeps the term that we have treated
 */
enum state
{
     s_start,
     s_method,
     s_uri,
     s_first_http_major,
     s_http_major,
     s_http_minor,
     s_header_done,
};

/*
 * words - return numbers of words in string
 */
int words(const char* s)
{
     unsigned int index = 0;
     const char *ch = s;
     int state = OUT;

     while (*ch) {
	  if (isspace(*ch))
	       state = OUT;
	  else if (state == OUT) {
	       state = IN;
	       ++index;
	  }

	  ++ch;
     }

     return index;
}

/*
 * read_requesthdrs - read and parse HTTP request headers
 */
int read_http_request(const char* line, http_request *r)
{
     int index = 0, current_state = s_start;
     char ch;
     while (current_state != s_header_done) {
     
	  switch (current_state) {
	   
	  case s_start:
	    
	       if (words(line) != 3) {
		    r->m = HTTP_INVALID;
		    current_state = s_header_done;
		    return EXIT_FAILURE;
	       } else { 
		    current_state = s_method;
	       }
	       
	       break;
	       
	  case s_method:
	       ch = line[index];
	       /* HTTP Method */
	       switch (ch) {
	       case 'O': r->m = HTTP_OPTIONS; index = 7; break;
	       case 'G': r->m = HTTP_GET; index = 3; break;
	       case 'H': r->m = HTTP_HEAD; index = 4; break;
	       case 'P': r->m = HTTP_POST; /* or PUT */ break;
	       case 'D': r->m = HTTP_DELETE; index = 6; break;
	       case 'T': r->m = HTTP_TRACE; index = 5; break;
	       case 'C': r->m = HTTP_CONNECT; index = 7; break;
	       default: r->m = HTTP_INVALID; break;
	       }

	       if (index == 0 && r->m == HTTP_POST) {
		    ch = line[index++];
		    if (ch == 'U') {
			 r->m = HTTP_PUT;
			 index = 3;
		    } else {
			 index = 4;
		    }
	       }
	       
	       index += 1;
	       current_state = s_uri;
	       
	       break;
	       
	  case s_uri:
	       /* Avoid uri */
	       while (line[index++] != ' ')
	       
	       current_state = s_first_http_major;
	       break;

	  case s_first_http_major:
	       if (strncmp("HTTP/", (line + index), 5) == 0) {
		    index += 5;
		    current_state = s_http_major;
	       } else {
		    r->m = HTTP_INVALID;
		    current_state = s_header_done;
		    return EXIT_FAILURE;
	       }
	       
	       break;
	       
	  case s_http_major:
	       if (isdigit(line[index])) {
		    r->major_version = atoi(&line[index]);
		    index += 2;
		    current_state = s_http_minor;
	       } else {
		    r->m = HTTP_INVALID;
		    current_state = s_header_done;
		    return EXIT_FAILURE;
	       }

	       break;

	  case s_http_minor:
	       if (isdigit(line[index])) {
		    r->minor_version = atoi(&line[index]);
		    while (line[index++] != ' ');
		    current_state = s_header_done;
	       } else {
		    r->m = HTTP_INVALID;
		    current_state = s_header_done;
		    return EXIT_FAILURE;
	       }

	       break;
	       
	  }
     }

     return EXIT_SUCCESS;
}
