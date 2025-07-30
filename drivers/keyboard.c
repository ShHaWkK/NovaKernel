#include "keyboard.h"
#include "../src/io.h"

#define BUF_SIZE 128
static char buffer[BUF_SIZE];
static int head = 0;
static int tail = 0;

/* Simple scancode to ASCII map for set 1 */
static const char keymap[128] = {
    0,27,'1','2','3','4','5','6','7','8','9','0','-','=',0,'\t',
    'q','w','e','r','t','y','u','i','o','p','[',']','\n',0,'a','s',
    'd','f','g','h','j','k','l',';','\'','`',0,'\\','z','x','c','v','b',
    'n','m',',','.','/',0,'*',0,' ',0,0,0,0,0,0,0,
};

void keyboard_init(void) {
    /* unmask keyboard IRQ1 */
    outb(0x21, inb(0x21) & ~0x02);
}

static void push_char(char c) {
    int next = (head + 1) % BUF_SIZE;
    if (next != tail) {
        buffer[head] = c;
        head = next;
    }
}

void keyboard_isr(void) {
    uint8_t sc = inb(0x60);
    if (sc < sizeof(keymap)) {
        char c = keymap[sc];
        if (c)
            push_char(c);
    }
}

int keyboard_getc(void) {
    if (head == tail)
        return -1;
    char c = buffer[tail];
    tail = (tail + 1) % BUF_SIZE;
    return c;
}

