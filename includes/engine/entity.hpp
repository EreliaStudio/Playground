#pragma once

#include <sparkle.hpp>

#include <concepts>
#include <memory>
#include <string>
#include <utility>

#include "engine/behaviour_collection.hpp"
#include "engine/registry.hpp"
#include "engine/system_participant_collection.hpp"

namespace spk
{
	class Engine;

	class Entity : public ContextualizableTrait<Engine *>,
				   public Registry<Engine *, Entity>::Object,
				   public InherenceTrait<Entity>,
				   public ActivableTrait,
				   public NameTrait,
				   public SystemParticipantCollection,
				   public BehaviourCollection
	{
	public:
		using OnParticipantEditionContractProvider = spk::ContractProvider<Entity *, System::Participant &>;
		using OnParticipantEditionCallback = OnParticipantEditionContractProvider::callback_type;
		using OnParticipantEditionContract = OnParticipantEditionContractProvider::Contract;

		using OnBehaviourEditionContractProvider = spk::ContractProvider<Entity *, Behaviour &>;
		using OnBehaviourEditionCallback = OnBehaviourEditionContractProvider::callback_type;
		using OnBehaviourEditionContract = OnBehaviourEditionContractProvider::Contract;

	private:
		using SystemParticipantCollection::registerParticipant;
		using SystemParticipantCollection::unregisterParticipant;
		using BehaviourCollection::registerBehaviour;
		using BehaviourCollection::unregisterBehaviour;

		OnParticipantEditionContractProvider _onParticipantAdditionContractProvider;
		OnParticipantEditionContractProvider _onParticipantRemovalContractProvider;
		OnBehaviourEditionContractProvider _onBehaviourAdditionContractProvider;
		OnBehaviourEditionContractProvider _onBehaviourRemovalContractProvider;

	public:
		Entity(const std::string &name, Entity *parent = nullptr);

		OnParticipantEditionContract subscribeToParticipantAddition(OnParticipantEditionCallback callback)
		{
			return _onParticipantAdditionContractProvider.subscribe(callback);
		}

		OnParticipantEditionContract subscribeToParticipantRemoval(OnParticipantEditionCallback callback)
		{
			return _onParticipantRemovalContractProvider.subscribe(callback);
		}

		OnBehaviourEditionContract subscribeToBehaviourAddition(OnBehaviourEditionCallback callback)
		{
			return _onBehaviourAdditionContractProvider.subscribe(callback);
		}

		OnBehaviourEditionContract subscribeToBehaviourRemoval(OnBehaviourEditionCallback callback)
		{
			return _onBehaviourRemovalContractProvider.subscribe(callback);
		}

		template <typename TParticipantType, typename... TArgs>
			requires std::derived_from<TParticipantType, System::Participant>
		TParticipantType &addParticipant(TArgs &&...args)
		{
			std::unique_ptr<TParticipantType> participant =
				std::make_unique<TParticipantType>(std::forward<TArgs>(args)...);

			TParticipantType &result = *participant;
			result.attach(this);
			registerParticipant(std::move(participant));
			_onParticipantAdditionContractProvider.trigger(this, result);

			return result;
		}

		void removeParticipant(System::Participant &participant)
		{
			_onParticipantRemovalContractProvider.trigger(this, participant);
			unregisterParticipant(participant);
		}

		template <typename TBehaviourType, typename... TArgs>
			requires std::derived_from<TBehaviourType, Behaviour>
		TBehaviourType &addBehaviour(TArgs &&...args)
		{
			std::unique_ptr<TBehaviourType> behaviour =
				std::make_unique<TBehaviourType>(std::forward<TArgs>(args)...);

			TBehaviourType &result = *behaviour;
			result.attach(this);
			registerBehaviour(std::move(behaviour));
			_onBehaviourAdditionContractProvider.trigger(this, result);

			return result;
		}

		void removeBehaviour(Behaviour &behaviour)
		{
			_onBehaviourRemovalContractProvider.trigger(this, behaviour);
			unregisterBehaviour(behaviour);
		}
	};
}
