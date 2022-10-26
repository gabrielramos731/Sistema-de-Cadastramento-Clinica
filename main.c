#include<stdio.h>
#include<stdlib.h>

typedef struct{
	char usuario[20];
	char senha[10];
} login;

typedef struct{
  char crm[6];
  char nome[50];
  char especialidade[20];
  char dataDeNascimento[10];
  char telefone[11];
  float valorHoraTrabalho;
} medico;

void verificarLogin(FILE *ponteiroLogin, int *contadorTentativas,  int *acesso);
void cadastrarNovoUsuario(FILE *ponteiroLogin);
void menuMedico(FILE *ponteiroMedicos);
void cadastroNovoMedico(FILE *ponteiroMedicos);

int main(){
	FILE *ponteiroLogin;
	FILE *ponteiroMedicos;
	int opcao, contadorTentativas = 0, acesso = 0;

  //login e cadastro
	do{
		printf("\n***** MENU *****\n");
		printf("(1) Efetuar login\n(2) Cadastrar usuario\n(0) Encerrar programa\n");
		scanf("%d", &opcao);

		switch(opcao){
			case 1:
				ponteiroLogin = fopen("dados/login.bin", "rb");
				verificarLogin(ponteiroLogin, &contadorTentativas, &acesso);  //chamada da função login
				if(contadorTentativas > 2){
					printf("\nLimite de tentavivas excedidas\nEncerrando o processo\n");
					return 1;
				}
				fclose(ponteiroLogin);
				break;

			case 2:
				ponteiroLogin = fopen("dados/login.bin", "ab");
				cadastrarNovoUsuario(ponteiroLogin);  //chamada da função cadastrar
				fclose(ponteiroLogin);
				break;

			case 0:
				printf("\nPrograma encerrado\n");
				return 1;

			default:
				printf("\nOpcao invalida\n\n");
		}
	} while (acesso != 1);

  //medico, usuario e paciente
	do{

    printf("Insira a opcao desejada: ");
    printf("\n(1) Medico");
    printf("\n(2) Paciente");
    printf("\n(3) Consulta");
    printf("\n(0) Encerrar programa\n");

    scanf("%d", &opcao);
    switch(opcao){
	  	case 1:
		  	printf("Entrando na seccao medicos...\n");
				menuMedico(ponteiroMedicos);
		  	break;
			case 2:
		    printf("Entrando na seccao paciente...\n");
				break;
			case 3:
				printf("Entrando na seccao consultas...\n");
   	 		break;
    	case 0:
      	printf("\nFechando programa...\n");
      	break;
	  	default:
	  		printf("\nOpcao invalida\n\n");
		}
	}while (opcao != 0);
}

void verificarLogin(FILE *ponteiroLogin, int *contadorTentativas,  int *acesso){
	char loginIndividuo[20];
	char senhaIndividuo[10];
	login inidividuo;

	do{
		fflush(stdin);
		printf("\nLogin: ");
		scanf("%[^\n]s", loginIndividuo);
		fflush(stdin);
		printf("Senha: ");
		scanf("%[^\n]s", senhaIndividuo);

		fseek(ponteiroLogin, 0*sizeof(login), SEEK_SET);  //resetar a posição da busca para o início
		while(fread(&inidividuo, sizeof(login), 1, ponteiroLogin)){
			if(strcmp(inidividuo.usuario, loginIndividuo) == 0 && strcmp(inidividuo.senha, senhaIndividuo) == 0){  //compara o login
				printf("\nLogin efetuado com sucesso");
				*acesso = 1;  //chave para prosseguir
				return 1;
			}
		}
		printf("\nLogin ou senha invalidos\n");
		*contadorTentativas += 1;
	} while(*contadorTentativas < 3);
}

void cadastrarNovoUsuario(FILE *ponteiroLogin){
	login individuo;

	fflush(stdin);
	printf("\nNovo nome de usuario: ");
	scanf("%[^\n]s", individuo.usuario);
	fflush(stdin);
	printf("Nova senha: ");
	scanf("%[^\n]s", individuo.senha);
	fwrite(&individuo, sizeof(login), 1, ponteiroLogin);  //cadastra sempre no final
}

void menuMedico(FILE *ponteiroMedicos){
	int opcao;

	do{
		printf("\n(1) Inserir novo medico");
		printf("\n(2) Buscar medico especifico por nome");
		printf("\n(3) Listar medicos por especialidade");
		printf("\n(4) Alterar dados de um medico especifico");
		printf("\n(0) Retornar\n");
		scanf("%d", &opcao);

		switch(opcao){
			case 1:
				ponteiroMedicos = fopen("dados/medicos.bin", "ab");
				cadastroNovoMedico(ponteiroMedicos);
				fclose(ponteiroMedicos);
				break;
			case 2:
				buscarMedico(ponteiroMedicos);
				break;
			case 3:
				// listar medicos por especialidade
				break;
			case 4:
				// alterar dados de um medico especifico
				break;
		}
	} while(opcao != 0);
}

void cadastroNovoMedico(FILE *ponteiroMedicos){

	medico medicoIndividuo;

	fflush(stdin);
	printf("\nCRM: ");
	scanf("%[^\n]s", medicoIndividuo.crm);
	fflush(stdin);
	printf("Nome: ");
	scanf("%[^\n]s", medicoIndividuo.nome);
	fflush(stdin);
	printf("Especialidade: ");
	scanf("%[^\n]s", medicoIndividuo.especialidade);
	fflush(stdin);
	printf("Data de nascimento: ");
	scanf("%[^\n]s", medicoIndividuo.especialidade);
	fflush(stdin);
	printf("Valor por hora trabalhada: ");
	scanf("%[^\n]f", &medicoIndividuo.valorHoraTrabalho);
	fflush(stdin);
	printf("Telefone: ");
	scanf("%[^\n]s", medicoIndividuo.telefone);
	fwrite(&medicoIndividuo, sizeof(medico), 1, ponteiroMedicos);
}

void buscarMedico(){
	
}