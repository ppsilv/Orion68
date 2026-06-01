
vbug2-1.elf:     file format elf32-m68k


Disassembly of section .text:

00000000 <_vectors>:
       0:	000f           	.short 0x000f
       2:	fff0           	.short 0xfff0
       4:	0000 00c0      	orib #-64,%d0
       8:	0000 0b20      	orib #32,%d0
       c:	0000 0b2a      	orib #42,%d0
      10:	0000 0b34      	orib #52,%d0
      14:	0000 0b3e      	orib #62,%d0
      18:	0000 0b48      	orib #72,%d0
      1c:	0000 0b52      	orib #82,%d0
      20:	0000 0b5c      	orib #92,%d0
      24:	0000 0b66      	orib #102,%d0
      28:	0000 0b70      	orib #112,%d0
      2c:	0000 0b7a      	orib #122,%d0
      30:	0000 0b84      	orib #-124,%d0
      34:	0000 0b84      	orib #-124,%d0
      38:	0000 0b84      	orib #-124,%d0
      3c:	0000 0b84      	orib #-124,%d0
      40:	0000 0b84      	orib #-124,%d0
      44:	0000 0b84      	orib #-124,%d0
      48:	0000 0b84      	orib #-124,%d0
      4c:	0000 0b84      	orib #-124,%d0
      50:	0000 0b84      	orib #-124,%d0
      54:	0000 0b84      	orib #-124,%d0
      58:	0000 0b84      	orib #-124,%d0
      5c:	0000 0b84      	orib #-124,%d0
      60:	0000 0b8e      	orib #-114,%d0
      64:	0000 0b98      	orib #-104,%d0
      68:	0000 0ba2      	orib #-94,%d0
      6c:	0000 0bac      	orib #-84,%d0
      70:	0000 0bb6      	orib #-74,%d0
      74:	0000 0bc0      	orib #-64,%d0
      78:	0000 0bca      	orib #-54,%d0
      7c:	0000 0bd4      	orib #-44,%d0
      80:	0000 0c04      	orib #4,%d0
      84:	0000 0c0e      	orib #14,%d0
      88:	0000 0c72      	orib #114,%d0
      8c:	0000 0c7c      	orib #124,%d0
      90:	0000 0c86      	orib #-122,%d0
      94:	0000 0c90      	orib #-112,%d0
      98:	0000 0c9a      	orib #-102,%d0
      9c:	0000 0ca4      	orib #-92,%d0
      a0:	0000 0cae      	orib #-82,%d0
      a4:	0000 0cb8      	orib #-72,%d0
      a8:	0000 0cc2      	orib #-62,%d0
      ac:	0000 0ccc      	orib #-52,%d0
      b0:	0000 0cd6      	orib #-42,%d0
      b4:	0000 0ce0      	orib #-32,%d0
      b8:	0000 0cea      	orib #-22,%d0
      bc:	0000 0cf4      	orib #-12,%d0

000000c0 <_start>:
      c0:	007c 0700      	oriw #1792,%sr
      c4:	41f9 0000 15c8 	lea 15c8 <__data_load_start>,%a0
      ca:	43f9 0008 0000 	lea 80000 <vbug_buffer>,%a1
      d0:	45f9 0008 0000 	lea 80000 <vbug_buffer>,%a2

000000d6 <copy_data>:
      d6:	b3ca           	cmpal %a2,%a1
      d8:	6c04           	bges de <go_ahead>
      da:	12d8           	moveb %a0@+,%a1@+
      dc:	60f8           	bras d6 <copy_data>

000000de <go_ahead>:
      de:	4eb9 0000 00e8 	jsr e8 <main>

000000e4 <.dead>:
      e4:	60fe           	bras e4 <.dead>
	...

000000e8 <main>:
      e8:	4e56 fff4      	linkw %fp,#-12
      ec:	42b9 0008 0100 	clrl 80100 <systemTick>
      f2:	4eb9 0000 0268 	jsr 268 <uart_init>
      f8:	4eb9 0000 0328 	jsr 328 <init_kbd>
      fe:	4879 0000 0a3c 	pea a3c <picovga_putchar>
     104:	4eb9 0000 0d00 	jsr d00 <set_console_output>
     10a:	588f           	addql #4,%sp
     10c:	4879 0000 08f8 	pea 8f8 <get_char>
     112:	4eb9 0000 0d18 	jsr d18 <set_console_input>
     118:	588f           	addql #4,%sp
     11a:	4878 0007      	pea 7 <_vectors+0x7>
     11e:	4878 0006      	pea 6 <_vectors+0x6>
     122:	4eb9 0000 0ab2 	jsr ab2 <picovga_set_color>
     128:	508f           	addql #8,%sp
     12a:	4879 0000 11f7 	pea 11f7 <MsgOrionInit+0xb1>
     130:	4eb9 0000 0eb4 	jsr eb4 <printf>
     136:	588f           	addql #4,%sp
     138:	42a7           	clrl %sp@-
     13a:	4878 0003      	pea 3 <_vectors+0x3>
     13e:	4eb9 0000 0ab2 	jsr ab2 <picovga_set_color>
     144:	508f           	addql #8,%sp
     146:	4879 0000 1146 	pea 1146 <MsgOrionInit>
     14c:	4879 0000 1213 	pea 1213 <MsgOrionInit+0xcd>
     152:	4eb9 0000 0eb4 	jsr eb4 <printf>
     158:	508f           	addql #8,%sp
     15a:	42ae fffc      	clrl %fp@(-4)
     15e:	700f           	moveq #15,%d0
     160:	2d40 fff8      	movel %d0,%fp@(-8)
     164:	2f2e fff8      	movel %fp@(-8),%sp@-
     168:	2f2e fffc      	movel %fp@(-4),%sp@-
     16c:	4eb9 0000 0a5a 	jsr a5a <picovga_gotoxy>
     172:	508f           	addql #8,%sp
     174:	4879 0000 1216 	pea 1216 <MsgOrionInit+0xd0>
     17a:	4eb9 0000 0eb4 	jsr eb4 <printf>
     180:	588f           	addql #4,%sp
     182:	4879 0000 1226 	pea 1226 <MsgOrionInit+0xe0>
     188:	4eb9 0000 0eb4 	jsr eb4 <printf>
     18e:	588f           	addql #4,%sp
     190:	4879 0000 1239 	pea 1239 <MsgOrionInit+0xf3>
     196:	4eb9 0000 0eb4 	jsr eb4 <printf>
     19c:	588f           	addql #4,%sp
     19e:	4879 0000 1256 	pea 1256 <MsgOrionInit+0x110>
     1a4:	4eb9 0000 0eb4 	jsr eb4 <printf>
     1aa:	588f           	addql #4,%sp
     1ac:	4879 0000 1268 	pea 1268 <MsgOrionInit+0x122>
     1b2:	4eb9 0000 0eb4 	jsr eb4 <printf>
     1b8:	588f           	addql #4,%sp
     1ba:	4879 0000 127e 	pea 127e <MsgOrionInit+0x138>
     1c0:	4eb9 0000 0eb4 	jsr eb4 <printf>
     1c6:	588f           	addql #4,%sp
     1c8:	4eb9 0000 08f8 	jsr 8f8 <get_char>
     1ce:	2d40 fff4      	movel %d0,%fp@(-12)
     1d2:	2f2e fff4      	movel %fp@(-12),%sp@-
     1d6:	4879 0000 1291 	pea 1291 <MsgOrionInit+0x14b>
     1dc:	4eb9 0000 0eb4 	jsr eb4 <printf>
     1e2:	508f           	addql #8,%sp
     1e4:	70d0           	moveq #-48,%d0
     1e6:	d1ae fff4      	addl %d0,%fp@(-12)
     1ea:	4879 0000 1294 	pea 1294 <MsgOrionInit+0x14e>
     1f0:	4eb9 0000 0eb4 	jsr eb4 <printf>
     1f6:	588f           	addql #4,%sp
     1f8:	7003           	moveq #3,%d0
     1fa:	b0ae fff4      	cmpl %fp@(-12),%d0
     1fe:	675a           	beqs 25a <main+0x172>
     200:	7003           	moveq #3,%d0
     202:	b0ae fff4      	cmpl %fp@(-12),%d0
     206:	6540           	bcss 248 <main+0x160>
     208:	7002           	moveq #2,%d0
     20a:	b0ae fff4      	cmpl %fp@(-12),%d0
     20e:	6750           	beqs 260 <main+0x178>
     210:	7002           	moveq #2,%d0
     212:	b0ae fff4      	cmpl %fp@(-12),%d0
     216:	6530           	bcss 248 <main+0x160>
     218:	4aae fff4      	tstl %fp@(-12)
     21c:	670a           	beqs 228 <main+0x140>
     21e:	7001           	moveq #1,%d0
     220:	b0ae fff4      	cmpl %fp@(-12),%d0
     224:	670a           	beqs 230 <main+0x148>
     226:	6020           	bras 248 <main+0x160>
     228:	4eb9 0000 0b0a 	jsr b0a <clrscr>
     22e:	6032           	bras 262 <main+0x17a>
     230:	2039 0008 0100 	movel 80100 <systemTick>,%d0
     236:	2f00           	movel %d0,%sp@-
     238:	4879 0000 1296 	pea 1296 <MsgOrionInit+0x150>
     23e:	4eb9 0000 0eb4 	jsr eb4 <printf>
     244:	508f           	addql #8,%sp
     246:	601a           	bras 262 <main+0x17a>
     248:	4879 0000 12ab 	pea 12ab <MsgOrionInit+0x165>
     24e:	4eb9 0000 0eb4 	jsr eb4 <printf>
     254:	588f           	addql #4,%sp
     256:	6000 ff0c      	braw 164 <main+0x7c>
     25a:	4e71           	nop
     25c:	6000 ff06      	braw 164 <main+0x7c>
     260:	4e71           	nop
     262:	6000 ff00      	braw 164 <main+0x7c>
	...

00000268 <uart_init>:
     268:	4e56 0000      	linkw %fp,#0
     26c:	207c 00ff 4003 	moveal #16728067,%a0
     272:	10bc 0000      	moveb #0,%a0@
     276:	207c 00ff 4007 	moveal #16728071,%a0
     27c:	10bc ff80      	moveb #-128,%a0@
     280:	207c 00ff 4001 	moveal #16728065,%a0
     286:	10bc 0001      	moveb #1,%a0@
     28a:	207c 00ff 4003 	moveal #16728067,%a0
     290:	10bc 0000      	moveb #0,%a0@
     294:	207c 00ff 4007 	moveal #16728071,%a0
     29a:	10bc 0003      	moveb #3,%a0@
     29e:	207c 00ff 4005 	moveal #16728069,%a0
     2a4:	10bc 0007      	moveb #7,%a0@
     2a8:	207c 00ff 4009 	moveal #16728073,%a0
     2ae:	10bc 0003      	moveb #3,%a0@
     2b2:	4e71           	nop
     2b4:	4e5e           	unlk %fp
     2b6:	4e75           	rts

000002b8 <uart_putchar>:
     2b8:	4e56 fffc      	linkw %fp,#-4
     2bc:	202e 0008      	movel %fp@(8),%d0
     2c0:	1000           	moveb %d0,%d0
     2c2:	1d40 fffe      	moveb %d0,%fp@(-2)
     2c6:	4e71           	nop
     2c8:	207c 00ff 400b 	moveal #16728075,%a0
     2ce:	1010           	moveb %a0@,%d0
     2d0:	1000           	moveb %d0,%d0
     2d2:	0280 0000 00ff 	andil #255,%d0
     2d8:	7220           	moveq #32,%d1
     2da:	c081           	andl %d1,%d0
     2dc:	67ea           	beqs 2c8 <uart_putchar+0x10>
     2de:	207c 00ff 4001 	moveal #16728065,%a0
     2e4:	10ae fffe      	moveb %fp@(-2),%a0@
     2e8:	4e71           	nop
     2ea:	4e5e           	unlk %fp
     2ec:	4e75           	rts
	...

000002f0 <mymemset>:
     2f0:	4e56 fffc      	linkw %fp,#-4
     2f4:	2d6e 0008 fffc 	movel %fp@(8),%fp@(-4)
     2fa:	6014           	bras 310 <mymemset+0x20>
     2fc:	202e fffc      	movel %fp@(-4),%d0
     300:	2200           	movel %d0,%d1
     302:	5281           	addql #1,%d1
     304:	2d41 fffc      	movel %d1,%fp@(-4)
     308:	222e 000c      	movel %fp@(12),%d1
     30c:	2040           	moveal %d0,%a0
     30e:	1081           	moveb %d1,%a0@
     310:	202e 0010      	movel %fp@(16),%d0
     314:	2200           	movel %d0,%d1
     316:	5381           	subql #1,%d1
     318:	2d41 0010      	movel %d1,%fp@(16)
     31c:	4a80           	tstl %d0
     31e:	66dc           	bnes 2fc <mymemset+0xc>
     320:	202e 0008      	movel %fp@(8),%d0
     324:	4e5e           	unlk %fp
     326:	4e75           	rts

00000328 <init_kbd>:
     328:	4e56 fffc      	linkw %fp,#-4
     32c:	2d7c 00ff 4100 	movel #16728320,%fp@(-4)
     332:	fffc 
     334:	206e fffc      	moveal %fp@(-4),%a0
     338:	5e88           	addql #7,%a0
     33a:	10bc ff83      	moveb #-125,%a0@
     33e:	206e fffc      	moveal %fp@(-4),%a0
     342:	5288           	addql #1,%a0
     344:	10bc 0008      	moveb #8,%a0@
     348:	206e fffc      	moveal %fp@(-4),%a0
     34c:	5688           	addql #3,%a0
     34e:	10bc 0000      	moveb #0,%a0@
     352:	206e fffc      	moveal %fp@(-4),%a0
     356:	5e88           	addql #7,%a0
     358:	10bc 0003      	moveb #3,%a0@
     35c:	206e fffc      	moveal %fp@(-4),%a0
     360:	5a88           	addql #5,%a0
     362:	10bc ffc7      	moveb #-57,%a0@
     366:	206e fffc      	moveal %fp@(-4),%a0
     36a:	5088           	addql #8,%a0
     36c:	10bc 0000      	moveb #0,%a0@
     370:	206e fffc      	moveal %fp@(-4),%a0
     374:	5688           	addql #3,%a0
     376:	10bc 0000      	moveb #0,%a0@
     37a:	4e71           	nop
     37c:	4e5e           	unlk %fp
     37e:	4e75           	rts

00000380 <read_kbd>:
     380:	4e56 fff8      	linkw %fp,#-8
     384:	2d7c 00ff 4100 	movel #16728320,%fp@(-4)
     38a:	fffc 
     38c:	4e71           	nop
     38e:	307c 000b      	moveaw #11,%a0
     392:	d1ee fffc      	addal %fp@(-4),%a0
     396:	1010           	moveb %a0@,%d0
     398:	1000           	moveb %d0,%d0
     39a:	0280 0000 00ff 	andil #255,%d0
     3a0:	7201           	moveq #1,%d1
     3a2:	c081           	andl %d1,%d0
     3a4:	67e8           	beqs 38e <read_kbd+0xe>
     3a6:	206e fffc      	moveal %fp@(-4),%a0
     3aa:	5288           	addql #1,%a0
     3ac:	1d50 fffb      	moveb %a0@,%fp@(-5)
     3b0:	102e fffb      	moveb %fp@(-5),%d0
     3b4:	4e5e           	unlk %fp
     3b6:	4e75           	rts

000003b8 <get_0x81>:
     3b8:	4e56 fffc      	linkw %fp,#-4
     3bc:	4eba ffc2      	jsr %pc@(380 <read_kbd>)
     3c0:	13c0 0008 0136 	moveb %d0,80136 <type>
     3c6:	4eba ffb8      	jsr %pc@(380 <read_kbd>)
     3ca:	13c0 0008 0135 	moveb %d0,80135 <length>
     3d0:	1039 0008 0135 	moveb 80135 <length>,%d0
     3d6:	0c00 0008      	cmpib #8,%d0
     3da:	6322           	blss 3fe <get_0x81+0x46>
     3dc:	42ae fffc      	clrl %fp@(-4)
     3e0:	6008           	bras 3ea <get_0x81+0x32>
     3e2:	4eba ff9c      	jsr %pc@(380 <read_kbd>)
     3e6:	52ae fffc      	addql #1,%fp@(-4)
     3ea:	1039 0008 0135 	moveb 80135 <length>,%d0
     3f0:	1000           	moveb %d0,%d0
     3f2:	0280 0000 00ff 	andil #255,%d0
     3f8:	b0ae fffc      	cmpl %fp@(-4),%d0
     3fc:	6ee4           	bgts 3e2 <get_0x81+0x2a>
     3fe:	1039 0008 0135 	moveb 80135 <length>,%d0
     404:	0c00 0008      	cmpib #8,%d0
     408:	6632           	bnes 43c <get_0x81+0x84>
     40a:	42ae fffc      	clrl %fp@(-4)
     40e:	6014           	bras 424 <get_0x81+0x6c>
     410:	4eba ff6e      	jsr %pc@(380 <read_kbd>)
     414:	206e fffc      	moveal %fp@(-4),%a0
     418:	d1fc 0008 0104 	addal #524548,%a0
     41e:	1080           	moveb %d0,%a0@
     420:	52ae fffc      	addql #1,%fp@(-4)
     424:	7007           	moveq #7,%d0
     426:	b0ae fffc      	cmpl %fp@(-4),%d0
     42a:	6ce4           	bges 410 <get_0x81+0x58>
     42c:	1039 0008 0106 	moveb 80106 <key_buffer+0x2>,%d0
     432:	672a           	beqs 45e <get_0x81+0xa6>
     434:	1039 0008 0106 	moveb 80106 <key_buffer+0x2>,%d0
     43a:	6024           	bras 460 <get_0x81+0xa8>
     43c:	42ae fffc      	clrl %fp@(-4)
     440:	6008           	bras 44a <get_0x81+0x92>
     442:	4eba ff3c      	jsr %pc@(380 <read_kbd>)
     446:	52ae fffc      	addql #1,%fp@(-4)
     44a:	1039 0008 0135 	moveb 80135 <length>,%d0
     450:	1000           	moveb %d0,%d0
     452:	0280 0000 00ff 	andil #255,%d0
     458:	b0ae fffc      	cmpl %fp@(-4),%d0
     45c:	6ee4           	bgts 442 <get_0x81+0x8a>
     45e:	4200           	clrb %d0
     460:	4e5e           	unlk %fp
     462:	4e75           	rts

00000464 <get_0x87>:
     464:	4e56 fffc      	linkw %fp,#-4
     468:	4eba ff16      	jsr %pc@(380 <read_kbd>)
     46c:	13c0 0008 0135 	moveb %d0,80135 <length>
     472:	42ae fffc      	clrl %fp@(-4)
     476:	6008           	bras 480 <get_0x87+0x1c>
     478:	4eba ff06      	jsr %pc@(380 <read_kbd>)
     47c:	52ae fffc      	addql #1,%fp@(-4)
     480:	1039 0008 0135 	moveb 80135 <length>,%d0
     486:	1000           	moveb %d0,%d0
     488:	0280 0000 00ff 	andil #255,%d0
     48e:	b0ae fffc      	cmpl %fp@(-4),%d0
     492:	6ee4           	bgts 478 <get_0x87+0x14>
     494:	4e71           	nop
     496:	4e71           	nop
     498:	4e5e           	unlk %fp
     49a:	4e75           	rts

0000049c <get_0x88>:
     49c:	4e56 fff8      	linkw %fp,#-8
     4a0:	4eba fede      	jsr %pc@(380 <read_kbd>)
     4a4:	1d40 fffb      	moveb %d0,%fp@(-5)
     4a8:	7001           	moveq #1,%d0
     4aa:	2d40 fffc      	movel %d0,%fp@(-4)
     4ae:	6014           	bras 4c4 <get_0x88+0x28>
     4b0:	4eba fece      	jsr %pc@(380 <read_kbd>)
     4b4:	206e fffc      	moveal %fp@(-4),%a0
     4b8:	d1fc 0008 0104 	addal #524548,%a0
     4be:	1080           	moveb %d0,%a0@
     4c0:	52ae fffc      	addql #1,%fp@(-4)
     4c4:	7000           	moveq #0,%d0
     4c6:	102e fffb      	moveb %fp@(-5),%d0
     4ca:	b0ae fffc      	cmpl %fp@(-4),%d0
     4ce:	6ee0           	bgts 4b0 <get_0x88+0x14>
     4d0:	1039 0008 0106 	moveb 80106 <key_buffer+0x2>,%d0
     4d6:	6674           	bnes 54c <get_0x88+0xb0>
     4d8:	1039 0008 0108 	moveb 80108 <key_buffer+0x4>,%d0
     4de:	666c           	bnes 54c <get_0x88+0xb0>
     4e0:	1039 0008 013b 	moveb 8013b <special_key_status>,%d0
     4e6:	6720           	beqs 508 <get_0x88+0x6c>
     4e8:	1039 0008 0138 	moveb 80138 <special_key_down>,%d0
     4ee:	6718           	beqs 508 <get_0x88+0x6c>
     4f0:	13fc 0001 0008 	moveb #1,80137 <special_key_up>
     4f6:	0137 
     4f8:	13fc 0000 0008 	moveb #0,80138 <special_key_down>
     4fe:	0138 
     500:	13fc 0000 0008 	moveb #0,8013b <special_key_status>
     506:	013b 
     508:	1039 0008 013a 	moveb 8013a <key_down>,%d0
     50e:	6716           	beqs 526 <get_0x88+0x8a>
     510:	13fc 0001 0008 	moveb #1,80139 <key_up>
     516:	0139 
     518:	13fc 0000 0008 	moveb #0,8013a <key_down>
     51e:	013a 
     520:	4200           	clrb %d0
     522:	6000 0156      	braw 67a <get_0x88+0x1de>
     526:	1039 0008 0138 	moveb 80138 <special_key_down>,%d0
     52c:	6718           	beqs 546 <get_0x88+0xaa>
     52e:	13fc 0001 0008 	moveb #1,80137 <special_key_up>
     534:	0137 
     536:	13fc 0000 0008 	moveb #0,80138 <special_key_down>
     53c:	0138 
     53e:	13fc 0000 0008 	moveb #0,8013b <special_key_status>
     544:	013b 
     546:	4200           	clrb %d0
     548:	6000 0130      	braw 67a <get_0x88+0x1de>
     54c:	1039 0008 0106 	moveb 80106 <key_buffer+0x2>,%d0
     552:	0c00 0001      	cmpib #1,%d0
     556:	670c           	beqs 564 <get_0x88+0xc8>
     558:	1039 0008 0106 	moveb 80106 <key_buffer+0x2>,%d0
     55e:	0c00 0010      	cmpib #16,%d0
     562:	661e           	bnes 582 <get_0x88+0xe6>
     564:	13fc 0001 0008 	moveb #1,8013b <special_key_status>
     56a:	013b 
     56c:	13fc 0001 0008 	moveb #1,80138 <special_key_down>
     572:	0138 
     574:	13fc 0000 0008 	moveb #0,80137 <special_key_up>
     57a:	0137 
     57c:	7001           	moveq #1,%d0
     57e:	6000 00fa      	braw 67a <get_0x88+0x1de>
     582:	1039 0008 0106 	moveb 80106 <key_buffer+0x2>,%d0
     588:	0c00 0002      	cmpib #2,%d0
     58c:	670c           	beqs 59a <get_0x88+0xfe>
     58e:	1039 0008 0106 	moveb 80106 <key_buffer+0x2>,%d0
     594:	0c00 0020      	cmpib #32,%d0
     598:	661e           	bnes 5b8 <get_0x88+0x11c>
     59a:	13fc 0001 0008 	moveb #1,80138 <special_key_down>
     5a0:	0138 
     5a2:	13fc 0000 0008 	moveb #0,80137 <special_key_up>
     5a8:	0137 
     5aa:	13fc 0002 0008 	moveb #2,8013b <special_key_status>
     5b0:	013b 
     5b2:	7002           	moveq #2,%d0
     5b4:	6000 00c4      	braw 67a <get_0x88+0x1de>
     5b8:	1039 0008 0106 	moveb 80106 <key_buffer+0x2>,%d0
     5be:	0c00 0004      	cmpib #4,%d0
     5c2:	661e           	bnes 5e2 <get_0x88+0x146>
     5c4:	13fc 0001 0008 	moveb #1,80138 <special_key_down>
     5ca:	0138 
     5cc:	13fc 0000 0008 	moveb #0,80137 <special_key_up>
     5d2:	0137 
     5d4:	13fc 0004 0008 	moveb #4,8013b <special_key_status>
     5da:	013b 
     5dc:	7004           	moveq #4,%d0
     5de:	6000 009a      	braw 67a <get_0x88+0x1de>
     5e2:	1039 0008 0106 	moveb 80106 <key_buffer+0x2>,%d0
     5e8:	0c00 0040      	cmpib #64,%d0
     5ec:	661c           	bnes 60a <get_0x88+0x16e>
     5ee:	13fc 0001 0008 	moveb #1,80138 <special_key_down>
     5f4:	0138 
     5f6:	13fc 0000 0008 	moveb #0,80137 <special_key_up>
     5fc:	0137 
     5fe:	13fc 0008 0008 	moveb #8,8013b <special_key_status>
     604:	013b 
     606:	7008           	moveq #8,%d0
     608:	6070           	bras 67a <get_0x88+0x1de>
     60a:	1039 0008 0108 	moveb 80108 <key_buffer+0x4>,%d0
     610:	0c00 0039      	cmpib #57,%d0
     614:	6652           	bnes 668 <get_0x88+0x1cc>
     616:	1039 0008 013d 	moveb 8013d <mod_caps>,%d0
     61c:	6620           	bnes 63e <get_0x88+0x1a2>
     61e:	13fc 0002 0008 	moveb #2,8013d <mod_caps>
     624:	013d 
     626:	1039 0008 013d 	moveb 8013d <mod_caps>,%d0
     62c:	1000           	moveb %d0,%d0
     62e:	0280 0000 00ff 	andil #255,%d0
     634:	2f00           	movel %d0,%sp@-
     636:	4eba 00d2      	jsr %pc@(70a <set_keyboard_leds>)
     63a:	588f           	addql #4,%sp
     63c:	601e           	bras 65c <get_0x88+0x1c0>
     63e:	13fc 0000 0008 	moveb #0,8013d <mod_caps>
     644:	013d 
     646:	1039 0008 013d 	moveb 8013d <mod_caps>,%d0
     64c:	1000           	moveb %d0,%d0
     64e:	0280 0000 00ff 	andil #255,%d0
     654:	2f00           	movel %d0,%sp@-
     656:	4eba 00b2      	jsr %pc@(70a <set_keyboard_leds>)
     65a:	588f           	addql #4,%sp
     65c:	13fc 0001 0008 	moveb #1,80138 <special_key_down>
     662:	0138 
     664:	7010           	moveq #16,%d0
     666:	6012           	bras 67a <get_0x88+0x1de>
     668:	13fc 0001 0008 	moveb #1,8013a <key_down>
     66e:	013a 
     670:	13fc 0000 0008 	moveb #0,80139 <key_up>
     676:	0139 
     678:	70fe           	moveq #-2,%d0
     67a:	4e5e           	unlk %fp
     67c:	4e75           	rts

0000067e <get_packet>:
     67e:	4e56 0000      	linkw %fp,#0
     682:	4eba fcfc      	jsr %pc@(380 <read_kbd>)
     686:	0c00 0057      	cmpib #87,%d0
     68a:	6670           	bnes 6fc <get_packet+0x7e>
     68c:	4eba fcf2      	jsr %pc@(380 <read_kbd>)
     690:	0c00 ffab      	cmpib #-85,%d0
     694:	666a           	bnes 700 <get_packet+0x82>
     696:	4eba fce8      	jsr %pc@(380 <read_kbd>)
     69a:	13c0 0008 0134 	moveb %d0,80134 <cmd>
     6a0:	1039 0008 0134 	moveb 80134 <cmd>,%d0
     6a6:	0c00 ff88      	cmpib #-120,%d0
     6aa:	6606           	bnes 6b2 <get_packet+0x34>
     6ac:	4eba fdee      	jsr %pc@(49c <get_0x88>)
     6b0:	6054           	bras 706 <get_packet+0x88>
     6b2:	1039 0008 0134 	moveb 80134 <cmd>,%d0
     6b8:	0c00 ff81      	cmpib #-127,%d0
     6bc:	6606           	bnes 6c4 <get_packet+0x46>
     6be:	4eba fcf8      	jsr %pc@(3b8 <get_0x81>)
     6c2:	603e           	bras 702 <get_packet+0x84>
     6c4:	1039 0008 0134 	moveb 80134 <cmd>,%d0
     6ca:	0c00 ff82      	cmpib #-126,%d0
     6ce:	660c           	bnes 6dc <get_packet+0x5e>
     6d0:	4eba fcae      	jsr %pc@(380 <read_kbd>)
     6d4:	13c0 0008 0134 	moveb %d0,80134 <cmd>
     6da:	6026           	bras 702 <get_packet+0x84>
     6dc:	1039 0008 0134 	moveb 80134 <cmd>,%d0
     6e2:	0c00 ff87      	cmpib #-121,%d0
     6e6:	669a           	bnes 682 <get_packet+0x4>
     6e8:	4eba fd7a      	jsr %pc@(464 <get_0x87>)
     6ec:	4879 0000 1539 	pea 1539 <OE_ALTGR_KEYMAP+0x80>
     6f2:	4eb9 0000 0eb4 	jsr eb4 <printf>
     6f8:	588f           	addql #4,%sp
     6fa:	6006           	bras 702 <get_packet+0x84>
     6fc:	4e71           	nop
     6fe:	6082           	bras 682 <get_packet+0x4>
     700:	4e71           	nop
     702:	6000 ff7e      	braw 682 <get_packet+0x4>
     706:	4e5e           	unlk %fp
     708:	4e75           	rts

0000070a <set_keyboard_leds>:
     70a:	4e56 ffe4      	linkw %fp,#-28
     70e:	202e 0008      	movel %fp@(8),%d0
     712:	1000           	moveb %d0,%d0
     714:	1d40 ffe4      	moveb %d0,%fp@(-28)
     718:	2d7c 00ff 4100 	movel #16728320,%fp@(-14)
     71e:	fff2 
     720:	1d7c 0057 ffe7 	moveb #87,%fp@(-25)
     726:	1d7c ffab ffe8 	moveb #-85,%fp@(-24)
     72c:	1d7c 0012 ffe9 	moveb #18,%fp@(-23)
     732:	422e ffea      	clrb %fp@(-22)
     736:	422e ffeb      	clrb %fp@(-21)
     73a:	422e ffec      	clrb %fp@(-20)
     73e:	422e ffed      	clrb %fp@(-19)
     742:	1d6e ffe4 ffee 	moveb %fp@(-28),%fp@(-18)
     748:	422e ffef      	clrb %fp@(-17)
     74c:	1d7c 000f fff0 	moveb #15,%fp@(-16)
     752:	422e ffff      	clrb %fp@(-1)
     756:	7002           	moveq #2,%d0
     758:	2d40 fffa      	movel %d0,%fp@(-6)
     75c:	6012           	bras 770 <set_keyboard_leds+0x66>
     75e:	41ee ffe7      	lea %fp@(-25),%a0
     762:	d1ee fffa      	addal %fp@(-6),%a0
     766:	1010           	moveb %a0@,%d0
     768:	d12e ffff      	addb %d0,%fp@(-1)
     76c:	52ae fffa      	addql #1,%fp@(-6)
     770:	7209           	moveq #9,%d1
     772:	b2ae fffa      	cmpl %fp@(-6),%d1
     776:	6ce6           	bges 75e <set_keyboard_leds+0x54>
     778:	1d6e ffff fff1 	moveb %fp@(-1),%fp@(-15)
     77e:	42ae fff6      	clrl %fp@(-10)
     782:	6030           	bras 7b4 <set_keyboard_leds+0xaa>
     784:	4e71           	nop
     786:	307c 000b      	moveaw #11,%a0
     78a:	d1ee fff2      	addal %fp@(-14),%a0
     78e:	1010           	moveb %a0@,%d0
     790:	1000           	moveb %d0,%d0
     792:	0280 0000 00ff 	andil #255,%d0
     798:	7220           	moveq #32,%d1
     79a:	c081           	andl %d1,%d0
     79c:	67e8           	beqs 786 <set_keyboard_leds+0x7c>
     79e:	206e fff2      	moveal %fp@(-14),%a0
     7a2:	5288           	addql #1,%a0
     7a4:	43ee ffe7      	lea %fp@(-25),%a1
     7a8:	d3ee fff6      	addal %fp@(-10),%a1
     7ac:	1011           	moveb %a1@,%d0
     7ae:	1080           	moveb %d0,%a0@
     7b0:	52ae fff6      	addql #1,%fp@(-10)
     7b4:	700a           	moveq #10,%d0
     7b6:	b0ae fff6      	cmpl %fp@(-10),%d0
     7ba:	6cc8           	bges 784 <set_keyboard_leds+0x7a>
     7bc:	4e71           	nop
     7be:	4e71           	nop
     7c0:	4e5e           	unlk %fp
     7c2:	4e75           	rts

000007c4 <get_kbd_key>:
     7c4:	4e56 fffc      	linkw %fp,#-4
     7c8:	202e 0008      	movel %fp@(8),%d0
     7cc:	1000           	moveb %d0,%d0
     7ce:	1d40 fffc      	moveb %d0,%fp@(-4)
     7d2:	422e ffff      	clrb %fp@(-1)
     7d6:	1039 0008 013b 	moveb 8013b <special_key_status>,%d0
     7dc:	661e           	bnes 7fc <get_kbd_key+0x38>
     7de:	1039 0008 013d 	moveb 8013d <mod_caps>,%d0
     7e4:	6616           	bnes 7fc <get_kbd_key+0x38>
     7e6:	7000           	moveq #0,%d0
     7e8:	102e fffc      	moveb %fp@(-4),%d0
     7ec:	41f9 0000 12b9 	lea 12b9 <OE_BASE_KEYMAP>,%a0
     7f2:	1d70 0800 ffff 	moveb %a0@(0,%d0:l),%fp@(-1)
     7f8:	6000 00f6      	braw 8f0 <get_kbd_key+0x12c>
     7fc:	1039 0008 013b 	moveb 8013b <special_key_status>,%d0
     802:	1000           	moveb %d0,%d0
     804:	0280 0000 00ff 	andil #255,%d0
     80a:	7202           	moveq #2,%d1
     80c:	c081           	andl %d1,%d0
     80e:	673a           	beqs 84a <get_kbd_key+0x86>
     810:	1039 0008 013d 	moveb 8013d <mod_caps>,%d0
     816:	0c00 0002      	cmpib #2,%d0
     81a:	6614           	bnes 830 <get_kbd_key+0x6c>
     81c:	7000           	moveq #0,%d0
     81e:	102e fffc      	moveb %fp@(-4),%d0
     822:	41f9 0000 12b9 	lea 12b9 <OE_BASE_KEYMAP>,%a0
     828:	1d70 0800 ffff 	moveb %a0@(0,%d0:l),%fp@(-1)
     82e:	6012           	bras 842 <get_kbd_key+0x7e>
     830:	7000           	moveq #0,%d0
     832:	102e fffc      	moveb %fp@(-4),%d0
     836:	41f9 0000 1339 	lea 1339 <OE_SHIFT_KEYMAP>,%a0
     83c:	1d70 0800 ffff 	moveb %a0@(0,%d0:l),%fp@(-1)
     842:	102e ffff      	moveb %fp@(-1),%d0
     846:	6000 00ac      	braw 8f4 <get_kbd_key+0x130>
     84a:	1039 0008 013b 	moveb 8013b <special_key_status>,%d0
     850:	1000           	moveb %d0,%d0
     852:	0280 0000 00ff 	andil #255,%d0
     858:	7201           	moveq #1,%d1
     85a:	c081           	andl %d1,%d0
     85c:	6714           	beqs 872 <get_kbd_key+0xae>
     85e:	7000           	moveq #0,%d0
     860:	102e fffc      	moveb %fp@(-4),%d0
     864:	41f9 0000 13b9 	lea 13b9 <OE_CTRL_KEYMAP>,%a0
     86a:	1d70 0800 ffff 	moveb %a0@(0,%d0:l),%fp@(-1)
     870:	607e           	bras 8f0 <get_kbd_key+0x12c>
     872:	1039 0008 013b 	moveb 8013b <special_key_status>,%d0
     878:	1000           	moveb %d0,%d0
     87a:	0280 0000 00ff 	andil #255,%d0
     880:	7204           	moveq #4,%d1
     882:	c081           	andl %d1,%d0
     884:	6714           	beqs 89a <get_kbd_key+0xd6>
     886:	7000           	moveq #0,%d0
     888:	102e fffc      	moveb %fp@(-4),%d0
     88c:	41f9 0000 1439 	lea 1439 <OE_ALT_KEYMAP>,%a0
     892:	1d70 0800 ffff 	moveb %a0@(0,%d0:l),%fp@(-1)
     898:	6056           	bras 8f0 <get_kbd_key+0x12c>
     89a:	1039 0008 013b 	moveb 8013b <special_key_status>,%d0
     8a0:	1000           	moveb %d0,%d0
     8a2:	0280 0000 00ff 	andil #255,%d0
     8a8:	7208           	moveq #8,%d1
     8aa:	c081           	andl %d1,%d0
     8ac:	6714           	beqs 8c2 <get_kbd_key+0xfe>
     8ae:	7000           	moveq #0,%d0
     8b0:	102e fffc      	moveb %fp@(-4),%d0
     8b4:	41f9 0000 14b9 	lea 14b9 <OE_ALTGR_KEYMAP>,%a0
     8ba:	1d70 0800 ffff 	moveb %a0@(0,%d0:l),%fp@(-1)
     8c0:	602e           	bras 8f0 <get_kbd_key+0x12c>
     8c2:	1039 0008 013d 	moveb 8013d <mod_caps>,%d0
     8c8:	6714           	beqs 8de <get_kbd_key+0x11a>
     8ca:	7000           	moveq #0,%d0
     8cc:	102e fffc      	moveb %fp@(-4),%d0
     8d0:	41f9 0000 1339 	lea 1339 <OE_SHIFT_KEYMAP>,%a0
     8d6:	1d70 0800 ffff 	moveb %a0@(0,%d0:l),%fp@(-1)
     8dc:	6012           	bras 8f0 <get_kbd_key+0x12c>
     8de:	4879 0000 153b 	pea 153b <OE_ALTGR_KEYMAP+0x82>
     8e4:	4eb9 0000 0eb4 	jsr eb4 <printf>
     8ea:	588f           	addql #4,%sp
     8ec:	4200           	clrb %d0
     8ee:	6004           	bras 8f4 <get_kbd_key+0x130>
     8f0:	102e ffff      	moveb %fp@(-1),%d0
     8f4:	4e5e           	unlk %fp
     8f6:	4e75           	rts

000008f8 <get_char>:
     8f8:	4e56 fff4      	linkw %fp,#-12
     8fc:	422e fffb      	clrb %fp@(-5)
     900:	42ae fffc      	clrl %fp@(-4)
     904:	422e fffb      	clrb %fp@(-5)
     908:	4878 0030      	pea 30 <_vectors+0x30>
     90c:	42a7           	clrl %sp@-
     90e:	4879 0008 0104 	pea 80104 <key_buffer>
     914:	4eba f9da      	jsr %pc@(2f0 <mymemset>)
     918:	4fef 000c      	lea %sp@(12),%sp
     91c:	4eba fd60      	jsr %pc@(67e <get_packet>)
     920:	1d40 fffb      	moveb %d0,%fp@(-5)
     924:	1039 0008 013d 	moveb 8013d <mod_caps>,%d0
     92a:	1000           	moveb %d0,%d0
     92c:	0280 0000 00ff 	andil #255,%d0
     932:	2f00           	movel %d0,%sp@-
     934:	4eba fdd4      	jsr %pc@(70a <set_keyboard_leds>)
     938:	588f           	addql #4,%sp
     93a:	4a2e fffb      	tstb %fp@(-5)
     93e:	6776           	beqs 9b6 <get_char+0xbe>
     940:	1039 0008 0108 	moveb 80108 <key_buffer+0x4>,%d0
     946:	0c00 0039      	cmpib #57,%d0
     94a:	6604           	bnes 950 <get_char+0x58>
     94c:	7000           	moveq #0,%d0
     94e:	606c           	bras 9bc <get_char+0xc4>
     950:	0c2e 0002 fffb 	cmpib #2,%fp@(-5)
     956:	6714           	beqs 96c <get_char+0x74>
     958:	0c2e fffd fffb 	cmpib #-3,%fp@(-5)
     95e:	620c           	bhis 96c <get_char+0x74>
     960:	7000           	moveq #0,%d0
     962:	102e fffb      	moveb %fp@(-5),%d0
     966:	e188           	lsll #8,%d0
     968:	2d40 fffc      	movel %d0,%fp@(-4)
     96c:	422e fffb      	clrb %fp@(-5)
     970:	1039 0008 0108 	moveb 80108 <key_buffer+0x4>,%d0
     976:	6730           	beqs 9a8 <get_char+0xb0>
     978:	1039 0008 0108 	moveb 80108 <key_buffer+0x4>,%d0
     97e:	1000           	moveb %d0,%d0
     980:	0280 0000 00ff 	andil #255,%d0
     986:	2f00           	movel %d0,%sp@-
     988:	4eba fe3a      	jsr %pc@(7c4 <get_kbd_key>)
     98c:	588f           	addql #4,%sp
     98e:	1000           	moveb %d0,%d0
     990:	0280 0000 00ff 	andil #255,%d0
     996:	2d40 fff6      	movel %d0,%fp@(-10)
     99a:	202e fff6      	movel %fp@(-10),%d0
     99e:	81ae fffc      	orl %d0,%fp@(-4)
     9a2:	202e fffc      	movel %fp@(-4),%d0
     9a6:	6014           	bras 9bc <get_char+0xc4>
     9a8:	02ae 0000 ff00 	andil #65280,%fp@(-4)
     9ae:	fffc 
     9b0:	202e fffc      	movel %fp@(-4),%d0
     9b4:	6006           	bras 9bc <get_char+0xc4>
     9b6:	4e71           	nop
     9b8:	6000 ff4e      	braw 908 <get_char+0x10>
     9bc:	4e5e           	unlk %fp
     9be:	4e75           	rts

000009c0 <main_kbd>:
     9c0:	4e56 fffc      	linkw %fp,#-4
     9c4:	4eba ff32      	jsr %pc@(8f8 <get_char>)
     9c8:	2d40 fffc      	movel %d0,%fp@(-4)
     9cc:	7020           	moveq #32,%d0
     9ce:	b0ae fffc      	cmpl %fp@(-4),%d0
     9d2:	6430           	bccs a04 <main_kbd+0x44>
     9d4:	0cae 0000 00ff 	cmpil #255,%fp@(-4)
     9da:	fffc 
     9dc:	6214           	bhis 9f2 <main_kbd+0x32>
     9de:	2f2e fffc      	movel %fp@(-4),%sp@-
     9e2:	4879 0000 153d 	pea 153d <OE_ALTGR_KEYMAP+0x84>
     9e8:	4eb9 0000 0eb4 	jsr eb4 <printf>
     9ee:	508f           	addql #8,%sp
     9f0:	6012           	bras a04 <main_kbd+0x44>
     9f2:	2f2e fffc      	movel %fp@(-4),%sp@-
     9f6:	4879 0000 1540 	pea 1540 <OE_ALTGR_KEYMAP+0x87>
     9fc:	4eb9 0000 0eb4 	jsr eb4 <printf>
     a02:	508f           	addql #8,%sp
     a04:	7042           	moveq #66,%d0
     a06:	b0ae fffc      	cmpl %fp@(-4),%d0
     a0a:	66b8           	bnes 9c4 <main_kbd+0x4>
     a0c:	7000           	moveq #0,%d0
     a0e:	4e5e           	unlk %fp
     a10:	4e75           	rts
	...

00000a14 <init_picoVga>:
     a14:	4e56 0000      	linkw %fp,#0
     a18:	4e71           	nop
     a1a:	4e5e           	unlk %fp
     a1c:	4e75           	rts

00000a1e <run_cmd>:
     a1e:	4e56 fffc      	linkw %fp,#-4
     a22:	202e 0008      	movel %fp@(8),%d0
     a26:	1000           	moveb %d0,%d0
     a28:	1d40 fffe      	moveb %d0,%fp@(-2)
     a2c:	207c 00ff 803d 	moveal #16744509,%a0
     a32:	10ae fffe      	moveb %fp@(-2),%a0@
     a36:	4e71           	nop
     a38:	4e5e           	unlk %fp
     a3a:	4e75           	rts

00000a3c <picovga_putchar>:
     a3c:	4e56 fffc      	linkw %fp,#-4
     a40:	202e 0008      	movel %fp@(8),%d0
     a44:	1000           	moveb %d0,%d0
     a46:	1d40 fffe      	moveb %d0,%fp@(-2)
     a4a:	207c 00ff 8001 	moveal #16744449,%a0
     a50:	10ae fffe      	moveb %fp@(-2),%a0@
     a54:	4e71           	nop
     a56:	4e5e           	unlk %fp
     a58:	4e75           	rts

00000a5a <picovga_gotoxy>:
     a5a:	4e56 0000      	linkw %fp,#0
     a5e:	207c 00ff 8033 	moveal #16744499,%a0
     a64:	202e 000c      	movel %fp@(12),%d0
     a68:	1080           	moveb %d0,%a0@
     a6a:	207c 00ff 8031 	moveal #16744497,%a0
     a70:	10bc 0000      	moveb #0,%a0@
     a74:	207c 00ff 802f 	moveal #16744495,%a0
     a7a:	202e 0008      	movel %fp@(8),%d0
     a7e:	1080           	moveb %d0,%a0@
     a80:	207c 00ff 802d 	moveal #16744493,%a0
     a86:	10bc 0000      	moveb #0,%a0@
     a8a:	2f2e 0008      	movel %fp@(8),%sp@-
     a8e:	2f2e 000c      	movel %fp@(12),%sp@-
     a92:	4879 0000 1545 	pea 1545 <OE_ALTGR_KEYMAP+0x8c>
     a98:	4eb9 0000 0eb4 	jsr eb4 <printf>
     a9e:	4fef 000c      	lea %sp@(12),%sp
     aa2:	207c 00ff 803d 	moveal #16744509,%a0
     aa8:	10bc ffa3      	moveb #-93,%a0@
     aac:	4e71           	nop
     aae:	4e5e           	unlk %fp
     ab0:	4e75           	rts

00000ab2 <picovga_set_color>:
     ab2:	4e56 fff8      	linkw %fp,#-8
     ab6:	222e 0008      	movel %fp@(8),%d1
     aba:	202e 000c      	movel %fp@(12),%d0
     abe:	1201           	moveb %d1,%d1
     ac0:	1d41 fffa      	moveb %d1,%fp@(-6)
     ac4:	1000           	moveb %d0,%d0
     ac6:	1d40 fff8      	moveb %d0,%fp@(-8)
     aca:	7000           	moveq #0,%d0
     acc:	102e fffa      	moveb %fp@(-6),%d0
     ad0:	e988           	lsll #4,%d0
     ad2:	7200           	moveq #0,%d1
     ad4:	4601           	notb %d1
     ad6:	c280           	andl %d0,%d1
     ad8:	2d41 fffc      	movel %d1,%fp@(-4)
     adc:	7000           	moveq #0,%d0
     ade:	102e fff8      	moveb %fp@(-8),%d0
     ae2:	81ae fffc      	orl %d0,%fp@(-4)
     ae6:	2f2e fffc      	movel %fp@(-4),%sp@-
     aea:	4879 0000 156a 	pea 156a <OE_ALTGR_KEYMAP+0xb1>
     af0:	4eb9 0000 0eb4 	jsr eb4 <printf>
     af6:	508f           	addql #8,%sp
     af8:	207c 00ff 8029 	moveal #16744489,%a0
     afe:	202e fffc      	movel %fp@(-4),%d0
     b02:	1080           	moveb %d0,%a0@
     b04:	4e71           	nop
     b06:	4e5e           	unlk %fp
     b08:	4e75           	rts

00000b0a <clrscr>:
     b0a:	4e56 0000      	linkw %fp,#0
     b0e:	207c 00ff 803d 	moveal #16744509,%a0
     b14:	10bc ffa4      	moveb #-92,%a0@
     b18:	4e71           	nop
     b1a:	4e5e           	unlk %fp
     b1c:	4e75           	rts
	...

00000b20 <SvcBusError>:
     b20:	4e56 0000      	linkw %fp,#0
     b24:	4e71           	nop
     b26:	4e5e           	unlk %fp
     b28:	4e73           	rte

00000b2a <Svcaddress_err>:
     b2a:	4e56 0000      	linkw %fp,#0
     b2e:	4e71           	nop
     b30:	4e5e           	unlk %fp
     b32:	4e73           	rte

00000b34 <SvcIllegalIns>:
     b34:	4e56 0000      	linkw %fp,#0
     b38:	4e71           	nop
     b3a:	4e5e           	unlk %fp
     b3c:	4e73           	rte

00000b3e <SvcDiv0_handler>:
     b3e:	4e56 0000      	linkw %fp,#0
     b42:	4e71           	nop
     b44:	4e5e           	unlk %fp
     b46:	4e73           	rte

00000b48 <SvcChkIns>:
     b48:	4e56 0000      	linkw %fp,#0
     b4c:	4e71           	nop
     b4e:	4e5e           	unlk %fp
     b50:	4e73           	rte

00000b52 <SvcTrapvIns>:
     b52:	4e56 0000      	linkw %fp,#0
     b56:	4e71           	nop
     b58:	4e5e           	unlk %fp
     b5a:	4e73           	rte

00000b5c <SvcPrivViolation>:
     b5c:	4e56 0000      	linkw %fp,#0
     b60:	4e71           	nop
     b62:	4e5e           	unlk %fp
     b64:	4e73           	rte

00000b66 <SvcTrace>:
     b66:	4e56 0000      	linkw %fp,#0
     b6a:	4e71           	nop
     b6c:	4e5e           	unlk %fp
     b6e:	4e73           	rte

00000b70 <SvcLineA>:
     b70:	4e56 0000      	linkw %fp,#0
     b74:	4e71           	nop
     b76:	4e5e           	unlk %fp
     b78:	4e73           	rte

00000b7a <SvcLineF>:
     b7a:	4e56 0000      	linkw %fp,#0
     b7e:	4e71           	nop
     b80:	4e5e           	unlk %fp
     b82:	4e73           	rte

00000b84 <bad_exception>:
     b84:	4e56 0000      	linkw %fp,#0
     b88:	4e71           	nop
     b8a:	4e5e           	unlk %fp
     b8c:	4e73           	rte

00000b8e <SpuriousHandler>:
     b8e:	4e56 0000      	linkw %fp,#0
     b92:	4e71           	nop
     b94:	4e5e           	unlk %fp
     b96:	4e73           	rte

00000b98 <Int1Handler>:
     b98:	4e56 0000      	linkw %fp,#0
     b9c:	4e71           	nop
     b9e:	4e5e           	unlk %fp
     ba0:	4e73           	rte

00000ba2 <Int2Handler>:
     ba2:	4e56 0000      	linkw %fp,#0
     ba6:	4e71           	nop
     ba8:	4e5e           	unlk %fp
     baa:	4e73           	rte

00000bac <Int3Handler>:
     bac:	4e56 0000      	linkw %fp,#0
     bb0:	4e71           	nop
     bb2:	4e5e           	unlk %fp
     bb4:	4e73           	rte

00000bb6 <Int4Handler>:
     bb6:	4e56 0000      	linkw %fp,#0
     bba:	4e71           	nop
     bbc:	4e5e           	unlk %fp
     bbe:	4e73           	rte

00000bc0 <Int5Handler>:
     bc0:	4e56 0000      	linkw %fp,#0
     bc4:	4e71           	nop
     bc6:	4e5e           	unlk %fp
     bc8:	4e73           	rte

00000bca <Int6Handler>:
     bca:	4e56 0000      	linkw %fp,#0
     bce:	4e71           	nop
     bd0:	4e5e           	unlk %fp
     bd2:	4e73           	rte

00000bd4 <Int7Handler>:
     bd4:	4e56 0000      	linkw %fp,#0
     bd8:	48e7 e000      	moveml %d0-%d2,%sp@-
     bdc:	2039 0008 0100 	movel 80100 <systemTick>,%d0
     be2:	2239 0008 0104 	movel 80104 <key_buffer>,%d1
     be8:	4282           	clrl %d2
     bea:	5281           	addql #1,%d1
     bec:	d182           	addxl %d2,%d0
     bee:	23c0 0008 0100 	movel %d0,80100 <systemTick>
     bf4:	23c1 0008 0104 	movel %d1,80104 <key_buffer>
     bfa:	4e71           	nop
     bfc:	4cdf 0007      	moveml %sp@+,%d0-%d2
     c00:	4e5e           	unlk %fp
     c02:	4e73           	rte

00000c04 <Trap0Handler>:
     c04:	4e56 0000      	linkw %fp,#0
     c08:	4e71           	nop
     c0a:	4e5e           	unlk %fp
     c0c:	4e73           	rte

00000c0e <Trap1Handler>:
     c0e:	4e56 0000      	linkw %fp,#0
     c12:	48e7 e0c0      	moveml %d0-%d2/%a0-%a1,%sp@-
     c16:	3000           	movew %d0,%d0
     c18:	0280 0000 ffff 	andil #65535,%d0
     c1e:	7401           	moveq #1,%d2
     c20:	b480           	cmpl %d0,%d2
     c22:	6708           	beqs c2c <Trap1Handler+0x1e>
     c24:	7402           	moveq #2,%d2
     c26:	b480           	cmpl %d0,%d2
     c28:	6716           	beqs c40 <Trap1Handler+0x32>
     c2a:	603a           	bras c66 <Trap1Handler+0x58>
     c2c:	2039 0008 0140 	movel 80140 <cconin>,%d0
     c32:	672c           	beqs c60 <Trap1Handler+0x52>
     c34:	2079 0008 0140 	moveal 80140 <cconin>,%a0
     c3a:	4e90           	jsr %a0@
     c3c:	2000           	movel %d0,%d0
     c3e:	6020           	bras c60 <Trap1Handler+0x52>
     c40:	2039 0008 0144 	movel 80144 <cconout>,%d0
     c46:	671c           	beqs c64 <Trap1Handler+0x56>
     c48:	2079 0008 0144 	moveal 80144 <cconout>,%a0
     c4e:	1001           	moveb %d1,%d0
     c50:	1000           	moveb %d0,%d0
     c52:	0280 0000 00ff 	andil #255,%d0
     c58:	2f00           	movel %d0,%sp@-
     c5a:	4e90           	jsr %a0@
     c5c:	588f           	addql #4,%sp
     c5e:	6004           	bras c64 <Trap1Handler+0x56>
     c60:	4e71           	nop
     c62:	6002           	bras c66 <Trap1Handler+0x58>
     c64:	4e71           	nop
     c66:	4e71           	nop
     c68:	4cee 0307 ffec 	moveml %fp@(-20),%d0-%d2/%a0-%a1
     c6e:	4e5e           	unlk %fp
     c70:	4e73           	rte

00000c72 <Trap2Handler>:
     c72:	4e56 0000      	linkw %fp,#0
     c76:	4e71           	nop
     c78:	4e5e           	unlk %fp
     c7a:	4e73           	rte

00000c7c <Trap3Handler>:
     c7c:	4e56 0000      	linkw %fp,#0
     c80:	4e71           	nop
     c82:	4e5e           	unlk %fp
     c84:	4e73           	rte

00000c86 <Trap4Handler>:
     c86:	4e56 0000      	linkw %fp,#0
     c8a:	4e71           	nop
     c8c:	4e5e           	unlk %fp
     c8e:	4e73           	rte

00000c90 <Trap5Handler>:
     c90:	4e56 0000      	linkw %fp,#0
     c94:	4e71           	nop
     c96:	4e5e           	unlk %fp
     c98:	4e73           	rte

00000c9a <Trap6Handler>:
     c9a:	4e56 0000      	linkw %fp,#0
     c9e:	4e71           	nop
     ca0:	4e5e           	unlk %fp
     ca2:	4e73           	rte

00000ca4 <Trap7Handler>:
     ca4:	4e56 0000      	linkw %fp,#0
     ca8:	4e71           	nop
     caa:	4e5e           	unlk %fp
     cac:	4e73           	rte

00000cae <Trap8Handler>:
     cae:	4e56 0000      	linkw %fp,#0
     cb2:	4e71           	nop
     cb4:	4e5e           	unlk %fp
     cb6:	4e73           	rte

00000cb8 <Trap9Handler>:
     cb8:	4e56 0000      	linkw %fp,#0
     cbc:	4e71           	nop
     cbe:	4e5e           	unlk %fp
     cc0:	4e73           	rte

00000cc2 <TrapAHandler>:
     cc2:	4e56 0000      	linkw %fp,#0
     cc6:	4e71           	nop
     cc8:	4e5e           	unlk %fp
     cca:	4e73           	rte

00000ccc <TrapBHandler>:
     ccc:	4e56 0000      	linkw %fp,#0
     cd0:	4e71           	nop
     cd2:	4e5e           	unlk %fp
     cd4:	4e73           	rte

00000cd6 <TrapCHandler>:
     cd6:	4e56 0000      	linkw %fp,#0
     cda:	4e71           	nop
     cdc:	4e5e           	unlk %fp
     cde:	4e73           	rte

00000ce0 <TrapDHandler>:
     ce0:	4e56 0000      	linkw %fp,#0
     ce4:	4e71           	nop
     ce6:	4e5e           	unlk %fp
     ce8:	4e73           	rte

00000cea <TrapEHandler>:
     cea:	4e56 0000      	linkw %fp,#0
     cee:	4e71           	nop
     cf0:	4e5e           	unlk %fp
     cf2:	4e73           	rte

00000cf4 <TrapFHandler>:
     cf4:	4e56 0000      	linkw %fp,#0
     cf8:	4e71           	nop
     cfa:	4e5e           	unlk %fp
     cfc:	4e73           	rte
	...

00000d00 <set_console_output>:
     d00:	4e56 0000      	linkw %fp,#0
     d04:	4aae 0008      	tstl %fp@(8)
     d08:	6708           	beqs d12 <set_console_output+0x12>
     d0a:	23ee 0008 0008 	movel %fp@(8),80144 <cconout>
     d10:	0144 
     d12:	4e71           	nop
     d14:	4e5e           	unlk %fp
     d16:	4e75           	rts

00000d18 <set_console_input>:
     d18:	4e56 0000      	linkw %fp,#0
     d1c:	4aae 0008      	tstl %fp@(8)
     d20:	6708           	beqs d2a <set_console_input+0x12>
     d22:	23ee 0008 0008 	movel %fp@(8),80140 <cconin>
     d28:	0140 
     d2a:	4e71           	nop
     d2c:	4e5e           	unlk %fp
     d2e:	4e75           	rts

00000d30 <itox>:
     d30:	48e7 3820      	moveml %d2-%d4/%a2,%sp@-
     d34:	202f 0018      	movel %sp@(24),%d0
     d38:	2040           	moveal %d0,%a0
     d3a:	4aaf 0014      	tstl %sp@(20)
     d3e:	660e           	bnes d4e <itox+0x1e>
     d40:	10bc 0030      	moveb #48,%a0@
     d44:	4228 0001      	clrb %a0@(1)
     d48:	4cdf 041c      	moveml %sp@+,%d2-%d4/%a2
     d4c:	4e75           	rts
     d4e:	7200           	moveq #0,%d1
     d50:	741c           	moveq #28,%d2
     d52:	45f9 0000 15a8 	lea 15a8 <powers.0+0x28>,%a2
     d58:	262f 0014      	movel %sp@(20),%d3
     d5c:	e4ab           	lsrl %d2,%d3
     d5e:	0283 0000 000f 	andil #15,%d3
     d64:	8283           	orl %d3,%d1
     d66:	6706           	beqs d6e <itox+0x3e>
     d68:	10f2 3800      	moveb %a2@(0,%d3:l),%a0@+
     d6c:	7201           	moveq #1,%d1
     d6e:	5982           	subql #4,%d2
     d70:	76fc           	moveq #-4,%d3
     d72:	b682           	cmpl %d2,%d3
     d74:	66e2           	bnes d58 <itox+0x28>
     d76:	4210           	clrb %a0@
     d78:	60ce           	bras d48 <itox+0x18>

00000d7a <itodec>:
     d7a:	48e7 3020      	moveml %d2-%d3/%a2,%sp@-
     d7e:	222f 0010      	movel %sp@(16),%d1
     d82:	202f 0014      	movel %sp@(20),%d0
     d86:	2040           	moveal %d0,%a0
     d88:	4a81           	tstl %d1
     d8a:	660e           	bnes d9a <itodec+0x20>
     d8c:	10bc 0030      	moveb #48,%a0@
     d90:	4228 0001      	clrb %a0@(1)
     d94:	4cdf 040c      	moveml %sp@+,%d2-%d3/%a2
     d98:	4e75           	rts
     d9a:	4a81           	tstl %d1
     d9c:	6c06           	bges da4 <itodec+0x2a>
     d9e:	10fc 002d      	moveb #45,%a0@+
     da2:	4481           	negl %d1
     da4:	45f9 0000 1580 	lea 1580 <powers.0>,%a2
     daa:	93c9           	subal %a1,%a1
     dac:	261a           	movel %a2@+,%d3
     dae:	7400           	moveq #0,%d2
     db0:	b283           	cmpl %d3,%d1
     db2:	641e           	bccs dd2 <itodec+0x58>
     db4:	2609           	movel %a1,%d3
     db6:	8682           	orl %d2,%d3
     db8:	2243           	moveal %d3,%a1
     dba:	670a           	beqs dc6 <itodec+0x4c>
     dbc:	0602 0030      	addib #48,%d2
     dc0:	10c2           	moveb %d2,%a0@+
     dc2:	327c 0001      	moveaw #1,%a1
     dc6:	b5fc 0000 15a8 	cmpal #5544,%a2
     dcc:	66de           	bnes dac <itodec+0x32>
     dce:	4210           	clrb %a0@
     dd0:	60c2           	bras d94 <itodec+0x1a>
     dd2:	9283           	subl %d3,%d1
     dd4:	5282           	addql #1,%d2
     dd6:	60d8           	bras db0 <itodec+0x36>

00000dd8 <itooct>:
     dd8:	48e7 3800      	moveml %d2-%d4,%sp@-
     ddc:	202f 0014      	movel %sp@(20),%d0
     de0:	2040           	moveal %d0,%a0
     de2:	4aaf 0010      	tstl %sp@(16)
     de6:	660e           	bnes df6 <itooct+0x1e>
     de8:	10bc 0030      	moveb #48,%a0@
     dec:	4228 0001      	clrb %a0@(1)
     df0:	4cdf 001c      	moveml %sp@+,%d2-%d4
     df4:	4e75           	rts
     df6:	7200           	moveq #0,%d1
     df8:	741e           	moveq #30,%d2
     dfa:	262f 0010      	movel %sp@(16),%d3
     dfe:	e4ab           	lsrl %d2,%d3
     e00:	0283 0000 0007 	andil #7,%d3
     e06:	8283           	orl %d3,%d1
     e08:	6708           	beqs e12 <itooct+0x3a>
     e0a:	0603 0030      	addib #48,%d3
     e0e:	10c3           	moveb %d3,%a0@+
     e10:	7201           	moveq #1,%d1
     e12:	5782           	subql #3,%d2
     e14:	76fd           	moveq #-3,%d3
     e16:	b682           	cmpl %d2,%d3
     e18:	66e0           	bnes dfa <itooct+0x22>
     e1a:	4210           	clrb %a0@
     e1c:	60d2           	bras df0 <itooct+0x18>

00000e1e <itoudec>:
     e1e:	48e7 3020      	moveml %d2-%d3/%a2,%sp@-
     e22:	226f 0010      	moveal %sp@(16),%a1
     e26:	202f 0014      	movel %sp@(20),%d0
     e2a:	b2fc 0000      	cmpaw #0,%a1
     e2e:	6610           	bnes e40 <itoudec+0x22>
     e30:	2040           	moveal %d0,%a0
     e32:	10bc 0030      	moveb #48,%a0@
     e36:	4228 0001      	clrb %a0@(1)
     e3a:	4cdf 040c      	moveml %sp@+,%d2-%d3/%a2
     e3e:	4e75           	rts
     e40:	45f9 0000 1580 	lea 1580 <powers.0>,%a2
     e46:	2040           	moveal %d0,%a0
     e48:	7200           	moveq #0,%d1
     e4a:	261a           	movel %a2@+,%d3
     e4c:	7400           	moveq #0,%d2
     e4e:	b3c3           	cmpal %d3,%a1
     e50:	6418           	bccs e6a <itoudec+0x4c>
     e52:	8282           	orl %d2,%d1
     e54:	6708           	beqs e5e <itoudec+0x40>
     e56:	0602 0030      	addib #48,%d2
     e5a:	10c2           	moveb %d2,%a0@+
     e5c:	7201           	moveq #1,%d1
     e5e:	b5fc 0000 15a8 	cmpal #5544,%a2
     e64:	66e4           	bnes e4a <itoudec+0x2c>
     e66:	4210           	clrb %a0@
     e68:	60d0           	bras e3a <itoudec+0x1c>
     e6a:	93c3           	subal %d3,%a1
     e6c:	5282           	addql #1,%d2
     e6e:	60de           	bras e4e <itoudec+0x30>

00000e70 <pad_zeros>:
     e70:	2f0a           	movel %a2,%sp@-
     e72:	246f 0008      	moveal %sp@(8),%a2
     e76:	2f0a           	movel %a2,%sp@-
     e78:	4eb9 0000 10d8 	jsr 10d8 <strlen>
     e7e:	588f           	addql #4,%sp
     e80:	b0af 000c      	cmpl %sp@(12),%d0
     e84:	6c20           	bges ea6 <pad_zeros+0x36>
     e86:	206f 000c      	moveal %sp@(12),%a0
     e8a:	d1ca           	addal %a2,%a0
     e8c:	2200           	movel %d0,%d1
     e8e:	4a81           	tstl %d1
     e90:	6c18           	bges eaa <pad_zeros+0x3a>
     e92:	222f 000c      	movel %sp@(12),%d1
     e96:	9280           	subl %d0,%d1
     e98:	7000           	moveq #0,%d0
     e9a:	15bc 0030 0800 	moveb #48,%a2@(0,%d0:l)
     ea0:	5280           	addql #1,%d0
     ea2:	b280           	cmpl %d0,%d1
     ea4:	6ef4           	bgts e9a <pad_zeros+0x2a>
     ea6:	245f           	moveal %sp@+,%a2
     ea8:	4e75           	rts
     eaa:	10b2 1800      	moveb %a2@(0,%d1:l),%a0@
     eae:	5381           	subql #1,%d1
     eb0:	5388           	subql #1,%a0
     eb2:	60da           	bras e8e <pad_zeros+0x1e>

00000eb4 <printf>:
     eb4:	4fef ffd4      	lea %sp@(-44),%sp
     eb8:	48e7 383e      	moveml %d2-%d4/%a2-%fp,%sp@-
     ebc:	266f 0050      	moveal %sp@(80),%a3
     ec0:	4def 0054      	lea %sp@(84),%fp
     ec4:	7400           	moveq #0,%d2
     ec6:	49f9 0000 1104 	lea 1104 <putchar>,%a4
     ecc:	1013           	moveb %a3@,%d0
     ece:	660c           	bnes edc <printf+0x28>
     ed0:	2002           	movel %d2,%d0
     ed2:	4cdf 7c1c      	moveml %sp@+,%d2-%d4/%a2-%fp
     ed6:	4fef 002c      	lea %sp@(44),%sp
     eda:	4e75           	rts
     edc:	0c00 0025      	cmpib #37,%d0
     ee0:	6600 01e8      	bnew 10ca <printf+0x216>
     ee4:	41eb 0001      	lea %a3@(1),%a0
     ee8:	95ca           	subal %a2,%a2
     eea:	7600           	moveq #0,%d3
     eec:	2648           	moveal %a0,%a3
     eee:	1010           	moveb %a0@,%d0
     ef0:	1200           	moveb %d0,%d1
     ef2:	0601 ffd0      	addib #-48,%d1
     ef6:	0c01 0009      	cmpib #9,%d1
     efa:	6300 00a2      	blsw f9e <printf+0xea>
     efe:	0c00 006c      	cmpib #108,%d0
     f02:	6604           	bnes f08 <printf+0x54>
     f04:	47e8 0001      	lea %a0@(1),%a3
     f08:	1013           	moveb %a3@,%d0
     f0a:	0c00 0078      	cmpib #120,%d0
     f0e:	6e20           	bgts f30 <printf+0x7c>
     f10:	0c00 0062      	cmpib #98,%d0
     f14:	6e00 00b0      	bgtw fc6 <printf+0x112>
     f18:	0c00 0046      	cmpib #70,%d0
     f1c:	6700 013e      	beqw 105c <printf+0x1a8>
     f20:	0c00 0058      	cmpib #88,%d0
     f24:	6700 0178      	beqw 109e <printf+0x1ea>
     f28:	0c00 0025      	cmpib #37,%d0
     f2c:	6700 018e      	beqw 10bc <printf+0x208>
     f30:	7822           	moveq #34,%d4
     f32:	d88f           	addl %sp,%d4
     f34:	4878 000a      	pea a <_vectors+0xa>
     f38:	42a7           	clrl %sp@-
     f3a:	2f04           	movel %d4,%sp@-
     f3c:	4eb9 0000 10ec 	jsr 10ec <memset>
     f42:	4878 0025      	pea 25 <_vectors+0x25>
     f46:	4bf9 0000 1104 	lea 1104 <putchar>,%a5
     f4c:	4e95           	jsr %a5@
     f4e:	4fef 0010      	lea %sp@(16),%sp
     f52:	4a83           	tstl %d3
     f54:	6708           	beqs f5e <printf+0xaa>
     f56:	4878 0030      	pea 30 <_vectors+0x30>
     f5a:	4e95           	jsr %a5@
     f5c:	588f           	addql #4,%sp
     f5e:	b4fc 0000      	cmpaw #0,%a2
     f62:	6f14           	bles f78 <printf+0xc4>
     f64:	2f04           	movel %d4,%sp@-
     f66:	2f0a           	movel %a2,%sp@-
     f68:	4eba fe10      	jsr %pc@(d7a <itodec>)
     f6c:	2f04           	movel %d4,%sp@-
     f6e:	4eb9 0000 1120 	jsr 1120 <puts>
     f74:	4fef 000c      	lea %sp@(12),%sp
     f78:	1013           	moveb %a3@,%d0
     f7a:	4880           	extw %d0
     f7c:	3040           	moveaw %d0,%a0
     f7e:	2f08           	movel %a0,%sp@-
     f80:	4e95           	jsr %a5@
     f82:	588f           	addql #4,%sp
     f84:	b4fc 0000      	cmpaw #0,%a2
     f88:	6f00 013a      	blew 10c4 <printf+0x210>
     f8c:	2f04           	movel %d4,%sp@-
     f8e:	4eb9 0000 10d8 	jsr 10d8 <strlen>
     f94:	5480           	addql #2,%d0
     f96:	588f           	addql #4,%sp
     f98:	d480           	addl %d0,%d2
     f9a:	6000 00f0      	braw 108c <printf+0x1d8>
     f9e:	5288           	addql #1,%a0
     fa0:	0c00 0030      	cmpib #48,%d0
     fa4:	6606           	bnes fac <printf+0xf8>
     fa6:	b4fc 0000      	cmpaw #0,%a2
     faa:	6714           	beqs fc0 <printf+0x10c>
     fac:	220a           	movel %a2,%d1
     fae:	d28a           	addl %a2,%d1
     fb0:	d281           	addl %d1,%d1
     fb2:	d5c1           	addal %d1,%a2
     fb4:	d5ca           	addal %a2,%a2
     fb6:	4880           	extw %d0
     fb8:	45f2 00d0      	lea %a2@(ffffffffffffffd0,%d0:w),%a2
     fbc:	6000 ff2e      	braw eec <printf+0x38>
     fc0:	7601           	moveq #1,%d3
     fc2:	6000 ff28      	braw eec <printf+0x38>
     fc6:	0600 ff9d      	addib #-99,%d0
     fca:	0c00 0015      	cmpib #21,%d0
     fce:	6200 ff60      	bhiw f30 <printf+0x7c>
     fd2:	0280 0000 00ff 	andil #255,%d0
     fd8:	d080           	addl %d0,%d0
     fda:	303b 0806      	movew %pc@(fe2 <printf+0x12e>,%d0:l),%d0
     fde:	4efb 0002      	jmp %pc@(fe2 <printf+0x12e>,%d0:w)
     fe2:	002c 003e ff4e 	orib #62,%a4@(-178)
     fe8:	007a           	.short 0x007a
     fea:	ff4e           	.short 0xff4e
     fec:	ff4e           	.short 0xff4e
     fee:	003e           	.short 0x003e
     ff0:	ff4e           	.short 0xff4e
     ff2:	ff4e           	.short 0xff4e
     ff4:	ff4e           	.short 0xff4e
     ff6:	ff4e           	.short 0xff4e
     ff8:	ff4e           	.short 0xff4e
     ffa:	00ca           	.short 0x00ca
     ffc:	ff4e           	.short 0xff4e
     ffe:	ff4e           	.short 0xff4e
    1000:	ff4e           	.short 0xff4e
    1002:	008e           	.short 0x008e
    1004:	ff4e           	.short 0xff4e
    1006:	006c ff4e ff4e 	oriw #-178,%a4@(-178)
    100c:	00bc           	.short 0x00bc
    100e:	201e           	movel %fp@+,%d0
    1010:	4880           	extw %d0
    1012:	3040           	moveaw %d0,%a0
    1014:	2f08           	movel %a0,%sp@-
    1016:	4eb9 0000 1104 	jsr 1104 <putchar>
    101c:	5282           	addql #1,%d2
    101e:	604c           	bras 106c <printf+0x1b8>
    1020:	4bef 002c      	lea %sp@(44),%a5
    1024:	2f0d           	movel %a5,%sp@-
    1026:	2f1e           	movel %fp@+,%sp@-
    1028:	4eba fd50      	jsr %pc@(d7a <itodec>)
    102c:	508f           	addql #8,%sp
    102e:	4a83           	tstl %d3
    1030:	6710           	beqs 1042 <printf+0x18e>
    1032:	b4fc 0000      	cmpaw #0,%a2
    1036:	6f0a           	bles 1042 <printf+0x18e>
    1038:	2f0a           	movel %a2,%sp@-
    103a:	2f0d           	movel %a5,%sp@-
    103c:	4eba fe32      	jsr %pc@(e70 <pad_zeros>)
    1040:	508f           	addql #8,%sp
    1042:	2f0d           	movel %a5,%sp@-
    1044:	4eb9 0000 1120 	jsr 1120 <puts>
    104a:	2f0d           	movel %a5,%sp@-
    104c:	6034           	bras 1082 <printf+0x1ce>
    104e:	4bef 002c      	lea %sp@(44),%a5
    1052:	2f0d           	movel %a5,%sp@-
    1054:	2f1e           	movel %fp@+,%sp@-
    1056:	4eba fdc6      	jsr %pc@(e1e <itoudec>)
    105a:	60d0           	bras 102c <printf+0x178>
    105c:	4879 0000 15b9 	pea 15b9 <powers.0+0x39>
    1062:	4eb9 0000 1120 	jsr 1120 <puts>
    1068:	5e82           	addql #7,%d2
    106a:	508e           	addql #8,%fp
    106c:	588f           	addql #4,%sp
    106e:	601c           	bras 108c <printf+0x1d8>
    1070:	2616           	movel %fp@,%d3
    1072:	588e           	addql #4,%fp
    1074:	41f9 0000 1120 	lea 1120 <puts>,%a0
    107a:	6716           	beqs 1092 <printf+0x1de>
    107c:	2f03           	movel %d3,%sp@-
    107e:	4e90           	jsr %a0@
    1080:	2f03           	movel %d3,%sp@-
    1082:	4eb9 0000 10d8 	jsr 10d8 <strlen>
    1088:	d480           	addl %d0,%d2
    108a:	508f           	addql #8,%sp
    108c:	528b           	addql #1,%a3
    108e:	6000 fe3c      	braw ecc <printf+0x18>
    1092:	4879 0000 15c1 	pea 15c1 <powers.0+0x41>
    1098:	4e90           	jsr %a0@
    109a:	5c82           	addql #6,%d2
    109c:	60ce           	bras 106c <printf+0x1b8>
    109e:	4bef 002c      	lea %sp@(44),%a5
    10a2:	2f0d           	movel %a5,%sp@-
    10a4:	2f1e           	movel %fp@+,%sp@-
    10a6:	4eba fc88      	jsr %pc@(d30 <itox>)
    10aa:	6080           	bras 102c <printf+0x178>
    10ac:	4bef 002c      	lea %sp@(44),%a5
    10b0:	2f0d           	movel %a5,%sp@-
    10b2:	2f1e           	movel %fp@+,%sp@-
    10b4:	4eba fd22      	jsr %pc@(dd8 <itooct>)
    10b8:	6000 ff72      	braw 102c <printf+0x178>
    10bc:	4878 0025      	pea 25 <_vectors+0x25>
    10c0:	6000 ff54      	braw 1016 <printf+0x162>
    10c4:	7002           	moveq #2,%d0
    10c6:	6000 fed0      	braw f98 <printf+0xe4>
    10ca:	4880           	extw %d0
    10cc:	3040           	moveaw %d0,%a0
    10ce:	2f08           	movel %a0,%sp@-
    10d0:	4e94           	jsr %a4@
    10d2:	6000 ff48      	braw 101c <printf+0x168>
	...

000010d8 <strlen>:
    10d8:	206f 0004      	moveal %sp@(4),%a0
    10dc:	7000           	moveq #0,%d0
    10de:	4a30 0800      	tstb %a0@(0,%d0:l)
    10e2:	6602           	bnes 10e6 <strlen+0xe>
    10e4:	4e75           	rts
    10e6:	5280           	addql #1,%d0
    10e8:	60f4           	bras 10de <strlen+0x6>
	...

000010ec <memset>:
    10ec:	206f 0004      	moveal %sp@(4),%a0
    10f0:	2008           	movel %a0,%d0
    10f2:	d0af 000c      	addl %sp@(12),%d0
    10f6:	b088           	cmpl %a0,%d0
    10f8:	6602           	bnes 10fc <memset+0x10>
    10fa:	4e75           	rts
    10fc:	10ef 000b      	moveb %sp@(11),%a0@+
    1100:	60f4           	bras 10f6 <memset+0xa>
	...

00001104 <putchar>:
    1104:	2f02           	movel %d2,%sp@-
    1106:	142f 000b      	moveb %sp@(11),%d2
    110a:	48e7 c080      	moveml %d0-%d1/%a0,%sp@-
    110e:	1202           	moveb %d2,%d1
    1110:	303c 0002      	movew #2,%d0
    1114:	4e41           	trap #1
    1116:	4cdf 0103      	moveml %sp@+,%d0-%d1/%a0
    111a:	241f           	movel %sp@+,%d2
    111c:	4e75           	rts
	...

00001120 <puts>:
    1120:	2f0b           	movel %a3,%sp@-
    1122:	2f0a           	movel %a2,%sp@-
    1124:	246f 000c      	moveal %sp@(12),%a2
    1128:	47f9 0000 1104 	lea 1104 <putchar>,%a3
    112e:	1012           	moveb %a2@,%d0
    1130:	6606           	bnes 1138 <puts+0x18>
    1132:	245f           	moveal %sp@+,%a2
    1134:	265f           	moveal %sp@+,%a3
    1136:	4e75           	rts
    1138:	528a           	addql #1,%a2
    113a:	4880           	extw %d0
    113c:	3040           	moveaw %d0,%a0
    113e:	2f08           	movel %a0,%sp@-
    1140:	4e93           	jsr %a3@
    1142:	588f           	addql #4,%sp
    1144:	60e8           	bras 112e <puts+0xe>

00001146 <MsgOrionInit>:
    1146:	5044 5333 3137 202d 2063 6f70 7972 6967     PDS317 - copyrig
    1156:	6874 2028 4329 2070 6473 696c 7661 2870     ht (C) pdsilva(p
    1166:	676f 7264 616f 292e 5642 7567 322e 310a     gordao).VBug2.1.
    1176:	4d43 3638 3030 3020 5379 7374 656d 204d     MC68000 System M
    1186:	6f6e 6974 6f72 0a42 7569 6c64 2044 6174     onitor.Build Dat
    1196:	653a 204d 6179 2033 3120 3230 3236 202d     e: May 31 2026 -
    11a6:	2031 333a 3234 3a31 350a 4275 696c 6420      13:24:15.Build 
    11b6:	436f 756e 7465 723a 2032 3231 3531 0a2d     Counter: 22151.-
    11c6:	2d2d 2d2d 2d2d 2d2d 2d2d 2d2d 2d2d 2d2d     ----------------
    11d6:	2d2d 2d2d 2d2d 2d2d 2d2d 2d2d 2d2d 2d2d     ----------------
    11e6:	2d2d 2d2d 2d2d 2d2d 2d2d 2d2d 2d2d 0a0a     --------------..
    11f6:	0049 6e69 6369 616e 646f 206f 2073 6973     .Iniciando o sis
    1206:	7465 6d61 2050 4453 3331 370a 0025 7300     tema PDS317..%s.
    1216:	5642 7567 322e 3120 2d20 4d65 6e75 0a00     VBug2.1 - Menu..
    1226:	2030 202d 2043 6c65 6172 2073 6372 6565      0 - Clear scree
    1236:	6e0a 0020 3120 2d20 5665 7269 6669 6361     n.. 1 - Verifica
    1246:	7220 6f20 7379 7374 656d 7469 636b 0a00     r o systemtick..
    1256:	2032 202d 204d 656d 6f72 7920 6475 6d70      2 - Memory dump
    1266:	0a00 2033 202d 2058 6d6f 6465 6d20 646f     .. 3 - Xmodem do
    1276:	776e 6c6f 6164 0a00 4368 6f6f 7365 2061     wnload..Choose a
    1286:	6e20 6f70 7469 6f6e 3a20 0025 6300 0a00     n option: .%c...
    1296:	7379 7374 656d 5469 636b 3a20 5b25 3038     systemTick: [%08
    12a6:	6c64 5d0a 0057 726f 6e67 206f 7074 696f     ld]..Wrong optio
    12b6:	6e0a                                         n..

000012b9 <OE_BASE_KEYMAP>:
    12b9:	0000 0000 6162 6364 6566 6768 696a 6b6c     ....abcdefghijkl
    12c9:	6d6e 6f70 7172 7374 7576 7778 797a 3132     mnopqrstuvwxyz12
    12d9:	3334 3536 3738 3930 0d1b 0809 202d 3d5b     34567890.... -=[
    12e9:	5d5c ff3b 2760 2c2e 2f02 f1f2 f3f4 f5f6     ]\.;'`,./.......
    12f9:	f7f8 f9fa fbfc 1ff0 fe18 1415 7f17 1613     ................
    1309:	1211 1005 2f2a 2d2b 0d31 3233 3435 3637     ..../*-+.1234567
    1319:	3839 302e ff0a 0000 0000 0000 0000 0000     890.............
	...

00001339 <OE_SHIFT_KEYMAP>:
    1339:	0000 0000 4142 4344 4546 4748 494a 4b4c     ....ABCDEFGHIJKL
    1349:	4d4e 4f50 5152 5354 5556 5758 595a 2140     MNOPQRSTUVWXYZ!@
    1359:	2324 255e 262a 2829 0d1b 0809 205f 2b7b     #$%^&*().... _+{
    1369:	7d7c ff3a 227e 3c3e 3f02 f1f2 f3f4 f5f6     }|.:"~<>?.......
    1379:	f7f8 f9fa fbfc 1ff0 fe18 1415 7f17 1613     ................
    1389:	1211 1005 2f2a 2d2b 0d31 3233 3435 3637     ..../*-+.1234567
    1399:	3839 302e ff0a 0000 0000 0000 0000 0000     890.............
	...

000013b9 <OE_CTRL_KEYMAP>:
    13b9:	0000 0000 a1a2 a3a4 a5a6 a7a8 a9aa abac     ................
    13c9:	adae afb0 b1b2 b3b4 b5b6 b7b8 b9ba e1e2     ................
    13d9:	e3e4 e5e6 e7e8 e9e0 0f00 0800 2000 0000     ............ ...
	...
    13f1:	0000 f1f2 f3f4 f5f6 f7f8 f9fa fbfc 0000     ................
    1401:	0000 0000 0000 0013 1211 1000 0000 0000     ................
	...

00001439 <OE_ALT_KEYMAP>:
    1439:	0000 0000 c1c2 c3c4 c5c6 c7c8 c9ca cbcc     ................
    1449:	cdce cfd0 d1d2 d3d4 d5d6 d7d8 d9da bcbd     ................
    1459:	bebf dbdc ddde dfbb 0d00 0800 2000 0000     ............ ...
	...
    1471:	0000 f1f2 f3f4 f5f6 f7f8 f9fa fbfc 0000     ................
    1481:	0000 0000 0000 0013 1211 1000 0000 0000     ................
	...

000014b9 <OE_ALTGR_KEYMAP>:
    14b9:	0000 0000 8182 8384 8586 8788 898a 8b8c     ................
    14c9:	8d8e 8f90 9192 9394 9596 9798 999a 9c9d     ................
    14d9:	9e9f eaeb eced ee9b 0d00 0800 2000 0000     ............ ...
	...
    14f1:	0000 f1f2 f3f4 f5f6 f7f8 f9fa fbfc 0000     ................
    1501:	0000 0000 0000 0013 1211 1000 0000 0000     ................
	...
    1539:	7c00 2e00 2563 0025 3032 5800 5365 7461     |...%c.%02X.Seta
    1549:	6e64 6f20 6120 706f 7369 7469 6f6e 2059     ndo a position Y
    1559:	205b 2530 3258 5d20 585b 2530 3258 5d0a      [%02X] X[%02X].
    1569:	0053 6574 616e 646f 2061 2063 6f72 205b     .Setando a cor [
    1579:	2530 3258 5d0a                               %02X]..

00001580 <powers.0>:
    1580:	3b9a ca00 05f5 e100 0098 9680 000f 4240     ;.............B@
    1590:	0001 86a0 0000 2710 0000 03e8 0000 0064     ......'........d
    15a0:	0000 000a 0000 0001                         ........
