#include "fatfs/ff.h"


FSIZE_t ftell(FIL* fp){
    f_tell(fp);
}

