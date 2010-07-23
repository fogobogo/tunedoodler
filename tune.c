
/* create first node of the linked list */
void
init_tune(tune **head, tune **cur, tune **new)
{
    (*head) = (tune *)malloc(sizeof(tune));
    (*head)->next = NULL;
    (*cur) = (*head);
}

/* initialize the members of the node struct */
void
store_tune(tune **cur, int x, int y, int i)
{
    if(i > BUTTON_NONE) {
        (*cur)->x = x;
        (*cur)->y = y;
        (*cur)->i = i;
    }
}

/* create a new node */
void
create_tune(tune **cur, tune **new)
{
    (*new) = (tune *)malloc(sizeof(tune));
    (*new)->next = NULL;
    /* link nodes */
    (*cur)->next = (*new);
    (*cur) = (*new);
}

/* free all nodes */
void
free_tune(tune **head, tune **cur)
{
    (*cur) = (*head);
    while((*cur)->next != NULL) {
        (*head) = (*cur)->next;
        free((*cur));
        (*cur) = (*head);
    }
}

/* print all nodes and the values of its members */
void
print_tune(tune **head, tune **cur)
{
    (*cur) = (*head);
    while((*cur)->next != NULL) {
        printf("cur x: %d\tcur y: %d\n",(*cur)->x,(*cur)->y);
        (*cur) = (*cur)->next;
    }
}


/* TODO: add removal of dupes */
/* what a beast */
tune*
merge_tune(tune *left, tune *right)
{
    tune *sort;
    tune *cur;

    /* if either linked list is empty return the other */
    if(left == NULL) { return(right); }
    if(right == NULL) { return(left); }

    /* compare both halfs that were given as argument */
    /* [comparsion starts here.] */
    if(left->x <= right->x) {
        sort  = left;
        /* move to next tune */
        left = left->next;
    }

    else {
        sort = right;
        right = right->next;
    } 
    /* [comparsion ends here] */

    /* since sort is now the first tune of the list with the sorted tunes.. */
    sort->next = NULL;
    cur = sort;

    /* now repeat until either half of the lists given as argument are emtpy */
    while(left != NULL && right != NULL) {
        /* compare again */
        /* [comparsion starts here.] */
        if(left->x <= right->x) {
            cur->next = left;
            left = left->next;
        }

        else {
            cur->next = right;
            right = right->next;
        }
        /* [comparsion ends here] */

        /* properly terminate list */
        cur = cur->next;
        cur->next = NULL;
    }

    /* if either half ran out of tunes before the other do this */
    if(left != NULL) { cur->next = left; }
    if(right != NULL) { cur->next = right; }

    /* ok. the new list(sort) is sorted and assembled now. return it */
    return(sort);

}

/* mergesort nodes */
tune*
msort_tune(tune *head, int n)
{
    int i;
    tune *left;
    tune *right;

    tune *tail;

    /* count the total number of tunes */
    /*
    while(head->next != NULL) {
        n++;
        head = head->next;
    }
    */


    /* stop splitting up nodes if theres only one... */
    if(n > 1) {

        /* split the nodes in half  */
        /* first left... */
        left = head;

        /* move to the node in the middle */
        tail = head;
        for(i=0;i<((n/2) - 1);i++) {
            tail = tail->next;
        }
        /* the next node will be the start of the second half */
        right = tail->next;
        /* now unlink both halfs from each other */
        tail->next = NULL;

        /* now do this over and over again with recursion */
        /* at least until we hit the condition of the if statement */
        left = msort_tune(left,(n/2));
        right = msort_tune(right,(n - (n/2)));

        head = merge_tune(left,right);

    }

    return(head);
}

/* remove a node */
void
delete_tune(int rx, int ry, int *n, tune **head, tune **cur)
{
    tune *prev;
    int i = 0;
    
    (*cur) = (*head);
    while((*cur)->next != NULL) {

        if((*cur)->x == rx
        && (*cur)->y == ry) {

            /* TODO: handle exception in case first node will be deleted */
            prev->next = (*cur)->next;
            free((*cur));
            printf("removed tune %d\n",i);
            (*n)--;
        }
        else {
            prev = (*cur);
            (*cur) = (*cur)->next;
        }
        i++;
    }
}

/* write members of the linked list to a file */
void
save_tune(tune **head, tune **cur)
{
    FILE *fd;

    fd = fopen("save.tune","w");
    (*cur) = (*head);
    while((*cur)->next != NULL) {
        fprintf(fd,"%d %d %d\n",(*cur)->x,(*cur)->y,(*cur)->i);
        (*cur) = (*cur)->next;
    }

    printf("saved.\n");
    fclose(fd);
}

void
load_tune(tune **head, tune **cur)
{
    FILE *fd;

    fd = fopen("save.tune","r");
    fclose(fd);
}
