#pragma once

enum class COMPONENT_TYPE : uint8
{
	TRANSFORM,
	MESH_RENDERER,
	// ... component �߰� �� ���� �߰�
	MONO_BEHAVIOUR,
	END,
};

enum
{
	// Monobehavior �� component ����
	FIXED_COMPONENT_COUNT = static_cast<uint8>(COMPONENT_TYPE::END) - 1
};

class GameObject;
class Transform;

class Component
{
public:
	Component(COMPONENT_TYPE type);
	// memory leak ����
	virtual ~Component();

public:
	virtual void Awake() { } // start ���� �ʱ�ȭ
	virtual void Start() { }
	virtual void Update() { }
	virtual void LateUpdate() { } // update ���� ������ update

public:
	// type Ȯ��
	COMPONENT_TYPE GetType() { return _type; }
	// gameobject(����) �Ҹ�Ǿ��� üũ
	bool IsValid() { return _gameObject.expired() == false; }

	shared_ptr<GameObject> GetGameObject();
	shared_ptr<Transform> GetTransform();

private:
	// gameobject�� SetGameObject�����Ű�� �ϱ� ����
	// private ���� �� friend class GameObject ����
	friend class GameObject;
	void SetGameObject(shared_ptr<GameObject> gameObject) { _gameObject = gameObject; }

protected:
	COMPONENT_TYPE _type;
	// gameobject(����) <-> component(��ǰ) ���� �˰� �־�� �Ѵ�.
	// shared_ptr�� ��� ��ȯ���� �߻� �ϱ� ������ �����ϱ� ����
	weak_ptr<GameObject> _gameObject;
};

