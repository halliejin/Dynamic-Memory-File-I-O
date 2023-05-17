/************************************************************************/
/* File Name : lc4_memory.c		 										*/
/* Purpose   : This file implements the linked_list helper functions	*/
/* 			   to manage the LC4 memory									*/
/*             															*/
/* Author(s) : Zihan Jin & Congyang Ding (see README)												*/
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "lc4_memory.h"


/*
 * adds a new node to a linked list pointed to by head
 */
int add_to_list (row_of_memory** head,
		 short unsigned int address,
		 short unsigned int contents)
{
    row_of_memory* current_node = *head;

    /* check to see if there is already an entry for this address and update the contents.  no additional steps required in this case */
    while (current_node != NULL){
      if (current_node->address == address){
        current_node->contents = contents;
        return 0;
      }

      current_node = current_node->next;
    }

    /* allocate memory for a single node */
		row_of_memory* new_node = (row_of_memory*) malloc(sizeof(row_of_memory));
		// -1 if malloc fails
		if (new_node == NULL){
			return -1;
		}
		/* populate fields in newly allocated node w/ address&contents, NULL for label and assembly */
		/* do not malloc() storage for label and assembly here - do it in parse_file() and reverse_assemble() */
    new_node->address = address;
    new_node->label = NULL;
    new_node->contents = contents;
    new_node->assembly = NULL;
    new_node->next = NULL;
		
		/* if *head is NULL, node created is the new head of the list! */
    if (*head == NULL){
      *head = new_node;
      return 0;
    }
		
		/* otherwise, insert node into the list in address ascending order */
    current_node = *head;
    row_of_memory* previous_node = NULL;

    while (current_node != NULL && current_node->address < address){
      // find the location based on addresses
      previous_node = current_node;
      current_node = current_node->next;
    }

    if (previous_node == NULL){
      new_node->next = *head;
      *head = new_node;
    }
    else{
      // insert the node
      previous_node->next = new_node;
      new_node->next = current_node;
    }
		
		/* return 0 for success, -1 if malloc fails */

	return 0 ;
}




/*
 * search linked list by address field, returns node if found
 */
row_of_memory* search_address (row_of_memory* head,
			       short unsigned int address )
{
  if (head == NULL) {
      return NULL;
    }
	/* traverse linked list, searching each node for "address"  */
  row_of_memory* this_node = head;

  while (this_node != NULL && this_node->address != address){
    this_node = this_node->next;
  }
	/* return pointer to node in the list if item is found */
  if (this_node != NULL) {
    return this_node;
  }
	/* return NULL if list is empty or if "address" isn't found */

	return NULL ;
}

/*
 * search linked list by opcode field, returns node if found
 */
row_of_memory* search_opcode  (row_of_memory* head,
				      short unsigned int opcode  )
{
    /* opcode parameter is in the least significant 4 bits of the short int and ranges from 0-15 */
		/* see assignment instructions for a detailed description */
    if (head == NULL) {
      return NULL;
    }

    row_of_memory* this_node = head;

    /* traverse linked list until node is found with matching opcode in the most significant 4 bits
	   AND "assembly" field of node is NULL */
    while (this_node != NULL){
      if ((this_node->contents >>12) == opcode && this_node->assembly == NULL){
          return this_node;   
      }
      this_node = this_node->next;
    }
    	/* return NULL if list is empty or if no matching nodes */
      return NULL;
}



void print_list (row_of_memory* head )
{
	/* make sure head isn't NULL */
  if (head != NULL){
    /* print out a header */
    char label_col[15] = "<label>";
    char address_col[15] = "<address>";
    char contents_col[15] = "<contents>";
    char assembly_col[15] = "<assembly>";
    printf("%-20s%-20s%-20s%-20s\n", label_col,address_col,contents_col,assembly_col);
    /* don't print assembly directives for non opcode 1 instructions if you are doing extra credit */
    row_of_memory* this_node = head;
	/* traverse linked list, print contents of each node */
    while (this_node != NULL){
      printf("%-20s%-20.04x%-20.04x%-20s\n", this_node->label,this_node->address,this_node->contents,this_node->assembly);
      this_node = this_node->next;
    }
  }
	else{
    printf("The linked list is empty.");
  }
}

/*
 * delete entire linked list
 */
int delete_list    (row_of_memory** head )
{
	/* delete entire list node by node */
	/* set the list head pointer to NULL upon deletion */
  row_of_memory** this_node = head;
  row_of_memory* prev;

  while (*this_node != NULL){
    prev = (*this_node)->next;
    if((*this_node)->label != NULL){
      free((*this_node)->label);
    }
    if((*this_node)->assembly != NULL){
      free((*this_node)->assembly);
    }
    free(*this_node);
    *this_node = prev;
  }

	*head = NULL;

  return 0;
}
