#include "chuck_dl.h"
#include "chuck_def.h"
#include <stdio.h>
#include <limits.h>

extern "C"{
#include "soundpipe.h"
}

CK_DLL_CTOR(tenv_ctor);
CK_DLL_DTOR(tenv_dtor);

CK_DLL_MFUN(tenv_setAttack);
CK_DLL_MFUN(tenv_getAttack);

CK_DLL_MFUN(tenv_setHold);
CK_DLL_MFUN(tenv_getHold);

CK_DLL_MFUN(tenv_setRelease);
CK_DLL_MFUN(tenv_getRelease);


CK_DLL_MFUN(tenv_setMode);
CK_DLL_MFUN(tenv_getMode);

CK_DLL_MFUN(tenv_trig);

CK_DLL_TICK(tenv_tick);

t_CKINT tenv_data_offset = 0;

struct TenvData {
    sp_data *sp;
    sp_tenv *tenv;
    SPFLOAT trig;
    SPFLOAT mode;
};

CK_DLL_QUERY(Butlp)
{
    QUERY->setname(QUERY, "TEnv");
    
    QUERY->begin_class(QUERY, "TEnv", "UGen");
    
    QUERY->add_ctor(QUERY, tenv_ctor);
    QUERY->add_dtor(QUERY, tenv_dtor);
    
    QUERY->add_ugen_func(QUERY, tenv_tick, NULL, 1, 1);
    
    QUERY->add_mfun(QUERY, tenv_setAttack, "float", "atk");
    QUERY->add_arg(QUERY, "float", "arg");
    QUERY->add_mfun(QUERY, tenv_getAttack, "float", "atk");
    
    QUERY->add_mfun(QUERY, tenv_setHold, "float", "hold");
    QUERY->add_arg(QUERY, "float", "arg");
    QUERY->add_mfun(QUERY, tenv_getHold, "float", "hold");
    
    QUERY->add_mfun(QUERY, tenv_setRelease, "float", "rel");
    QUERY->add_arg(QUERY, "float", "arg");
    QUERY->add_mfun(QUERY, tenv_getRelease, "float", "rel");
    
    QUERY->add_mfun(QUERY, tenv_setMode, "float", "mode");
    QUERY->add_arg(QUERY, "float", "arg");
    QUERY->add_mfun(QUERY, tenv_getMode, "float", "mode");
    
    QUERY->add_mfun(QUERY, tenv_trig, "void", "trig");
    
    tenv_data_offset = QUERY->add_mvar(QUERY, "int", "@tenv_data", false);
    
    QUERY->end_class(QUERY);

    return TRUE;
}


CK_DLL_CTOR(tenv_ctor)
{
    OBJ_MEMBER_INT(SELF, tenv_data_offset) = 0;
   
    TenvData * data = new TenvData;
    sp_create(&data->sp);
    data->sp->sr = API->vm->get_srate();
    sp_tenv_create(&data->tenv);
    sp_tenv_init(data->sp, data->tenv, 0.1, 0.1, 0.1);
    data->tenv->sigmode = 1; 
    data->trig = 0;
    OBJ_MEMBER_INT(SELF, tenv_data_offset) = (t_CKINT) data;
}

CK_DLL_DTOR(tenv_dtor)
{
    TenvData * data = (TenvData *) OBJ_MEMBER_INT(SELF, tenv_data_offset);
    if(data)
    {
        sp_tenv_destroy(&data->tenv);
        sp_destroy(&data->sp);

        delete data;
        OBJ_MEMBER_INT(SELF, tenv_data_offset) = 0;
        data = NULL;
    }
}

CK_DLL_TICK(tenv_tick)
{
    TenvData * data = (TenvData *) OBJ_MEMBER_INT(SELF, tenv_data_offset);
    data->tenv->in = in;
    
    if(data->tenv->sigmode = 1) {
        sp_tenv_compute(data->sp, data->tenv, &data->trig, out);
    } else {
        sp_tenv_compute(data->sp, data->tenv, &in, out);
    }
    
    data->trig = 0;
    return TRUE;
}

CK_DLL_MFUN(tenv_setAttack)
{
    TenvData * data = (TenvData *) OBJ_MEMBER_INT(SELF, tenv_data_offset);
    data->tenv->atk = GET_NEXT_FLOAT(ARGS);
    RETURN->v_float = data->tenv->atk;
}

CK_DLL_MFUN(tenv_getAttack)
{
    TenvData * data = (TenvData *) OBJ_MEMBER_INT(SELF, tenv_data_offset);
    RETURN->v_float = data->tenv->atk;
}


CK_DLL_MFUN(tenv_setHold)
{
    TenvData * data = (TenvData *) OBJ_MEMBER_INT(SELF, tenv_data_offset);
    data->tenv->hold = GET_NEXT_FLOAT(ARGS);
    RETURN->v_float = data->tenv->hold;
}

CK_DLL_MFUN(tenv_getHold)
{
    TenvData * data = (TenvData *) OBJ_MEMBER_INT(SELF, tenv_data_offset);
    RETURN->v_float = data->tenv->hold;
}

CK_DLL_MFUN(tenv_setRelease)
{
    TenvData * data = (TenvData *) OBJ_MEMBER_INT(SELF, tenv_data_offset);
    data->tenv->rel = GET_NEXT_FLOAT(ARGS);
    RETURN->v_float = data->tenv->rel;
}

CK_DLL_MFUN(tenv_getRelease)
{
    TenvData * data = (TenvData *) OBJ_MEMBER_INT(SELF, tenv_data_offset);
    RETURN->v_float = data->tenv->rel;
}

CK_DLL_MFUN(tenv_setMode)
{
    TenvData * data = (TenvData *) OBJ_MEMBER_INT(SELF, tenv_data_offset);
    data->tenv->sigmode = GET_NEXT_FLOAT(ARGS);
    RETURN->v_float = data->tenv->sigmode;
}

CK_DLL_MFUN(tenv_getMode)
{
    TenvData * data = (TenvData *) OBJ_MEMBER_INT(SELF, tenv_data_offset);
    RETURN->v_float = data->tenv->sigmode;
}

CK_DLL_MFUN(tenv_trig)
{
    TenvData * data = (TenvData *) OBJ_MEMBER_INT(SELF, tenv_data_offset);
    data->trig = 1.0;
}
