#include "../utils/servidor.h"

int main(){
	char* ip = "127.0.0.1";
	char* puerto = "4444";

	iniciar_servidor(ip, puerto);
    return 0;
}
