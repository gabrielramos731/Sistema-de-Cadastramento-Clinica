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

typedef struct {
	char nome[50];
	char cpf[11];
	char dataDeNascimento[10];
	char telefone[11];
} paciente;