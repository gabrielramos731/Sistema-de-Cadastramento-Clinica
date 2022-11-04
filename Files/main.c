#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"struct.h"
#include"funcoesCadastro.h"

int main(){

	FILE *ponteiroLogin = 0;
	FILE *ponteiroMedicos = 0;
	FILE *ponteiroPacientes = 0;
  FILE *ponteiroConsultas = 0;
	int opcao, contadorTentativas = 0, acesso = 0;

  	//login e cadastro
	do{
		//menu para primeiro acesso e criação de login
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
				ponteiroLogin = fopen("dados/login.bin", "a+b");
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

  	//menu para acesso de medico, usuario e paciente
	do{

    printf("\nInsira a opcao desejada: ");
    printf("\n(1) Medico");
    printf("\n(2) Paciente");
    printf("\n(3) Consulta");
    printf("\n(0) Encerrar programa\n");
    scanf("%d", &opcao);

	//faz uma função de menu para cada opção de secção
    switch(opcao){
	  	case 1:
			//menu de medicos
		      printf("Entrando na seccao medicos...\n");
			  menuMedico(ponteiroMedicos);
		    break;
		  case 2:
			//menu de pacientes
		 	  printf("Entrando na seccao paciente...\n");
			  menuPaciente(ponteiroPacientes);
			  break;
		  case 3:
			//menu de consultas
			  printf("Entrando na seccao consultas...\n");
        	  menuConsulta(ponteiroConsultas,ponteiroMedicos,ponteiroPacientes);
   	 		break;
    	case 0:
      	  printf("\nFechando programa...\n");
      	break;
	  	default:
	  	  printf("\nOpcao invalida\n");
		}
	}while (opcao != 0);
};
