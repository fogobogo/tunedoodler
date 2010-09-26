/*
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *      
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *      
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

/* create first node */
void
init_tune(tune_t **head, tune_t **cur, tune_t **new)
{
    (*head) = (tune_t *)malloc(sizeof(tune_t));
    (*head)->next = NULL;
    (*cur) = (*head);
}

/* save to the members of the node struct */
void
store_tune(tune_t **cur, int x, int y, int i, int page)
{
    if(i > BUTTON_NONE) {
        (*cur)->x = x + (page * 25);
        (*cur)->y = y; 
        (*cur)->i = i;
        printf("x: %d y: %d i: %d\n",x,y,i);
        printf("x: %d y: %d i: %d\n",(*cur)->x,(*cur)->y,(*cur)->i);
    }
}

/* create a new node */
void
create_tune(tune_t **cur, tune_t **new)
{
    (*new) = (tune_t *)malloc(sizeof(tune_t));
    (*new)->next = NULL;
    /* link nodes */
    (*cur)->next = (*new);
    (*cur) = (*new);
}

/* free all nodes */
void
free_tune(tune_t **head, tune_t **cur)
{
    (*cur) = (*head);
    while((*cur)->next != NULL) {
        (*head) = (*cur)->next;
        free((*cur));
        (*cur) = (*head);
    }
}

int
count_tune(tune_t *head, tune_t *cur)
{
    int n = 0; /* node count */
    cur = head;
    while(cur->next != NULL) {
        n++;
        cur = cur->next;
    }

    return(n);
}

/* print all nodes and the values of its members */
void
print_tune(tune_t **head, tune_t **cur)
{
    (*cur) = (*head);
    while((*cur)->next != NULL) {
        printf("cur x: %d\tcur y: %d\n",(*cur)->x,(*cur)->y);
        (*cur) = (*cur)->next;
    }
}


/* TODO: add removal of dupes */
/* what a beast */
tune_t*
merge_tune(tune_t *left, tune_t *right)
{
    tune_t *sort;
    tune_t *cur;

    /* if either half of the list is empty return the other */
    if(left == NULL) { return(right); }
    if(right == NULL) { return(left); }

    /* TODO: make function of this ? */
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

/* perform mergesort on nodes */
tune_t*
msort_tune(tune_t *head, int n)
{
    int i;
    tune_t *left;
    tune_t *right;

    tune_t *tail;

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
        /* ...then right */
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
delete_tune(tune_t **head, tune_t **cur, int *n, int rx, int ry)
{
    tune_t *prev;
    int i = 0;
    
    (*cur) = (*head);
    while((*cur)->next != NULL) {

        /* if the current relative coordinates match a node */
        if((*cur)->x == rx
        && (*cur)->y == ry) {

            /* in case the first node is deleted move the head */
            if((*cur) == (*head)) {
                (*head) = (*head)->next;
            }
            /* else relink */
            else { prev->next = (*cur)->next; }
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
save_tune(tune_t **head, tune_t **cur)
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
load_tune(tune_t **head, tune_t **cur)
{
    FILE *fd;

    fd = fopen("save.tune","r");
    fclose(fd);
}


void
play_tune(tune_t *cur, tune_t *head, int n, sound_t *sounds, int tempo)
{
    int pos; /* position */
    float pitch[10];

    pitch[0] = 5.0;
    pitch[1] = 4.5;
    pitch[2] = 4.0;
    pitch[3] = 3.5;
    pitch[4] = 3.0;
    pitch[5] = 2.5;
    pitch[6] = 2.0;
    pitch[7] = 1.5;
    pitch[8] = 1.0;
    pitch[9] = 0.5;
    pitch[10] = 0;

    /* start at the beginning */
    pos = 0;
    cur = head;

    while(n != 0) {
        while(cur->x == pos) {
            /* PLAY ROUTINE COMES HERE */
            /* pitch = abs(((float)(cur->y) - 10)) * (0.2); */
            printf("pitch: %.2f\n",pitch[cur->y]);
            /* fill channel with the payload (sound) we want to pass to the output (soundcard) */
            process_audio(&voice[cur->y], &sounds[cur->i], 1, pitch[cur->y]);

            cur = cur->next;
            n--; /* count down nodes */
            printf("n: %d\n",n);
        }

        SDL_Delay(tempo);
        pos++;
    }
}

