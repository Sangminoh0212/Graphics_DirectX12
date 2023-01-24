#pragma once
#include "Component.h"
#include "Object.h"

class Transform;
class MeshRenderer;
class Camera;
class Light;
class MonoBehaviour;

class GameObject : public Object, public enable_shared_from_this<GameObject>
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
	shared_ptr<Light> GetLight();

	void AddComponent(shared_ptr<Component> component);

	void SetCheckFrustum(bool checkFrustum) { _checkFrustum = checkFrustum; }
	bool GetCheckFrustum() { return _checkFrustum; }

	void SetLayerIndex(uint8 layer) { _layerIndex = layer; }
	uint8 GetLayerIndex() { return _layerIndex; }


private:
	// 멤버 변수 선언
	
	// 일반적인 component는 array로 선언.
	// 고정적으로 사용할 component
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	// Monobehavior을 이용하여 custom으로 만들 component는 따로 관리
	vector<shared_ptr<MonoBehaviour>> _scripts;

	bool _checkFrustum = true;
	uint8 _layerIndex = 0;
};

