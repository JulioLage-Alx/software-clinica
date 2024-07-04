#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdio>
#include <algorithm>
#include <sstream>
#include <limits>
#include <iomanip>

using namespace std;

class DATA

{
private:
    int dia;
    int mes;
    int ano;
    int horas;
    int minutos;

public:
    DATA(int d, int m, int a) : dia(d), mes(m), ano(a) {}
    DATA(int d, int m, int a, int h, int mn) : dia(d), mes(m), ano(a), horas(h), minutos(mn)
    {
        if (!validarData(d, m, a) || !validarHora(h, mn))
        {
            throw invalid_argument("Data ou hora inválidas.");
        }
    }

    int getdia() const { return dia; }
    int getmes() const { return mes; }
    int getano() const { return ano; }
    int gethoras() const { return horas; }
    int getminutos() const { return minutos; }

    bool operator==(const DATA &other) const
    {
        return dia == other.dia && mes == other.mes && ano == other.ano && horas == other.horas && minutos == other.minutos;
    }

    string toString() const
    {
        return to_string(dia) + "/" + to_string(mes) + "/" + to_string(ano);
    }
    friend istream &operator>>(istream &in, DATA &data)
    {
        cout << "Dia: ";
        in >> data.dia;
        cout << "Mês: ";
        in >> data.mes;
        cout << "Ano: ";
        in >> data.ano;
        return in;
    }

    bool validarData(int d, int m, int a) const
    {
        if (m < 1 || m > 12)
        {
            return false;
        }

        if (d < 1)
        {
            return false;
        }

        static const int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        int max_day = days_in_month[m - 1];

        if (m == 2)
        {
            bool is_leap_year = (a % 4 == 0 && a % 100 != 0) || (a % 400 == 0);
            if (is_leap_year)
            {
                max_day = 29;
            }
        }

        return d <= max_day;
    }

    // Função para validar os dados da hora
    bool validarHora(int h, int mn) const
    {
        if (h < 0 || h > 23)
        {
            return false;
        }

        if (mn < 0 || mn > 59)
        {
            return false;
        }

        return true;
    }
};
class PACIENTE {
private:
    string nome;
    DATA data;
    int codigo;
    string telefone;
    string logradouro;
    int numero;
    string bairro;
    string complemento;
    long int CEP;
    string cidade;
    string estado;

public:
    // Construtor atualizado com os novos atributos
    PACIENTE(string n, DATA date, int cod, string tel, string log, int num, string brr, string comple, long int cep, string cid, string est)
        : nome(n), data(date), codigo(cod), telefone(tel), logradouro(log), numero(num), bairro(brr), complemento(comple), CEP(cep), cidade(cid), estado(est) {}

    // Métodos get para os atributos
    string getNome() const { return nome; }
    DATA getData() const { return data; }
    int getCodigo() const { return codigo; }
    string getTelefone() const { return telefone; }
    string getLogradouro() const { return logradouro; }
    int getNumero() const { return numero; }
    string getBairro() const { return bairro; }
    string getComplemento() const { return complemento; }
    long int getCEP() const { return CEP; }
    string getCidade() const { return cidade; }
    string getEstado() const { return estado; }
    };

class MEDICO {
private:
    string nome;
    string especialidade;
    int codigo;
    string telefone;

public:
    // Construtor atualizado com o novo atributo
    MEDICO(string n, string esp, int cod, string tel) : nome(n), especialidade(esp), codigo(cod), telefone(tel) {}

    // Métodos get para os atributos
    string getNome() const { return nome; }
    string getEspecialidade() const { return especialidade; }
    int getCodigo() const { return codigo; }
    string getTelefone() const { return telefone; }
};

class CONSULTA
{
private:
    DATA data;
    int codigoPaciente;
    int codigoMedico;
    int horas;
    int minutos;

public:
    CONSULTA(DATA date, int pac, int med, int horas, int minutos) : data(date), codigoPaciente(pac), codigoMedico(med), horas(horas), minutos(minutos) {}
    DATA getdata() const { return data; }
    int getpaciente() const { return codigoPaciente; }
    int getmedico() const { return codigoMedico; }
    int getHoras() const { return horas; }
    int getMinutos() const { return minutos; }
};


string validarTelefone() {
    string telefone;
    bool valido = false;

    while (!valido) {
        cout << "Digite o telefone (11 dígitos): ";
        getline(cin, telefone);

        // Remove espaços em branco extras
        telefone.erase(remove_if(telefone.begin(), telefone.end(), ::isspace), telefone.end());

        // Verifica se o telefone tem 11 dígitos numéricos
        if (telefone.length() == 11 && all_of(telefone.begin(), telefone.end(), ::isdigit)) {
            valido = true;
        } else {
            cout << "Telefone inválido. Por favor, digite novamente." << endl;
        }
    }

    return telefone;
}
int geraCodigo(const vector<PACIENTE> &lista) {
    if (lista.empty()) {
        return 1; // Se a lista está vazia, retorna 1 como primeiro código
    } else {
        // Encontra o último código usado na lista de pacientes
        int ultimoCodigo = lista.back().getCodigo();
        return ultimoCodigo + 1; // Retorna o próximo código
    }
}

int contarConsultasPorMedico(const vector<CONSULTA> &listaConsultas, int codigoMedico, const DATA &data)
{
    return count_if(listaConsultas.begin(), listaConsultas.end(), [&data, codigoMedico](const CONSULTA &c)
                    { return c.getdata().getdia() == data.getdia() &&
                             c.getdata().getmes() == data.getmes() &&
                             c.getdata().getano() == data.getano() &&
                             c.getmedico() == codigoMedico; });
}

bool verificarIntervaloConsulta(const vector<CONSULTA> &listaConsultas, const DATA &data, int codigoMedico, int horas, int minutos)
{
    // Filtra apenas as consultas do médico e do mesmo dia
    vector<CONSULTA> consultasDoMedico;
    for (const auto &consulta : listaConsultas)
    {
        if (consulta.getmedico() == codigoMedico && consulta.getdata() == data)
        {
            consultasDoMedico.push_back(consulta);
        }
    }

    // Ordena as consultas por hora (e minutos, se necessário)
    sort(consultasDoMedico.begin(), consultasDoMedico.end(), [](const CONSULTA &c1, const CONSULTA &c2)
         {
        if (c1.getHoras() == c2.getHoras()) {
            return c1.getMinutos() < c2.getMinutos();
        }
        return c1.getHoras() < c2.getHoras(); });

    // Verifica o intervalo de 30 minutos entre as consultas
    int totalConsultas = consultasDoMedico.size();
    for (int i = 1; i < totalConsultas; ++i)
    {
        const CONSULTA &consultaAnterior = consultasDoMedico[i - 1];
        const CONSULTA &consultaAtual = consultasDoMedico[i];

        // Calcula a diferença em minutos entre as consultas
        int diffHoras = consultaAtual.getHoras() - consultaAnterior.getHoras();
        int diffMinutos = consultaAtual.getMinutos() - consultaAnterior.getMinutos();
        int diffTotalMinutos = diffHoras * 60 + diffMinutos;

        // Se o intervalo for menor que 30 minutos, retorna falso
        if (diffTotalMinutos < 30)
        {
            return false;
        }
    }

    // Se todas as consultas respeitarem o intervalo mínimo, retorna verdadeiro
    return true;
}

void marcaConsulta(vector<CONSULTA> &listaConsultas, const vector<PACIENTE> &listaPacientes,
                   const vector<MEDICO> &listaMedicos, DATA data, int codigoPaciente, int codigoMedico, string arquivoconsulta,
                   int horas, int minutos)
{
    // Verificar se o paciente e o médico existem nas listas
    auto itPaciente = find_if(listaPacientes.begin(), listaPacientes.end(), [codigoPaciente](const PACIENTE &p)
                              { return p.getCodigo() == codigoPaciente; });

    auto itMedico = find_if(listaMedicos.begin(), listaMedicos.end(), [codigoMedico](const MEDICO &m)
                            { return m.getCodigo() == codigoMedico; });

    if (itPaciente == listaPacientes.end())
    {
        cout << "Paciente com código " << codigoPaciente << " não encontrado." << endl;
        return;
    }

    if (itMedico == listaMedicos.end())
    {
        cout << "Médico com código " << codigoMedico << " não encontrado." << endl;
        return;
    }

    // Verificar quantas consultas o médico já tem no dia especificado
    int consultasNoDia = contarConsultasPorMedico(listaConsultas, codigoMedico, data);

    if (consultasNoDia >= 2)
    {
        cout << "O médico com código " << codigoMedico << " já atingiu o limite de consultas para o dia." << endl;
        return;
    }

    // Verificar intervalo de 30 minutos entre as consultas do médico no mesmo dia
    if (!verificarIntervaloConsulta(listaConsultas, data, codigoMedico, horas, minutos))
    {
        cout << "Intervalo mínimo de 30 minutos entre consultas não respeitado para o médico com código " << codigoMedico << "." << endl;
        return;
    }

    // Adicionar a nova consulta
    listaConsultas.push_back(CONSULTA(data, codigoPaciente, codigoMedico, horas, minutos));
    cout << "Consulta marcada com sucesso!" << endl;

    // Atualizar o arquivo de consultas
    ofstream anota(arquivoconsulta, ios::app);
    if (!anota)
    {
        cout << "Não foi possível cadastrar essa consulta no arquivo." << endl;
    }
    else
    {
        anota << data.toString() << " - Paciente: " << codigoPaciente << " - Médico: " << codigoMedico << " - Hora: " << horas << ":" << minutos << endl;
        cout << "Consulta registrada no arquivo." << endl;
    }
}

void cancelarconsulta(vector<CONSULTA> &lista, int codpaciente, string arquivo)
{

    // Remover o arquivo antigo
    if (remove(arquivo.c_str()) != 0)
    {
        cerr << "Erro ao excluir o arquivo: " << arquivo << endl;
    }

    // Remover o MEDICO do vetor
    lista.erase(remove_if(lista.begin(), lista.end(),
                          [codpaciente](const CONSULTA &p)
                          { return p.getpaciente() == codpaciente; }),
                lista.end());

    // Criar um novo arquivo com os pacientes restantes

    ofstream anota(arquivo, ios::app);
    if (!anota)
    {
        cout << "Não foi possível cadastrar essa consulta no arquivo." << endl;
    }
    else
    {
        for (const auto &paci : lista)
        {
            anota << paci.getdata().toString() << " - Paciente: " << paci.getpaciente() << " - Médico: " << paci.getmedico() << " - Hora: " << paci.getHoras() << ":" << paci.getMinutos() << endl;
        }
    }
    anota.close();

    cout << "Consulta registrada no arquivo." << endl;
}

vector<CONSULTA> inicializarConsulta(const string &nomeArquivo)
{
    vector<CONSULTA> consultas;
    ifstream arquivo(nomeArquivo);

    if (!arquivo.is_open())
    {
        cerr << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
        return consultas; // Retorna vetor vazio se houver erro ao abrir o arquivo
    }

    string linha;
    while (getline(arquivo, linha))
    {
        stringstream ss(linha);
        string dataStr, pacienteCodigoStr, medicoCodigoStr, horaStr, minutoStr;

        // Extrai dados da linha
        getline(ss, dataStr, '-');
        getline(ss, pacienteCodigoStr, '-');
        getline(ss, medicoCodigoStr, '-');
        getline(ss, horaStr, ':');
        getline(ss, minutoStr, '-');

        // Converte a string de data para um objeto DATA
        stringstream ssData(dataStr);
        int dia, mes, ano;
        char delim;
        ssData >> dia >> delim >> mes >> delim >> ano;
        DATA data(dia, mes, ano);

        // Converte as strings de código de paciente, médico, hora e minuto para inteiros
        int pacienteCodigo = stoi(pacienteCodigoStr);
        int medicoCodigo = stoi(medicoCodigoStr);
        int horas = stoi(horaStr);
        int minutos = stoi(minutoStr);

        // Cria um objeto CONSULTA e adiciona ao vetor
        consultas.push_back(CONSULTA(data, pacienteCodigo, medicoCodigo, horas, minutos));
    }

    arquivo.close();
    return consultas;
}
int geraCodigoMedico(const vector<MEDICO> &lista) {
    if (lista.empty()) {
        return 1; // Se a lista está vazia, retorna 1 como primeiro código
    } else {
        // Encontra o último código usado na lista de médicos
        int ultimoCodigo = lista.back().getCodigo();
        return ultimoCodigo + 1; // Retorna o próximo código
    }
}

void cadastraMEDICO(vector<MEDICO> &lista, string nome, string especialidade, string telefone, string nometxt) {
    int cod = geraCodigoMedico(lista);
    MEDICO novo(nome, especialidade, cod, telefone);
    lista.push_back(novo);

    ofstream anota(nometxt, ios::app);
    if (!anota) {
        cout << "Nao foi possivel cadastrar esse medico" << endl;
    } else {
        anota << nome << " - " << especialidade << " - " << cod << " - " << telefone << endl;
        cout << "Medico Cadastrado com Sucesso" << endl
             << "Nome: " << nome << endl
             << "Especialidade: " << especialidade << endl
             << "Codigo: " << cod << endl
             << "Telefone: " << telefone << endl;
    }
    anota.close();
}


void descadastramedico(vector<MEDICO> &lista, string nometxt, string nome)
{
    // Remover o arquivo antigo
    if (remove(nometxt.c_str()) != 0)
    {
        cerr << "Erro ao excluir o arquivo: " << nometxt << endl;
    }

    // Remover o MEDICO do vetor
    lista.erase(remove_if(lista.begin(), lista.end(),
                          [nome](const MEDICO &p)
                          { return p.getNome() == nome; }),
                lista.end());

    // Criar um novo arquivo com os pacientes restantes
    ofstream anota(nometxt, ios::app);
    if (!anota) {
        cout << "Nao foi possivel cadastrar esse medico" << endl;
    } else {
          for (const auto &paci : lista)
        {
            anota << paci.getNome() << " - " << paci.getEspecialidade() << " - " << paci.getCodigo() << paci.getTelefone()<<endl;
        }
    }
    anota.close();
}

      

bool nomeEstaNoMedico(const vector<MEDICO> &lista, const string &nomeProcurado)
{
    // Percorre o vetor de pacientes
    for (const auto &paciente : lista)
    {
        // Compara o nome procurado com o nome do paciente atual
        if (paciente.getNome() == nomeProcurado)
        {
            return true; // Encontrou o nome no vetor
        }
    }
    return false; // Não encontrou o nome no vetor
}

void EditarMedico(vector<MEDICO> &lista, string nometxt, string nome)
{

    string newnome;
    string newespecialidade;
    string newtelefone ;

    if (nomeEstaNoMedico(lista, nome))
    {
        descadastramedico(lista, nometxt, nome);
        cout << "Qual nome deseja manter" << endl;
        cin >> newnome;
        cout << "Insira a nova especialidade" << endl;
        cin >> newespecialidade;
        newtelefone = validarTelefone();
        cadastraMEDICO(lista, newnome, nometxt, newespecialidade,newtelefone);
        cout << "Medico editado com sucesso." << endl;
    }
    else
        cout << "Medico procurado nao esta cadastrado" << endl;
}

void ListarMedicos(const vector<MEDICO> &lis)
{
    cout << "----LISTA DE PACIENTES----" << endl;

    for (const auto &medicos : lis)
    {
        cout << "Nome: " << medicos.getNome() << endl;
        cout << "Especialidade: " << medicos.getEspecialidade() << endl;
        cout << "Telefone: " << medicos.getTelefone() << endl;
        cout << "Codigo: " << medicos.getCodigo() << endl
             << endl;
    }
}

vector<MEDICO> inicializarMedico(const string &nomeArquivo) {
    vector<MEDICO> medicos;
    ifstream arquivo(nomeArquivo);

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
        return medicos; // Retorna vetor vazio se houver erro ao abrir o arquivo
    }

    string linha;
    while (getline(arquivo, linha)) {
        stringstream ss(linha);
        string nome, especialidade, telefone;
        int codigo;

        // Extrai nome, especialidade, código e telefone da linha
        getline(ss, nome, '-');
        getline(ss, especialidade, '-');
        ss >> codigo;
        ss.ignore(1, '-'); // Ignora o caractere '-' após o código
        getline(ss, telefone);

        // Cria um objeto MEDICO e adiciona ao vetor
        medicos.push_back(MEDICO(nome, especialidade, codigo, telefone));
    }

    arquivo.close();
    return medicos;
}


void cadastraPaciente(vector<PACIENTE> &lista, string nome, DATA date, string telefone, string logradouro, int numero, string bairro, string complemento, long int CEP, string cidade, string estado, string nometxt) {
    int cod = geraCodigo(lista);
    PACIENTE novo(nome, date, cod, telefone, logradouro, numero, bairro, complemento, CEP, cidade, estado);
    lista.push_back(novo);

    ofstream anota(nometxt, ios::app);
    if (!anota) {
        cout << "Nao foi possivel cadastrar esse paciente" << endl;
    } else {
        anota << nome << " - " << date.toString() << " - " << cod << " - " << telefone << " - " << logradouro << " - " << numero << " - " << bairro << " - " << complemento << " - " << CEP << " - " << cidade << " - " << estado << endl;
        cout << "Paciente Cadastrado com Sucesso" << endl
             << "Nome: " << nome << endl
             << "Nascimento: " << date.getdia() << "/" << date.getmes() << "/" << date.getano() << endl
             << "Codigo: " << cod << endl
             << "Telefone: " << telefone << endl
             << "Logradouro: " << logradouro << endl
             << "Numero: " << numero << endl
             << "Bairro: " << bairro << endl
             << "Complemento: " << complemento << endl
             << "CEP: " << CEP << endl
             << "Cidade: " << cidade << endl
             << "Estado: " << estado << endl;
    }
    anota.close();
}

void descadastraPaciente(vector<PACIENTE> &lista, string nometxt, string nome) {
    // Remover o arquivo antigo
    if (remove(nometxt.c_str()) != 0) {
        cerr << "Erro ao excluir o arquivo: " << nometxt << endl;
    }

    // Remover o paciente do vetor
    lista.erase(remove_if(lista.begin(), lista.end(),
                          [nome](const PACIENTE &p) { return p.getNome() == nome; }),
                lista.end());

    // Criar um novo arquivo com os pacientes restantes
    ofstream anota(nometxt, ios::app);
    if (!anota) {
        cout << "Nao foi possivel criar o arquivo: " << nometxt << endl;
    } else {
        for (const auto &paci : lista) {
            anota << paci.getNome() << " - " << paci.getData().toString() << " - " << paci.getCodigo() << " - " << paci.getTelefone() << " - " << paci.getLogradouro() << " - " << paci.getNumero() << " - " << paci.getBairro() << " - " << paci.getComplemento() << " - " << paci.getCEP() << " - " << paci.getCidade() << " - " << paci.getEstado() << endl;
        }
    }
    anota.close();
}

bool nomeEstaNoVector(const vector<PACIENTE> &lista, const string &nomeProcurado)
{
    // Percorre o vetor de pacientes
    for (const auto &paciente : lista)
    {
        // Compara o nome procurado com o nome do paciente atual
        if (paciente.getNome() == nomeProcurado)
        {
            return true; // Encontrou o nome no vetor
        }
    }
    return false; // Não encontrou o nome no vetor
}

void EditarPaciente(vector<PACIENTE> &lista, string nometxt, string nome)
{
    if (nomeEstaNoVector(lista, nome))
    {
        descadastraPaciente(lista, nometxt, nome);

        string newNome, newTelefone, newLogradouro, newBairro, newComplemento, newCidade, newEstado;
        int newDia, newMes, newAno, newNumero;
        long int newCEP;

        cout << "Qual nome deseja manter: ";
        cin >> newNome;
        cout << "Insira a nova data (dia mes ano): ";
        cin >> newDia >> newMes >> newAno;
        DATA newData(newDia, newMes, newAno);

        newTelefone  = validarTelefone();
        cout << "Insira o novo logradouro: ";
        cin >> newLogradouro;
        cout << "Insira o novo número: ";
        cin >> newNumero;
        cout << "Insira o novo bairro: ";
        cin >> newBairro;
        cout << "Insira o novo complemento: ";
        cin >> newComplemento;
        cout << "Insira o novo CEP: ";
        cin >> newCEP;
        cout << "Insira a nova cidade: ";
        cin >> newCidade;
        cout << "Insira o novo estado: ";
        cin >> newEstado;

        int cod = lista.size() + 1; // Atualizar o código do paciente
        PACIENTE novo(newNome, newData, cod, newTelefone, newLogradouro, newNumero, newBairro, newComplemento, newCEP, newCidade, newEstado);
        lista.push_back(novo);

        ofstream anota(nometxt, ios::app);
        if (!anota)
        {
            cout << "Nao foi possivel cadastrar esse paciente" << endl;
        }
        else
        {
            anota << novo.getNome() << " - " << novo.getData().toString() << " - " << novo.getCodigo() << " - " << novo.getTelefone() << " - " << novo.getLogradouro() << " - " << novo.getNumero() << " - " << novo.getBairro() << " - " << novo.getComplemento() << " - " << novo.getCEP() << " - " << novo.getCidade() << " - " << novo.getEstado() << endl;
            cout << "Paciente editado com sucesso." << endl;
        }
        anota.close();
    }
    else
    {
        cout << "Paciente procurado nao esta cadastrado" << endl;
    }
}


void ListarPacientes(const vector<PACIENTE> &lis)
{
    cout << "----LISTA DE PACIENTES----" << endl;

    for (const auto &pacientes : lis)
    {
        cout << "Nome: " << pacientes.getNome() << endl;
        cout << "Data: " << pacientes.getData().getdia() << "/" << pacientes.getData().getmes() << "/" << pacientes.getData().getano() << endl;
        cout << "Codigo: " << pacientes.getCodigo() << endl;
        cout << "Telefone: " << pacientes.getTelefone() << endl;
        cout << "Logradouro: " << pacientes.getLogradouro() << " Numero: " << pacientes.getNumero() << endl;
        cout << "Bairro: " << pacientes.getBairro() << endl;
        cout << "Complemento: " << pacientes.getComplemento() << endl;
        cout << "CEP: " << pacientes.getCEP() << endl;
        cout << "Cidade: " << pacientes.getCidade() << endl;
        cout << "Estado: " << pacientes.getEstado() << endl;
        cout << endl;
    }
}


vector<PACIENTE> inicializarPacientes(const string &nomeArquivo)
{
    vector<PACIENTE> pacientes;
    ifstream arquivo(nomeArquivo);

    if (!arquivo.is_open())
    {
        cerr << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
        return pacientes; // Retorna vetor vazio se houver erro ao abrir o arquivo
    }

    string linha;
    while (getline(arquivo, linha))
    {
        stringstream ss(linha);
        string nome, dataStr, telefone, logradouro, bairro, complemento, cidade, estado;
        int codigo, numero;
        long int CEP;

        // Extrai os dados da linha
        getline(ss, nome, '-');
        getline(ss, dataStr, '-');
        ss >> codigo;
        ss.ignore(); // Ignora o espaço após o código
        getline(ss, telefone, '-');
        getline(ss, logradouro, '-');
        ss >> numero;
        ss.ignore(); // Ignora o espaço após o número
        getline(ss, bairro, '-');
        getline(ss, complemento, '-');
        ss >> CEP;
        ss.ignore(); // Ignora o espaço após o CEP
        getline(ss, cidade, '-');
        getline(ss, estado, '-');

        // Converte a string de data para um objeto DATA
        stringstream ssData(dataStr);
        int dia, mes, ano;
        char delim;
        ssData >> dia >> delim >> mes >> delim >> ano;
        DATA data(dia, mes, ano);

        // Cria um objeto PACIENTE e adiciona ao vetor
        pacientes.push_back(PACIENTE(nome, data, codigo, telefone, logradouro, numero, bairro, complemento, CEP, cidade, estado));
    }

    arquivo.close();
    return pacientes;
}


void menuConsulta(string &ArquivoPaciente, vector<PACIENTE> &Listadepaciente, string &ArquivoMedico, vector<MEDICO> &Listademedicos,
                  string &ArquivoConsulta, vector<CONSULTA> &ListadeConsulta)
{
    int escolha;
    bool marcado = true;
    while (marcado)
    {
        cout << "<------------------------------->" << endl;
        cout << "\tMenu Consultas" << endl;
        cout << "<------------------------------->" << endl;
        cout << "1. Marcar Consulta" << endl;
        cout << "2. Cancelar Consulta" << endl;
        cout << "3. Voltar ao Menu Principal" << endl;
        cout << "Escolha uma opção: ";
        cin >> escolha;

        switch (escolha)
        {
        case 1:
        {
            int codmed, codpac;
            int dia, mes, ano, horas, minutos;
            cout << "Informe o codigo do paciente: " << endl;
            cin >> codpac;
            cout << "Informe o codigo do medico: " << endl;
            cin >> codmed;
            cout << "Informe o dia da consulta(dd mm aaaa): " << endl;
            cin >> dia >> mes >> ano;
            DATA date(dia, mes, ano);
            cout << "Informe a hora da consulta (hh mm): ";
            cin >> horas >> minutos;
            marcaConsulta(ListadeConsulta, Listadepaciente, Listademedicos, date, codpac, codmed, ArquivoConsulta, horas, minutos);

            break;
        }
        case 2:
        {
            int codpac;
            cout << "Informe o codigo do paciente que deseja desmarca a consulta: " << endl;
            cin >> codpac;
            cancelarconsulta(ListadeConsulta, codpac, ArquivoConsulta);
            break;
        }
        case 3:
            marcado = false;
            break;
        default:
            cout << "Opção inválida, por favor tente novamente." << endl;
            break;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}
void menuCadastro(string &ArquivoPaciente, vector<PACIENTE> &Listadepaciente, string &ArquivoMedico, vector<MEDICO> &Listademedicos)
{
    bool marcado = true;
    while (marcado)
    {
        cout << "<------------------------------->" << endl;
        cout << "\tMenu Cadastro" << endl;
        cout << "<------------------------------->" << endl;
        cout << "1. Cadastrar Paciente" << endl;
        cout << "2. Consultar Cadastro de Paciente" << endl;
        cout << "3. Listar Pacientes" << endl;
        cout << "4. Editar Paciente" << endl;
        cout << "5. Remover Paciente" << endl;
        cout << "6. Cadastrar Médico" << endl;
        cout << "7. Consultar Cadastro de Médico" << endl;
        cout << "8. Listar Médicos" << endl;
        cout << "9. Editar Médico" << endl;
        cout << "10. Remover Médico" << endl;
        cout << "11. Voltar ao Menu Principal" << endl;
        cout << "Escolha uma opção: ";
        int escolha;
        cin >> escolha;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignora qualquer coisa deixada no buffer de entrada

        switch (escolha)
        {
        case 1:
        {
            string nome;
    int dia, mes, ano;
    cout << "Informe o nome do paciente: ";
    cin.ignore(); // Limpa o buffer do newline pendente
    getline(cin, nome);

    cout << "Informe a data de nascimento (dd mm aaaa): ";
    cin >> dia >> mes >> ano;
    DATA date(dia, mes, ano);


    string telefone  = validarTelefone();
    cin.ignore(); 

    cout << "Informe o logradouro: ";
    string logradouro;
    getline(cin, logradouro);

    cout << "Informe o número: ";
    int numero;
    cin >> numero;

    cout << "Informe o bairro: ";
    string bairro;
    cin.ignore(); // Limpa o buffer do newline pendente
    getline(cin, bairro);

    cout << "Informe o complemento: ";
    string complemento;
    getline(cin, complemento);

    cout << "Informe o CEP: ";
    long int CEP;
    cin >> CEP;

    cout << "Informe a cidade: ";
    string cidade;
    cin.ignore(); // Limpa o buffer do newline pendente
    getline(cin, cidade);

    cout << "Informe o estado: ";
    string estado;
    getline(cin, estado);

    cadastraPaciente(Listadepaciente, nome, date, telefone, logradouro, numero, bairro, complemento, CEP, cidade, estado, ArquivoPaciente);
    break;
        }
        case 2:
        {
            string nome;
            cout << "Informe o nome do paciente que voce deseja procurar: " << endl;
            getline(cin, nome);
            if (nomeEstaNoVector(Listadepaciente, nome))
            {
                cout << "Paciente " << nome << "esta cadatrado" << endl;
            }
            else
                cout << "Paciente procurado nao esta cadastrado" << endl;

            break;
        }
        case 3:
        {

            ListarPacientes(Listadepaciente);

            break;
        }
        case 4:
        {
            string nome;
            cout << "Informe o nome do paciente que voce deseja editar " << endl;
            getline(cin, nome);
            EditarPaciente(Listadepaciente, ArquivoPaciente, nome);

            break;
        }
        case 5:
        {
            string nome;
            cout << "Informe o nome do paciente que voce deseja descadastra " << endl;
            getline(cin, nome);
            descadastraPaciente(Listadepaciente, ArquivoPaciente, nome);

            break;
        }
        case 6:
        {
            string nome, especialidade,telefone;
            cout << "Informe o nome do medico: " << endl;
            getline(cin, nome);
            cout << "Informe a especialidade: " << endl;
            cin >> especialidade;
            telefone  = validarTelefone();
            cadastraMEDICO(Listademedicos, nome, ArquivoMedico, especialidade,telefone);

            break;
        }
        case 7:
        {
            string nome;
            cout << "Informe o nome do Medico que voce deseja procurar: " << endl;
            getline(cin, nome);
            if (nomeEstaNoMedico(Listademedicos, nome))
            {
                cout << "Medico " << nome << "esta cadatrado" << endl;
            }
            else
                cout << "Medico procurado nao esta cadastrado" << endl;

            break;
        }
        case 8:
        {
            ListarMedicos(Listademedicos);

            break;
        }
        case 9:
        {
            string nome;
            cout << "Informe o nome do medico que voce deseja editar " << endl;
            getline(cin, nome);
            EditarMedico(Listademedicos, ArquivoMedico, nome);

            break;
        }
        case 10:
        {
            string nome;
            cout << "Informe o nome do paciente que voce deseja descadastra " << endl;
            getline(cin, nome);
            descadastramedico(Listademedicos, ArquivoMedico, nome);

            break;
        }
        case 11:
            marcado = false;
            break;
        default:
            cout << "Opção inválida, por favor tente novamente." << endl;
            break;
        }

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void consultasPorMedico(const vector<CONSULTA> &consultas, const vector<MEDICO> &medicos, const string &nomeOuCodigoMedico)
{
    int codigoMedico = -1;
    try
    {
        codigoMedico = stoi(nomeOuCodigoMedico);
    }
    catch (...)
    {
        cerr << "Erro ao converter código do médico para inteiro." << endl;
    }

    bool encontrado = false;
    for (const auto &medico : medicos)
    {
        if (medico.getCodigo() == codigoMedico || medico.getNome() == nomeOuCodigoMedico)
        {
            cout << "Consultas do médico " << medico.getNome() << ":" << endl;
            for (size_t i = 0; i < consultas.size(); ++i)
            {
                if (consultas[i].getmedico() == medico.getCodigo())
                {
                    cout << "Consulta " << (i + 1) << ": Data " << consultas[i].getdata().toString() << " - Paciente: " << consultas[i].getpaciente() << " - Hora: " << consultas[i].getHoras() << ":" << consultas[i].getMinutos() << endl;
                }
            }
            encontrado = true;
            break;
        }
    }

    if (!encontrado)
    {
        cout << "Médico não encontrado." << endl;
    }
}
void consultasPorPaciente(const vector<CONSULTA> &consultas, const vector<PACIENTE> &pacientes, const string &nomeOuCodigoPaciente)
{
    int codigoPaciente = -1;
    try
    {
        codigoPaciente = stoi(nomeOuCodigoPaciente);
    }
    catch (...)
    {
        // nomeOuCodigoPaciente is not a number, treat it as a name
    }

    bool encontrado = false;
    for (const auto &paciente : pacientes)
    {
        if (paciente.getCodigo() == codigoPaciente || paciente.getNome() == nomeOuCodigoPaciente)
        {
            cout << "Consultas do paciente " << paciente.getNome() << ":" << endl;
            for (size_t i = 0; i < consultas.size(); ++i)
            {
                if (consultas[i].getpaciente() == paciente.getCodigo())
                {
                    cout << "Consulta " << (i + 1) << ": Data " << consultas[i].getdata().toString() << " - Médico: " << consultas[i].getmedico() << " - Hora: " << consultas[i].getHoras() << ":" << consultas[i].getMinutos() << endl;
                }
            }
            encontrado = true;
            break;
        }
    }

    if (!encontrado)
    {
        cout << "Paciente não encontrado." << endl;
    }
}

void consultasPorData(const vector<CONSULTA> &consultas, const DATA &dataConsulta)
{
    cout << "Consultas no dia " << dataConsulta.toString() << ":" << endl;
    bool encontrouConsulta = false;

    for (size_t i = 0; i < consultas.size(); ++i)
    {
        const CONSULTA &consulta = consultas[i];
        if (consulta.getdata() == dataConsulta)
        {
            cout << "Consulta " << (i + 1) << ": Paciente " << consulta.getpaciente() << " - Médico " << consulta.getmedico() << " - Hora " << consulta.getHoras() << ":" << consulta.getMinutos() << endl;
            encontrouConsulta = true;
        }
    }

    if (!encontrouConsulta)
    {
        cout << "Nenhuma consulta encontrada para esta data." << endl;
    }
}

void menuHorarios(const vector<CONSULTA> &consultas, const vector<MEDICO> &medicos, const vector<PACIENTE> &pacientes)
{
    int escolha;
    bool marcado = true;

    while (marcado)
    {
        cout << "<------------------------------->" << endl;
        cout << "\tMenu Horários" << endl;
        cout << "<------------------------------->" << endl;
        cout << "1. Consultas por Médico" << endl;
        cout << "2. Consultas por Paciente" << endl;
        cout << "3. Consultas por Data" << endl;
        cout << "4. Voltar ao Menu Principal" << endl;
        cout << "Escolha uma opção: ";
        cin >> escolha;

        switch (escolha)
        {
        case 1:
        {
            string nomeOuCodigoMedico;
            cout << "Digite o nome ou código do médico: ";
            cin >> nomeOuCodigoMedico;
            consultasPorMedico(consultas, medicos, nomeOuCodigoMedico);
            break;
        }
        case 2:
        {
            string nomeOuCodigoPaciente;
            cout << "Digite o nome ou código do paciente: ";
            cin >> nomeOuCodigoPaciente;
            consultasPorPaciente(consultas, pacientes, nomeOuCodigoPaciente);
            break;
        }
        case 3:
        {
            int dia, mes, ano;
            cout << "Digite a data (DD MM AAAA): ";
            cin >> dia >> mes >> ano;
            DATA dataConsulta(dia, mes, ano, 0, 0); // Supondo que a classe DATA tem um construtor apropriado
            consultasPorData(consultas, dataConsulta);
            break;
        }
        case 4:
            marcado = false;
            break;
        default:
            cout << "Opção inválida, por favor tente novamente." << endl;
            break;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void menu(string &ArquivoPaciente, vector<PACIENTE> LISTADEPACIENTE, string &ArquivoMEDICO, vector<MEDICO> LISTADEMEDICO, string &ArquivoCONSULTA, vector<CONSULTA> LISTADECONSULTA)
{

    int escolha;
    bool continuar = true;

    while (continuar)
    {
        cout << "<------------------------------->" << endl;
        cout << "\tMenu Principal" << endl;
        cout << "<------------------------------->" << endl;
        cout << "\t1. Consultas" << endl;
        cout << "\t2. Area de Cadastros" << endl;
        cout << "\t3. Quadro de Horarios" << endl;
        cout << "\t0. Sair" << endl;
        cout << "\tEscolha uma opcao:\n ";
        cin >> escolha;

        switch (escolha)
        {

        case 1:
            menuConsulta(ArquivoPaciente, LISTADEPACIENTE, ArquivoMEDICO, LISTADEMEDICO, ArquivoCONSULTA, LISTADECONSULTA);
            break;
        case 2:
            menuCadastro(ArquivoPaciente, LISTADEPACIENTE, ArquivoMEDICO, LISTADEMEDICO);
            break;
        case 3:
            menuHorarios(LISTADECONSULTA, LISTADEMEDICO, LISTADEPACIENTE);
            break;
        case 0:
            exit(1);
        default:
            cout << "Opção inválida, por favor tente novamente." << endl;

            break;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int main()
{

    string ArquivoPaciente = "pacientes.txt";
    string ArquivoMedico = "medico.txt";
    string ArquivoConsulta = "Consulta.txt";
    vector<MEDICO> listaMedico = inicializarMedico(ArquivoMedico);
    vector<PACIENTE> listaPacientes = inicializarPacientes(ArquivoPaciente);
    vector<CONSULTA> listaConsulta = inicializarConsulta(ArquivoConsulta);
    menu(ArquivoPaciente, listaPacientes, ArquivoMedico, listaMedico, ArquivoConsulta, listaConsulta);

    return 0;
}