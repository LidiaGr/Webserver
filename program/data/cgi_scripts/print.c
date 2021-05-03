#include <unistd.h>

int main ()
{
	write(1, "HELLO", 5);
	write(1, "\0", 1);
}
