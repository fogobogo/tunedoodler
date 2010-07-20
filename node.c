void
init_nodes(node **head, node **cur, node **new)
{
    (*head) = (node *)malloc(sizeof(node));
    (*head)->next = NULL;
    (*cur) = (*head);
}

void
store_node(SDL_Event event, SDL_Surface *display, pompface ui, point offset, node **cur, node **new)
{
    (*cur)->x = event.button.x / ui.icon->h;
    (*cur)->y = (event.button.y - offset.y) / ui.icon->h;
    (*cur)->i = ui.active;

    printf("cur x: %d\tcur y: %d\n",(*cur)->x,(*cur)->y);
    (*new) = (node *)malloc(sizeof(node));
    (*new)->next = NULL;
    /* link nodes */
    (*cur)->next = (*new);
    (*cur) = (*new);
}

void
free_nodes(node **head, node **cur)
{
    (*cur) = (*head);
    while((*cur)->next != NULL) {
        (*head) = (*cur)->next;
        free((*cur));
        (*cur) = (*head);
    }
}

void
print_nodes(node **head, node **cur)
{
    (*cur) = (*head);
    while((*cur)->next != NULL) {
        printf("cur x: %d\tcur y: %d\n",(*cur)->x,(*cur)->y);
        (*cur) = (*cur)->next;
    }
}

node*
merge_nodes(node *left, node *right)
{
    node *sort;
    node *cur;

    /* if either linked list is empty return the other */
    if(left == NULL) { return(right); }
    if(right == NULL) { return(left); }

    /* compare both halfs that were given as argument */
    /* [comparsion starts here.] */
    if(left->x <= right->x) {
        sort  = left;
        /* move to next node */
        left = left->next;
    }

    else {
        sort = right;
        right = right->next;
    } 
    /* [comparsion ends here] */

    /* since sort is now the first node of the list with the sorted nodes.. */
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

    /* if either half ran out of nodes before the other do this */
    if(left != NULL) { cur->next = left; }
    if(right != NULL) { cur->next = right; }

    /* ok. the new list(sort) is sorted and assembled now. return it */
    return(sort);

}


node*
msort_nodes(node *head, int n)
{
    int i;
    node *left;
    node *right;

    node *tail;

    /* count the total number of nodes */
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
        left = msort_nodes(left,(n/2));
        right = msort_nodes(right,(n - (n/2)));

        head = merge_nodes(left,right);

    }

    return(head);
}












