//
//
#define __KERNEL_CS     0x10
#define __KERNEL_DS     0x18

#define __USER_CS       0x23
#define __USER_DS       0x2B

#define PAGE_SHIFT                      12
#define PAGE_SIZE                       ((unsigned long)1<<PAGE_SHIFT)

long user_stack [ PAGE_SIZE>>2 ];

struct {
  long * a;
  short b;
} stack_start = { &user_stack[PAGE_SIZE>>2], __KERNEL_DS };

static void non_function(void) {
}
