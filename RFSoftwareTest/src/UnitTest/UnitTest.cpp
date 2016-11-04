
#include "UnitTest.h"

#include <iostream>




//=================================================================================================
UnitTest::UnitTest(std::string name){

	this->name = name;

	this->assertTotalCount = 0;
	this->assertFailCount = 0;
	this->assertPassCount = 0;

	std::cout << "=============================================================" << std::endl;
	std::cout << "UNIT TEST " << (this->name) << " STARTED" << std::endl;
	std::cout << std::endl;

}
//=================================================================================================
UnitTest::~UnitTest(){

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "UNIT TEST " << (this->name) << " REPORT:" << std::endl;
	std::cout << "total test count: " << assertTotalCount << std::endl;
	std::cout << "test pass count: " << assertPassCount << std::endl;
	if (assertFailCount==0){
		std::cout << "test fail count: " << assertFailCount << std::endl;
	}else{
		std::cout << std::endl;
		std::cout << "!!!!!! TEST FAIL COUNT: " << assertFailCount << " !!!!!!" << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;

}
//=================================================================================================
void UnitTest::assertTrue(std::string message,bool condition){
	
	assertTotalCount++;
	if (condition==true){
		assertPassCount++;
	}else{
		assertFailCount++;
		std::cout << "ASSERT FAIL: " << message << std::endl;
	}
}
//=================================================================================================
void UnitTest::assertFalse(std::string message,bool condition){
	
	assertTotalCount++;
	if (condition==false){
		assertPassCount++;
	}else{
		assertFailCount++;
		std::cout << "ASSERT FAIL: " << message << std::endl;
	}
}
//=================================================================================================
void UnitTest::assertEquals(std::string message,uint32_t expected,uint32_t actual){

	assertTotalCount++;
	if (expected==actual){
		assertPassCount++;
	}else{
		assertFailCount++;
		std::cout << "ASSERT FAIL: " << message << std::endl;
		std::cout << "expected=" << expected << ", actual=" << actual << std::endl;
	}

}
//=================================================================================================
void UnitTest::assertEquals(std::string message,int32_t expected,int32_t actual){

	assertTotalCount++;
	if (expected==actual){
		assertPassCount++;
	}else{
		assertFailCount++;
		std::cout << "ASSERT FAIL: " << message << std::endl;
		std::cout << "expected=" << expected << ", actual=" << actual << std::endl;
	}

}
//=================================================================================================
void UnitTest::assertEquals(std::string message,uint8_t expected,uint8_t actual){

	assertTotalCount++;
	if (expected==actual){
		assertPassCount++;
	}else{
		assertFailCount++;
		std::cout << "ASSERT FAIL: " << message << std::endl;
		std::cout << "expected=" << (uint32_t)expected << ", actual=" << (uint32_t)actual << std::endl;
	}

}
//=================================================================================================



