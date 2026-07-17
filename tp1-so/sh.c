#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*
Answers can be either in Portuguese or in English.
Respostas podem sem tanto em português como em inglês.
*/

/*
1. Term of commitment

The group members declare that all code developed for this project is their own.
The group members declare that they have not copied material from the Internet
  nor obtained code from third parties.

2. Group members and allocation of effort

Fill in the lines below with the name and email of the group members.
Replace XX with the contribution of each group member in the development of the work.

João Carlos Ferraz de Sousa johncharles2003@ufmg.br 50%
Luiz Felipe Gondim Araujo luizfelipegondim@ufmg.br 50%

3. Solutions

Briefly describe the solutions implemented for this project and justify their choices.

Na Task 1 foi implementada a função fork1 com chamada de sistema fork e com verificação 
explicita de erro por meio de perror e retorno do identificador do processo criado. 

Na Task 2 foi implementada a execução de comandos simples utilizando execvp. A escolha de 
execvp permite que o sistema operacional resolva automaticamente o caminho do executável 
com base na variável PATH, o que simplificou a implementação.

Na Task 3 foi implementado o redirecionamento de entrada e saída utilizando open para obtenção 
do descritor de arquivo e dup2 para associar esse descritor ao fluxo padrão correspondente. 
A escolha de dup2, em vez de dup, foi feita porque dup2 permite especificar explicitamente qual 
descritor será substituído, o que garante que stdin ou stdout sejam corretamente redirecionados. 
Além disso, dup2 fecha automaticamente o descritor de destino caso já esteja em uso, evitando 
inconsistências e erros como descritores inválidos. Após o redirecionamento, o comando associado 
é executado de forma recursiva, garantindo integração com outras funcionalidades.

Na Task 4 foi implementado o mecanismo de pipes utilizando pipe para criação do canal de 
comunicação e fork para geração de dois processos filhos independentes. Em seguida, dup é 
utilizado para redirecionar a saída do processo da esquerda para a extremidade de escrita do 
pipe e a entrada do processo da direita para a extremidade de leitura.

Na Task 5 o comando cd foi tratado diretamente no processo principal do shell, pois a alteração 
de diretório precisa persistir entre os comandos realizados. Caso fosse executado em um processo 
filho, a mudança ocorreria apenas naquele contexto e seria perdida após o término do processo. 
A mensagem de erro foi ajustada para refletir o que realmente o erro quer dizer, indicando a 
inexistência do diretório em vez de um problema relacionado a processos.

4. Bibliographic references

Add the bibliographic references here.

  https://man7.org/linux/man-pages/

  https://man7.org/linux/man-pages/man2/fork.2.html
  https://man7.org/linux/man-pages/man3/exec.3.html
  https://man7.org/linux/man-pages/man2/pipe.2.html
  https://man7.org/linux/man-pages/man2/dup.2.html
  https://man7.org/linux/man-pages/man2/open.2.html

*/

/****************************************************************
 * Simplified xv6 Shell
 *
 * This code was adapted from the UNIX xv6 code and material from
 * the MIT Operating Systems course (6.828).
 ***************************************************************/

#define MAXARGS 10

/* Every command has a type. After identifying the command's type,
    the code converts a *cmd into the specific command type. */
struct cmd {
    int type; /* ' ' (exec)
                 '|' (pipe)
                 '<' or '>' (redirection) */
};

struct execcmd {
    int type;             // ' ' (exec)
    char *argv[MAXARGS];  // Arguments for the command to be executed
};

struct redircmd {
    int type;         // < or > (redirection)
    struct cmd *cmd;  // The command to execute (e.g., an execcmd)
    char *file;       // The input or output file
    int mode;         // The mode in which the file should be opened
    int fd;           // The file descriptor number to be used
};

struct pipecmd {
    int type;           // | (pipe)
    struct cmd *left;   // Left side of the pipe
    struct cmd *right;  // Right side of the pipe
};

int fork1(void);                                        // Fork but exit if an error occurs
struct cmd *parsecmd(char *);                           // Process the command line
void handle_simple_cmd(struct execcmd *ecmd);           // Handle simple commands
void handle_redirection(struct redircmd *rcmd);         // Handle redirection
void handle_pipe(struct pipecmd *pcmd, int *p, int r);  // Handle pipes

/* Execute the command cmd. It never returns. */
void runcmd(struct cmd *cmd) {
    int p[2], r;
    struct execcmd *ecmd;
    struct pipecmd *pcmd;
    struct redircmd *rcmd;

    if (cmd == 0)
        exit(0);

    switch (cmd->type) {
        default:
            fprintf(stderr, "Unknown command type\n");
            exit(-1);

        case ' ':
            ecmd = (struct execcmd *)cmd;
            if (ecmd->argv[0] == 0)
                exit(0);
            handle_simple_cmd(ecmd);
            break;

        case '>':
        case '<':
            rcmd = (struct redircmd *)cmd;
            handle_redirection(rcmd);
            runcmd(rcmd->cmd);
            break;

        case '|':
            pcmd = (struct pipecmd *)cmd;
            handle_pipe(pcmd, p, r);
            break;
    }
    exit(0);
}

int fork1(void) {
    /* Task 1: Implement the fork1 function.
    The function is supposed to create a new process using the `fork()` system call.
    It should print a message if the fork fails, otherwise return the process ID of the child process (or -1 if the fork fails).
    */
    /* END OF TASK 1 */
    int pid = fork();

    if (pid < 0) {
        perror("fork");
        return -1;
    }

    return pid;
}


void handle_simple_cmd(struct execcmd *ecmd) {
    // void handle_simple_cmd(struct execcmd *ecmd) {
    //     /* Task 2: Implement the code below to execute simple commands. */
    //     fprintf(stderr, "exec not implemented\n");
    //     /* END OF TASK 2 */
    // }
    if (ecmd->argv[0] == NULL)
        exit(0);

    execvp(ecmd->argv[0], ecmd->argv);

    perror("execvp failed");
    exit(1);
}


void handle_redirection(struct redircmd *rcmd) {
    // void handle_redirection(struct redircmd *rcmd) {
    //     /* Task 3: Implement the code below to handle input/output redirection. */
    //     fprintf(stderr, "redir not implemented\n");
    //     /* END OF TASK 3 */
    // }
    int fd = open(rcmd->file, rcmd->mode, 0644);
    if (fd < 0) {
        perror("open failed");
        exit(1);
    }

    if (dup2(fd, rcmd->fd) < 0) {
        perror("dup2 failed");
        exit(1);
    }

    close(fd);

    runcmd(rcmd->cmd);
}


void handle_pipe(struct pipecmd *pcmd, int *p, int r) {
    // void handle_pipe(struct pipecmd *pcmd, int *p, int r) {
    //     /* Task 4: Implement the code below to handle pipes. */
    //     fprintf(stderr, "pipe not implemented\n");
    //     /* END OF TASK 4 */
    // }
    if (pipe(p) < 0) {
        perror("pipe failed");
        exit(1);
    }

    if (fork1() == 0) {
        close(1);
        dup(p[1]);
        close(p[0]);
        close(p[1]);

        runcmd(pcmd->left);
    }

    if (fork1() == 0) {
        close(0);
        dup(p[0]);
        close(p[1]);
        close(p[0]);

        runcmd(pcmd->right);
    }

    close(p[0]);
    close(p[1]);

    wait(0);
    wait(0);
}

int getcmd(char *buf, int nbuf) {
    if (isatty(fileno(stdin)))
        fprintf(stdout, "$ ");
    memset(buf, 0, nbuf);
    fgets(buf, nbuf, stdin);
    if (buf[0] == 0)  // EOF
        return -1;
    return 0;
}

int main(void) {
    static char buf[100];
    int r;

    // Read and execute commands.
    while (getcmd(buf, sizeof(buf)) >= 0) {
        /* Task 5: Explain the purpose of the if statement below and correct the error message.
        Why is the current error message incorrect? Justify the new message. */
        /* Answer:
            Esse if trata o comando "cd" diretamente no processo principal do shell,
            sem criar um processo filho. Isso é necessário porque o comando "cd"
            altera o diretório atual do processo. Se fosse executado em um processo
            filho (usando fork), a mudança não afetaria o shell principal.

            A mensagem de erro original "process does not exist" está incorreta,
            pois o erro não tem relação com processos, mas sim com diretórios.
            A mensagem correta deve indicar que o diretório não existe.
         */
        if (buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' ') {
            buf[strlen(buf) - 1] = 0;
            if (chdir(buf + 3) < 0)
                fprintf(stderr, "cd: no such file or directory\n");
                // fprintf(stderr, "process does not exist\n");
            continue;
        }
        /* END OF TASK 5 */

        if (fork1() == 0)
            runcmd(parsecmd(buf));
        wait(&r);
    }
    exit(0);
}

/****************************************************************
 * Helper functions for creating command structures
 ***************************************************************/

struct cmd *
execcmd(void) {
    struct execcmd *cmd;

    cmd = malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = ' ';
    return (struct cmd *)cmd;
}

struct cmd *
redircmd(struct cmd *subcmd, char *file, int type) {
    struct redircmd *cmd;

    cmd = malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = type;
    cmd->cmd = subcmd;
    cmd->file = file;
    cmd->mode = (type == '<') ? O_RDONLY : O_WRONLY | O_CREAT | O_TRUNC;
    cmd->fd = (type == '<') ? 0 : 1;
    return (struct cmd *)cmd;
}

struct cmd *
pipecmd(struct cmd *left, struct cmd *right) {
    struct pipecmd *cmd;

    cmd = malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = '|';
    cmd->left = left;
    cmd->right = right;
    return (struct cmd *)cmd;
}

/****************************************************************
 * Command Line Processing
 ***************************************************************/

char whitespace[] = " \t\r\n\v";
char symbols[] = "<|>";

int gettoken(char **ps, char *es, char **q, char **eq) {
    char *s;
    int ret;

    s = *ps;
    while (s < es && strchr(whitespace, *s))
        s++;
    if (q)
        *q = s;
    ret = *s;
    switch (*s) {
        case 0:
            break;
        case '|':
        case '<':
            s++;
            break;
        case '>':
            s++;
            break;
        default:
            ret = 'a';
            while (s < es && !strchr(whitespace, *s) && !strchr(symbols, *s))
                s++;
            break;
    }
    if (eq)
        *eq = s;

    while (s < es && strchr(whitespace, *s))
        s++;
    *ps = s;
    return ret;
}

int peek(char **ps, char *es, char *toks) {
    char *s = *ps;
    while (s < es && strchr(whitespace, *s)) s++;
    *ps = s;
    return *s && strchr(toks, *s);
}

struct cmd *parseline(char **, char *);
struct cmd *parsepipe(char **, char *);
struct cmd *parseexec(char **, char *);

/* Copy characters from the input buffer, starting from s to es.
 * Place a null terminator at the end to create a valid string. */
char *mkcopy(char *s, char *es) {
    int n = es - s;
    char *c = malloc(n + 1);
    assert(c);
    strncpy(c, s, n);
    c[n] = 0;
    return c;
}

struct cmd *
parsecmd(char *s) {
    char *es;
    struct cmd *cmd;

    es = s + strlen(s);
    cmd = parseline(&s, es);
    peek(&s, es, "");
    if (s != es) {
        fprintf(stderr, "leftovers: %s\n", s);
        exit(-1);
    }
    return cmd;
}

struct cmd *
parseline(char **ps, char *es) {
    struct cmd *cmd;
    cmd = parsepipe(ps, es);
    return cmd;
}

struct cmd *
parsepipe(char **ps, char *es) {
    struct cmd *cmd;

    cmd = parseexec(ps, es);
    if (peek(ps, es, "|")) {
        gettoken(ps, es, 0, 0);
        cmd = pipecmd(cmd, parsepipe(ps, es));
    }
    return cmd;
}

struct cmd *
parseredirs(struct cmd *cmd, char **ps, char *es) {
    int tok;
    char *q, *eq;

    while (peek(ps, es, "<>")) {
        tok = gettoken(ps, es, 0, 0);
        if (gettoken(ps, es, &q, &eq) != 'a') {
            fprintf(stderr, "missing file for redirection\n");
            exit(-1);
        }
        switch (tok) {
            case '<':
                cmd = redircmd(cmd, mkcopy(q, eq), '<');
                break;
            case '>':
                cmd = redircmd(cmd, mkcopy(q, eq), '>');
                break;
        }
    }
    return cmd;
}

struct cmd *
parseexec(char **ps, char *es) {
    char *q, *eq;
    int tok, argc;
    struct execcmd *cmd;
    struct cmd *ret;

    ret = execcmd();
    cmd = (struct execcmd *)ret;

    argc = 0;
    ret = parseredirs(ret, ps, es);
    while (!peek(ps, es, "|")) {
        if ((tok = gettoken(ps, es, &q, &eq)) == 0)
            break;
        if (tok != 'a') {
            fprintf(stderr, "syntax error\n");
            exit(-1);
        }
        cmd->argv[argc] = mkcopy(q, eq);
        argc++;
        if (argc >= MAXARGS) {
            fprintf(stderr, "too many args\n");
            exit(-1);
        }
        ret = parseredirs(ret, ps, es);
    }
    cmd->argv[argc] = 0;
    return ret;
}