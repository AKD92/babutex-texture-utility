

/************************************************************************************
    Entry point for the BABUTEX TEXTURE UTILITY PROGRAM
    Either UPDATE or MERGE operation will be available to execute from here
    Author:             Ashis Kumar Das
    Email:              akd.bracu@gmail.com
    GitHub:             https://github.com/AKD92
*************************************************************************************/








#include "btex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int btex_merge(void);



int main(void) {

    int optval;
    optval = btex_merge();
    return optval;
}
