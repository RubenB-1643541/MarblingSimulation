#include "pch.h"
#include "InterfaceComponent.h"

namespace RenderEngine {
    void InterfaceComponent::Init()
    {
        OnInit();
        for (InterfaceComponent* component : _subComponents)
            component->Init();
    }
    void InterfaceComponent::Update()
    {
        OnUpdate();
        for (InterfaceComponent* component : _subComponents)
            component->Update();
    }
    void InterfaceComponent::Draw()
    {
        OnDraw();
        for (InterfaceComponent* component : _subComponents)
            component->Draw();
        EndDraw();
    }
    void InterfaceComponent::AddComponent(InterfaceComponent* component)
    {
        component->Init();
        _subComponents.push_back(component);
    }
    void InterfaceComponent::RemoveComponent(int index)
    {
        ERROR("Remove Component Not Implemented");
    }
}