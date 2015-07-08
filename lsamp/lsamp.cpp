#include "chuck_dl.h"
#include "chuck_def.h"
#include <stdio.h>
#include <limits.h>

extern "C"{
#include "soundpipe.h"
#include "sp_lsamp.h"
}

CK_DLL_CTOR(lsamp_ctor);
CK_DLL_DTOR(lsamp_dtor);

CK_DLL_MFUN(lsamp_select);
CK_DLL_MFUN(lsamp_open_file);

CK_DLL_TICK(lsamp_tick);

t_CKINT lsamp_data_offset = 0;

struct lsampData {
    sp_data *sp;
    sp_lsamp *lsamp;
    lsamp_data *ld;
    lsamp_handle *lh;
    int loaded;
    int init;
    int selected;
    SPFLOAT trig;
};

CK_DLL_QUERY(LSamp)
{
    QUERY->setname(QUERY, "LSamp");
    
    QUERY->begin_class(QUERY, "LSamp", "UGen");
    
    QUERY->add_ctor(QUERY, lsamp_ctor);
    QUERY->add_dtor(QUERY, lsamp_dtor);
    
    QUERY->add_ugen_func(QUERY, lsamp_tick, NULL, 1, 1);
    
    QUERY->add_mfun(QUERY, lsamp_select, "void", "select");
    QUERY->add_arg(QUERY, "string", "keyword");
  
    QUERY->add_mfun(QUERY, lsamp_open_file, "void", "open");
    QUERY->add_arg(QUERY, "string", "filename");
    
    lsamp_data_offset = QUERY->add_mvar(QUERY, "int", "@lsamp_data", false);
    
    QUERY->end_class(QUERY);

    return TRUE;
}


CK_DLL_CTOR(lsamp_ctor)
{
    OBJ_MEMBER_INT(SELF, lsamp_data_offset) = 0;
   
    lsampData * data = new lsampData;
    sp_create(&data->sp);
    data->sp->sr = API->vm->get_srate();
    sp_lsamp_create(&data->lsamp);
    //sp_lsamp_init(data->sp, data->lsamp);
    data->loaded = 0;
    data->selected = 0;   
    data->trig = 0;
    data->init = 0;
    OBJ_MEMBER_INT(SELF, lsamp_data_offset) = (t_CKINT) data;
}

CK_DLL_DTOR(lsamp_dtor)
{
    lsampData * data = (lsampData *) OBJ_MEMBER_INT(SELF, lsamp_data_offset);
    if(data)
    {
        if(data->loaded) {
           lsamp_close(&data->ld);
        }
        if(data->init) {
            lsamp_close_handle(&data->lh);
        }
        sp_lsamp_destroy(&data->lsamp);
        sp_destroy(&data->sp);
        
        delete data;
        OBJ_MEMBER_INT(SELF, lsamp_data_offset) = 0;
        data = NULL;
    }
}

CK_DLL_TICK(lsamp_tick)
{
    lsampData * data = (lsampData *) OBJ_MEMBER_INT(SELF, lsamp_data_offset);
    
    if(data->selected) {
        sp_lsamp_compute(data->sp, data->lsamp, &data->trig, out);
        data->trig = 0;
    } else {
        *out = 0;
    }
    return TRUE;
}

CK_DLL_MFUN(lsamp_open_file)
{
    lsampData * data = (lsampData *) OBJ_MEMBER_INT(SELF, lsamp_data_offset);
    Chuck_String *ckstring = GET_CK_STRING(ARGS);
    const char * filename = ckstring->str.c_str();
    if(!data->loaded) {
        if(lsamp_open(&data->ld, filename, LSAMP_READ)) {
            data->loaded = 1;
        }
    }
}

CK_DLL_MFUN(lsamp_select)
{
    lsampData * data = (lsampData *) OBJ_MEMBER_INT(SELF, lsamp_data_offset);
    Chuck_String *ckstring = GET_CK_STRING(ARGS);
    const char * keyword = ckstring->str.c_str();
    if(data->loaded) {
        if(!data->init) {
            if(lsamp_open_handle(data->ld, &data->lh, 
            lsamp_rowid(data->ld, keyword), 1024)) {
                data->selected = 1;
                sp_lsamp_init(data->sp, data->lsamp, data->ld, data->lh);
                data->init = 1;
                data->trig = 1;
            }
        } else {
            data->lsamp->rowid = lsamp_rowid(data->ld, keyword);
            data->selected = 1;    
            data->trig = 1;
        }
    }
}


