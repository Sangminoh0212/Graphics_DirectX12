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
	// ��� ���� ����
	
	// �Ϲ����� component�� array�� ����.
	// ���������� ����� component
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	// Monobehavior�� �̿��Ͽ� custom���� ���� component�� ���� ����
	vector<shared_ptr<MonoBehaviour>> _scripts;
};

