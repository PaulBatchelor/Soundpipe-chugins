

#include "chuck_dl.h"
#include "chuck_def.h"

#include <stdio.h>
#include <limits.h>


CK_DLL_CTOR(tone_ctor);
CK_DLL_DTOR(tone_dtor);

CK_DLL_MFUN(tone_setBits);
CK_DLL_MFUN(tone_getBits);

CK_DLL_TICK(tone_tick);

t_CKINT tone_data_offset = 0;


struct ToneData
{
    int bits;
    int downsampleFactor;
    
    int currentSampleCount;
    SAMPLE currentSample;
};


CK_DLL_QUERY(Bitcrusher)
{
    QUERY->setname(QUERY, "Tone");
    
    QUERY->begin_class(QUERY, "Tone", "UGen");
    
    QUERY->add_ctor(QUERY, tone_ctor);
    QUERY->add_dtor(QUERY, tone_dtor);
    
    QUERY->add_ugen_func(QUERY, tone_tick, NULL, 1, 1);
    
    QUERY->add_mfun(QUERY, tone_setBits, "int", "bits");
    QUERY->add_arg(QUERY, "int", "arg");
    
    QUERY->add_mfun(QUERY, tone_getBits, "int", "bits");
    
    tone_data_offset = QUERY->add_mvar(QUERY, "int", "@tone_data", false);
    
    QUERY->end_class(QUERY);

    return TRUE;
}


CK_DLL_CTOR(tone_ctor)
{
    OBJ_MEMBER_INT(SELF, tone_data_offset) = 0;
    
    ToneData * data = new ToneData;
    
    OBJ_MEMBER_INT(SELF, tone_data_offset) = (t_CKINT) data;
}

CK_DLL_DTOR(tone_dtor)
{
    ToneData * data = (ToneData *) OBJ_MEMBER_INT(SELF, tone_data_offset);
    if(data)
    {
        delete data;
        OBJ_MEMBER_INT(SELF, tone_data_offset) = 0;
        data = NULL;
    }
}

CK_DLL_TICK(tone_tick)
{
    ToneData * data = (ToneData *) OBJ_MEMBER_INT(SELF, tone_data_offset);
    
    
    *out = in;

    return TRUE;
}

CK_DLL_MFUN(tone_setBits)
{
    ToneData * data = (ToneData *) OBJ_MEMBER_INT(SELF, tone_data_offset);
    // TODO: sanity check
    data->bits = GET_NEXT_INT(ARGS);
    RETURN->v_int = data->bits;
}

CK_DLL_MFUN(tone_getBits)
{
    ToneData * data = (ToneData *) OBJ_MEMBER_INT(SELF, tone_data_offset);
    RETURN->v_int = data->bits;
}

