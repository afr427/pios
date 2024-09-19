#ifndef LIST_H
#define LIST_H

struct list_element {

	struct list_element *next;
	int data;

};

struct list_element c = { NULL, 0};
struct list_element b = { &c,   0};
struct list_element a = { &b,   0};
struct list_element *head = &a;

list_add(struct list_element **list_head, struct list_element *new_element);
list_remove(struct list_element **list_head, struct list_element *remove_element);

#endif

