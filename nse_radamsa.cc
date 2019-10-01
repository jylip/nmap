#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "nse_lua.h"

#include <radamsa.h>


static int radamsa_init(lua_State *L) {
    // no handle or anything, radamsa has some weird heap implementation in static memory.
    // this still needs to be called before anything.
    init();
    return 1;
}

static int radamsa_radamsa(lua_State *L) {

    // incoming arguments are in the lua stack: input buffer, max output length and the seed for radamsa
    // we allocate a block of memory and pass the contents to caller via lua stack before freeing the block.

    size_t input_len;
    unsigned char* input = (unsigned char*) luaL_checklstring(L, 1, &input_len);
    size_t output_max_len = luaL_checkinteger(L, 2);
    unsigned int seed = luaL_checkinteger(L, 3);

    unsigned char * output = (unsigned char *) malloc(output_max_len);
    if (!output) {
        return luaL_error(L, "could not allocate memory!");
    }

    size_t len = radamsa(input, input_len, output, output_max_len, seed);
    lua_pushlstring(L, (char *) output, len);
    free(output);
    return 1;
}

LUALIB_API int luaopen_radamsa(lua_State *L) {
    const luaL_Reg radamsa_funcs[] = {
        {"init", radamsa_init},
        {"radamsa", radamsa_radamsa},
        {NULL, NULL}
    };

    luaL_newlib(L, radamsa_funcs);
    return 1;
}
