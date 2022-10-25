#include<stdio.h>
#include<stdlib.h>

typedef struct{
	char usuario[20];
	char senha[10];
} login;

void verificarLogin(FILE *ponteiroLogin, int *contadorTentativas,  int *acesso);
void cadastrarNovoUsuario(FILE *ponteiroLogin);

int main(){
	FILE *ponteiroLogin;
	int opcao, contadorTentativas = 0, acesso = 0;

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

	printf("login aceito");  //as outras opções vem agora
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
 
