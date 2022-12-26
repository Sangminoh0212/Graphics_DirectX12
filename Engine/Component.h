#pragma once

enum class COMPONENT_TYPE : uint8
{
	TRANSFORM,
	MESH_RENDERER,
	// ... component 추가 시 마다 추가
	MONO_BEHAVIOUR,
	END,
};

enum
{
	// Monobehavior 뺀 component 개수
	FIXED_COMPONENT_COUNT = static_cast<uint8>(COMPONENT_TYPE::END) - 1
};

class GameObject;
class Transform;

class Component
{
public:
	Component(COMPONENT_TYPE type);
	// memory leak 예방
	virtual ~Component();

public:
	virtual void Awake() { } // start 이전 초기화
	virtual void Start() { }
	virtual void Update() { }
	virtual void LateUpdate() { } // update 다음 마지막 update

public:
	// type 확인
	COMPONENT_TYPE GetType() { return _type; }
	// gameobject(주인) 소멸되었나 체크
	bool IsValid() { return _gameObject.expired() == false; }

	shared_ptr<GameObject> GetGameObject();
	shared_ptr<Transform> GetTransform();

private:
	// gameobject만 SetGameObject실행시키게 하기 위해
	// private 설정 후 friend class GameObject 설정
	friend class GameObject;
	void SetGameObject(shared_ptr<GameObject> gameObject) { _gameObject = gameObject; }

protected:
	COMPONENT_TYPE _type;
	// gameobject(주인) <-> component(부품) 서로 알고 있어야 한다.
	// shared_ptr의 경우 순환구조 발생 하기 때문에 방지하기 위해
	weak_ptr<GameObject> _gameObject;
};

