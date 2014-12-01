// Control functions for taking data parsed by "options" and calling other functions, then writing status updates to output file.


////////////////////////////////////////////////////////////
// Function to write vector of strings to file.
// In main have data to be sent to GUI appended to a vector<string>
// Once all functions have added their status/ messages
// Call this function to write them to the output file.

int Control::write(std::vector<std::string> &lines, std::string targetFile)
{
    std::ofstream outfile;
    int i=0;
	
	struct stat buf;                                        // If there is already a file of this name, delete it.
    if( stat( targetFile.c_str(), &buf ) != -1) remove( targetFile.c_str() );
	
    outfile.open(targetFile.c_str(),std::ios::app);         // Open the file and write contents of vector to it.
	if(!outfile) return 1;	
    for (i=0; i < lines.size(); i++ ) outfile << lines[i] << std::endl;
	lines.clear();                                          // Clear the vector.
    outfile.close();
	return 0;
}//end funct write.


////////////////////////////////////////////////////////////
// Function to build string from pair of strings and append
// that string into vector<string>

int Control::addMsg(std::vector<std::string> &vec, std::string arg1, std::string arg2 )
{
  arg1.append(arg2);
  vec.push_back(arg1);
  return 0;
}// end add string function.

////////////////////////////////////////////////////////////
// Function to build string from a string and an int and append
// that string into vector<string>

int Control::addMsg(std::vector<std::string> &vec, std::string arg1, int arg2 )
{
  char tmp[16]={'\0'};
  sprintf(tmp,"%i",arg2);
  arg1.append(tmp);
  vec.push_back(arg1);
  return 0;
}// end add string function.

// For making longer messages such as " 0 ^ 9 ^ C:/programfiles/file.txt ^ 4 ^ 12 ^ 4 ^ 12 "
// I think we should make an additional unique addMsg function that takes the specific arguments and formats them.
// Then does the same functionality as addMsg adding that string to the vector.
