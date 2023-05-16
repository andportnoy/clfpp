#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX(a,b) (((a)<(b))? (b) : (a))

#define v_c(v)   (((int *)(v))[-2])
#define v_l(v)   (((int *)(v))[-1])
#define v_cap(v) ((v)? v_c(v): 0)
#define v_len(v) ((v)? v_l(v): 0)
#define v_ptr(v) ((v)? ((void *)((int *)(v)-2)): 0)

#define v_adjoin(v,x) do {				      \
	if ((v) == NULL || v_c(v) == v_l(v)) {		      \
		const int cap = (v)? 2*v_c(v): 8;	      \
		const int len = v_len(v);		      \
		size_t size = 2*sizeof(int) + cap*sizeof*(v); \
		int *p = realloc(v_ptr(v), size);	      \
		if (!p)					      \
			exit(EXIT_FAILURE);		      \
		(v) = (void *)(p+2);			      \
		v_c(v) = cap;				      \
		v_l(v) = len;				      \
	}						      \
	v[v_l(v)++] = x;				      \
} while (0)

#define v_flush(v) do {				\
		if (v)				\
			v_l(v) = 0;		\
	} while(0)

#define v_free(v) free(v_ptr(v))

void tableprint(char ***table) {
	int *width = 0;
	for (int i=0; table[i]; ++i) {
		char **row = table[i];
		for (int j=0; row[j]; ++j) {
			char *word = row[j];
			int w = strlen(word);
			if (!(j<v_len(width)))
				v_adjoin(width, 0);
			width[j] = MAX(width[j], w);
		}
	}
	assert(width);
	for (int i=0; table[i]; ++i) {
		char **row = table[i];
		for (int j=0; row[j]; ++j) {
			char *word = row[j];
			if (j)
				printf("  ");
			printf("%-*s", width[j], word);
		}
		printf("\n");
	}
	v_free(width);
}

void tablefree(char ***table) {
	for (int i=0; table[i]; ++i) {
		char **row = table[i];
		for (int j=0; row[j]; ++j) {
			char *word = row[j];
			v_free(word);
		}
		v_free(row);
	}
	v_free(table);
}

char *word(char **sp) {
	char *s = *sp;
	char *w = 0;
	while (*s == ' ')
		s++;

	if (!*s)
		return NULL;

	if (*s == '[') {
		v_adjoin(w, *s++);
		while (*s != ']')
			v_adjoin(w, *s++);
		v_adjoin(w, *s++);
	} else if (*s == '"') {
		v_adjoin(w, *s++);
		while (*s != '"')
			v_adjoin(w, *s++);
		v_adjoin(w, *s++);
	} else {
		while (*s != ' ')
			v_adjoin(w, *s++);
	}
	v_adjoin(w, '\0');
	*sp = s;
	return w;
}

char **row(char *s) {
	char **r = 0;
	char *w = 0;
	while ((w = word(&s)))
		v_adjoin(r, w);
	v_adjoin(r, NULL);
	return r;
}

int main(void) {
	int c;
	char *s = 0;
	char ***table = 0;
	while ((c = getchar()) != EOF) {
		if (c != '\n')
			v_adjoin(s, c);
		else {
			v_adjoin(s, '\0');
			v_adjoin(table, row(s));
			v_flush(s);
		}
	}
	v_free(s);
	v_adjoin(table, 0);
	tableprint(table);
	tablefree(table);
}
