#define recursive_flag  (1<<0)
#define follow_flag     (1<<1)
#define alpha_flag      (1<<2)
#define sbo_flag        (1<<3)

struct args {
    char *explude;
    int min;
    char *ignore;
    char *log;
    char *output;
};
extern struct args *params;
