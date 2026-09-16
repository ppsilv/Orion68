typedef struct __attribute__((packed)) {
    uint32_t d[8];   /* D0-D7 */
    uint32_t a[7];   /* A0-A6 -- a[5] é A5 */
    uint16_t sr;
    uint32_t pc;
} task_init_frame_t;

void Task_CreateFromELF(TCB *tcb, void *stack_base, uint32_t stack_size,
                         uint32_t entry, uint32_t a5_value)
{
    uint8_t *stack_top = (uint8_t*)stack_base + stack_size;
    task_init_frame_t *frame =
        (task_init_frame_t*)(stack_top - sizeof(task_init_frame_t));

    memset(frame, 0, sizeof(*frame));
    frame->a[5] = a5_value;   /* A5 = base da GOT, pra task PIC funcionar */
    frame->sr   = 0x2000;     /* modo supervisor, interrupcoes habilitadas */
    frame->pc   = entry;

    tcb->sp = (uint32_t)frame;   /* offset 0 da TCB, que voce ja usa */
}
