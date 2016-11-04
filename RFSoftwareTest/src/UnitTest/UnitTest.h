
#ifndef UNIT_TEST_H
#define UNIT_TEST_H

#include <cstdint>
#include <string>


class UnitTest{

private:

	uint32_t assertTotalCount;
	uint32_t assertFailCount;
	uint32_t assertPassCount;

	std::string name;

public:

	UnitTest(std::string name);
	~UnitTest();

	void assertTrue(std::string message,bool condition);
	void assertFalse(std::string message,bool condition);

	void assertEquals(std::string message,uint32_t expected,uint32_t actual);
	void assertEquals(std::string message,int32_t expected,int32_t actual);
	void assertEquals(std::string message,uint8_t expected,uint8_t actual);


};

#endif

