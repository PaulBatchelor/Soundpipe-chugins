#include "lsamp/lsamp.h"

typedef struct {
    lsamp_data *ld;
    lsamp_handle *lh;
    SPFLOAT trig;
    uint32_t rowid;
    int counter;
} sp_lsamp;

int sp_lsamp_create(sp_lsamp **p);
int sp_lsamp_init(sp_data *sp, sp_lsamp *p, lsamp_data *ld, lsamp_handle *lh);
int sp_lsamp_compute(sp_data *sp, sp_lsamp *p, SPFLOAT *in, SPFLOAT *out);
int sp_lsamp_destroy(sp_lsamp **p);

