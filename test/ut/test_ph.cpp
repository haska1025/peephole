#include <cppunit/config/SourcePrefix.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/RepeatedTest.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestCase.h>

#include <unistd.h>

#include "../../src/libpeephole/ph_iothread.h"
#include "../../src/libpeephole/tcplistener.h"
#include "../../src/libpeephole/ph_log.h"

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

class TestTCP
{
public:
    static void test_tcp()
    {
        TcpListener tl(uv_default_loop());
        CPPUNIT_ASSERT(0 == tl.Open("127.0.0.1", 0));

        TcpListener tl1(uv_default_loop());
        CPPUNIT_ASSERT(0 == tl1.Open("0.0.0.0", 0));

        TcpListener tl2(uv_default_loop());
        CPPUNIT_ASSERT(0 != tl2.Open(NULL, 0));

        TcpListener tl3(uv_default_loop());
        CPPUNIT_ASSERT(0 == tl3.Open(NULL, 9820));

        uv_run(uv_default_loop(), UV_RUN_NOWAIT);
        LOG_INFO("Before wait for 30s");
        usleep(30*1000*1000);
        LOG_INFO("After wait for 30s");
        tl.Close();
        tl1.Close();
        tl2.Close();
        tl3.Close();
    }
};

class TestPH : public TestFixture
{
    CPPUNIT_TEST_SUITE(TestPH);
        CPPUNIT_TEST(test_iothread);
        CPPUNIT_TEST(test_tcp);
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
    void test_tcp()
    {
        TestTCP::test_tcp();
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestPH);

