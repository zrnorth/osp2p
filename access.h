// Access file for Lab 4 design problem.
// functions for controlling access via reading the ACCESS_FILE.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "osp2p.h"

// This function checks against the ACCESS_FILE to see if the
// file is allowed to be accessed. Returns true / false based on 
// whether or not the file is forbidden.

// Different types of rules in our ACCESS_FILE.
typedef enum 
{
    ALLOWFILE , //explicity allows a file to be served
    DENYFILE   // denies a file to be served.
}   RULE_TYPE;
 //  later:
 //   ALLOWUSER,
 //   DENYUSER
    
//Used for 
typedef struct ar
{
    RULE_TYPE type; //the rule (tag)
    char* string;   //the string following the tag (who it applies to.)
} access_rule;



//Generates a rule struct from an inputted line.
//Returns NULL if syntax error.

access_rule* generate_rule(const char* line);


//called to check if the file access is ok with the ACCESS_FILE.
bool file_access_ok(const char* filename);
