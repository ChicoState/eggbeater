////////////////////////////////////////////////////////////
// Control functions for taking data parsed by "options" 
// and calling other functions, 
// then writing status updates to output file.


Control::Control( Options opt ) {
// copy all data into opt so that Control object and member functions have access to it.
    sessionID = opt.getSessionID();
    cliAction = opt.getAction();
    cipherMode = opt.getCipherMode();
    currentStatus = opt.getStatus();
    errorList = opt.getErrors();
    fileList = opt.getFileList();
    
}  // Constructor

~Control::Control()               // Destructor
{
}

bool Control::run(void){
// case statement for what action to do.
// Also do error checking on opt data?
  switch ( cliAction )
  {
    default:
    case CLI_Action::None:
      return False;
      break;
    
    case CLI_Action::StartSession:
      // call start session
      newSession();
      break;
    
    case CLI_Action::RefreshSession:
      // call refresh session
      refreshSession();
      break;
    
    case CLI_Action::CloseSession:
      //call close session
      closeSession();
      break;
    
    case CLI_Action::Encrypt:
      //call encrypt
      encryptFiles();
      break;
    
    case CLI_Action::Decrypt:
      // call decrypt
      decryptFiles();
      break;
    
    case CLI_Action::DiscoverDevice:
      // call discover
      break;
  }// end case.

}// end run.
String Control::getStatus(){
  addMsg(fileVec, "Status=",opt.getCurrentStatus();
  Control::writeVec(fileVec, tmpFile);
}
void Control::newSession(){
  addMsg(fileVec, "SessionID=", sessionID );
  addMsg(fileVec, "Action=", cliAction );
  Control::writeVec(fileVec, tmpFile);
}
//void Control::openSession(){}
//void Control::refreshSession(){}
//void Control::closeSession(){}
int Control::encryptFiles(string encMode, string oFile, vector<uint8_t> pwordKey, vector<uint8_t> ivec){

  if( (pwordKey.size() == CryptoPP::AES::MAX_KEYLENGTH) && (ivec.size() == CryptoPP::AES::BLOCKSIZE) )
  {
    //pulls vectors into byte and initilization vectors
    copy(pwordKey.begin(), pwordKey.end(), key);
    copy(ivec.begin(), ivec.end(), iv);

    //Adds .egg to siginifiy that the file is encrypted by eggbeater
    string ofilename = oFile;
    string outFile = oFile + ".egg";
    string efilename = outFile;   

    // creates a copy of the input file
    // and converts it to binary for encryption.
    // Helps avoid the creation of corrupted files
    // durring decryption process
    ifstream ifile(oFile.c_str(), ios::binary);
    ifstream::pos_type size = ifile.seekg(0, ios_base::end).tellg();
    ifile.seekg(0, ios_base::beg);

    string temp;
    temp.resize(size);
    ifile.read((char*)temp.data(), temp.size());

    if(encMode == "OFB")
    {
      OFB_Mode< AES >::Encryption e1;
      e1.SetKeyWithIV( key, sizeof(key), iv, sizeof(iv) );
     
      StringSource( temp, true,
                      new StreamTransformationFilter( e1,
                      new FileSink( efilename.c_str() ))); 
    }
    else if(encMode == "CFB")
    {
      CFB_Mode< AES >::Encryption e1;
      e1.SetKeyWithIV( key, sizeof(key), iv, sizeof(iv) );

      StringSource( temp, true,
                      new StreamTransformationFilter( e1,
                      new FileSink( efilename.c_str() ))); 
    }
    else if(encMode == "GCM")
    {
      GCM< AES >::Encryption e1;
      e1.SetKeyWithIV( key, sizeof(key), iv, sizeof(iv) );
     
      StringSource ss1( temp, true,
                      new AuthenticatedEncryptionFilter( e1,
                      new FileSink( efilename.c_str() )));
    }
    else
    {
      cerr << "[ERROR] Invalid Mode" <<endl;
      return 1;
    }

    return 0;
  }
  else
  {
    cerr<<"[ERROR] Key size is insufficient"<<endl;
    return 1;
  }
}
int Control::decryptFiles(string decMode, string efile, vector<uint8_t> pwordKey, vector<uint8_t> ivec){

  //Allows encryption if input Vectors are of the required size
  if( (pwordKey.size() == CryptoPP::AES::MAX_KEYLENGTH) && (ivec.size() == CryptoPP::AES::BLOCKSIZE) )
  {
    //Checks if the ending of the provided filename
    //is an eggbeater encrypted file, otherwise decryption
    //will halt.
    if(!efile.compare(efile.size() - 4, 4, ".egg") == 0)
    {
       cerr<<" [ERROR] The File provided is not an Eggbeater encrypted file \n";
       cerr<<"         Please choose a .egg file to decrypt \n";
       return 1;
    }

    //Removes the .egg from the file type provided
    string efilename = efile;
    efile.erase(efile.end()-4, efile.end());
    string rfilename = efile;

    copy(pwordKey.begin(), pwordKey.end()-1, key);
    copy(ivec.begin(), ivec.end()-1, iv);

    if(decMode == "OFB")
    {
      OFB_Mode< AES >::Decryption d2;
      d2.SetKeyWithIV( key, sizeof(key), iv, sizeof(iv) );
     
      FileSource( efilename.c_str(), true,
                      new StreamTransformationFilter( d2,
                      new FileSink( rfilename.c_str() ))); 
    }
    else if(decMode == "CFB")
    {
      CFB_Mode< AES >::Decryption d2;
      d2.SetKeyWithIV( key, sizeof(key), iv, sizeof(iv) );
     
      FileSource( efilename.c_str(), true,
                      new StreamTransformationFilter( d2,
                      new FileSink( rfilename.c_str() )));
    }
    else if(decMode == "GCM")
    {
      GCM< AES >::Decryption d2;
      d2.SetKeyWithIV( key, sizeof(key), iv, sizeof(iv) );
     
      FileSource fs2( efilename.c_str(), true,
                      new AuthenticatedDecryptionFilter( d2,
                      new FileSink( rfilename.c_str() ),
                      AuthenticatedDecryptionFilter::THROW_EXCEPTION));
    }
    else
    {
      cerr << " [ERROR] Decryption Error" <<endl;
      return 1;
    }

    return 0;
  }
  else
  {
    cerr<<" [ERROR] Key size is insufficient"<<endl;
    return 1;
  }

}


////////////////////////////////////////////////////////////
// Function to write vector of strings to file.
// In main have data to be sent to GUI appended to a vector<string>
// Once all functions have added their status/messages
// Call this function to write them to the output file.

int Control::writeVec(std::vector<std::string> &lines, std::string targetFile)
{
  if(vec == NULL) return 1;
  std::ofstream outfile;
  int i=0;
	
	struct stat buf;                                          // If there is already a file of this name, delete it.
    if( stat( targetFile.c_str(), &buf ) != -1) remove( targetFile.c_str() );
	
    outfile.open(targetFile.c_str(),std::ios::app);         // Open the file and write contents of vector to it.
	if(!outfile) return 1;	
    for (i=0; i < lines.size(); i++ ) outfile << lines[i] << std::endl;
	lines.clear();                                            // Clear the vector.
    outfile.close();
	return 0;
}//end funct write.


////////////////////////////////////////////////////////////
// Function to build string from pair of strings and append
// that string into vector<string>
// example call could be addMsg("^!error ", errorMessage[i] );
// or addMsg("^!error ", "No session ID given!" );

int Control::addMsg(std::vector<std::string> &vec, std::string arg1, std::string arg2 )
{
  if(vec == NULL) return 1;
  arg1.append(arg2);
  vec.push_back(arg1);
  return 0;
}// end add string function.

////////////////////////////////////////////////////////////
// Function to build string from a string and an int and append
// that string into vector<string>

int Control::addMsg(std::vector<std::string> &vec, std::string arg1, int arg2 )
{
  if(vec == NULL) return 1;
  char tmp[16]={'\0'};
  sprintf(tmp,"%i",arg2);
  arg1.append(tmp);
  vec.push_back(arg1);
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

*/


////////////////////////////////////////////////////////////
// addMsg function to add message to vector<string> to output to file.
/*
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


int Control::addMsg( Status_t status )
{ 

  if(vec == NULL) return 1;
  const int tmpSize=16;
  string ans="\0";
  char tmp[tmpSize]={'\0'};
  
  sprintf(tmp,"%i",status.OverallFilesDone);
  ans.append(tmp);
  
  memset(tmp,0,tmpSize);
  sprintf(tmp," ^ %i ^ ",stutus.OverallFilesTotal);
  ans.append(tmp);
  
  ans.append(path);
  
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
  ans.append(tmp);
  
  vec.push_back(ans);
  return 0;
}// end add string function.



