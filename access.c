// Access file for Lab 4 design problem.
// functions for controlling access via reading the ACCESS_FILE.
#include "access.h"

// This function checks against the ACCESS_FILE to see if the
// file is allowed to be accessed. Returns true / false based on 
// whether or not the file is forbidden.



//Returns NULL if syntax error.

access_rule* generate_rule(const char* line)
{
    access_rule* rule = malloc(sizeof(access_rule));


    int i = 0;

    char curr = line[i];
    if (line[i] != '<') 
    {
            free(rule);
            message("Syntax error in ACCESS_RULE file: lines must begin with '<'\n");
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
            message("Syntax error in ACCESS_RULE file: unterminated '>'\n");
            return NULL;
        }
        rule_type_string[i-1] = line[i];
    }
    rule_type_string[i-1] = '\0'; //finish off the string with a null.
    //Now we need to compare the rulestring to see if it is valid.
    if (strcmp(rule_type_string, "ALLOWFILE") == 0)
        rule->type = ALLOWFILE;
    else if (strcmp(rule_type_string, "DENYFILE") == 0)
        rule->type = DENYFILE;
    //user ops
    else if (strcmp(rule_type_string, "ALLOWUSER") == 0)
        rule->type = ALLOWUSER;
    else if (strcmp(rule_type_string, "DENYUSER") == 0)
        rule->type = DENYUSER;
    else
    {
        message("Syntax error in ACCESS_FILE.\n");
        return NULL;
    }
        
    //TODO: ADD MORE HERE LATER.

    free(rule_type_string);
    i++; //move past the close_tag_pos to the beginning of the string
    while (line[i] == ' ' || line[i] == '\t') 
        i++; //ignore leading spaces or tabs
    
    char* string = malloc(sizeof(char) * 100); //because what the hey

    strcpy(string, line+i);
    //save the string to the rule and return
    //a little housekeeping: want to get rid of any trailing newline
    if (string[strlen(string)-1] == '\n')
        string[strlen(string)-1] = '\0';
    rule->string = string;
    message("\nRULE type: %i\n", rule->type);
    message("RULE string: %s", rule->string);


    return rule;
}

bool file_access_ok(const char* filename)
{
    //we initially assume access will be allowed.
    //This value will be modified as we go on, based on rules we find.
    bool access = true;

    //Open the ACCESS_FILE
    char line[100];

    FILE *input = fopen("../ACCESS_FILE", "r");
    if (!input)
    {
        // For backwards compatibility, if there is no ACCESS_FILE we assume
        // all accesses are OK.
        message("No ACCESS_FILE found. Assuming access is ok");
        return access; 
    }

    //parse the file for filename
    int linenum = 0;
    access_rule* current_rule;
    while (fgets(line, 100, input)) //search line by line
    {
        current_rule = generate_rule(line); //get the rule for this line.
        if (!current_rule) continue; //ignore nulls, blank lines, and syntax errors
        
        if (current_rule->type == ALLOWFILE || current_rule->type == DENYFILE) 
        //file privilege rule
        {
            message("%i\n", strcmp(current_rule->string, "all"));
            if ((strcmp(current_rule->string, filename) == 0)  
            //this is a rule pertaining to this file
            ||  (strcmp(current_rule->string, "all") == 0))  
            //this is a rule pertaining to all files.
            {
                if (current_rule->type == ALLOWFILE)
                    access = true;
                else //type == DENYFILE
                    access = false;
            }
        }
    }
    fclose(input); //close the ACCESS_FILE
    //when finished, we return whether this is an allowed file (default) or disallowed file.
    return access;

}

//Checks to see if the peer is allowed to access a file

bool peer_access_ok (struct sockaddr_in peer_addr)
{

    message("* Got connection from %s\n", 
        inet_ntoa(peer_addr.sin_addr));

    bool access = true;
    char line[100];
    //debug
    char cwd[1024];
    message("DEBUG: current dir:  %s\n", getcwd(cwd, sizeof(cwd)));
    FILE *input = fopen("../ACCESS_FILE", "r");

    
    //function gets the connected peer's ip address
    char *user_id = inet_ntoa(peer_addr.sin_addr); 

    int linenum = 0;
    access_rule* current_rule;
    while (fgets(line, 100, input))
    {
        current_rule = generate_rule(line);
        if (!current_rule) continue;
        if (current_rule->type == ALLOWUSER &&
            strcmp(current_rule->string, user_id) == 0)
               
            access = true;

        else if (current_rule->type == DENYUSER &&
            strcmp(current_rule->string, user_id) == 0)
           
            access = false;    
    }
    fclose(input);
    return access;
}

    


