#include "tester.h"
#include <cppunit/ui/text/TestRunner.h>
#include "testsqlagent.h"

void Test::Tester::testSQLAgent()
{
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(Test::TestSQLAgent::suite());
    runner.run();
}
