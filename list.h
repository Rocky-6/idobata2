#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define L_USERNAME 15

typedef struct _imember {
  char username[L_USERNAME];
  int number;
  struct _imember *next;
} *imember;

typedef struct {
  imember top;
} list;

imember make_cell(char *username, int number, imember member);
list *make_list(void);
void delete_cell(imember member);
void delete_list(list *ls);
imember nth_cell(imember member, int n);
int nth(list *ls, int n, bool *err);
bool insert_nth(list *ls, int n, char name[], int x);
bool delete_nth(list *ls, int n);
bool push(list *ls, char name[], int x);
void print_list(list *ls);

#endif
