#include "chuck_dl.h"
#include "chuck_def.h"
#include <stdio.h>
#include <limits.h>

extern "C"{
#include "soundpipe.h"
}

CK_DLL_CTOR(tone_ctor);
CK_DLL_DTOR(tone_dtor);

CK_DLL_MFUN(tone_setCutoff);
CK_DLL_MFUN(tone_getCutoff);

CK_DLL_TICK(tone_tick);

t_CKINT tone_data_offset = 0;

struct ToneData {
    float foo;
    sp_data *sp;
    sp_tone *tone;
};

CK_DLL_QUERY(Bitcrusher)
{
    QUERY->setname(QUERY, "Tone");
    
    QUERY->begin_class(QUERY, "Tone", "UGen");
    
    QUERY->add_ctor(QUERY, tone_ctor);
    QUERY->add_dtor(QUERY, tone_dtor);
    
    QUERY->add_ugen_func(QUERY, tone_tick, NULL, 1, 1);
    
    QUERY->add_mfun(QUERY, tone_setCutoff, "float", "cutoff");
    QUERY->add_arg(QUERY, "float", "arg");
    
    QUERY->add_mfun(QUERY, tone_getCutoff, "float", "cutoff");
    
    tone_data_offset = QUERY->add_mvar(QUERY, "int", "@tone_data", false);
    
    QUERY->end_class(QUERY);

    return TRUE;
}


CK_DLL_CTOR(tone_ctor)
{
    OBJ_MEMBER_INT(SELF, tone_data_offset) = 0;
   
    ToneData * data = new ToneData;
    sp_create(&data->sp);
    data->sp->sr = API->vm->get_srate();
    sp_tone_create(&data->tone);
    sp_tone_init(data->sp, data->tone);
       
    OBJ_MEMBER_INT(SELF, tone_data_offset) = (t_CKINT) data;
}

CK_DLL_DTOR(tone_dtor)
{
    ToneData * data = (ToneData *) OBJ_MEMBER_INT(SELF, tone_data_offset);
    if(data)
    {
        sp_tone_destroy(&data->tone);
        sp_destroy(&data->sp);

        delete data;
        OBJ_MEMBER_INT(SELF, tone_data_offset) = 0;
        data = NULL;
    }
}

CK_DLL_TICK(tone_tick)
{
    ToneData * data = (ToneData *) OBJ_MEMBER_INT(SELF, tone_data_offset);
    
    sp_tone_compute(data->sp, data->tone, &in, out);
    return TRUE;
}

CK_DLL_MFUN(tone_setCutoff)
{
    ToneData * data = (ToneData *) OBJ_MEMBER_INT(SELF, tone_data_offset);
    data->tone->hp = GET_NEXT_FLOAT(ARGS);
    RETURN->v_float = data->tone->hp;
}

CK_DLL_MFUN(tone_getCutoff)
{
    ToneData * data = (ToneData *) OBJ_MEMBER_INT(SELF, tone_data_offset);
    RETURN->v_float = data->tone->hp;
}

