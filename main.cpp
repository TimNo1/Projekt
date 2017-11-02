#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "ProjektConfig.h"
#include "finder.h"

int main (int argc, char *argv[])
{
    fprintf(stdout,"Version %d.%d\n",
                Projekt_VERSION_MAJOR,
                Projekt_VERSION_MINOR);
    
    fprintf(stdout, "Enter a string to get all the 3mers of:\n");
    std::string s = "";
    getline(std::cin, s);
    
    std::vector<finder::kMerTriple*> v = *finder::getKmers(s, 3);
        
    for (long i=0, n=v.size(); i<n; i++) {
        fprintf(stdout, "%s %ld\n", v[i]->s.c_str(), v[i]->i);
    }
        
    return 0;
}

