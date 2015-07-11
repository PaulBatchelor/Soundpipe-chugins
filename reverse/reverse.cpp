#include "chuck_dl.h"
#include "chuck_def.h"
#include <stdio.h>
#include <limits.h>

extern "C"{
#include "soundpipe.h"
}

CK_DLL_CTOR(reverse_ctor);
CK_DLL_DTOR(reverse_dtor);

CK_DLL_MFUN(reverse_setDelay);
CK_DLL_MFUN(reverse_getDelay);

CK_DLL_MFUN(reverse_setMix);
CK_DLL_MFUN(reverse_getMix);

CK_DLL_TICK(reverse_tick);

t_CKINT reverse_data_offset = 0;

struct reverseData {
    sp_data *sp;
    sp_reverse *reverse;
    int set;
    SPFLOAT mix;
};

CK_DLL_QUERY(Reverse)
{
    QUERY->setname(QUERY, "Reverse");
    
    QUERY->begin_class(QUERY, "Reverse", "UGen");
    
    QUERY->add_ctor(QUERY, reverse_ctor);
    QUERY->add_dtor(QUERY, reverse_dtor);
    
    QUERY->add_ugen_func(QUERY, reverse_tick, NULL, 1, 1);
    
    QUERY->add_mfun(QUERY, reverse_setDelay, "float", "delay");
    QUERY->add_arg(QUERY, "float", "arg");
    QUERY->add_mfun(QUERY, reverse_getDelay, "float", "delay");
    
    QUERY->add_mfun(QUERY, reverse_setMix, "float", "mix");
    QUERY->add_arg(QUERY, "float", "arg");
    QUERY->add_mfun(QUERY, reverse_getMix, "float", "mix");

    reverse_data_offset = QUERY->add_mvar(QUERY, "int", "@reverse_data", false);
    
    QUERY->end_class(QUERY);

    return TRUE;
}


CK_DLL_CTOR(reverse_ctor)
{
    OBJ_MEMBER_INT(SELF, reverse_data_offset) = 0;
   
    reverseData * data = new reverseData;
    sp_create(&data->sp);
    data->sp->sr = API->vm->get_srate();
    sp_reverse_create(&data->reverse);
    data->set = 0;
    data->mix = 1;       

    OBJ_MEMBER_INT(SELF, reverse_data_offset) = (t_CKINT) data;
}

CK_DLL_DTOR(reverse_dtor)
{
    reverseData * data = (reverseData *) OBJ_MEMBER_INT(SELF, reverse_data_offset);
    if(data)
    {
        sp_reverse_destroy(&data->reverse);
        sp_destroy(&data->sp);
        delete data;
        OBJ_MEMBER_INT(SELF, reverse_data_offset) = 0;
        data = NULL;
    }
}

CK_DLL_TICK(reverse_tick)
{
    reverseData * data = (reverseData *) OBJ_MEMBER_INT(SELF, reverse_data_offset);
    SPFLOAT rev;
    if(data->set == 1) { 
        sp_reverse_compute(data->sp, data->reverse, &in, &rev);
    } else {
        *out = 0;
    }

    *out = data->mix * rev + (1 - data->mix) * in;

    return TRUE;
}

CK_DLL_MFUN(reverse_setDelay)
{
    reverseData * data = (reverseData *) OBJ_MEMBER_INT(SELF, reverse_data_offset);
    SPFLOAT delay = GET_NEXT_FLOAT(ARGS);
    if(data->set == 0) {
        sp_reverse_init(data->sp, data->reverse, delay);
        data->set = 1;
    }

    RETURN->v_float = data->reverse->delay;
}

CK_DLL_MFUN(reverse_getDelay)
{
    reverseData * data = (reverseData *) OBJ_MEMBER_INT(SELF, reverse_data_offset);
    RETURN->v_float = data->reverse->delay;
}

CK_DLL_MFUN(reverse_setMix)
{
    reverseData * data = (reverseData *) OBJ_MEMBER_INT(SELF, reverse_data_offset);
    data->mix = GET_NEXT_FLOAT(ARGS);
    RETURN->v_float = data->mix;
}

CK_DLL_MFUN(reverse_getMix)
{
    reverseData * data = (reverseData *) OBJ_MEMBER_INT(SELF, reverse_data_offset);
    RETURN->v_float = data->mix;
}

