// Access file for Lab 4 design problem.
// functions for controlling access via reading the ACCESS_FILE.
#ifndef ACCESS_H
#define ACCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <netinet/in.h> //for sockaddr_in
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

#include "osp2p.h"
#include "osppeer.h"

// This function checks against the ACCESS_FILE to see if the
// file is allowed to be accessed. Returns true / false based on 
// whether or not the file is forbidden.

// Different types of rules in our ACCESS_FILE.
typedef enum 
{
    ALLOWFILE , //explicity allows a file to be served
    DENYFILE,   // denies a file to be served.
    ALLOWUSER,
    DENYUSER
}   RULE_TYPE;
    
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


//called to check if the peer access is ok with the ACCESS_FILE.
bool peer_access_ok (struct sockaddr_in peer_addr);

#endif
