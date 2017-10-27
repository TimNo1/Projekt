#include <stdio.h>
#include <stdlib.h>
#include "ProjektConfig.h"
int main (int argc, char *argv[])
{
    fprintf(stdout,"Version %d.%d\n",
            Projekt_VERSION_MAJOR,
            Projekt_VERSION_MINOR);
  return 0;
}
