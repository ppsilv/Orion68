#include "fatfs/ff.h"


FRESULT ffindnext (DIR* dp, FILINFO* fno){
    return f_findnext ( dp, fno);
}
