#include "data.h"

namespace pushbox {

int g_CurrentLevel = 1;
bool g_SoundOpen = true;
int g_Direct = 2;
bool g_Pushing = false;

Map g_Maps[MAX_LEVEL] = {
    {
        8, 8, 4, 4,
        {
            {{Empty}, {Empty}, {Wall}, {Wall}, {Wall}, {Empty}, {Empty}, {Empty}},
            {{Empty}, {Empty}, {Wall}, {Ground, true}, {Wall}, {Empty}, {Empty}, {Empty}},
            {{Empty}, {Empty}, {Wall}, {Ground}, {Wall}, {Wall}, {Wall}, {Wall}},
            {{Wall}, {Wall}, {Wall}, {Box}, {Ground}, {Box}, {Ground, true}, {Wall}},
            {{Wall}, {Ground, true}, {Ground}, {Box}, {Man}, {Wall}, {Wall}, {Wall}},
            {{Wall}, {Wall}, {Wall}, {Wall}, {Box}, {Wall}, {Empty}, {Empty}},
            {{Empty}, {Empty}, {Empty}, {Wall}, {Ground, true}, {Wall}, {Empty}, {Empty}},
            {{Empty}, {Empty}, {Empty}, {Wall}, {Wall}, {Wall}, {Empty}, {Empty}},
        },
    },
    {
        9, 9, 1, 1,
        {
            {{Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Empty}, {Empty}, {Empty}, {Empty}},
            {{Wall}, {Man}, {Ground}, {Ground}, {Wall}, {Empty}, {Empty}, {Empty}, {Empty}},
            {{Wall}, {Ground}, {Box}, {Box}, {Wall}, {Empty}, {Wall}, {Wall}, {Wall}},
            {{Wall}, {Ground}, {Box}, {Ground}, {Wall}, {Empty}, {Wall}, {Ground, true}, {Wall}},
            {{Wall}, {Wall}, {Wall}, {Ground}, {Wall}, {Wall}, {Wall}, {Ground, true}, {Wall}},
            {{Empty}, {Wall}, {Wall}, {Ground}, {Ground}, {Ground}, {Ground}, {Ground, true}, {Wall}},
            {{Empty}, {Wall}, {Ground}, {Ground}, {Ground}, {Wall}, {Ground}, {Ground}, {Wall}},
            {{Empty}, {Wall}, {Ground}, {Ground}, {Ground}, {Wall}, {Wall}, {Wall}, {Wall}},
            {{Empty}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Empty}, {Empty}, {Empty}},
        },
    },
    {
        10, 7, 3, 3,
        {
            {{Empty}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Empty}, {Empty}},
            {{Empty}, {Wall}, {Ground}, {Ground}, {Ground}, {Ground}, {Ground}, {Wall}, {Wall}, {Wall}},
            {{Wall}, {Wall}, {Box}, {Wall}, {Wall}, {Wall}, {Ground}, {Ground}, {Ground}, {Wall}},
            {{Wall}, {Ground}, {Ground}, {Man}, {Box}, {Ground}, {Ground}, {Box}, {Ground}, {Wall}},
            {{Wall}, {Ground}, {Ground, true}, {Ground, true}, {Wall}, {Ground}, {Box}, {Ground}, {Wall}, {Wall}},
            {{Wall}, {Wall}, {Ground, true}, {Ground, true}, {Wall}, {Ground}, {Ground}, {Ground}, {Wall}, {Empty}},
            {{Empty}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Empty}},
        },
    },
    {
        6, 8, 1, 2,
        {
            {{Empty}, {Wall}, {Wall}, {Wall}, {Wall}, {Empty}},
            {{Wall}, {Wall}, {Ground}, {Ground}, {Wall}, {Empty}},
            {{Wall}, {Man}, {Box}, {Ground}, {Wall}, {Empty}},
            {{Wall}, {Wall}, {Box}, {Ground}, {Wall}, {Wall}},
            {{Wall}, {Wall}, {Ground}, {Box}, {Ground}, {Wall}},
            {{Wall}, {Ground, true}, {Box}, {Ground}, {Ground}, {Wall}},
            {{Wall}, {Ground, true}, {Ground, true}, {Box, true}, {Ground, true}, {Wall}},
            {{Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}},
        },
    },
    {
        8, 8, 2, 2,
        {
            {{Empty}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Empty}, {Empty}},
            {{Empty}, {Wall}, {Ground}, {Ground}, {Wall}, {Wall}, {Wall}, {Empty}},
            {{Empty}, {Wall}, {Man}, {Box}, {Ground}, {Ground}, {Wall}, {Empty}},
            {{Wall}, {Wall}, {Wall}, {Ground}, {Wall}, {Ground}, {Wall}, {Wall}},
            {{Wall}, {Ground, true}, {Wall}, {Ground}, {Wall}, {Ground}, {Ground}, {Wall}},
            {{Wall}, {Ground, true}, {Box}, {Ground}, {Ground}, {Wall}, {Ground}, {Wall}},
            {{Wall}, {Ground, true}, {Ground}, {Ground}, {Ground}, {Box}, {Ground}, {Wall}},
            {{Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}},
        },
    },
    {
        10, 8, 8, 1,
        {
            {{Empty}, {Empty}, {Empty}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}},
            {{Empty}, {Empty}, {Wall}, {Wall}, {Ground}, {Ground}, {Wall}, {Ground}, {Man}, {Wall}},
            {{Empty}, {Empty}, {Wall}, {Ground}, {Ground}, {Ground}, {Wall}, {Ground}, {Ground}, {Wall}},
            {{Empty}, {Empty}, {Wall}, {Box}, {Ground}, {Box}, {Ground}, {Box}, {Ground}, {Wall}},
            {{Empty}, {Empty}, {Wall}, {Ground}, {Box}, {Wall}, {Wall}, {Ground}, {Ground}, {Wall}},
            {{Wall}, {Wall}, {Wall}, {Ground}, {Box}, {Ground}, {Wall}, {Ground}, {Wall}, {Wall}},
            {{Wall}, {Ground, true}, {Ground, true}, {Ground, true}, {Ground, true}, {Ground, true}, {Ground}, {Ground}, {Wall}, {Empty}},
            {{Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Empty}},
        },
    },
    {
        10, 7, 8, 3,
        {
            {{Empty}, {Empty}, {Empty}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Empty}},
            {{Empty}, {Wall}, {Wall}, {Wall}, {Ground}, {Ground}, {Ground}, {Ground}, {Wall}, {Empty}},
            {{Wall}, {Wall}, {Ground, true}, {Ground}, {Box}, {Wall}, {Wall}, {Ground}, {Wall}, {Wall}},
            {{Wall}, {Ground, true}, {Ground, true}, {Box}, {Ground}, {Box}, {Ground}, {Ground}, {Man}, {Wall}},
            {{Wall}, {Ground, true}, {Ground, true}, {Ground}, {Box}, {Ground}, {Box}, {Ground}, {Wall}, {Wall}},
            {{Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Ground}, {Ground}, {Wall}, {Empty}},
            {{Empty}, {Empty}, {Empty}, {Empty}, {Empty}, {Wall}, {Wall}, {Wall}, {Wall}, {Empty}},
        },
    },
    {
        11, 9, 8, 7,
        {
            {{Empty}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Empty}},
            {{Empty}, {Wall}, {Ground}, {Ground}, {Wall}, {Wall}, {Ground}, {Ground}, {Ground}, {Wall}, {Empty}},
            {{Empty}, {Wall}, {Ground}, {Ground}, {Ground}, {Box}, {Ground}, {Ground}, {Ground}, {Wall}, {Empty}},
            {{Empty}, {Wall}, {Box}, {Ground}, {Wall}, {Wall}, {Wall}, {Ground}, {Box}, {Wall}, {Empty}},
            {{Empty}, {Wall}, {Ground}, {Wall}, {Ground, true}, {Ground, true}, {Ground, true}, {Wall}, {Ground}, {Wall}, {Empty}},
            {{Wall}, {Wall}, {Ground}, {Wall}, {Ground, true}, {Ground, true}, {Ground, true}, {Wall}, {Ground}, {Wall}, {Wall}},
            {{Wall}, {Ground}, {Box}, {Ground}, {Ground}, {Box}, {Ground}, {Ground}, {Box}, {Ground}, {Wall}},
            {{Wall}, {Ground}, {Ground}, {Ground}, {Ground}, {Ground}, {Wall}, {Ground}, {Man}, {Ground}, {Wall}},
            {{Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}, {Wall}},
        },
    },
};

} // namespace pushbox

