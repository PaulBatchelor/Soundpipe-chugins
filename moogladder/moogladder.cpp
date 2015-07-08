#include "chuck_dl.h"
#include "chuck_def.h"
#include <stdio.h>
#include <limits.h>

extern "C"{
#include "soundpipe.h"
}

CK_DLL_CTOR(moogladder_ctor);
CK_DLL_DTOR(moogladder_dtor);

CK_DLL_MFUN(moogladder_setCutoff);
CK_DLL_MFUN(moogladder_getCutoff);

CK_DLL_MFUN(moogladder_setRes);
CK_DLL_MFUN(moogladder_getRes);

CK_DLL_TICK(moogladder_tick);

t_CKINT moogladder_data_offset = 0;

struct moogladderData {
    sp_data *sp;
    sp_moogladder *moogladder;
};

CK_DLL_QUERY(Moog)
{
    QUERY->setname(QUERY, "Moog");
    
    QUERY->begin_class(QUERY, "Moog", "UGen");
    
    QUERY->add_ctor(QUERY, moogladder_ctor);
    QUERY->add_dtor(QUERY, moogladder_dtor);
    
    QUERY->add_ugen_func(QUERY, moogladder_tick, NULL, 1, 1);
    
    QUERY->add_mfun(QUERY, moogladder_setCutoff, "float", "cutoff");
    QUERY->add_arg(QUERY, "float", "arg");
    QUERY->add_mfun(QUERY, moogladder_getCutoff, "float", "cutoff");
    
    QUERY->add_mfun(QUERY, moogladder_setRes, "float", "res");
    QUERY->add_arg(QUERY, "float", "arg");
    QUERY->add_mfun(QUERY, moogladder_getRes, "float", "res");
    
    moogladder_data_offset = QUERY->add_mvar(QUERY, "int", "@moogladder_data", false);
    
    QUERY->end_class(QUERY);

    return TRUE;
}


CK_DLL_CTOR(moogladder_ctor)
{
    OBJ_MEMBER_INT(SELF, moogladder_data_offset) = 0;
   
    moogladderData * data = new moogladderData;
    sp_create(&data->sp);
    data->sp->sr = API->vm->get_srate();
    sp_moogladder_create(&data->moogladder);
    sp_moogladder_init(data->sp, data->moogladder);
       
    OBJ_MEMBER_INT(SELF, moogladder_data_offset) = (t_CKINT) data;
}

CK_DLL_DTOR(moogladder_dtor)
{
    moogladderData * data = (moogladderData *) OBJ_MEMBER_INT(SELF, moogladder_data_offset);
    if(data)
    {
        sp_moogladder_destroy(&data->moogladder);
        sp_destroy(&data->sp);

        delete data;
        OBJ_MEMBER_INT(SELF, moogladder_data_offset) = 0;
        data = NULL;
    }
}

CK_DLL_TICK(moogladder_tick)
{
    moogladderData * data = (moogladderData *) OBJ_MEMBER_INT(SELF, moogladder_data_offset);
    
    sp_moogladder_compute(data->sp, data->moogladder, &in, out);
    return TRUE;
}

CK_DLL_MFUN(moogladder_setCutoff)
{
    moogladderData * data = (moogladderData *) OBJ_MEMBER_INT(SELF, moogladder_data_offset);
    data->moogladder->freq= GET_NEXT_FLOAT(ARGS);
    RETURN->v_float = data->moogladder->freq;
}

CK_DLL_MFUN(moogladder_getCutoff)
{
    moogladderData * data = (moogladderData *) OBJ_MEMBER_INT(SELF, moogladder_data_offset);
    RETURN->v_float = data->moogladder->freq;
}

CK_DLL_MFUN(moogladder_setRes)
{
    moogladderData * data = (moogladderData *) OBJ_MEMBER_INT(SELF, moogladder_data_offset);
    data->moogladder->res= GET_NEXT_FLOAT(ARGS);
    RETURN->v_float = data->moogladder->res;
}

CK_DLL_MFUN(moogladder_getRes)
{
    moogladderData * data = (moogladderData *) OBJ_MEMBER_INT(SELF, moogladder_data_offset);
    RETURN->v_float = data->moogladder->res;
}

