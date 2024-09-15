#include <stdio.h>
#include "display.h"
#include "texture.h"

int texture_width = 64;
int texture_height = 64;

tex2_t tex2_clone(tex2_t *t)
{
    tex2_t result = {
        .u = t->u,
        .v = t->v,
    };
    return result;
}