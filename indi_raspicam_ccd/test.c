
#include <stdio.h>
#include <signal.h>

#define BLOCK_SIZE 512

void my_handler(int signum)
{
    if (signum == SIGUSR1)
    {
        FILE* f = fopen("test.bmp", "rb");

        if (f)
        {
            char buffer[BLOCK_SIZE];
            for(;;)
            {
                size_t bytes = fread(buffer, sizeof(char),BLOCK_SIZE, f);
                fwrite(buffer, sizeof(char), bytes, stdout);
                fflush(stdout);
                if (bytes < BLOCK_SIZE)
                    if (feof(f))
                        break;
            }
        }
    }
}

int main( int argc, const char* argv[] )
{
    signal(SIGUSR1, my_handler);
    while (1)
    {
        pause();
    }
}
