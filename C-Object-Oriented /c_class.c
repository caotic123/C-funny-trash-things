#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "p99/p99.h"

#define FUNCTIONOBJECT__L(t, type_return, class_object, esc, ...)              \
  (void *)({                                                                   \
    type_return t(class class_object __VA_ARGS__) esc;                         \
    t;                                                                         \
  })
#define message(type_return, esc, ...)                                         \
  FUNCTIONOBJECT__L(P99_UNIQ(METHOD_FUNCTION_L), type_return, this, esc,       \
                    __VA_ARGS__)
#define constructor(esc, ...)                                                  \
  new_method(self, "constuctor",                                               \
             FUNCTIONOBJECT__L(P99_UNIQ(METHOD_FUNCTION_L), class, this, {     \
               (esc);                                                          \
               return this;                                                    \
             }, __VA_ARGS__));
#define create_method(y, z) new_method(self, "" #y, z)
#define args(...) , __VA_ARGS__

#define eval(func_lambda, x) ((void *(*)(void *, void *))func_lambda)(env_, x)

#define type(x) sizeof(x)
#define val_(x, y)                                                             \
  ({                                                                           \
    y *t = malloc(sizeof(y));                                                  \
    (*t) = x;                                                                  \
    t;                                                                         \
  })
#define method(return, class, func, ...)                                       \
  (((return (*)())call(class, "" #func)))(class __VA_ARGS__)
#define call_constructor(class, ...)                                           \
  (((void **(*)())call(class, "constuctor")))(class __VA_ARGS__)
#define set(x, y, d) _set(this, "" #x, val_(y, d))
#define get(y, c) *((y *)_get(this, "" #c))
#define object(x, y) obj(self, "" #x, y)
#define new_class() _new_class(class__s)
#define Template_Class(esc)                                        \
  ({                                                                           \
    class self = new_class();                                                  \
    esc self;                                                                  \
  });

#define new (type_object_name, f) void **type_object_name = f;
#define class void **
#define int_ *(int *)
#define s_int_ *(signed int *)
#define u_int_ *(unsigned int *)
#define char_ *(char *)
#define float_ *(float *)
#define double_ *(double *)

struct obj_type {
  int *obj;
  void **st_obj;
  void **method;
  char **method_name;
  char **obj_name;
  void *constr;
  int len;
  int m_len;
};

struct c_ {
  void ***class_;
  int len;
} class__s;

void p_class() {
  class__s.class_ = malloc(sizeof(class));
  class__s.len = 0;
}

void new_obj(struct obj_type *o_, char *_, int obj_len) {
  if (o_->len < 1) {
    o_->obj = malloc(sizeof(struct obj_type));
    o_->st_obj = malloc(sizeof(void *));
    o_->obj_name = malloc(sizeof(char *) * sizeof(obj_len));
    o_->obj[o_->len] = obj_len;
  } else {
    o_->obj = realloc(o_->obj, sizeof(struct obj_type) * o_->len);
    o_->st_obj = realloc(o_->st_obj, sizeof(void *) * o_->len);
    o_->obj_name = realloc(o_->obj_name, sizeof(char *) * o_->len);
  }

  o_->st_obj[o_->len] = malloc(obj_len);
  o_->obj_name[o_->len] = malloc(strlen(_));
  strncpy(o_->obj_name[o_->len], _, strlen(_));
  o_->obj[o_->len] = obj_len;
  o_->len++;
}

void *get_obj(struct obj_type *o_, char *obj_name) {
  for (int __ = 0; __ <= o_->len; __++) {
    if (strcmp(o_->obj_name[__], obj_name) == 0) {
      return o_->st_obj[__];
    }
  }

  return NULL;
}

void set_obj(struct obj_type *o_, char *obj_name, void *p) {
  for (int __ = 0; __ <= o_->len - 1; __++) {
    if (strcmp(o_->obj_name[__], obj_name) == 0) {
      memcpy(o_->st_obj[__], p, o_->obj[__]);
    }
  }

  return;
}

void new_method(class __x, char *meth_name, void *m) {
  struct obj_type *p = ((struct obj_type *)*__x);
  if (p->m_len < 1) {
    p->method = malloc(sizeof(void *) * p->m_len);
    p->method_name = malloc(sizeof(char *));
  } else {
    p->method = realloc(p->method, sizeof(void *) * p->m_len);
    p->method_name = realloc(p->method_name, sizeof(char *) * p->m_len);
  }

  p->method_name[p->m_len] = malloc(strlen(meth_name));
  strncpy(p->method_name[p->m_len], meth_name, strlen(meth_name));
  p->method[p->m_len] = m;
  p->m_len++;
}

void *call(class __x, char *func) {
  struct obj_type *p = ((struct obj_type *)*__x);
  for (int i = 0; i <= p->m_len - 1; i++) {
    if (strcmp(p->method_name[i], func) == 0) {
      return p->method[i];
    }
  }
  return NULL;
}

void obj(class __x_, char *name, int len) {
  new_obj(((struct obj_type *)*__x_), name, len);
}

void *_get(class __x_, char *name) {
  return get_obj(((struct obj_type *)*__x_), name);
}

void _set(class __x_, char *name, void *p) {
  set_obj(((struct obj_type *)*__x_), name, p);
}

void **_new_class(struct c_ class__pr) {
  struct obj_type *obj = malloc(sizeof(struct obj_type));
  obj->len = 0;
  void **_class = malloc(sizeof(void *));
  _class[0] = (void *)obj;
  class__pr.class_ = realloc(class__pr.class_, sizeof(class) * class__s.len);
  class__pr.class_[class__s.len] = _class;
  class__pr.len++;
  return _class;
}

int main() {
  class x = Template_Class(

      constructor(
          {
            set(string_value, y, char *);
            set(string_length, strlen(y), int);
          },
          args(char* y));

      create_method(len, message(int,
          {
            return get(int, string_length); 
          }
       ));

      create_method(
          print, message(void, 
           {
            printf("%s\n", get(char *, string_value)); 
           }
       ));

      create_method(
          append, message(void, 
           {
            char* new_char = malloc(method(int, x, len)+strlen(y));
            strcat(new_char, get(char *, string_value));
            strcat(new_char, y);
            set(string_value, new_char, char *);
            set(string_length, strlen(new_char), int);
           }
       , args(char* y)));
    
      object(string_value, type(char *));
      object(string_length, type(int));
  );

  call_constructor(x, args("hello"));
  method(void, x, append, args(" "));
  method(void, x, append, args("world!"));
  method(void, x, print);
  printf("the string had %d character\n", method(int, x, len));
}