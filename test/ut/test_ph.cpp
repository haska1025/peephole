#include <cppunit/config/SourcePrefix.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/RepeatedTest.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestCase.h>

#include "../../src/libpeephole/ph_iothread.h"

using namespace CPPUNIT_NS;

class TestIOThread
{
public:
    static void test_iothread()
    {
        IOThread iothr;

        CPPUNIT_ASSERT(0 == iothr.Start());
        CPPUNIT_ASSERT(0 == iothr.Stop());
        iothr.WaitStop();
    }
};

class TestPH : public TestFixture
{
    CPPUNIT_TEST_SUITE(TestPH);
        CPPUNIT_TEST(test_iothread);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp()
    {
    }
    void setDown()
    {
    }

    void test_iothread()
    {
        TestIOThread::test_iothread();
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestPH);

