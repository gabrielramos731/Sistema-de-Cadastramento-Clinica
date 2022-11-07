#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct {
	char usuario[20];
	char senha[10];
} login;

typedef struct {
  char crm[6];
  char nome[50];
  char especialidade[20];
  char dataDeNascimento[10];
  char telefone[11];
  float valorHoraTrabalho;
} medico;

typedef struct {
	char nome[50];
	char cpf[11];
	char dataDeNascimento[10];
	char telefone[11];
} paciente;

typedef struct {
  char crmMedico[6];
  char cpfPaciente[11];
  char data[10];
  char sintoma[100];  
  char encaminhamento[100];
} consulta;

void verificarLogin(FILE *ponteiroLogin, int *contadorTentativas,  int *acesso);
void cadastrarNovoUsuario(FILE *ponteiroLogin);
void menuMedico(FILE *ponteiroMedicos);
void cadastroNovoMedico(FILE *ponteiroMedicos);
void buscarMedico(FILE *ponteiroMedicos);
void listarPorEspecialidade(FILE *ponteiroMedicos);
void alterarDadosMedico(FILE *ponteiroMedicos);
void menuPaciente(FILE *ponteiroPacientes);
void cadastroNovoPaciente (FILE *ponteiroPacientes);
void buscarPaciente (FILE *ponteiroPacientes);
void alterarDadosPaciente(FILE *ponteiroPacientes);
int verificaDado(FILE *arquivoGenerico, char *dadoComparado, int medicoPaciente);
void menuConsulta(FILE *ponteiroConsultas, FILE *ponteiroMedicos, FILE *ponteiroPacientes);
void inserirNovaConsulta(FILE *ponteiroConsultas, FILE *ponteiroMedicos,FILE *ponteiroPacientes);
void listarConsultasMedico(FILE *ponteiroConsultas, FILE *ponteiroMedicos);
void listarConsultasPacientes(FILE *ponteiroConsultas, FILE *ponteiroPacientes);

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

void verificarLogin(FILE *ponteiroLogin, int *contadorTentativas,  int *acesso){

	char loginIndividuo[20];
	char senhaIndividuo[10];
	login individuo;

	do{
		fflush(stdin);
		printf("\nLogin: ");
		scanf("%[^\n]s", loginIndividuo);
		fflush(stdin);
		printf("Senha: ");
		scanf("%[^\n]s", senhaIndividuo);

		//resetar a posição da busca para o início
		fseek(ponteiroLogin, 0*sizeof(login), SEEK_SET);  
		while(fread(&individuo, sizeof(login), 1, ponteiroLogin)){
			//compara o login
			if(strcmp(individuo.usuario, loginIndividuo) == 0 && strcmp(individuo.senha, senhaIndividuo) == 0){
				system("cls");
				printf("\nLogin efetuado com sucesso\n");
				*acesso = 1;  //chave para prosseguir
				return;
			}
		}
		printf("\a");
		printf("\nLogin ou senha invalidos\n");
		*contadorTentativas += 1;
	} while(*contadorTentativas < 3);
};

//---------------------------------Inicio Cadastro--------------------------------------------

void cadastrarNovoUsuario(FILE *ponteiroLogin){

	login individuo;
	char usuarioTeste[20];

	fflush(stdin);
	printf("\nNovo nome de usuario: ");
	scanf("%[^\n]s", individuo.usuario);
	fflush(stdin);
	printf("Nova senha: ");
	scanf("%[^\n]s", individuo.senha);

	strcpy(usuarioTeste, individuo.usuario);
  fseek(ponteiroLogin, 0*sizeof(login), SEEK_SET);
	while(fread(&individuo, sizeof(login), 1, ponteiroLogin)){
		if(strcmp(individuo.usuario, usuarioTeste) == 0){  //compara o login
			printf("\nUsuario ja cadastrado!\n");
			return;
		}
	}
  
	fwrite(&individuo, sizeof(login), 1, ponteiroLogin);  //cadastra sempre no final
	system("cls");
	printf("\nUsuario cadastrado com sucesso!\n");
  
};

//----------------------------------Fim Cadastro---------------------------------------------

//-------------------------------Inicio Medicos----------------------------------------------

void menuMedico(FILE *ponteiroMedicos){

	int opcao;

	//Menu de acesso as funções de medico
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
				ponteiroMedicos = fopen("dados/medicos.bin", "rb");
				buscarMedico(ponteiroMedicos);
				fclose(ponteiroMedicos);
				break;
			case 3:
				ponteiroMedicos = fopen("dados/medicos.bin", "rb");
				listarPorEspecialidade(ponteiroMedicos);
				fclose(ponteiroMedicos);
				break;
			case 4:
				ponteiroMedicos = fopen("dados/medicos.bin", "r+b");
				alterarDadosMedico(ponteiroMedicos);
				fclose(ponteiroMedicos);
				break;
			case 0:
			printf("\nRetornando...\n");
			break;
			default:
				printf("\nFuncao invalida!\n");
		}
	} while(opcao != 0);
};

void cadastroNovoMedico(FILE *ponteiroMedicos){

	medico medicoIndividuo;
	char crmTeste[6];

	fflush(stdin);
	printf("\nNome: ");
	scanf("%[^\n]s", medicoIndividuo.nome);
	fflush(stdin);
	printf("CRM: ");
	scanf("%[^\n]s", medicoIndividuo.crm);
	fflush(stdin);

	//Chamada da função para verificar possivel conflito de crm igual
	strcpy(crmTeste, medicoIndividuo.crm);
	if(verificaDado(ponteiroMedicos, crmTeste, 1) == 1) {
		printf("\nMedico ja cadastrado\n");
		return;
	}

	printf("Especialidade: ");
	scanf("%[^\n]s", medicoIndividuo.especialidade);
	fflush(stdin);
	printf("Data de nascimento: ");
	scanf("%[^\n]s", medicoIndividuo.dataDeNascimento);
	fflush(stdin);
	printf("Telefone: ");
	scanf("%[^\n]s", medicoIndividuo.telefone);
	printf("Valor por hora trabalhada: ");
	scanf("%f%*c", &medicoIndividuo.valorHoraTrabalho);
	fflush(stdin);
	fwrite(&medicoIndividuo, sizeof(medico), 1, ponteiroMedicos);
};

void buscarMedico(FILE *ponteiroMedicos){

	medico medicoIndividuo;
	char nomeMedicoBuscado[50];
	int cont = 0;

	printf("\nNome: ");
	fflush(stdin);
	scanf("%[^\n]s", nomeMedicoBuscado);
	fseek(ponteiroMedicos, 0*sizeof(medico), SEEK_SET);
	while(fread(&medicoIndividuo, sizeof(medico), 1, ponteiroMedicos)){
		if(strcmp(medicoIndividuo.nome, nomeMedicoBuscado) == 0){
			printf("\nNome: %s", medicoIndividuo.nome);
			printf("\nCRM: %s", medicoIndividuo.crm);
			printf("\nEspecialidade: %s", medicoIndividuo.especialidade);
			printf("\nData de nascimento: %s", medicoIndividuo.dataDeNascimento);
			printf("\nTelefone: %s", medicoIndividuo.telefone);
			printf("\nValor por hora trabalhada: %.2f\n", medicoIndividuo.valorHoraTrabalho);
			cont++;
		}
	}
	if(cont == 0){
		printf("\nMedico nao encontrado\n");
	}
	cont = 0;
};

void listarPorEspecialidade(FILE *ponteiroMedicos){

	medico medicoIndividuo;
	char especialidadeBuscada[20];
	int achou = 0;

	fflush(stdin);
	printf("\nEspecialidade desejada: ");
	scanf("%[^\n]s", especialidadeBuscada);

	fseek(ponteiroMedicos, 0*sizeof(medico), SEEK_SET);
	while(fread(&medicoIndividuo, sizeof(medico), 1, ponteiroMedicos)){
		if(strcmp(medicoIndividuo.especialidade, especialidadeBuscada) == 0){
			printf("\nNome: %s", medicoIndividuo.nome);
			printf("\nCRM: %s", medicoIndividuo.crm);
			printf("\nEspecialidade: %s", medicoIndividuo.especialidade);
			printf("\nData de nascimento: %s", medicoIndividuo.dataDeNascimento);
			printf("\nTelefone: %s", medicoIndividuo.telefone);
			printf("\nValor por hora trabalhada: %.2f\n", medicoIndividuo.valorHoraTrabalho);
			achou = 1;
		}
	}
	if(achou == 0)
		printf("\nEspecialidade nao cadastrada\n");
};

void alterarDadosMedico(FILE *ponteiroMedicos){

	medico medicoIndividuo;
	char crmIndividuo[6];
	int contadorArquivo = 0, achou = 0;
	char crmTeste[6];
	char crmTeste2[6];

	printf("\nCRM do medico: ");
	fflush(stdin);
	scanf("%[^\n]s", crmIndividuo);

	while(fread(&medicoIndividuo, sizeof(medico), 1, ponteiroMedicos)){
		if(strcmp(medicoIndividuo.crm, crmIndividuo) == 0){
			fflush(stdin);
			printf("\nNovo nome: ");
			scanf("%[^\n]s", medicoIndividuo.nome);
			fflush(stdin);
			strcpy(crmTeste2, medicoIndividuo.crm);  //faz uma copia do crm antigo
			printf("Novo CRM: ");
			scanf("%[^\n]s", medicoIndividuo.crm);
			fflush(stdin);

			strcpy(crmTeste, medicoIndividuo.crm);  //faz uma copia do crm novo
			//Chamada da função para verificar possivel conflito de crm igual
				if(verificaDado(ponteiroMedicos, crmTeste, 1) == 1  && strcmp(crmTeste, crmTeste2) != 0) {
					printf("\nMedico ja cadastrado\n");
					return;
				}

			printf("Nova especialidade: ");
			scanf("%[^\n]s", medicoIndividuo.especialidade);
			fflush(stdin);
			printf("Nova data de nascimento: ");
			scanf("%[^\n]s", medicoIndividuo.dataDeNascimento);
			fflush(stdin);
			printf("Novo telefone: ");
			scanf("%[^\n]s", medicoIndividuo.telefone);
			fflush(stdin);
			printf("Novo valor por hora trabalhada: ");
			scanf("%f%*c", &medicoIndividuo.valorHoraTrabalho);
			fflush(stdin);
			
			//apontador para no local onde o crm foi encontrado para alterar os dados
			fseek(ponteiroMedicos, (contadorArquivo)*sizeof(medico), SEEK_SET);
			fwrite(&medicoIndividuo, sizeof(medico), 1, ponteiroMedicos);
			achou = 1;
			break;
		}
		contadorArquivo++;
	}
	if(achou == 0)
		printf("\nCRM nao encontrado\n");
};

//-------------------------------fim Medicos------------------------------------------------

//------------------------------Inicio Pacientes--------------------------------------------

void menuPaciente(FILE *ponteiroPacientes){

	int opcao;

	//menu para acessar as funções de paciente
	do{
		printf("\n(1) Inserir novo paciente");
		printf("\n(2) Buscar pelo nome");
		printf("\n(3) Alterar dados de um paciente");
		printf("\n(0) Retornar\n");
		scanf("%d",&opcao);

		switch (opcao){
			case 1:
				ponteiroPacientes = fopen("dados/pacientes.bin","ab");
				cadastroNovoPaciente(ponteiroPacientes);
				fclose(ponteiroPacientes);
				break;
			case 2:
				ponteiroPacientes = fopen("dados/pacientes.bin","rb");
				buscarPaciente(ponteiroPacientes);
				fclose(ponteiroPacientes);
				break;
			case 3:
				ponteiroPacientes = fopen("dados/pacientes.bin","r+b");
				alterarDadosPaciente(ponteiroPacientes);
				fclose(ponteiroPacientes);
			case 0:
				printf("\nRetornando...\n");
				break;
			default:
				printf("\nFuncao Invalida!\n");
			}	
	}while(opcao != 0);
};

void cadastroNovoPaciente (FILE *ponteiroPacientes){

	paciente pacienteIndividuo;
	char cpfTeste[11];
	
		fflush(stdin);
		printf("\nNome: ");
		scanf("%[^\n]s", pacienteIndividuo.nome);
		fflush(stdin);
		printf("CPF: ");
		scanf("%[^\n]s", pacienteIndividuo.cpf);
		fflush(stdin);

		//Chamada da função para verificar possivel conflito de cpf igual
		strcpy(cpfTeste, pacienteIndividuo.cpf);
		if(verificaDado(ponteiroPacientes, cpfTeste, 0) == 1) {
			printf("\nPaciente ja cadastrado\n");
			return;
		}

		printf("Data de nascimento: ");
		scanf("%[^\n]s", pacienteIndividuo.dataDeNascimento);
		fflush(stdin);
		printf("Telefone: ");
		scanf("%[^\n]s", pacienteIndividuo.telefone);
		fflush(stdin);
		fwrite(&pacienteIndividuo, sizeof(paciente), 1, ponteiroPacientes);

	//---------------------verificar no bando de dados de existe um cpf cadastrado-------------------------------------
	
};

void buscarPaciente (FILE *ponteiroPacientes){

	paciente pacienteIndividuo;
	char nomePacienteBuscado[50];
	int cont = 0;

	printf("\nNome: ");
	fflush(stdin);
	scanf("%[^\n]s", nomePacienteBuscado);
	fseek(ponteiroPacientes, 0*sizeof(paciente), SEEK_SET);
	while(fread(&pacienteIndividuo, sizeof(paciente), 1, ponteiroPacientes)){
	 	if(strcmp(pacienteIndividuo.nome, nomePacienteBuscado) == 0){
	 		printf("\nNome: %s", pacienteIndividuo.nome);
			printf("\nCPF: %s", pacienteIndividuo.cpf);
			printf("\nData de nascimento: %s", pacienteIndividuo.dataDeNascimento);
			printf("\nTelefone: %s\n", pacienteIndividuo.telefone);
	 		cont++;
	 	}
	}
	if(cont == 0){
		printf("\nPaciente nao encontrado\n");
	}
	cont = 0;
};

void alterarDadosPaciente(FILE *ponteiroPacientes){

	paciente pacienteIndividuo;
	char nomePacienteBuscado[50];
	int contadorArquivo = 0, achou = 0;
	char cpfTeste[11];  //novo cpf digitado
	char cpfTeste2[11];  //cpf ja cadastrado do usuario

	fflush(stdin);
	printf("\nNome: ");
	scanf("%[^\n]s", nomePacienteBuscado);

	while(fread(&pacienteIndividuo, sizeof(paciente), 1, ponteiroPacientes)){
		if(strcmp(nomePacienteBuscado,pacienteIndividuo.nome)==0){
			fflush(stdin);
			printf("\nNome: ");
			scanf("%[^\n]s",pacienteIndividuo.nome);
			fflush(stdin);
			strcpy(cpfTeste2, pacienteIndividuo.cpf);  //faz uma cópia do cpf antigo
			printf("CPF: ");
			scanf("%[^\n]s",pacienteIndividuo.cpf);
			fflush(stdin);
			
			strcpy(cpfTeste, pacienteIndividuo.cpf);  //faz uma cópia do cpf novo
			//Chamada da função para verificar possivel conflito de cpf igual
			if(verificaDado(ponteiroPacientes, cpfTeste, 0) == 1 && strcmp(cpfTeste, cpfTeste2) != 0) {
				printf("\nPaciente ja cadastrado\n");
				return;
			}

			printf("Data de Nascimento: ");
			scanf("%[^\n]s",pacienteIndividuo.dataDeNascimento);
			fflush(stdin);
			printf("Telefone: ");
			scanf("%[^\n]s",pacienteIndividuo.telefone);
			fflush(stdin);
			
			fseek(ponteiroPacientes, (contadorArquivo)*sizeof(paciente), SEEK_SET);
			fwrite(&pacienteIndividuo, sizeof(paciente), 1, ponteiroPacientes);
			achou = 1;
			break;
		}
		contadorArquivo++;
	}
	if(achou == 1)
		printf("\nDados alterados com sucesso!\n");
	if(achou == 0)
		printf("\nPaciente nao encontrado!\n");
};

//-------------------------------fim Pacientes---------------------------------------------

int verificaDado(FILE *arquivoGenerico, char *dadoComparado, int medicoPaciente){

	medico medicoIndividuo;
	paciente pacienteIndividuo;

	//Caso medicoPaciente seja 1, trata-se de uma busca por crm, caso nao, sera busca de cpf de paciente
	if(medicoPaciente == 1){   //se for medico
		arquivoGenerico	= fopen("dados/medicos.bin", "rb");
		fseek(arquivoGenerico, 0*sizeof(medico), SEEK_SET);
		while(fread(&medicoIndividuo, sizeof(medico), 1, arquivoGenerico)){
			if(strcmp(dadoComparado, medicoIndividuo.crm) == 0){
				fclose(arquivoGenerico);
				return 1;  //crm repetido
			}
		}
		return 0;  //crm nao repetido
	}

	else{   //se for paciente
		arquivoGenerico	= fopen("dados/pacientes.bin", "rb");
		fseek(arquivoGenerico, 0*sizeof(paciente), SEEK_SET);
		while(fread(&pacienteIndividuo, sizeof(paciente), 1, arquivoGenerico)){
			if(strcmp(dadoComparado, pacienteIndividuo.cpf) == 0){
				fclose(arquivoGenerico);
				return 1;  //cpf repetido
			}
		}
		return 0;  //cpf nao repetido
	}
};

// ------------------------------inicio consulta-------------------------------------------

void menuConsulta(FILE *ponteiroConsultas, FILE *ponteiroMedicos, FILE *ponteiroPacientes){

	int opcao;

	//Menu para acessar as funções de consulta
	do{
		printf("\n(1) Inserir dados de nova consulta");
		printf("\n(2) Listar todas consultas de um medico");
		printf("\n(3) Listar todas consultas de um paciente");
		printf("\n(0) Retornar\n");
		scanf("%d", &opcao);

		switch (opcao){
			case 1:
				ponteiroConsultas = fopen("dados/consultas.bin", "ab");
				inserirNovaConsulta(ponteiroConsultas,ponteiroMedicos,ponteiroPacientes);
				fclose(ponteiroConsultas);
				break;
			case 2:
				ponteiroConsultas = fopen("dados/consultas.bin", "rb");
				listarConsultasMedico(ponteiroConsultas,ponteiroMedicos);
				fclose(ponteiroConsultas);
				break;
			case 3:
				ponteiroConsultas = fopen("dados/consultas.bin", "rb");
				listarConsultasPacientes(ponteiroConsultas,ponteiroPacientes);
				fclose(ponteiroConsultas);
				break;
			case 0:
				printf("\nRetornando...\n");
				break;
			default:
				printf("\nOpcao invalida!\n");
		}
	} while(opcao != 0);
};

void inserirNovaConsulta(FILE *ponteiroConsultas, FILE *ponteiroMedicos, FILE *ponteiroPacientes){
	
	consulta consultaIndividuo;
	

	fflush(stdin);
	printf("\nCRM do medico: ");
	scanf("%[^\n]s", consultaIndividuo.crmMedico);
	fflush(stdin);

	//função para verificar existencia do medico no bando de dados
	if(verificaDado(ponteiroMedicos,consultaIndividuo.crmMedico,1) == 0){
		printf("\nMedico nao cadastrado, por favor cadastre primeiro!\n");
		return;
	}

	printf("CPF do paciente: ");
	scanf("%[^\n]s", consultaIndividuo.cpfPaciente);
	fflush(stdin);

	//função para verificar existencia do paciente no bando de dados
	if(verificaDado(ponteiroPacientes,consultaIndividuo.cpfPaciente,0) == 0){
		printf("\nPaciente nao cadastrado, por favor cadastre primeiro!\n");
		return;
	}

	printf("Data da consulta: ");
	scanf("%[^\n]s", consultaIndividuo.data);
	fflush(stdin);
	printf("Sintomas: ");
	scanf("%[^\n]s", consultaIndividuo.sintoma);
	fflush(stdin);
	printf("Encaminhamentos: ");
	scanf("%[^\n]s", consultaIndividuo.encaminhamento);
	fflush(stdin);
	fwrite(&consultaIndividuo, sizeof(consulta), 1, ponteiroConsultas);
};

void listarConsultasMedico(FILE *ponteiroConsultas, FILE *ponteiroMedicos){

	consulta consultaIndividuo;
	char crmTeste[6];

	fflush(stdin);
	printf("\nCRM: ");
	scanf("%[^\n]s", crmTeste);

	if(verificaDado(ponteiroMedicos, crmTeste, 1) == 0){
		printf("\nMedido nao cadastrado!\n");
		return;
	}
	
	fseek(ponteiroConsultas, 0*sizeof(consulta), SEEK_SET);
	while(fread(&consultaIndividuo, sizeof(consulta), 1, ponteiroConsultas)){
		if(strcmp(consultaIndividuo.crmMedico, crmTeste) == 0){
			printf("\nCRM: %s", consultaIndividuo.crmMedico);
			printf("\nCPF do Paciente: %s", consultaIndividuo.cpfPaciente);
			printf("\nData da consulta: %s", consultaIndividuo.data);
			printf("\nSintomas: %s", consultaIndividuo.sintoma);
			printf("\nEncaminhamento: %s\n", consultaIndividuo.encaminhamento);
		}
	}
};

void listarConsultasPacientes(FILE *ponteiroConsultas, FILE *ponteiroPacientes){

	consulta consultaIndividuo;
	char cpfTeste[6];

	fflush(stdin);
	printf("\nCPF: ");
	scanf("%[^\n]s", cpfTeste);

	if(verificaDado(ponteiroPacientes, cpfTeste, 1) == 0){
		printf("\nPaciente nao cadastrado!\n");
		return;
	}
	
	fseek(ponteiroConsultas, 0*sizeof(consulta), SEEK_SET);
	while(fread(&consultaIndividuo, sizeof(consulta), 1, ponteiroConsultas)){
		if(strcmp(consultaIndividuo.cpfPaciente, cpfTeste) == 0){
			printf("\nCRM: %s", consultaIndividuo.crmMedico);
			printf("\nCPF do Paciente: %s", consultaIndividuo.cpfPaciente);
			printf("\nData da consulta: %s", consultaIndividuo.data);
			printf("\nSintomas: %s", consultaIndividuo.sintoma);
			printf("\nEncaminhamento: %s\n", consultaIndividuo.encaminhamento);
		}
	}
};

// ------------------------------fim consulta----------------------------------------------