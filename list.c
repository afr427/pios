#include <stdio.h>
#include <stdlib.h>

void list_add(struct list_element **list_head, struct list_element *new_element) {
    if (*list_head == NULL) {
        // If the list is empty, make the new element the head
        *list_head = new_element;
        new_element->next = NULL;
    } else {
        // Add the new element at the beginning of the list
        new_element->next = *list_head;
        *list_head = new_element;
    }
}

void list_remove(struct list_element **list_head, struct list_element *element) {
    if (*list_head == NULL) {
        // List is empty, nothing to remove
        return;
    }

    if (*list_head == element) {
        // If the element to remove is the head
        *list_head = (*list_head)->next;
        return;
    }

    struct list_element *current = *list_head;
    struct list_element *prev = NULL;

    // Traverse the list to find the element
    while (current != NULL && current != element) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        // Element not found in the list
        return;
    }

    // Remove the element by updating the next pointer of the previous element
    prev->next = current->next;
}


