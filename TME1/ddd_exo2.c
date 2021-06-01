#include<stdio.h>
#include<stdlib.h>

struct mystruct {
    int value;
    struct mystruct *next;
};

struct mystruct* insert_list (struct mystruct *list, int value) {
    struct mystruct *new;
    new = malloc(sizeof(struct mystruct));

    new->value = value;
    new->next = list;

    return new;
}

int main(void) {
    struct mystruct *head;

    head = insert_list(NULL, 5);
    head = insert_list(head, 3);    
    head = insert_list(head, 6);    
    head = insert_list(head, 10);    
    head = insert_list(head, 17);    

    struct mystruct *scan = head;
    do {
        printf("value=%d\n", scan->value);
        scan = scan->next;
    } while(scan);

    return 0;
}
