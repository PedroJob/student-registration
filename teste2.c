#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "functions.h"

int main()
{
    // starting pointers and lists
    lista_periodos *listaperiodos = (lista_periodos *)malloc(sizeof(lista_periodos));
    listaperiodos->head = NULL;
    listaperiodos->last = NULL;
    listaperiodos->size = 0;

    printf("\n==================== SISTEMA DE CADASTRAMENTO DE ALUNOS ====================\n");
    int aux = 0, opt = 1, codigo = 0;
    periodo *novoperiodo = NULL;

    printf("Deseja recuperar os registros anteriores? (1-sim)\n\n");
    printf("Digite a opcao: ");
    scanf("%d", &aux);

    if (aux == 1 && recuperar_txt(listaperiodos))
    {
        opt = 0;
        novoperiodo = listaperiodos->last;
    }

    do
    {
        switch (opt)
        {
        case 1: // adicionar periodo
            float key = 0.0;
            printf("\nDigite o novo periodo que deseja cadastrar: ");
            scanf("%f", &key);
            novoperiodo = novo_periodo(key);
            add_periodo(listaperiodos, novoperiodo);
            system("clear");
            break;

        case 2: // adicionar aluno ao período
            printf("\n*periodo atual: %.1f*\n", novoperiodo->periodo);

            aluno *novoaluno = menu_aluno();
            aluno *novoaluno_copia = novo_aluno(novoaluno->codigo, novoaluno->nome, novoaluno->cpf);
            add_aluno(novoperiodo->alunos, novoaluno);

            printf("\nInsira as disciplinas que esse aluno participa: \n");
            disciplina *nova = menu_disciplina();
            disciplina *nova_copia = nova_disciplina(nova->codigo, nova->nome, nova->professor, nova->creditos);

            // obriga a adicionar uma disciplina (opcional)

            if (buscar_disciplina(novoperiodo->disciplinas, nova->codigo) == NULL)
            {
                system("clear");
                add_disciplina(novoperiodo->disciplinas, nova);
            }
            add_aluno(nova->alunos, novoaluno_copia);
            add_disciplina(novoaluno->disciplinas, nova_copia);

            int exit = 0;

            do
            {
                printf("\ndeseja adicionar mais uma? (1 - sim, 2 - nao)\n");
                scanf("%d", &exit);

                if (exit == 1)
                {
                    system("clear");
                    nova = menu_disciplina();
                    nova_copia = nova_disciplina(nova->codigo, nova->nome, nova->professor, nova->creditos);
                    if (buscar_disciplina(novoperiodo->disciplinas, nova->codigo) == NULL)
                    {
                        system("clear");
                        add_disciplina(novoperiodo->disciplinas, nova);
                    }
                    add_disciplina(novoaluno->disciplinas, nova_copia);
                    add_aluno(nova->alunos, novoaluno);
                }
                else if (exit != 2 && exit != 1)
                {
                    printf("\n*digite uma opcao valida*\n");
                }
            } while (exit != 2);

            break;

        case 3: // consultar disciplinas de um aluno
            printf("\n*periodo atual: %.1f*\n", novoperiodo->periodo);

            printf("\nqual aluno deseja consultar as disciplinas?\n");
            print_alunos(novoperiodo->alunos);
            printf("\ndigite o codigo do aluno desejado: ");
            int code = 0;
            scanf("%d", &code);

            aluno *find = buscar_aluno(novoperiodo->alunos, code);
            if (find == NULL)
            {
                break;
            }
            system("clear");
            printf("\nAs disciplinas cursadas por esse aluno sao: \n");
            print_disciplinas(find->disciplinas);
            break;
        case 4:
            printf("\ndisciplinas no periodo: %.1f", novoperiodo->periodo);
            print_disciplinas(novoperiodo->disciplinas);
            break;
        case 5: // consultar alunos de uma disciplina
            printf("\n*periodo atual: %1f*\n", novoperiodo->periodo);
            printf("\nqual disciplina deseja consultar?\n");
            print_disciplinas(novoperiodo->disciplinas);
            printf("\ndigite o codigo da dsiciplina desejada: ");
            scanf("%d", &codigo);
            if (buscar_disciplina(novoperiodo->disciplinas, codigo) == NULL)
            {
                break;
            }
            system("clear");
            printf("\nos alunos que cursam essa disciplina sao: \n");
            print_alunos((buscar_disciplina(novoperiodo->disciplinas, codigo))->alunos);
            break;

        case 6: // consultar alunos do periodo
            print_alunos(novoperiodo->alunos);
            break;

        case 7: // remover aluno de uma disciplina
            printf("\n*periodo atual: %.1f*\n", novoperiodo->periodo);
            printf("\ndigite o codigo do aluno que deseja remover\n");
            print_alunos(novoperiodo->alunos);
            scanf("%d", &codigo);
            aluno *alvo = buscar_aluno(novoperiodo->alunos, codigo);
            if (alvo == NULL)
            {
                break;
            }
            printf("\nqual disciplina deseja remover? \n");
            print_disciplinas((buscar_aluno(novoperiodo->alunos, codigo))->disciplinas);
            printf("\ndigite o codigo da disciplina que deseja remover\n");
            scanf("%d", &codigo);
            pop_disciplina(alvo->disciplinas, codigo);

            break;

        case 8: // remover aluno do periodo
            printf("\n*periodo atual: %.1f*\n", novoperiodo->periodo);
            print_alunos(novoperiodo->alunos);
            printf("\ndigite o codigo do aluno que deseja remover\n");
            scanf("%d", &codigo);
            aluno *al = buscar_aluno(novoperiodo->alunos, codigo);

            if (al == NULL)
            {
                break;
            }

            disciplina *finder = al->disciplinas->head;

            while (finder != NULL)
            {
                pop_aluno(finder->alunos, codigo);
                finder = finder->prox;
                system("clear");
            }

            pop_aluno(novoperiodo->alunos, codigo);
            break;

        case 9: // remover disciplina do período
            printf("\n*periodo atual: %.1f*\n", novoperiodo->periodo);
            print_disciplinas(novoperiodo->disciplinas);
            printf("\ndigite o codigo da disciplina que deseja remover\n");
            scanf("%d", &codigo);

            disciplina *disc = buscar_disciplina(novoperiodo->disciplinas, codigo);

            if (disc == NULL)
            {
                break;
            }
            aluno *ptr = disc->alunos->head;

            while (ptr != NULL)
            {
                pop_disciplina(ptr->disciplinas, codigo);
                ptr = ptr->prox;
                system("clear");
            }

            pop_disciplina(novoperiodo->disciplinas, codigo);

            break;

        case 10: // acessar periodo anterior
            print_periodos(listaperiodos);
            printf("\nqual periodo deseja acessar?\n");
            scanf("%f", &key);
            if (buscar_periodo(listaperiodos, key) == NULL)
            {
                break;
            }
            novoperiodo = buscar_periodo(listaperiodos, key);
            break;
        case 11: // remover periodo;
            print_periodos(listaperiodos);
            printf("\nqual periodo deseja excluir?\n");
            scanf("%f", &key);
            pop_periodo(listaperiodos, key);
            break;
        default:
            break;
        }

        printf("\n*periodo atual: %.1f*\n", novoperiodo->periodo);
        menu();
        scanf("%d", &opt);
        system("clear");
    } while (opt != 12);

    salvar_txt(listaperiodos);
    return 0;
}
