#include <boost/bind.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/threadpool.hpp>

using namespace boost;
using namespace boost::threadpool;
using namespace boost::lockfree;

struct TestNode {
    int id;
    char name[64];
    TestNode()
    {
        memset(this, 0, sizeof(TestNode));
    }
};

boost::lockfree::queue<struct TestNode, fixed_sized<false> > que(0);

int ID = 0;
void ThreadSetQueue()
{
    bool ret = false;
    struct TestNode node;
    strcpy(node.name, "hello world");

    while (1) {
        node.id = ID++;
        ret = que.push(node);
        boost::thread::sleep(boost::get_system_time() + boost::posix_time::millisec(1000));
    }
}

void ThreadSetQueue2()
{
    bool ret = false;

    struct TestNode node;
    strcpy(node.name, "hello johnny");

    while (1) {
        node.id = ID++;
        ret = que.push(node);
        boost::thread::sleep(boost::get_system_time() + boost::posix_time::millisec(500));
    }
}

void ThreadGetQueue()
{
    bool ret = true;
    struct TestNode node;

    while (1) {
        if (que.pop(node))
            printf("get ID:%d, name:%s", node.id, node.name);
        else
            boost::thread::sleep(boost::get_system_time() + boost::posix_time::millisec(200));
    }
}

int main(int argc, char* atgv[])
{
    pool QueThread(3);
    QueThread.schedule(&ThreadSetQueue);
    QueThread.schedule(&ThreadSetQueue2);
    QueThread.schedule(&ThreadGetQueue);
    QueThread.wait();
    return 0;
}
