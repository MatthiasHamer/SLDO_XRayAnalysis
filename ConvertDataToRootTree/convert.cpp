#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>
#include <TFile.h>
#include <TTree.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char **argv) {

  char fn_lineReg[200];
  char fn_loadReg[200];
  if( argc <=2 ) {
    sprintf( fn_lineReg, "../data/LineRegulation_%s.csv", argv[1] );
    sprintf( fn_loadReg, "../data/LoadRegulation_%s.csv", argv[1] );
  }
  else {
    sprintf( fn_lineReg, "%s/LineRegulation_%s.csv", argv[2], argv[1] );
    sprintf( fn_loadReg, "%s/LoadRegulation_%s.csv", argv[2], argv[1] );
  }
  
  ifstream f_lineReg( fn_lineReg, ios::in );
  ifstream f_loadReg( fn_loadReg, ios::in );

  char filename[200];
  sprintf( filename, "ntuple_%s.root", argv[1] );
  TFile *f = new TFile( filename, "RECREATE" );
  TTree *tLineReg = new TTree("LineRegulation", "LineRegulation" );
  TTree *tLoadReg = new TTree("LoadRegulation", "LoadRegulation" );
  
  double line_vin = 0.;
  double line_iin = 0.;
  double line_vout1 = 0.;
  double line_vout2 = 0.;

  double load_vin = 0.;
  double load_iin = 0.;
  double load_vout1 = 0.;
  double load_vout2 = 0.;
  double load_iload = 0.;
  double load_iout = 0.;
  
  string sdose( argv[1] );
  size_t cut = sdose.find("krad");
  if( cut == std::string::npos ) cut = sdose.find("Mrad");

  double dose = atof( sdose.substr( 0, cut ).c_str() );
  cout << "dose is " << dose << endl;
  if( sdose.substr( cut, sdose.length() - cut ) == "krad" ) { dose *= 1.e3; cout << "multiplying dose by 1.e3" << endl; }
  else if( sdose.substr( cut, sdose.length() - cut ) == "Mrad" ) { dose *= 1.e6; cout << "multiplying dose by 1.e6" << endl; }
  else cout << "unknown conversion factor" << endl;


  tLineReg->Branch("Dose", &dose );
  tLineReg->Branch("V_in", &line_vin );
  tLineReg->Branch("I_in", &line_iin );
  tLineReg->Branch("V_out1", &line_vout1 );
  tLineReg->Branch("V_out2", &line_vout2 );

  tLoadReg->Branch("Dose", &dose );
  tLoadReg->Branch("V_in", &load_vin );
  tLoadReg->Branch("I_in", &load_iin );
  tLoadReg->Branch("V_out1", &load_vout1 );
  tLoadReg->Branch("V_out2", &load_vout2 );
  tLoadReg->Branch("I_load", &load_iload );
  tLoadReg->Branch("I_Iout", &load_iout );


  //skip first line for both files.
  string line;
  std::getline( f_lineReg, line );
  std::getline( f_loadReg, line );

  while( f_lineReg.good() ) {
    std::getline( f_lineReg, line );
    if( f_lineReg.eof() ) break;

    size_t pos[3];
    pos[0] = line.find(",");
    pos[1] = line.find(",", pos[0]+1 );
    pos[2] = line.find(",", pos[1]+1 );
    string s_vin = line.substr( 0, pos[0] );
    string s_iin = line.substr( pos[0] + 1, pos[1] - pos[0]-1 );
    string s_vout2 = line.substr( pos[1] + 1, pos[2] - pos[1]-1 );
    string s_vout1 = line.substr( pos[2] + 1, line.length() - pos[2]-1 );
    
    line_vin = atof( s_vin.c_str() );
    line_iin = atof( s_iin.c_str() );
    line_vout1 = atof( s_vout1.c_str() );
    line_vout2 = atof( s_vout2.c_str() );
   
    if( line_vin > 10. || line_iin > 10. || line_vout1 > 10. || line_vout2 > 10. ) {
      cout << "WARNING! : Check raw output data for : " << argv[1] << "! Line Regulation measurements read : " << line_vin << " " << line_iin << " " << line_vout1 << " " << line_vout2 << endl;
      getchar();
    }
    //cout << "adding " << line_vin << " " << line_iin << " " << line_vout1 << " " << line_vout2 << endl;
    tLineReg->Fill();

  }

  while( f_loadReg.good() ) {
    std::getline( f_loadReg, line );
    if( f_loadReg.eof() ) break;
    
    size_t pos[5];
    pos[0] = line.find(",");
    for( unsigned int i = 1; i <= 4; ++i ) {
      pos[i] = line.find(",", pos[i-1] + 1 );
    }

    string s_iload = line.substr(0, pos[0] );
    string s_vout2 = line.substr( pos[0] + 1, pos[1] - pos[0] - 1 );
    string s_iout = line.substr( pos[1] + 1, pos[2] - pos[1] - 1 );
    string s_vin = line.substr( pos[2] + 1, pos[3] - pos[2] - 1 );
    string s_iin = line.substr( pos[3] + 1, pos[4] - pos[3] - 1 );
    string s_vout1 = line.substr( pos[4] + 1, line.length() - pos[4] - 1 );
  
    load_vin = atof( s_vin.c_str() );
    load_iin = atof( s_iin.c_str() );
    load_vout1 = atof( s_vout1.c_str() );
    load_vout2 = atof( s_vout2.c_str() );
    load_iload = atof( s_iload.c_str() );
    load_iout = atof( s_iout.c_str() );
  
    if( load_vin > 10. || load_iin > 10. || load_vout1 > 10. || load_vout2 > 10. || load_iload > 10. || load_iout > 10. ) {
      cout << "WARNING! : Check raw output data for : " << argv[1] << "! Load Regulation measurements read : " << load_vin << " " << load_iin << " " << load_vout1 << " " << load_vout2 << " " << load_iload << " " << load_iout << endl;
      getchar();
    }
    tLoadReg->Fill(); 
  
  }
  if( tLoadReg->GetEntries() <= 0 ) std::cout << "WARNING: Tree LoadRegulation is empty! Check if raw data file exists!" << endl;
  if( tLineReg->GetEntries() <= 0 ) std::cout << "WARNING: Tree LineRegulation is empty! Check if raw data file exists!" << endl;

  tLineReg->Write();
  tLoadReg->Write();
  f->Close();
}

