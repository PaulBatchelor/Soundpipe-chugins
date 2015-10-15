#include "chuck_dl.h"
#include "chuck_def.h"
#include <stdio.h>
#include <limits.h>

extern "C"{
#include "tinyprop.h"
}

CK_DLL_CTOR(prop_ctor);
CK_DLL_DTOR(prop_dtor);


CK_DLL_MFUN(prop_parseString);

CK_DLL_MFUN(prop_getNext);

CK_DLL_MFUN(prop_setScale);
CK_DLL_MFUN(prop_getScale);

CK_DLL_MFUN(prop_getType);
CK_DLL_MFUN(prop_getDur);

CK_DLL_TICK(prop_tick);

t_CKINT prop_data_offset = 0;

struct propData {
    prop_data *prop;
    int parsed;
    prop_event evt;
};

CK_DLL_QUERY(Prop)
{
    QUERY->setname(QUERY, "Prop");
    
    QUERY->begin_class(QUERY, "Prop", "UGen");
    
    QUERY->add_ctor(QUERY, prop_ctor);
    QUERY->add_dtor(QUERY, prop_dtor);
    
    QUERY->add_ugen_func(QUERY, prop_tick, NULL, 1, 1);
    
    QUERY->add_mfun(QUERY, prop_parseString, "void", "parse");
    QUERY->add_arg(QUERY, "string", "prop");
    
    QUERY->add_mfun(QUERY, prop_setScale, "float", "scale");
    QUERY->add_arg(QUERY, "float", "scale");
    QUERY->add_mfun(QUERY, prop_getScale, "float", "scale");
    
    QUERY->add_mfun(QUERY, prop_getNext, "void", "next");
    
    QUERY->add_mfun(QUERY, prop_getDur, "float", "dur");
    QUERY->add_mfun(QUERY, prop_getType, "float", "type");
    
    prop_data_offset = QUERY->add_mvar(QUERY, "int", "@prop_data", false);
    
    QUERY->end_class(QUERY);

    return TRUE;
}


CK_DLL_CTOR(prop_ctor)
{
    OBJ_MEMBER_INT(SELF, prop_data_offset) = 0;
   
    propData * data = new propData;
    prop_create(&data->prop);
    data->parsed = 0;
    data->evt.val = 0;
    data->evt.type = 0;
           
    OBJ_MEMBER_INT(SELF, prop_data_offset) = (t_CKINT) data;
}

CK_DLL_DTOR(prop_dtor)
{
    propData * data = (propData *) OBJ_MEMBER_INT(SELF, prop_data_offset);
    if(data)
    {
        prop_destroy(&data->prop);
        delete data;
        OBJ_MEMBER_INT(SELF, prop_data_offset) = 0;
        data = NULL;
    }
}

CK_DLL_TICK(prop_tick)
{
    *out = 0;
    return TRUE;
}

CK_DLL_MFUN(prop_setScale)
{
    propData * data = (propData *) OBJ_MEMBER_INT(SELF, prop_data_offset);
    data->prop->scale = GET_NEXT_FLOAT(ARGS);
    RETURN->v_float = data->prop->scale;
}

CK_DLL_MFUN(prop_getScale)
{
    propData * data = (propData *) OBJ_MEMBER_INT(SELF, prop_data_offset);
    RETURN->v_float = data->prop->scale;
}

CK_DLL_MFUN(prop_getNext)
{
    propData * data = (propData *) OBJ_MEMBER_INT(SELF, prop_data_offset);
    if(data->parsed) data->evt = prop_next(data->prop);
}

CK_DLL_MFUN(prop_getType)
{
    propData * data = (propData *) OBJ_MEMBER_INT(SELF, prop_data_offset);
    RETURN->v_float = data->evt.type;
}

CK_DLL_MFUN(prop_getDur)
{
    propData * data = (propData *) OBJ_MEMBER_INT(SELF, prop_data_offset);
    RETURN->v_float = prop_time(data->prop, data->evt);
}

CK_DLL_MFUN(prop_parseString)
{
    propData * data = (propData *) OBJ_MEMBER_INT(SELF, prop_data_offset);
    Chuck_String * ckstring = GET_CK_STRING(ARGS);
    const char * str = ckstring->str.c_str();
    if(!data->parsed) {
        data->parsed = 1;
        plumber_parse_string(&data->pd, str);
        //prop_parse(data->prop, str);        
    }
}
