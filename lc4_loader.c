/************************************************************************/
/* File Name : lc4_loader.c		 										*/
/* Purpose   : This file implements the loader (ld) from PennSim		*/
/*             It will be called by main()								*/
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "lc4_memory.h"

/* declarations of functions that must defined in lc4_loader.c */

FILE* open_file(char* file_name)
{
  // check if the file_name passed in is valid
	if (file_name == NULL || *file_name == '\0') {
        return NULL;
    }
    // open the file based on the passed in file_name for reading
    FILE* fp = fopen(file_name, "rb"); 
    // check if the file is opened successfully
    if (fp == NULL) { 
        return NULL;
    }
    return fp;
}

int parse_file (FILE* my_obj_file, row_of_memory** memory)
  
/* remember to adjust 16-bit values read from the file for endiannness
 * remember to check return values from fread() and/or fgetc()
 */ 
{
  row_of_memory* this_node = *memory;
  

  do {
    // get the header while handling endianness
    int byte1 = fgetc(my_obj_file);
		if (byte1 == EOF) break;
		int byte2 = fgetc(my_obj_file);
    if (byte2 == EOF) break;
    unsigned int header = (byte1<<8) | byte2;

    // get address while handling endianness
    short unsigned int add1 = fgetc(my_obj_file);
    short unsigned int address = (add1<<8) + fgetc(my_obj_file);

    // get count while handling endianness
    short unsigned int n1 = fgetc(my_obj_file);
    short unsigned int n = (n1<<8) + fgetc(my_obj_file);

    // check what type of the header is
    // header is "code"
    if (header == 0xCADE){
      for (int i = 0; i < n; ++i){
        // read contents
        short unsigned int cont1 = fgetc(my_obj_file);
        short unsigned int content = (cont1<<8) + fgetc(my_obj_file);
        add_to_list(&this_node, address + i, content);

      }
    }

    // header is "data"
    if (header == 0xDADA){
      for (int i = 0; i < n; ++i){
        // read contents
        int cont1 = fgetc(my_obj_file);
        unsigned int content = (cont1<<8) + fgetc(my_obj_file);
        add_to_list(&this_node, address + i, content);
      }

    }

    // header is "symbol"
    if (header == 0xC3B7){
      char* counter = malloc(sizeof(char) * (n + 1));
      row_of_memory* temp = search_address(this_node, address);
      if (temp == NULL){
        add_to_list(&this_node, address, 0);
        temp = search_address(this_node, address);
      }
      for (int j = 0; j < n; ++j){
        counter[j] = fgetc(my_obj_file);
      }

      counter[n] = '\0';
      temp->label = counter;
    }

  }while (1);
  *memory = this_node;
  
  int test = fclose(my_obj_file);
  if (test != 0){
    return 2;
  }
  return 0 ;
}
