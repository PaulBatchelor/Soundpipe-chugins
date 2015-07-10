#include "chuck_dl.h"
#include "chuck_def.h"
#include <stdio.h>
#include <limits.h>

extern "C"{
#include "soundpipe.h"
}

CK_DLL_CTOR(del_ctor);
CK_DLL_DTOR(del_dtor);

CK_DLL_MFUN(del_setMaxdel);
CK_DLL_MFUN(del_getMaxdel);

CK_DLL_MFUN(del_setDelTime);
CK_DLL_MFUN(del_getDelTime);

CK_DLL_MFUN(del_setFeedback);
CK_DLL_MFUN(del_getFeedback);

CK_DLL_MFUN(del_setDry);
CK_DLL_MFUN(del_getDry);

CK_DLL_MFUN(del_setWet);
CK_DLL_MFUN(del_getWet);

CK_DLL_TICK(del_tick);

t_CKINT del_data_offset = 0;

struct delData {
    sp_data *sp;
    sp_del *del;
    int delset;
    SPFLOAT feedback;
    SPFLOAT last;
    SPFLOAT dry;
    SPFLOAT wet;
};

CK_DLL_QUERY(Delay)
{
    QUERY->setname(QUERY, "Delay3");
    
    QUERY->begin_class(QUERY, "Delay3", "UGen");
    
    QUERY->add_ctor(QUERY, del_ctor);
    QUERY->add_dtor(QUERY, del_dtor);
    
    QUERY->add_ugen_func(QUERY, del_tick, NULL, 1, 1);
    
    QUERY->add_mfun(QUERY, del_setMaxdel, "float", "maxTime");
    QUERY->add_arg(QUERY, "float", "arg");
    QUERY->add_mfun(QUERY, del_getMaxdel, "float", "maxTime");
    
    QUERY->add_mfun(QUERY, del_setDelTime, "float", "time");
    QUERY->add_arg(QUERY, "float", "arg");
    QUERY->add_mfun(QUERY, del_getDelTime, "float", "time");
    
    QUERY->add_mfun(QUERY, del_setFeedback, "float", "feedback");
    QUERY->add_arg(QUERY, "float", "arg");
    QUERY->add_mfun(QUERY, del_getFeedback, "float", "feedback");
    
    QUERY->add_mfun(QUERY, del_setDry, "float", "dry");
    QUERY->add_arg(QUERY, "float", "arg");
    QUERY->add_mfun(QUERY, del_getDry, "float", "dry");
    
    QUERY->add_mfun(QUERY, del_setWet, "float", "wet");
    QUERY->add_arg(QUERY, "float", "arg");
    QUERY->add_mfun(QUERY, del_getWet, "float", "wet");
    
    del_data_offset = QUERY->add_mvar(QUERY, "int", "@del3_data", false);
    
    QUERY->end_class(QUERY);

    return TRUE;
}


CK_DLL_CTOR(del_ctor)
{
    OBJ_MEMBER_INT(SELF, del_data_offset) = 0;
   
    delData * data = new delData;
    sp_create(&data->sp);
    data->sp->sr = API->vm->get_srate();
    sp_del_create(&data->del);
    data->delset = 0;
    data->last = 0;
    data->feedback = 0;
    data->dry = 0.5;
    data->wet = 0.5;
  
    OBJ_MEMBER_INT(SELF, del_data_offset) = (t_CKINT) data;
}

CK_DLL_DTOR(del_dtor)
{
    delData * data = (delData *) OBJ_MEMBER_INT(SELF, del_data_offset);
    if(data)
    {
        sp_del_destroy(&data->del);
        sp_destroy(&data->sp);

        delete data;
        OBJ_MEMBER_INT(SELF, del_data_offset) = 0;
        data = NULL;
    }
}

CK_DLL_TICK(del_tick)
{
    delData * data = (delData *) OBJ_MEMBER_INT(SELF, del_data_offset);
    if(data->delset == 1) {
        SPFLOAT delIn = in + data->last * data->feedback;
        SPFLOAT delOut = 0;
        sp_del_compute(data->sp, data->del, &delIn, &delOut);
        data->last = delOut;
        //*out = (data->mix * delOut) + (1 - data->mix) * in;
        *out = (data->wet * delOut) + (data->dry * in);
    } else if (data->delset == 0) {
        fprintf(stderr, "Delay3: please set the max delay variable before running!\n");
        data->delset = -1;
    }
    return TRUE;
}

CK_DLL_MFUN(del_setMaxdel)
{
    delData * data = (delData *) OBJ_MEMBER_INT(SELF, del_data_offset);
    SPFLOAT maxdel = GET_NEXT_FLOAT(ARGS);
    if(data->delset != 1) {
        sp_del_init(data->sp, data->del, maxdel);
        data->delset = 1;
    }
    RETURN->v_float = data->del->maxdel;
}

CK_DLL_MFUN(del_getMaxdel)
{
    delData * data = (delData *) OBJ_MEMBER_INT(SELF, del_data_offset);
    if(data->delset == 1) RETURN->v_float = data->del->maxdel;
    else RETURN->v_float = 0;
}

CK_DLL_MFUN(del_setDelTime)
{
    delData * data = (delData *) OBJ_MEMBER_INT(SELF, del_data_offset);
    if(data->delset == 1) {
        data->del->del = GET_NEXT_FLOAT(ARGS);
        RETURN->v_float = data->del->del;
    } else {
        RETURN->v_float = 0;
    }
}

CK_DLL_MFUN(del_getDelTime)
{
    delData * data = (delData *) OBJ_MEMBER_INT(SELF, del_data_offset);
    
    if(data->delset == 1) RETURN->v_float = data->del->del;
    else RETURN->v_float = 0;
}

CK_DLL_MFUN(del_setFeedback)
{
    delData * data = (delData *) OBJ_MEMBER_INT(SELF, del_data_offset);
    data->feedback = GET_NEXT_FLOAT(ARGS);
    RETURN->v_float = data->feedback;
}

CK_DLL_MFUN(del_getFeedback)
{
    delData * data = (delData *) OBJ_MEMBER_INT(SELF, del_data_offset);
    RETURN->v_float = data->feedback;
}

CK_DLL_MFUN(del_setDry)
{

    delData * data = (delData *) OBJ_MEMBER_INT(SELF, del_data_offset);
    data->dry= GET_NEXT_FLOAT(ARGS);
    RETURN->v_float = data->dry;
}

CK_DLL_MFUN(del_getDry)
{

    delData * data = (delData *) OBJ_MEMBER_INT(SELF, del_data_offset);
    RETURN->v_float = data->dry;
}

CK_DLL_MFUN(del_setWet)
{

    delData * data = (delData *) OBJ_MEMBER_INT(SELF, del_data_offset);
    data->wet = GET_NEXT_FLOAT(ARGS);
    RETURN->v_float = data->wet;
}

CK_DLL_MFUN(del_getWet)
{

    delData * data = (delData *) OBJ_MEMBER_INT(SELF, del_data_offset);
    RETURN->v_float = data->dry;
}
