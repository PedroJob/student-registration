struct lista_alunos;

typedef struct Disciplina
{
    int codigo, creditos;
    char *nome, *professor;
    struct lista_alunos *alunos;
    struct Disciplina *prox;
} disciplina;

typedef struct
{
    disciplina *head, *last;
    int size;
} lista_disciplinas;

typedef struct Aluno
{
    int codigo;
    long int cpf;
    char *nome;
    lista_disciplinas *disciplinas;
    struct Aluno *prox;
} aluno;

typedef struct lista_alunos
{
    aluno *head, *last;
    int size;
} lista_alunos;

typedef struct Periodo
{
    float periodo;
    lista_alunos *alunos;
    lista_disciplinas *disciplinas;
    struct Periodo *prox;
} periodo;

typedef struct
{
    periodo *head, *last;
    int size;
} lista_periodos;

aluno *novo_aluno(int codigo, char *nome, long int cpf);

disciplina *nova_disciplina(int codigo, char *nome, char *professor, int creditos);

periodo *novo_periodo(float valor);

int add_aluno(lista_alunos *lista, aluno *novo);

int add_disciplina(lista_disciplinas *lista, disciplina *nova);

int add_periodo(lista_periodos *lista, periodo *novo);

void menu();

aluno *menu_aluno();

disciplina *menu_disciplina();

void print_disciplinas(lista_disciplinas *lista);

void print_alunos(lista_alunos *lista);

void print_periodos(lista_periodos *lista);

void flush_in();

aluno *buscar_aluno(lista_alunos *lista, int codigo);

disciplina *buscar_disciplina(lista_disciplinas *lista, int codigo);

periodo *buscar_periodo(lista_periodos *lista, float valor);

void pop_aluno(lista_alunos *lista, int codigo);

void pop_disciplina(lista_disciplinas *lista, int codigo);

void pop_periodo(lista_periodos *lista, float valor);

void salvar_txt(lista_periodos *lista);

int recuperar_txt(lista_periodos *lista);

void menu()
{
    printf("\n  O que deseja fazer agora?\n");
    printf("\n1 - Adicionar novo periodo");
    printf("\n2 - Adicionar novo aluno");
    printf("\n3 - Listar disciplinas de um aluno");
    printf("\n4 - Listar disciplinas do periodo");
    printf("\n5 - Listar alunos de uma disciplina");
    printf("\n6 - Listar alunos do periodo");
    printf("\n7 - Remover aluno de uma disciplina");
    printf("\n8 - Remover aluno do periodo");
    printf("\n9 - Remover disciplina do periodo");
    printf("\n10 - Acessar periodo anterior");
    printf("\n11 - Remover periodo");
    printf("\n12 - Sair e salvar\n");
    printf("\ndigite a opcao desejada: ");
    return;
}

void salvar_txt(lista_periodos *lista)
{
    FILE *fptr;

    fptr = fopen("data.txt", "w");

    periodo *periodo_atual = lista->head;

    if (fptr == NULL)
    {
        printf("\n*nao foi possivel abrir o arquivo*\n");
        return;
    }
    else
    {
        while (periodo_atual != NULL)
        {   
            fprintf(fptr, "%s", "PERIODO\n");
            fprintf(fptr, "%.1f\n", periodo_atual->periodo);

            aluno *aluno_atual = periodo_atual->alunos->head;
            while (aluno_atual != NULL)
            {   
                fprintf(fptr, "%s", "ALUNO\n");
                fprintf(fptr, "%s %d %ld\n", (aluno_atual->nome), (aluno_atual->codigo), (aluno_atual->cpf));
                fprintf(fptr, "%s", "DISCIPLINAS\n");
                disciplina *disciplina_atual = aluno_atual->disciplinas->head;
                while (disciplina_atual != NULL)
                {
                    fprintf(fptr, "%s %s %d %d\n", (disciplina_atual->nome), (disciplina_atual->professor), (disciplina_atual->codigo), (disciplina_atual->creditos));

                    disciplina_atual = disciplina_atual->prox;
                }
                aluno_atual = aluno_atual->prox;
            }
            periodo_atual = periodo_atual->prox;
        }
    }

    fclose(fptr);
    return;
}

int recuperar_txt(lista_periodos *lista)
{
    FILE *fptr;

    fptr = fopen("data.txt", "r");

    if (fptr == NULL)
    {
        printf("\n*nao foi possivel abrir o arquivo*\n");
        return 0;
    }

    int codigo_aluno = 0, codigo_disciplina = 0, creditos = 0;
    long int cpf = 0;
    float valor = 0.0;
    char buffer[500], nome_disci[256], nome_aluno[256], professor[256];

    periodo *periodo_atual = lista->last; 
    aluno *aluno_atual = NULL; 

    fscanf(fptr, "%s", buffer);
    do
    { 
        if(strcmp(buffer, "PERIODO") == 0)
        {
            fscanf(fptr, "%f\n", &valor);
            periodo *novop = novo_periodo(valor);
            add_periodo(lista, novop); 
            periodo_atual = novop;  
            fscanf(fptr, "%s", buffer);
        }

        if(strcmp(buffer, "ALUNO") == 0)
        {   
            fscanf(fptr, "%s %d %ld", nome_aluno, &codigo_aluno, &cpf);
            aluno *novo = novo_aluno(codigo_aluno, nome_aluno, cpf);
            add_aluno(periodo_atual->alunos, novo);
            aluno_atual = novo; 
            fscanf(fptr, "%s", buffer);
        }

        if(strcmp(buffer, "DISCIPLINAS") == 0){
            do
            {
                //fscanf(fptr, "%s %s %d %d\n", nome_disci, professor, &codigo_disciplina, &creditos);
                fscanf(fptr, "%s", nome_disci);
                if(strcmp(nome_disci, "ALUNO") == 0 || strcmp(nome_disci, "PERIODO") == 0){
                    getc(fptr); 
                    strcpy(buffer, nome_disci);
                    break; 
                }
                fscanf(fptr, "%s %d %d\n", professor, &codigo_disciplina, &creditos);
                disciplina *nova = nova_disciplina(codigo_disciplina, nome_disci, professor, creditos); 
                add_aluno(nova->alunos, novo_aluno(aluno_atual->codigo, aluno_atual->nome, aluno_atual->cpf)); 
                add_disciplina(periodo_atual->disciplinas, nova);
                add_disciplina(aluno_atual->disciplinas,  nova_disciplina(codigo_disciplina, nome_disci, professor, creditos));
            }while(!feof(fptr));
        }
    } while (!feof(fptr));
    fclose(fptr);
    return 1;
    
}

void flush_in()
{
    int ch;
    do
    {
        ch = fgetc(stdin);
    } while (ch != EOF && ch != '\n');
}

void pop_aluno(lista_alunos *lista, int codigo)
{
    aluno *finder = lista->head, *prev = NULL;
    // lista vazia
    if (finder == NULL)
    {
        printf("\n*a lista esta vazia*\n");
        return;
    }
    // primeiro da lista
    if ((finder->codigo = codigo))
    {
        lista->head = finder->prox;
        free(finder);
        return;
    }
    // buscar elemento
    while (finder != NULL && finder->codigo != codigo)
    {
        prev = finder;
        finder = finder->prox;
    }

    if (finder == NULL)
    {
        printf("\n*o elemento nao foi encontrado*\n");
        return;
    }

    prev->prox = finder->prox;

    return;
}

void pop_disciplina(lista_disciplinas *lista, int codigo)
{
    disciplina *finder = lista->head, *prev = NULL;
    // lista vazia
    if (finder == NULL)
    {
        printf("\n*a lista esta vazia*\n");
        free(finder);
        return;
    }
    // primeiro da lista
    if ((finder->codigo = codigo))
    {
        lista->head = finder->prox;
        free(finder);
        return;
    }
    // buscar elemento
    while (finder != NULL && finder->codigo != codigo)
    {
        prev = finder;
        finder = finder->prox;
    }

    if (finder == NULL)
    {
        printf("\n*o elemento nao foi encontrado*\n");
        return;
    }

    prev->prox = finder->prox;

    free(finder);
    return;
}

void pop_periodo(lista_periodos *lista, float valor)
{
    periodo *finder = lista->head, *prev = NULL;
    // lista vazia
    if (finder == NULL)
    {
        printf("\n*a lista esta vazia*\n");
        free(finder);
        return;
    }
    // primeiro da lista
    if ((finder->periodo = valor))
    {
        lista->head = finder->prox;
        free(finder);
        return;
    }
    // buscar elemento
    while (finder != NULL && finder->periodo != valor)
    {
        prev = finder;
        finder = finder->prox;
    }

    if (finder == NULL)
    {
        printf("\n*o elemento nao foi encontrado*\n");
        return;
    }

    prev->prox = finder->prox;

    free(finder);
    return;
}

void print_disciplinas(lista_disciplinas *lista)
{
    disciplina *finder = lista->head;

    while (finder != NULL)
    {
        printf("\n\n%d - %s", (finder->codigo), (finder->nome));
        finder = finder->prox;
    }

    free(finder);

    return;
}

void print_alunos(lista_alunos *lista)
{
    aluno *finder = lista->head;

    while (finder != NULL)
    {
        printf("\n\n%d - %s", (finder->codigo), (finder->nome));
        finder = finder->prox;
    }

    free(finder);

    return;
}

void print_periodos(lista_periodos *lista)
{
    periodo *finder = lista->head;

    while (finder != NULL)
    {
        printf("\n%.1f", (finder->periodo));
        finder = finder->prox;
    }

    printf("\n");

    return;
}

aluno *buscar_aluno(lista_alunos *lista, int codigo)
{
    aluno *finder = lista->head;

    while (finder != NULL)
    {
        if (finder->codigo == codigo) // achou aluno
        {
            return finder;
        }

        finder = finder->prox;
    }

    printf("\n*o aluno nao foi encontrado*\n");
    free(finder);
    return NULL;
}

disciplina *buscar_disciplina(lista_disciplinas *lista, int codigo)
{
    disciplina *finder = lista->head;

    while (finder != NULL)
    {
        if ((finder->codigo == codigo)) // achou aluno
        {
            return finder;
        }

        finder = finder->prox;
    }

    printf("\n*a disciplina nao foi encontrada*\n");

    free(finder);
    return NULL;
}

periodo *buscar_periodo(lista_periodos *lista, float valor)
{
    periodo *finder = lista->head;

    while (finder != NULL)
    {
        if ((finder->periodo == valor)) // achou perioo
        {
            return finder;
        }

        finder = finder->prox;
    }

    printf("\n*o periodo nao foi encontrado*\n");

    free(finder);
    return NULL;
}

aluno *menu_aluno()
{
    long int cpf = 0;
    int codigo = 0;
    char buffer[256], *nome;

    printf("\nDigite o CPF do aluno: ");
    scanf("%11li", &cpf);
    flush_in();
    printf("\nDigite o codigo do aluno: ");
    scanf("%5d", &codigo);
    printf("\nDigite o nome do aluno: ");
    flush_in();
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    nome = malloc(sizeof(strlen(buffer) + 1));
    strcpy(nome, buffer);
    system("clear");
    printf("\naluno registrado!\n");
    return novo_aluno(codigo, nome, cpf);
}

disciplina *menu_disciplina()

{
    int code = 0;
    char buffer_disc[100], *nome_disc;
    char buffer_prof[100], *professor;
    int creditos = 0;

    printf("\nDigite o codigo da disciplina: ");
    scanf("%5d", &code);

    printf("\nDigite o nome da disciplina: ");
    flush_in();
    fgets(buffer_disc, sizeof(buffer_disc), stdin);
    buffer_disc[strcspn(buffer_disc, "\n")] = 0;
    nome_disc = malloc(sizeof(strlen(buffer_disc) + 1));
    strcpy(nome_disc, buffer_disc);

    printf("\nDigite o nome do professor: ");
    fgets(buffer_prof, sizeof(buffer_prof), stdin);
    buffer_prof[strcspn(buffer_prof, "\n")] = 0;
    professor = malloc(sizeof(strlen(buffer_prof) + 1));
    strcpy(professor, buffer_prof);

    printf("\nDigite o numero de creditos da disciplina: ");
    scanf("%d", &creditos);

    system("clear");
    printf("\ndisciplina registrada!\n");

    return nova_disciplina(code, nome_disc, professor, creditos);
}

aluno *novo_aluno(int codigo, char *nome, long int cpf)
{
    aluno *novo_aluno = (aluno *)malloc(sizeof(aluno));

    lista_disciplinas *disciplinas = (lista_disciplinas *)malloc(sizeof(lista_disciplinas));
    disciplinas->head = NULL;
    disciplinas->last = NULL;
    disciplinas->size = 0;

    novo_aluno->disciplinas = disciplinas;
    novo_aluno->nome = malloc(sizeof(nome) + 1);
    strcpy(novo_aluno->nome, nome);
    novo_aluno->codigo = codigo;
    novo_aluno->cpf = cpf;
    novo_aluno->prox = NULL;

    return novo_aluno;
}

disciplina *nova_disciplina(int codigo, char *nome, char *professor, int creditos)
{
    disciplina *nova_disciplina = (disciplina *)malloc(sizeof(disciplina));

    lista_alunos *alunos = (lista_alunos *)malloc(sizeof(lista_alunos));
    alunos->head = NULL;
    alunos->last = NULL;
    alunos->size = 0;
    nova_disciplina->alunos = alunos;
    nova_disciplina->nome = malloc(sizeof(nome) + 1);
    strcpy(nova_disciplina->nome, nome);
    nova_disciplina->professor = malloc(sizeof(professor) + 1);
    strcpy(nova_disciplina->professor, professor);
    nova_disciplina->creditos = creditos;
    nova_disciplina->codigo = codigo;
    nova_disciplina->prox = NULL;

    return nova_disciplina;
}

periodo *novo_periodo(float valor)
{
    periodo *novo_periodo = (periodo *)malloc(sizeof(periodo));

    lista_disciplinas *disciplinas = (lista_disciplinas *)malloc(sizeof(lista_disciplinas));
    disciplinas->head = NULL;
    disciplinas->last = NULL;
    disciplinas->size = 0;

    lista_alunos *alunos = (lista_alunos *)malloc(sizeof(lista_alunos));
    alunos->head = NULL;
    alunos->last = NULL;
    alunos->size = 0;

    novo_periodo->periodo = valor;
    novo_periodo->prox = NULL;
    novo_periodo->alunos = alunos;
    novo_periodo->disciplinas = disciplinas;

    return novo_periodo;
}

int add_aluno(lista_alunos *lista, aluno *novo)
{
    // lista vazia
    if (lista->head == NULL && lista->head == NULL)
    {
        lista->head = novo;
        lista->last = novo;
        lista->size++;
        return 1;
    }

    lista->last->prox = novo;
    lista->last = novo;
    lista->size++;

    return 1;
}

int add_disciplina(lista_disciplinas *lista, disciplina *nova)
{

    if (lista->head == NULL && lista->head == NULL)
    {
        lista->head = nova;
        lista->last = nova;
        lista->size++;
        return 1;
    }

    lista->last->prox = nova;
    lista->last = nova;
    lista->size++;

    return 1;
}

int add_periodo(lista_periodos *lista, periodo *novo)
{
    if (lista->head == NULL && lista->head == NULL)
    {
        lista->head = novo;
        lista->last = novo;
        lista->size++;
        return 1;
    }

    lista->last->prox = novo;
    lista->last = novo;
    lista->size++;

    return 1;
}
