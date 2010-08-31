
void init_tune(tune_t **head, tune_t **cur, tune_t **new);
void store_tune(tune_t **cur, int x, int y, int i, int yoff);
void create_tune(tune_t **cur, tune_t **new);
void free_tune(tune_t **head, tune_t **cur);
void print_tune(tune_t **head, tune_t **cur);
tune_t* merge_tune(tune_t *left, tune_t *right);
tune_t* msort_tune(tune_t *head, int n)
void delete_tune(tune_t **head, tune_t **cur, int *n, int rx, int ry);
void save_tune(tune_t **head, tune_t **cur);
void load_tune(tune_t **head, tune_t **cur);
void play_tune(tune_t *cur, tune_t *head, int n, sound_t *sounds, int tempo);
