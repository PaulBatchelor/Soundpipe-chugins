#include <stdlib.h>
#include "soundpipe.h"
#include "sp_lsamp.h"

int sp_lsamp_create(sp_lsamp **p) 
{
    *p = malloc(sizeof(sp_lsamp));
    return SP_OK;
}

int sp_lsamp_init(sp_data *sp, sp_lsamp *p, lsamp_data *ld, lsamp_handle *lh) 
{
    p->trig = 0;
    p->ld = ld;
    p->lh = lh;
    p->counter = 0;
    p->rowid = lh->rowid;
    return SP_OK;
}

int sp_lsamp_compute(sp_data *sp, sp_lsamp *p, SPFLOAT *in, SPFLOAT *out) 
{
    if(*in) {
        lsamp_change_sample(p->ld, p->lh, p->rowid);
        p->counter = 0;
    } 
    
    if(p->counter == 0) {
        lsamp_load_to_buf(p->lh, lsamp_get_frmpos(p->lh));
    }
    
   
    if(lsamp_get_frmpos(p->lh) >= lsamp_get_total_frames(p->lh)) {
        *out = 0;
    } else {
       *out = (SPFLOAT) lsamp_get_frame(p->lh, p->counter);
       p->counter = (p->counter + 1) % lsamp_get_bufsize(p->lh);
    }
   
    return SP_OK;
}

int sp_lsamp_destroy(sp_lsamp **p)
{
    free(*p);
    return SP_OK;
}