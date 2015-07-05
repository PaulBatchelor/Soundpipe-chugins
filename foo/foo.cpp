#include "chuck_dl.h"
#include "chuck_def.h"
#include <stdio.h>
#include <limits.h>

extern "C"{
#include "soundpipe.h"
}

CK_DLL_CTOR(foo_ctor);
CK_DLL_DTOR(foo_dtor);

CK_DLL_MFUN(foo_setVar);
CK_DLL_MFUN(foo_getVar);

CK_DLL_TICK(foo_tick);

t_CKINT foo_data_offset = 0;

struct fooData {
    sp_data *sp;
    sp_foo *foo;
};

CK_DLL_QUERY(Foo)
{
    QUERY->setname(QUERY, "Foo");
    
    QUERY->begin_class(QUERY, "Foo", "UGen");
    
    QUERY->add_ctor(QUERY, foo_ctor);
    QUERY->add_dtor(QUERY, foo_dtor);
    
    QUERY->add_ugen_func(QUERY, foo_tick, NULL, 1, 1);
    
    QUERY->add_mfun(QUERY, foo_setVar, "float", "var");
    QUERY->add_arg(QUERY, "float", "arg");
    
    QUERY->add_mfun(QUERY, foo_getVar, "float", "var");
    
    foo_data_offset = QUERY->add_mvar(QUERY, "int", "@foo_data", false);
    
    QUERY->end_class(QUERY);

    return TRUE;
}


CK_DLL_CTOR(foo_ctor)
{
    OBJ_MEMBER_INT(SELF, foo_data_offset) = 0;
   
    fooData * data = new fooData;
    sp_create(&data->sp);
    data->sp->sr = API->vm->get_srate();
    sp_foo_create(&data->foo);
    sp_foo_init(data->sp, data->foo);
       
    OBJ_MEMBER_INT(SELF, foo_data_offset) = (t_CKINT) data;
}

CK_DLL_DTOR(foo_dtor)
{
    fooData * data = (fooData *) OBJ_MEMBER_INT(SELF, foo_data_offset);
    if(data)
    {
        sp_foo_destroy(&data->foo);
        sp_destroy(&data->sp);

        delete data;
        OBJ_MEMBER_INT(SELF, foo_data_offset) = 0;
        data = NULL;
    }
}

CK_DLL_TICK(foo_tick)
{
    fooData * data = (fooData *) OBJ_MEMBER_INT(SELF, foo_data_offset);
    
    sp_foo_compute(data->sp, data->foo, &in, out);
    return TRUE;
}

CK_DLL_MFUN(foo_setVar)
{
    fooData * data = (fooData *) OBJ_MEMBER_INT(SELF, foo_data_offset);
    data->foo->var = GET_NEXT_FLOAT(ARGS);
    RETURN->v_float = data->foo->var;
}

CK_DLL_MFUN(foo_getVar)
{
    fooData * data = (fooData *) OBJ_MEMBER_INT(SELF, foo_data_offset);
    RETURN->v_float = data->foo->var;
}

