// Importamos o header referente a lib FS_File_Record
#include "FS_File_Record.h"

String fileName; // Nome do arquivo
int sizeOfRecord = 5; // Default 3 (somando \r\n o tamanho será 5)
File pFile; // Ponteiro do arquivo

// Construtor que seta somente o nome do arquivo, deixando o tamanho de registro default 3
FS_File_Record::FS_File_Record(String _fileName)
{
  fileName = _fileName;
}

// Construtor que seta nome do arquivo e tamanho de registro +2 (\r\n)
FS_File_Record::FS_File_Record(String _fileName, int _sizeOfRecord)
{
  fileName = _fileName;
  sizeOfRecord = _sizeOfRecord+2;
}

// Inicializa SPIFFS
bool FS_File_Record::init()
{    
  return SPIFFS.begin(true); 
}

// Posiciona o ponteiro do arquivo no início
bool FS_File_Record::rewind()
{
  if(pFile)
    return pFile.seek(0);

  return false;
}

// Lê a próxima linha do arquivo
bool FS_File_Record::readFileNextRecord(String *line, String *errorMsg)
{  
  *errorMsg = "";
  *line = "";
  // Se o ponteiro estiver nulo  
  if(!pFile)
  {
    // Abre arquivo para leitura
    pFile = SPIFFS.open(fileName.c_str(), FILE_READ);

    // Se aconteceu algum erro 
    if(!pFile)
    {
      // Guarda msg de erro
      *errorMsg = "Failed to open the file";
      // Retorna falso
      return false;  
    }
  }

  // Se for possível ler o arquivo
  if(pFile.available())
  {
    // Lê arquivo
    *line = pFile.readStringUntil('\n');
    // Retorna true
    return true;
  }

  // Se o arquivo estiver vazio retorna true
  if(pFile.size()<=0)
    return true;

  // Se não for possível ler o arquivo retorna falso    
  return false;
}

//Posiciona ponteiro do arquivo na posição "pos"
bool FS_File_Record::seekFile(int pos)
{
  // Se o ponteiro estiver nulo  
  if(pFile)
    pFile.close();

  pFile = SPIFFS.open(fileName.c_str(), FILE_READ); // Abre o arquivo para leitura

  // Posiciona o ponteiro na posição multiplicando pelo tamanho do registro
  return pFile.seek(sizeOfRecord*pos);  
}

// Escreve no arquivo
bool FS_File_Record::writeFile(String line, String *errorMsg)
{
  if(pFile)
    pFile.close();

  pFile = SPIFFS.open(fileName.c_str(), FILE_APPEND);

  // Se foi possível abrir
  if (pFile) 
  {
    // Escreve registro
    pFile.println(line);
    // Fecha arquivo
    pFile.close();
    // Retorna true
    return true;
  }
  
  // Se não foi possível abrir guarda mensagem de erro e retorna false
  *errorMsg = "Failed to open the file: "+String(fileName);  
  return false;  
}

// Lê o último registro do arquivo
bool FS_File_Record::readFileLastRecord(String *line, String *errorMsg)
{
  // Variável que guardará o tamanho do arquivo
  int sizeArq;

  // Limpa string
  *errorMsg = "";

  // Se o arquivo está aberto, fecha
  if(pFile)
    pFile.close();

  // Abre o arquivo para leitura
  pFile = SPIFFS.open(fileName.c_str(), FILE_READ);

  // Se não foi possível abrir o arquivo
  if(!pFile)
  {
    // Guarda mensagem de erro e retorna false
    *errorMsg = "Failed to open the file: "+String(fileName);
    return false;
  }
    
  // Obtém o tamanho do arquivo
  sizeArq = pFile.size();
  Serial.println("Size: "+String(sizeArq));

  // Se existe ao menos um registro
  if(sizeArq >= sizeOfRecord)
  {
    pFile.seek(sizeArq-sizeOfRecord); // Posiciona o ponteiro no final do arquivo menos o tamanho de um registro (sizeOfRecord)
    *line =  pFile.readStringUntil('\n');
    pFile.close();
  }
  
  return true;  
}

// Exclui arquivo
bool FS_File_Record::destroyFile()
{
  // Se o arquivo estiver aberto, fecha
  if(pFile)
    pFile.close();

  // Exclui arquivo e retorna o resultado da função "remove"  
  return SPIFFS.remove((char*)fileName.c_str());
}

// Função que busca um registro
// "pos" é a posição referente ao registro buscado
String FS_File_Record::findRecord(int pos) 
{
  // Linha que receberá o valor do registro buscado
  String line = "", errorMsg = "";

  // Posiciona na posição desejada
  // Obs. A posição se inicia com zero "0" 
  if(!seekFile(pos))
    return "Seek error"; 
    
  // Lê o registro
  if(!readFileNextRecord(&line, &errorMsg))
    return errorMsg;

  return line;    
}

// Verifica se o arquivo existe
bool FS_File_Record::fileExists()
{
  return SPIFFS.exists((char*)fileName.c_str());
}

// Cria um novo arquivo, se já existir um arquivo de mesmo nome, ele será removido antes
void FS_File_Record::newFile()
{
  if(pFile)
    pFile.close();

  SPIFFS.remove((char*)fileName.c_str());
  pFile = SPIFFS.open(fileName.c_str(), FILE_WRITE);
  pFile.close();
}

// Obtém o nome do arquivo
String FS_File_Record::getFileName()
{  return fileName; }

// Seta o nome do arquivo
void FS_File_Record::setFileName(String _fileName)
{  fileName = _fileName; }

// Obtém o tamanho do registro
int FS_File_Record::getSizeRecord()
{ return sizeOfRecord-2; }

// Seta o tamanho do registro
void FS_File_Record::setSizeRecord(int _sizeOfRecord)
{ sizeOfRecord = _sizeOfRecord+2; }

// Verifica se existe espaço suficiente na memória flash
bool FS_File_Record::availableSpace()
{ 
  return getUsedSpace() + sizeOfRecord <= getTotalSpace();
  // ou também pode ser:
  //return getUsedSpace()+(sizeof(char)*sizeOfRecord) <= getTotalSpace();  // sizeof(char) = 1
}

// Retorna o tamanho em bytes total da memória flash
int FS_File_Record::getTotalSpace()
{  
  return SPIFFS.totalBytes();
}

// Retorna a quantidade usada de memória flash
int FS_File_Record::getUsedSpace()
{
  return SPIFFS.usedBytes();
}

