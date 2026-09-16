#include "fatfs/ff.h"

FRESULT frename(const TCHAR* path){
    return f_unlink(  path);
}
