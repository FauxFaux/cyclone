typedef int size_t;
extern	struct	_iobuf {
int	_cnt;
unsigned char *_ptr;
unsigned char *_base;
int	_bufsiz;
short	_flag;
char	_file;	} _iob[];
typedef struct _iobuf FILE;
extern struct _iobuf	*fopen(const char *, const char *);
extern struct _iobuf	*fdopen(int, const char *);
extern struct _iobuf	*freopen(const char *, const char *, FILE *);
extern struct _iobuf	*popen(const char *, const char *);
extern struct _iobuf	*tmpfile(void);
extern long	ftell(FILE *);
extern char	*fgets(char *, int, FILE *);
extern char	*gets(char *);
extern char	*sprintf(char *, const char *, ...);
extern char	*ctermid(char *);
extern char	*cuserid(char *);
extern char	*tempnam(const char *, const char *);
extern char	*tmpnam(char *);
extern	char	_ctype_[];
extern struct _iobuf *popen(const char *, const char *), *tmpfile(void);
extern int pclose(FILE *);
extern void rewind(FILE *);
extern void abort(void), free(void *), exit(int), perror(const char *);
extern char *getenv(const char *), *malloc(size_t), *realloc(void *, size_t);
extern int system(const char *);
extern double atof(const char *);
extern char *strcpy(char *, const char *), *strncpy(char *, const char *, size_t), *strcat(char *, const char *), *strncat(char *, const char *, size_t), *strerror(int);
extern char *strpbrk(const char *, const char *), *strtok(char *, const char *), *strchr(const char *, int), *strrchr(const char *, int), *strstr(const char *, const char *);
extern int strcoll(const char *, const char *), strxfrm(char *, const char *, size_t), strncmp(const char *, const char *, size_t), strlen(const char *), strspn(const char *, const char *), strcspn(const char *, const char *);
extern char *memmove(void *, const void *, size_t), *memccpy(void *, const void *, int, size_t), *memchr(const void *, int, size_t), *memcpy(void *, const void *, size_t), *memset(void *, int, size_t);
extern int memcmp(const void *, const void *, size_t), strcmp(const char *, const char *);
extern long util_cpu_time();
extern int util_getopt();
extern void util_getopt_reset();
extern char *util_path_search();
extern char *util_file_search();
extern int util_pipefork();
extern void util_print_cpu_stats();
extern char *util_print_time();
extern int util_save_image();
extern char *util_strsav();
extern char *util_tilde_expand();
extern void util_restart();
extern int util_optind;
extern char *util_optarg;
typedef struct sm_element_struct sm_element;
typedef struct sm_row_struct sm_row;
typedef struct sm_col_struct sm_col;
typedef struct sm_matrix_struct sm_matrix;
struct sm_element_struct {
int row_num;	int col_num;	sm_element *next_row;	sm_element *prev_row;	sm_element *next_col;	sm_element *prev_col;	char *user_word;	};
struct sm_row_struct {
int row_num;	int length;	int flag;	sm_element *first_col;	sm_element *last_col;	sm_row *next_row;	sm_row *prev_row;	char *user_word;	};
struct sm_col_struct {
int col_num;	int length;	int flag;	sm_element *first_row;	sm_element *last_row;	sm_col *next_col;	sm_col *prev_col;	char *user_word;	};
struct sm_matrix_struct {
sm_row **rows;	int rows_size;	sm_col **cols;	int cols_size;	sm_row *first_row;	sm_row *last_row;	int nrows;	sm_col *first_col;	sm_col *last_col;	int ncols;	char *user_word;	};
extern sm_matrix *sm_alloc(), *sm_alloc_size(), *sm_dup();
extern void sm_free(), sm_delrow(), sm_delcol(), sm_resize();
extern void sm_write(), sm_print(), sm_dump(), sm_cleanup();
extern void sm_copy_row(), sm_copy_col();
extern void sm_remove(), sm_remove_element();
extern sm_element *sm_insert(), *sm_find();
extern sm_row *sm_longest_row();
extern sm_col *sm_longest_col();
extern int sm_read(), sm_read_compressed();
extern sm_row *sm_row_alloc(), *sm_row_dup(), *sm_row_and();
extern void sm_row_free(), sm_row_remove(), sm_row_print();
extern sm_element *sm_row_insert(), *sm_row_find();
extern int sm_row_contains(), sm_row_intersects();
extern int sm_row_compare(), sm_row_hash();
extern sm_col *sm_col_alloc(), *sm_col_dup(), *sm_col_and();
extern void sm_col_free(), sm_col_remove(), sm_col_print();
extern sm_element *sm_col_insert(), *sm_col_find();
extern int sm_col_contains(), sm_col_intersects();
extern int sm_col_compare(), sm_col_hash();
extern int sm_row_dominance(), sm_col_dominance(), sm_block_partition();
extern sm_row *sm_minimum_cover();
typedef struct stats_struct stats_t;
struct stats_struct {
int debug;	int max_print_depth;	int max_depth;	int nodes;	int component;	int comp_count;	int gimpel_count;	int gimpel;	long start_time;	int no_branching;
int lower_bound;
};
typedef struct solution_struct solution_t;
struct solution_struct {
sm_row *row;
int cost;
};
extern solution_t *solution_alloc();
extern void solution_free();
extern solution_t *solution_dup();
extern void solution_accept();
extern void solution_reject();
extern void solution_add();
extern solution_t *solution_choose_best();
extern solution_t *sm_maximal_independent_set(sm_matrix *A, int *weight);
extern solution_t *sm_mincov();
extern int gimpel_reduce();
static sm_matrix *build_intersection_matrix(sm_matrix *A);

solution_t * sm_maximal_independent_set(sm_matrix *A, int *weight)
{
register sm_row *best_row, *prow;
register sm_element *p;
int least_weight;
sm_row *save;
sm_matrix *B;
solution_t *indep;
indep = solution_alloc();
B = build_intersection_matrix(A);
while (B->nrows > 0) {
best_row = B->first_row;
for(prow = B->first_row->next_row; prow != 0; prow = prow->next_row) {
if (prow->length < best_row->length) {
best_row = prow;
}
}
if (weight == ((int *) 0)) {
least_weight = 1;
} else {
prow = ((( best_row->row_num) >= 0 && ( best_row->row_num) < (A)->rows_size) ? (A)->rows[ best_row->row_num] : (sm_row *) 0);
least_weight = weight[prow->first_col->col_num];
for(p = prow->first_col->next_col; p != 0; p = p->next_col) {
if (weight[p->col_num] < least_weight) {
least_weight = weight[p->col_num];
}
}
}
indep->cost += least_weight;
(void) sm_row_insert(indep->row, best_row->row_num);
save = sm_row_dup(best_row);
for(p = save->first_col; p != 0; p = p->next_col) {
sm_delrow(B, p->col_num);
sm_delcol(B, p->col_num);
}
sm_row_free(save);
}
sm_free(B);
return indep;
}

static sm_matrix *
build_intersection_matrix(sm_matrix *A)
{
register sm_row *prow, *prow1;
register sm_element *p, *p1;
register sm_col *pcol;
sm_matrix *B;
B = sm_alloc();
for(prow = A->first_row; prow != 0; prow = prow->next_row) {
for(p = prow->first_col; p != 0; p = p->next_col) {
pcol = ((( p->col_num) >= 0 && ( p->col_num) < (A)->cols_size) ? (A)->cols[ p->col_num] : (sm_col *) 0);
for(p1 = pcol->first_row; p1 != 0; p1 = p1->next_row) {
prow1 = ((( p1->row_num) >= 0 && ( p1->row_num) < (A)->rows_size) ? (A)->rows[ p1->row_num] : (sm_row *) 0);
prow1->flag = 0;
}
}
for(p = prow->first_col; p != 0; p = p->next_col) {
pcol = ((( p->col_num) >= 0 && ( p->col_num) < (A)->cols_size) ? (A)->cols[ p->col_num] : (sm_col *) 0);
for(p1 = pcol->first_row; p1 != 0; p1 = p1->next_row) {
prow1 = ((( p1->row_num) >= 0 && ( p1->row_num) < (A)->rows_size) ? (A)->rows[ p1->row_num] : (sm_row *) 0);
if (! prow1->flag) {
prow1->flag = 1;
(void) sm_insert(B, prow->row_num, prow1->row_num);
}
}
}
}
return B;
}
