//
//  main.c
//  C_linkedlist
//
//  A basic linked list implementation in C.
//
//
//
//  Created by Christopher Gleeson on 12/22/15.
//  Copyright © 2015 Christopher Gleeson. All rights reserved.
//
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct node {
    int value;
    struct node *next;
};


//Delete the list entirely.
//Returns: -1 if the list was already empty, else returns 0 on success.
int delete_list(struct node **head) {
    //case1:  empty list
    if(*head == NULL) {
        fputs("Delete called on empty list!\n", stderr);
        return -1;
    }else{ //the list is not empty
        struct node *current = *head;
        struct node *temp;
        while(current != NULL) {
            temp = current;
            current = current->next;
            free(temp);
        }
        //reassign the head pointer to null
        *head = NULL;
    }
    return 0;
}

//Search the list for the dvalue and assign the index into findex
//Return -1 on failure and 0 on success.
int search_by_value(struct node **head, int dvalue, int *findex) {
    struct node *current = *head;
    int index = 1;
    while(current->next != NULL) {
        if(current->value == dvalue) {
            *findex = index;
            return 0;
        }else {
            current = current->next;
        }
        index++;
    }
    //the element could still be the last item
    if(current->value == dvalue) {
        *findex = index;
        return 0;
    }
    
    //if we got here, we did not find the value
    return -1;
}

//Inserts to the front of the list, updating pointer to head.
//Asserts on malloc failure and returns 0 on success
int insert_at_front(struct node **head, int newval) {
    struct node *newnode = malloc(sizeof(struct node));
    assert(newnode != NULL);

    newnode->value = newval;
    //case 1:  insert into an empty list
    if(*head == NULL) {
        *head = newnode;
        newnode->next = NULL;
    }else{ //there is at least one node in the list already
        struct node *temp;
        temp = *head;
        *head = newnode;
        newnode->next = temp;
    }
    return 0;
}

//Inserts to the tail of the list, updating pointer to head.
//Asserts on malloc failure and returns 0 on success
int insert_at_tail(struct node **head, int newval) {
    struct node *newnode = malloc(sizeof(struct node));
    assert(newnode != NULL);

    newnode->value = newval;
    //case 1:  insert into empty list
    if(*head == NULL) {
        return insert_at_front(head, newval);
    }else{ //there is at least one node in the list already
        struct node *current = *head;
        while(current->next != NULL) {
            current = current->next;
        }//we stop when current is pointing to the last item
        current->next = newnode;
        newnode->next = NULL;
    }
    return 0;
}

//Inserts a node into the list at index "newval" where index is from
//0 to N-1 for N elements.
//Returns: -1 if the index was below 0, 0 on success
int insert_at_index(struct node **head, int newval, int index) {
    if(index < 0) {
        fputs("Index to insert_at_index is < 0, index out of bounds!\n", stderr);
        return -1;
    }else if(index == 0) {
        return insert_at_front(head, newval);
    }else {
        struct node *current = *head;
        struct node *prev = current;
        //initialize i to the first position
        int i = 0;
        struct node *newnode = malloc(sizeof(struct node));
        assert(newnode != NULL);

        newnode->value = newval;
        while(current->next != NULL && i < (index - 1)){
            //If the index is higher than the size of the list
            //insertion happens at the tail.
            prev = current;
            current = current->next;
            i++;
        }
        //Insert the new node at the position of current
        //which is either index if index is in the list, or
        //is at the tail
        prev->next = newnode;
        newnode->next = current;
    }
    return 0;
    
}

//Deletes the head node, making the next node (if it exists)
//the new head.
//Returns: -1 on error (empty list) or 0 on success.
int delete_head(struct node **head) {
    //handle the empty list case
    if(*head == NULL) {
        fputs("Delete head called on empty list!\n", stderr);
        return -1;
    }
    struct node *temp;
    temp = *head;
    *head = (*head)->next;
    free(temp);

    return 0;
}

//Deletes the tail node.
//Returns: -1 on error (empty list) or 0 on success.
int delete_tail(struct node **head) {
    //handle the empty list case
    if(*head == NULL) {
        fputs("Delete tail called on empty list!\n", stderr);
        return -1;
    }
    struct node *current = *head;
    struct node *prev = current;
    while(current->next != NULL) {
        prev = current;
        current = current->next;
    }
    prev->next = NULL;
    free(current);
    return 0;
}

//Deletes the node at specified index if index is in the list.
//Returns:  -1 on error, 0 on success
int delete_at_index(struct node **head, int index) {
    if(index < 0) {
        fputs("Index to delete_at_index is < 0, index out of bounds!\n", stderr);
        return -1;
    }else if(index==1) {
        return delete_head(head);
    }else {
        struct node *current = *head;
        struct node *prev = current;
        int i=0;
        while(current->next != NULL && i < (index-1)){
            prev = current;
            current = current->next;
            i++;
        }
        prev->next = current->next;
        current->next = NULL;
        free(current);
    }
    return 0;
}

//Deletes the node at specified index if index is in the list.
//Returns:  -1 on error, 0 on success
int delete_by_value(struct node **head, int value) {
    int index = 0;
    int *iptr = &index;
    int result = search_by_value(head, value, iptr);
    if(result != -1 && iptr != NULL) {
        return delete_at_index(head, *iptr);
    }
    return -1;
}

//Prints the list
void print_list(struct node **head) {
    if(*head == NULL) {
        printf("List is empty!\n");
        return;
    }
    struct node *current = *head;
    int i=1;
    while(current != NULL) {
        printf("Value of node %d is %d\n", i, current->value);
        current = current->next;
        i++;
    }
}

//Reverses the list and updates head pointer to point
//to the new list.
//Returns 0 on success, empty list, singleton list.
int reverse_list(struct node **head) {
    struct node *current = *head;
    struct node *newhead = NULL;
    struct node *temp = NULL;
    //case 1:  if the list is empty, or has only one element,
    //return success
    if(*head == NULL || current->next == NULL) {
        return 0;
    }
    while(current != NULL) {
        //store new head into temp
        temp = current->next;
        //detach current to new list
        current->next = newhead;
        //update head to next element in list
        *head = temp;
        //set the new head of the new list
        newhead = current;
        //move to next position in the list
        current = *head;
    }
    //finally, set *head to point to the newly reversed list
    *head = newhead;
    return 0;
}

int main(int argc, const char * argv[]) {
    //This main is just for testing purposes so you can exercise the code
    
    int result = -1;
    
    struct node *head=NULL;
    struct node **hhead = &head;
    
    //Populate the list
    insert_at_front(hhead, 3);
    insert_at_front(hhead, 2);
    insert_at_front(hhead, 1);
    insert_at_tail(hhead, 4);
    insert_at_tail(hhead, 5);
    insert_at_tail(hhead, 6);
    printf("List after 6 insertions is:\n");
    print_list(hhead);
    printf("\n");
    
    //Reverse the list
    reverse_list(hhead);
    printf("List after reversal: \n");
    print_list(hhead);
    printf("\n");
    
    //Reverse the list again
    reverse_list(hhead);
    printf("List after reversal: \n");
    print_list(hhead);
    printf("\n");
    
    //Some insertions by index
    insert_at_index(hhead, 666, 4);
    printf("List after insert val of 666 at index 4 is:\n");
    print_list(hhead);
    printf("\n");
    insert_at_index(hhead, 888, 7);
    printf("List after insert val of 888 at index 7 is:\n");
    print_list(hhead);
    printf("\n");
    
    //pointer to receive the lookup return index.
    int returnindex = 0;
    int *indxptr = &returnindex;
    
    
    search_by_value(hhead, 666, indxptr);
    
    printf("Search by value for value 666 returned: %d\n", *indxptr);
    print_list(hhead);
    printf("\n");
    
    delete_by_value(hhead, 666);
    printf("List after delete by value of 666 is:\n");
    print_list(hhead);
    printf("\n");
        
    search_by_value(hhead, 2, indxptr);
    printf("Search by value for value 2 returned: %d\n", *indxptr);
    print_list(hhead);
    printf("\n");
    
    search_by_value(hhead, 3, indxptr);
    printf("Search by value for value 3 returned: %d\n", *indxptr);
    print_list(hhead);
    printf("\n");
    
    //This one we expect to fail the search, so we should catch the
    //returned error and do something.
    result = search_by_value(hhead, 777, indxptr);
    if(result != 0){
        printf("Search by value for value 777 returned -1, value does not exist!\n");
    }else{
        printf("Search by value for value 777 returned: %d\n", *indxptr);
    }
    print_list(hhead);
    printf("\n");
    
    delete_head(hhead);
    printf("List after delete head is:\n");
    print_list(hhead);
    printf("\n");
    
    delete_tail(hhead);
    printf("List after delete tail is:\n");
    print_list(hhead);
    printf("\n");

    printf("Delete at index called with index 3...\n");
    delete_at_index(hhead, 3);
    printf("List after delete at index of 3 is:\n");
    print_list(hhead);
    printf("\n");
    
    printf("Delete at index called with index 2...\n");
    delete_at_index(hhead, 2);
    printf("List after delete at index of 2 is:\n");
    print_list(hhead);
    printf("\n");
    
    printf("Delete at index called with index 1...\n");
    delete_at_index(hhead, 1);
    printf("List after delete at index of 1 is:\n");
    print_list(hhead);
    printf("\n");
    
    printf("Delete at index called with index 1...\n");
    delete_at_index(hhead, 1);
    printf("List after delete at index of 1 is:\n");
    print_list(hhead);
    printf("\n");
    
    delete_list(hhead);
    printf("List after deletion of list is:\n");
    print_list(hhead);
    
    return 0;
}













