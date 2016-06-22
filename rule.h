#pragma   once 
#define MaxRightLength 20
#define MaxRuleNum 20
#define MaxVtNum 20
#define MaxVnNum 20
#define MaxVTNum 20

struct Rule{
	char leftCharacter;
	char rightCharacter[MaxRightLength];
	int rightLength;
};

struct FirstVT{
	char A;
	int len;
	char FVT[MaxVTNum];
};

struct LastVT{
	char A;
	int len;
	char LVT[MaxVTNum];
};
