#include "Motion.h"
#include "Operator.h"
#include "WindowManager.h"
#include "Log.h"
using namespace std;

Motion :: Motion(const std::tuple<gtWindowManager*, std::string>& args):
    m_pWM(std::get<0>(args))
{
    auto itr = find(ENTIRE(g_MotionString), std::get<1>(args));
    assert(itr != g_MotionString.end());
    m_ID = (eMotion)distance(g_MotionString.begin(), itr);
    LOGf("motion id: %s", (unsigned)m_ID);
}

bool Motion :: execute()
{
    auto pending = m_pWM->pending();

    for(auto itr = pending.rbegin();
        itr!=pending.rend();
        ++itr)
    {
        auto oper = dynamic_cast<Operator*>(itr->get());
        if(oper)
        {
            oper->execute();
            return true;
        }
    }

    m_pWM->execute_default_operator();

    return true;
}

