#include "stdafx.h"
#include "Obfuscator.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CAnalisator analisator;

	analisator.OpenFile(argv[1]);
	if (!analisator.LoadLexem("operators.txt", "mathOperators.txt", "separators.txt",
		"var.txt", "signs.txt"))
	{
		return 1;
	}
	analisator.Analise();

	analisator.WriteTokens(argv[2]);

	return 0;
}