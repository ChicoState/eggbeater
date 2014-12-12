#include <eggbeater/Control.h>
#include <eggbeater/Discovery.h>
#include <eggbeater/Serial.h>

#include <sstream>
#include <iostream>
#include <iomanip>

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)

  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
  #endif

  #include <Windows.h>

  #define EGGBEATER_SLEEP(x) Sleep( x )

#else

  #include <unistd.h>

  #define EGGBEATER_SLEEP(x) usleep( ( x ) * 1000 )

#endif

#define PACKET_TIMEOUT 1000
#define DEVICE_VID ( 0x0483 )
#define DEVICE_PID ( 0x5740 )

using namespace std;

using namespace EggBeater;

#if defined(_EGGBEATER_DEBUG_) && (_EGGBEATER_DEBUG_ > 0)
  #define DEBUG_PRINT(expr) { expr; }
#else
  #define DEBUG_PRINT(...)
#endif

////////////////////////////////////////////////////////////
// Control functions for taking data parsed by "options" 
// and calling other functions, 
// then writing status updates to output file.


Control::Control( Options opt )
{
// copy all data into opt so that Control object and member functions have access to it.
    sessionID = opt.getSessionID();
    cliAction = opt.getAction();
    cipherMode = opt.getCipherMode();
    currentStatus = opt.getCurrentStatus();
    errorList = opt.getErrors();
    fileList = opt.getFileList();
    if (opt.hasGUIFile())
    {
      tmpFile = opt.getGUIFile();
    }
    else
    {
      // Sentinel to output via std::cout
      tmpFile = "-";
    }
    
}  // End Constructor

Control::~Control()                                      // Destructor
{
}

bool Control::run(void)
{
 // case statement for what action to do.
 // Also do error checking on opt data?
  //tmpFile = getTempPath();
  /*
  char tmp[120]={'\0'};
  int pathSize = getTempPath( sizeof(tmp),tmp);          // Get the windows tmp file path.
  if(pathSize < 1) return false;                         // Check that it worked.
  else tmp[pathSize] = '\0';                             // Make sure last char is a NULL.
  tmpFile = tmp; // */
  
  switch ( cliAction )
  {
    default:
    case CLI_Action::None:
      DEBUG_PRINT(std::cout << "No action set" << std::endl);
      return false;
      break;
    
    case CLI_Action::StartSession:
      // call start session
      DEBUG_PRINT(std::cout << "Start session" << std::endl);
      newSession();
      break;
    
    case CLI_Action::RefreshSession:
      // call refresh session
      DEBUG_PRINT(std::cout << "Refresh session" << std::endl);
      refreshSession();
      break;
    
    case CLI_Action::CloseSession:
      //call close session
      DEBUG_PRINT(std::cout << "Close session" << std::endl);
      closeSession();
      break;
    
    case CLI_Action::Encrypt:
      // call encrypt
      // need to loop through files in list.
      DEBUG_PRINT(std::cout << "Encrypt files" << std::endl);
      encryptFiles();
      break;
    
    case CLI_Action::Decrypt:
      // call decrypt.
      // need to loop through files in list.
      DEBUG_PRINT(std::cout << "Decrypt files" << std::endl);
      decryptFiles();
      break;
    
    case CLI_Action::DiscoverDevice:
      DEBUG_PRINT(std::cout << "Discover device" << std::endl);
      // call discover
      
//      devList = discover_devices(uint16_t vid, uint16_t pid); 
      break;
  }// end case.
  
  return true;
}// end run.

////////////////////////////////////////////////////////////
// Internal Function to get initialization vector (IV)
int Control::getIV()
{
  ByteArray hardIV      ({0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                          0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F});
  iv.resize(hardIV.size());
  
  for (unsigned int x = 0; x < hardIV.size(); x++)
    iv[x] = hardIV[x];
                      
  return iv.size();
}// End get IV


////////////////////////////////////////////////////////////  
// Internal function to get key value from st board.
int Control::getKey()
{
  // Try to discover device
  StringList devs = discover_devices(DEVICE_VID, DEVICE_PID);
  
  if (devs.empty())
  {
    addMsg(fileVec, "^!fatal", "Could not find attached device");
    writeVec(fileVec, tmpFile);
    return 0;
  }
  
  uint32_t id = 0;
  
  std::stringstream str(sessionID);
  str >> id;
  
  ByteArray buffer(4);
  *((uint32_t*)buffer.data()) = id;
  
  Packet pIn(CommandType::GetFileKey, buffer);
  Packet pOut;
  
  if (!sendPacket(devs.front(), pIn, pOut))
  {
    writeVec(fileVec, tmpFile);
    return 0;
  }
  
  if (pOut.isValid() != PacketError::None)
  {
    addMsg(fileVec, "^!fatal", "Serial device did not return a valid packet");
    writeVec(fileVec, tmpFile);
    return 0;
  }
  
  if (pOut.getDataLength() == 4)
  {
    uint32_t id = *(uint32_t*)pOut.getPacketData();
    
    std::stringstream str;
    str << "Error encountered: " << std::hex << id;
    
    addMsg(fileVec, "^!fatal", str.str());
    writeVec(fileVec, tmpFile);
    return 0;
  }
  
  if (pOut.getDataLength() < 32)
  {
    addMsg(fileVec, "^!fatal", "Serial device did not return enough data");
    writeVec(fileVec, tmpFile);
    return 0;
  }
  
  key.clear();
  
  const uint8_t* data = pOut.getPacketData();
  uint16_t index, limit = pOut.getDataLength();
  
  for (index = 0; index < limit; index++)
  {
    this->key.push_back(data[index]);
  }
  
  return this->key.size();
}// end getKey


////////////////////////////////////////////////////////////
// Return last status in tmpFile to the gui.

String Control::getStatus()
{
  addMsg(fileVec,"sessionID ", sessionID );
  addMsg(fileVec, currentStatus);
  Control::writeVec(fileVec, tmpFile);
  
  return "";
}

bool Control::sendPacket(String device, Packet& in)
{
  SerialCommunication comm(device);
  
  if (!comm.isOpen())
  {
    addMsg(fileVec, "^!fatal", "Could not connect to serial device");
    return false;
  }
  
  if (!comm.sendPacket(in))
  {
    addMsg(fileVec, "^!fatal", "Could not send packet to serial device");
    return false;
  }
  
  return true;
}

bool Control::sendPacket(String device, Packet& in, Packet& out)
{
  int ticks = 0;
  SerialCommunication comm(device);
  
  if (!comm.isOpen())
  {
    addMsg(fileVec, "^!fatal", "Could not connect to serial device");
    return false;
  }
  
  if (!comm.sendPacket(in))
  {
    addMsg(fileVec, "^!fatal", "Could not send packet to serial device");
    return false;
  }
  
  EGGBEATER_SLEEP(1000);
  
  while (ticks < PACKET_TIMEOUT)
  { 
    if (comm.haveData())
    {
      DEBUG_PRINT(std::cout << "have data" << std::endl);
      if (comm.receivePacket(out))
      {
        DEBUG_PRINT(std::cout << "Have packet" << std::endl);
        
        std::shared_ptr<ByteArray> ptr = out.getRawPacket();
        
        DEBUG_PRINT(std::cout << "packet.size: " << ptr->size() << std::endl);
        
        if (out.isValid() == PacketError::None)
        {
          DEBUG_PRINT(std::cout << "Packet is valid" << std::endl);
          break;
        }
        else
        {
          DEBUG_PRINT(std::cout << "Packet is valid - false (" << (uint32_t)(out.isValid()) << ")" << std::endl);
          
          auto d = out.getRawPacket();
          for (uint32_t i = 0; i < d->size(); i++)
          {
            DEBUG_PRINT(std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)d->at(i) << std::endl);
          }
        }
      }
    }
    else
    {
      DEBUG_PRINT(std::cout << "have data - false" << std::endl);
    }
    
    EGGBEATER_SLEEP(1000);
    ticks++;
  }
  
  if (out.isValid() != PacketError::None)
  {
    addMsg(fileVec, "^!fatal", "Serial device did not return a valid packet");
    writeVec(fileVec, tmpFile);
    return false;
  }
  
  return true;
}


////////////////////////////////////////////////////////////
// Start a new session with the micro controller.

void Control::newSession()
{
  // Try to discover device
  StringList devs = discover_devices(DEVICE_VID, DEVICE_PID);
  
  if (devs.empty())
  {
    addMsg(fileVec, "^!fatal", "Could not find attached device");
    writeVec(fileVec, tmpFile);
    return;
  }
  
  String device = devs.front();
  DEBUG_PRINT(std::cout << "COM device: " << device << std::endl);
  ByteArray buffer;
  Packet in(CommandType::NewSession, buffer);
  Packet out;
  
  if (!this->sendPacket(device, in, out))
  {
    //addMsg(fileVec, "^!fatal", "Could not transmit packet to
    return;
  }
  
  if (out.getDataLength() < 4)
  {
    addMsg(fileVec, "^!fatal", "Serial device did not return enough data");
    writeVec(fileVec, tmpFile);
    return;
  }
  
  uint32_t id = *(uint32_t*)(out.getPacketData());
  
  std::stringstream str;
  str << id;
  
  sessionID = str.str();

  addMsg(fileVec, "sessionID ", sessionID );
  addMsg(fileVec, "^!done ");
  
  writeVec(fileVec, tmpFile);
}

////////////////////////////////////////////////////////////
// Open a previously existing session with the micro controller.

void Control::openSession()
{
  // Get session ID from board.
  // Authenticate.
}

////////////////////////////////////////////////////////////
// Member function to refresh session with micro controller.

void Control::refreshSession()
{
  // Try to discover device
  StringList devs = discover_devices(DEVICE_VID, DEVICE_PID);
  
  if (devs.empty())
  {
    addMsg(fileVec, "^!fatal", "Could not find attached device");
    writeVec(fileVec, tmpFile);
    return;
  }
  
  uint32_t id = 0;
  if (sessionID == "")
  {
    addMsg(fileVec, "^!fatal", "You did not specify a session ID");
    writeVec(fileVec, tmpFile);
    return;
  }
  
  std::stringstream str(sessionID);
  str >> id;
  
  ByteArray buffer(4);
  *((uint32_t*)buffer.data()) = id;
  
  Packet pIn(CommandType::RefreshSession, buffer);
  Packet pOut;
  
  if (!sendPacket(devs.front(), pIn, pOut))
  {
    writeVec(fileVec, tmpFile);
    return;
  }
  
  if (pOut.isValid() != PacketError::None)
  {
    addMsg(fileVec, "^!fatal", "Serial device did not return a valid packet");
    writeVec(fileVec, tmpFile);
    return;
  }
  
  addMsg(fileVec, "sessionID ", sessionID );
  addMsg(fileVec, "^!done ");
  
  writeVec(fileVec, tmpFile);
}

////////////////////////////////////////////////////////////
// Close the current session.

void Control::closeSession()
{
  // Try to discover device
  StringList devs = discover_devices(DEVICE_VID, DEVICE_PID);
  
  if (devs.empty())
  {
    addMsg(fileVec, "^!fatal", "Could not find attached device");
    writeVec(fileVec, tmpFile);
    return;
  }
  
  uint32_t id = 0;
  if (sessionID == "")
  {
    addMsg(fileVec, "^!fatal", "You did not specify a session ID");
    writeVec(fileVec, tmpFile);
    return;
  }
  
  std::stringstream str(sessionID);
  str >> id;
  
  ByteArray buffer(4);
  *((uint32_t*)buffer.data()) = id;
  
  Packet pIn(CommandType::CloseSession, buffer);
  
  if (!sendPacket(devs.front(), pIn))
  {
    writeVec(fileVec, tmpFile);
    return;
  }

  addMsg(fileVec, "sessionID ", sessionID );
  addMsg(fileVec, "^!done ");
  
  writeVec(fileVec, tmpFile);
}


////////////////////////////////////////////////////////////
// Encrypt a file.

int Control::encryptFiles()
{
  DEBUG_PRINT(std::cout << "GetIV" << std::endl);
  if( !getIV() )
  {
    addMsg(fileVec, "sessionID ", sessionID);
    addMsg(fileVec, "^!fatal", "Could not get valid IV.");
    writeVec( fileVec, tmpFile);
    return 1;
  }
  DEBUG_PRINT(std::cout << "GetKey" << std::endl);
  if( !getKey() )
  {
    addMsg(fileVec, "sessionID ", sessionID);
    addMsg(fileVec, "^!fatal", "Could not get valid Key.");
    writeVec( fileVec, tmpFile);
    return 1;
  }

  DEBUG_PRINT(std::cout << "Start crypto" << std::endl);
  Crypto myCrypt;
  DEBUG_PRINT(std::cout << "set cipher mode" << std::endl);
  myCrypt.setCipherMode(cipherMode);
  DEBUG_PRINT(std::cout << "set key" << std::endl);
  myCrypt.setEncryptionKey(key);
  DEBUG_PRINT(std::cout << "set IV" << std::endl);
  myCrypt.setInitialVector(iv);
  
  for (auto iter : fileList)
  {
    DEBUG_PRINT(std::cout << ".encryptFile(" << iter << ")" << std::endl);
    myCrypt.encryptFile(iter, iter + ".egg");
  }
  
  //DEBUG_PRINT(std::cout << ".encryptFile(" << oFile << ")" << std::endl);
  //myCrypt.encryptFile(oFile, oFile.append(".egg"));

  DEBUG_PRINT(std::cout << "write message" << std::endl);
  addMsg(fileVec, "sessionID ", sessionID);
  addMsg( fileVec, currentStatus );
  writeVec( fileVec, tmpFile);
  return 0;
}// end Encrypt files.



////////////////////////////////////////////////////////////
// Decrypt a file.

int Control::decryptFiles()
{
  DEBUG_PRINT(std::cout << "GetIV" << std::endl);
  if( !getIV() )
  {
    addMsg(fileVec, "sessionID ", sessionID);
    addMsg(fileVec, "^!fatal", "Could not get valid IV.");
    writeVec( fileVec, tmpFile);
    return 1;
  }
  DEBUG_PRINT(std::cout << "GetKey" << std::endl);
  if( !getKey() )
  {
    addMsg(fileVec, "sessionID ", sessionID);
    addMsg(fileVec, "^!fatal", "Could not get valid Key.");
    writeVec( fileVec, tmpFile);
    return 1;
  }

  DEBUG_PRINT(std::cout << "Start crypto" << std::endl);
  Crypto myCrypt;
  DEBUG_PRINT(std::cout << "set cipher mode" << std::endl);
  myCrypt.setCipherMode(cipherMode);
  DEBUG_PRINT(std::cout << "set key" << std::endl);
  myCrypt.setEncryptionKey(key);
  DEBUG_PRINT(std::cout << "set IV" << std::endl);
  myCrypt.setInitialVector(iv);
  
  for (auto iter : fileList)
  {
    DEBUG_PRINT(std::cout << ".decryptFile(" << iter << ")" << std::endl);
    myCrypt.decryptFile(iter, iter.substr(iter.length() - 4));
  }
  
  //DEBUG_PRINT(std::cout << ".encryptFile(" << oFile << ")" << std::endl);
  //myCrypt.encryptFile(oFile, oFile.append(".egg"));

  DEBUG_PRINT(std::cout << "write message" << std::endl);
  addMsg(fileVec, "sessionID ", sessionID);
  addMsg( fileVec, currentStatus );
  writeVec( fileVec, tmpFile);
  return 0;
}// End decryption function.



////////////////////////////////////////////////////////////
// Function to write vector of strings to file.
// In main have data to be sent to GUI appended to a vector<string>
// Once all functions have added their status/messages
// Call this function to write them to the output file.

int Control::writeVec(std::vector<std::string> &lines, std::string targetFile)
{
  if(&lines == NULL) return 1;
  std::ofstream outfile;
  unsigned int i=0;
  
  if (targetFile == "" || targetFile == "-")
  {
    for (auto line : lines)
      std::cout << line << std::endl;
    
    lines.clear();
  }
  else
  {
    struct stat buf;                                          // If there is already a file of this name, delete it.
    if( stat( targetFile.c_str(), &buf ) != -1) remove( targetFile.c_str() );
    
    outfile.open(targetFile.c_str(),std::ios::app);         // Open the file and write contents of vector to it.
    if(!outfile)
    {
      std::cout << "Could not open temporary file at: " << tmpFile << std::endl;
      return 1;  
    }
    for (i=0; i < lines.size(); i++ ) outfile << lines[i] << std::endl;
    lines.clear();                                            // Clear the vector.
    outfile.close();
  }
  
  return 0;
}//end funct write.


////////////////////////////////////////////////////////////
// Function to build string from pair of strings and append
// that string into vector<string>
// example call could be addMsg("^!error ", errorMessage[i] );
// or addMsg("^!error ", "No session ID given!" );

int Control::addMsg(std::vector<std::string> &vec, std::string arg1, std::string arg2 )
{
  if(&vec == NULL) return 1;
  arg1.append(" ");
  arg1.append(arg2);
  vec.push_back(arg1);
  return 0;
}// end add string function.

////////////////////////////////////////////////////////////
// Function to build string from a string and an int and append
// that string into vector<string>

int Control::addMsg(std::vector<std::string> &vec, std::string arg1, int arg2 )
{
  if(&vec == NULL) return 1;
  std::stringstream s;
  
  s << arg1 << " " << arg2;
  
  vec.push_back(s.str());
  /*
  char tmp[16]={'\0'};
  sprintf(tmp,"%i",arg2);
  arg1.append(tmp);
  vec.push_back(arg1); // */
  return 0;
}// end add string function.


////////////////////////////////////////////////////////////
// addMsg function to add message to vector<string> to output to file.
/* Notes on data to parse.
  struct Status_t
  {
    uint32_t CurrentBlocksDone,
             CurrentBlocksTotal,
             OverallFilesDone,
             OverallFilesTotal,
             OverallBlocksDone,
             OverallBlocksTotal;
    std::string CurrentPath;
  };
*/

int Control::addMsg( std::vector<std::string> &vec, Status_t status )
{ 

  if(&vec == NULL) return 1;
  const int tmpSize=16;
  /*
  String ans="status ";
  char tmp[tmpSize]={'\0'};
  
  sprintf(tmp,"%i",status.OverallFilesDone);
  ans.append(tmp);
  
  memset(tmp,0,tmpSize);
  sprintf(tmp," ^ %i ^ ",status.OverallFilesTotal);
  ans.append(tmp);
  
  ans.append(status.CurrentPath);
  
  memset(tmp,0,tmpSize);
  sprintf(tmp," ^ %i ",status.CurrentBlocksDone);
  ans.append(tmp);
  
  memset(tmp,0,tmpSize);
  sprintf(tmp," ^ %i ",status.CurrentBlocksTotal);
  ans.append(tmp);
  
  memset(tmp,0,tmpSize);
  sprintf(tmp," ^ %i ",status.OverallBlocksDone);
  ans.append(tmp);
  
  memset(tmp,0,tmpSize);
  sprintf(tmp," ^ %i ",status.OverallBlocksTotal);
  ans.append(tmp); // */
  
  std::stringstream ans;
  
  ans << "status " << status.OverallFilesDone
      << " ^ "  << status.OverallFilesTotal
      << " ^ "  << status.CurrentBlocksDone
      << " ^ "  << status.CurrentPath
      << " ^ "  << status.CurrentBlocksTotal
      << " ^ "  << status.OverallBlocksDone
      << " ^ "  << status.OverallBlocksTotal;
  
  vec.push_back(ans.str());
  return 0;
}// end add string function.

/*
// Notes on file format from meeting..
Data Flags used in output file:
  ^!error
  ^!fatal
        File Contents:
          --start_session (decimal)
          --refresh_session (1/0)
          --encrypt -running status 
                        (everyline)
                          # overall(files done), # total, current(path, blocks done, blocks total), overall(blocks done, blocks total)
          --decrypt -running status 
                        (everyline)
                          # overall(files done), # total, current(path, blocks done, blocks total), overall(blocks done, blocks total)
          --discovery (1/0)
          --Close_session (done)
EX file-encrypt:   0 ^ 9 ^ C:/programfiles/file.txt ^ 4 ^ 12 ^ 4 ^ 12
EX file-error:   ^!error ----------------
EX file-close:   ^!done

// New name tags to use.
SessionID 3XXXX
^!error message
^!fatal message
^!done
status 7 ^ 10 ^ "blahasdl..."
refresh 1 or 0
discovered  1 or 0
close 1 or 0

*/


