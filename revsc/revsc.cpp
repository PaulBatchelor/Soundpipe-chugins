#include "chuck_dl.h"
#include "chuck_def.h"
#include <stdio.h>
#include <limits.h>

extern "C" {
    #include "soundpipe.h"
}

CK_DLL_CTOR(revsc_ctor);
CK_DLL_DTOR(revsc_dtor);

CK_DLL_MFUN(revsc_setFeedback);
CK_DLL_MFUN(revsc_getFeedback);

CK_DLL_MFUN(revsc_setCutoff);
CK_DLL_MFUN(revsc_getCutoff);

CK_DLL_MFUN(revsc_setMix);
CK_DLL_MFUN(revsc_getMix);

CK_DLL_TICKF(revsc_tick);

t_CKINT revsc_data_offset = 0;

struct RevscData {
    sp_data *sp;
    sp_revsc *revsc;
    SAMPLE mix;
};

CK_DLL_QUERY(RevSC)
{
    QUERY->setname(QUERY, "RevSC");
    
    QUERY->begin_class(QUERY, "RevSC", "UGen");
    
    QUERY->add_ctor(QUERY, revsc_ctor);
    QUERY->add_dtor(QUERY, revsc_dtor);
    
    QUERY->add_ugen_funcf(QUERY, revsc_tick, NULL, 1, 2);
    
    QUERY->add_mfun(QUERY, revsc_setFeedback, "float", "feedback");
    QUERY->add_arg(QUERY, "float", "arg");
    
    QUERY->add_mfun(QUERY, revsc_getFeedback, "float", "feedback");
    
    QUERY->add_mfun(QUERY, revsc_setCutoff, "float", "cutoff");
    QUERY->add_arg(QUERY, "float", "arg");
    
    QUERY->add_mfun(QUERY, revsc_getCutoff, "float", "cutoff");
    
    
    QUERY->add_mfun(QUERY, revsc_setMix, "float", "mix");
    QUERY->add_arg(QUERY, "float", "arg");
    
    QUERY->add_mfun(QUERY, revsc_getMix, "float", "mix");
    
    revsc_data_offset = QUERY->add_mvar(QUERY, "int", "@revsc_data", false);
    
    QUERY->end_class(QUERY);

    return TRUE;
}


CK_DLL_CTOR(revsc_ctor)
{
    OBJ_MEMBER_INT(SELF, revsc_data_offset) = 0;
   
    RevscData * data = new RevscData;
    sp_create(&data->sp); data->sp->sr = API->vm->get_srate();
    sp_revsc_create(&data->revsc);
    sp_revsc_init(data->sp, data->revsc);
    data->mix = 0.5;
    OBJ_MEMBER_INT(SELF, revsc_data_offset) = (t_CKINT) data;
}

CK_DLL_DTOR(revsc_dtor)
{
    RevscData * data = (RevscData *) OBJ_MEMBER_INT(SELF, revsc_data_offset);
    if(data)
    {
        sp_destroy(&data->sp);
        sp_revsc_destroy(&data->revsc);
        delete data;
        OBJ_MEMBER_INT(SELF, revsc_data_offset) = 0;
        data = NULL;
    }
}

CK_DLL_TICKF(revsc_tick)
{
    RevscData * data = (RevscData *) OBJ_MEMBER_INT(SELF, revsc_data_offset);
    SPFLOAT outL, outR;
    
    sp_revsc_compute(data->sp, data->revsc, in, in, &outL, &outR);
    out[0] = outL * data->mix + *in * (1 - data->mix);
    out[1] = outR * data->mix + *in * (1 - data->mix);
    return TRUE;
}

CK_DLL_MFUN(revsc_setFeedback)
{
    RevscData * data = (RevscData *) OBJ_MEMBER_INT(SELF, revsc_data_offset);
    data->revsc->feedback= GET_NEXT_FLOAT(ARGS);
    RETURN->v_float = data->revsc->feedback;
}

CK_DLL_MFUN(revsc_getFeedback)
{
    RevscData * data = (RevscData *) OBJ_MEMBER_INT(SELF, revsc_data_offset);
    RETURN->v_float = data->revsc->feedback;
}

CK_DLL_MFUN(revsc_setCutoff)
{
    RevscData * data = (RevscData *) OBJ_MEMBER_INT(SELF, revsc_data_offset);
    data->revsc->lpfreq= GET_NEXT_FLOAT(ARGS);
    RETURN->v_float = data->revsc->lpfreq;
}

CK_DLL_MFUN(revsc_getCutoff)
{
    RevscData * data = (RevscData *) OBJ_MEMBER_INT(SELF, revsc_data_offset);
    RETURN->v_float = data->revsc->lpfreq;
}

CK_DLL_MFUN(revsc_setMix)
{
    RevscData * data = (RevscData *) OBJ_MEMBER_INT(SELF, revsc_data_offset);
    data->mix = GET_NEXT_FLOAT(ARGS);
    RETURN->v_float = data->mix;
}

CK_DLL_MFUN(revsc_getMix)
{
    RevscData * data = (RevscData *) OBJ_MEMBER_INT(SELF, revsc_data_offset);
    RETURN->v_float = data->mix;
}
