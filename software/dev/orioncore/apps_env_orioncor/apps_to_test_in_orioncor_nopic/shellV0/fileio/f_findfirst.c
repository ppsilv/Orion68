#include "fatfs/ff.h"


FRESULT ffindfirst (DIR* dp, FILINFO* fno, const TCHAR* path, const TCHAR* pattern){
    return f_findfirst ( dp, fno, path, pattern);
}
