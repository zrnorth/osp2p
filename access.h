// Access file for Lab 4 design problem.
// functions for controlling access via reading the ACCESS_FILE.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// This function checks against the ACCESS_FILE to see if the
// file is allowed to be accessed. Returns true / false based on 
// whether or not the file is forbidden.

// Different types of rules in our ACCESS_FILE.
typedef enum 
{
    ALLOWFILE, //explicity allows a file to be served
    DENYFILE   // denies a file to be served.
} RULE_TYPE;
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

access_rule* generate_rule(const char* line)
{
    access_rule* rule = malloc(sizeof(access_rule));


    int i = 0;

    char curr = line[i];
    if (line[i] != '<') 
    {
            free(rule);
            return NULL; //syntax error
    } //else

    i++; //first char of rule
    char* close_tag_pos = strchr(line+i, '>'); //gets the end of tag index pos.

    char* rule_type_string = malloc(sizeof(char) * 100); 
    for (; line+i != close_tag_pos; i++)
    {
        if (line[i] == '\0' || line[i] == '\n') //syntax error.
        {
            free(rule);
            return NULL;
        }
        rule_type_string[i-1] = line[i];
    }
    rule_type_string[i] = '\0'; //finish off the string with a null.
    //Now we need to compare the rulestring to see if it is valid.
    if (strcmp(rule_type_string, "ALLOWFILE") == 0)
        rule->type == ALLOWFILE;
    else if (strcmp(rule_type_string, "DENYFILE") == 0)
        rule->type == DENYFILE;
    //TODO: ADD MORE HERE LATER.

    free(rule_type_string);
    i++; //move past the close_tag_pos to the beginning of the string
    while (strcmp(line+i, " ") == 0 || strcmp(line+i,"\t") == 0) i++; //ignore leading spaces or tabs
    
    char* string = malloc(sizeof(char) * 100); //because what the hey
    int stringiter = 0;
    //get the string and save it
    for (; line[i] != '\n' && line[i] != '\0'; stringiter++, i++)
    {
        string[stringiter] == line[i];
    }
    string[stringiter] = '\0'; //make sure to nullterminate the string.

    //save the string to the rule and return
    rule->string = string;

    return rule;
}

bool file_access_ok(const char* filename)
{
    //we initially assume access will be allowed.
    //This value will be modified as we go on, based on rules we find.
    bool access = true;

    //Open the ACCESS_FILE
    char line[100];
    FILE *input = fopen("ACCESS_FILE", "r");
    if (!input)
    {
        // For backwards compatibility, if there is no ACCESS_FILE we assume
        // all accesses are OK.
        return access; 
    }

    //parse the file for filename
    int linenum = 0;
    access_rule* current_rule;
    while (fgets(line, 100, input)) //search line by line
    {
        current_rule = generate_rule(line); //get the rule for this line.
        if (!current_rule) continue; //ignore nulls, blank lines, and syntax errors
        
        if (current_rule->type == ALLOWFILE || current_rule->type == DENYFILE) //file privilege rule
        {
            if ((strncmp(current_rule->string, filename, 100) == 0)  //this is a rule pertaining to this file
            ||  (strncmp(current_rule->string, "all", 100) == 0))  //this is a rule pertaining to all files.
            {
                if (current_rule->type == ALLOWFILE)
                    access = true;
                else //type == DENYFILE
                    access = false;
            }
        }
    }
    //when finished, we return whether this is an allowed file (default) or disallowed file.
    return access;

}

