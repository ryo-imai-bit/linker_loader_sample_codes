extern int extvalue;;
extern int extbssvalue;
int extfunc();

static int dummy_value = 3;
static int dummy_bssvalue;
static int value = 1;
static int bssvalue;
char *string;

static void dummy_func() {};

static int func() {
	return (extvalue + extbssvalue + value + bssvalue);
}

int main() {
	extfunc();
	func();
	string = "sample_string";
	return (0);
}

// The role of the %rip register https://cs.brown.edu/courses/csci1310/2020/notes/l08.html
// The %rip register on x86-64 is a special-purpose register that always holds the memory address of the next instruction to execute in the program's code segment. The processor increments %rip automatically after each instruction, and control flow instructions like branches set the value of %rip to change the next instruction.
// Perhaps surprisingly, %rip also shows up when an assembly program refers to a global variable. See the sidebar under "Addressing modes" below to understand how %rip-relative addressing works.

/**
%rip-relative addressing for global variables
x86-64 code often refers to globals using %rip-relative addressing: a global variable named a is referenced as a(%rip). This style of reference supports position-independent code (PIC), a security feature. It specifically supports position-independent executables (PIEs), which are programs that work independently of where their code is loaded into memory.

When the operating system loads a PIE, it picks a random starting point and loads all instructions and globals relative to that starting point. The PIE's instructions never refer to global variables using direct addressing: there is no movl global_int, %eax. Globals are referenced relatively instead, using deltas relative to the next %rip: to load a global variable into a register, the compiler emits movl global_int(%rip), %eax. These relative addresses work independent of the starting point! For instance, consider an instruction located at (starting-point + 0x80) that loads a variable g located at (starting-point + 0x1000) into %rax. In a non-PIE, the instruction might be written as movq g, %rax; but this relies on g having a fixed address. In a PIE, the instruction might be written movq g(%rip), %rax, which works out without having to know the starting address of the program's code in memory at compile time (instead, %rip contains a number some known number of bytes apart from the starting point, so any address relative to %rip is also relative to the starting point).

At starting point…	The mov instruction is at…	The next instruction is at…	And g is at…	So the delta (g - next %rip) is…
0x400000	0x400080	0x400087	0x401000	0xF79
0x404000	0x404080	0x404087	0x405000	0xF79
0x4003F0	0x400470	0x400477	0x4013F0	0xF79
*/
