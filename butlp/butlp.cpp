#include "chuck_dl.h"
#include "chuck_def.h"
#include <stdio.h>
#include <limits.h>

extern "C"{
#include "soundpipe.h"
}

CK_DLL_CTOR(butlp_ctor);
CK_DLL_DTOR(butlp_dtor);

CK_DLL_MFUN(butlp_setCutoff);
CK_DLL_MFUN(butlp_getCutoff);

CK_DLL_TICK(butlp_tick);

t_CKINT butlp_data_offset = 0;

struct ButlpData {
    sp_data *sp;
    sp_butlp *butlp;
};

CK_DLL_QUERY(Butlp)
{
    QUERY->setname(QUERY, "Butlp");
    
    QUERY->begin_class(QUERY, "Butlp", "UGen");
    
    QUERY->add_ctor(QUERY, butlp_ctor);
    QUERY->add_dtor(QUERY, butlp_dtor);
    
    QUERY->add_ugen_func(QUERY, butlp_tick, NULL, 1, 1);
    
    QUERY->add_mfun(QUERY, butlp_setCutoff, "float", "cutoff");
    QUERY->add_arg(QUERY, "float", "arg");
    
    QUERY->add_mfun(QUERY, butlp_getCutoff, "float", "cutoff");
    
    butlp_data_offset = QUERY->add_mvar(QUERY, "int", "@butlp_data", false);
    
    QUERY->end_class(QUERY);

    return TRUE;
}


CK_DLL_CTOR(butlp_ctor)
{
    OBJ_MEMBER_INT(SELF, butlp_data_offset) = 0;
   
    ButlpData * data = new ButlpData;
    sp_create(&data->sp);
    data->sp->sr = API->vm->get_srate();
    sp_butlp_create(&data->butlp);
    sp_butlp_init(data->sp, data->butlp);
       
    OBJ_MEMBER_INT(SELF, butlp_data_offset) = (t_CKINT) data;
}

CK_DLL_DTOR(butlp_dtor)
{
    ButlpData * data = (ButlpData *) OBJ_MEMBER_INT(SELF, butlp_data_offset);
    if(data)
    {
        sp_butlp_destroy(&data->butlp);
        sp_destroy(&data->sp);

        delete data;
        OBJ_MEMBER_INT(SELF, butlp_data_offset) = 0;
        data = NULL;
    }
}

CK_DLL_TICK(butlp_tick)
{
    ButlpData * data = (ButlpData *) OBJ_MEMBER_INT(SELF, butlp_data_offset);
    
    sp_butlp_compute(data->sp, data->butlp, &in, out);
    return TRUE;
}

CK_DLL_MFUN(butlp_setCutoff)
{
    ButlpData * data = (ButlpData *) OBJ_MEMBER_INT(SELF, butlp_data_offset);
    data->butlp->freq = GET_NEXT_FLOAT(ARGS);
    RETURN->v_float = data->butlp->freq;
}

CK_DLL_MFUN(butlp_getCutoff)
{
    ButlpData * data = (ButlpData *) OBJ_MEMBER_INT(SELF, butlp_data_offset);
    RETURN->v_float = data->butlp->freq;
}

