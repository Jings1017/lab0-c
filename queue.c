#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;
    list_ele_t *node = q->head;
    while (node) {
        list_ele_t *tmp = node->next;
        free(node->value);
        free(node);
        node = tmp;
        q->size--;
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false;

    list_ele_t *newh = malloc(sizeof(list_ele_t));

    if (!newh)
        return false;

    newh->value = malloc(strlen(s) + 1);
    if (!newh->value) {
        free(newh);
        return false;
    }

    strncpy(newh->value, s, strlen(s) + 1);

    if (!q->tail)
        q->tail = newh;
    newh->next = q->head;
    q->head = newh;

    q->size++;

    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;

    list_ele_t *newh = malloc(sizeof(list_ele_t));

    if (!newh)
        return false;

    newh->next = NULL;

    newh->value = malloc(strlen(s) + 1);
    if (!newh->value) {
        free(newh);
        return false;
    }

    strncpy(newh->value, s, strlen(s) + 1);

    if (!q->head)
        q->head = newh;
    else
        q->tail->next = newh;
    q->tail = newh;
    q->size++;

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head)
        return false;

    list_ele_t *node = q->head;
    q->head = q->head->next;

    if (sp) {
        strncpy(sp, node->value, bufsize);
        sp[bufsize - 1] = '\0';
    }

    free(node->value);
    free(node);

    if (!q->head)
        q->tail = NULL;

    q->size--;

    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q || !q->head)
        return 0;
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || !q->head || q->size <= 1)
        return;

    list_ele_t *rev_list = q->head;
    list_ele_t *aux_list = q->head->next;

    q->tail = rev_list;
    rev_list->next = NULL;

    while (aux_list) {
        list_ele_t *tmp = aux_list;
        aux_list = aux_list->next;
        tmp->next = rev_list;
        rev_list = tmp;
    }

    q->head = rev_list;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
list_ele_t *merge(list_ele_t *l1, list_ele_t *l2)
{
    if (!l2)
        return l1;
    if (!l1)
        return l2;

    list_ele_t *curr, *head;

    /* choose head */
    if (strcmp(l1->value, l2->value) < 0) {
        head = l1;
        l1 = l1->next;
    } else {
        head = l2;
        l2 = l2->next;
    }
    curr = head;

    while (l1 && l2) {
        if (strcmp(l1->value, l2->value) < 0) {
            curr->next = l1;
            l1 = l1->next;
        } else {
            curr->next = l2;
            l2 = l2->next;
        }
        curr = curr->next;
    }
    if (l1)
        curr->next = l1;
    if (l2)
        curr->next = l2;
    return head;
}

list_ele_t *mergeSort(list_ele_t *head)
{
    if (!head || !head->next)
        return head;

    /* divide two parts */
    list_ele_t *fast = head->next;
    list_ele_t *slow = head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    fast = slow->next;
    slow->next = NULL;

    list_ele_t *l1 = mergeSort(head);
    list_ele_t *l2 = mergeSort(fast);

    return merge(l1, l2);
}

void q_sort(queue_t *q)
{
    if (!q || !q->head || q->size <= 1)
        return;
    q->head = mergeSort(q->head);
    list_ele_t *tmp = q->head;
    while (tmp->next) {
        tmp = tmp->next;
    }
    q->tail = tmp;
}