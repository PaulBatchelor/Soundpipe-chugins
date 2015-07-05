#include "chuck_dl.h"
#include "chuck_def.h"
#include <stdio.h>
#include <limits.h>

extern "C"{
#include "soundpipe.h"
}

CK_DLL_CTOR(metro_ctor);
CK_DLL_DTOR(metro_dtor);

CK_DLL_MFUN(metro_setFreq);
CK_DLL_MFUN(metro_getFreq);

CK_DLL_TICK(metro_tick);

t_CKINT metro_data_offset = 0;

struct metroData {
    sp_data *sp;
    sp_metro *metro;
};

CK_DLL_QUERY(Metro)
{
    QUERY->setname(QUERY, "Metro");
    
    QUERY->begin_class(QUERY, "Metro", "UGen");
    
    QUERY->add_ctor(QUERY, metro_ctor);
    QUERY->add_dtor(QUERY, metro_dtor);
    
    QUERY->add_ugen_func(QUERY, metro_tick, NULL, 1, 1);
    
    QUERY->add_mfun(QUERY, metro_setFreq, "float", "freq");
    QUERY->add_arg(QUERY, "float", "arg");
    
    QUERY->add_mfun(QUERY, metro_getFreq, "float", "freq");
    
    metro_data_offset = QUERY->add_mvar(QUERY, "int", "@metro_data", false);
    
    QUERY->end_class(QUERY);

    return TRUE;
}


CK_DLL_CTOR(metro_ctor)
{
    OBJ_MEMBER_INT(SELF, metro_data_offset) = 0;
   
    metroData * data = new metroData;
    sp_create(&data->sp);
    data->sp->sr = API->vm->get_srate();
    sp_metro_create(&data->metro);
    sp_metro_init(data->sp, data->metro, 2);
       
    OBJ_MEMBER_INT(SELF, metro_data_offset) = (t_CKINT) data;
}

CK_DLL_DTOR(metro_dtor)
{
    metroData * data = (metroData *) OBJ_MEMBER_INT(SELF, metro_data_offset);
    if(data)
    {
        sp_metro_destroy(&data->metro);
        sp_destroy(&data->sp);

        delete data;
        OBJ_MEMBER_INT(SELF, metro_data_offset) = 0;
        data = NULL;
    }
}

CK_DLL_TICK(metro_tick)
{
    metroData * data = (metroData *) OBJ_MEMBER_INT(SELF, metro_data_offset);
    
    sp_metro_compute(data->sp, data->metro, &in, out);
    return TRUE;
}

CK_DLL_MFUN(metro_setFreq)
{
    metroData * data = (metroData *) OBJ_MEMBER_INT(SELF, metro_data_offset);
    data->metro->freq = GET_NEXT_FLOAT(ARGS);
    RETURN->v_float = data->metro->freq;
}

CK_DLL_MFUN(metro_getFreq)
{
    metroData * data = (metroData *) OBJ_MEMBER_INT(SELF, metro_data_offset);
    RETURN->v_float = data->metro->freq;
}

