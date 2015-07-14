#pragma once

using namespace std;

class CObfuscator
{
public:

	enum Type
	{
		Const,
		For,
		Var,
		Function,
		Procedure
	};

	CObfuscator(string const &fileName,
		string const & operatorsPath)
	{
		if (!OpenFile(fileName))
		{
			return;
		}

		LoadLexem(operatorsPath);
		Analise();
	}

	~CAnalisator()
	{
	};
};