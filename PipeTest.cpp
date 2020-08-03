int main() {
	int pipefd[2];
	pid_t cpid;
	pid_t ppid;

	int i = 0;
	for (int i = 0; i < num; i++) {
		cpid = fork();
		if (cpid == -1) {
			printf("Fork process error!\n");
			return 0;
		}
		if (cpid == 0）{
			break;
		}
	}
	if (i < num) {
		close(pipefd[0]);
		std::string tmp = std::tostring(i);
		for (int j = 0; j <= i; j++) {
			tmp = tmp + ", " + std::tostring(j);
			if(write(pipefd[1], const_cast<char*>(tmp.c_str())) == -1) {
				printf("Process %d write packet %s error!\n", getpid(), packet);
				exit(-1);
			}
			if(kill(ppid, SIGUSR1) == -1) {
				perror("Fail to send SIGUSR1!\n");
				exit(-1);
			}
			sleep(1);
		}
				

