#include "chuck_dl.h"
#include "chuck_def.h"
#include <stdio.h>
#include <limits.h>

extern "C"{
#include "soundpipe.h"
}

CK_DLL_CTOR(fosc_ctor);
CK_DLL_DTOR(fosc_dtor);

CK_DLL_MFUN(fosc_setFreq);
CK_DLL_MFUN(fosc_getFreq);

CK_DLL_MFUN(fosc_setCar);
CK_DLL_MFUN(fosc_getCar);

CK_DLL_MFUN(fosc_setMod);
CK_DLL_MFUN(fosc_getMod);

CK_DLL_MFUN(fosc_setIndex);
CK_DLL_MFUN(fosc_getIndex);

CK_DLL_TICK(fosc_tick);

t_CKINT fosc_data_offset = 0;

struct FoscData {
    sp_data *sp;
    sp_fosc *fosc;
    sp_ftbl *ft;
};

CK_DLL_QUERY(Fosc)
{
    QUERY->setname(QUERY, "FOsc");
    
    QUERY->begin_class(QUERY, "FOsc", "UGen");
    
    QUERY->add_ctor(QUERY, fosc_ctor);
    QUERY->add_dtor(QUERY, fosc_dtor);
    
    QUERY->add_ugen_func(QUERY, fosc_tick, NULL, 1, 1);
    
    QUERY->add_mfun(QUERY, fosc_setFreq, "float", "freq");
    QUERY->add_arg(QUERY, "float", "arg");
    QUERY->add_mfun(QUERY, fosc_getFreq, "float", "freq");
    
    QUERY->add_mfun(QUERY, fosc_setCar, "float", "car");
    QUERY->add_arg(QUERY, "float", "arg");
    QUERY->add_mfun(QUERY, fosc_getCar, "float", "car");
    
    QUERY->add_mfun(QUERY, fosc_setMod, "float", "mod");
    QUERY->add_arg(QUERY, "float", "arg");
    QUERY->add_mfun(QUERY, fosc_getMod, "float", "mod");
    
    QUERY->add_mfun(QUERY, fosc_setIndex, "float", "index");
    QUERY->add_arg(QUERY, "float", "arg");
    QUERY->add_mfun(QUERY, fosc_getIndex, "float", "index");

    fosc_data_offset = QUERY->add_mvar(QUERY, "int", "@fosc_data", false);
    
    QUERY->end_class(QUERY);

    return TRUE;
}


CK_DLL_CTOR(fosc_ctor)
{
    OBJ_MEMBER_INT(SELF, fosc_data_offset) = 0;
   
    FoscData * data = new FoscData;
    sp_create(&data->sp);
    data->sp->sr = API->vm->get_srate();
    sp_fosc_create(&data->fosc);
    sp_ftbl_create(data->sp, &data->ft, 4096);
    sp_gen_sine(data->ft);
    sp_fosc_init(data->sp, data->fosc, data->ft);
    OBJ_MEMBER_INT(SELF, fosc_data_offset) = (t_CKINT) data;
}

CK_DLL_DTOR(fosc_dtor)
{
    FoscData * data = (FoscData *) OBJ_MEMBER_INT(SELF, fosc_data_offset);
    if(data)
    {
        sp_ftbl_destroy(&data->ft);
        sp_fosc_destroy(&data->fosc);
        sp_destroy(&data->sp);

        delete data;
        OBJ_MEMBER_INT(SELF, fosc_data_offset) = 0;
        data = NULL;
    }
}

CK_DLL_TICK(fosc_tick)
{
    FoscData * data = (FoscData *) OBJ_MEMBER_INT(SELF, fosc_data_offset);
    
    sp_fosc_compute(data->sp, data->fosc, &in, out);
    return TRUE;
}

CK_DLL_MFUN(fosc_setFreq)
{
    FoscData * data = (FoscData *) OBJ_MEMBER_INT(SELF, fosc_data_offset);
    data->fosc->freq = GET_NEXT_FLOAT(ARGS);
    RETURN->v_float = data->fosc->freq;
}

CK_DLL_MFUN(fosc_getFreq)
{
    FoscData * data = (FoscData *) OBJ_MEMBER_INT(SELF, fosc_data_offset);
    RETURN->v_float = data->fosc->freq;
}

CK_DLL_MFUN(fosc_setCar)
{
    FoscData * data = (FoscData *) OBJ_MEMBER_INT(SELF, fosc_data_offset);
    data->fosc->car= GET_NEXT_FLOAT(ARGS);
    RETURN->v_float = data->fosc->car;
}

CK_DLL_MFUN(fosc_getCar)
{
    FoscData * data = (FoscData *) OBJ_MEMBER_INT(SELF, fosc_data_offset);
    RETURN->v_float = data->fosc->car;
}

CK_DLL_MFUN(fosc_setMod)
{
    FoscData * data = (FoscData *) OBJ_MEMBER_INT(SELF, fosc_data_offset);
    data->fosc->mod= GET_NEXT_FLOAT(ARGS);
    RETURN->v_float = data->fosc->mod;
}

CK_DLL_MFUN(fosc_getMod)
{
    FoscData * data = (FoscData *) OBJ_MEMBER_INT(SELF, fosc_data_offset);
    RETURN->v_float = data->fosc->mod;
}

CK_DLL_MFUN(fosc_setIndex)
{
    FoscData * data = (FoscData *) OBJ_MEMBER_INT(SELF, fosc_data_offset);
    data->fosc->indx= GET_NEXT_FLOAT(ARGS);
    RETURN->v_float = data->fosc->indx;
}

CK_DLL_MFUN(fosc_getIndex)
{
    FoscData * data = (FoscData *) OBJ_MEMBER_INT(SELF, fosc_data_offset);
    RETURN->v_float = data->fosc->indx;
}

