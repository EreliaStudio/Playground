#include <sparkle.hpp>
#include <set>

#define DEBUG_LINE() std::cout << __FILE__ << ":" << __LINE__ << " -> " << __FUNCTION__ << std::endl

template<typename T>
concept Hashable =
	std::equality_comparable<T> &&
	requires(const T& value)
	{
		{ std::hash<T>{}(value) } -> std::convertible_to<std::size_t>;
	};

template<typename TContext>
	requires Hashable<TContext>
class ContextualizableTrait
{
public:
	using OnContextEditionContractProvider = spk::ContractProvider<const TContext&, const TContext&>;
	using OnContextEditionContract = typename OnContextEditionContractProvider::Contract;
	using OnContextEditionCallback = typename OnContextEditionContractProvider::callback_type;

private:
	TContext _context;
	OnContextEditionContractProvider _onContextEditionContractProvider;

public:
	explicit ContextualizableTrait(const TContext& context = {}) :
		_context(context)
	{
	}

	[[nodiscard]] OnContextEditionContract subscribeToContextEdition(OnContextEditionCallback callback)
	{
		return _onContextEditionContractProvider.subscribe(std::move(callback));
	}

	void changeContext(const TContext& newContext)
	{
		if (_context == newContext)
		{
			return;
		}

		_onContextEditionContractProvider.trigger(_context, newContext);

		_context = newContext;
	}

	[[nodiscard]] const TContext& context() const
	{
		return _context;
	}
};

template<typename TContext, typename TType>
	requires Hashable<TContext>
class Registry
{
public:
	using ElementSet = std::set<TType*>;

	class Object
	{
	private:
		using ContextualType = ContextualizableTrait<TContext>;
		using ContextEditionContract = typename ContextualType::OnContextEditionContract;

		ContextEditionContract _contextEditionContract;

	protected:
		Object()
		{
			static_assert(std::derived_from<TType, ContextualType>);

			TType* object = static_cast<TType*>(this);
			ContextualType* contextual = static_cast<ContextualType*>(object);

			Registry::add(contextual->context(), object);

			_contextEditionContract = contextual->subscribeToContextEdition(
				[object](const TContext& oldContext, const TContext& newContext)
				{
					Registry::remove(oldContext, object);
					Registry::add(newContext, object);
				});
		}

		~Object()
		{
			TType* object = static_cast<TType*>(this);
			ContextualType* contextual = static_cast<ContextualType*>(object);

			Registry::remove(contextual->context(), object);
		}

		Object(const Object&) = delete;
		Object& operator=(const Object&) = delete;

		Object(Object&&) = delete;
		Object& operator=(Object&&) = delete;
	};

	class Query
	{
	public:
		class Selection
		{
		private:
			TContext _context;
			ElementSet _elements;

			Selection(const TContext& context, ElementSet elements) :
				_context(context),
				_elements(std::move(elements))
			{
			}

			friend class Query;

		public:
			template<typename TPredicate>
				requires std::predicate<TPredicate, const TType*>
			Selection& where(TPredicate predicate)
			{
				std::erase_if(_elements, [&](TType* element)
					{
						return !std::invoke(predicate, static_cast<const TType*>(element));
					});

				return *this;
			}

			Selection& unionWith(const Selection& other)
			{
				_elements.insert(other._elements.begin(), other._elements.end());
				return *this;
			}

			template<typename TRegisteredType>
				requires std::derived_from<TRegisteredType, TType>
			Selection& unionWith()
			{
				return unionWith(
					Query(_context).template select<TRegisteredType>()
				);
			}

			Selection& intersectWith(const Selection& other)
			{
				std::erase_if(_elements, [&](TType* element)
					{
						return !other._elements.contains(element);
					});

				return *this;
			}

			template<typename TRegisteredType>
				requires std::derived_from<TRegisteredType, TType>
			Selection& intersectWith()
			{
				return intersectWith(
					Query(_context).template select<TRegisteredType>()
				);
			}

			Selection& exclude(const Selection& other)
			{
				std::erase_if(_elements, [&](TType* element)
					{
						return other._elements.contains(element);
					});

				return *this;
			}

			[[nodiscard]] const ElementSet& toSet() const
			{
				return _elements;
			}

			operator ElementSet() const
			{
				return _elements;
			}
		};

	private:
		TContext _context;

	public:
		explicit Query(const TContext& context) :
			_context(context)
		{
		}

		template<typename TRegisteredType>
			requires std::derived_from<TRegisteredType, TType>
		[[nodiscard]] Selection select() const
		{
			ElementSet result;

			for (TRegisteredType* element : Registry<TContext, TRegisteredType>::elements(_context))
			{
				result.insert(static_cast<TType*>(element));
			}

			return Selection(_context, std::move(result));
		}
	};

private:
	static inline std::unordered_map<TContext, ElementSet> _elementsByContext;

	static bool add(const TContext& context, TType* element)
	{
		return _elementsByContext[context].insert(element).second;
	}

	static bool remove(const TContext& context, TType* element)
	{
		auto it = _elementsByContext.find(context);

		if (it == _elementsByContext.end())
		{
			return false;
		}

		const bool result = it->second.erase(element) != 0;

		if (it->second.empty())
		{
			_elementsByContext.erase(it);
		}

		return result;
	}

public:
	[[nodiscard]] static const ElementSet& elements(const TContext& context)
	{
		static const ElementSet empty;

		auto it = _elementsByContext.find(context);

		if (it == _elementsByContext.end())
		{
			return empty;
		}

		return it->second;
	}
};

class Engine;
class Entity;

class Component :
	public ContextualizableTrait<Engine*>,
	public spk::ActivableTrait,
	public spk::NameTrait
{
private:
	using OwnerContextualizableTrait = ContextualizableTrait<Engine*>;

	Entity* _owner;
	OwnerContextualizableTrait::OnContextEditionContract _onOwnerContextEditionContract;

public:
	Component(const std::string& name = "Unnamed component", Entity* owner = nullptr) :
		spk::NameTrait(name)
	{
		activate();
		attach(owner);
	}
	Component(Entity* owner) :
		Component()
	{
		attach(owner);
	}

	void attach(Entity* owner)
	{
		_owner = owner;
		if (_owner != nullptr)
		{
			_onOwnerContextEditionContract = _owner->subscribeToContextEdition([&](Engine* const&, Engine* const&newContext){
				changeContext(newContext);
			});
		}
	}

	Entity* owner()
	{
		return _owner;
	}

	const Entity* owner() const
	{
		return _owner;
	}
};

#include <regex>

class Entity :
	public ContextualizableTrait<Engine*>,
	public Registry<Engine*, Entity>::Object,
	public spk::ActivableTrait,
	public spk::NameTrait
{
private:
	std::vector<std::unique_ptr<Component>> _components;

public:
	Entity(const std::string& name, Entity* owner = nullptr) :
		spk::NameTrait(name)
	{

	}

	template <typename TComponentType, typename ... TArgs>
	TComponentType& addComponent(TArgs ... args)
	{
		std::unique_ptr<TComponentType> newComponent = std::make_unique<TComponentType>(std::forward(args)...);
		
		TComponentType& result = *newComponent;

		result.attach(this);

		_components.push_back(std::move(newComponent));

		return result;
	}

	template <typename TComponentType>
	TComponentType* getComponent(const std::string& regex)
	{
		const std::regex expression(regex);

		for (auto& component : _components)
		{
			TComponentType* castedComponent = dynamic_cast<TComponentType*>(component.get());

			if (castedComponent != nullptr && std::regex_search(component->name(), expression))
			{
				return castedComponent;
			}
		}

		return nullptr;
	}

	template <typename TComponentType>
	std::vector<TComponentType*> getComponents(const std::string& regex)
	{
		std::vector<TComponentType*> result;

		const std::regex expression(regex);

		for (auto& component : _components)
		{
			TComponentType* castedComponent = dynamic_cast<TComponentType*>(component.get());

			if (castedComponent != nullptr && std::regex_search(component->name(), expression))
			{
				result.push_back(castedComponent);
			}
		}

		return result;
	}
};

class Entity2D;

class Component2D :
	public Component
{
private:
	using Component::attach;
	using Component::owner;

public:
	Component2D(const std::string& name): 
		Component2D(name)
	{
	}
	
	void attach(Entity2D* owner)
	{
		attach(owner);
	}
	
	Entity2D* owner() override
	{
		return dynamic_cast<Entity2D*>(owner());
	}
};

enum class ReferenceFrame
{
	Local,
	World
};

class Transform2D :
	public Component2D
{
	friend class Entity2D;
public:
	using OnEditionContractProvider = spk::ContractProvider<const Transform&>;
	using OnEditionCallback = OnEditionContractProvider::callback_type;
	using OnEditionContract = OnEditionContractProvider::Contract;
	
private:
	spk::Vector2 _position = {0, 0};
	spk::Vector2 _size = {1, 1};
	float _rotation = 0;
	
	spk::ContractProvider<const Transform&> _onEditionContractProvider;
	OnEditionContract _selfEditionContract;
	
	spk::CachedData<spk::Vector2> _worldPosition = [&](){
		//Same as _modelMatrix, but for the _position : need to add the position of the object but in world coordinates : must take into acount the rotation and all of the parent
	};
	spk::CachedData<spk::Vector2> _worldSize = [&](){
		//Same as _worldPosition
	};
	spk::CachedData<float> _worldRotation = [&](){
		//Same as _worldPosition
	};
	spk::CachedData<spk::Matrix4x4> _localModelMatrix = [&](){
		//Use _position, size and _rotation to compute the local matrix
	};
	spk::CachedData<spk::Matrix4x4> _modelMatrix = [&](){
		//Use _localModelMatrix added to the owner()->parent()->transform2D().modelMatrix.
		//Need to verify that owner()->parent() != nullptr, than that parent() is of type Entity2D (and than ask for its transform2D) or than that parent()->getComponent<Transform2D>
	};
	spk::CachedData<spk::Matrix4x4> _inverseModelMatrix = [&]()}{
		//Return the inverse of the _modelMatrix
	};
	
	void _clearWorldCaches()
	{
		_worldPosition.invalidate();
		_worldSize.invalidate();
		_worldRotation.invalidate();
		_modelMatrix.invalidate();
		_inverseModelMatrix.invalidate();
	}
	
	void _clearAllCaches()
	{
		_clearWorldCaches();
		_localModelMatrix();
	}

public:
	Transform2D(const std::string& name): 
		Component2D(name)
	{
		_selfEditionContract = subscribeToEdition([&](const Transform&){
			_clearAllCaches();
		});
	}
	
	OnEditionContract subscribeToEdition(OnEditionCallback callback)
	{
		return _onEditionContractProvider.subscribe(callback);
	}
	
	const spk::Vector2& position(ReferenceFrame referenceFrame = ReferenceFrame::Local)
	{
		switch(referenceFrame)
		{
			case ReferenceFrame::Local:
				return _position;
			default:
				return _worldPosition;
		}
	}
	
	const spk::Vector2& size(ReferenceFrame referenceFrame = ReferenceFrame::Local)
	{
		switch(referenceFrame)
		{
			case ReferenceFrame::Local:
				return _size;
			default:
				return _worldSize;
		}
	}
	
	float rotation(ReferenceFrame referenceFrame = ReferenceFrame::Local)
	{
		switch(referenceFrame)
		{
			case ReferenceFrame::Local:
				return _rotation;
			default:
				return _worldRotation;
		}
	}
	
	void place(spk::Vector2 position)
	{
		_position = position;
		_onEditionContractProvider.trigger(*this);
	}
	
	void move(spk::Vector2 delta)
	{
		_position += delta;
		_onEditionContractProvider.trigger(*this);
	}
	
	void resize(spk::Vector2 size)
	{
		_size = size;
		_onEditionContractProvider.trigger(*this);
	}
	
	void setRotation(float rotation)
	{
		_rotation = rotation;
		_onEditionContractProvider.trigger(*this);
	}
	
	void rotate(float rotation)
	{
		_rotation += rotation;
		_onEditionContractProvider.trigger(*this);
	}
	
	spk::matrix4x4 localModelMatrix() const
	{
		return _localModelMatrix;
	}
	
	spk::matrix4x4 modelMatrix() const
	{
		return _modelMatrix;
	}
	
	spk::matrix4x4 inverseModelMatrix() const
	{
		return _inverseModelMatrix;
	}
};

class Entity2D :
	public Entity,
	public Registry<Engine*, Entity2D>::Object
{
private:
	Transform2D& _transform;
	Transform2D::Contract _onParentTransformEditionContract;

	void _onParentChange(Entity2D* newParent)
	{
		_onParentTransformEditionContract.resign();
		
		if (newParent != nullptr)
		{
			_onParentTransformEditionContract = castedParent->transform().subscribeToEdition([&](){
				_transform._clearWorldCaches();
				//We should probably propagate the reset to all the child entity, as their own world data need to be invalidated too
			};
		}
	}

public:
	Entity2D(const std::string& name, Entity* owner = nullptr) :
		Entity(name, owner),
		_transform(addComponent<Transform2D>("Transform"))
	{
		_onParentChange(dynamic_cast<Entity2D*>(owner));
	}
	
	Transform2D& transform()
	{
		return _transform;
	}
	
	const Transform2D& transform() const
	{
		return _transform;
	}
};

class Engine
{
private:
	std::string _name;

public:
	explicit Engine(std::string name) :
		_name(std::move(name))
	{
	}

	[[nodiscard]] const std::string& name() const
	{
		return _name;
	}
};
