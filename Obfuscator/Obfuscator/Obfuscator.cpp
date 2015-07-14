// Obfuscator.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
using namespace std;
bool flagComment = false, flagStr = false;
bool findFunc = false; bool constFlag = false; 
bool varFlag = false;
int zCode = 48,cycleBorder = 5, curr;
map <string, string> procFuncMap;
map<string, string> constMap;
map<string, string> varMap;
vector <string> code;
set<string> keyWords;
set<char> setOfSpecialChars = { '+', '-', '/', '*', '=', '<', '>', '[', ']', '.', ',', '(', ')', ':', ';', '^', '@', '$', '#' };
vector<string> initKeyWords =
{ "and", "exports", "mod", "shr", "array", "file", "nil", "string",
"asm", "for", "not", "then", "begin", "function", "object", "to",
"case", "goto", "of", "type", "const", "if", "or", "unit", "constructor",
"implementation", "packed", "until", "destructor", "in", "procedure", "uses",
"div", "inherited", "program", "var", "do", "inline", "record", "while",
"downto", "interface", "repeat", "with", "else", "label", "set", "xor",
"end", "library", "shl", "real", "word", "double", "integer", "byte",
"shortint", "smallint", "longword", "int64", "qword", "extended", "single",
"real48", "boolean", "char", "readln", "writeln", "read", "write", "text", "crt",
"abs", "sin", "cos", "arctan", "sqrt", "sqr", "power", "exp", "ln", "frac",
"lnt", "random", "succ", "pred", "inc", "dec", "str", "val", "trunc", "round",
"odd", "chr", "ord", "length", "concat", "copy", "delete", "new", "dispose",
"insert", "pos", "assign", "rewrite", "close", "reset", "seek", "true", "false",
"exit", "fileexists", "eof", "halt", "clrscr" };

string ChangeName(const string &str)
{
	string res;
	int len = str.length();
	if (str.length() == 1)
	{
		len += 2;
	}
	for (size_t i = 0; i < len; ++i)
	{
		int lod = rand() % 2;
		if (i == 0)
		{
			lod = 1;
		}
		if (lod) //letter or digit
		{
			char letter = char(rand() % 26 + 97);
			int uol = rand() % 2;
			if (uol) //upper case or lower case
			{
				res += tolower(letter);
			}
			else
			{
				res += toupper(letter);
			}
		}
		else
		{
			res += char(rand() % 10 + 48);
		}
	}
	string checkStr;
	for (size_t i = 0; i < res.length(); ++i)
	{
		checkStr += tolower(res[i]);
	}
	if (find(initKeyWords.begin(), initKeyWords.end(), checkStr) != initKeyWords.end())
	{
		res = ChangeName(res);
	}
	return res;
}

void RenameFuncProc(string &str)
{
	string newStr, var, newVar;
	newStr = str.substr(0, curr);
	string name, newName, ost;
	bool goNext = true, locVar = false;
	for (size_t i = curr; i < str.length(); ++i)
	{
		
		if (str[i] != '(' && str[i] != ';' && str[i] != ':' && goNext)
		{
			name += tolower(str[i]);
		}
		else
		{
			if (str[i] == '(')
			{
				ost += str[i];
				locVar = true;
				goNext = false;
			}
			else
			{
				if (locVar)
				{
					if (str[i] != ',' && str[i] != ':')
					{
						var += str[i];
					}
					else if (str[i] == ',')
					{
						ost += var;
						ost += ',';
						newVar = ChangeName(var);
						varMap[var] = newVar;
						var = "";
					}
					else if (str[i] == ':')
					{
						ost += var;
						ost += ':';
						newVar = ChangeName(var);
						varMap[var] = newVar;
						var = "";
						locVar = false;
					}
				}
				else
				{
					ost += str[i];
				}
				goNext = false;
			}
		}
	}
	newName = ChangeName(name);
	procFuncMap[name] = newName;
}

string DeleteCommentsAndSpaces(string &str)
{
	string res;
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (str[i] == '\t')
		{
			str[i] = ' ';
		}
		if ((str[i] == '\'') && (!flagStr))
		{
			flagStr = true;
			res += str[i];
		}
		else if (str[i] == '\'')
		{
			flagStr = false;
			res += str[i];
		}
		else if ((str[i] == '/') && (str[i + 1] == '/') && !flagStr)
		{
			break;
		}
		else if (str[i] == '{' && !flagStr)
		{
			flagComment = true;
		}
		else if (str[i] == ' ' && !flagComment && !flagStr)
		{
			if (i != 0 && (isalpha(str[i - 1]) || isdigit(str[i - 1])))
			{
				while (str[i] == ' ')
				{
					i++;
				}
				if (isalpha(str[i]) || isdigit(str[i]))
				{
					res += ' '; 
					res += tolower(str[i]);
				}
				else
				{
					res += tolower(str[i]);
				}
			}
		}
		else if (flagComment)
		{
			while (str[i] != '\0')
			{
				if (str[i] == '}')
				{
					flagComment = false;
					i++;
					break;
				}
				i++;
			}
		}
		else
		{
			res += tolower(str[i]);
			if (res == "function" || res == "procedure")
			{
				findFunc = true;
				curr = i + 2;
			}
			if (res == "const")
			{
				constFlag = true;
			}
			if (res == "var")
			{
				varFlag = true;
			}
		}
	}
	if (res.length() > 0 && res[res.length() - 1] != ';')
	{
		str = res + ' ';
	}
	else
	{
		str = res;
	}
	return str;
}

void ReadFile(const string &fileName)
{
	ifstream fin(fileName);
	if (!fin.is_open())
	{
		cout << "Error: can't load input file\n";
		exit(1);
	}
	string count;
	while (!fin.eof())
	{
		getline(fin, count);
		code.push_back(count);
	}
	fin.close();
}

string CreateString(const string &var, const string &left)
{
	string str;
	str += var;
	str += " := ";
	str += left;
	str += ";";
	return str;
}

void BreakCycle(vector<string> &code, const string &cycleType, const string &var, const string &left, const string &right, bool isInteger, size_t &count)
{
	{
		size_t lineCount = count;
		++lineCount;
		string str = code[lineCount];

		size_t start, end;
		int first, cyclesCount;
		if (str == "begin ")
		{
			start = lineCount;
			int cycles = 1;
			while (cycles)
			{
				++lineCount;
				string str = code[lineCount];
				if (str == "begin ")
				{
					++cycles;
				}
				str.erase(str.end() - 1);
				if (str == "end")
				{
					--cycles;
				}
			}
			end = lineCount;

			vector<string> cyclesArray;
			string temp;

			temp = CreateString(var, left);

			cyclesArray.push_back(temp);
			for (size_t i = start; i < end + 1; ++i)
			{
				cyclesArray.push_back(code[i]);
			}

			cyclesCount = atoi(right.c_str());
			first = atoi(left.c_str());
			string temp1;
			if (cycleType == "to")
			{
				temp1 += ("inc(");
				temp1 += var;
				temp1 += (");");
				cyclesCount = abs(cyclesCount - first + 1);
			}
			if (cycleType == "downto")
			{
				temp1 += ("dec(");
				temp1 += var;
				temp1 += (");");
				cyclesCount = abs(first - cyclesCount + 1);
			}
			cyclesArray.push_back(temp1);

			for (size_t i = start - 1; i < end + 1; ++i)
			{
				code[i] = "";
			}

			vector<string>::iterator position;
			for (size_t i = 0; i < cyclesCount; ++i)
			{
				position = code.begin() + start - 1;
				if (i == 0)
				{
					code.insert(position, cyclesArray.begin(), cyclesArray.end());
				}
				else
				{
					code.insert(position + cyclesArray.size(), cyclesArray.begin() + 1, cyclesArray.end());
				}
			}
		}
		else
		{
			string result;
			bool isEndOfString = false;
			start = lineCount;
			while (!isEndOfString)
			{
				string temp = code[lineCount];
				for (size_t i = 0; i < temp.size(); ++i)
				{
					if (temp[i] != ';')
					{
						result.push_back(temp[i]);
					}
					else
					{
						result.push_back(';');
						isEndOfString = true;
						break;
					}
				}
				if (isEndOfString)
				{
					break;
				}
				++lineCount;
			}
			end = lineCount;

			vector<string> cyclesArray;
			string temp;

			temp = CreateString(var, left);

			cyclesArray.push_back(temp);
			cyclesArray.push_back(result);

			cyclesCount = atoi(right.c_str());
			first = atoi(left.c_str());
			string temp1;
			if (cycleType == "to")
			{
				temp1 += ("inc(");
				temp1 += var;
				temp1 += (");");
				cyclesCount = abs(cyclesCount - first + 1);
			}
			if (cycleType == "downto")
			{
				temp1 += ("dec(");
				temp1 += var;
				temp1 += (");");
				cyclesCount = abs(first - cyclesCount + 1);
			}
			cyclesArray.push_back(temp1);

			for (size_t i = start - 1; i < end + 1; ++i)
			{
				code[i] = "";
			}

			vector<string>::iterator position;
			for (size_t i = 0; i < cyclesCount; ++i)
			{
				position = code.begin() + start - 1;
				if (i == 0)
				{
					code.insert(position, cyclesArray.begin(), cyclesArray.end());
				}
				else
				{
					code.insert(position + cyclesArray.size(), cyclesArray.begin() + 1, cyclesArray.end());
				}
			}
		}
	}
}

void ChangeCycleForToWhile(vector<string> &code, const string &cycleType, const string &var, const string &left, const string &right, bool isInteger, size_t &count)
{
	{
		size_t lineCount = count;
		++lineCount;
		string str = code[lineCount];

		size_t start, end;
		if (str == "begin ")
		{
			start = lineCount;
			int cycles = 1;
			while (cycles)
			{
				++lineCount;
				string str = code[lineCount];
				if (str == "begin ")
				{
					++cycles;
				}
				str.erase(str.end() - 1);
				if (str == "end")
				{
					--cycles;
				}
			}
			end = lineCount;

			vector<string> cyclesArray;
			string temp;

			temp = CreateString(var, left);

			string temp1;
			if (cycleType == "to")
			{
				temp1 += ("While ");
				temp1 += var;
				temp1 += (" < ");
				temp1 += right;
				temp1 += (" + 1");
				temp1 += (" Do ");
			}
			if (cycleType == "downto")
			{
				temp1 += ("While ");
				temp1 += var;
				temp1 += (" > ");
				temp1 += right;
				temp1 += (" - 1");
				temp1 += (" Do ");
			}
			cyclesArray.push_back(temp);
			cyclesArray.push_back(temp1);
			for (size_t i = start; i < end; ++i)
			{
				cyclesArray.push_back(code[i]);
			}
			temp1.clear();
			if (cycleType == "to")
			{
				temp1 += ("inc(");
				temp1 += var;
				temp1 += (");");
			}
			if (cycleType == "downto")
			{
				temp1 += ("dec(");
				temp1 += var;
				temp1 += (");");
			}
			cyclesArray.push_back(temp1);
			cyclesArray.push_back(code[end]);
			for (size_t i = start - 1; i < end + 1; ++i)
			{
				code[i] = "";
			}

			vector<string>::iterator position;
			position = code.begin() + start - 1;
			code.insert(position, cyclesArray.begin(), cyclesArray.end());
		}
		else
		{
			string result;
			bool isEndOfString = false;
			start = lineCount;
			while (!isEndOfString)
			{
				string temp = code[lineCount];
				for (size_t i = 0; i < temp.size(); ++i)
				{
					if (temp[i] != ';')
					{
						result.push_back(temp[i]);
					}
					else
					{
						result.push_back(';');
						isEndOfString = true;
						break;
					}
				}
				if (isEndOfString)
				{
					break;
				}
				++lineCount;
			}
			end = lineCount;

			vector<string> cyclesArray;
			string temp;

			temp = CreateString(var, left);

			string temp1;
			if (cycleType == "to")
			{
				temp1 += ("While ");
				temp1 += var;
				temp1 += (" < ");
				temp1 += right;
				temp1 += (" + 1");
				temp1 += (" Do ");
			}
			if (cycleType == "downto")
			{
				temp1 += ("While ");
				temp1 += var;
				temp1 += (" > ");
				temp1 += right;
				temp1 += (" - 1");
				temp1 += (" Do ");
			}

			cyclesArray.push_back(temp);
			cyclesArray.push_back(temp1);
			cyclesArray.push_back("Begin ");
			cyclesArray.push_back(result);

			temp1.clear();
			if (cycleType == "to")
			{
				temp1 += ("inc(");
				temp1 += var;
				temp1 += (");");
			}
			if (cycleType == "downto")
			{
				temp1 += ("dec(");
				temp1 += var;
				temp1 += (");");
			}
			cyclesArray.push_back(temp1);
			cyclesArray.push_back("End;");
			for (size_t i = start - 1; i < end + 1; ++i)
			{
				code[i] = "";
			}
			vector<string>::iterator position;
			position = code.begin() + start - 1;
			code.insert(position, cyclesArray.begin(), cyclesArray.end());
		}
	}
}

bool isVarInt(const string &str)
{
	if (isdigit(str[0]))
	{
		return true;
	}

	return false;
}

void FindBorders(const string &str, int &first_1, string &first)
{
	if (isVarInt(str))
	{
		first_1 = atoi(str.c_str());
	}
	else
	{
		first = str;
	}
}

void AnalyzeCycle(vector<string> &code, size_t &count)
{
	string str = code[count];
	int first_1 = 0, second_1 = 0, difference = 0;
	string first, second, cycleType, var;

	vector<string> partsOfCycle;
	string temp;
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (str[i] != ' ' && str[i] != ':')
		{
			temp += str[i];
		}
		else if (str[i] == ' ')
		{
			partsOfCycle.push_back(temp);
			temp = "";
		}
		else if (str[i] == ':')
		{
			partsOfCycle.push_back(temp);
			temp = ":=";
			partsOfCycle.push_back(temp);
			temp = "";
			i++;
		}
	}
	var = partsOfCycle[1];
	cycleType = partsOfCycle[4];

	FindBorders(partsOfCycle[3], first_1, first);
	FindBorders(partsOfCycle[5], second_1, second);

	if (first_1 && second_1)
	{
		difference = abs(second_1 - first_1);
		if (difference <= 4 && difference > 0)
		{
			BreakCycle(code, cycleType, var, partsOfCycle[3], partsOfCycle[5], true, count);
		}
		else
		{
			ChangeCycleForToWhile(code, cycleType, var, partsOfCycle[3], partsOfCycle[5], true, count);
		}
	}
	else
	{
		ChangeCycleForToWhile(code, cycleType, var, partsOfCycle[3], partsOfCycle[5], false, count);
	}

}

void FindCycles(vector<string> &code)
{
	for (size_t i = 0; i < code.size(); ++i)
	{
		string str = code[i];
		if (str.size() > 2)
		{
			string temp(str);
			temp.erase(temp.begin() + 3, temp.end());
			if (temp == "for")
			{
				AnalyzeCycle(code, i);
			}
		}
	}
}

void RenameVars(size_t &j)
{
	string res, var, newVar, str;
	j++;
	str = DeleteCommentsAndSpaces(code[j]);
	while (str != "begin ")
	{
		for (size_t i = 0; i < str.size(); ++i)
		{
			string temp = str.substr(0, 9);
			if (temp == "procedure")
			{
				--j;
				return;
			}
			temp.erase(temp.end() - 1);
			if (temp == "function")
			{
				--j;
				return;
			}
			if (str[i] == ':')
			{
				break;
			}
			else
			{
				while ((str[i] != ',') && (str[i] != ':'))
				{
					var += str[i];
					++i;
				}
				if ((str[i] != ','))
				{
					
					--i;
				}
				newVar = ChangeName(var);
				varMap[var] = newVar;
				var = ""; newVar = "";
			}
		}
		j++;
		str = DeleteCommentsAndSpaces(code[j]);
	}
}

bool FoundString(char symb, bool &wasString, size_t &i)
{
	if (symb == '\'' && wasString)
	{
		wasString = false;
		i++;
		return false;
	}
	if (symb == '\'' && !wasString)
	{
		wasString = true;
		return true;
	}
	return false;
}

string ChangeConstToValue(string &str, size_t &j)
{
	size_t start = j, end;
	string var, val;
	str = DeleteCommentsAndSpaces(code[j + 1]);
	while ((str != "var ") && (str != "type ") && (str.size()))
	{
		bool varEnd = false;
		for (size_t i = 0; i < str.size(); ++i)
		{
			if (str[i] != '=' && !varEnd)
			{
				var += str[i];
			}
			else if (str[i] != ';')
			{
				if (str[i] == '=')
				{
					i++;
				}
				varEnd = true;
				val += str[i];
			}
		}
		constMap[var] = val;
		var = ""; val = "";
		j++;
		str = DeleteCommentsAndSpaces(code[j]);
	}
	end = j;
	for (size_t i = start; i < end; ++i)
	{
		code[i] = "";
	}
	return str;
}

void CheckForChangeVariable(string &var)
{
	bool wasString = false;
	if (var.length() == 0)
	{
		return;
	}
	for(auto it = varMap.begin(); it != varMap.end(); ++it)
	{
		for (size_t i = 0; i < var.length(); ++i)
		{
			if (!FoundString(var[i], wasString, i))
			{
				string symb = "";
				symb += var[i];
				if (it->first == symb)
				{
					if ((i == 0 && !isalpha(var[i + it->first.length()])
						&& !isdigit(var[i + it->first.length()])) ||
						(i > 0 && !isalpha(var[i - 1])
						&& !isalpha(var[i + it->first.length()])
						&& !isdigit(var[i + it->first.length()])))
					{
						if (i != string::npos)
						{
							var.replace(i, it->first.length(), it->second);
						}
					}
				}
			}
		}
	}
}

void CheckForNewProcsAndFuncs(string &funcOrProc)
{
	for (auto it = procFuncMap.begin(); it != procFuncMap.end(); ++it)
	{
		int n = funcOrProc.find(it->first);
		if (n != string::npos)
		{
			funcOrProc.replace(n, it->first.length(), it->second);
		}
	}
}

void CheckForChangeConst(string &cons)
{
	for (auto it = constMap.begin(); it != constMap.end(); ++it)
	{
		int n = cons.find(it->first);
		if (n != string::npos)
		{
			cons.replace(n, it->first.length(), it->second);
		}
	}
}

void ChangeRegister(string &str)
{
	string newStr;
	bool wasString = false;
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (str[i] == '\'' && wasString)
		{
			wasString = false;
			newStr += str[i];
			i++;
		}
		if (str[i] == '\'' && !wasString)
		{
			wasString = true;
		}
		int regNum = rand() % 2;
		if (!wasString)
		{
			if (regNum)
			{
				newStr += toupper(str[i]);
			}
			else
			{
				newStr += tolower(str[i]);
			}
		}
		else
		{
			newStr += str[i];
		}
	}
	str = newStr;
}

void MakeChanges()
{
	for (size_t i = 0; i < code.size(); ++i)
	{
		FindCycles(code);
		/*CheckForNewProcsAndFuncs(code[i]);
		CheckForChangeVariable(code[i]);
		CheckForChangeConst(code[i]);
		ChangeRegister(code[i]);*/
	}
}

void CollectInfo()
{
	for (size_t i = 0; i < code.size(); ++i)
	{
		DeleteCommentsAndSpaces(code[i]);
		if (code[i].length() > 0)
		{
			if (findFunc)
			{
				RenameFuncProc(code[i]);
				findFunc = false;
			}
			if (constFlag)
			{
				ChangeConstToValue(code[i], i);
				constFlag = false;
			}
			if (varFlag)
			{
				RenameVars(i);
				varFlag = false;
			}
		}
	}
}

int main(int argc, char* argv[])
{
	for (size_t i = 0; i < initKeyWords.size(); ++i)
	{
		keyWords.insert(initKeyWords[i]);
	}
	string name_inp = argv[1];
	string name_out = argv[2];
	ifstream input(name_inp);
	if (argc != 3) {
		printf("Usage: lab1 [input file] [output file] \n");
		return 0;
	}
	ReadFile(name_inp);
	CollectInfo();
	MakeChanges();
	ofstream output(name_out);
	if (!output.is_open())
	{
		cout << "Error: cannot create output file\n";
		return 0;
	}
	input.close();
	for (size_t i = 0; i < code.size(); i++)
	{
		output << code[i] + '\n';
	}
	return 0;
}

