#include <cstdio>
#include <string>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <stack>

using namespace std ;

struct Table {
	string name ;
	bool exist ;
};

struct Token {
	string token ;
	int type ;
	int value ;
};

struct Line {
	int num ;
	string origin ;
	vector <Token> tkns ;
};

struct group {
	int start ;
	int end ;
	int length ;
	string token ;
};

struct op {
	string operation ;
	string machine ;
	int format ;
};

struct symbl {
	string symbol ;
	string address ;
};

struct literal {
	string name ;
	string inputL ;
	string address ;
	int type ;
	bool exist ;
	bool isDefined ;
};

struct out {
	int line ;
	string loc ;
	string inputL ;
	string machine ;
};

vector <string> input ;
vector <Line> tokens ;

class Hash_Lit {
	public:
	void make( literal * & table ) {
		for ( int i = 0 ; i < 100 ; i++ ) {
			table[i].name = "" ;
			table[i].inputL = "" ;
			table[i].address = "" ;
			table[i].type = 0 ;
			table[i].exist = false ;
			table[i].isDefined = false ;
		}
	}
	
	int getHash( string name ) {
		int result = 0 ;
		for ( int i = 0 ; i < strlen( name.c_str() ) ; i++ ) {
			int plus = (int)name.c_str()[i] ;
			result = result + plus ;
		}
		
		return result % 100 ;
	}
	
	void getTable( literal lit, literal * & table ) {
		int value = getHash( lit.name ) ;
		findOrInsert( value, lit, table ) ;
	}
	
	void findOrInsert( int value, literal lit, literal * & table ) {
		int i = 0 ;
		while ( i < 100 ) {
			if ( table[value].exist == false ) {
				table[value].exist = true ;
				table[value].inputL = lit.inputL ;
				table[value].name = lit.name ;
				table[value].type = lit.type ;
				break ;
			}
			else if ( lit.name == table[value].name )
				break ;
			else {
				value = ( value + 1 ) % 100 ;
				i++ ;
			}
		}
	}
	
	void clear( literal * table ) {
		if ( table != NULL )
			delete [] table ;	
	}
};

class Hash {
public:
	void make( Table * & table ) {
		for ( int i = 0 ; i < 100 ; i++ ) {
			table[i].name = "" ;
			table[i].exist = false ;
		}
	}
	
	int getHash( string name ) {
		int result = 0 ;
		for ( int i = 0 ; i < strlen( name.c_str() ) ; i++ ) {
			int plus = (int)name.c_str()[i] ;
			result = result + plus ;
		}
		
		return result % 100 ;
	}
	
	int getTable( string name, Table * & table ) {
		int value = getHash( name ) ;
		return findOrInsert( value, name, table ) ;
	}
	
	int findOrInsert( int value, string name, Table * & table ) {
		int i = 0 ;
		while ( i < 100 ) {
			if ( table[value].exist == false ) {
				table[value].exist = true ;
				table[value].name = name ;
				return value ;
			}
			else if ( name == table[value].name ) {
				return value ;
			}
			else {
				value = ( value + 1 ) % 100 ;
				i++ ;
			}
		}
		
		return -1 ;
	}
};

class AllTable {
	Hash hash ;
	vector <string> table1 ;
	vector <string> table2 ;
	vector <string> table3 ;
	vector <string> table4 ;	
	Table * table5 ;
	Table * table6 ;
	Table * table7 ;
public:		
	AllTable() {
		table5 = new Table[100] ;
		table6 = new Table[100] ;
		table7 = new Table[100] ;
	}
	
	void MakeTable( string num, vector <string> & table ) {
		string str = "Table" + num + ".table" ;
		fstream file ;
		int stNo = 0 ;
		file.open( str.c_str(), fstream::in );
		string a, final = "" ;
		while ( file >> a ) {
			table.push_back( a ) ;
		}
		file.close();		
	}
	
	void makeHash() {
		hash.make( table5 );
		hash.make( table6 );
		hash.make( table7 );
	}
	
	void make() {
		MakeTable( "1", table1 );
		MakeTable( "2", table2 );
		MakeTable( "3", table3 );
		MakeTable( "4", table4 );
		makeHash() ;
	}
	
	int getHashValue( string token, int table ) {
		if ( table == 5 )
			return hash.getTable( token, table5 ) ;
		else if ( table == 6 )
			return hash.getTable( token, table6 ) ;
		else if ( table == 7 )
			return hash.getTable( token, table7 ) ;		
	}
	
	int insSize() {
		return table1.size() ;
	}
	
	int exSize() {
		return table2.size() ;
	}
	
	int regSize() {
		return table3.size() ;
	}
	
	int delSize() {
		return table4.size() ;
	}
	
	string insToken( int pos ) {
		return table1.at( pos ) ;
	} 
	
	string exToken( int pos ) {
		return table2.at( pos ) ;
	} 
	
	string regToken( int pos ) {
		return table3.at( pos ) ;
	} 
	
	string delToken( int pos ) {
		return table4.at( pos ) ;
	} 
};

class MakeToken {
	string token ;
	Hash hash ;
	AllTable table ;
public:
	MakeToken( AllTable table1 ) {
		table = table1 ;
	}
	
	bool IsWhiteSpace( char ch ) {
		if ( ch ==' ' || ch == '\t' || ch == '\n' )
			return true ;
		return false ;	
	}
	
	bool IsInstruction( string token, int & value ) {
		for ( int i = 0 ; i < table.insSize() ; i++ ) {
			if ( stricmp( token.c_str(), table.insToken( i ).c_str() ) == 0 ) {
				value = i + 1 ;
				return true ;				
			}
		}
		
		value = 0 ;
		return false ;
	}
	
	bool IsPseudo( string token, int & value ) {
		for ( int i = 0 ; i < table.exSize() ; i++ ) {
			if ( stricmp( token.c_str(), table.exToken( i ).c_str() ) == 0 ) {
				value = i + 1 ;
				return true ;	
			}
		}
		
		value = 0 ;
		return false ;
	}
	bool IsRegister( string token, int & value ) {
		for ( int i = 0 ; i < table.regSize() ; i++ )	{
			if ( stricmp( token.c_str(), table.regToken( i ).c_str() ) == 0 ) {
				value = i + 1 ;
				return true ;	
			}
		}
		
		value = 0 ;
		return false ;
	}
	
	bool IsDelimiter( string token, int & value ) {
		for ( int i = 0 ; i < table.delSize() ;i++ ) {
			if ( strcmp( token.c_str(), table.delToken( i ).c_str() ) == 0 ) {
				value = i + 1 ;
				return true ;
			}
		}
		
		value = 0 ;
		return false ;
	}
	
	bool IsDelimiter( char ch ) {
		for ( int i = 0 ; i < table.delSize() ; i++ ) {
			if ( table.delToken( i ).c_str()[0] == ch )
				return true ;
		}
		return false ;
	}
	
	bool IsInteger( string token ) {
		if ( token[0] < '0' || token[0] > '9' ) 
			return false ;
		return true ;
	}
	
	void getValue( string token, int & type, int & value, int special ) {
		if ( special == 1 ) {
			type = 7 ;
			value = table.getHashValue( token, 7 ) ;
		}
		else if ( special == 2 ) {
			type = 6 ;
			string change ;
			transform(token.begin(), token.end(), back_inserter(change), ::toupper);
			value = table.getHashValue( change, 6 ) ;
		}
		else if ( special == 3 ) {
			type = 5 ;
			value = table.getHashValue( token, 5 ) ;
		} 
		else if ( IsInstruction( token, value ) ) {
			type = 1 ;
		}
		else if ( IsPseudo( token, value ) ) {
			type = 2 ;
		}
		else if ( IsRegister( token, value ) ) {
			type = 3 ;
		} 
		else if ( IsDelimiter( token, value ) ) {
			type = 4 ;
		}
		else if ( IsInteger( token ) ) {
			type = 6 ;
			value = table.getHashValue( token, 6 ) ;
		}
		else { 
			type = 5 ;
			value = table.getHashValue( token, 5 ) ;
		}
	}

	void addResult( int type, int value, string token, Line & curLine ) {
		Token temp ;
		temp.type = type ;
		temp.value = value ;
		temp.token = token ;
		curLine.tkns.push_back( temp );
	}
	
	void getToken() {
		bool skip = false, notToken = false ;
		int special = -1, quote = 0 ;
		int type, value ;
		for ( int i = 0 ; i < input.size() ; i++ ) {
			Line temp ;
			temp.origin = input.at( i ) ;
			temp.num = i + 1 ;
			for ( int j = 0 ; j < input.at( i ).size() ; j++ ) {
				if ( input[i][j] == '\n' ) {
					if ( special != -1 && quote == 1 )
						token = "" ;
					quote = 0 ;
					special = -1 ;					
				}
				if ( skip ) {
					if ( input[i][j] == '\n' ) 
						skip = false ;
					token = "" ;	
				}
				else if ( IsWhiteSpace( input[i][j] ) ) {
					if ( special != -1 ) {
						token = token + input[i][j] ;
					}
					else {
						if ( token != "" ) {
							getValue( token, type, value, -1 ) ;
							addResult( type, value, token, temp ) ;
						}
						token = "" ;
					}
				}
				else if ( IsDelimiter( input[i][j] ) ) {
					if ( special != -1 && input[i][j] != '\'' && quote == 1 ) 
						token = token + input[i][j] ;
					else {
					if ( input[i][j] == '\'' && special == -1 ) {
						quote++ ;
						if ( stricmp( token.c_str(), "C" ) == 0 ) {
							notToken = true ;
							special = 1 ;
						}	
						else if ( stricmp( token.c_str(), "X" ) == 0 ) {
							notToken = true ;
							special = 2 ;
						}
						else {
							notToken = true ;
							special = 3 ;
						}
					}
					else if ( input[i][j] == '\'' && special == -1 )
						quote++ ;
					else if ( input[i][j] == '.' ) {
						skip = true ;
					}
					
					if ( !notToken && token != "" ) {
						getValue( token, type, value, special ) ;
						addResult( type, value, token, temp ) ;					
					}
					notToken = false ;	
					token = input[i][j] ;
					if ( quote == 2 )
						special = -1 ;
					
					getValue( token, type, value, -1 ) ;
					addResult( type, value, token, temp ) ;
					token = "" ;
					
					}
				}
				else {
					token = token + input[i][j] ;
				}
			}
			
			if (!temp.origin.empty() && temp.origin[temp.origin.length()-1] == '\n') {
    			temp.origin.erase(temp.origin.length()-1);
			}
			tokens.push_back( temp ) ;
		}
	}
	
	void print() {
		cout << endl ;
		int j = 0 ;
		for ( int i = 0 ; i < tokens.size() ; i++ ) {
			cout << tokens.at( i ).origin << endl ;
			for ( int j = 0 ; j < tokens.at( i ).tkns.size() ; j++ ) 	
				cout << "(" << tokens.at( i ).tkns.at( j ).type << "," << tokens.at( i ).tkns.at( j ).value << ")" ;	
			cout << endl ;	
		}
		
	}
	
	void outFile( string fileName ) {
		fstream outFile ;
		outFile.open( fileName.c_str(), fstream::out );
		int j = 0 ;
		for ( int i = 0 ; i < tokens.size() ; i++ ) {
			outFile << tokens.at( i ).origin ;
			for ( int j = 0 ; j < tokens.at( i ).tkns.size() ; j++ ) 	
				outFile << "(" << tokens.at( i ).tkns.at( j ).type << "," << tokens.at( i ).tkns.at( j ).value << ")" ;	
			outFile << endl ;	
		}
		
		outFile << endl ;
		outFile.close();
	}
	
	vector <Line> getTokens() {
		return tokens ;
	}
	
	void clear() {
		tokens.clear() ;
	}
};

class SIC {	
	string locctr ;
	string base ;
	vector <op> optab ;
	vector <symbl> symtab ;
	Hash_Lit hash ;
	literal * littab ;
	vector <out> final ;
	AllTable table ;
public:
	SIC( AllTable table1 ) {
		table = table1 ;
		littab = new literal[100] ;
		hash.make( littab ) ;
	}
	
	void set( AllTable table1 ) {
		table = table1 ;
		littab = new literal[100] ;
		hash.make( littab ) ;
	}
	
	void makeOpcode() {
		for ( int i = 0 ; i < table.insSize() ; i++ ) {
			op temp ;
			temp.operation = table.insToken( i ) ;
			if ( stricmp( temp.operation.c_str(), "ADD" ) == 0 ) {
				temp.machine = "18" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "ADDF" ) == 0 ) {
				temp.machine = "58" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "ADDR" ) == 0 ) {
				temp.machine = "90" ;
				temp.format = 2 ;
			}
			if ( stricmp( temp.operation.c_str(), "AND" ) == 0 ) {
				temp.machine = "40" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "CLEAR" ) == 0 ) {
				temp.machine = "B4" ;
				temp.format = 2 ;
			}
			if ( stricmp( temp.operation.c_str(), "COMP" ) == 0 ) {
				temp.machine = "28" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "COMPF" ) == 0 ) {
				temp.machine = "88" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "COMPR" ) == 0 ) {
				temp.machine = "A0" ;
				temp.format = 2 ;
			}
			if ( stricmp( temp.operation.c_str(), "DIV" ) == 0 ) {
				temp.machine = "24" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "DIVF" ) == 0 ) {
				temp.machine = "64" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "DIVR" ) == 0 ) {
				temp.machine = "9C" ;
				temp.format = 2 ;
			}
			if ( stricmp( temp.operation.c_str(), "FIX" ) == 0 ) {
				temp.machine = "C4" ;
				temp.format = 1 ;
			}
			if ( stricmp( temp.operation.c_str(), "FLOAT" ) == 0 ) {
				temp.machine = "C0" ;
				temp.format = 1 ;
			}
			if ( stricmp( temp.operation.c_str(), "HIO" ) == 0 ) {
				temp.machine = "F4" ;
				temp.format = 1 ;
			}
			if ( stricmp( temp.operation.c_str(), "J" ) == 0 ) {
				temp.machine = "3C" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "JEQ" ) == 0 ) {
				temp.machine = "30" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "JGT" ) == 0 ) {
				temp.machine = "34" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "JLT" ) == 0 ) {
				temp.machine = "38" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "JSUB" ) == 0 ) {
				temp.machine = "48" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "LDA" ) == 0 ) {
				temp.machine = "00" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "LDB" ) == 0 ) {
				temp.machine = "68" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "LDCH" ) == 0 ) {
				temp.machine = "50" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "LDF" ) == 0 ) {
				temp.machine = "70" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "LDL" ) == 0 ) {
				temp.machine = "08" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "LDS" ) == 0 ) {
				temp.machine = "6C" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "LDT" ) == 0 ) {
				temp.machine = "74" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "LDX" ) == 0 ) {
				temp.machine = "04" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "LPS" ) == 0 ) {
				temp.machine = "D0" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "MUL" ) == 0 ) {
				temp.machine = "20" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "MULF" ) == 0 ) {
				temp.machine = "60" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "MULR" ) == 0 ) {
				temp.machine = "98" ;
				temp.format = 2 ;
			}
			if ( stricmp( temp.operation.c_str(), "NORM" ) == 0 ) {
				temp.machine = "C8" ;
				temp.format = 1 ;
			}
			if ( stricmp( temp.operation.c_str(), "OR" ) == 0 ) {
				temp.machine = "44" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "RD" ) == 0 ) {
				temp.machine = "D8" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "RMO" ) == 0 ) {
				temp.machine = "AC" ;
				temp.format = 2 ;
			}
			if ( stricmp( temp.operation.c_str(), "RSUB" ) == 0 ) {
				temp.machine = "4C" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "SHIFTL" ) == 0 ) {
				temp.machine = "A4" ;
				temp.format = 2 ;
			}
			if ( stricmp( temp.operation.c_str(), "SHIFTR" ) == 0 ) {
				temp.machine = "A8" ;
				temp.format = 2 ;
			}
			if ( stricmp( temp.operation.c_str(), "SIO" ) == 0 ) {
				temp.machine = "F0" ;
				temp.format = 1 ;
			}
			if ( stricmp( temp.operation.c_str(), "SSK" ) == 0 ) {
				temp.machine = "EC" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "STA" ) == 0 ) {
				temp.machine = "0C" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "STB" ) == 0 ) {
				temp.machine = "78" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "STCH" ) == 0 ) {
				temp.machine = "54" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "STF" ) == 0 ) {
				temp.machine = "80" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "STI" ) == 0 ) {
				temp.machine = "D4" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "STL" ) == 0 ) {
				temp.machine = "14" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "STS" ) == 0 ) {
				temp.machine = "7C" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "STSW" ) == 0 ) {
				temp.machine = "E8" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "STT" ) == 0 ) {
				temp.machine = "84" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "STX" ) == 0 ) {
				temp.machine = "10" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "SUB" ) == 0 ) {
				temp.machine = "1C" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "SUBF" ) == 0 ) {
				temp.machine = "5C" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "SUBR" ) == 0 ) {
				temp.machine = "94" ;
				temp.format = 2 ;
			}
			if ( stricmp( temp.operation.c_str(), "SVC" ) == 0 ) {
				temp.machine = "B0" ;
				temp.format = 2 ;
			}
			if ( stricmp( temp.operation.c_str(), "TD" ) == 0 ) {
				temp.machine = "E0" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "TIO" ) == 0 ) {
				temp.machine = "F8" ;
				temp.format = 1 ;
			}
			if ( stricmp( temp.operation.c_str(), "TIX" ) == 0 ) {
				temp.machine = "2C" ;
				temp.format = 34 ;
			}
			if ( stricmp( temp.operation.c_str(), "TIXR" ) == 0 ) {
				temp.machine = "B8" ;
				temp.format = 2 ;
			}
			if ( stricmp( temp.operation.c_str(), "WD" ) == 0 ) {
				temp.machine = "DC" ;
				temp.format = 34 ;
			}
			
			optab.push_back( temp ) ;
		} 
	}
	
	void PassCommentLine( int & line ) {		
		while ( line-1 < tokens.size() ) {
			if ( tokens.at( line-1 ).tkns.empty() )
				line++ ;
			else if ( tokens.at( line-1 ).tkns[0].token == "." )
				line++ ;
			else 
				break ;		
		}
	}
	
	string getLab( int line ) {
		if ( tokens.at( line-1 ).tkns[0].type == 5 )
			return tokens.at( line-1 ).tkns[0].token ;
		else
			return "" ;	
	}
	
	string getIns( int line, int & pos ) {
		if ( tokens.at( line-1 ).tkns[0].type == 5 ) {
			if ( tokens.at( line-1 ).tkns.size() == 1 ) {
				pos = -1 ;
				return "" ;
			}
			else if ( stricmp( tokens.at( line-1 ).tkns[1].token.c_str(), "EQU" ) == 0 ) {
				pos = 1 ;
				return tokens.at( line-1 ).tkns[1].token ;
			}
			else if ( tokens.at( line-1 ).tkns[1].type == 1 ) {
				pos = 1 ;
				return tokens.at( line-1 ).tkns[1].token ;
			}
			else if ( tokens.at( line-1 ).tkns[1].type == 2 ) {
				pos = 1 ;
				return tokens.at( line-1 ).tkns[1].token ;
			}
			else {
				pos = -1 ;
				return "" ;
			}
		}
		else if ( tokens.at( line-1 ).tkns[0].type == 1 ) {
			pos = 0 ;
			return tokens.at( line-1 ).tkns[0].token ;
		}
		else if ( tokens.at( line-1 ).tkns[0].type == 2 ) {
			pos = 0 ; 
			return tokens.at( line-1 ).tkns[0].token ;
		}
		else {
			pos = -1 ;
			return "" ;
		}
			
	}
	
	string getInsXE( int line, int & pos ) {
		if ( stricmp( tokens.at( line-1 ).tkns[0].token.c_str(), "BASE" ) == 0 ) {
			pos = 0 ; 
			return tokens.at( line-1 ).tkns[0].token ;
		}
		else if ( stricmp( tokens.at( line-1 ).tkns[0].token.c_str(), "NOBASE" ) == 0 ) {
			pos = 0 ; 
			return tokens.at( line-1 ).tkns[0].token ;
		}
		else if ( stricmp( tokens.at( line-1 ).tkns[0].token.c_str(), "LTORG" ) == 0 ) {
			pos = 0 ; 
			return tokens.at( line-1 ).tkns[0].token ;
		}
		else if ( tokens.at( line-1 ).tkns[0].token == "+" ) {
			if ( tokens.at( line-1 ).tkns[1].type == 1 ) {
				pos = 1 ;
				return tokens.at( line-1 ).tkns[1].token ;	
			}
			else {
				pos = -1 ;
				return "" ;
			}
		}
		else if ( tokens.at( line-1 ).tkns[0].type == 5 ) {
			if ( tokens.at( line-1 ).tkns.size() == 1 ) {
				pos = -1 ;
				return "" ;
			}
			if ( tokens.at( line-1 ).tkns[1].token == "+" ) {
				pos = 2 ;
				return tokens.at( line-1 ).tkns[2].token ;
			}
			else if ( stricmp( tokens.at( line-1 ).tkns[1].token.c_str(), "BASE" ) == 0 ) {
				pos = 1 ; 
				return tokens.at( line-1 ).tkns[1].token ;
			}
			else if ( stricmp( tokens.at( line-1 ).tkns[1].token.c_str(), "NOBASE" ) == 0 ) {
				pos = 1 ; 
				return tokens.at( line-1 ).tkns[1].token ;
			}
			else if ( stricmp( tokens.at( line-1 ).tkns[1].token.c_str(), "LTORG" ) == 0 ) {
				pos = 1 ; 
				return tokens.at( line-1 ).tkns[1].token ;
			}
			else if ( stricmp( tokens.at( line-1 ).tkns[1].token.c_str(), "EQU" ) == 0 ) {
				pos = 1 ;
				return tokens.at( line-1 ).tkns[1].token ;
			}
			else if ( tokens.at( line-1 ).tkns[1].type == 1 ) {
				pos = 1 ;
				return tokens.at( line-1 ).tkns[1].token ;
			}
			else if ( tokens.at( line-1 ).tkns[1].type == 2 ) {
				pos = 1 ;
				return tokens.at( line-1 ).tkns[1].token ;
			}
			else {
				pos = -1 ;
				return "" ;
			}
		}
		else if ( tokens.at( line-1 ).tkns[0].type == 1 ) {
			pos = 0 ;
			return tokens.at( line-1 ).tkns[0].token ;
		}
		else if ( tokens.at( line-1 ).tkns[0].type == 2 ) {
			pos = 0 ; 
			return tokens.at( line-1 ).tkns[0].token ;
		}
		else {
			pos = -1 ;
			return "" ;
		}
			
	}
	
	bool checksym( string token ) {
		for ( int i = 0 ; i < symtab.size() ; i++ ) {
			if ( symtab.at( i ).symbol == token ) 
				return false ;
		} 
		
		return true ; 
	}
	
	bool checkIns( int line, int pos ) {
		if ( tokens.at( line-1 ).tkns[pos].type == 1 )
			return true ;
		else
			return false ;		
	}
	
	bool isSIC( int line, int pos ) {
		string str = tokens.at( line-1 ).tkns[pos].token ;
		if ( stricmp( str.c_str(), "ADD" ) == 0 )
			return true ;
		else if ( stricmp( str.c_str(), "AND" ) == 0 )
			return true ;
		else if ( stricmp( str.c_str(), "COMP" ) == 0 )
			return true ;
		else if ( stricmp( str.c_str(), "DIV" ) == 0 )
			return true ;			
		else if ( stricmp( str.c_str(), "J" ) == 0 )
			return true ;	
		else if ( stricmp( str.c_str(), "JEQ" ) == 0 )
			return true ;
		else if ( stricmp( str.c_str(), "JGT" ) == 0 )
			return true ;	
		else if ( stricmp( str.c_str(), "JLT" ) == 0 )
			return true ;
		else if ( stricmp( str.c_str(), "JSUB" ) == 0 )
			return true ;
		else if ( stricmp( str.c_str(), "LDA" ) == 0 )
			return true ;
		else if ( stricmp( str.c_str(), "LDCH" ) == 0 )
			return true ;
		else if ( stricmp( str.c_str(), "LDL" ) == 0 )
			return true ;
		else if ( stricmp( str.c_str(), "LDX" ) == 0 )
			return true ;
		else if ( stricmp( str.c_str(), "MUL" ) == 0 )
			return true ;
		else if ( stricmp( str.c_str(), "OR" ) == 0 )
			return true ;
		else if ( stricmp( str.c_str(), "RD" ) == 0 )
			return true ;
		else if ( stricmp( str.c_str(), "RSUB" ) == 0 )
			return true ;
		else if ( stricmp( str.c_str(), "STA" ) == 0 )
			return true ;
		else if ( stricmp( str.c_str(), "STCH" ) == 0 )
			return true ;
		else if ( stricmp( str.c_str(), "STL" ) == 0 )
			return true ;
		else if ( stricmp( str.c_str(), "STSW" ) == 0 )
			return true ;
		else if ( stricmp( str.c_str(), "STX" ) == 0 )
			return true ;
		else if ( stricmp( str.c_str(), "SUB" ) == 0 )
			return true ;
		else if ( stricmp( str.c_str(), "TD" ) == 0 )
			return true ;
		else if ( stricmp( str.c_str(), "TIX" ) == 0 )
			return true ;
		else if ( stricmp( str.c_str(), "WD" ) == 0 )
			return true ;
		else
			return false ;	
	}
	
	void Digitloc( string & str, int num ) {
		string temp = "" ;
		if ( str.length() < num ) {
			int loss = num - str.length() ;
			for ( int  i = 0 ; i < loss ; i++ )
				temp = temp + "0" ;
			
			temp = temp  + str ;
			str = temp ;
		}
		
	}
	
	int stringHexToInt( string str ) {
		int temp ;
    	stringstream ss ;
    	ss << hex << str ; 
    	ss >> temp ;
    	ss.clear() ;
    	return temp ;
	}
	
	string IntToStringHex( int num ) {
		stringstream ss ;
		string str ;
		ss << hex << num ;
    	ss >> str ;
    	string change ;
		transform(str.begin(), str.end(), back_inserter(change), ::toupper);
		return change ;	
	}
	
	void strCalculate( string & str, int add ) {
		int temp = stringHexToInt( str ) ;
    	temp = temp + add ;
		str = IntToStringHex( temp ) ;		
	}
	
	string getDisp( string loc, string ta, bool & isPC ) {
		strCalculate( loc, 3 ) ;
		stringstream ss ;
		string disp ;
		int ta_i, loc_i, disp_i ;
		ss << hex << ta ;
		ss >> ta_i ;
		ss.clear() ;
		ss << hex << loc ;
		ss >> loc_i ;
		ss.clear() ;
		disp_i = ta_i - loc_i ;
		if ( disp_i > 2047 || disp_i < -2048 ) {
			if ( base == "No" )
				return "No" ;
			else {
				int base_i ;
				ss << hex << base ;
				ss >> base_i ;
				ss.clear() ;
				disp_i = ta_i - base_i ;
				if ( disp_i > 4095 || disp_i < 0 )
					return "No" ;
				else 
					isPC = false ;			
			}	
		}
		else
			isPC = true ;
		
		ss << hex << disp_i ;
    	ss >> disp ;
    	string change ;
		transform(disp.begin(), disp.end(), back_inserter(change), ::toupper) ;
		if ( disp_i >= 0 ) {
			disp = change ;
			Digitloc( disp, 3 ) ;
		}
		else {
			disp = change.substr( 5 ) ;
		}
		
		return disp ;
	}
	
	void makeLiteral( literal temp ) {
		hash.getTable( temp, littab ) ;
	}
	
	void getLitAddr() {
		for ( int i = 0 ; i < 100 ; i++ ) {
			if ( littab[i].exist && !littab[i].isDefined ) {
				cout << littab[i].name ;
				littab[i].address = locctr ;
				int length = 0 ;
				out temp ;
				temp.line = -1 ;
				temp.loc = locctr ;
				temp.inputL = "	" + littab[i].inputL+ "	" ;
				temp.machine = "" ;
				if ( littab[i].type == 6 )  {
					length = littab[i].name.size() / 2 ;
					temp.machine = littab[i].name ;
				}
				else if ( littab[i].type == 7 ) {
					length = littab[i].name.size() ;
					for ( int j = 0 ; j < littab[i].name.size() ; j++ ) {
						char ch = littab[i].name[j] ;
						int ascii = (int)ch ;
						stringstream ss ;
						ss << hex << ascii ;
    					string change, hexi ;
    					ss >> change ;
						transform(change.begin(), change.end(), back_inserter(hexi), ::toupper);
						Digitloc( hexi, 2 ) ;
						temp.machine = temp.machine + hexi ;
					} 
				}
				else if ( littab[i].type == 8 ) {
					length = 3 ;
					temp.machine = turnHex( littab[i].name ) ;
					Digitloc( temp.machine, 6 ) ;
				}
				strCalculate( locctr, length ) ;
				Digitloc( locctr, 4 ) ;
				final.push_back( temp ) ;
				littab[i].isDefined = true ;
			}
		}
	}
	
	bool isDec( string num ) {
		for ( int i = 0 ; i < num.length() ; i++ ) {
			if ( num[i] < '0' || num[i] > '9' )
				return false ;	
		}
		
		return true ;
	}
	
	void pass1() {
		int curLine = 1, pos ;
		out temp ;
		bool doit = false ;
		PassCommentLine( curLine ) ;		
		if ( getIns( curLine, pos ) == "START" ) {
			temp.line = curLine ;
			temp.inputL = tokens.at( curLine-1 ).origin ;
			if ( tokens.at( curLine-1 ).tkns[pos+1].type == 6 ) {
				locctr = tokens.at( curLine-1 ).tkns[pos+1].token ;
				Digitloc( locctr, 4 ) ;
				temp.loc = locctr ;
				temp.machine = "" ;
				if ( pos != 0 ) {
					symbl tmp ;
					tmp.symbol = tokens.at( curLine-1 ).tkns[0].token ;
					tmp.address = locctr ;
					symtab.push_back( tmp ) ;
				}
				curLine++ ;
			}
			else {
				temp.loc = "" ;
				temp.machine = "Syntax Error: {label} START hex_num" ;
				locctr = "0000" ;
			}
			final.push_back( temp ) ;
			
		}
		else {
			locctr = "0" ;
			Digitloc( locctr, 4 ) ;
		}	
		while ( curLine-1 < tokens.size() ) {
			PassCommentLine( curLine ) ;
			string ins = getIns( curLine, pos ) ;
			vector <group> grp ;
			GetGroups( tokens.at( curLine-1 ).tkns, pos+1, grp ) ;
			bool special = false, notError = true ;
			symbl tmp ;
			if ( pos != 0 ) {
				if ( checksym( tokens.at( curLine-1 ).tkns[0].token ) ) {
					if ( ins == "" ) ;
					else if ( stricmp( ins.c_str(), "EQU" ) != 0 ) {
						tmp.symbol = tokens.at( curLine-1 ).tkns[0].token ;
						tmp.address = locctr ;
					}
					else
						special = true ;
				}
				else { 
					temp.machine = "Error : Duplicate Symbol" ;
				}
			}
			if ( stricmp( ins.c_str(), "END" ) == 0 ) {
				temp.line = curLine ;
				temp.loc = "" ;
				temp.inputL = tokens.at( curLine-1 ).origin ;
				temp.machine = "" ;
				final.push_back( temp ) ;
				doit = true ;
				break ;
			}
			temp.line = curLine ;
			temp.loc = locctr ;
			temp.inputL = tokens.at( curLine-1 ).origin ;
			temp.machine = "" ;
			if ( ins == "" ) {
				temp.loc = "" ;
				temp.machine = "Syntax Error : Invalid Operation Code" ;
			}
			if ( stricmp( ins.c_str(), "EQU" ) == 0 ) {
				vector <Token> group ;
				symbl sym ;
				if ( tokens.at( curLine-1 ).tkns[pos+1].token == "*" ) ;
				else if ( tokens.at( curLine-1 ).tkns[pos+1].type == 6 )
					temp.loc = turnHex( tokens.at( curLine-1 ).tkns[pos+1].token ) ;
				else if ( tokens.at( curLine-1 ).tkns.size() == pos+1 ) {
					if ( findSym( tokens.at( curLine-1 ).tkns[pos+1].token, sym ) ) {
						temp.loc = sym.address ;
					}
					else {
						temp.loc = "" ;
						temp.machine = "Syntax Error: Undefined Symbol" ;
						notError = false ;
						special = false ;
					}
				}
				else if ( isArthimetic( tokens.at( curLine-1 ).tkns, pos, group ) ) {
					string address = "0000" ;
					if ( getAddress( group, address ) ) {
						temp.loc = address ;
					}
					else {
						temp.loc = "" ;
						temp.machine = "Syntax Error: Undefined Symbol" ;
						notError = false ;
						special = false ;
					}
				} 
				if ( special ) {
					tmp.symbol = tokens.at( curLine-1 ).tkns[0].token ;
					Digitloc( temp.loc, 4 ) ;
					tmp.address = temp.loc ;
					special = false ;
				}
			}
			else if ( checkIns( curLine, pos ) ) {
				if ( isSIC( curLine, pos ) ) {
					if ( InsGroup( tokens.at( curLine-1 ).tkns[pos].token, grp.size() ) ) {
						strCalculate( locctr, 3 ) ;		// SIC length = 3
						Digitloc( locctr, 4 ) ;
					}
					else {
						if ( grp.size() == 0 ) {
							strCalculate( locctr, 3 ) ;
							Digitloc( locctr, 4 ) ;
						}
						else if ( grp.size() == 2 ) {
							if ( stricmp( grp.at( 1 ).token.c_str(), "X" ) == 0 && tokens.at( curLine-1 ).tkns[pos+1].type == 5 ) {
								strCalculate( locctr, 3 ) ;
								Digitloc( locctr, 4 ) ;
							}
							else {
								temp.loc = "" ;
								temp.machine = "Syntax Error : standard¡G<INS> <m>" ;
								notError = false ;
							}
						}
						else {
							if ( tokens.at( curLine-1 ).tkns[pos+1].type == 5 ) {
								strCalculate( locctr, 3 ) ;
								Digitloc( locctr, 4 ) ;
							}
							else {
								temp.loc = "" ;
								temp.machine = "Syntax Error : standard¡G<INS> <m>" ;
								notError = false ;
							}
						}
					}
				}	
				else {
					temp.loc = "" ;
					temp.machine = "Syntax Error : Invalid Operation Code" ;
					notError = false ;
				}
			}
			else if ( stricmp( ins.c_str(), "WORD" ) == 0 ) {
				int length = 3 ;
				if ( tokens.at( curLine-1 ).tkns[pos+1].token == "\'" && tokens.at( curLine-1 ).tkns[pos+3].token == "\'" ) { 
					if ( tokens.at( curLine-1 ).tkns[pos+1].type != 5 && tokens.at( curLine-1 ).tkns[pos+1].type != 6 ) {
						temp.loc = "" ;
						temp.machine = "Lexical Error:{label} WORD X''¡K | C''¡K | dec_num" ;
						notError = false ;
						length = 0 ;
					}
				}
				else if ( !isDec( tokens.at( curLine-1 ).tkns[pos+1].token ) ) {
					temp.loc = "" ;
					temp.machine = "Lexical Error:{label} WORD X''¡K | C''¡K | dec_num" ;
					notError = false ;
					length = 0 ;
				}
				strCalculate( locctr, length ) ;
				Digitloc( locctr, 4 ) ;
			}
			else if ( stricmp( ins.c_str(), "RESW" ) == 0 ) {
				if ( isDec( tokens.at( curLine-1 ).tkns[pos+1].token ) ) {
					string str = tokens.at( curLine-1 ).tkns[pos+1].token ;
					int num ;
					stringstream ss ;
					ss << str ;
					ss >> num ;
					strCalculate( locctr, num * 3 ) ;	
					Digitloc( locctr, 4 ) ;
				}
				else {
					temp.loc = "" ;
					temp.machine = "{label} RESW dec_num" ;
					notError = false ;
				}
			}
			else if ( stricmp( ins.c_str(), "RESB" ) == 0 ) {
				if ( isDec( tokens.at( curLine-1 ).tkns[pos+1].token ) ) {
					string str = tokens.at( curLine-1 ).tkns[pos+1].token ;
					int num ;
					stringstream ss ;
					ss << str ;
					ss >> num ;
					strCalculate( locctr, num ) ;
					Digitloc( locctr, 4 ) ;
				}
				else {
					temp.loc = "" ;
					temp.machine = "{label} RESB dec_num" ;
					notError = false ;
				}
			}
			else if ( stricmp( ins.c_str(), "BYTE" ) == 0 ) {
				int length = 0 ;
				if ( tokens.at( curLine-1 ).tkns[pos+1].token == "\'" && tokens.at( curLine-1 ).tkns[pos+3].token == "\'" ) {
					if ( tokens.at( curLine-1 ).tkns[pos+2].type == 6 )  {
						length = tokens.at( curLine-1 ).tkns[pos+2].token.size() / 2 ;
					}
					else if ( tokens.at( curLine-1 ).tkns[pos+2].type == 7 ) {
						length = tokens.at( curLine-1 ).tkns[pos+2].token.size() ;
					}
					else {
						temp.loc = "" ;
						temp.machine = "Lexical Error:{label} BYTE X''¡K | C''¡K | dec_num"  ;
						length = 0 ;
						notError = false ;
					}	
				}
				else {
					if ( !isDec( tokens.at( curLine-1 ).tkns[pos+1].token) ) {
						length = turnHex( tokens.at( curLine-1 ).tkns[pos+1].token ).length() / 2 ;
					}
					else {
						temp.loc = "" ;
						temp.machine = "Lexical Error:{label} BYTE X''¡K | C''¡K | dec_num"  ;
						length = 0 ;
						notError = false ;
					}
				}
				strCalculate( locctr, length ) ;
				Digitloc( locctr, 4 ) ;
			}
			else {
				temp.loc = "" ;
				temp.machine = "Syntax Error : Invalid Operation Code" ;
			}
			
			if ( !doit )
				final.push_back( temp ) ;
			else
				doit = false ;	
			
			if ( notError ) {
				symtab.push_back( tmp ) ;
			}
			
			curLine++ ;
			grp.clear() ;
		}	
	}
	
	void pass1xe() {
		int curLine = 1, pos ;
		out temp ;
		bool doit = false ;
		PassCommentLine( curLine ) ;
		base = "No" ;
		if ( getIns( curLine, pos ) == "START" ) {
			temp.line = curLine ;
			temp.inputL = tokens.at( curLine-1 ).origin ;
			if ( tokens.at( curLine-1 ).tkns[pos+1].type == 6 ) {
				locctr = tokens.at( curLine-1 ).tkns[pos+1].token ;
				Digitloc( locctr, 4 ) ;
				temp.loc = locctr ;
				temp.machine = "" ;
				if ( pos != 0 ) {
					symbl tmp ;
					tmp.symbol = tokens.at( curLine-1 ).tkns[0].token ;
					tmp.address = locctr ;
					symtab.push_back( tmp ) ;
				}
				curLine++ ;
			}
			else {
				temp.loc = "" ;
				temp.machine = "Syntax Error: {label} START hex_num" ;
				locctr = "0000" ;
			}
			final.push_back( temp ) ;
			
		}
		else {
			locctr = "0" ;
			Digitloc( locctr, 4 ) ;
		}
		while ( curLine-1 < tokens.size() ) {
			PassCommentLine( curLine ) ;
			string ins = getInsXE( curLine, pos ) ;
			vector <group> grp ;
			GetGroups( tokens.at( curLine-1 ).tkns, pos+1, grp ) ;
			bool special = false, notError = true ;
			symbl tmp ;
			if ( pos != 0 ) {
				if ( checksym( tokens.at( curLine-1 ).tkns[0].token ) ) {
					if ( ins == "" ) ;
					else if ( stricmp( ins.c_str(), "EQU" ) != 0 ) {
						tmp.symbol = tokens.at( curLine-1 ).tkns[0].token ;
						tmp.address = locctr ;
					}
					else
						special = true ;
				}
				else { 
					temp.machine = "Error : Duplicate Symbol" ;
				}
			}
			if ( stricmp( ins.c_str(), "END" ) == 0 ) {
				temp.line = curLine ;
				temp.loc = "" ;
				temp.inputL = tokens.at( curLine-1 ).origin ;
				temp.machine = "" ;
				final.push_back( temp ) ;
				getLitAddr() ;
				doit = true ;
				break ;
			}
			temp.line = curLine ;
			temp.loc = locctr ;
			temp.inputL = tokens.at( curLine-1 ).origin ;
			temp.machine = "" ;
			if ( ins == "" ) {
				temp.loc = "" ;
				temp.machine = "Syntax Error : Invalid Operation Code" ;
			}
			if ( stricmp( ins.c_str(), "EQU" ) == 0 ) {
				vector <Token> group ;
				symbl sym ;
				if ( tokens.at( curLine-1 ).tkns[pos+1].token == "*" ) ;
				else if ( tokens.at( curLine-1 ).tkns[pos+1].type == 6 )
					temp.loc = turnHex( tokens.at( curLine-1 ).tkns[pos+1].token ) ;
				else if ( tokens.at( curLine-1 ).tkns.size() == pos+1 ) {
					if ( findSym( tokens.at( curLine-1 ).tkns[pos+1].token, sym ) ) {
						temp.loc = sym.address ;
					}
					else {
						temp.loc = "" ;
						temp.machine = "Syntax Error: Undefined Symbol" ;
						notError = false ;
						special = false ;
					}
				}
				else if ( isArthimetic( tokens.at( curLine-1 ).tkns, pos, group ) ) {
					string address = "0000" ;
					if ( getAddress( group, address ) ) {
						temp.loc = address ;
					}
					else {
						temp.loc = "" ;
						temp.machine = "Syntax Error: Undefined Symbol" ;
						notError = false ;
						special = false ;
					}
				} 
				if ( special ) {
					tmp.symbol = tokens.at( curLine-1 ).tkns[0].token ;
					Digitloc( temp.loc, 4 ) ;
					tmp.address = temp.loc ;
					special = false ;
				}
			}
			else if ( stricmp( ins.c_str(), "BASE" ) == 0 ) {
				temp.loc = "" ;
				if ( tokens.at( curLine-1 ).tkns[pos+1].type == 5) ;
				else if ( isDec(tokens.at( curLine-1 ).tkns[pos+1].token ) ) ;
				else {
					temp.machine = "Syntax Error: {label} BASE dec_num | symbol" ;
					notError = false ; 
				} 
			} 
			else if ( stricmp( ins.c_str(), "NOBASE" ) == 0 ) {
				base = "No" ;
			}
			else if ( checkIns( curLine, pos ) ) {
				op mac ;
				int format = findOpFormat( tokens.at( curLine-1 ).tkns, pos ) ;
				findOp( tokens.at( curLine-1 ).tkns[pos].token, mac ) ;
				if ( format == 1 ) {
					if ( InsGroupXE( tokens.at( curLine-1 ).tkns[pos].token, grp.size() ) ) {
						strCalculate( locctr, 1 ) ;
						Digitloc( locctr, 4 ) ;
					}
					else {
						temp.loc = "" ;
						temp.machine = "Syntax Error: format1¡G<INS>" ;
						notError = false ; 
					}
					
				}
				else if ( format == 2 ) {
					if ( InsGroupXE( tokens.at( curLine-1 ).tkns[pos].token, grp.size() ) ) {
						if ( findRegNum( tokens.at( curLine-1 ).tkns[pos+1].token ) != "" ) {
							if ( grp.size() == 2 ) {
								if ( findRegNum( tokens.at( curLine-1 ).tkns[pos+1].token ) != "" ) {
									strCalculate( locctr, 2 ) ;
									Digitloc( locctr, 4 ) ;
								} 
								else {
									temp.loc = "" ;
									temp.machine = "Syntax Error: format2¡G<INS> r1 {, r2}" ;
									notError = false ; 
								}
							}
							else if ( grp.size() == 1 ) {
								strCalculate( locctr, 2 ) ;
								Digitloc( locctr, 4 ) ;
							}
						}
					}
					else {
						temp.loc = "" ;
						temp.machine = "Syntax Error: format2¡G<INS> r1 {, r2}" ;
						notError = false ; 
					}
				}
				else if ( format == 3 ) {
					if ( InsGroupXE( tokens.at( curLine-1 ).tkns[pos].token, grp.size() ) ) {
						if ( grp.size() == 0 ) {
							strCalculate( locctr, 3 ) ;
							Digitloc( locctr, 4 ) ;
						}
						else if ( grp.size() == 2 ) {
							if ( stricmp( grp.at( 1 ).token.c_str(), "X" ) == 0 && tokens.at( curLine-1 ).tkns[pos+1].type == 5 ) {
								strCalculate( locctr, 3 ) ;
								Digitloc( locctr, 4 ) ;
							}
							else {
								if ( tokens.at( curLine-1 ).tkns[pos+1].token == "#" ) {
									if ( tokens.at( curLine-1 ).tkns[pos+1].type == 5 ) {
										strCalculate( locctr, 3 ) ;
										Digitloc( locctr, 4 ) ;
									} 
									else if ( isDec( tokens.at( curLine-1 ).tkns[pos+1].token ) ) {
										strCalculate( locctr, 3 ) ;
										Digitloc( locctr, 4 ) ;
									}
									else {
										temp.loc = "" ;
										temp.machine = "Syntax Error: format3¡G<INS> <m>" ;
										notError = false ; 
									}
								}
								else if ( tokens.at( curLine-1 ).tkns[pos+1].token == "@" ) {
									if ( tokens.at( curLine-1 ).tkns[pos+1].type == 5 ) {
										strCalculate( locctr, 3 ) ;
										Digitloc( locctr, 4 ) ;
									} 
									else {
										temp.loc = "" ;
										temp.machine = "Syntax Error: format3¡G<INS> <m>" ;
										notError = false ; 
									}
								}
								else if ( tokens.at( curLine-1 ).tkns[pos+1].token == "=" ) {
									if ( tokens.at( curLine-1 ).tkns[pos+2].token == "\'" && tokens.at( curLine-1 ).tkns[pos+4].token == "\'" ) {
										if ( tokens.at( curLine-1 ).tkns[pos+3].type == ( 6 || 7 ) ) {
											strCalculate( locctr, 3 ) ;
											Digitloc( locctr, 4 ) ;
										}
										else {
											temp.loc = "" ;
											temp.machine = "Syntax Error: format3¡G<INS> <m>" ;
											notError = false ; 
										}
									} 
									else if ( isDec( tokens.at( curLine-1 ).tkns[pos+2].token ) ) {
										strCalculate( locctr, 3 ) ;
										Digitloc( locctr, 4 ) ;
									}
									else {
										temp.loc = "" ;
										temp.machine = "Syntax Error: format3¡G<INS> <m>" ;
										notError = false ; 
									}
								}
								else {
									temp.loc = "" ;
									temp.machine = "Syntax Error: format3¡G<INS> <m>" ;
									notError = false ;	
								}
								
							}
						}
						else {
							strCalculate( locctr, 3 ) ;
							Digitloc( locctr, 4 ) ;
						}
					}
					else {
						temp.loc = "" ;
						temp.machine = "Syntax Error: format3¡G<INS> <m>" ;
						notError = false ; 
					}
				}
				else if ( format == 4 ) {
					if ( InsGroupXE( tokens.at( curLine-1 ).tkns[pos].token, grp.size() ) ) {
						if ( grp.size() == 0 ) {
							strCalculate( locctr, 4 ) ;
							Digitloc( locctr, 4 ) ;
						}
						else if ( grp.size() == 2 ) {
							if ( stricmp( grp.at( 1 ).token.c_str(), "X" ) == 0 && tokens.at( curLine-1 ).tkns[pos+1].type == 5 ) {
								strCalculate( locctr, 4 ) ;
								Digitloc( locctr, 4 ) ;
							}
							else {
								if ( tokens.at( curLine-1 ).tkns[pos+1].token == "#" ) {
									if ( tokens.at( curLine-1 ).tkns[pos+1].type == 5 ) {
										strCalculate( locctr, 4 ) ;
										Digitloc( locctr, 4 ) ;
									} 
									else if ( isDec( tokens.at( curLine-1 ).tkns[pos+1].token ) ) {
										strCalculate( locctr, 4 ) ;
										Digitloc( locctr, 4 ) ;
									}
									else {
										temp.loc = "" ;
										temp.machine = "Syntax Error: format4¡G+<INS> <m>" ;
										notError = false ; 
									}
								}
								else if ( tokens.at( curLine-1 ).tkns[pos+1].token == "@" ) {
									if ( tokens.at( curLine-1 ).tkns[pos+1].type == 5 ) {
										strCalculate( locctr, 4 ) ;
										Digitloc( locctr, 4 ) ;
									} 
									else {
										temp.loc = "" ;
										temp.machine = "Syntax Error: format4¡G+<INS> <m>" ;
										notError = false ; 
									}
								}
								else if ( tokens.at( curLine-1 ).tkns[pos+1].token == "=" ) {
									if ( tokens.at( curLine-1 ).tkns[pos+2].token == "\'" && tokens.at( curLine-1 ).tkns[pos+4].token == "\'" ) {
										if ( tokens.at( curLine-1 ).tkns[pos+3].type == ( 6 || 7 ) ) {
											strCalculate( locctr, 4 ) ;
											Digitloc( locctr, 4 ) ;
										}
										else {
											temp.loc = "" ;
											temp.machine = "Syntax Error: format4¡G+<INS> <m>" ;
											notError = false ; 
										}
									} 
									else if ( isDec( tokens.at( curLine-1 ).tkns[pos+2].token ) ) {
										strCalculate( locctr, 4 ) ;
										Digitloc( locctr, 4 ) ;
									}
									else {
										temp.loc = "" ;
										temp.machine = "Syntax Error: format4¡G+<INS> <m>" ;
										notError = false ; 
									}
								}
								else {
									temp.loc = "" ;
									temp.machine = "Syntax Error: format4¡G+<INS> <m>" ;
									notError = false ;	
								}
								
							}
						}
						else {
							strCalculate( locctr, 4 ) ;
							Digitloc( locctr, 4 ) ;
						}
					}
					else {
						temp.loc = "" ;
						temp.machine = "Syntax Error: format4¡G+<INS> <m>" ;
						notError = false ; 
					}
				}
			}
			else if ( stricmp( ins.c_str(), "WORD" ) == 0 ) {
				int length = 3 ;
				if ( tokens.at( curLine-1 ).tkns[pos+1].token == "\'" && tokens.at( curLine-1 ).tkns[pos+3].token == "\'" ) { 
					if ( tokens.at( curLine-1 ).tkns[pos+1].type != 5 && tokens.at( curLine-1 ).tkns[pos+1].type != 6 ) {
						temp.loc = "" ;
						temp.machine = "Lexical Error:{label} WORD X''¡K | C''¡K | dec_num" ;
						notError = false ;
						length = 0 ;
					}
				}
				else if ( !isDec( tokens.at( curLine-1 ).tkns[pos+1].token ) ) {
					temp.loc = "" ;
					temp.machine = "Lexical Error:{label} WORD X''¡K | C''¡K | dec_num" ;
					notError = false ;
					length = 0 ;
				}
				strCalculate( locctr, length ) ;
				Digitloc( locctr, 4 ) ;
			}
			else if ( stricmp( ins.c_str(), "RESW" ) == 0 ) {
				if ( isDec( tokens.at( curLine-1 ).tkns[pos+1].token ) ) {
					string str = tokens.at( curLine-1 ).tkns[pos+1].token ;
					int num ;
					stringstream ss ;
					ss << str ;
					ss >> num ;
					strCalculate( locctr, num * 3 ) ;	
					Digitloc( locctr, 4 ) ;
				}
				else {
					temp.loc = "" ;
					temp.machine = "{label} RESW dec_num" ;
					notError = false ;
				}
			}
			else if ( stricmp( ins.c_str(), "RESB" ) == 0 ) {
				if ( isDec( tokens.at( curLine-1 ).tkns[pos+1].token ) ) {
					string str = tokens.at( curLine-1 ).tkns[pos+1].token ;
					int num ;
					stringstream ss ;
					ss << str ;
					ss >> num ;
					strCalculate( locctr, num ) ;
					Digitloc( locctr, 4 ) ;
				}
				else {
					temp.loc = "" ;
					temp.machine = "{label} RESB dec_num" ;
					notError = false ;
				}
			}
			else if ( stricmp( ins.c_str(), "BYTE" ) == 0 ) {
				int length = 0 ;
				if ( tokens.at( curLine-1 ).tkns[pos+1].token == "\'" && tokens.at( curLine-1 ).tkns[pos+3].token == "\'" ) {
					if ( tokens.at( curLine-1 ).tkns[pos+2].type == 6 )  {
						length = tokens.at( curLine-1 ).tkns[pos+2].token.size() / 2 ;
					}
					else if ( tokens.at( curLine-1 ).tkns[pos+2].type == 7 ) {
						length = tokens.at( curLine-1 ).tkns[pos+2].token.size() ;
					}
					else {
						temp.loc = "" ;
						temp.machine = "Lexical Error:{label} BYTE X''¡K | C''¡K | dec_num"  ;
						length = 0 ;
						notError = false ;
					}	
				}
				else {
					if ( !isDec( tokens.at( curLine-1 ).tkns[pos+1].token) ) {
						length = turnHex( tokens.at( curLine-1 ).tkns[pos+1].token ).length() / 2 ;
					}
					else {
						temp.loc = "" ;
						temp.machine = "Lexical Error:{label} BYTE X''¡K | C''¡K | dec_num"  ;
						length = 0 ;
						notError = false ;
					}
				}
				strCalculate( locctr, length ) ;
				Digitloc( locctr, 4 ) ;
			}
			else if ( stricmp( ins.c_str(), "LTORG" ) == 0 ) {
				temp.loc = "" ;
			}
			else {
				temp.loc = "" ;
				temp.machine = "Syntax Error : Invalid Operation Code" ;
			}
			
			if ( stricmp( ins.c_str(), "LTORG" ) == 0 ) {
				final.push_back( temp ) ;
				doit = true ;
				getLitAddr() ;
			}

			if ( tokens.at( curLine-1 ).tkns.size() > pos+1 && tokens.at( curLine-1 ).tkns[pos+1].token == "=" && notError ) {
				literal lit_temp ;
				if ( tokens.at( curLine-1 ).tkns[pos+2].token == "\'" && tokens.at( curLine-1 ).tkns[pos+4].token == "\'" ) {
					lit_temp.name = tokens.at( curLine-1 ).tkns[pos+3].token ;
					lit_temp.type = tokens.at( curLine-1 ).tkns[pos+3].type ;
					lit_temp.address = "" ;
					if ( lit_temp.type == 6 ) {
						lit_temp.inputL = "=X\'" + tokens.at( curLine-1 ).tkns[pos+3].token + "\'" ;
						makeLiteral( lit_temp ) ;
					}	
					else if ( lit_temp.type == 7 ) {
						lit_temp.inputL = "=C\'" + tokens.at( curLine-1 ).tkns[pos+3].token + "\'" ;
						makeLiteral( lit_temp ) ;
					}						
					else {
						temp.loc = "" ;
						temp.machine = "Syntax Error: = X''¡K | C''¡K " ;
						notError = false ;
					}
				}
				else if ( isDec( tokens.at( curLine-1 ).tkns[pos+2].token ) ){
					lit_temp.name = tokens.at( curLine-1 ).tkns[pos+2].token ;
					lit_temp.type = 8 ;
					lit_temp.address = "" ;
					lit_temp.inputL = "=" + tokens.at( curLine-1 ).tkns[pos+2].token ;
					makeLiteral( lit_temp ) ;
				}
				else {
					temp.loc = "" ;
					temp.machine = "Syntax Error: = X''¡K | C''¡K " ;
					notError = false ;
				}
				
			}
			
			if ( !doit )
				final.push_back( temp ) ;
			else
				doit = false ;	
			
			if ( notError ) {
				symtab.push_back( tmp ) ;
			}
			
			curLine++ ;
			grp.clear() ;
		}	
	}
	
	bool isArthimetic( vector <Token> tokens, int pos, vector <Token> & group ) {
		int count = 0 ;
		for ( int i = pos+1 ; i < tokens.size() ; i++ ) {
			if ( count % 2 == 0 && tokens.at( i ).type == 5 ) 
				group.push_back( tokens.at( i ) ) ;
			else if ( count % 2 == 1 && ( tokens.at( i ).token == "+" || tokens.at( i ).token == "-" 
					|| tokens.at( i ).token == "*" || tokens.at( i ).token == "/" ) )
				group.push_back( tokens.at( i ) ) ;
			else {
				group.clear() ;
				return false ;
			}
			count++ ;	
		}
		
		if ( count % 2 == 0 ) {
			group.clear() ;
			return false ;
		}
		else
			return true ;
	}
	
	bool getAddress( vector <Token> group, string & address ) {
		int current = 0, sec = 0 ;
		symbl sym ;
		bool first = true ;
		string operand ;
		for ( int i = 0 ; i < group.size() ; i++ ) {
			if ( group.at( i ).type == 5 ) {
				if ( findSym( group.at( i ).token, sym ) ) {
					if ( first ) {
						current = stringHexToInt( sym.address ) ;
						first = false ;
					}	
					else {
						sec = stringHexToInt( sym.address ) ;
						if ( operand == "+" )
							current = current + sec ;
						else if ( operand == "-" )
							current = current - sec ;
						else if ( operand == "*" )
							current = current * sec ;
						else if ( operand == "/" )
							current = current / sec ;			
					}
						
				}
				else
					return false ;
			}
			else if ( group.at( i ).token == "+" )
				operand = "+" ;
			else if ( group.at( i ).token == "-" )
				operand = "-" ;
			else if ( group.at( i ).token == "*" )
				operand = "*" ;
			else if ( group.at( i ).token == "/" )
				operand = "/" ;			
		}
		
		if ( current >= 0 ) {
			address = IntToStringHex( current ) ;
			Digitloc( address, 4 ) ;
		}
		else {
			
			address = IntToStringHex( current ) ;
			address = address.substr( 5 ) ;
		}
		return true ;
	}
	
	int findOpFormat( vector <Token> tokens, int pos ) {
		int format ;
		for ( int i = 0 ; i < optab.size() ; i++ ) {
			if ( stricmp( tokens[pos].token.c_str(), optab.at( i ).operation.c_str() ) == 0 ) {
				format = optab.at( i ).format ;
			}
		}
		if ( format == 34 ) {
			if ( pos == 0 )
				format = 3 ;
			else if ( tokens[pos-1].token == "+" )
				format = 4 ;
			else
				format = 3 ;	
		}	
		return format ;
	}
	
	bool findOp( string opcode, op & get ) {			
		for ( int i = 0 ; i < optab.size() ; i++ ) {
			if ( stricmp( opcode.c_str(), optab.at( i ).operation.c_str() ) == 0 ) {
				get = optab.at( i ) ;
				return true ;
			}
		}
		
		return false ;
	}
	
	bool findSym( string sym, symbl & get ) {
		for ( int i = 0 ; i < symtab.size() ; i++ ) {
			if ( sym == symtab.at( i ).symbol ) {
				get = symtab.at( i ) ;
				return true ;
			}
		}
		
		return false ;
	}
	
	int findSymPos( string sym ) {
		for ( int i = 0 ; i < symtab.size() ; i++ ) {
			if ( sym == symtab.at( i ).symbol ) {
				return i ;
			}
		}
		
		return -1 ;
	}
	
	bool findLiteral( string lit, int type, literal & get ) {
		for ( int i = 0 ; i < 100 ; i++ ) {
			if ( lit == littab[i].name && type == littab[i].type ) {
				get = littab[i] ;
				return true ;
			}
		}
		return false ;
	}
	
	void GetGroups( vector <Token> token, int pos, vector <group> & grp ) {
		group temp ;
		temp.start = pos ;
		temp.end = pos ;
		temp.length = 0 ;
		temp.token = "" ;
		for ( int i = pos ; i < token.size() ; i++ ) {
			if ( temp.length == 0 ) {
				temp.start = i ;
				temp.end = i ;
				temp.length = 1 ;
				temp.token = token.at( i ).token ;
			}
			else if ( token.at( i ).token == "," ) {
				grp.push_back( temp ) ;
				temp.start = i+1 ;
				temp.end = i+1 ;
				temp.length = 0 ;
				temp.token = "" ;
			}
			else {
				temp.end++ ;
				temp.length++ ;
				temp.token = temp.token + token.at( pos ).token ;
			}	
		}
		
		if ( temp.token != "" ) {
			grp.push_back( temp ) ;
		}		
	}
	
	string findRegNum( string reg ) {
		if ( stricmp( reg.c_str(), "A" ) == 0 )
			return "0" ;
		if ( stricmp( reg.c_str(), "X" ) == 0 )
			return "1" ;
		if ( stricmp( reg.c_str(), "L" ) == 0 )
			return "2" ;
		if ( stricmp( reg.c_str(), "B" ) == 0 )
			return "3" ;
		if ( stricmp( reg.c_str(), "S" ) == 0 )
			return "4" ;
		if ( stricmp( reg.c_str(), "T" ) == 0 )
			return "5" ;
		if ( stricmp( reg.c_str(), "F" ) == 0 )
			return "6" ;
		if ( stricmp( reg.c_str(), "PC" ) == 0 )
			return "8" ;
		if ( stricmp( reg.c_str(), "SW" ) == 0 )
			return "9" ;
		return "" ;		
	}
	
	Line getLines( int line, int & store ) {
		Line temp ;
		for ( int i = 0 ; i < tokens.size() ; i++ ) {
			if ( tokens.at( i ).num == line ) {
				temp = tokens.at( i ) ;
				break ;
			}
		}
		
		for ( int j = 0 ; j < final.size() ; j++ ) {
			if ( line == final.at( j ).line ) {
				store = j ;
				break ;
			}			
		}
		
		return temp ;
	} 
	
	string turnHex( string str ) {
		int temp ;
		stringstream ss ;
    	ss << str ; 
    	ss >> temp ;
    	ss.clear() ;
    	ss << hex << temp ;
    	string change, final ;
    	ss >> change ;
		transform(change.begin(), change.end(), back_inserter(final), ::toupper);
		return final ;
	}
	
	bool InsGroup( string ins, int group ) {
		if ( stricmp( ins.c_str(), "RSUB" ) == 0 )
			if ( group == 0 )
				return true ;
			else
				return false ;
		else 
			if ( group == 1 || group == 2 )
				return true ;
			else
				return false ; 
	}
	
	bool InsGroupXE( string ins, int group ) {
		if ( stricmp( ins.c_str(), "FIX" ) == 0 || stricmp( ins.c_str(), "FLOAT" ) == 0 || stricmp( ins.c_str(), "HIO" ) == 0 
			|| stricmp( ins.c_str(), "NORM" ) == 0 || stricmp( ins.c_str(), "RSUB" ) == 0 || stricmp( ins.c_str(), "SIO" ) == 0
			|| stricmp( ins.c_str(), "TIO" ) == 0 )
			if ( group == 0 )
				return true ;
			else
				return false ;
		else if ( stricmp( ins.c_str(), "CLEAR" ) == 0 || stricmp( ins.c_str(), "SVC" ) == 0 || stricmp( ins.c_str(), "TIXR" ) == 0 )
			if ( group == 1 )
				return true ;
			else
				return false ;
		else if ( stricmp( ins.c_str(), "ADDR" ) == 0 || stricmp( ins.c_str(), "COMPR" ) == 0 || stricmp( ins.c_str(), "DIVR" ) == 0 
				|| stricmp( ins.c_str(), "MULR" ) == 0 || stricmp( ins.c_str(), "RMO" ) == 0 || stricmp( ins.c_str(), "SHIFTL" ) == 0
				|| stricmp( ins.c_str(), "SHIFTR" ) == 0 || stricmp( ins.c_str(), "SUBR" ) == 0 )
			if ( group == 2 )
				return true ;
			else
				return false ;
		else 
			if ( group == 1 || group == 2 )
				return true ;
			else
				return false ; 
	}
	
	void pass2() {
		int num = 0, pos = 0 ;
		if ( getIns( final.at( num ).line, pos ) == "START" )
			num++ ;
		for (; num < final.size() ; num++ ) {
			int storeLine ;
			Line curLine = getLines( final.at( num ).line, storeLine ) ;
			string ins = getIns( curLine.num, pos ) ;
			if ( ins == "END" )
				break ;
				
			op mac ;
			symbl symbol ;
			vector <group> tokenGroup ;
			if ( final.at( storeLine ).machine != "" ) ; 
			else if ( findOp( ins, mac ) ) {
				GetGroups( curLine.tkns, pos+1, tokenGroup ) ;
				if ( InsGroup( curLine.tkns[pos].token, tokenGroup.size() ) ) {
					if ( tokenGroup.size() == 0 )
						final.at( storeLine ).machine = mac.machine + "0000" ;	
					else if ( findSym( curLine.tkns[pos+1].token, symbol ) ) {
						if ( tokenGroup.size() == 1 )
							final.at( storeLine ).machine = mac.machine + symbol.address ;
						else if ( stricmp( tokenGroup.at( 1 ).token.c_str(), "X" ) == 0 ) {
							char small = symbol.address[0] ;
							string str = "" ;
							str = str + small ;
							strCalculate( str, 8 ) ;
							string tempstr = symbol.address ;
							small = str[0] ;
							tempstr[0] = small ;
							final.at( storeLine ).machine = mac.machine + tempstr ;
						}
						else 
							final.at( storeLine ).machine = "Syntax Error" ;
					}
					else
						final.at( storeLine ).machine = "Error: undefined symbol";
				}
				else
					final.at( storeLine ).machine = "Syntax Error" ;	
			}
			else if ( ins == "WORD" ) {
				string addr = turnHex( curLine.tkns[pos+1].token ) ;
				Digitloc( addr, 6 ) ;
				final.at( storeLine ).machine = addr ;
			}
			else if ( ins == "BYTE" ) {
				if ( curLine.tkns[pos+2].type == 6 )  
					final.at( storeLine ).machine = curLine.tkns[pos+2].token ;
				else if ( curLine.tkns[pos+2].type == 7 ) {
					for ( int j = 0 ; j < curLine.tkns[pos+2].token.length() ; j++ ) {
						char ch = curLine.tkns[pos+2].token[j] ;
						int ascii = (int)ch ;
						stringstream ss ;
						ss << hex << ascii ;
    					string change, hexi ;
    					ss >> change ;
						transform(change.begin(), change.end(), back_inserter(hexi), ::toupper);
						Digitloc( hexi, 2 ) ;
						final.at( storeLine ).machine = final.at( storeLine ).machine + hexi ;
					} 
				}
			}
				
		}
	}
	
	void pass2xe() {
		int num = 0, pos = 0 ;
		if ( getInsXE( final.at( num ).line, pos ) == "START" )
			num++ ;
		for (; num < final.size() ; num++ ) {
			int storeLine ;
			Line curLine = getLines( final.at( num ).line, storeLine ) ;
			string ins = getInsXE( curLine.num, pos ) ;
			locctr = final.at( storeLine ).loc ;
			if ( ins == "END" )
				break ;
				
			op mac ;
			symbl symbol ;
			literal lit ;
			vector <group> tokenGroup ;
			if ( final.at( storeLine ).machine != "" ) ;  
			else if ( findOp( ins, mac ) ) {
				int format = findOpFormat( curLine.tkns, pos ) ;
				GetGroups( curLine.tkns, pos+1, tokenGroup ) ;
				if ( !InsGroupXE( curLine.tkns[pos].token, tokenGroup.size() ) )
					final.at( storeLine ).machine = "Syntax Error" ;
				else if ( format == 1 ) {
					final.at( storeLine ).machine = mac.machine ;
				}
				else if ( format == 2 ) {
					if ( tokenGroup.size() > 2 ) 
						final.at( storeLine ).machine = "Error : There is over threee token group" ;
					else if ( tokenGroup.size() == 1 ) {
						string reg = findRegNum( curLine.tkns[tokenGroup.at( 0 ).start].token ) ;
						final.at( storeLine ).machine = mac.machine + reg + "0" ;
					}
					else { // tokengroup = 2
						string reg1 = findRegNum( curLine.tkns[tokenGroup.at( 0 ).start].token ) ;
						string reg2 = findRegNum( curLine.tkns[tokenGroup.at( 1 ).start].token ) ;
						final.at( storeLine ).machine = mac.machine + reg1 + reg2 ;	
					}
				}
				else if ( format == 3 ) {
					bool isPC ;
					string disp ;
					if ( tokenGroup.size() == 0 ) {
						char small = mac.machine[1] ;
						string str = "" ;
						str = str + small ;
						strCalculate( str, 3 ) ;
						string tempstr = " " ;
						small = str[0] ;
						tempstr[0] = small ;
						final.at( storeLine ).machine = mac.machine[0] + tempstr + "0000" ;
					}
					else if ( tokenGroup.at( 0 ).token[0] == '#' ) {
						char small = mac.machine[1] ;
						string str = "" ;
						str = str + small ;
						strCalculate( str, 1 ) ;
						string tempstr = " " ;
						small = str[0] ;
						tempstr[0] = small ;
						if ( curLine.tkns[pos+2].type == 6 ) {
							disp = turnHex( curLine.tkns[pos+2].token ) ;
							Digitloc( disp, 3 ) ;
							final.at( storeLine ).machine = mac.machine[0] + tempstr + "0" + disp ;				
						}
						else if ( curLine.tkns[pos+2].type == 5 ) {
							if ( findSym( curLine.tkns[pos+2].token, symbol ) ) {
								disp = getDisp( locctr, symbol.address, isPC ) ;
								if ( disp != "No" ) {
									if ( isPC )
										final.at( storeLine ).machine = mac.machine[0] + tempstr + "2" + disp ;
									else
										final.at( storeLine ).machine = mac.machine[0] + tempstr + "4" + disp ;	
									
								}
								else
									final.at( storeLine ).machine = "Error: Overflow" ;
							}
							else
								final.at( storeLine ).machine = "Error: undefined symbol" ;	
						}
						else
							final.at( storeLine ).machine = "Syntax Error" ;
					}
					else if ( tokenGroup.at( 0 ).token[0] == '@' ) {
						char small = mac.machine[1] ;
						string str = "" ;
						str = str + small ;
						strCalculate( str, 2 ) ;
						string tempstr = " " ;
						small = str[0] ;
						tempstr[0] = small ;
						if ( curLine.tkns[pos+2].type == 5 ) {
							if ( findSym( curLine.tkns[pos+2].token, symbol ) ) {
								disp = getDisp( locctr, symbol.address, isPC ) ;
								if ( disp != "No" ) {
									if ( isPC )
										final.at( storeLine ).machine = mac.machine[0] + tempstr + "2" + disp ;
									else
										final.at( storeLine ).machine = mac.machine[0] + tempstr + "4" + disp ;	
									
								}
								else
									final.at( storeLine ).machine = "Error: Overflow" ;
							}
							else
								final.at( storeLine ).machine = "Error: undefined symbol" ;	
						}
						else
							final.at( storeLine ).machine = "Syntax Error" ;
					}
					else if ( tokenGroup.at( 0 ).token[0] == '=' ) {
						char small = mac.machine[1] ;
						string str = "" ;
						str = str + small ;
						strCalculate( str, 3 ) ;
						string tempstr = " " ;
						small = str[0] ;
						tempstr[0] = small ;
						if ( curLine.tkns[pos+2].token == "\'" && curLine.tkns[pos+4].token == "\'" ) {
							if ( findLiteral( curLine.tkns[pos+3].token, curLine.tkns[pos+3].type, lit ) ) {
								disp = getDisp( locctr, lit.address, isPC ) ;
								if ( disp != "No" ) {
									if ( isPC )
										final.at( storeLine ).machine = mac.machine[0] + tempstr + "2" + disp ;
									else
										final.at( storeLine ).machine = mac.machine[0] + tempstr + "4" + disp ;	
									
								}
								else
									final.at( storeLine ).machine = "Error: Overflow" ;
							}
							else 
								final.at( storeLine ).machine = "Error: Literal Not Definition" ;
						}
						else {
							if ( findLiteral( curLine.tkns[pos+2].token, 8, lit ) ) {
								disp = getDisp( locctr, lit.address, isPC ) ;
								if ( disp != "No" ) {
									if ( isPC )
										final.at( storeLine ).machine = mac.machine[0] + tempstr + "2" + disp ;
									else
										final.at( storeLine ).machine = mac.machine[0] + tempstr + "4" + disp ;	
									
								}
								else
									final.at( storeLine ).machine = "Error: Overflow" ;
							}
							else 
								final.at( storeLine ).machine = "Error: Literal Not Definition" ;
						}
						
					}
					else if ( curLine.tkns[pos+1].type == 5 ) {
						if ( findSym( curLine.tkns[pos+1].token, symbol ) ) {
							if ( tokenGroup.size() > 2 ) 
								final.at( storeLine ).machine = "Error : There is over threee token group" ;
							else if ( tokenGroup.size() == 1 ) {
								char small = mac.machine[1] ;
								string str = "" ;
								str = str + small ;
								strCalculate( str, 3 ) ;
								string tempstr = " " ;
								small = str[0] ;
								tempstr[0] = small ;
								disp = getDisp( locctr, symbol.address, isPC ) ;
								if ( disp != "No" ) {
									if ( isPC )
										final.at( storeLine ).machine = mac.machine[0] + tempstr + "2" + disp ;
									else
										final.at( storeLine ).machine = mac.machine[0] + tempstr + "4" + disp ;	
									
								}
								else
									final.at( storeLine ).machine = "Error: Overflow" ;
							}
							else if ( stricmp( tokenGroup.at( 1 ).token.c_str(), "X" ) == 0 ) {
								char small = mac.machine[1] ;
								string str = "" ;
								str = str + small ;
								strCalculate( str, 3 ) ;
								string tempstr = " " ;
								small = str[0] ;
								tempstr[0] = small ;
								disp = getDisp( locctr, symbol.address, isPC ) ;
								if ( disp != "No" ) {
									if ( isPC )
										final.at( storeLine ).machine = mac.machine[0] + tempstr + "A" + disp ;
									else
										final.at( storeLine ).machine = mac.machine[0] + tempstr + "C" + disp ;	
									
								}
								else
									final.at( storeLine ).machine = "Error: Overflow" ;
							}
							else
								final.at( storeLine ).machine = "Syntax Error" ;		
						}
						else
							final.at( storeLine ).machine = "Error: undefined symbol" ;
					}
					else if ( curLine.tkns[pos+1].type == 6 ) {
						char small = mac.machine[1] ;
						string str = "" ;
						str = str + small ;
						strCalculate( str, 3 ) ;
						string tempstr = " " ;
						small = str[0] ;
						tempstr[0] = small ;
						if ( curLine.tkns[pos].token == "comp" && curLine.tkns[pos+1].token == "0" ) {
							disp = curLine.tkns[pos+1].token ;
							Digitloc( disp, 3 ) ;
							final.at( storeLine ).machine = mac.machine[0] + tempstr + "0" + disp ;
						}
						else {
							disp = getDisp( locctr, curLine.tkns[pos+1].token, isPC ) ;
							if ( disp != "No" ) {
								if ( isPC )
									final.at( storeLine ).machine = mac.machine[0] + tempstr + "A" + disp ;
								else
									final.at( storeLine ).machine = mac.machine[0] + tempstr + "C" + disp ;	
									
							}
							else
								final.at( storeLine ).machine = "Error: Overflow" ;
						}
						
					}
					else
						final.at( storeLine ).machine = "Syntax Error" ;
				}
				else if ( format == 4 ) {
					if ( tokenGroup.size() == 0 ) {
						char small = mac.machine[1] ;
						string str = "" ;
						str = str + small ;
						strCalculate( str, 3 ) ;
						string tempstr = " " ;
						small = str[0] ;
						tempstr[0] = small ;
						final.at( storeLine ).machine = mac.machine[0] + tempstr + "000000" ;
					}
					else if ( tokenGroup.at( 0 ).token[0] == '#' ) {
						char small = mac.machine[1] ;
						string str = "" ;
						str = str + small ;
						strCalculate( str, 1 ) ;
						string tempstr = " " ;
						small = str[0] ;
						tempstr[0] = small ;
						string address ;
						if ( curLine.tkns[pos+2].type == 6 ) {
							address = turnHex( curLine.tkns[pos+2].token ) ;
							Digitloc( address, 5 ) ;
							final.at( storeLine ).machine = mac.machine[0] + tempstr + "1" + address ;				
						}
						else if ( curLine.tkns[pos+2].type == 5 ) {
							if ( findSym( curLine.tkns[pos+2].token, symbol ) ) {
								address = symbol.address ;
								Digitloc( address, 5 ) ;
								final.at( storeLine ).machine = mac.machine[0] + tempstr + "1" + address ;
							}
							else
								final.at( storeLine ).machine = "Error: undefined symbol" ;	
						}
						else
							final.at( storeLine ).machine = "Syntax Error" ;
					}
					else if ( tokenGroup.at( 0 ).token[0] == '@' ) {
						char small = mac.machine[1] ;
						string str = "" ;
						str = str + small ;
						strCalculate( str, 2 ) ;
						string tempstr = " " ;
						small = str[0] ;
						tempstr[0] = small ;
						string address ;
						if ( curLine.tkns[pos+2].type == 5 ) {
							if ( findSym( curLine.tkns[pos+2].token, symbol ) ) {
								address = symbol.address ;
								Digitloc( address, 5 ) ;
								final.at( storeLine ).machine = mac.machine[0] + tempstr + "1" + address ;
							}
							else
								final.at( storeLine ).machine = "Error: undefined symbol" ;	
						}
						else
							final.at( storeLine ).machine = "Syntax Error" ;
					}
					else if ( tokenGroup.at( 0 ).token[0] == '=' ) {
						char small = mac.machine[1] ;
						string str = "" ;
						str = str + small ;
						strCalculate( str, 3 ) ;
						string tempstr = " " ;
						small = str[0] ;
						tempstr[0] = small ;
						string address ;
						if ( curLine.tkns[pos+2].token == "\'" && curLine.tkns[pos+4].token == "\'" ) {
							if ( findLiteral( curLine.tkns[pos+3].token, curLine.tkns[pos+3].type, lit ) ) {
								address = lit.address ;
								Digitloc( address, 5 ) ;
								final.at( storeLine ).machine = mac.machine[0] + tempstr + "1" + address ;
							}
							else 
								final.at( storeLine ).machine = "Error: Literal Not Definition" ;
						}
						else {
							if ( findLiteral( curLine.tkns[pos+2].token, 8, lit ) ) {
								address = lit.address ;
								Digitloc( address, 5 ) ;
								final.at( storeLine ).machine = mac.machine[0] + tempstr + "1" + address ;
							}
							else 
								final.at( storeLine ).machine = "Error: Literal Not Definition" ;
						}
						
					}
					else if ( curLine.tkns[pos+1].type == 5 ) {
						if ( findSym( curLine.tkns[pos+1].token, symbol ) ) {
							if ( tokenGroup.size() > 2 ) 
								final.at( storeLine ).machine = "Error : There is over threee token group" ;
							else if ( tokenGroup.size() == 1 ) {
								char small = mac.machine[1] ;
								string str = "" ;
								str = str + small ;
								strCalculate( str, 3 ) ;
								string tempstr = " " ;
								small = str[0] ;
								tempstr[0] = small ;
								string address = symbol.address ;
								Digitloc( address, 5 ) ;
								final.at( storeLine ).machine = mac.machine[0] + tempstr + "1" + address ;
							}
							else if ( stricmp( tokenGroup.at( 1 ).token.c_str(), "X" ) == 0 ) {
								char small = mac.machine[1] ;
								string str = "" ;
								str = str + small ;
								strCalculate( str, 3 ) ;
								string tempstr = " " ;
								small = str[0] ;
								tempstr[0] = small ;
								string address = symbol.address ;
								Digitloc( address, 5 ) ;
								final.at( storeLine ).machine = mac.machine[0] + tempstr + "9" + address ;
							}
							else
								final.at( storeLine ).machine = "Syntax Error" ;		
						}
						else
							final.at( storeLine ).machine = "Error: undefined symbol" ;
					}
					else if ( curLine.tkns[pos+1].type == 6 ) {
						char small = mac.machine[1] ;
						string str = "" ;
						str = str + small ;
						strCalculate( str, 3 ) ;
						string tempstr = " " ;
						small = str[0] ;
						tempstr[0] = small ;
						string address = curLine.tkns[pos+1].token ;
						Digitloc( address, 5 ) ;
						final.at( storeLine ).machine = mac.machine[0] + tempstr + "1" + address ;
					}
					else
						final.at( storeLine ).machine = "Syntax Error" ;
				}		
			}
			else if ( ins == "BASE" ) {
				if ( curLine.tkns[pos+1].type == 5 ) {
					if ( findSym( curLine.tkns[pos+1].token, symbol ) )
						base = symbol.address ;
					else
						final.at( storeLine ).machine = "Error: undefined symbol" ;
				}
				else if ( isDec( curLine.tkns[pos+1].token ) )
					base = turnHex( curLine.tkns[pos+1].token ) ;
				else
					final.at( storeLine ).machine = "Syntax Error: " ;	
			}
			else if ( ins == "WORD" ) {
				string addr = "" ;
				if ( curLine.tkns[pos+1].token == "\'" && curLine.tkns[pos+3].token == "\'" ) {
					if ( curLine.tkns[pos+2].type == 6 ) {
						addr = curLine.tkns[pos+2].token ;
					}
					else if ( curLine.tkns[pos+2].type == 7 ) {
						for ( int j = 0 ; j < curLine.tkns[pos+2].token.length() ; j++ ) {
							char ch = curLine.tkns[pos+2].token[j] ;
							int ascii = (int)ch ;
							stringstream ss ;
							ss << hex << ascii ;
    						string change, hexi ;
    						ss >> change ;
							transform(change.begin(), change.end(), back_inserter(hexi), ::toupper);
							Digitloc( hexi, 2 ) ;
							addr = addr + hexi ;
						}
					}
				}
				else
					addr = turnHex( curLine.tkns[pos+1].token ) ;
				Digitloc( addr, 6 ) ;
				final.at( storeLine ).machine = addr ;
			}
			else if ( ins == "BYTE" ) {
				if ( curLine.tkns[pos+1].token == "\'" && curLine.tkns[pos+3].token == "\'" ) {
					if ( curLine.tkns[pos+2].type == 6 )  
						final.at( storeLine ).machine = curLine.tkns[pos+2].token ;
					else if ( curLine.tkns[pos+2].type == 7 ) {
						for ( int j = 0 ; j < curLine.tkns[pos+2].token.length() ; j++ ) {
							char ch = curLine.tkns[pos+2].token[j] ;
							int ascii = (int)ch ;
							stringstream ss ;
							ss << hex << ascii ;
    						string change, hexi ;
    						ss >> change ;
							transform(change.begin(), change.end(), back_inserter(hexi), ::toupper);
							Digitloc( hexi, 2 ) ;
							final.at( storeLine ).machine = final.at( storeLine ).machine + hexi ;
						} 
					}
					else
						final.at( storeLine ).machine = "Lexical Error" ;
				}
				else
					final.at( storeLine ).machine = turnHex( curLine.tkns[pos+1].token ) ;
			}		
		}
	}
	
	bool hasLiteral( vector <Token> tkns ) {
		for ( int i = 0 ; i < tkns.size() ; i++ ) {
			if ( stricmp( tkns.at( i ).token.c_str(), "LTORG" ) == 0 
				|| stricmp( tkns.at( i ).token.c_str(), "END" ) == 0 )
				return true ;
		}
		return false ;
	}
	
	void print() {
		cout << endl << "Line	Loc	Source statement		Object code" << endl ;
		cout << endl ;
		int j = 0 ;
		for ( int i = 0 ; i < tokens.size() ; i++ ) {
			bool isCom ;
			if ( j >= final.size() )
				break ;
			
			while ( j < final.size() ) {
				if ( final.at( j ).line < i+1 )
					j++ ;
				else if ( final.at( j ).line == i+1 ) {
					isCom = true ;
					break ;
				}
				else if ( final.at( j ).line > i+1 ) {
					isCom = false ;
					break ;
				}
			}
			
			cout << setw(3) << (i+1) * 5 << "	" ;
			if ( isCom )
				cout << final.at( j ).loc << "	" << tokens.at( i ).origin << "		" << final.at( j ).machine << endl ;
			else	
				cout << "	" << tokens.at( i ).origin << endl ;
			if ( hasLiteral( tokens.at( i ).tkns ) ) {
				while ( j+1 < final.size() ) {
					j++ ;
					if ( final.at( j ).line == -1 ) {
						cout << "	" << final.at( j ).loc << "	*" << final.at( j ).inputL ;
						cout << "		" << final.at( j ).machine << endl ; 
					}	
					else
						break ;
				}
			}
		}
	}
	
	void outFile( string fileName ) {
		fstream outFile ;
		outFile.open( fileName.c_str(), fstream::out );
		outFile << "Line	Loc	Source statement		Object code" << endl ;
		outFile << endl ;
		int j = 0 ;	
		for ( int i = 0 ; i < tokens.size() ; i++ ) {
			bool isCom ;
			if ( j == final.size() )
				break ;
			
			while ( j < final.size() ) {
				if ( final.at( j ).line < i+1 )
					j++ ;
				else if ( final.at( j ).line == i+1 ) {
					isCom = true ;
					break ;
				}
				else if ( final.at( j ).line > i+1 ) {
					isCom = false ;
					break ;
				}
			}
			
			outFile << setw(3) << (i+1) * 5 << "	" ;
			if ( isCom )
				outFile << final.at( j ).loc << "	" << tokens.at( i ).origin << "		" << final.at( j ).machine << endl ;
			else	
				outFile << "	" << tokens.at( i ).origin << endl ;
			if ( hasLiteral( tokens.at( i ).tkns ) ) {
				while ( j+1 < final.size() ) {
					j++ ;
					if ( final.at( j ).line == -1 ) {
						outFile << "	" << final.at( j ).loc << "	*" << final.at( j ).inputL ;
						outFile << "		" << final.at( j ).machine << endl ; 
					}	
					else
						break ;
				}
			}
		}
		
		outFile.close();
	}
	
	void clear() {
		locctr = "" ;
		base = "No" ;
		optab.clear() ;
		symtab.clear() ;
		hash.clear( littab ) ;
		final.clear() ;
	}
};

bool readFile( string fileName ) {
	fstream file ;
	file.open( fileName.c_str(), fstream::in ) ;
	if ( !file.is_open() ) {
		cout << endl << "### The input file is not on the directory! ###" << endl ;
		return false ;
	}
	else {
		string temp ;
		while ( getline( file, temp ) ) {
			if ( temp != "" ) {
				temp = temp + '\n' ;
				input.push_back( temp ) ;
			}
		}
		
		file.close();
		return true ;
	}	
}

int main() {
	string command ;
	AllTable table ;
	table.make() ;
	MakeToken token( table ) ; 
	SIC assembler( table ) ;
	do {
		string fileName ;
		cout << endl << "***Assembler Implement****" ;
		cout << endl << "* 0. Exit                *" ;
		cout << endl << "* 1. SIC Assembler       *" ;
		cout << endl << "* 2. SIC/XE Assembler    *" ;
		cout << endl << "**************************" ;
		cout << endl << "Input a command(0, 1, 2): " ;
		cin >> command ;
		if ( command.compare( "1" ) == 0 || command.compare( "2" ) == 0 ) {
			cout << endl << "Input file name([0]Quit): " ;
			cin >> fileName ;
			token.clear() ;
			input.clear() ;
			assembler.clear() ;
			table.makeHash() ;
			assembler.set( table ) ;
			if ( fileName == "0" ) ;
			else if ( readFile( fileName ) ) {
				token.getToken() ;
				token.print() ;	
				assembler.makeOpcode() ;
				if ( command.compare( "1" ) == 0 )	{
					assembler.pass1() ;
					assembler.pass2() ;
					assembler.print() ;	
					cout << endl << "Output file name([0]Quit): " ;
					cin >> fileName ;
					if ( fileName != "0" )
						assembler.outFile( fileName ) ;	
				}
				else {
					assembler.pass1xe() ;
					assembler.pass2xe() ;
					assembler.print() ;
					cout << endl << "Output file name([0]Quit): " ;
					cin >> fileName ;
					if ( fileName != "0" )
						assembler.outFile( fileName ) ;
				}	
			}			
		}		
		else if ( command.compare( "0" ) == 0 )	
			break ;
		else
			cout << endl << "### Command does not exist! ###" << endl ;			
	} while( true ) ;
	system("pause") ;
} 
