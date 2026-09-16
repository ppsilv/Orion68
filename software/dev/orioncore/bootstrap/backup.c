          .long    0x000FFFF0      /*                      0x0000*//* Pilha no topo real da RAM (ajuste se seu limite for outro) */
          .long    _start          /*                      0x0004*/
          .long    SvcBusError     /*0x00080000*/      /*SvcBusError           0x0008*/
          .long    Svcaddress_err  /*0x00080006*/      /*Svcaddress_err        0x000c*/
          .long    SvcIllegalIns   /*0x0008000c*/      /*SvcIllegalIns         0x0010*/
          .long    SvcDiv0_handler /*0x00080012*/      /*SvcDiv0_handler       0x0014*/
          .long    0x00080018      /*SvcChkIns             0x0018*/
          .long    0x0008001e      /*SvcTrapvIns           0x001c*/
          .long    0x00080024      /*SvcPrivViolation      0x0020*/
          .long    0x0008002a      /*SvcTrace              0x0024*/
          .long    0x00080030      /*SvcLineA              0x0028*/
          .long    0x00080036      /*SvcLineF              0x002c*/
          .long    0x0008003c      /*bad_exception         0x0030*/
          .long    0x00080042      /*bad_exception         0x0034*/
          .long    0x00080048      /*bad_exception         0x0038*/
          .long    0x0008004e      /*bad_exception         0x003c*/
          .long    0x00080054      /*bad_exception         0x0040*/
          .long    0x0008005a      /*bad_exception         0x0044*/
          .long    0x00080060      /*bad_exception         0x0048*/
          .long    0x00080066      /*bad_exception         0x004c*/
          .long    0x0008006c      /*bad_exception         0x0050*/
          .long    0x00080072      /*bad_exception         0x0054*/
          .long    0x00080078      /*bad_exception         0x0058*/
          .long    0x0008007e      /*bad_exception         0x005c*/
          .long    0x00080084      /*SpuriousHandler       0x0060*/
          .long    0x0008008a      /*Int1Handler           0x0064*/
          .long    Int2Handler          /*0x00080090*/      /*Int2Handler           0x0068*/
          .long    Int3Handler          /*0x00080096*/      /*Int3Handler           0x006c*/
          .long    0x0008009c      /*Int4Handler           0x0070*/
          .long    0x000800a2      /*Int5Handler           0x0074*/
          .long    0x000800a8      /*Int6Handler           0x0078*/
          .long    0x000800ae      /*Int7Handler           0x007c*/
          .long    0x000800b4      /*Trap0Handler          0x0080*/
          .long    0x000800ba      /*Trap1Handler          0x0084*/
          .long    0x000800c0      /*Trap2Handler          0x0088*/
          .long    0x000800c6      /*Trap3Handler          0x008c*/
          .long    0x000800cc      /*Trap4Handler          0x0090*/
          .long    0x000800d2      /*Trap5Handler          0x0094*/
          .long    0x000800d8      /*Trap6Handler          0x0098*/
          .long    0x000800de      /*Trap7Handler          0x009c*/
          .long    0x000800e4      /*Trap8Handler          0x00a0*/
          .long    0x000800ea      /*Trap9Handler          0x00a4*/
          .long    0x000800f0      /*TrapAHandler          0x00a8*/
          .long    0x000800f6      /*TrapBHandler          0x00ac*/
          .long    0x000800fc      /*TrapCHandler          0x00b0*/
          .long    0x00080102      /*TrapDHandler          0x00b4*/
          .long    0x00080108      /*TrapEHandler          0x00b8*/
          .long    0x0008010e      /*TrapFHandler          0x00bc*/
          /* User defined area for excetions 206 entries */
          /*.long    0x00080114      User defined          0x00c0*/
          /*.long    0x000805f4      User defined          0x03fc*/
