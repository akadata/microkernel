#define SAVE_CONTEXT() \
{ \
    __asm__ __volatile__ ( \
      "push r31 \n\t" \
      /* Push the status register. */ \
      "in r31, __SREG__ \n\t" \
      "push r31 \n\t" \
      "push r30 \n\t" \
      "push r29 \n\t" \
      "push r28 \n\t" \
      "push r27 \n\t" \
      "push r26 \n\t" \
      "push r25 \n\t" \
      "push r24 \n\t" \
      "push r23 \n\t" \
      "push r22 \n\t" \
      "push r21 \n\t" \
      "push r20 \n\t" \
      "push r19 \n\t" \
      "push r18 \n\t" \
      "push r17 \n\t" \
      "push r16 \n\t" \
      "push r15 \n\t" \
      "push r14 \n\t" \
      "push r13 \n\t" \
      "push r12 \n\t" \
      "push r11 \n\t" \
      "push r10 \n\t" \
      "push r9 \n\t" \
      "push r8 \n\t" \
      "push r7 \n\t" \
      "push r6 \n\t" \
      "push r5 \n\t" \
      "push r4 \n\t" \
      "push r3 \n\t" \
      "push r2 \n\t" \
      "push r1 \n\t" \
      "push r0 \n\t" \
      /* The C compiler assumes that the zero register is \
      zero. */ \
      "clr r1 \n\t" \
      /* Store the stack pointer at a safe place. */ \
      "ldi r26, lo8(inter_sp) \n\t" \
      "ldi r27, hi8(inter_sp) \n\t" \
      "in r0, __SP_L__ \n\t" \
      "st x+, r0 \n\t" \
      "in r0, __SP_H__ \n\t" \
      "st x, r0 \n\t"); \
}

#define RESTORE_CONTEXT() \
{ \
    __asm__ __volatile__ ( \
      /* Install the new stack pointer inter_sp. */ \
      "ldi r26, lo8(inter_sp) \n\t" \
      "ldi r27, hi8(inter_sp) \n\t"  \
      "ld r28, x+ \n\t" \
      "out __SP_L__, r28 \n\t" \
      "ld r29, x \n\t" \
      "out __SP_H__, r29 \n\t" \
      /* The SP register is restored. Now pop the saved \
      register values back to the registers. We start with \
      the general purpose registers. */ \
      "pop r0 \n\t" \
      "pop r1 \n\t" \
      "pop r2 \n\t" \
      "pop r3 \n\t" \
      "pop r4 \n\t" \
      "pop r5 \n\t" \
      "pop r6 \n\t" \
      "pop r7 \n\t" \
      "pop r8 \n\t" \
      "pop r9 \n\t" \
      "pop r10 \n\t" \
      "pop r11 \n\t" \
      "pop r12 \n\t" \
      "pop r13 \n\t" \
      "pop r14 \n\t" \
      "pop r15 \n\t" \
      "pop r16 \n\t" \
      "pop r17 \n\t" \
      "pop r18 \n\t" \
      "pop r19 \n\t" \
      "pop r20 \n\t" \
      "pop r21 \n\t" \
      "pop r22 \n\t" \
      "pop r23 \n\t" \
      "pop r24 \n\t" \
      "pop r25 \n\t" \
      "pop r26 \n\t" \
      "pop r27 \n\t" \
      "pop r28 \n\t" \
      "pop r29 \n\t" \
      "pop r30 \n\t" \
      "pop r31 \n\t" \
      /* status register */ \
      "out __SREG__, r31 \n\t" \
      "pop r31 \n\t" \
     ); \
}

struct context {
    /* an anonymous padding byte */
    unsigned char _padding_byte;
    /* general purpose registers */
    unsigned char r0,  r1,  r2,  r3,  r4,  r5,  r6,  r7;
    unsigned char r8,  r9,  r10, r11, r12, r13, r14, r15;
    unsigned char r16, r17, r18, r19, r20, r21, r22, r23;
    unsigned char r24, r25, r26, r27, r28, r29, r30;
    /* status register */
    unsigned char rSREG;
    unsigned char r31;
    /* program counter */
    unsigned char pc_low, pc_high;
};

void abort(void);
