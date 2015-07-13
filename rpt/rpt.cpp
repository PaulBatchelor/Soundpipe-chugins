#include "chuck_dl.h"
#include "chuck_def.h"
#include <stdio.h>
#include <limits.h>

extern "C"{
#include "soundpipe.h"
}

CK_DLL_CTOR(rpt_ctor);
CK_DLL_DTOR(rpt_dtor);

CK_DLL_MFUN(rpt_setBPM);
CK_DLL_MFUN(rpt_getBPM);

CK_DLL_MFUN(rpt_setRep);
CK_DLL_MFUN(rpt_getRep);

CK_DLL_MFUN(rpt_setDiv);
CK_DLL_MFUN(rpt_getDiv);

CK_DLL_MFUN(rpt_trig);

CK_DLL_TICK(rpt_tick);

t_CKINT rpt_data_offset = 0;

struct rptData {
    sp_data *sp;
    sp_rpt *rpt;
    SPFLOAT trig;
};

CK_DLL_QUERY(Rpt)
{
    QUERY->setname(QUERY, "Rpt");
    
    QUERY->begin_class(QUERY, "Rpt", "UGen");
    
    QUERY->add_ctor(QUERY, rpt_ctor);
    QUERY->add_dtor(QUERY, rpt_dtor);
    
    QUERY->add_ugen_func(QUERY, rpt_tick, NULL, 1, 1);
    
    QUERY->add_mfun(QUERY, rpt_setBPM, "float", "bpm");
    QUERY->add_arg(QUERY, "float", "arg");
    QUERY->add_mfun(QUERY, rpt_getBPM, "float", "bpm");
    
    QUERY->add_mfun(QUERY, rpt_setRep, "float", "rep");
    QUERY->add_arg(QUERY, "float", "arg");
    QUERY->add_mfun(QUERY, rpt_getRep, "float", "rep");
    
    QUERY->add_mfun(QUERY, rpt_setDiv, "float", "div");
    QUERY->add_arg(QUERY, "float", "arg");
    QUERY->add_mfun(QUERY, rpt_getDiv, "float", "div");
    
    QUERY->add_mfun(QUERY, rpt_trig, "void", "trig");
    
    rpt_data_offset = QUERY->add_mvar(QUERY, "int", "@rpt_data", false);
    
    QUERY->end_class(QUERY);

    return TRUE;
}


CK_DLL_CTOR(rpt_ctor)
{
    OBJ_MEMBER_INT(SELF, rpt_data_offset) = 0;
   
    rptData * data = new rptData;
    sp_create(&data->sp);
    data->sp->sr = API->vm->get_srate();
    sp_rpt_create(&data->rpt);
    sp_rpt_init(data->sp, data->rpt, 1.0);
    data->trig = 0;       
    OBJ_MEMBER_INT(SELF, rpt_data_offset) = (t_CKINT) data;
}

CK_DLL_DTOR(rpt_dtor)
{
    rptData * data = (rptData *) OBJ_MEMBER_INT(SELF, rpt_data_offset);
    if(data)
    {
        sp_rpt_destroy(&data->rpt);
        sp_destroy(&data->sp);

        delete data;
        OBJ_MEMBER_INT(SELF, rpt_data_offset) = 0;
        data = NULL;
    }
}

CK_DLL_TICK(rpt_tick)
{
    rptData * data = (rptData *) OBJ_MEMBER_INT(SELF, rpt_data_offset);
    
    sp_rpt_compute(data->sp, data->rpt, &data->trig, &in, out);
    data->trig = 0.0;
    return TRUE;
}

CK_DLL_MFUN(rpt_setBPM)
{
    rptData * data = (rptData *) OBJ_MEMBER_INT(SELF, rpt_data_offset);
    data->rpt->bpm = GET_NEXT_FLOAT(ARGS);
    RETURN->v_float = data->rpt->bpm;
}

CK_DLL_MFUN(rpt_getBPM)
{
    rptData * data = (rptData *) OBJ_MEMBER_INT(SELF, rpt_data_offset);
    RETURN->v_float = data->rpt->bpm;
}

CK_DLL_MFUN(rpt_setRep)
{
    rptData * data = (rptData *) OBJ_MEMBER_INT(SELF, rpt_data_offset);
    data->rpt->rep = GET_NEXT_FLOAT(ARGS);
    RETURN->v_float = data->rpt->rep;
}

CK_DLL_MFUN(rpt_getRep)
{
    rptData * data = (rptData *) OBJ_MEMBER_INT(SELF, rpt_data_offset);
    RETURN->v_float = data->rpt->rep;
}

CK_DLL_MFUN(rpt_setDiv)
{
    rptData * data = (rptData *) OBJ_MEMBER_INT(SELF, rpt_data_offset);
    data->rpt->div = GET_NEXT_FLOAT(ARGS);
    RETURN->v_float = data->rpt->div;
}

CK_DLL_MFUN(rpt_getDiv)
{
    rptData * data = (rptData *) OBJ_MEMBER_INT(SELF, rpt_data_offset);
    RETURN->v_float = data->rpt->div;
}

CK_DLL_MFUN(rpt_trig)
{
    rptData * data = (rptData *) OBJ_MEMBER_INT(SELF, rpt_data_offset);
    data->trig = 1.0;
}
