#include <iostream>
#include<iostream>
#include<fstream>
#include<ctype.h>
#include<string.h>
#include <cstdlib>
using namespace std;
//Lexical
// Returns "true" if the character is a DELIMITER.
bool isDelimiter(char ch)
{
if (ch == ' ' || ch == '+' || ch == '*' || ch == '>' ||
    ch == '<' || ch == '(' || ch == ')' )
    return (true);
return (false);
}
// Returns 'true' if the character is an OPERATOR.
bool isOperator(char ch)
{
if (ch == '+' || ch == '*' || ch == '>' || ch == '<' )
    return (true);
return (false);
}
// Returns 'true' if the string is a VALID IDENTIFIER.
bool validIdentifier(char* str)
{
if (str[0] == '0' || str[0] == '1' || str[0] == '2' ||
    str[0] == '3' || str[0] == '4' || str[0] == '5' ||
    str[0] == '6' || str[0] == '7' || str[0] == '8' ||
    str[0] == '9' || isDelimiter(str[0]) == true)
    return (false);
return (true);
}
// Returns 'true' if the string is a KEYWORD.
bool isKeyword(char* str)
{
if (!strcmp(str, "cin") || !strcmp(str, "cout") || !strcmp(str, "int")|| !strcmp(str, "float"))
    return (true);
return (false);
}
// Returns 'true' if the string is an INTEGER.
bool isInteger(char* str)
{
int i, len = strlen(str);
if (len == 0)
    return (false);
for (i = 0; i < len; i++) {
    if (str[i] != '0' && str[i] != '1' && str[i] != '2'
        && str[i] != '3' && str[i] != '4' && str[i] != '5'
        && str[i] != '6' && str[i] != '7' && str[i] != '8'
        && str[i] != '9' || (str[i] == '-' && i > 0))
        return (false);
}
return (true);
}
// Returns 'true' if the string is a REAL NUMBER.
bool isRealNumber(char* str)
{
int i, len = strlen(str);
bool hasDecimal = false;
if (len == 0)
    return (false);
for (i = 0; i < len; i++) {
    if (str[i] != '0' && str[i] != '1' && str[i] != '2'
        && str[i] != '3' && str[i] != '4' && str[i] != '5'
        && str[i] != '6' && str[i] != '7' && str[i] != '8'
        && str[i] != '9' && str[i] != '.' ||(str[i] == '-' && i > 0))
        return (false);
    if (str[i] == '.')
        hasDecimal = true;
}
return (hasDecimal);
}
// Extracts the SUBSTRING.
char* subString(char* str, int left, int right)
{
int i;
char* subStr = (char*)malloc(
sizeof(char) * (right - left + 2));
for (i = left; i <= right; i++)
    subStr[i - left] = str[i];
subStr[right - left + 1] = '\0';
return (subStr);
}
// Parsing the input STRING.
void parse(char* str)
{
int left = 0, right = 0;
int len = strlen(str);
while (right <= len && left <= right) {

    if (isDelimiter(str[right]) == false)
        right++;
    if (isDelimiter(str[right]) == true && left == right) {
        if (isOperator(str[right]) == true)
            cout << "IS AN OPERATOR\n" << (str[right]);
        right++;
        left = right;
}
    else if (isDelimiter(str[right]) == true && left != right|| (right == len && left != right)) {
        char* subStr = subString(str, left, right - 1);
        if (isKeyword(subStr) == true)
            cout << (subStr) << "IS A KEYWORD\n" ;
        else if (isInteger(subStr) == true)
            cout << (subStr) << "IS AN INTEGER\n";
        else if (isRealNumber(subStr) == true)
            cout << (subStr) << "IS A REAL NUMBER\n";
        else if (validIdentifier(subStr) == true && isDelimiter(str[right - 1]) == false)
            cout << (subStr) << "IS A VALID IDENTIFIER\n" ;
        else if (validIdentifier(subStr) == false && isDelimiter(str[right - 1]) == false)
            cout << (subStr) << "IS NOT A VALID IDENTIFIER\n";
        left = right;
        }
    }
return;
}
/***************************PARSING**************************************/
enum token{
	cin_sy , cout_sy, cin_op_sy, cout_op_sy, lparn_sy, rparn_sy,
    plus_sy, mult_sy, id, int_sy, real_sy, error_sy, end_source_sy };
class parser{
	private:
		ifstream f;
		token curr_token;
		token check_reserved (string s){
			if (s=="cin") return cin_sy;
			else if (s=="cout") return cout_sy;
			else return id;
		}
		token get_token(){
			char ch;
			string s;
			int state = 0;
			while(state>=0 && state <=11 && !f.eof()){
				switch(state){
					case 0:
						f.get(ch);
						if (f.eof()) return end_source_sy;
						else if (isspace(ch)) {s=ch; state = 0;}
						else if (isdigit(ch)) {s=ch; state = 1;}
						else if (ch=='.') {s=ch; state = 2;}
						else if (isalpha(ch)) {s=ch; state = 4;}
						else if (ch=='<') state = 5;
						else if (ch=='>') state = 6;
						else if (ch=='+') state = 7;
						else if (ch=='*') state = 8;
						else if (ch=='(') state = 9;
						else if (ch==')') state = 10;
						else state = 11;
						break;
					case 1:
						f.get(ch);
						if (isdigit(ch)) {s+=ch; state=1;}
						else if (ch=='.') state=3;
						else {f.putback(ch); return int_sy;}
						break;
					case 2:
						f.get(ch);
						if (isdigit(ch)) {s+=ch; state=2;}
						else
						{
							f.putback(ch);
							return real_sy;
						}
						break;
					case 3:
						f.get(ch);
						if (isdigit(ch)) {s+=ch; state=3;}
						else
						{
							f.putback(ch);
							return real_sy;
						}
						break;
					case 4:
						f.get(ch);
						if(isalnum(ch)) {s+=ch; state=4;}
						else
						{
							f.putback(ch);
							return check_reserved(s);
						}
						break;
					case 5:
						f.get(ch);
						if(ch=='<') return cout_op_sy;
						else return error_sy;
						break;
					case 6:
						f.get(ch);
						if(ch=='>') return cin_op_sy;
						else return error_sy;
						break;
					case 7:
						return plus_sy; break;
					case 8:
						return mult_sy; break;
					case 9:
						return lparn_sy; break;
					case 10:
						return rparn_sy; break;
					case 11:
						return error_sy; break;
				}
			}
			return end_source_sy;
		}
	string name(token t){
		switch(t){
			case cin_sy: return "cin token";         break;
			case cout_sy: return "cout token";       break;
			case cin_op_sy: return ">> token";       break;
			case cout_op_sy: return "<< token";      break;
			case id: return "identifier";            break;
			case int_sy: return "int token";         break;
			case real_sy: return "real token";       break;
			case plus_sy: return "+ token";          break;
			case mult_sy: return "* token";          break;
			case lparn_sy: return "( token";         break;
			case rparn_sy: return ") token";         break;
			case error_sy: return "error token";     break;
			case end_source_sy: return "End of file";break;
		}
		return "error token";
	}
	public:
		parser(string filename){
			f.open(filename.c_str());
		}
		~parser(){
			f.close();
		}
		void match(token terminal){
			if (curr_token == terminal)
				cout<<name(terminal)<<" is matched\n";
			else
				syntax_error(curr_token);
			curr_token = get_token();
		}
		void syntax_error(token terminal){
			cout<<name(terminal)<<" is not Expected\n";
		}
		void sample_parser(){
			curr_token = get_token();
			program();
			match(end_source_sy);
		}
		void program(){
			if(curr_token == cin_sy){
				match(cin_sy);
				match(cin_op_sy);
				expr_seq();
			}
			else if(curr_token == cout_sy){
				match(cout_sy);
				match(cout_op_sy);
				expr_seq();
			}
			else
				syntax_error(curr_token);
		}
		void expr_seq(){
			expr();
			while(curr_token == id || curr_token == int_sy || curr_token == real_sy || curr_token == lparn_sy || curr_token == rparn_sy ){
				expr();
			}
		}
		void expr(){
			switch(curr_token){
				case id: match(id);             break;
				case int_sy : match(int_sy);    break;
				case real_sy : match(real_sy);  break;

				case lparn_sy:
					match (lparn_sy);
					if (curr_token == plus_sy){
						match (plus_sy);
						expr_seq();
						match(rparn_sy);
					}
					else if (curr_token == mult_sy){
						match (mult_sy);
						expr_seq();
						match(rparn_sy);
					}
					else{
						syntax_error(curr_token);
					}
					break;
			}
		}
};
int main()
{
    string name;
    cout<<"Enter File Name:"; cin>>name;
    parser p(name);
    p.sample_parser();
    p.~parser();
}
