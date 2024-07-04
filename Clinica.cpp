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
class PACIENTE

{
private:
    string nome;
    DATA data;
    int codigo;

public:
    PACIENTE(string n, DATA date, int cod) : nome(n), data(date), codigo(cod) {}

    string getNome() const { return nome; }
    DATA getData() const { return data; }
    int getCodigo() const { return codigo; }
};

class MEDICO

{
private:
    string nome;
    string especialidade;
    int codigo;

public:
    MEDICO(string n, string esp, int cod) : nome(n), especialidade(esp), codigo(cod) {}

    string getNome() const { return nome; }
    string getespecialidade() const { return especialidade; }
    int getCodigo() const { return codigo; }
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

void cadastraMEDICO(vector<MEDICO> &lista, string nome, string nometxt, string especialidade)
{
    int cod = lista.size() + 1;
    MEDICO novo(nome, especialidade, cod);
    lista.push_back(novo);

    ofstream anota(nometxt, ios::app);
    if (!anota)
    {
        cout << "Nao foi possivel cadastrar esse medico" << endl;
    }
    else
    {
        for (const auto &paci : lista)
        {
            anota << paci.getNome() << " - " << paci.getespecialidade() << " - " << paci.getCodigo() << endl;
        }

        cout << "Medico Cadastrado com Sucesso" << endl
             << "Nome: " << nome << endl
             << "Especialidade: " << especialidade << endl
             << "Codigo: " << cod << endl;
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
    if (!anota)
    {
        cout << "Nao foi possivel criar o arquivo: " << nometxt << endl;
    }
    else
    {
        for (const auto &paci : lista)
        {
            anota << paci.getNome() << " - " << paci.getespecialidade() << " - " << paci.getCodigo() << endl;
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
    if (nomeEstaNoMedico(lista, nome))
    {
        descadastramedico(lista, nometxt, nome);
        cout << "Qual nome deseja manter" << endl;
        cin >> newnome;
        cout << "Insira a nova especialidade" << endl;
        cin >> newespecialidade;
        cadastraMEDICO(lista, newnome, nometxt, newespecialidade);
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
        cout << "Especialidade: " << medicos.getespecialidade() << endl;
        cout << "Codigo: " << medicos.getCodigo() << endl
             << endl;
    }
}

vector<MEDICO> inicializarMedico(const string &nomeArquivo)
{
    vector<MEDICO> medico;
    ifstream arquivo(nomeArquivo);

    if (!arquivo.is_open())
    {
        cerr << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
        return medico; // Retorna vetor vazio se houver erro ao abrir o arquivo
    }

    string linha;
    while (getline(arquivo, linha))
    {
        stringstream ss(linha);
        string nome, especialidade;
        int codigo;

        // Extrai nome, data e código da linha
        getline(ss, nome, '-');
        getline(ss, especialidade, '-');
        ss >> codigo;

        // Cria um objeto PACIENTE e adiciona ao vetor
        medico.push_back(MEDICO(nome, especialidade, codigo));
    }

    arquivo.close();
    return medico;
}

void cadastraPaciente(vector<PACIENTE> &lista, string nome, DATA date, string nometxt)
{
    int cod = lista.size() + 1;
    PACIENTE novo(nome, date, cod);
    lista.push_back(novo);

    ofstream anota(nometxt, ios::app);
    if (!anota)
    {
        cout << "Nao foi possivel cadastrar esse paciente" << endl;
    }
    else
    {
        for (const auto &paci : lista)
        {
            anota << paci.getNome() << " - " << paci.getData().toString() << " - " << paci.getCodigo() << endl;
        }

        cout << "Paciente Cadastrado com Sucesso" << endl
             << "Nome: " << nome << endl
             << "Nascimento: " << date.getdia() << "/" << date.getmes() << "/" << date.getano() << endl
             << "Codigo: " << cod << endl;
    }
    anota.close();
}

void descadastraPaciente(vector<PACIENTE> &lista, string nometxt, string nome)
{
    // Remover o arquivo antigo
    if (remove(nometxt.c_str()) != 0)
    {
        cerr << "Erro ao excluir o arquivo: " << nometxt << endl;
    }

    // Remover o paciente do vetor
    lista.erase(remove_if(lista.begin(), lista.end(),
                          [nome](const PACIENTE &p)
                          { return p.getNome() == nome; }),
                lista.end());

    // Criar um novo arquivo com os pacientes restantes
    ofstream anota(nometxt, ios::app);
    if (!anota)
    {
        cout << "Nao foi possivel criar o arquivo: " << nometxt << endl;
    }
    else
    {
        for (const auto &paci : lista)
        {
            anota << paci.getNome() << " - " << paci.getData().toString() << " - " << paci.getCodigo() << endl;
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

    string newnome;
    DATA newdata(0, 0, 0);
    if (nomeEstaNoVector(lista, nome))
    {
        descadastraPaciente(lista, nometxt, nome);
        cout << "Qual nome deseja manter" << endl;
        cin >> newnome;
        cout << "Insira a nova data (dia mes ano)" << endl;
        cin >> newdata;
        cadastraPaciente(lista, newnome, newdata, nometxt);
        cout << "Paciente editado com sucesso." << endl;
    }
    else
        cout << "Paciente procurado nao esta cadastrado" << endl;
}

void ListarPacientes(const vector<PACIENTE> &lis)
{
    cout << "----LISTA DE PACIENTES----" << endl;

    for (const auto &pacientes : lis)
    {
        cout << "Nome: " << pacientes.getNome() << endl;
        cout << "Data: " << pacientes.getData().getdia() << "/" << pacientes.getData().getmes() << "/" << pacientes.getData().getano() << endl;
        cout << "Codigo: " << pacientes.getCodigo() << endl
             << endl;
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
        string nome, dataStr;
        int codigo;

        // Extrai nome, data e código da linha
        getline(ss, nome, '-');
        getline(ss, dataStr, '-');
        ss >> codigo;

        // Converte a string de data para um objeto DATA
        stringstream ssData(dataStr);
        int dia, mes, ano;
        char delim;
        ssData >> dia >> delim >> mes >> delim >> ano;
        DATA data(dia, mes, ano);

        // Cria um objeto PACIENTE e adiciona ao vetor
        pacientes.push_back(PACIENTE(nome, data, codigo));
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
            cout << "Informe o nome do paciente: " << endl;
            getline(cin, nome);
            cout << "Informe a data de nascimento (dd mm aaaa): " << endl;
            cin >> dia >> mes >> ano;
            DATA date(dia, mes, ano);
            cadastraPaciente(Listadepaciente, nome, date, ArquivoPaciente);

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
            string nome, especialidade;
            cout << "Informe o nome do medico: " << endl;
            getline(cin, nome);
            cout << "Informe a especialidade: " << endl;
            cin >> especialidade;
            cadastraMEDICO(Listademedicos, nome, ArquivoMedico, especialidade);

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