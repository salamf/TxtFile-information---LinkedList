#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

typedef struct word_node {

    char* word;
    struct word_node* next;


}word_node;

typedef struct gen_node {

    int word_len;

    struct word_node* word_list_head;
    int num_words;

    struct gen_node* next;

}gen_node;

// Reverses a linked list
// used to print the order of the words
// in decending alpha order since they're 
// stored in increasing alpha order,
void reverse_list(word_node** head){
    word_node* prev = NULL;
    word_node* curr = *head;
    word_node* next = NULL;

    while(curr != NULL){
        next = curr -> next;
        
        curr -> next = prev;

        prev = curr;
        curr = next;
    }    
    *head = prev;
}

// Given the head of the general list, and the input word's
// word length, this function checks if the current length
// already exists in the list
int word_len_exists(gen_node* head, int word_len){
    
    gen_node* curr = head;
    while(curr != NULL){
        if(curr -> word_len == word_len) return 1;
        curr = curr -> next;
    }
    return 0;
}

// Given the head of a word list and an input word
// this function checks if the input word already exisits
// in the list
int word_exists(word_node* head, char* word){
    
    word_node* curr = head;
    while(curr != NULL){
        if(strcmp(curr -> word, word) == 0) return 1;
        curr = curr -> next;
    }
    return 0; 
}

// Used when the first letter of 2 words is =
// This function recursively checks the following letters
// After the first one until they are not equal, and returns
// A number that decides where to store the word
int check_next_letter(char* word, word_node* node, int index){
    if (*(word + index) < *((node -> word) + index))
        return 1;
    else if (*(word + index) == *((node -> word) + index))
       return check_next_letter(word, node, index + 1);
    else
        return 0;
}

// Given a head to a word_list, and an input word,
// this function inserts the word (if it doesn't already exist)
// into the word_list given in increasing alpha order
void insert_word(word_node** head, char* word){  

    if(!word_exists(*head, word)){

        word_node* new_word = (word_node*)malloc(sizeof(word_node));
         if(new_word == NULL){
            fprintf(stderr, "Memory couldn't be allocated, line 91 in linklist.h");
            exit(3);
        }        

        new_word -> word = (char*)calloc(1, strlen(word) * sizeof(char) + 1);    
        strncpy(new_word -> word, word, strlen(word));
        new_word -> next = NULL;

        if (*head == NULL){
            *head = new_word;
            return;
        }

        if (check_next_letter(word, *head, 0) == 1){
            new_word -> next = *head;
            *head = new_word;
            return;
         }

        word_node* curr = (*head) -> next;
        word_node* prev = *head;
        while(curr != NULL){

            if(check_next_letter(word, curr, 0) == 1){
                prev -> next = new_word;
                new_word -> next = curr;
                return;
            }
 
            prev = curr;
            curr = curr -> next;
        }
    
        prev -> next = new_word;
    }
}

// Creates a new node if the input word length doesn't already
// exist in the list. Inserts the node in increasing length order.
// If it already exists, it just adds the word into the existent node
void insert_gen(gen_node** head, int word_length, char* word){
    
    if(!word_len_exists(*head, word_length)){

        gen_node* input_node = (gen_node*)calloc(1, sizeof(gen_node));  //allocate space then store
        if(input_node == NULL){                                         //neccessary data into input_node
            fprintf(stderr, "Memory couldn't be allocated, line 137 in linklist.h");
            exit(3);
        }        
 
        input_node -> word_len = word_length;
        insert_word(&(input_node -> word_list_head), word);   
        input_node -> num_words = 1;
        input_node -> next = NULL;        

        if (*head == NULL){
            *head = input_node;
            return;
        }

        if(input_node -> word_len < (*head) -> word_len){
            input_node -> next = *head;
            *head = input_node;
            return;
        }
        
        gen_node* curr = (*head) -> next;
        gen_node* prev = *head;
        while(curr != NULL){
            
            if (input_node -> word_len <  curr -> word_len){
                prev -> next = input_node;
                input_node -> next = curr;
                return;
            }
            prev = curr;
            curr = curr -> next;
        }

        prev -> next = input_node;
        return;
    }

    gen_node* curr = *head;
    while(curr -> word_len != word_length){
        curr = curr -> next;
    }

    insert_word(&(curr -> word_list_head), word);
    curr -> num_words++;
}
