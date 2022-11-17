/**
 * Project: compiler of IFJ22 programming language
 *
 * @brief The implementation of a parser.
 *
 * @author Patrik Čerbák <xcerba00@stud.fit.vutbr.cz>
 * @author TODO - dopište se tady
 */

#ifndef _PARSER_H
#define _PARSER_H

/**
 * Function for geting data into the token.
*/
void get_tkn();

/**
 * Function for freeing the token.
*/
void free_tkn();

/**
 * A function for the <start> rule.
 * 
 * @return True if it was correct.
*/
bool start();

/**
 * A function for the <program> rule.
 * 
 * @return True if it was correct.
*/
bool program();

/**
 * A function for the <konec> rule.
 * 
 * @return True if it was correct.
*/
bool konec();

#endif