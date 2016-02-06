#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "http_request.h"

#define CR '\n'
#define LF '\r'
#define HT '\t'
#define OUT 0
#define IN 1

enum state
{
     s_start,
     s_method,
     s_uri,
     s_first_http_major,
     s_http_major,
     s_first_http_minor,
     s_fist_minor,
     s_header_done
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
void read_requesthdrs(const char* line, http_request *r)
{
     int index, current_state = s_start;
     char ch;
     while (current_state != s_header_done) {
     
	  switch (current_state) {
	  
	  case s_start:
	       if (words(line) != 3)
		    r->m = HTTP_INVALID;
	       else 
		    current_state = s_method;
	       
	  case s_method:
	       index = 0;
	       ch = line[index];
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
	       current_state = s_header_done;
	  }
     }
}
