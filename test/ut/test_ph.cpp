#include <cppunit/config/SourcePrefix.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/RepeatedTest.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestCase.h>

#include <unistd.h>
#include <string.h>
#include "../../src/libpeephole/ph_iothread.h"
#include "../../src/libpeephole/tcplistener.h"
#include "../../src/libpeephole/ph_log.h"
#include "../../src/libpeephole/ioadapter.h"
#include "../../src/libpeephole/tcphandler.h"

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
    class TcpHandlerAdapter: public IOAdapter
    {
    public:
        TcpHandlerAdapter(int m):mode_(m)
        {
        }

        virtual void OnConnect()
        {
            LOG_INFO("Connect success. this(%p)", this);
            send();
        }
        virtual int OnRecv(IOBuffer *buf)
        {
            send();
            return 0;
        }
        virtual int OnSend()
        {
            send();
            return 0;
        }
        virtual void OnClose()
        {
            LOG_INFO("Connect meet error of eof");
        }
    private:
        void send()
        {
            int rc = 0;
            const char *clientmsg = "Hello server, i'm client.";
            const char *servermsg = "Hello client, i'm server.";
            const char *msg = mode_==1?clientmsg:servermsg;  
            rc = handler_->Send(msg, strlen(msg));
            LOG_INFO("Send mode(%d) msg(%s)", mode_, msg);
            if (rc != 0 && rc != -104)
                handler_->Close();
        }

        int mode_;// 1: client, 2:server
    };
    class ListenAdapter : public IOAdapter
    {
    public:

        virtual IOAdapter * OnAccept()
        {
            return new TcpHandlerAdapter(2);
        }
    };
    static void test_tcp()
    {
        int rc;
        ListenAdapter la;
        TcpListener tl(uv_default_loop(), &la);
        CPPUNIT_ASSERT(0 == tl.Open("127.0.0.1", 0));

        TcpListener tl1(uv_default_loop(), &la);
        CPPUNIT_ASSERT(0 == tl1.Open("0.0.0.0", 0));

        TcpListener tl2(uv_default_loop(), &la );
        CPPUNIT_ASSERT(0 != tl2.Open(NULL, 0));

        TcpListener tl3(uv_default_loop(), &la);
        CPPUNIT_ASSERT(0 == tl3.Open(NULL, 9820));

        TcpHandlerAdapter tha(1);
        TcpHandler cli_th(uv_default_loop(), &tha);

        cli_th.Connect("127.0.0.1", 9820);
        CPPUNIT_ASSERT(rc == 0);

        uv_run(uv_default_loop(), UV_RUN_DEFAULT);
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

