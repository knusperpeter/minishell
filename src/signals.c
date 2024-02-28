


/*function that handles the cmd+c (SIGINT) signal*/
void signal_cmd_c(int signal, siginfo_t *info, void *ucontent)
{
    (void)ucontext;
    (void)info;
    ////give the promt back!!!
    return (130);
}

/*function that handles the cmd+d (SIGTERM) signal*/
void signal_cmd_d(int signal, siginfo_t *info, void *ucontent)
{
    (void)ucontext;
    (void)info;
    printf("exit\n");                           //original funnction
    exit (0);

}

/*function that handles the cmd+\ (SIGQUIT) signal*/
void signal_cmd_bs(int signal, siginfo_t *info, void *ucontent)
{
    (void)ucontext;
    (void)info;

    printf("\Quit (core dumped)\n");
    return (131);
}

int main(void)
{
    struct sigaction    shell;
    
    shell.sa_flags = SA_SIGINFO;
    shell.sa_sigaction = &signal_cmd_c;
    sigemptyset(&shell.sa_mask);
    sigaction(SIGINT, &shell, NULL);

    shell.sa_sigaction = &signal_cmd_d;
    sigaction(SIGTERM, &shell, NULL);

    shell.sa_sigaction = &signal_cmd_bs;
    sigaction(SIGQUIT, &shell, NULL);

    while (1)
        sleep(1);
    return (0);
}
