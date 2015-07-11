#include <stdint.h>

enum {
PTYPE_OFF,
PTYPE_ON,
PMODE_INSERT,
PMODE_SETDIV,
PMODE_SETMUL,
PMODE_UNSETMUL,
PMODE_INIT,
PSTATUS_NOTOK,
PSTATUS_OK
};

typedef struct prop_event {
    char type;
    uint32_t pos;
    uint32_t val;
    struct prop_event *next;
} prop_event;

typedef struct {
    uint32_t mul;
    uint32_t div;
    uint32_t tmp;
    uint32_t num;
    float scale;
    int mode;
    uint32_t pos;
    uint32_t evtpos;
    prop_event root;
    prop_event *last;
} prop_data;

int prop_create(prop_data **pd);
int prop_parse(prop_data *pd, const char *str);
prop_event prop_next(prop_data *pd);
float prop_time(prop_data *pd, prop_event evt);
int prop_destroy(prop_data **pd);