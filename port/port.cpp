#include "chuck_dl.h"
#include "chuck_def.h"
#include <stdio.h>
#include <limits.h>

extern "C"{
#include "soundpipe.h"
}

CK_DLL_CTOR(port_ctor);
CK_DLL_DTOR(port_dtor);

CK_DLL_MFUN(port_setHTime);
CK_DLL_MFUN(port_getHTime);

CK_DLL_TICK(port_tick);

t_CKINT port_data_offset = 0;

struct portData {
    sp_data *sp;
    sp_port *port;
};

CK_DLL_QUERY(Port)
{
    QUERY->setname(QUERY, "Port");
    
    QUERY->begin_class(QUERY, "Port", "UGen");
    
    QUERY->add_ctor(QUERY, port_ctor);
    QUERY->add_dtor(QUERY, port_dtor);
    
    QUERY->add_ugen_func(QUERY, port_tick, NULL, 1, 1);
    
    QUERY->add_mfun(QUERY, port_setHTime, "float", "htime");
    QUERY->add_arg(QUERY, "float", "arg");
    
    QUERY->add_mfun(QUERY, port_getHTime, "float", "htime");
    
    port_data_offset = QUERY->add_mvar(QUERY, "int", "@port_data", false);
    
    QUERY->end_class(QUERY);

    return TRUE;
}


CK_DLL_CTOR(port_ctor)
{
    OBJ_MEMBER_INT(SELF, port_data_offset) = 0;
   
    portData * data = new portData;
    sp_create(&data->sp);
    data->sp->sr = API->vm->get_srate();
    sp_port_create(&data->port);
    sp_port_init(data->sp, data->port, 0.02);
       
    OBJ_MEMBER_INT(SELF, port_data_offset) = (t_CKINT) data;
}

CK_DLL_DTOR(port_dtor)
{
    portData * data = (portData *) OBJ_MEMBER_INT(SELF, port_data_offset);
    if(data)
    {
        sp_port_destroy(&data->port);
        sp_destroy(&data->sp);

        delete data;
        OBJ_MEMBER_INT(SELF, port_data_offset) = 0;
        data = NULL;
    }
}

CK_DLL_TICK(port_tick)
{
    portData * data = (portData *) OBJ_MEMBER_INT(SELF, port_data_offset);
    
    sp_port_compute(data->sp, data->port, &in, out);
    return TRUE;
}

CK_DLL_MFUN(port_setHTime)
{
    portData * data = (portData *) OBJ_MEMBER_INT(SELF, port_data_offset);
    data->port->htime= GET_NEXT_FLOAT(ARGS);
    RETURN->v_float = data->port->htime;
}

CK_DLL_MFUN(port_getHTime)
{
    portData * data = (portData *) OBJ_MEMBER_INT(SELF, port_data_offset);
    RETURN->v_float = data->port->htime;
}

