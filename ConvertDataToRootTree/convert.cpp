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
  if( argc <=4 ) {
    sprintf( fn_lineReg, "../../../data/IV_CURR_%s_load_0mA_vref_%smV_vofs_%smV.csv", argv[1], argv[2], argv[3] );
    sprintf( fn_loadReg, "../../../data/Loadreg_CURR_%s_vref_%smV_vofs_%smV.csv", argv[1], argv[2], argv[3] );
  }
  else {
    sprintf( fn_lineReg, "%s/data/IV_CURR_%s_load_0mA_vref_%smV_vofs_%smV.csv", argv[1], argv[2], argv[3], argv[4] );
    sprintf( fn_loadReg, "%s/data/Loadreg_CURR_%s_vref_%smV_vofs_%smV.csv", argv[1], argv[2], argv[3], argv[4] );
  }
 
  cout << "trying to open file : " << fn_loadReg << endl;
  ifstream f_lineReg( fn_lineReg, ios::in );
  ifstream f_loadReg( fn_loadReg, ios::in );

  char filename[200];
  sprintf( filename, "ntuple_%s_vref_%s_vofs_%s.root", argv[1], argv[2], argv[3] );
  TFile *f = new TFile( filename, "RECREATE" );
  TTree *tLineReg = new TTree("LineRegulation", "LineRegulation" );
  TTree *tLoadReg = new TTree("LoadRegulation", "LoadRegulation" );
 
  double vref = atof(argv[1])/1000.;
  double vofs = atof(argv[2])/1000.;

  double line_vin = 0.;
  double line_iin = 0.;
  double line_vout = 0.;
  double line_iload = 0.;
  double line_vref_chip = 0.;
  double line_vofs_chip = 0.;
  double line_vr1 = 0.;
  double line_vref_int = 0.;

  double load_vin = 0.;
  double load_iin = 0.;
  double load_vout = 0.;
  double load_iout = 0.;
  double load_vout_sm1 = 0.;
  double load_vout_sm2 = 0.;
  double load_iout_sm1 = 0.;
  double load_iout_sm2 = 0.;
  double load_iout_set = 0.;
  double load_vref_chip = 0.;
  double load_vofs_chip = 0.;
  double load_vr1 = 0.;
  double load_vref_int = 0.;

  string sdose( argv[1] );
  size_t cut = sdose.find("krad");
  if( cut == std::string::npos ) cut = sdose.find("Mrad");

  double dose = atof( sdose.substr( 0, cut ).c_str() );
  cout << "dose is " << dose << endl;
  if( sdose.substr( cut, sdose.length() - cut ) == "krad" ) { dose *= 1.e3; cout << "multiplying dose by 1.e3" << endl; }
  else if( sdose.substr( cut, sdose.length() - cut ) == "Mrad" ) { dose *= 1.e6; cout << "multiplying dose by 1.e6" << endl; }
  else cout << "unknown conversion factor" << endl;


  tLineReg->Branch("Dose", &dose );
  tLineReg->Branch("V_ref", &vref );
  tLineReg->Branch("V_ofs", &vofs );
  tLineReg->Branch("V_in", &line_vin );
  tLineReg->Branch("I_in", &line_iin );
  tLineReg->Branch("V_out", &line_vout );
  tLineReg->Branch("I_load", &line_iload );
  tLineReg->Branch("V_Ref_Chip", &line_vref_chip );
  tLineReg->Branch("V_Ofs_Chip", &line_vofs_chip );
  tLineReg->Branch("V_R1", &line_vr1 );
  tLineReg->Branch("V_Ref_Int", &line_vref_int );

  tLoadReg->Branch("Dose", &dose );
  tLoadReg->Branch("V_ref", &vref );
  tLoadReg->Branch("V_ofs", &vofs );
  tLoadReg->Branch("V_in", &load_vin );
  tLoadReg->Branch("I_in", &load_iin );
  tLoadReg->Branch("V_out", &load_vout );
  tLoadReg->Branch("I_out", &load_iout );
  tLoadReg->Branch("V_out_SM1", &load_vout_sm1 );
  tLoadReg->Branch("V_out_SM2", &load_vout_sm2 );
  tLoadReg->Branch("I_out_SM1", &load_iout_sm1 );
  tLoadReg->Branch("I_out_SM2", &load_iout_sm2 );
  tLoadReg->Branch("I_out_set", &load_iout_set );
  tLoadReg->Branch("V_Ref_Chip", &load_vref_chip );
  tLoadReg->Branch("V_Ofs_Chip", &load_vofs_chip );
  tLoadReg->Branch("V_R1", &load_vr1 );
  tLoadReg->Branch("V_Ref_Int", &load_vref_int );


  //skip first line for both files.
  string line;
  std::getline( f_lineReg, line );
  std::getline( f_loadReg, line );

  cout << endl << endl;
  cout << "reading values for line regulation for dose " << dose << endl;
  cout << endl << endl;
  cout << "Vin \t Iin \t Vout \t Iload \t Vref_Chip \t Vofs_Chip \t VR1 \t Vref_int" << endl; 
  cout << "--------------------------------------------------------------------------------" << endl;
  while( f_lineReg.good() ) {
    std::getline( f_lineReg, line );
    if( f_lineReg.eof() ) break;

    size_t pos[7];
    pos[0] = line.find(",");
    for( unsigned int i = 1; i < 7; ++i ) {
      pos[i] = line.find(",", pos[i-1]+1 );
    }
    string vals[8];
    for( unsigned int i = 0; i < 8; ++i ) {
      if( i == 0 ) vals[i] = line.substr(0, pos[0] );
      else if ( i < 7 ) vals[i] = line.substr( pos[i-1] + 1, pos[i] - pos[i-1] -1 );
      else vals[i] = line.substr( pos[i-1] + 1, line.length() - pos[i-1]-1 );
    }
    
    line_vin = atof( vals[0].c_str() ); 
    line_iin = atof( vals[1].c_str() );
    line_vout = atof( vals[2].c_str() );
    line_iload = atof( vals[3].c_str() );
    line_vref_chip = atof( vals[4].c_str() );
    line_vofs_chip = atof( vals[5].c_str() );
    line_vr1 = atof( vals[6].c_str() );
    line_vref_int = atof( vals[7].c_str() );

    if( line_vin > 10. || line_iin > 10. || line_vout > 10. || line_iload > 10. || line_vref_chip > 10. || line_vofs_chip > 10. || line_vr1 > 10. || line_vref_int > 10. ) {
      cout << "WARNING! : Check raw output data for : " << argv[1] << "! Line Regulation measurements read : " << line_vin << " " << line_iin << " " << line_vout << " " << endl;
      getchar();
    }
    //cout << line_vin << "\t" << line_iin << "\t" << line_vout << "\t" << line_iload << "\t" << line_vref_chip << "\t" << line_vofs_chip << "\t" << line_vr1 << "\t" << line_vref_int << endl;
    tLineReg->Fill();

  }
  
  
  cout << endl << endl;
  cout << "reading values for line regulation for dose " << dose << endl;
  cout << endl << endl;
  cout << "Vin \t Iin \t Vout \t Iload \t Vout_SM1 \t Vout_SM2 \t Iout_SM1 \t Iout_SM2 \t Iout_Set \t Vref_Chip \t Vofs_Chip \t VR1 \t Vref_int" << endl; 
  cout << "--------------------------------------------------------------------------------" << endl;

  while( f_loadReg.good() ) {
    std::getline( f_loadReg, line );
    if( f_loadReg.eof() ) break;
    
    size_t pos[12];
    pos[0] = line.find(",");
    for( unsigned int i = 1; i < 12; ++i ) {
      pos[i] = line.find(",", pos[i-1] + 1 );
    }
    
    string vals[13];
    for( unsigned int i = 0; i < 13; ++i ) {
      if( i == 0 ) vals[i] = line.substr(0, pos[0] );
      else if ( i < 13 ) vals[i] = line.substr( pos[i-1] + 1, pos[i] - pos[i-1] -1 );
      else vals[i] = line.substr( pos[i-1] + 1, line.length() - pos[i-1]-1 );
    }

    load_vin = atof( vals[0].c_str() );
    load_iin = atof( vals[1].c_str() );
    load_vout = atof( vals[2].c_str() );
    load_iout = atof( vals[3].c_str() );
    load_vout_sm1 = atof( vals[4].c_str() );
    load_vout_sm2 = atof( vals[5].c_str() );
    load_iout_sm1 = atof( vals[6].c_str() );
    load_iout_sm2 = atof( vals[7].c_str() );
    load_iout_set = atof( vals[8].c_str() );
    load_vref_chip = atof( vals[9].c_str() );
    load_vofs_chip = atof( vals[10].c_str() );
    load_vr1 = atof( vals[11].c_str() );
    load_vref_int = atof( vals[12].c_str() );

  
    if( load_vin > 10. || load_iin > 10. || load_vout > 10. || load_iout > 10. || load_vout_sm1 > 10. || load_vout_sm2 > 10. || load_iout_sm1 > 10. || load_iout_sm2 > 10. || load_iout_set > 10. || load_vref_chip > 10. || load_vofs_chip > 10. || load_vr1 > 10. || load_vref_int > 10. ) {
      cout << "WARNING! : Check raw output data for : " << argv[1] << "! Load Regulation measurements read : " << load_vin << " " << load_iin << " " << load_vout << " " << " " << load_iout << " " << load_iout << endl;
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

