/* 
Tinyprop
By Paul Batchelor

Tinyprop a tiny C implementation of prop, a proportional rhythmic notation system.

*/
#include <stdlib.h>
#include <stdio.h>
#include "tinyprop.h"

static void mode_insert(prop_data *pd, char type) 
{
    prop_event *evt = malloc(sizeof(prop_event));
    evt->type = type;
    evt->val = pd->mul;
    evt->pos = pd->num;
    pd->last->next = evt;
    pd->last = evt;
    pd->num++;
    
}

static void mode_setdiv(prop_data *pd, char n)
{
    if(pd->tmp == 0 && n == 0) n = 1;
    pd->tmp *= 10;
    pd->tmp += n;
}

static void mode_setmul(prop_data *pd)
{
    pd->mul *= pd->tmp;
    pd->div = pd->tmp;
    pd->tmp = 0;
}

static void mode_unsetmul(prop_data *pd)
{
    if(pd->div > 0) pd->mul /= pd->div;
}

int prop_create(prop_data **pd)
{
    *pd = malloc(sizeof(prop_data));
    prop_data *pdp = *pd;

    pdp->num = 0;
    pdp->mul = 1;
    pdp->div = 0;
    pdp->scale = 1;   
    pdp->mode = PMODE_INIT;
    pdp->pos = 1;
    pdp->evtpos = 0;
    pdp->last = &pdp->root;    
    pdp->tmp = 0;
    return PSTATUS_OK;
}

int prop_parse(prop_data *pd, const char *str)
{
    char c;
    while(*str != 0) {
        c = str[0];

        switch(c) {
        
            case '+':
                mode_insert(pd, PTYPE_ON);
                break;
            case '-':
                mode_insert(pd, PTYPE_OFF);
                break;
                
            case '0':
                mode_setdiv(pd, 0);    
                break;
            case '1':
                mode_setdiv(pd, 1);    
                break;
            case '2':
                mode_setdiv(pd, 2);    
                break;
            case '3':
                mode_setdiv(pd, 3);    
                break;
            case '4':
                mode_setdiv(pd, 4);    
                break;
            case '5':
                mode_setdiv(pd, 5);    
                break;
            case '6':
                mode_setdiv(pd, 6);    
                break;
            case '7':
                mode_setdiv(pd, 7);    
                break;
            case '8':
                mode_setdiv(pd, 8);    
                break;
            case '9':
                mode_setdiv(pd, 9);    
                break;
            case '(':
                mode_setmul(pd);
                break;
            case ')':
                mode_unsetmul(pd);
                break;
            case ' ': break;
            case '\n': break;
            case '\t': break;
                
            default:
                return PSTATUS_NOTOK;
        }        
        pd->pos++;        
        str++;
    }
    pd->last = &pd->root;
    return PSTATUS_OK;
}

prop_event prop_next(prop_data *pd)
{
    if(pd->evtpos >= pd->num ) {
        pd->last = &pd->root;
        pd->evtpos = 0;
    }
    prop_event p = *pd->last->next;
    pd->last = pd->last->next;
    pd->evtpos++;
    return p;
}

float prop_time(prop_data *pd, prop_event evt)
{
    return 1.0 * (pd->scale / evt.val);
}

int prop_destroy(prop_data **pd)
{
    uint32_t i;
    prop_data *pdp = *pd;
    prop_event *evt, *next;
    
    evt = pdp->root.next;
    
    for(i = 0; i < pdp->num; i++) {
        next = evt->next;
        free(evt);
        evt = next;
    }    
    
    free(*pd);
    return PSTATUS_OK;
}

