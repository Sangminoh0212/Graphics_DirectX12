#pragma once
#include "Component.h"

class Transform;
class MeshRenderer;
class Camera;
class MonoBehaviour;

class GameObject : public enable_shared_from_this<GameObject>
{
public:
	GameObject();
	virtual ~GameObject();

	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();

	shared_ptr<Component> GetFixedComponent(COMPONENT_TYPE);

	shared_ptr<Transform> GetTransform();
	shared_ptr<MeshRenderer> GetMeshRenderer();
	shared_ptr<Camera> GetCamera();

	void AddComponent(shared_ptr<Component> component);

private:
	// 멤버 변수 선언
	
	// 일반적인 component는 array로 선언.
	// 고정적으로 사용할 component
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	// Monobehavior을 이용하여 custom으로 만들 component는 따로 관리
	vector<shared_ptr<MonoBehaviour>> _scripts;
};

