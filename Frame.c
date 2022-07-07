#include "Frame.h"


/**
 * Function that allows to create a struct Frame from a string.
 * @param string String containing the string frame.
 * @return Frame with the information organized.
 */
Frame createFrameFromString(char string[256]) {
    int i;
    Frame frame;

    //Init strings with 0 to avoid saving junk.
    memset(frame.origin, 0, sizeof(frame.origin));
    memset(frame.data, 0, sizeof(frame.data));


    i = 0;
    while (i < ORIGIN_LENGTH) {
        frame.origin[i] = string[i];
        i++;
    }

    frame.type = string[15];

    i = 16;
    while (i < 256) {
        frame.data[i - 16] = string[i];
        i++;
    }


    return frame;

}