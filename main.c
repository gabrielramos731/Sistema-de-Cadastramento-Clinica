#include<stdio.h>

typedef struct{
	char usuario[20];
	char senha[10];
} login;

void verificarLogin(FILE *ponteiroLogin);
void cadastrarNovoUsuario(FILE *ponteiroLogin);

int main(){
	FILE *ponteiroLogin;
	int opcao;

	
	do{
		printf("***** MENU *****\n");
		printf("(1) Efetuar login\n(2) Cadastrar usuario\n(0) Encerrar programa\n");
		scanf("%d", &opcao);

		switch(opcao){
			case 1:
				ponteiroLogin = fopen("dados/login.bin", "rb");
				verificarLogin(ponteiroLogin);
				fclose(ponteiroLogin);
				break;
			case 2:
				ponteiroLogin = fopen("dados/login.bin", "ab");
				cadastrarNovoUsuario(ponteiroLogin);
				fclose(ponteiroLogin);
				break;
		}
	} while (opcao != 0);
}

void verificarLogin(FILE *ponteiroLogin){
	char loginIndividuo[20];
	char senhaIndividuo[10];

	fflush(stdin);
	printf("Login: ");
	scanf("%[^\n]s", loginIndividuo);
	fflush(stdin);
	printf("\nSenha: ");
	scanf("%[^\n]s", senhaIndividuo);

	
}

void cadastrarNovoUsuario(FILE *ponteiroLogin){
	login individuo;

	fflush(stdin);
	printf("Novo nome de usuario: ");
	scanf("%[^\n]s", individuo.usuario);
	fflush(stdin);
	printf("\nNova senha: ");
	scanf("%[^\n]s", individuo.senha);
	fwrite(&individuo, sizeof(login), 1, ponteiroLogin);
	printf("\n");
}
