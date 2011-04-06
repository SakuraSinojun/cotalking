
#include <stdio.h>
#include "../include/manager.h"

int main(int argc, char **argv)
{
	TS_Manager *manager = new TS_Manager();
	manager->init(argc, argv);
	manager->run();

	return 0;
}

