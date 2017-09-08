#include "Stdafx.h"
#pragma once
#include "CefSharpApp.h"

namespace CefSharp
{
    using namespace msclr;

    int GetDomainId()
    {
        return AppDomain::CurrentDomain->Id;
    }

    void CefSharpApp::OnContextInitialized()
    {
        int id = AppDomain::CurrentDomain->Id;

        if (static_cast<Action^>(_onContextInitialized) != nullptr)
        {
            _onContextInitialized->Invoke();
        }
        //msclr::call_in_appdomain(_appdomainId, [](Action^ action)
        //{
        //    if (static_cast<Action^>(action) != nullptr)
        //    {
        //        action->Invoke();
        //    }
        //}, _onContextInitialized);
        call_in_appdomain(_appdomainId, &GetDomainId);
    }
}
