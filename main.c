//
//  main.c
//  C_linkedlist
//
//  A basic linked list implementation in C.
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

//search the list for the dvalue and return the index if found
//return -1 if not found
int search_by_value(struct node **head, int dvalue) {
    struct node *current = *head;
    int index = 1;
    while(current->next != NULL) {
        if(current->value == dvalue) {
            return index;
        }else {
            current = current->next;
        }
        index++;
    }
    //the element could still be the last item
    if(current->value == dvalue) {
        return index;
    }
    
    //if we got here, we did not find the value
    return -1;
}

int insert_at_front(struct node **head, int newval) {
    struct node *newnode = malloc(sizeof(struct node));
    if(newnode == NULL) {
        assert(newnode != NULL);
        //if we failed to allocate from the heap, bail out.
    }
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

int insert_at_tail(struct node **head, int newval) {
    struct node *newnode = malloc(sizeof(struct node));
    newnode->value = newval;
    if(newnode == NULL) {
        assert(newnode != NULL);
        //if we failed to allocate from the heap, bail out.
    }
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

//inserts a node into the list at position "newval", first position is 1.
//index of 0 not allowed.
int insert_at_index(struct node **head, int newval, int index) {
    if(index<1) {
        fputs("Index to insert_at_index is < 1, index out of bounds!\n", stderr);
        return -1;
    }else if(index==1) {
        return insert_at_front(head, newval);
    }else {
        struct node *current = *head;
        //initialize i to the first position (index 1 is first position)
        int i = 1;
        struct node *newnode = malloc(sizeof(struct node));
        if(newnode == NULL) {
            assert(newnode != NULL);
            //if we failed to allocate from the heap, bail out.
        }
        newnode->value = newval;
        while(current->next->next != NULL && i < (index-1)){
            //we need to look one ahead so we have the pointer
            //to the element that must be inserted in front of
            current = current->next;
            i++;
        }
        //special case:  if the index was the equal to or higher than the tail
        //in this case, we would stop one element too soon.
        if(i<(index-1)) current = current->next;
        
        struct node * temp;
        temp = current->next;
        current->next = newnode;
        newnode->next = temp;
    }
    return 0;
    
}

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

int delete_tail(struct node **head) {
    //handle the empty list case
    if(*head == NULL) {
        fputs("Delete tail called on empty list!\n", stderr);
        return -1;
    }
    struct node *current = *head;
    while(current->next->next != NULL) {
        //have to look one ahead to update the pointer to the last element
        current = current->next;
    }
    struct node * temp = current->next;
    current->next = NULL;
    free(temp);
    return 0;
}

int delete_at_index(struct node **head, int index) {
    if(index<1) {
        fputs("Index to delete_at_index is < 1, index out of bounds!\n", stderr);
        return -1;
    }else if(index==1) {
        return delete_head(head);
    }else {
        struct node *current = *head;
        int i=1;
        while(current->next->next != NULL && i < (index-1)){
            //we need to look one ahead so we have the pointer
            //to the element that must be updated in the deletion
            current = current->next;
            i++;

        }
        //special case:  if the index was the equal to or higher than the tail
        //in this case, we would stop one element too soon.
        if(i<(index-1)) {
            return delete_tail(head);
        }
        
        //Regular case, current points to the item before the item to be deleted
        struct node *temp = current->next->next;
        free(current->next);
        current->next = temp;
    }
    return 0;
}

int delete_by_value(struct node **head, int value) {
    int delindex = search_by_value(head, value);
    if(delindex != -1) {
        return delete_at_index(head, delindex);
    }
    return -1;
}


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
    
    struct node *head=NULL;
    struct node **hhead = &head;
    insert_at_front(hhead, 3);
    insert_at_front(hhead, 2);
    insert_at_front(hhead, 1);
    insert_at_tail(hhead, 4);
    insert_at_tail(hhead, 5);
    insert_at_tail(hhead, 6);
    printf("List after 6 insertions is:\n");
    print_list(hhead);
    printf("\n");
    
    reverse_list(hhead);
    printf("List after reversal: \n");
    print_list(hhead);
    printf("\n");
    
    reverse_list(hhead);
    printf("List after reversal: \n");
    print_list(hhead);
    printf("\n");
    
    insert_at_index(hhead, 666, 5);
    printf("List after insert val of 666 at index 5 is:\n");
    print_list(hhead);
    printf("\n");
    
    insert_at_index(hhead, 888, 7);
    printf("List after insert val of 888 at index 7 is:\n");
    print_list(hhead);
    printf("\n");
    
    int si = search_by_value(hhead, 666);
    printf("Search by value for value 666 returned: %d\n", si);
    print_list(hhead);
    printf("\n");
    
    delete_by_value(hhead, 666);
    printf("List after delete by value of 666 is:\n");
    print_list(hhead);
    printf("\n");
        
    si = search_by_value(hhead, 2);
    printf("Search by value for value 2 returned: %d\n", si);
    print_list(hhead);
    printf("\n");
    
    si = search_by_value(hhead, 3);
    printf("Search by value for value 3 returned: %d\n", si);
    print_list(hhead);
    printf("\n");
    
    si = search_by_value(hhead, 777);
    printf("Search by value for value 777 returned: %d\n", si);
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
