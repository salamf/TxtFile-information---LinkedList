#define _POSIX_C_SOURCE 200809L
#include "linked_list.h"

// Check if sort is in args
int check_sorted(char* arr[], int len){

    for(int i = 0; i < len; i++){
        if(strcmp(arr[i], "--sort") == 0){
            return 1;
        }
    }
    return 0;
}

// Finds the index of the filename in argv
// return -1 if it couldn't be found
int find_file_index(char* arr[], int len){

    for(int i = 0; i < len; i++){
        if(strstr(arr[i], ".txt") != NULL) return i;
    }
    return -1;
}

int check_infile(char* arr[], int len){
    
    for(int i = 0; i < len; i++){
        if(strcmp(arr[i], "--infile") == 0)
            return 1;
    }
    
    return 0;
}

// Free's everything in a given word list
void free_word_list(word_node** head){
    word_node* temp;

    while(*head != NULL){
        free((*head) -> word);
        temp = *head;
        *head = (*head) -> next;
        free(temp);
    }
}

// Free's the whole list
void free_list(gen_node** head){
   gen_node* temp;

   while (*head != NULL){

       free_word_list(&((*head) -> word_list_head));

       temp = *head;
       *head = (*head) -> next;
       free(temp);
    }
}

// Prints the words in a given word list
void print_words(word_node* head){
    
    word_node* curr = head;
    while(curr != NULL){
        printf("\"%s\"", curr -> word);
 
        if(curr -> next == NULL)
            break;
        else if(curr -> next -> next == NULL)
            printf(" and "); 
        else
            printf(", ");

        curr = curr -> next;
    }
}

// Prints everything in the list (formatted)
void print_no_sort(gen_node* head){

    gen_node* curr = head;
    while(curr != NULL){
        printf("Count[%02d]=%02d;", curr -> word_len, curr -> num_words);

        printf(" (words: ");
        print_words(curr -> word_list_head);
        printf(")\n");

        curr = curr -> next;
    }
}

// Prints everything in the list when --sort
// is in args. So first reverses the word list
// then prints it in the same way as the above function
void print_sort(gen_node* head){

    gen_node* curr = head;
    while(curr != NULL){
        printf("Count[%02d]=%02d;", curr -> word_len, curr -> num_words);
        
        reverse_list(&(curr -> word_list_head));

        printf(" (words: ");
        print_words(curr -> word_list_head);
        printf(")\n");

        curr = curr -> next;
    }
}

int 
main(int argc, char *argv[]) {
    

    int filename_index = find_file_index(argv, argc);   // Get index of input_file (in argv)

    if(filename_index == -1 || check_infile(argv, argc) == 0){   // Exit program with code 1 if no .txt file was found
        fprintf(stderr, "program: missing '--infile <filename> [--sort]'\n");
        exit(1);
    }

    FILE* input_file = fopen(argv[filename_index], "r");    //if file name was found, open file

    if(input_file == NULL){     // Exit program with code 2 if unable to open file
        fprintf(stderr, "unable to open '%s' for reading\n", argv[filename_index]);
        exit(2);
    }
    
    gen_node* head = NULL; // Create a NULL head node, will store all data linked to this head.

    size_t size = 0;

    char* line = NULL;
    while((getline(&line, &size, input_file)) >= 0){
        
        for (char *c = line; *c; c++)   // Set all non-alpha chars to ' ' so I can tokenize
            if (!isalpha(*c))           // without non-alpha chars. 
                *c = ' ';
            
        char* token = strtok(line, " ");
        while(token != NULL){
            insert_gen((&head), strlen(token), token);  // Insert word tokenized into my list
                                                        // (inserts it into correct spot)
            token = strtok(NULL, " ");
        }
    }
    
    if (!check_sorted(argv, argc))  // If --sorted not in args
        print_no_sort(head);        // print words in ascending alpha order
    else
        print_sort(head);           // If --sorted in args
                                    // print words in descending alpha order
    free(line);         // Free line
    free_list(&head);   // Free my whole linked list
    
    fclose(input_file);
    
    return 0;
}
