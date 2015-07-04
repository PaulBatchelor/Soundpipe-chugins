#include <stdint.h>

#define SP_BUFSIZE 4096
#define SPFLOAT float
#define SP_OK 1
#define SP_NOT_OK 0

typedef unsigned long sp_frame;

typedef struct sp_auxdata {
    size_t size;
    void *ptr;
} sp_auxdata;

typedef struct sp_data { 
    SPFLOAT *out;
    int sr;
    int nchan;
    unsigned long len;
    unsigned long pos;
    char filename[200];
} sp_data; 

typedef struct {
    char state;
    SPFLOAT val;
} sp_param;

int sp_create(sp_data **spp);

int sp_destroy(sp_data **spp);
int sp_process(sp_data *sp, void *ud, void (*callback)(sp_data *, void *));

SPFLOAT sp_midi2cps(SPFLOAT nn);

int sp_set(sp_param *p, SPFLOAT val);
typedef struct {
        SPFLOAT hp;
        SPFLOAT c1, c2, yt1, prvhp;
        SPFLOAT tpidsr;
} sp_tone;

int sp_tone_create(sp_tone **t);
int sp_tone_destroy(sp_tone **t);
int sp_tone_init(sp_data *sp, sp_tone *t);
int sp_tone_compute(sp_data *sp, sp_tone *t, SPFLOAT *in, SPFLOAT *out);
