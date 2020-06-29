#include "Bench.h"
#include "Runner.h"
#include "program.h"
#include <algorithm>

#include "VMirage1.h"

class Simple : public Test, public TestBench<VMirage1>
{
public:
	Simple() : Test("Mirage 1"), TestBench(99, "simple.vcd") {}

	void Initialize() override
    {
		for (int i = 0, j = 0; i < sizeof(data) / 2; ++i, j+= 2)
		{
			myCore->Mirage1__DOT__mem_inst__DOT__mem[i] = (data[j] << 8) | data[j + 1];
		}

		printf("%#08x\n", myCore->Mirage1__DOT__mem_inst__DOT__mem[0x400]);
	}

	bool Execute() override
	{
		myCore->aReset = 1;
		Tick();
		myCore->aReset = 0;
		Tick();

		while(!myCore->Mirage1__DOT__risc16_inst__DOT__haltState && !myCore->Mirage1__DOT__risc16_inst__DOT__errorState)
		{
			Tick();
		}

		TEST_ASSERT_EQ(myCore->anOutPortA, 0x0001, "PortA");

		return true;
	}

	void Tick() override
	{
		myCore->eval();
		DumpTrace();
		myCore->aClock = 1;
		myCore->eval();
		DumpTrace();
		myCore->aClock = 0;
	}

	void Clean() override
	{
		delete this;
	}
};

int main(int argc, char** argv)
{
    Verilated::commandArgs(argc, argv);
	Verilated::traceEverOn(true);
	
	TestSuite suite;
	suite.myName = "Mirage 1";
	suite.myTests.emplace_back(new Simple());

	bool success = suite.Execute();
	suite.Stop();

	return (success ? 0 : -1);
}