#include <stdio.h>
#include <string.h>
#include "math.h"
#include "p99/p99.h"
#include <stdlib.h>

#define MONADAPPLICATION(t, t_, x, esc) \
    (void*)({                           \
        monoid t(monoid t_)             \
        {                               \
            void* x = t_.reciever;      \
            t_.reciever = esc;          \
            return t_;                  \
        }                               \
        t;                              \
    })
#define wrap(x, esc) MONADAPPLICATION(P99_UNIQ(LAMBDA_FUNCTION_L), P99_UNIQ(MONOID_DYNAMIC_VALUE), x, esc)
#define ret(x) ({typeof(x) d = x; typeof(x)* _ = malloc(sizeof(typeof(x))); memcpy(_, &d, sizeof(typeof(x))); ret_monoid((void*)_, DEFAULT, DEFAULT); })

#define eval(func_lambda, x) ((void* (*)(void*, void*))func_lambda)(env_, x)
#define LAMBDA(x) ({closure* env_ = lambda__(); x; })
#define Type(_) sizeof(_)
#define generate_(d, f)
#define data(e, d, ...) data_structure(e, d, __VA_ARGS__)
#define data_structure(e, d, ...) \
    enum e##enum { __VA_ARGS__ }; \
    union e##union {              \
        d;                        \
    };                            \
    struct e##struct {            \
        union e##union _;         \
        enum e##enum algebriac;   \
    };
#define sum_type(_) \
    struct _ {      \
    }
#define take(...) ...
#define Value(d)     \
    struct d##TYPE { \
        monoid var;  \
    } d##var
#define Empty(d)     \
    struct d##TYPE { \
    } d##var
#define dataType(d, _, f) data___(P99_UNIQ(____type), d, _, f)
#define data___(__, d, p, f) ({struct d##struct* __ = malloc(sizeof(struct d##struct)); (*__).algebriac = (enum d##enum)p; (*__)._.p##var = (struct p##TYPE)f; (void*)__; })
#define getData(x) ((struct Maybestruct*)x) //definitely it's not munch safe
#define getDataValue(x, y) ((*getData(x))._.y##var).var
#define Optional void *
#define DEFAULT NULL
#define instance_bind(esc) instance_bind_(P99_UNIQ(binder), esc)
#define instance_bind_(n, esc) ({monoid n(monoid _, void** app) {esc} n; })

typedef struct {
    struct {
        void** app;
        int i;
    } aplications;
    void* reciever;
    void* binder;
    void* __ev;
} monoid;

monoid bind__(monoid _, void** app)
{
    return (monoid){ { ({if (_.aplications.i > 1) {_.aplications.app = realloc(_.aplications.app, sizeof(void**)*_.aplications.i);} _.aplications.app[_.aplications.i-1] = app; _.aplications.app; }), _.aplications.i + 1 }, _.reciever, _.binder, _.__ev };
}

monoid _____ev(monoid _)
{
    monoid ret_ = _;
    for (int i = 1; i <= _.aplications.i - 1; i++) {
        ret_ = ((monoid (*)(monoid))_.aplications.app[i - 1])(ret_);
    }
    return _;
}

monoid ret_monoid(void* v, void* binder, void* __)
{
    return (monoid){ { malloc(sizeof(void**)), 1 }, v, binder == NULL ? bind__ : binder, __ == NULL ? _____ev : __ };
}

monoid bind(monoid _, void** app)
{
    return ((monoid (*)(monoid, void*))_.binder)(_, app);
}

monoid evaluation(monoid _)
{
    return ((monoid (*)(monoid))_.__ev)(_);
}

monoid setInstance(monoid _, void* instace_app_bind, void* instace_app_eval)
{
    return (monoid){ _.aplications, _.reciever, instace_app_bind, instace_app_eval };
}

data(Maybe, Value(Just); Empty(Nothing), Just, Nothing)

    Optional bind_maybe(Optional _, void* closure)
{
    switch (getData(_)->algebriac) {
    case Just:
        return dataType(Maybe, Just, { bind(getDataValue(_, Just), closure) });
    case Nothing:
        return dataType(Maybe, Nothing, {});
    }
}

monoid evaluation_maybe(Optional _)
{
    switch (getData(_)->algebriac) {
    case Just:
        return evaluation(getDataValue(_, Just));
    case Nothing:
        return ret(NULL);
    }
}

void* print(void* m)
{
    printf("Sqrt of %d : ", *(int*)m);
    return m;
}

void* print_sqrt(void* m)
{
    printf("%f\n", sqrt(*(int*)m));
    return m;
}

int main()
{
    /* kx :: String -> Maybe Int
    kx x = case (reads x) of
                             [(a, "")] -> Just a
                            _ -> Nothing
    tryPrint k = case k of 
                          Just k -> print (sqrt $ fromIntegral k)
                          Nothing -> return ()

    r = getLine >>= (\x -> (return (kx x)) >>= tryPrint) */

    // we can the same code this way above.
    
    Optional getInt()
    {
        char s[400];
        scanf("%400s", s);
        return (strcmp(s, "0") != 0 && atoi(s) == 0) ? dataType(Maybe, Nothing, {}) : dataType(Maybe, Just, { ret(atoi(s)) });
    }

    printf("Insert a number to calculate sqrt\n");
    evaluation_maybe(bind_maybe(bind_maybe(getInt(), wrap(y, print(y))), wrap(x, print_sqrt(x)))); //if a non-valid number was inserted just don't calculate
    return 0;
}