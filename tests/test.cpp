#include <ktl/psignals/psignals.hpp>
   
extern "C"
int main(int argc, char *argv[])
{
    return 0;
}


    namespace kps = ktl::psignals;

    kps::sigset newsigset{SIGTERM, SIGINT};
    kps::sigset oldsigset = kps::this_thread::set_mask(newsigset);
    kps::sigset cursigset = kps::this_thread::get_mask();

    std::cout << "Signals in new mask: ";
    for (kps::signum_t signum : newsigset.signals())
        std::cout << signum << " ";
    std::cout << std::endl;

    BOOST_CHECK(oldsigset.empty());
    BOOST_CHECK(newsigset == cursigset);
    BOOST_CHECK(oldsigset != cursigset);
    BOOST_CHECK(newsigset.has(SIGTERM));
    BOOST_CHECK(newsigset.has(SIGINT));
    BOOST_CHECK(newsigset.has(SIGHUP) == false);

    newsigset.clear();
    BOOST_CHECK(newsigset.empty());
    BOOST_CHECK(newsigset.has(SIGTERM) == false);
    BOOST_CHECK(newsigset.has(SIGINT) == false);
    BOOST_CHECK(newsigset.has(SIGHUP) == false);

    kps::this_thread::clear_mask();
    cursigset = kps::this_thread::get_mask();
    BOOST_CHECK(cursigset.empty());
    BOOST_CHECK(cursigset.has(SIGTERM) == false);
    BOOST_CHECK(cursigset.has(SIGINT) == false);
    BOOST_CHECK(cursigset.has(SIGHUP) == false);

    kps::this_thread::fill_mask();
    cursigset = kps::this_thread::get_mask();
    BOOST_CHECK(cursigset.empty() == false);
    BOOST_CHECK(cursigset.has(SIGTERM));
    BOOST_CHECK(cursigset.has(SIGINT));
    BOOST_CHECK(cursigset.has(SIGHUP));
    BOOST_CHECK(cursigset.has(SIGRTMIN));
    BOOST_CHECK(cursigset.has(SIGRTMAX));
    BOOST_CHECK(cursigset.has(kps::rt::signum(0))); // SIGRTMIN
    BOOST_CHECK(cursigset.has(kps::rt::signum(1))); // SIGRTMIN+1
    BOOST_CHECK(cursigset.has(kps::rt::signum(kps::rt::sigcount()))); // SIGRTMAX    
}

BOOST_AUTO_TEST_CASE(wait)
{
    namespace kps = ktl::psignals;

    kps::sigset signals{ SIGTERM, SIGINT, SIGHUP };
    for (kps::sigcnt_t rtsigcnt = 0; rtsigcnt <= kps::rt::sigcount(); ++rtsigcnt)
        signals += kps::rt::signum(rtsigcnt);

    BOOST_TEST_MESSAGE("Please type Ctrl^C");
    const kps::signum_t signum = kps::wait(signals);
}

BOOST_AUTO_TEST_CASE(rt_wait)
{
    namespace kps = ktl::psignals;

    BOOST_TEST_MESSAGE("Please type Ctrl^C");
    const kps::signum_t signum = kps::wait({ SIGTERM, SIGINT, SIGHUP });
}

BOOST_AUTO_TEST_CASE(timed_wait)
{
    namespace kps = ktl::psignals;

    BOOST_TEST_MESSAGE("Please type Ctrl^C within 2.25 seconds");
    const kps::signum_t signum  = kps::wait({ SIGTERM, SIGINT, SIGHUP }, std::chrono::nanoseconds(2250000000));
}

BOOST_AUTO_TEST_SUITE_END() // tests
BOOST_AUTO_TEST_SUITE_END() // psignals
BOOST_AUTO_TEST_SUITE_END() // ktl
